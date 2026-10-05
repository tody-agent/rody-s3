#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "display_policy.h"
#include "pin_catalog.h"
#include "wheel_cmd.h"

void setUp() {}
void tearDown() {}

// 1. Simulate battery monitor auto-detect & bypass logic
static bool simIsBatteryLow(float v, float lowThreshold) {
  // If no voltage divider is connected, pin reads near 0V (< 1.0V)
  // In that case, do not trigger false low-battery lockout
  if (v < 1.0f) {
    return false; // Bypass
  }
  return (v < lowThreshold);
}

void test_battery_bypass_when_disconnected() {
  // Disconnected / floating ADC reads 0.0V
  TEST_ASSERT_FALSE(simIsBatteryLow(0.0f, 3.4f));
  TEST_ASSERT_FALSE(simIsBatteryLow(0.4f, 3.4f));
}

void test_battery_check_when_connected() {
  // Connected battery below threshold
  TEST_ASSERT_TRUE(simIsBatteryLow(3.2f, 3.4f));
  TEST_ASSERT_TRUE(simIsBatteryLow(3.0f, 3.4f));

  // Connected battery healthy
  TEST_ASSERT_FALSE(simIsBatteryLow(3.7f, 3.4f));
  TEST_ASSERT_FALSE(simIsBatteryLow(4.15f, 3.4f));
}

// 2. Simulate Emotion string parser
enum class TestEmotion {
  IDLE, HAPPY, LISTENING, THINKING, SPEAKING, DRIVE_FWD, DRIVE_REV,
  TURN_LEFT, TURN_RIGHT, DIZZY, OBSTACLE, SLEEPY, UNKNOWN
};

static TestEmotion parseEmotion(const char* name) {
  if (strcmp(name, "idle") == 0) return TestEmotion::IDLE;
  if (strcmp(name, "happy") == 0) return TestEmotion::HAPPY;
  if (strcmp(name, "listen") == 0 || strcmp(name, "listening") == 0) return TestEmotion::LISTENING;
  if (strcmp(name, "think") == 0 || strcmp(name, "thinking") == 0) return TestEmotion::THINKING;
  if (strcmp(name, "speak") == 0 || strcmp(name, "speaking") == 0) return TestEmotion::SPEAKING;
  if (strcmp(name, "fwd") == 0 || strcmp(name, "forward") == 0) return TestEmotion::DRIVE_FWD;
  if (strcmp(name, "rev") == 0 || strcmp(name, "backward") == 0) return TestEmotion::DRIVE_REV;
  if (strcmp(name, "left") == 0) return TestEmotion::TURN_LEFT;
  if (strcmp(name, "right") == 0) return TestEmotion::TURN_RIGHT;
  if (strcmp(name, "dizzy") == 0) return TestEmotion::DIZZY;
  if (strcmp(name, "obstacle") == 0) return TestEmotion::OBSTACLE;
  if (strcmp(name, "sleep") == 0 || strcmp(name, "sleepy") == 0) return TestEmotion::SLEEPY;
  return TestEmotion::UNKNOWN;
}

void test_emotion_name_parser() {
  TEST_ASSERT_TRUE(parseEmotion("happy") == TestEmotion::HAPPY);
  TEST_ASSERT_TRUE(parseEmotion("listen") == TestEmotion::LISTENING);
  TEST_ASSERT_TRUE(parseEmotion("listening") == TestEmotion::LISTENING);
  TEST_ASSERT_TRUE(parseEmotion("think") == TestEmotion::THINKING);
  TEST_ASSERT_TRUE(parseEmotion("fwd") == TestEmotion::DRIVE_FWD);
  TEST_ASSERT_TRUE(parseEmotion("obstacle") == TestEmotion::OBSTACLE);
  TEST_ASSERT_TRUE(parseEmotion("sleepy") == TestEmotion::SLEEPY);
  TEST_ASSERT_TRUE(parseEmotion("unknown_xyz") == TestEmotion::UNKNOWN);
}

