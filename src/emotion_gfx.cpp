#include "emotion_gfx.h"
#include "store.h"
#include "oled_diag.h"
#include "../include/debug_log.h"
#include "../include/display_policy.h"
#include "../include/pins.h"
#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <math.h>

namespace emotion_gfx {

struct DisplayMetrics {
  int width;
  int height;
  int eyeW;
  int eyeH;
  int eyeRadius;
  int leftEyeX;
  int rightEyeX;
  int centerY;
  int sparkleR1;
  int sparkleR2;
  bool isRound;
};

static DisplayMetrics getMetricsForType(store::DisplayType dt) {
  DisplayMetrics m;
  switch (dt) {
    case store::DisplayType::GC9A01_128:
      m.width = 240; m.height = 240;
      m.eyeW = 50; m.eyeH = 70; m.eyeRadius = 24;
      m.leftEyeX = 76; m.rightEyeX = 164; m.centerY = 120;
      m.sparkleR1 = 6; m.sparkleR2 = 3;
      m.isRound = true;
      break;

    case store::DisplayType::ST7735_180: // 1.8" 160x128 landscape
      m.width = 160; m.height = 128;
      m.eyeW = 36; m.eyeH = 50; m.eyeRadius = 18;
      m.leftEyeX = 48; m.rightEyeX = 112; m.centerY = 64;
      m.sparkleR1 = 4; m.sparkleR2 = 2;
      m.isRound = false;
      break;

    case store::DisplayType::ST7735_096: // 0.96" 160x80 IPS landscape
      m.width = 160; m.height = 80;
      m.eyeW = 34; m.eyeH = 44; m.eyeRadius = 16;
      m.leftEyeX = 48; m.rightEyeX = 112; m.centerY = 40;
      m.sparkleR1 = 3; m.sparkleR2 = 2;
      m.isRound = false;
      break;

    case store::DisplayType::SSD1306_096:
    case store::DisplayType::SSD1306_096_SPI: // 0.96" 128x64 OLED (I2C or SPI)
      m.width = 128; m.height = 64;
      m.eyeW = 28; m.eyeH = 36; m.eyeRadius = 12;
      m.leftEyeX = 38; m.rightEyeX = 90; m.centerY = 32;
      m.sparkleR1 = 3; m.sparkleR2 = 1;
      m.isRound = false;
      break;


    case store::DisplayType::ST7789_154:
    default:
      m.width = 240; m.height = 240;
      m.eyeW = 54; m.eyeH = 74; m.eyeRadius = 26;
      m.leftEyeX = 72; m.rightEyeX = 168; m.centerY = 120;
      m.sparkleR1 = 6; m.sparkleR2 = 3;
      m.isRound = false;
      break;
  }
  return m;
}

class Panel_RodySSD1306Spi : public lgfx::Panel_SSD1306 {
public:
  bool init(bool use_reset) override {
    // SPI OLED has no MISO. Panel_1bitOLED::init skips every command when the
    // status read fails, and the stock list also sets contrast to 0.
    if (!Panel_HasBuffer::init(use_reset)) {
      return false;
    }
    size_t count = 0;
    const display_policy::Cmd* cmds = display_policy::bringUpCommands(&count);
    startWrite();
    for (size_t i = 0; i < count; i++) {
      uint32_t word = cmds[i].b[0];
      uint32_t bits = 8;
      if (cmds[i].n > 1) {
        word |= (uint32_t)cmds[i].b[1] << 8;
        bits = 16;
      }
      _bus->writeCommand(word, bits);
    }
    endWrite();
    setInvert(_invert);
    setRotation(_rotation);
    return true;
  }
};

class Panel_RodySSD1306 : public lgfx::Panel_SSD1306 {
public:
  bool init(bool use_reset) override {
    if (!Panel_HasBuffer::init(use_reset)) {
      return false;
    }
    display_policy::I2cFrame frames[24];
    size_t n = display_policy::buildOledFrames(display_policy::OledSeq::BringUp, frames, 24);
    for (size_t i = 0; i < n; i++) {
      beginTransaction();
      _bus->writeBytes(frames[i].data + 1, frames[i].len - 1, false, true);
      endTransaction();
    }
    setInvert(_invert);
    setRotation(_rotation);
    return true;
  }
};

class LGFX_Rody : public lgfx::LGFX_Device {
  lgfx::Bus_SPI _bus_spi;
  lgfx::Bus_I2C _bus_i2c;
  lgfx::Light_PWM _light_instance;
  lgfx::Panel_Device* _panel = nullptr;

public:
  LGFX_Rody() {}
  ~LGFX_Rody() {
    if (_panel) delete _panel;
  }

