#include "pet_emotions.h"
#include "../include/pet_pins.h"
#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <math.h>

namespace pet_emotions {

class LGFX_Pet : public lgfx::LGFX_Device {
  lgfx::Panel_ST7789 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;

public:
  LGFX_Pet() {
    {
      auto cfg = _bus_instance.config();
      cfg.spi_host = SPI2_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read = 16000000;
      cfg.spi_3wire = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk = pet_pins::TFT_SCLK; // 42
      cfg.pin_mosi = pet_pins::TFT_MOSI; // 41
      cfg.pin_miso = -1;
      cfg.pin_dc = pet_pins::TFT_DC;     // 40
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }
    {
      auto cfg = _panel_instance.config();
      cfg.pin_cs = pet_pins::TFT_CS;     // 38
      cfg.pin_rst = pet_pins::TFT_RST;   // 39
      cfg.pin_busy = -1;
      cfg.panel_width = 240;
      cfg.panel_height = 240;
      cfg.offset_x = 0;
      cfg.offset_y = 0;
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = false;
      cfg.invert = true;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;
      _panel_instance.config(cfg);
    }
    {
      auto cfg = _light_instance.config();
      cfg.pin_bl = pet_pins::TFT_BLK;    // 21
      cfg.invert = false;
      cfg.freq = 44100;
      cfg.pwm_channel = 7;
      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);
    }
    setPanel(&_panel_instance);
  }
};

static LGFX_Pet lcd;
static LGFX_Sprite sprite(&lcd);

static PetMood currentMood = PetMood::IDLE;
static uint32_t animFrame = 0;
static uint32_t lastFrameTime = 0;
static bool isBlinking = false;
static uint32_t nextBlinkTime = 0;

static const uint16_t COLOR_CYAN    = 0x1E5F; // #22d3ee
static const uint16_t COLOR_PINK    = 0xF9D7; // #f43f5e
static const uint16_t COLOR_YELLOW  = 0xFFE0; // #fbbf24
static const uint16_t COLOR_RED     = 0xF800; // #ef4444
static const uint16_t COLOR_BLUE    = 0x3CF7; // #3b82f6
static const uint16_t COLOR_WHITE   = 0xFFFF;
static const uint16_t COLOR_BG      = 0x0000;

static void drawHeart(int cx, int cy, int size, uint16_t color) {
  int r = size / 2;
  sprite.fillCircle(cx - r / 2, cy - r / 2, r / 2, color);
  sprite.fillCircle(cx + r / 2, cy - r / 2, r / 2, color);
  sprite.fillTriangle(cx - size / 2, cy - r / 4, cx + size / 2, cy - r / 4, cx, cy + size / 2, color);
}

bool init() {
  lcd.init();
  lcd.setRotation(0);
  lcd.setBrightness(180);
  
  sprite.setColorDepth(16);
  if (!sprite.createSprite(240, 240)) {
    Serial.println("# [pet_emotions] Failed to allocate 240x240 sprite");
    return false;
  }
  
  nextBlinkTime = millis() + 2000;
  Serial.println("# [pet_emotions] ST7789 Pet Emotion Engine initialized (60 FPS)");
  return true;
}

void setMood(PetMood mood) {
  if (currentMood != mood) {
    currentMood = mood;
    animFrame = 0;
  }
}

PetMood getMood() {
  return currentMood;
}

const char* getMoodStr(PetMood mood) {
  switch (mood) {
    case PetMood::IDLE:         return "idle";
    case PetMood::PURRING:      return "purring";
    case PetMood::HURT:         return "hurt";
    case PetMood::BELLY_UP:     return "belly_up";
    case PetMood::SHAKEN_DIZZY: return "shaken_dizzy";
    case PetMood::ANGRY:        return "angry";
    case PetMood::STARTLED:     return "startled";
    case PetMood::ANNOYED:      return "annoyed";
    case PetMood::SLEEPY:       return "sleepy";
    case PetMood::DISCO:        return "disco";
    case PetMood::SNEEZE:       return "sneeze";
    case PetMood::AIRPLANE:     return "airplane";
    case PetMood::TICKLE:       return "tickle";
    case PetMood::HIGH_FIVE:    return "high_five";
    case PetMood::NUDGE:        return "nudge";
    case PetMood::COOL_GLASSES: return "cool_glasses";
    default:                    return "unknown";
  }
}