// 3. Simulate Voice command alias parser
enum class TestVoiceCommand {
  NONE, FORWARD, BACKWARD, LEFT, RIGHT, STOP, SPIN, HAPPY
};

static TestVoiceCommand parseVoiceCommand(const char* name) {
  if (strcmp(name, "forward") == 0 || strcmp(name, "fwd") == 0 || strcmp(name, "tien") == 0) {
    return TestVoiceCommand::FORWARD;
  }
  if (strcmp(name, "backward") == 0 || strcmp(name, "rev") == 0 || strcmp(name, "lui") == 0) {
    return TestVoiceCommand::BACKWARD;
  }
  if (strcmp(name, "left") == 0 || strcmp(name, "trai") == 0) {
    return TestVoiceCommand::LEFT;
  }
  if (strcmp(name, "right") == 0 || strcmp(name, "phai") == 0) {
    return TestVoiceCommand::RIGHT;
  }
  if (strcmp(name, "stop") == 0 || strcmp(name, "dung") == 0) {
    return TestVoiceCommand::STOP;
  }
  if (strcmp(name, "spin") == 0 || strcmp(name, "quay") == 0) {
    return TestVoiceCommand::SPIN;
  }
  if (strcmp(name, "happy") == 0 || strcmp(name, "vui") == 0 || strcmp(name, "rody") == 0 || strcmp(name, "chao") == 0 || strcmp(name, "otto") == 0) {
    return TestVoiceCommand::HAPPY;
  }
  return TestVoiceCommand::NONE;
}

void test_voice_command_vietnamese_aliases() {
  TEST_ASSERT_TRUE(parseVoiceCommand("tien") == TestVoiceCommand::FORWARD);
  TEST_ASSERT_TRUE(parseVoiceCommand("lui") == TestVoiceCommand::BACKWARD);
  TEST_ASSERT_TRUE(parseVoiceCommand("trai") == TestVoiceCommand::LEFT);
  TEST_ASSERT_TRUE(parseVoiceCommand("phai") == TestVoiceCommand::RIGHT);
  TEST_ASSERT_TRUE(parseVoiceCommand("dung") == TestVoiceCommand::STOP);
  TEST_ASSERT_TRUE(parseVoiceCommand("quay") == TestVoiceCommand::SPIN);
  TEST_ASSERT_TRUE(parseVoiceCommand("vui") == TestVoiceCommand::HAPPY);
  TEST_ASSERT_TRUE(parseVoiceCommand("rody") == TestVoiceCommand::HAPPY);
  TEST_ASSERT_TRUE(parseVoiceCommand("chao") == TestVoiceCommand::HAPPY);
}

void test_voice_command_english_aliases() {
  TEST_ASSERT_TRUE(parseVoiceCommand("forward") == TestVoiceCommand::FORWARD);
  TEST_ASSERT_TRUE(parseVoiceCommand("backward") == TestVoiceCommand::BACKWARD);
  TEST_ASSERT_TRUE(parseVoiceCommand("left") == TestVoiceCommand::LEFT);
  TEST_ASSERT_TRUE(parseVoiceCommand("right") == TestVoiceCommand::RIGHT);
  TEST_ASSERT_TRUE(parseVoiceCommand("stop") == TestVoiceCommand::STOP);
  TEST_ASSERT_TRUE(parseVoiceCommand("spin") == TestVoiceCommand::SPIN);
}

// 4. Test Audio RMS energy level normalization
static uint8_t calcAudioLevel(int64_t sumSquares, size_t samples) {
  int32_t rms = (int32_t)sqrtf((float)(sumSquares / (samples > 0 ? samples : 1)));
  int level = rms / 400;
  if (level > 100) level = 100;
  return (uint8_t)level;
}

