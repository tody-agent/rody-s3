#include "emotion_gfx.h"
#include "../include/pins.h"
#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

namespace emotion_gfx {

class LGFX_Otto : public lgfx::LGFX_Device {
  lgfx::Panel_ST7789 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;

public:
  LGFX_Otto() {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI2_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000; // 40MHz SPI write
      cfg.freq_read = 16000000;
      cfg.spi_3wire = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = pins::TFT_SCLK; // GPIO 42
      cfg.pin_mosi = pins::TFT_MOSI; // GPIO 41
      cfg.pin_miso = -1;
      cfg.pin_dc = pins::TFT_DC;     // GPIO 40
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs = pins::TFT_CS;     // GPIO 38
      cfg.pin_rst = pins::TFT_RST;   // GPIO 39
      cfg.pin_busy = -1;
      cfg.panel_width = 240;
      cfg.panel_height = 240;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = false;
      cfg.invert = true; // ST7789 standard inverted
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      _panel_instance.config(cfg);
    }
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = pins::TFT_BLK;    // GPIO 21
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 7;
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }
    setPanel(&_panel_instance);
  }
};

static LGFX_Otto tft;
static LGFX_Sprite canvas(&tft);
static bool tftInitialized = false;

static Emotion currentEmotion = Emotion::IDLE;
static uint32_t lastAnimTime = 0;
static uint32_t nextBlinkTime = 0;
static bool isBlinking = false;
static float blinkProgress = 0.0f; // 0.0 (open) to 1.0 (closed)
static int pupilOffsetX = 0;
static int pupilOffsetY = 0;
static uint32_t animFrame = 0;

void init() {
  tft.init();
  tft.setBrightness(180);
  tft.fillScreen(TFT_BLACK);

  // Allocate 240x240 double buffer in PSRAM or SRAM
  canvas.setColorDepth(16);
  if (!canvas.createSprite(240, 240)) {
    Serial.println("# [gfx] Failed to create 240x240 sprite, fallback to direct draw");
    tftInitialized = false;
  } else {
    tftInitialized = true;
    Serial.println("# [gfx] LovyanGFX ST7789 initialized with 240x240 double buffer");
  }

  nextBlinkTime = millis() + 2000;
}

void setEmotion(Emotion e) {
  currentEmotion = e;
  animFrame = 0;
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
  if (h > 24) {
    int hx = cx + w / 4;
    int hy = cy - h / 4;
    canvas.fillCircle(hx, hy, 6, TFT_WHITE);
    canvas.fillCircle(hx - 10, hy + 12, 3, TFT_WHITE);
  }
}

