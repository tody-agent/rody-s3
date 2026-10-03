#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

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

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_battery_bypass_when_disconnected);
  RUN_TEST(test_battery_check_when_connected);
  RUN_TEST(test_emotion_name_parser);
  RUN_TEST(test_voice_command_vietnamese_aliases);
  RUN_TEST(test_voice_command_english_aliases);
  RUN_TEST(test_audio_level_calculation);
  return UNITY_END();
}