void test_audio_level_calculation() {
  // Zero audio
  TEST_ASSERT_EQUAL_UINT8(0, calcAudioLevel(0, 256));

  // Moderate audio: RMS = 16000 -> level = 40%
  int64_t modSquares = (int64_t)16000 * 16000 * 256;
  TEST_ASSERT_EQUAL_UINT8(40, calcAudioLevel(modSquares, 256));

  // Loud audio: clamped to 100%
  int64_t loudSquares = (int64_t)60000 * 60000 * 256;
  TEST_ASSERT_EQUAL_UINT8(100, calcAudioLevel(loudSquares, 256));
}

// 5. Test IMU Emergency Stop / Tilt detection logic
struct SimMotionState {
  float pitch, roll, az, totalAccel;
  bool isBellyUp() const { return az < -0.45f; }
  bool isFallen() const { return !isBellyUp() && (fabsf(pitch) > 65.0f || fabsf(roll) > 65.0f); }
  bool isFreefall() const { return totalAccel < 0.25f; }
  bool isEmergencyStop() const { return isBellyUp() || isFallen() || isFreefall(); }
};

void test_imu_tilt_emergency_logic() {
  // Normal upright robot
  SimMotionState normal = { 0.0f, 0.0f, 1.0f, 1.0f };
  TEST_ASSERT_FALSE(normal.isEmergencyStop());
  TEST_ASSERT_FALSE(normal.isBellyUp());
  TEST_ASSERT_FALSE(normal.isFallen());

  // Tilted 70 degrees (fallen on side)
  SimMotionState fallen = { 10.0f, 72.0f, 0.3f, 1.0f };
  TEST_ASSERT_TRUE(fallen.isEmergencyStop());
  TEST_ASSERT_TRUE(fallen.isFallen());
  TEST_ASSERT_FALSE(fallen.isBellyUp());

  // Flipped upside down (belly up)
  SimMotionState bellyUp = { 0.0f, 0.0f, -0.9f, 1.0f };
  TEST_ASSERT_TRUE(bellyUp.isEmergencyStop());
  TEST_ASSERT_TRUE(bellyUp.isBellyUp());

  // Dropped / Freefall
  SimMotionState drop = { 0.0f, 0.0f, 0.1f, 0.15f };
  TEST_ASSERT_TRUE(drop.isEmergencyStop());
  TEST_ASSERT_TRUE(drop.isFreefall());
}

// 6. Test Emotion Animated Classification (Display power saving)
static bool simIsAnimatedEmotion(TestEmotion e) {
  return (e == TestEmotion::HAPPY ||
          e == TestEmotion::LISTENING ||
          e == TestEmotion::THINKING ||
          e == TestEmotion::SPEAKING ||
          e == TestEmotion::DRIVE_FWD ||
          e == TestEmotion::DIZZY ||
          e == TestEmotion::SLEEPY);
}

void test_emotion_animated_classification() {
  // Static emotions (skip SPI push when not blinking)
  TEST_ASSERT_FALSE(simIsAnimatedEmotion(TestEmotion::IDLE));
  TEST_ASSERT_FALSE(simIsAnimatedEmotion(TestEmotion::DRIVE_REV));
  TEST_ASSERT_FALSE(simIsAnimatedEmotion(TestEmotion::TURN_LEFT));
  TEST_ASSERT_FALSE(simIsAnimatedEmotion(TestEmotion::TURN_RIGHT));
  TEST_ASSERT_FALSE(simIsAnimatedEmotion(TestEmotion::OBSTACLE));

  // Animated emotions (continuous 30 FPS updates)
  TEST_ASSERT_TRUE(simIsAnimatedEmotion(TestEmotion::HAPPY));
  TEST_ASSERT_TRUE(simIsAnimatedEmotion(TestEmotion::THINKING));
  TEST_ASSERT_TRUE(simIsAnimatedEmotion(TestEmotion::SPEAKING));
  TEST_ASSERT_TRUE(simIsAnimatedEmotion(TestEmotion::SLEEPY));
  TEST_ASSERT_TRUE(simIsAnimatedEmotion(TestEmotion::DIZZY));
}