void update() {
  uint32_t now = millis();
  if (now - lastFrameTime < 16) return; // ~60 FPS
  lastFrameTime = now;
  animFrame++;

  // Handle random eye blink in IDLE
  if (currentMood == PetMood::IDLE) {
    if (!isBlinking && now >= nextBlinkTime) {
      isBlinking = true;
    } else if (isBlinking && now >= nextBlinkTime + 130) {
      isBlinking = false;
      nextBlinkTime = now + random(2000, 5000);
    }
  } else {
    isBlinking = false;
  }

  sprite.fillScreen(COLOR_BG);

  int leftEyeX = 70;
  int rightEyeX = 170;
  int eyeY = 120;

  switch (currentMood) {
    case PetMood::IDLE: {
      if (isBlinking) {
        sprite.fillRoundRect(leftEyeX - 28, eyeY - 3, 56, 6, 3, COLOR_CYAN);
        sprite.fillRoundRect(rightEyeX - 28, eyeY - 3, 56, 6, 3, COLOR_CYAN);
      } else {
        sprite.fillRoundRect(leftEyeX - 25, eyeY - 45, 50, 90, 25, COLOR_CYAN);
        sprite.fillRoundRect(rightEyeX - 25, eyeY - 45, 50, 90, 25, COLOR_CYAN);
        // Highlights
        sprite.fillCircle(leftEyeX + 10, eyeY - 25, 9, COLOR_WHITE);
        sprite.fillCircle(leftEyeX - 5, eyeY + 15, 5, COLOR_WHITE);
        sprite.fillCircle(rightEyeX + 10, eyeY - 25, 9, COLOR_WHITE);
        sprite.fillCircle(rightEyeX - 5, eyeY + 15, 5, COLOR_WHITE);
      }
      break;
    }

    case PetMood::PURRING: {
      // Mắt cười cong trăng khuyết thỏa mãn
      sprite.drawArc(leftEyeX, eyeY + 10, 32, 24, 200, 340, COLOR_CYAN);
      sprite.drawArc(rightEyeX, eyeY + 10, 32, 24, 200, 340, COLOR_CYAN);
      // Trái tim đập phập phồng ở giữa 2 mắt
      int heartScale = 18 + (int)(sinf((float)animFrame * 0.15f) * 5.0f);
      int heartY = 90 - (int)((animFrame % 60) * 0.4f);
      drawHeart(120, heartY, heartScale, COLOR_PINK);
      break;
    }

    case PetMood::HURT: {
      // Mắt cụp xuống đau đớn
      sprite.fillRoundRect(leftEyeX - 26, eyeY - 25, 52, 65, 20, COLOR_BLUE);
      sprite.fillRoundRect(rightEyeX - 26, eyeY - 25, 52, 65, 20, COLOR_BLUE);
      // Lông mày buồn cụp
      sprite.drawLine(leftEyeX - 30, eyeY - 45, leftEyeX + 25, eyeY - 35, COLOR_WHITE);
      sprite.drawLine(rightEyeX + 30, eyeY - 45, rightEyeX - 25, eyeY - 35, COLOR_WHITE);
      // Giọt nước mắt rơi
      int tearY = eyeY + 25 + ((animFrame * 2) % 60);
      sprite.fillCircle(leftEyeX - 10, tearY, 6, COLOR_CYAN);
      sprite.fillCircle(rightEyeX + 10, tearY, 6, COLOR_CYAN);
      break;
    }

    case PetMood::BELLY_UP: {
      // Mắt trợn to xoay vòng, chớp đỏ cầu cứu
      uint16_t col = (animFrame % 20 < 10) ? COLOR_RED : COLOR_YELLOW;
      float rotAngle = (float)animFrame * 0.2f;
      int offX = (int)(cosf(rotAngle) * 16.0f);
      int offY = (int)(sinf(rotAngle) * 16.0f);

      sprite.fillCircle(leftEyeX, eyeY, 35, col);
      sprite.fillCircle(rightEyeX, eyeY, 35, col);
      sprite.fillCircle(leftEyeX + offX, eyeY + offY, 12, COLOR_BG);
      sprite.fillCircle(rightEyeX + offX, eyeY + offY, 12, COLOR_BG);
      break;
    }

    case PetMood::SHAKEN_DIZZY: {
      // Mắt xoắn ốc trôn ốc
      float baseA = (float)animFrame * 0.25f;
      for (int i = 0; i < 2; i++) {
        int cx = (i == 0) ? leftEyeX : rightEyeX;
        for (int r = 10; r < 36; r += 8) {
          float a = baseA + (float)r * 0.15f;
          int px = cx + (int)(cosf(a) * r);
          int py = eyeY + (int)(sinf(a) * r);
          sprite.fillCircle(px, py, 4, COLOR_YELLOW);
        }
      }
      break;
    }

    case PetMood::ANGRY: {
      // Mắt tam giác xếch nhọn hung hăng màu đỏ
      sprite.fillTriangle(leftEyeX - 25, eyeY + 25, leftEyeX + 25, eyeY + 25, leftEyeX + 25, eyeY - 30, COLOR_RED);
      sprite.fillTriangle(rightEyeX - 25, eyeY + 25, rightEyeX + 25, eyeY + 25, rightEyeX - 25, eyeY - 30, COLOR_RED);
      // Con ngươi bốc lửa
      sprite.fillCircle(leftEyeX + 5, eyeY + 5, 8, COLOR_YELLOW);
      sprite.fillCircle(rightEyeX - 5, eyeY + 5, 8, COLOR_YELLOW);
      break;
    }

    case PetMood::STARTLED: {
      // Giật mình: Mắt mở to hết cỡ, đồng tử nhỏ xíu co giật
      int tremor = (animFrame % 4 < 2) ? 2 : -2;
      sprite.fillRoundRect(leftEyeX - 35, eyeY - 50 + tremor, 70, 100, 35, COLOR_WHITE);
      sprite.fillRoundRect(rightEyeX - 35, eyeY - 50 + tremor, 70, 100, 35, COLOR_WHITE);
      // Đồng tử thu nhỏ
      sprite.fillCircle(leftEyeX, eyeY + tremor, 10, COLOR_BG);
      sprite.fillCircle(rightEyeX, eyeY + tremor, 10, COLOR_BG);
      break;
    }

    case PetMood::ANNOYED: {
      // Mắt nhăn mày tịt lại vì tiếng ồn
      sprite.fillRoundRect(leftEyeX - 25, eyeY - 4, 50, 8, 4, COLOR_CYAN);
      sprite.fillRoundRect(rightEyeX - 25, eyeY - 4, 50, 8, 4, COLOR_CYAN);
      // Lông mày nhíu cau lại
      sprite.drawLine(leftEyeX - 25, eyeY - 20, leftEyeX + 25, eyeY - 10, COLOR_YELLOW);
      sprite.drawLine(rightEyeX - 25, eyeY - 10, rightEyeX + 25, eyeY - 20, COLOR_YELLOW);
      break;
    }

    case PetMood::SLEEPY: {
      // Mắt lim dim ngủ say
      sprite.drawArc(leftEyeX, eyeY, 26, 20, 20, 160, COLOR_BLUE);
      sprite.drawArc(rightEyeX, eyeY, 26, 20, 20, 160, COLOR_BLUE);
      // Chữ Zzz bay lên
      int zY = 70 - (int)((animFrame % 80) * 0.6f);
      sprite.drawString("Z", 150, zY, &fonts::Font2);
      sprite.drawString("z", 165, zY - 15, &fonts::Font0);
      break;
    }

    case PetMood::DISCO: {
      // Kính râm disco neon chiptune nhấp nháy
      sprite.fillRoundRect(35, eyeY - 24, 170, 48, 8, COLOR_BG);
      sprite.drawRoundRect(35, eyeY - 24, 170, 48, 8, COLOR_WHITE);
      // Shutter shade stripes
      uint16_t cols[3] = {COLOR_PINK, COLOR_CYAN, COLOR_YELLOW};
      for (int bar = 0; bar < 4; bar++) {
        uint16_t c = cols[(bar + (animFrame / 4)) % 3];
        sprite.fillRect(40, eyeY - 18 + bar * 11, 70, 4, c);
        sprite.fillRect(130, eyeY - 18 + bar * 11, 70, 4, c);
      }
      break;
    }

    case PetMood::SNEEZE: {
      if (animFrame < 40) {
        // Hít sâu: Mắt co thắt run rẩy
        int tremor = (animFrame % 2 == 0) ? 2 : -2;
        sprite.drawLine(leftEyeX - 25, eyeY + tremor, leftEyeX + 25, eyeY + tremor, COLOR_CYAN);
        sprite.drawLine(rightEyeX - 25, eyeY + tremor, rightEyeX + 25, eyeY + tremor, COLOR_CYAN);
        sprite.fillCircle(120, eyeY + 16, 4, COLOR_PINK);
      } else {
        // Hắt xì bùng nổ: Mắt trợn to, lệ bắn
        sprite.fillCircle(leftEyeX, eyeY, 32, COLOR_WHITE);
        sprite.fillCircle(rightEyeX, eyeY, 32, COLOR_WHITE);
        sprite.fillCircle(leftEyeX, eyeY, 14, COLOR_BG);
        sprite.fillCircle(rightEyeX, eyeY, 14, COLOR_BG);
        sprite.fillCircle(leftEyeX - 38, eyeY - 12, 6, COLOR_CYAN);
        sprite.fillCircle(rightEyeX + 38, eyeY - 12, 6, COLOR_CYAN);
      }
      break;
    }

    case PetMood::AIRPLANE: {
      // Kính phi công & vệt gió bay lượn
      sprite.fillRect(20, eyeY - 4, 200, 8, 0x8A22); // Dây da kính
      sprite.fillCircle(leftEyeX, eyeY, 34, 0xB482);
      sprite.fillCircle(rightEyeX, eyeY, 34, 0xB482);
      sprite.fillCircle(leftEyeX, eyeY, 28, COLOR_CYAN);
      sprite.fillCircle(rightEyeX, eyeY, 28, COLOR_CYAN);
      sprite.drawLine(leftEyeX - 10, eyeY - 10, leftEyeX + 10, eyeY + 10, COLOR_WHITE);
      sprite.drawLine(rightEyeX - 10, eyeY - 10, rightEyeX + 10, eyeY + 10, COLOR_WHITE);
      // Vệt gió
      int wx = 240 - ((animFrame * 9) % 240);
      sprite.drawLine(wx, 45, wx - 35, 45, COLOR_WHITE);
      sprite.drawLine((wx + 90) % 240, 195, ((wx + 90) % 240) - 40, 195, COLOR_WHITE);
      break;
    }

    case PetMood::TICKLE: {
      // Cù lét: Mắt > < và má hồng cười nhột
      sprite.drawLine(leftEyeX - 25, eyeY - 16, leftEyeX + 15, eyeY, COLOR_CYAN);
      sprite.drawLine(leftEyeX - 25, eyeY + 16, leftEyeX + 15, eyeY, COLOR_CYAN);
      sprite.drawLine(rightEyeX + 25, eyeY - 16, rightEyeX - 15, eyeY, COLOR_CYAN);
      sprite.drawLine(rightEyeX + 25, eyeY + 16, rightEyeX - 15, eyeY, COLOR_CYAN);
      // Má hồng
      sprite.fillCircle(leftEyeX - 8, eyeY + 28, 12, COLOR_PINK);
      sprite.fillCircle(rightEyeX + 8, eyeY + 28, 12, COLOR_PINK);
      // Miệng cười
      sprite.drawArc(120, eyeY + 20, 14, 10, 10, 170, COLOR_YELLOW);
      break;
    }

    case PetMood::HIGH_FIVE: {
      if (animFrame < 60) {
        // Lời mời đập tay
        sprite.fillRoundRect(100, 80, 40, 50, 10, COLOR_YELLOW);
        sprite.fillCircle(120, 75, 18, COLOR_YELLOW);
        sprite.drawString("TAP!", 98, 145, &fonts::Font2);
      } else {
        // Pháo hoa chiến thắng
        sprite.drawArc(leftEyeX, eyeY + 10, 26, 20, 200, 340, COLOR_CYAN);
        sprite.drawArc(rightEyeX, eyeY + 10, 26, 20, 200, 340, COLOR_CYAN);
        for (int p = 0; p < 8; p++) {
          float a = (float)p * 0.785f + (float)animFrame * 0.1f;
          int px = 120 + (int)(cosf(a) * 45.0f);
          int py = 60 + (int)(sinf(a) * 35.0f);
          sprite.fillCircle(px, py, 3, (p % 2 == 0) ? COLOR_PINK : COLOR_YELLOW);
        }
      }
      break;
    }

    case PetMood::NUDGE: {
      // Mắt cún long lanh nũng nịu đòi xoa đầu
      sprite.fillRoundRect(leftEyeX - 28, eyeY - 45, 56, 90, 28, COLOR_CYAN);
      sprite.fillRoundRect(rightEyeX - 28, eyeY - 45, 56, 90, 28, COLOR_CYAN);
      // Ánh sáng lấp lánh trong mắt
      sprite.fillCircle(leftEyeX + 8, eyeY - 24, 12, COLOR_WHITE);
      sprite.fillCircle(leftEyeX - 10, eyeY + 12, 7, COLOR_WHITE);
      sprite.fillCircle(leftEyeX + 12, eyeY + 18, 4, COLOR_WHITE);
      sprite.fillCircle(rightEyeX + 8, eyeY - 24, 12, COLOR_WHITE);
      sprite.fillCircle(rightEyeX - 10, eyeY + 12, 7, COLOR_WHITE);
      sprite.fillCircle(rightEyeX + 12, eyeY + 18, 4, COLOR_WHITE);
      // Má hồng
      sprite.fillCircle(leftEyeX - 14, eyeY + 36, 8, COLOR_PINK);
      sprite.fillCircle(rightEyeX + 14, eyeY + 36, 8, COLOR_PINK);
      break;
    }

    case PetMood::COOL_GLASSES: {
      // Kính đen Thug Life cực ngầu
      sprite.fillRect(38, eyeY - 18, 164, 36, COLOR_WHITE);
      sprite.fillRect(40, eyeY - 16, 160, 32, COLOR_BG);
      sprite.drawLine(60, eyeY - 12, 80, eyeY + 12, COLOR_WHITE);
      sprite.drawLine(150, eyeY - 12, 170, eyeY + 12, COLOR_WHITE);
      break;
    }
  }

  sprite.pushSprite(0, 0);
}

} // namespace pet_emotions