// Procedural rendering of Xiaozhi / Mochi face
static void renderFace() {
  canvas.fillScreen(TFT_BLACK);

  int eyeW = 54;
  int eyeH = 74;
  int eyeRadius = 26;
  uint16_t eyeColor = canvas.color565(0x22, 0xd3, 0xee); // Cyan / Electric blue

  int leftEyeX = 72 + pupilOffsetX;
  int leftEyeY = 120 + pupilOffsetY;
  int rightEyeX = 168 + pupilOffsetX;
  int rightEyeY = 120 + pupilOffsetY;

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
      eyeColor = canvas.color565(0x34, 0xd3, 0x99); // Mint green
      int bounce = (animFrame % 10 < 5) ? 4 : 0;
      canvas.fillRoundRect(leftEyeX - 30, leftEyeY - 24 + bounce, 60, 48, 22, eyeColor);
      canvas.fillCircle(leftEyeX, leftEyeY + 12 + bounce, 24, TFT_BLACK);

      canvas.fillRoundRect(rightEyeX - 30, rightEyeY - 24 + bounce, 60, 48, 22, eyeColor);
      canvas.fillCircle(rightEyeX, rightEyeY + 12 + bounce, 24, TFT_BLACK);

      // Cute blush circles
      canvas.fillCircle(leftEyeX - 18, leftEyeY + 44, 9, canvas.color565(0xf4, 0x72, 0xb6));
      canvas.fillCircle(rightEyeX + 18, rightEyeY + 44, 9, canvas.color565(0xf4, 0x72, 0xb6));
      break;
    }

    case Emotion::LISTENING: {
      // Big attentive eyes + Audio Wave pulse at bottom
      eyeColor = canvas.color565(0x60, 0xa5, 0xfa); // Sky blue
      drawEye(72, 108, 62, 82, 30, eyeColor);
      drawEye(168, 108, 62, 82, 30, eyeColor);

      // Listening audio wave bars at bottom
      for (int i = 0; i < 7; i++) {
        int barH = 6 + (int)(18 * sinf((animFrame * 0.4f) + i * 0.8f));
        if (barH < 4) barH = 4;
        canvas.fillRoundRect(78 + i * 12, 195 - barH / 2, 6, barH, 3, canvas.color565(0x38, 0xbd, 0xf8));
      }
      break;
    }

    case Emotion::THINKING: {
      // Eyes looking up-right + Rotating thinking dot
      eyeColor = canvas.color565(0xfb, 0xbf, 0x24); // Amber
      drawEye(72 + 12, 120 - 14, 52, 68, 24, eyeColor);
      drawEye(168 + 12, 120 - 14, 52, 68, 24, eyeColor);

      // Rotating orbital dots
      float angle = (animFrame * 0.2f);
      int dotX = 120 + (int)(22 * cosf(angle));
      int dotY = 190 + (int)(8 * sinf(angle));
      canvas.fillCircle(dotX, dotY, 4, TFT_WHITE);
      canvas.fillCircle(120, 190, 2, TFT_DARKGRAY);
      break;
    }

    case Emotion::SPEAKING: {
      // Bouncing mouth animation
      eyeColor = canvas.color565(0xa7, 0x8b, 0xfa); // Soft purple
      drawEye(72, 110, eyeW, eyeH, eyeRadius, eyeColor);
      drawEye(168, 110, eyeW, eyeH, eyeRadius, eyeColor);

      int mouthH = 6 + (animFrame % 6) * 4;
      canvas.fillRoundRect(120 - 20, 182 - mouthH / 2, 40, mouthH, 6, canvas.color565(0xf4, 0x72, 0xb6));
      break;
    }

    case Emotion::DRIVE_FWD: {
      // Dynamic forward slant eyes
      eyeColor = canvas.color565(0x38, 0xbd, 0xf8);
      drawEye(72, 120, 56, 70, 20, eyeColor);
      drawEye(168, 120, 56, 70, 20, eyeColor);
      // Speed arrow or dashes
      int dashOffset = (animFrame * 3) % 20;
      canvas.drawFastHLine(110, 185 + dashOffset, 20, eyeColor);
      break;
    }

    case Emotion::DRIVE_REV: {
      // Cautious glance backward
      eyeColor = canvas.color565(0xfb, 0x92, 0x3c); // Orange
      drawEye(72 - 12, 120 + 8, 50, 64, 20, eyeColor);
      drawEye(168 - 12, 120 + 8, 50, 64, 20, eyeColor);
      break;
    }

    case Emotion::TURN_LEFT: {
      eyeColor = canvas.color565(0x38, 0xbd, 0xf8);
      drawEye(72 - 20, 120, 54, 74, 24, eyeColor);
      drawEye(168 - 20, 120, 54, 74, 24, eyeColor);
      break;
    }

    case Emotion::TURN_RIGHT: {
      eyeColor = canvas.color565(0x38, 0xbd, 0xf8);
      drawEye(72 + 20, 120, 54, 74, 24, eyeColor);
      drawEye(168 + 20, 120, 54, 74, 24, eyeColor);
      break;
    }

    case Emotion::DIZZY: {
      // Spiral eyes @_@
      eyeColor = canvas.color565(0xf4, 0x3f, 0x5e); // Rose / Red
      for (int r = 8; r <= 28; r += 7) {
        float rot = (animFrame * 0.3f);
        canvas.drawArc(72, 120, r, r - 3, (int)(rot * 57.3f), (int)((rot + 4.5f) * 57.3f), eyeColor);
        canvas.drawArc(168, 120, r, r - 3, (int)(-rot * 57.3f), (int)((-rot + 4.5f) * 57.3f), eyeColor);
      }
      break;
    }

    case Emotion::OBSTACLE: {
      // Cross eyes: X _ X
      eyeColor = canvas.color565(0xef, 0x44, 0x44); // Bright Red
      // Left X
      canvas.drawLine(48, 96, 96, 144, eyeColor);
      canvas.drawLine(48, 97, 96, 145, eyeColor);
      canvas.drawLine(48, 144, 96, 96, eyeColor);
      canvas.drawLine(48, 145, 96, 97, eyeColor);
      // Right X
      canvas.drawLine(144, 96, 192, 144, eyeColor);
      canvas.drawLine(144, 97, 192, 145, eyeColor);
      canvas.drawLine(144, 144, 192, 96, eyeColor);
      canvas.drawLine(144, 145, 192, 97, eyeColor);
      break;
    }

    case Emotion::SLEEPY: {
      // Drooping half-eyes + floating Zzz
      eyeColor = canvas.color565(0x94, 0xa3, 0xb8); // Dim blue gray
      canvas.fillRoundRect(72 - 25, 125, 50, 14, 6, eyeColor);
      canvas.fillRoundRect(168 - 25, 125, 50, 14, 6, eyeColor);

      // Floating Zzz
      int zY = 80 - ((animFrame * 2) % 40);
      canvas.setTextColor(canvas.color565(0x38, 0xbd, 0xf8));
      canvas.setTextSize(2);
      canvas.drawString("z", 160, zY + 20);
      canvas.drawString("Z", 175, zY + 8);
      canvas.setTextSize(3);
      canvas.drawString("Z", 192, zY - 8);
      break;
    }
  }

  // Push the completed frame buffer to ST7789 display
  canvas.pushSprite(0, 0);
}

void update() {
  uint32_t now = millis();
  if (now - lastAnimTime < 33) { // ~30 FPS animation cycle
    return;
  }
  lastAnimTime = now;
  animFrame++;

  // Handle natural blinking during IDLE
  if (currentEmotion == Emotion::IDLE) {
    if (!isBlinking && now >= nextBlinkTime) {
      isBlinking = true;
      blinkProgress = 0.0f;
    }
    if (isBlinking) {
      blinkProgress += 0.25f;
      if (blinkProgress >= 1.0f) {
        blinkProgress = 1.0f;
        isBlinking = false;
        // Schedule next blink in 2.5 to 4.5 seconds
        nextBlinkTime = now + 2500 + (random(0, 2000));
        // Randomly gaze left, right, or center
        int r = random(0, 5);
        if (r == 0) pupilOffsetX = -12;
        else if (r == 1) pupilOffsetX = 12;
        else pupilOffsetX = 0;
      }
    }
  } else {
    isBlinking = false;
    pupilOffsetX = 0;
    pupilOffsetY = 0;
  }

  renderFace();
}

}