// 7. Test Display Configuration and Metrics Validation
enum class TestDisplayType : uint8_t {
  ST7789_154 = 0,
  GC9A01_128 = 1,
  ST7735_180 = 2,
  ST7735_096 = 3,
  SSD1306_096 = 4,
  SSD1306_096_SPI = 5
};

static const char* getTestDisplayTypeName(TestDisplayType dt) {
  switch (dt) {
    case TestDisplayType::GC9A01_128: return "1.28\" GC9A01 (240x240 Round)";
    case TestDisplayType::ST7735_180: return "1.8\" ST7735 (160x128)";
    case TestDisplayType::ST7735_096: return "0.96\" ST7735 (160x80)";
    case TestDisplayType::SSD1306_096: return "0.96\" SSD1306 (128x64 OLED I2C)";
    case TestDisplayType::SSD1306_096_SPI: return "0.96\" SSD1306 (128x64 OLED SPI)";
    case TestDisplayType::ST7789_154:
    default:
      return "1.54\" ST7789 (240x240)";
  }
}

static bool isValidDisplayId(int id) {
  return (id >= 0 && id <= 5);
}

struct TestDisplayMetrics {
  int width;
  int height;
  int eyeW;
  int eyeH;
  int eyeRadius;
  int leftEyeX;
  int rightEyeX;
  int centerY;
  bool isRound;
};

static TestDisplayMetrics getTestMetrics(TestDisplayType dt) {
  TestDisplayMetrics m;
  switch (dt) {
    case TestDisplayType::GC9A01_128:
      m.width = 240; m.height = 240;
      m.eyeW = 50; m.eyeH = 70; m.eyeRadius = 24;
      m.leftEyeX = 76; m.rightEyeX = 164; m.centerY = 120;
      m.isRound = true;
      break;
    case TestDisplayType::ST7735_180:
      m.width = 160; m.height = 128;
      m.eyeW = 36; m.eyeH = 50; m.eyeRadius = 18;
      m.leftEyeX = 48; m.rightEyeX = 112; m.centerY = 64;
      m.isRound = false;
      break;
    case TestDisplayType::ST7735_096:
      m.width = 160; m.height = 80;
      m.eyeW = 34; m.eyeH = 44; m.eyeRadius = 16;
      m.leftEyeX = 48; m.rightEyeX = 112; m.centerY = 40;
      m.isRound = false;
      break;
    case TestDisplayType::SSD1306_096:
    case TestDisplayType::SSD1306_096_SPI:
      m.width = 128; m.height = 64;
      m.eyeW = 28; m.eyeH = 36; m.eyeRadius = 12;
      m.leftEyeX = 38; m.rightEyeX = 90; m.centerY = 32;
      m.isRound = false;
      break;
    case TestDisplayType::ST7789_154:
    default:
      m.width = 240; m.height = 240;
      m.eyeW = 54; m.eyeH = 74; m.eyeRadius = 26;
      m.leftEyeX = 72; m.rightEyeX = 168; m.centerY = 120;
      m.isRound = false;
      break;
  }
  return m;
}