  bool initPanel(store::DisplayType dt) {
    if (_panel) {
      delete _panel;
      _panel = nullptr;
    }

    if (dt == store::DisplayType::SSD1306_096) {
      // 0.96" I2C OLED (SSD1306 / SH1106)
      auto cfg = _bus_i2c.config();
      int sdaPin = store::getOledSdaPin();
      int sclPin = store::getOledSclPin();
      cfg.i2c_port = 1; // Dedicated I2C_NUM_1 to prevent IDF conflict with Arduino Wire on port 0!
      cfg.pin_sda = sdaPin;
      cfg.pin_scl = sclPin;
      cfg.i2c_addr = store::getOledAddr();
      cfg.freq_write = 100000; // Standard 100kHz for universal OLED clone compatibility
      cfg.freq_read  = 0;
      cfg.prefix_len = 1;
      cfg.prefix_cmd = 0x00;
      cfg.prefix_data = 0x40;
      _bus_i2c.config(cfg);

      auto p = new Panel_RodySSD1306();
      p->setBus(&_bus_i2c);
      auto p_cfg = p->config();
      p_cfg.panel_width = 128;
      p_cfg.panel_height = 64;
      p_cfg.offset_x = 0;
      p_cfg.offset_y = 0;
      p_cfg.bus_shared = false;
      p->config(p_cfg);
      _panel = p;
      setPanel(_panel);
      return true;
    }

    // SPI Bus Configuration (Standard 4-wire SPI with separate D/C pin)
    auto bus_cfg = _bus_spi.config();
    bus_cfg.spi_host = SPI2_HOST;
    bus_cfg.spi_mode = 0;
    bus_cfg.freq_write = (dt == store::DisplayType::SSD1306_096_SPI) ? 4000000 :
                         ((dt == store::DisplayType::ST7735_180 || dt == store::DisplayType::ST7735_096) ? 20000000 : 40000000);
    bus_cfg.freq_read = 16000000;
    bus_cfg.spi_3wire = false; // Standard 4-wire SPI with separate DC pin
    bus_cfg.dma_channel = SPI_DMA_CH_AUTO;
    bus_cfg.pin_sclk = pins::TFT_SCLK; // 42
    bus_cfg.pin_mosi = pins::TFT_MOSI; // 41
    bus_cfg.pin_miso = -1;
    bus_cfg.pin_dc   = pins::TFT_DC;   // 40
    _bus_spi.config(bus_cfg);

    // Backlight PWM (1000 Hz standard PWM frequency)
    auto light_cfg = _light_instance.config();
    light_cfg.pin_bl = pins::TFT_BLK; // 21
    light_cfg.invert = false;
    light_cfg.freq = 1000;
    light_cfg.pwm_channel = 1;
    _light_instance.config(light_cfg);

    if (dt == store::DisplayType::GC9A01_128) {
      // 1.28" Round GC9A01 (240x240)
      auto p = new lgfx::Panel_GC9A01();
      p->setBus(&_bus_spi);
      p->setLight(&_light_instance);
      auto p_cfg = p->config();
      p_cfg.pin_cs = pins::TFT_CS;   // 38
      p_cfg.pin_rst = pins::TFT_RST; // 39
      p_cfg.pin_busy = -1;
      p_cfg.panel_width = 240;
      p_cfg.panel_height = 240;
      p_cfg.offset_x = 0;
      p_cfg.offset_y = 0;
      p_cfg.offset_rotation = 0;
      p_cfg.readable = false;
      p_cfg.invert = true;
      p_cfg.rgb_order = false;
      p->config(p_cfg);
      _panel = p;
    }
    else if (dt == store::DisplayType::ST7735_180) {
      // 1.8" ST7735S (160x128 Landscape)
      auto p = new lgfx::Panel_ST7735S();
      p->setBus(&_bus_spi);
      p->setLight(&_light_instance);
      auto p_cfg = p->config();
      p_cfg.pin_cs = pins::TFT_CS;
      p_cfg.pin_rst = pins::TFT_RST;
      p_cfg.pin_busy = -1;
      p_cfg.panel_width = 160;
      p_cfg.panel_height = 128;
      p_cfg.offset_x = 0;
      p_cfg.offset_y = 0;
      p_cfg.offset_rotation = 1; // Landscape
      p_cfg.readable = false;
      p_cfg.invert = false;
      p_cfg.rgb_order = false;
      p->config(p_cfg);
      _panel = p;
    }
    else if (dt == store::DisplayType::ST7735_096) {
      // 0.96" ST7735S (160x80 IPS Landscape via setRotation(1))
      auto p = new lgfx::Panel_ST7735S();
      p->setBus(&_bus_spi);
      p->setLight(&_light_instance);
      auto p_cfg = p->config();
      p_cfg.pin_cs = pins::TFT_CS;
      p_cfg.pin_rst = pins::TFT_RST;
      p_cfg.pin_busy = -1;
      p_cfg.panel_width = 80;
      p_cfg.panel_height = 160;
      p_cfg.offset_x = 26;
      p_cfg.offset_y = 1;
      p_cfg.offset_rotation = 0;
      p_cfg.readable = false;
      p_cfg.invert = true;
      p_cfg.rgb_order = false;
      p->config(p_cfg);
      _panel = p;
    }
    else if (dt == store::DisplayType::SSD1306_096_SPI) {
      // 0.96" SSD1306 OLED SPI (128x64 7-pin). Write-only: no status read.
      auto p = new Panel_RodySSD1306Spi();
      p->setBus(&_bus_spi);
      auto p_cfg = p->config();
      p_cfg.pin_cs = pins::TFT_CS;
      p_cfg.pin_rst = pins::TFT_RST;
      p_cfg.panel_width = 128;
      p_cfg.panel_height = 64;
      p_cfg.offset_x = 0;
      p_cfg.offset_y = 0;
      p_cfg.bus_shared = false;
      p_cfg.readable = false;
      p->config(p_cfg);
      _panel = p;
    }
    else {
      // 1.54" ST7789 (240x240 Square - Default)
      auto p = new lgfx::Panel_ST7789();
      p->setBus(&_bus_spi);
      p->setLight(&_light_instance);
      auto p_cfg = p->config();
      p_cfg.pin_cs = pins::TFT_CS;
      p_cfg.pin_rst = pins::TFT_RST;
      p_cfg.pin_busy = -1;
      p_cfg.panel_width = 240;
      p_cfg.panel_height = 240;
      p_cfg.offset_x = 0;
      p_cfg.offset_y = 0;
      p_cfg.offset_rotation = 0;
      p_cfg.readable = false;
      p_cfg.invert = true;
      p_cfg.rgb_order = false;
      p->config(p_cfg);
      _panel = p;
    }

    setPanel(_panel);
    return true;
  }
};

static LGFX_Rody tft;
static LGFX_Sprite canvas(&tft);
static bool tftInitialized = false;

static store::DisplayType activeType = store::DisplayType::ST7789_154;
static DisplayMetrics metrics = getMetricsForType(store::DisplayType::ST7789_154);

static Emotion currentEmotion = Emotion::IDLE;
static uint32_t lastAnimTime = 0;
static uint32_t nextBlinkTime = 0;
static bool isBlinking = false;
static float blinkProgress = 0.0f; // 0.0 (open) to 1.0 (closed)
static int pupilOffsetX = 0;
static int pupilOffsetY = 0;
static uint32_t animFrame = 0;
static bool dirty = true;

static inline bool isAnimatedEmotion(Emotion e) {
  return (e == Emotion::HAPPY ||
          e == Emotion::LISTENING ||
          e == Emotion::THINKING ||
          e == Emotion::SPEAKING ||
          e == Emotion::DRIVE_FWD ||
          e == Emotion::DIZZY ||
          e == Emotion::SLEEPY);
}

static void renderFace();

void init() {
  activeType = store::getDisplayType();
  metrics = getMetricsForType(activeType);

  // Backlight high even for an I2C selection, so an IPS on this header can glow.
  pinMode(pins::TFT_BLK, OUTPUT);
  digitalWrite(pins::TFT_BLK, HIGH);

  // SPI panels get a reset pulse. CS stays high until the driver selects it.
  if (activeType != store::DisplayType::SSD1306_096) {
    pinMode(pins::TFT_CS, OUTPUT);
    digitalWrite(pins::TFT_CS, HIGH);

    pinMode(pins::TFT_RST, OUTPUT);
    digitalWrite(pins::TFT_RST, HIGH);
    delay(5);
    digitalWrite(pins::TFT_RST, LOW);
    delay(20);
    digitalWrite(pins::TFT_RST, HIGH);
    delay(50);
  }

  if (activeType == store::DisplayType::SSD1306_096) {
    // Hardware wake up & force light up via Wire1 BEFORE LovyanGFX driver is installed
    oled_diag::forceHardwareLightUp(store::getOledSdaPin(), store::getOledSclPin(), store::getOledAddr());
  }

  tft.initPanel(activeType);
  tft.init();

  // 2. Crucial display rotation:
  if (activeType == store::DisplayType::ST7735_096) {
    tft.setRotation(1); // Rotates 80x160 into 160x80 Landscape!
  } else if (activeType == store::DisplayType::ST7735_180) {
    tft.setRotation(1);
  } else {
    tft.setRotation(0);
  }

  // 3. Keep backlight pin HIGH and force contrast
  pinMode(pins::TFT_BLK, OUTPUT);
  digitalWrite(pins::TFT_BLK, HIGH);
  tft.setBrightness(255); // Maximum contrast / brightness for both TFT and OLED!

  // 4. Hardware light-up flash sequence (directly to screen memory!)
  if (activeType == store::DisplayType::SSD1306_096 || activeType == store::DisplayType::SSD1306_096_SPI) {
    tft.fillScreen(TFT_WHITE); // Flash full white on OLED!
    delay(200);
    tft.fillScreen(TFT_BLACK);
  } else {
    tft.fillScreen(TFT_WHITE); // Flash full white!
    delay(150);
    tft.fillScreen(TFT_RED);
    delay(120);
    tft.fillScreen(TFT_GREEN);
    delay(120);
    tft.fillScreen(TFT_BLUE);
    delay(120);
    tft.fillScreen(TFT_BLACK);
  }

  // 5. Allocate double buffer canvas sprite matching display resolution
  canvas.setColorDepth((activeType == store::DisplayType::SSD1306_096 || activeType == store::DisplayType::SSD1306_096_SPI) ? 1 : 16);
  if (!canvas.createSprite(metrics.width, metrics.height)) {
    Serial.printf("# [gfx] Failed to create %dx%d sprite, fallback direct draw\n", metrics.width, metrics.height);
    tftInitialized = false;
  } else {
    tftInitialized = true;
    Serial.printf("# [gfx] %s (%dx%d) initialized with double buffer\n",
                  store::getDisplayTypeName(activeType), metrics.width, metrics.height);
  }

  char gfxLine[96];
  snprintf(gfxLine, sizeof(gfxLine), "GFX %s %dx%d %s",
           store::getDisplayTypeName(activeType), metrics.width, metrics.height,
           tftInitialized ? "OK" : "FAIL");
  debug_log::push(gfxLine);

  // 6. Draw immediate visual splash screen
  if (tftInitialized) {
    if (activeType == store::DisplayType::SSD1306_096 || activeType == store::DisplayType::SSD1306_096_SPI) {
      canvas.fillScreen(TFT_BLACK);
      canvas.drawRect(0, 0, metrics.width, metrics.height, TFT_WHITE);
      canvas.drawRect(2, 2, metrics.width - 4, metrics.height - 4, TFT_WHITE);
      canvas.setTextColor(TFT_WHITE);
      canvas.setTextDatum(textdatum_t::middle_center);
      canvas.drawString("RODY 0.96 OLED", metrics.width / 2, 22);
      canvas.drawString("READY!", metrics.width / 2, 44);
      canvas.pushSprite(0, 0);
    } else {
      canvas.fillScreen(canvas.color565(0x02, 0x84, 0xc7)); // Vibrant Cyan Blue
      canvas.drawRect(0, 0, metrics.width, metrics.height, TFT_WHITE);
      canvas.setTextColor(TFT_WHITE);
      canvas.setTextDatum(textdatum_t::middle_center);
      canvas.drawString("RODY 0.96 IPS", metrics.width / 2, metrics.height / 2 - 12);
      canvas.drawString("INIT OK!", metrics.width / 2, metrics.height / 2 + 12);
      canvas.pushSprite(0, 0);
    }
    delay(300);
  }

  nextBlinkTime = millis() + 2000;
  dirty = true;
  renderFace();
}

void testDisplayPattern() {
  Serial.printf("# [gfx] Running live visual test on %s (%dx%d)...\n",
                store::getDisplayTypeName(activeType), metrics.width, metrics.height);

  // Test backlight toggle
  pinMode(pins::TFT_BLK, OUTPUT);
  digitalWrite(pins::TFT_BLK, HIGH);

  if (activeType == store::DisplayType::SSD1306_096) {
    // Direct hardware I2C visual test (guarantees pixel lighting)
    oled_diag::runVisualTest(store::getOledSdaPin(), store::getOledSclPin(), store::getOledAddr());
  } else if (activeType == store::DisplayType::SSD1306_096_SPI) {
    // OLED SPI test pattern
    tft.fillScreen(TFT_WHITE);
    delay(300);
    tft.fillScreen(TFT_BLACK);
    delay(150);
    for (int r = 4; r < 32; r += 4) {
      tft.drawRect(64 - r * 2, 32 - r, r * 4, r * 2, TFT_WHITE);
    }
    delay(400);
  } else {
    // TFT RGB Color bars: Red, Green, Blue, White, Black
    tft.fillScreen(TFT_WHITE);
    delay(200);
    tft.fillScreen(TFT_RED);
    delay(200);
    tft.fillScreen(TFT_GREEN);
    delay(200);
    tft.fillScreen(TFT_BLUE);
    delay(200);
    tft.fillScreen(TFT_BLACK);
    delay(100);
  }

  // Restore animated face
  dirty = true;
  renderFace();
  Serial.println("# [gfx] Visual test completed. Smiling face restored.");
  Serial.flush();
}

void switchDisplay(store::DisplayType dt) {
  store::setDisplayType(dt);
  Serial.printf("# [gfx] Saved display type %s (%d) to NVS. Rebooting cleanly in 300ms...\n",
                store::getDisplayTypeName(dt), (int)dt);
  Serial.flush();
  delay(300);
  ESP.restart();
}

bool isReady() {
  return tftInitialized;
}

int getWidth() {
  return metrics.width;
}

int getHeight() {
  return metrics.height;
}

const char* getActiveDisplayModel() {
  return store::getDisplayTypeName(activeType);
}

void setEmotion(Emotion e) {
  if (currentEmotion != e) {
    currentEmotion = e;
    animFrame = 0;
    dirty = true;
  }
}

Emotion getEmotion() {
  return currentEmotion;
}

const char* getEmotionStr() {
  switch (currentEmotion) {
    case Emotion::IDLE:       return "idle";
    case Emotion::HAPPY:      return "happy";
    case Emotion::LISTENING:  return "listening";
    case Emotion::THINKING:   return "thinking";
    case Emotion::SPEAKING:   return "speaking";
    case Emotion::DRIVE_FWD:  return "drive_fwd";
    case Emotion::DRIVE_REV:  return "drive_rev";
    case Emotion::TURN_LEFT:  return "turn_left";
    case Emotion::TURN_RIGHT: return "turn_right";
    case Emotion::DIZZY:      return "dizzy";
    case Emotion::OBSTACLE:   return "obstacle";
    case Emotion::SLEEPY:     return "sleepy";
    default:                  return "unknown";
  }
}

bool setEmotionByName(const char* name) {
  if (strcmp(name, "idle") == 0)       { setEmotion(Emotion::IDLE); return true; }
  if (strcmp(name, "happy") == 0)      { setEmotion(Emotion::HAPPY); return true; }
  if (strcmp(name, "listen") == 0 || strcmp(name, "listening") == 0) { setEmotion(Emotion::LISTENING); return true; }
  if (strcmp(name, "think") == 0 || strcmp(name, "thinking") == 0)   { setEmotion(Emotion::THINKING); return true; }
  if (strcmp(name, "speak") == 0 || strcmp(name, "speaking") == 0)   { setEmotion(Emotion::SPEAKING); return true; }
  if (strcmp(name, "fwd") == 0 || strcmp(name, "forward") == 0)      { setEmotion(Emotion::DRIVE_FWD); return true; }
  if (strcmp(name, "rev") == 0 || strcmp(name, "backward") == 0)     { setEmotion(Emotion::DRIVE_REV); return true; }
  if (strcmp(name, "left") == 0)      { setEmotion(Emotion::TURN_LEFT); return true; }
  if (strcmp(name, "right") == 0)     { setEmotion(Emotion::TURN_RIGHT); return true; }
  if (strcmp(name, "dizzy") == 0)     { setEmotion(Emotion::DIZZY); return true; }
  if (strcmp(name, "obstacle") == 0)  { setEmotion(Emotion::OBSTACLE); return true; }
  if (strcmp(name, "sleep") == 0 || strcmp(name, "sleepy") == 0)     { setEmotion(Emotion::SLEEPY); return true; }
  return false;
}

// Draw a single Mochi Eye (round squircle with pupil highlight)
static void drawEye(int cx, int cy, int w, int h, int r, uint16_t color) {
  if (h <= 4) {
    // Eye slit during full blink
    canvas.fillRoundRect(cx - w / 2, cy - 2, w, 4, 2, color);
    return;
  }
  canvas.fillRoundRect(cx - w / 2, cy - h / 2, w, h, r, color);

  // Cute specular reflection highlight in upper right
  if (h > (int)(24 * ((float)metrics.height / 240.0f))) {
    int hx = cx + w / 4;
    int hy = cy - h / 4;
    canvas.fillCircle(hx, hy, metrics.sparkleR1, TFT_WHITE);
    canvas.fillCircle(hx - metrics.sparkleR1 * 1.5f, hy + metrics.sparkleR1 * 2, metrics.sparkleR2, TFT_WHITE);
  }
}

// Procedural responsive rendering of face
static void renderFace() {
  canvas.fillScreen(TFT_BLACK);

  float scaleX = (float)metrics.width / 240.0f;
  float scaleY = (float)metrics.height / 240.0f;

  int eyeW = metrics.eyeW;
  int eyeH = metrics.eyeH;
  int eyeRadius = metrics.eyeRadius;
  uint16_t eyeColor = (activeType == store::DisplayType::SSD1306_096)
                      ? TFT_WHITE
                      : canvas.color565(0x22, 0xd3, 0xee); // Cyan / Electric blue

  int pScaleX = (int)(pupilOffsetX * scaleX);
  int pScaleY = (int)(pupilOffsetY * scaleY);

  int leftEyeX  = metrics.leftEyeX + pScaleX;
  int leftEyeY  = metrics.centerY + pScaleY;
  int rightEyeX = metrics.rightEyeX + pScaleX;
  int rightEyeY = metrics.centerY + pScaleY;

  switch (currentEmotion) {
    case Emotion::IDLE: {
      // Natural blinking
      int curH = eyeH;
      if (isBlinking) {
        curH = (int)(eyeH * (1.0f - blinkProgress));
        if (curH < 4) curH = 4;
      }
      drawEye(leftEyeX, leftEyeY, eyeW, curH, eyeRadius, eyeColor);
      drawEye(rightEyeX, rightEyeY, eyeW, curH, eyeRadius, eyeColor);
      break;
    }

    case Emotion::HAPPY: {
      // Crescent happy eyes: ^ _ ^
      eyeColor = (activeType == store::DisplayType::SSD1306_096)
                 ? TFT_WHITE
                 : canvas.color565(0x34, 0xd3, 0x99); // Mint green
      int bounce = (animFrame % 10 < 5) ? (int)(4 * scaleY) : 0;
      int rW = (int)(60 * scaleX);
      int rH = (int)(48 * scaleY);
      int rRad = (int)(22 * scaleX);
      int cRad = (int)(24 * scaleY);

      canvas.fillRoundRect(leftEyeX - rW / 2, leftEyeY - rH / 2 + bounce, rW, rH, rRad, eyeColor);
      canvas.fillCircle(leftEyeX, leftEyeY + (int)(12 * scaleY) + bounce, cRad, TFT_BLACK);

      canvas.fillRoundRect(rightEyeX - rW / 2, rightEyeY - rH / 2 + bounce, rW, rH, rRad, eyeColor);
      canvas.fillCircle(rightEyeX, rightEyeY + (int)(12 * scaleY) + bounce, cRad, TFT_BLACK);

      // Cute blush circles
      int blushR = (int)(9 * scaleX);
      if (blushR < 3) blushR = 3;
      canvas.fillCircle(leftEyeX - (int)(18 * scaleX), leftEyeY + (int)(38 * scaleY), blushR, canvas.color565(0xf4, 0x72, 0xb6));
      canvas.fillCircle(rightEyeX + (int)(18 * scaleX), rightEyeY + (int)(38 * scaleY), blushR, canvas.color565(0xf4, 0x72, 0xb6));
      break;
    }

    case Emotion::LISTENING: {
      // Big attentive eyes + Audio Wave pulse at bottom
      eyeColor = canvas.color565(0x60, 0xa5, 0xfa); // Sky blue
      int lW = (int)(eyeW * 1.15f);
      int lH = (int)(eyeH * 1.1f);
      drawEye(metrics.leftEyeX, metrics.centerY - (int)(10 * scaleY), lW, lH, eyeRadius, eyeColor);
      drawEye(metrics.rightEyeX, metrics.centerY - (int)(10 * scaleY), lW, lH, eyeRadius, eyeColor);

      // Listening audio wave bars at bottom
      int numBars = metrics.width < 150 ? 5 : 7;
      int barSpacing = (int)(12 * scaleX);
      int startX = (metrics.width / 2) - ((numBars - 1) * barSpacing) / 2;
      int waveBaseY = metrics.height - (int)(25 * scaleY);

      for (int i = 0; i < numBars; i++) {
        int barH = (int)((6 + (int)(16 * sinf((animFrame * 0.4f) + i * 0.8f))) * scaleY);
        if (barH < 3) barH = 3;
        canvas.fillRoundRect(startX + i * barSpacing - 2, waveBaseY - barH / 2, (int)(5 * scaleX) > 2 ? (int)(5 * scaleX) : 3, barH, 2, canvas.color565(0x38, 0xbd, 0xf8));
      }
      break;
    }

    case Emotion::THINKING: {
      // Eyes looking up-right + Rotating thinking dot
      eyeColor = canvas.color565(0xfb, 0xbf, 0x24); // Amber
      drawEye(metrics.leftEyeX + (int)(12 * scaleX), metrics.centerY - (int)(12 * scaleY), eyeW, eyeH, eyeRadius, eyeColor);
      drawEye(metrics.rightEyeX + (int)(12 * scaleX), metrics.centerY - (int)(12 * scaleY), eyeW, eyeH, eyeRadius, eyeColor);

      // Rotating orbital dots
      float angle = (animFrame * 0.2f);
      int dotRadiusX = (int)(22 * scaleX);
      int dotRadiusY = (int)(8 * scaleY);
      int centerX = metrics.width / 2;
      int centerY = metrics.height - (int)(35 * scaleY);
      int dotX = centerX + (int)(dotRadiusX * cosf(angle));
      int dotY = centerY + (int)(dotRadiusY * sinf(angle));
      canvas.fillCircle(dotX, dotY, (int)(4 * scaleX) > 2 ? (int)(4 * scaleX) : 2, TFT_WHITE);
      canvas.fillCircle(centerX, centerY, 2, TFT_DARKGRAY);
      break;
    }

    case Emotion::SPEAKING: {
      // Bouncing mouth animation
      eyeColor = canvas.color565(0xa7, 0x8b, 0xfa); // Soft purple
      drawEye(metrics.leftEyeX, metrics.centerY - (int)(8 * scaleY), eyeW, eyeH, eyeRadius, eyeColor);
      drawEye(metrics.rightEyeX, metrics.centerY - (int)(8 * scaleY), eyeW, eyeH, eyeRadius, eyeColor);

      int mouthH = (int)((6 + (animFrame % 6) * 3) * scaleY);
      int mouthW = (int)(36 * scaleX);
      int mouthY = metrics.height - (int)(35 * scaleY);
      canvas.fillRoundRect(metrics.width / 2 - mouthW / 2, mouthY - mouthH / 2, mouthW, mouthH, 5, canvas.color565(0xf4, 0x72, 0xb6));
      break;
    }

    case Emotion::DRIVE_FWD: {
      // Dynamic forward slant eyes
      eyeColor = canvas.color565(0x38, 0xbd, 0xf8);
      drawEye(metrics.leftEyeX, metrics.centerY, eyeW, eyeH, eyeRadius, eyeColor);
      drawEye(metrics.rightEyeX, metrics.centerY, eyeW, eyeH, eyeRadius, eyeColor);
      // Speed arrow or dashes
      int dashOffset = (animFrame * 3) % (int)(20 * scaleY + 1);
      int dashY = metrics.height - (int)(35 * scaleY) + dashOffset;
      if (dashY < metrics.height - 4) {
        canvas.drawFastHLine(metrics.width / 2 - (int)(10 * scaleX), dashY, (int)(20 * scaleX), eyeColor);
      }
      break;
    }

    case Emotion::DRIVE_REV: {
      // Cautious glance backward
      eyeColor = canvas.color565(0xfb, 0x92, 0x3c); // Orange
      drawEye(metrics.leftEyeX - (int)(10 * scaleX), metrics.centerY + (int)(6 * scaleY), eyeW, eyeH, eyeRadius, eyeColor);
      drawEye(metrics.rightEyeX - (int)(10 * scaleX), metrics.centerY + (int)(6 * scaleY), eyeW, eyeH, eyeRadius, eyeColor);
      break;
    }

    case Emotion::TURN_LEFT: {
      eyeColor = canvas.color565(0x38, 0xbd, 0xf8);
      drawEye(metrics.leftEyeX - (int)(16 * scaleX), metrics.centerY, eyeW, eyeH, eyeRadius, eyeColor);
      drawEye(metrics.rightEyeX - (int)(16 * scaleX), metrics.centerY, eyeW, eyeH, eyeRadius, eyeColor);
      break;
    }

    case Emotion::TURN_RIGHT: {
      eyeColor = canvas.color565(0x38, 0xbd, 0xf8);
      drawEye(metrics.leftEyeX + (int)(16 * scaleX), metrics.centerY, eyeW, eyeH, eyeRadius, eyeColor);
      drawEye(metrics.rightEyeX + (int)(16 * scaleX), metrics.centerY, eyeW, eyeH, eyeRadius, eyeColor);
      break;
    }

    case Emotion::DIZZY: {
      // Spiral eyes @_@
      eyeColor = canvas.color565(0xf4, 0x3f, 0x5e); // Rose / Red
      int maxR = (int)(eyeW * 0.52f);
      int step = (int)(7 * scaleX);
      if (step < 3) step = 3;
      for (int r = step; r <= maxR; r += step) {
        float rot = (animFrame * 0.3f);
        canvas.drawArc(metrics.leftEyeX, metrics.centerY, r, r - 2, (int)(rot * 57.3f), (int)((rot + 4.5f) * 57.3f), eyeColor);
        canvas.drawArc(metrics.rightEyeX, metrics.centerY, r, r - 2, (int)(-rot * 57.3f), (int)((-rot + 4.5f) * 57.3f), eyeColor);
      }
      break;
    }

    case Emotion::OBSTACLE: {
      // Cross eyes: X _ X
      eyeColor = canvas.color565(0xef, 0x44, 0x44); // Bright Red
      int hW = eyeW / 2;
      int hH = eyeH / 2;
      // Left X
      canvas.drawLine(metrics.leftEyeX - hW, metrics.centerY - hH, metrics.leftEyeX + hW, metrics.centerY + hH, eyeColor);
      canvas.drawLine(metrics.leftEyeX - hW, metrics.centerY - hH + 1, metrics.leftEyeX + hW, metrics.centerY + hH + 1, eyeColor);
      canvas.drawLine(metrics.leftEyeX - hW, metrics.centerY + hH, metrics.leftEyeX + hW, metrics.centerY - hH, eyeColor);
      canvas.drawLine(metrics.leftEyeX - hW, metrics.centerY + hH + 1, metrics.leftEyeX + hW, metrics.centerY - hH + 1, eyeColor);
      // Right X
      canvas.drawLine(metrics.rightEyeX - hW, metrics.centerY - hH, metrics.rightEyeX + hW, metrics.centerY + hH, eyeColor);
      canvas.drawLine(metrics.rightEyeX - hW, metrics.centerY - hH + 1, metrics.rightEyeX + hW, metrics.centerY + hH + 1, eyeColor);
      canvas.drawLine(metrics.rightEyeX - hW, metrics.centerY + hH, metrics.rightEyeX + hW, metrics.centerY - hH, eyeColor);
      canvas.drawLine(metrics.rightEyeX - hW, metrics.centerY + hH + 1, metrics.rightEyeX + hW, metrics.centerY - hH + 1, eyeColor);
      break;
    }

    case Emotion::SLEEPY: {
      // Drooping half-eyes + floating Zzz
      eyeColor = canvas.color565(0x94, 0xa3, 0xb8); // Dim blue gray
      int halfH = (int)(14 * scaleY);
      if (halfH < 5) halfH = 5;
      canvas.fillRoundRect(metrics.leftEyeX - eyeW / 2, metrics.centerY, eyeW, halfH, 4, eyeColor);
      canvas.fillRoundRect(metrics.rightEyeX - eyeW / 2, metrics.centerY, eyeW, halfH, 4, eyeColor);

      // Floating Zzz
      int zY = (int)(metrics.centerY - (20 * scaleY) - ((animFrame * 2) % (int)(30 * scaleY + 1)));
      canvas.setTextColor(canvas.color565(0x38, 0xbd, 0xf8));
      canvas.setTextSize(1);
      canvas.drawString("z", metrics.width / 2 + (int)(30 * scaleX), zY + (int)(15 * scaleY));
      canvas.setTextSize(scaleX > 0.8f ? 2 : 1);
      canvas.drawString("Z", metrics.width / 2 + (int)(45 * scaleX), zY + (int)(5 * scaleY));
      canvas.setTextSize(scaleX > 0.8f ? 3 : 2);
      canvas.drawString("Z", metrics.width / 2 + (int)(60 * scaleX), zY - (int)(5 * scaleY));
      break;
    }
  }

  // Push the completed frame buffer to display
  canvas.pushSprite(0, 0);
}

void update() {
  uint32_t now = millis();
  if (now - lastAnimTime < 33) { // ~30 FPS animation cycle
    return;
  }
  lastAnimTime = now;

  // Handle natural blinking during IDLE
  if (currentEmotion == Emotion::IDLE) {
    if (!isBlinking && now >= nextBlinkTime) {
      isBlinking = true;
      blinkProgress = 0.0f;
      dirty = true;
    }
    if (isBlinking) {
      blinkProgress += 0.25f;
      dirty = true;
      if (blinkProgress >= 1.0f) {
        blinkProgress = 1.0f;
        isBlinking = false;
        // Schedule next blink in 2.5 to 4.5 seconds
        nextBlinkTime = now + 2500 + (random(0, 2000));
        // Randomly gaze left, right, or center
        int r = random(0, 5);
        int newPupilX = (r == 0) ? -12 : (r == 1 ? 12 : 0);
        if (newPupilX != pupilOffsetX) {
          pupilOffsetX = newPupilX;
          dirty = true;
        }
      }
    }
  } else {
    if (isBlinking || pupilOffsetX != 0 || pupilOffsetY != 0) {
      isBlinking = false;
      pupilOffsetX = 0;
      pupilOffsetY = 0;
      dirty = true;
    }
  }

  if (isAnimatedEmotion(currentEmotion)) {
    animFrame++;
    dirty = true;
  }

  // If SSD1306 OLED mode is active but not yet locked:
  // Continually pulse wake-up sequence across candidate headers every 1.5s
  if (activeType == store::DisplayType::SSD1306_096 && !store::isOledDetected()) {
    static uint32_t lastOledPulse = 0;
    if (now - lastOledPulse > 1500) {
      lastOledPulse = now;
      oled_diag::pulseAllPossibleOledHeaders();
    }
  }

  // Energy & SPI Throttling: only redraw & push buffer when dirty
  if (dirty) {
    renderFace();
    dirty = false;
  }
}

} // namespace emotion_gfx