void test_display_type_validation_and_names() {
  TEST_ASSERT_TRUE(isValidDisplayId(0));
  TEST_ASSERT_TRUE(isValidDisplayId(1));
  TEST_ASSERT_TRUE(isValidDisplayId(2));
  TEST_ASSERT_TRUE(isValidDisplayId(3));
  TEST_ASSERT_TRUE(isValidDisplayId(4));
  TEST_ASSERT_TRUE(isValidDisplayId(5));
  TEST_ASSERT_FALSE(isValidDisplayId(-1));
  TEST_ASSERT_FALSE(isValidDisplayId(6));
  TEST_ASSERT_FALSE(isValidDisplayId(99));

  TEST_ASSERT_NOT_NULL(strstr(getTestDisplayTypeName(TestDisplayType::ST7789_154), "1.54"));
  TEST_ASSERT_NOT_NULL(strstr(getTestDisplayTypeName(TestDisplayType::GC9A01_128), "1.28"));
  TEST_ASSERT_NOT_NULL(strstr(getTestDisplayTypeName(TestDisplayType::ST7735_180), "1.8"));
  TEST_ASSERT_NOT_NULL(strstr(getTestDisplayTypeName(TestDisplayType::ST7735_096), "0.96"));
  TEST_ASSERT_NOT_NULL(strstr(getTestDisplayTypeName(TestDisplayType::SSD1306_096), "SSD1306"));
  TEST_ASSERT_NOT_NULL(strstr(getTestDisplayTypeName(TestDisplayType::SSD1306_096_SPI), "OLED SPI"));
}

void test_display_metrics_bounds_and_round_safety() {
  TestDisplayType types[] = {
    TestDisplayType::ST7789_154,
    TestDisplayType::GC9A01_128,
    TestDisplayType::ST7735_180,
    TestDisplayType::ST7735_096,
    TestDisplayType::SSD1306_096,
    TestDisplayType::SSD1306_096_SPI
  };

  for (auto t : types) {
    TestDisplayMetrics m = getTestMetrics(t);
    // Eye bounds must be inside display resolution
    TEST_ASSERT_TRUE(m.leftEyeX - m.eyeW / 2 >= 0);
    TEST_ASSERT_TRUE(m.rightEyeX + m.eyeW / 2 <= m.width);
    TEST_ASSERT_TRUE(m.centerY - m.eyeH / 2 >= 0);
    TEST_ASSERT_TRUE(m.centerY + m.eyeH / 2 <= m.height);
    // Symmetry check
    int leftOffset = m.leftEyeX;
    int rightOffset = m.width - m.rightEyeX;
    TEST_ASSERT_EQUAL_INT(leftOffset, rightOffset);
  }

  // GC9A01 circular bezel safety:
  // Center is (120, 120), radius R = 120.
  // The furthest corner of the right eye is at (rightEyeX + eyeW/2, centerY - eyeH/2)
  TestDisplayMetrics roundM = getTestMetrics(TestDisplayType::GC9A01_128);
  float dx = (roundM.rightEyeX + roundM.eyeW / 2) - 120.0f; // 164 + 25 - 120 = 69
  float dy = (roundM.centerY - roundM.eyeH / 2) - 120.0f;   // 120 - 35 - 120 = -35
  float distFromCenter = sqrtf(dx * dx + dy * dy);         // sqrt(4761 + 1225) = sqrt(5986) = ~77.37px
  // Dist must be well within round bezel safe radius of 120px
  TEST_ASSERT_TRUE(distFromCenter < 115.0f);
}

void test_saved_display_type_survives_oled_detect() {
  using display_policy::Type;
  // NVS already has a choice. A later I2C ACK must not replace it.
  TEST_ASSERT_EQUAL_INT((int)Type::ST7735_096,
                        (int)display_policy::resolveType(3, true));
  TEST_ASSERT_EQUAL_INT((int)Type::SSD1306_096_SPI,
                        (int)display_policy::resolveType(5, true));
  TEST_ASSERT_EQUAL_INT((int)Type::SSD1306_096,
                        (int)display_policy::resolveType(4, false));
  // First boot only: no saved id.
  TEST_ASSERT_EQUAL_INT((int)Type::SSD1306_096,
                        (int)display_policy::resolveType(255, true));
  TEST_ASSERT_EQUAL_INT((int)Type::ST7735_096,
                        (int)display_policy::resolveType(255, false));
}

void test_oled_probe_rejects_address_only_ack() {
  TEST_ASSERT_FALSE(display_policy::acceptOledProbe(true, false, false));
  TEST_ASSERT_FALSE(display_policy::acceptOledProbe(true, true, true));
  TEST_ASSERT_TRUE(display_policy::acceptOledProbe(true, true, false));
  TEST_ASSERT_FALSE(display_policy::acceptOledProbe(false, true, false));
}

void test_oled_silk_pins_are_tried_before_swap() {
  display_policy::PinPair first = display_policy::oledPinCandidate(0);
  TEST_ASSERT_EQUAL_INT(41, first.sda);
  TEST_ASSERT_EQUAL_INT(42, first.scl);
  display_policy::PinPair swapped = display_policy::oledPinCandidate(1);
  TEST_ASSERT_EQUAL_INT(42, swapped.sda);
  TEST_ASSERT_EQUAL_INT(41, swapped.scl);
}

void test_oled_init_commands_are_separate_i2c_frames() {
  display_policy::I2cFrame frames[32];
  size_t n = display_policy::buildOledFrames(display_policy::OledSeq::BringUp, frames, 32);
  TEST_ASSERT_TRUE(n >= 8);

  int frame_ae = -1;
  int frame_af = -1;
  int frame_charge = -1;
  bool saw_a5 = false;
  for (size_t i = 0; i < n; i++) {
    TEST_ASSERT_TRUE(frames[i].len >= 2);
    TEST_ASSERT_EQUAL_UINT8(0x00, frames[i].data[0]);
    if (frames[i].len == 2 && frames[i].data[1] == 0xAE) frame_ae = (int)i;
    if (frames[i].len == 2 && frames[i].data[1] == 0xAF) frame_af = (int)i;
    if (frames[i].len == 3 && frames[i].data[1] == 0x8D && frames[i].data[2] == 0x14) {
      frame_charge = (int)i;
    }
    for (uint8_t b = 1; b < frames[i].len; b++) {
      if (frames[i].data[b] == 0xA5) saw_a5 = true;
    }
  }
  TEST_ASSERT_TRUE(frame_ae >= 0);
  TEST_ASSERT_TRUE(frame_af > frame_ae);
  TEST_ASSERT_TRUE(frame_charge >= 0);
  TEST_ASSERT_FALSE(saw_a5);

  display_policy::I2cFrame on[4];
  size_t n_on = display_policy::buildOledFrames(display_policy::OledSeq::AllPixelsOn, on, 4);
  TEST_ASSERT_EQUAL_UINT(1, n_on);
  TEST_ASSERT_EQUAL_UINT8(0x00, on[0].data[0]);
  TEST_ASSERT_EQUAL_UINT8(0xA5, on[0].data[1]);
}

void test_resolve_choice_reports_source() {
  using display_policy::Source;
  using display_policy::Type;
  display_policy::Choice saved = display_policy::resolveChoice(0, true);
  TEST_ASSERT_EQUAL_INT((int)Type::ST7789_154, (int)saved.type);
  TEST_ASSERT_EQUAL_INT((int)Source::Saved, (int)saved.source);

  display_policy::Choice spi = display_policy::resolveChoice(5, true);
  TEST_ASSERT_EQUAL_INT((int)Type::SSD1306_096_SPI, (int)spi.type);
  TEST_ASSERT_EQUAL_INT((int)Source::Saved, (int)spi.source);

  display_policy::Choice autoOled = display_policy::resolveChoice(255, true);
  TEST_ASSERT_EQUAL_INT((int)Type::SSD1306_096, (int)autoOled.type);
  TEST_ASSERT_EQUAL_INT((int)Source::AutoOled, (int)autoOled.source);

  display_policy::Choice autoDefault = display_policy::resolveChoice(255, false);
  TEST_ASSERT_EQUAL_INT((int)Type::ST7735_096, (int)autoDefault.type);
  TEST_ASSERT_EQUAL_INT((int)Source::AutoDefault, (int)autoDefault.source);
}

void test_stale_i2c_selection_uses_spi_oled_when_header_is_live() {
  using display_policy::Source;
  using display_policy::Type;
  // Bench: saved I2C id, command probe rejected, SPI header pins pulled up.
  display_policy::Choice fixed = display_policy::resolveChoice(4, false, true);
  TEST_ASSERT_EQUAL_INT((int)Type::SSD1306_096_SPI, (int)fixed.type);
  TEST_ASSERT_EQUAL_INT((int)Source::AutoSpi, (int)fixed.source);

  TEST_ASSERT_EQUAL_INT((int)Type::SSD1306_096,
                        (int)display_policy::resolveType(4, true, true));
  TEST_ASSERT_EQUAL_INT((int)Type::ST7735_096,
                        (int)display_policy::resolveType(3, false, true));
  TEST_ASSERT_EQUAL_INT((int)Type::SSD1306_096,
                        (int)display_policy::resolveType(4, false, false));
  TEST_ASSERT_EQUAL_INT((int)Type::SSD1306_096,
                        (int)display_policy::resolveType(4, false));
}

void test_spi_header_needs_four_pulled_up_pins() {
  TEST_ASSERT_TRUE(display_policy::spiHeaderPresent(true, true, true, true, true));
  TEST_ASSERT_TRUE(display_policy::spiHeaderPresent(true, true, true, true, false));
  TEST_ASSERT_FALSE(display_policy::spiHeaderPresent(true, true, false, false, false));
  TEST_ASSERT_FALSE(display_policy::spiHeaderPresent(false, false, false, false, false));
}

void test_display_id_rejects_out_of_range() {
  TEST_ASSERT_TRUE(display_policy::isValidId(0));
  TEST_ASSERT_TRUE(display_policy::isValidId(5));
  TEST_ASSERT_FALSE(display_policy::isValidId(-1));
  TEST_ASSERT_FALSE(display_policy::isValidId(6));
}

void test_pin_catalog_matches_wiring_and_skips_forbidden_outputs() {
  size_t n = 0;
  const pin_catalog::Entry* rows = pin_catalog::entries(&n);
  TEST_ASSERT_TRUE(n >= 16);

  const pin_catalog::Entry* irL = pin_catalog::find(pins::IR_L);
  const pin_catalog::Entry* irR = pin_catalog::find(pins::IR_R);
  TEST_ASSERT_NOT_NULL(irL);
  TEST_ASSERT_NOT_NULL(irR);
  TEST_ASSERT_EQUAL_INT(10, irL->gpio);
  TEST_ASSERT_EQUAL_INT(11, irR->gpio);
  TEST_ASSERT_EQUAL_INT((int)pin_catalog::Bus::GpioIn, (int)irL->bus);
  TEST_ASSERT_EQUAL_INT((int)pin_catalog::Bus::GpioIn, (int)irR->bus);

  const pin_catalog::Entry* boot = pin_catalog::find(0);
  TEST_ASSERT_NOT_NULL(boot);
  TEST_ASSERT_EQUAL_INT((int)pin_catalog::Bus::GpioIn, (int)boot->bus);

  const int forbidden[] = {3, 19, 20, 43, 44, 45, 46};
  for (int gpio : forbidden) {
    TEST_ASSERT_NULL(pin_catalog::find(gpio));
  }
  for (int gpio = 26; gpio <= 37; gpio++) {
    TEST_ASSERT_NULL(pin_catalog::find(gpio));
  }
  for (size_t i = 0; i < n; i++) {
    TEST_ASSERT_TRUE(rows[i].name != nullptr && rows[i].name[0] != '\0');
  }
}

void test_oled_header_scan_skips_i2s_pins() {
  const int i2s[] = {
    pins::MIC_SCK, pins::MIC_WS, pins::MIC_SD,
    pins::SPK_DIN, pins::SPK_LRC, pins::SPK_BCLK
  };
  bool sawI2c = false;
  bool sawTft = false;
  bool sawIr = false;
  bool sawUs = false;
  for (int i = 0; i < display_policy::kOledHeaderCandidateCount; i++) {
    display_policy::PinPair pair = display_policy::oledHeaderCandidate(i);
    for (int banned : i2s) {
      TEST_ASSERT_FALSE(pair.sda == banned || pair.scl == banned);
    }
    if (pair.sda == pins::I2C_SDA && pair.scl == pins::I2C_SCL) sawI2c = true;
    if (pair.sda == pins::TFT_MOSI && pair.scl == pins::TFT_SCLK) sawTft = true;
    if (pair.sda == pins::IR_L && pair.scl == pins::IR_R) sawIr = true;
    if (pair.sda == pins::US_TRIG && pair.scl == pins::US_ECHO) sawUs = true;
  }
  TEST_ASSERT_TRUE(sawI2c);
  TEST_ASSERT_TRUE(sawTft);
  TEST_ASSERT_TRUE(sawIr);
  TEST_ASSERT_TRUE(sawUs);
}

void test_wheel_command_accepts_page_and_serial_names() {
  wheel_cmd::Side side = wheel_cmd::Side::Both;
  TEST_ASSERT_TRUE(wheel_cmd::parse("left", &side));
  TEST_ASSERT_EQUAL_INT((int)wheel_cmd::Side::Left, (int)side);
  TEST_ASSERT_TRUE(wheel_cmd::parse("L", &side));
  TEST_ASSERT_EQUAL_INT((int)wheel_cmd::Side::Left, (int)side);
  TEST_ASSERT_TRUE(wheel_cmd::parse("right", &side));
  TEST_ASSERT_EQUAL_INT((int)wheel_cmd::Side::Right, (int)side);
  TEST_ASSERT_TRUE(wheel_cmd::parse("R", &side));
  TEST_ASSERT_EQUAL_INT((int)wheel_cmd::Side::Right, (int)side);
  TEST_ASSERT_TRUE(wheel_cmd::parse("both", &side));
  TEST_ASSERT_EQUAL_INT((int)wheel_cmd::Side::Both, (int)side);
  TEST_ASSERT_TRUE(wheel_cmd::parse("ALL", &side));
  TEST_ASSERT_EQUAL_INT((int)wheel_cmd::Side::Both, (int)side);
  TEST_ASSERT_FALSE(wheel_cmd::parse("nope", &side));
  TEST_ASSERT_FALSE(wheel_cmd::parse(nullptr, &side));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_battery_bypass_when_disconnected);
  RUN_TEST(test_battery_check_when_connected);
  RUN_TEST(test_emotion_name_parser);
  RUN_TEST(test_voice_command_vietnamese_aliases);
  RUN_TEST(test_voice_command_english_aliases);
  RUN_TEST(test_audio_level_calculation);
  RUN_TEST(test_imu_tilt_emergency_logic);
  RUN_TEST(test_emotion_animated_classification);
  RUN_TEST(test_display_type_validation_and_names);
  RUN_TEST(test_display_metrics_bounds_and_round_safety);
  RUN_TEST(test_saved_display_type_survives_oled_detect);
  RUN_TEST(test_oled_probe_rejects_address_only_ack);
  RUN_TEST(test_oled_silk_pins_are_tried_before_swap);
  RUN_TEST(test_oled_init_commands_are_separate_i2c_frames);
  RUN_TEST(test_resolve_choice_reports_source);
  RUN_TEST(test_stale_i2c_selection_uses_spi_oled_when_header_is_live);
  RUN_TEST(test_spi_header_needs_four_pulled_up_pins);
  RUN_TEST(test_display_id_rejects_out_of_range);
  RUN_TEST(test_pin_catalog_matches_wiring_and_skips_forbidden_outputs);
  RUN_TEST(test_oled_header_scan_skips_i2s_pins);
  RUN_TEST(test_wheel_command_accepts_page_and_serial_names);
  return UNITY_END();
}

