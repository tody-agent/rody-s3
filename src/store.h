#pragma once
#include <stdint.h>
#include "calib.h"

struct CalBlob {
  uint16_t ver;
  calib::Wheel l;
  calib::Wheel r;
  float wheelbase_cm;
  uint32_t crc;
};

namespace store {
constexpr uint16_t CAL_VERSION = 1;
extern CalBlob gCal;
extern bool gCalValid;

enum class DisplayType : uint8_t {
  ST7789_154 = 0,      // 1.54" ST7789 240x240 (Vuông - Mặc định)
  GC9A01_128 = 1,      // 1.28" GC9A01 240x240 (Tròn - Circular)
  ST7735_180 = 2,      // 1.8"  ST7735  160x128 (Chữ nhật - Landscape)
  ST7735_096 = 3,      // 0.96" ST7735  160x80  (IPS Mini - Landscape)
  SSD1306_096 = 4,     // 0.96" SSD1306 128x64  (OLED I2C 4-pin)
  SSD1306_096_SPI = 5  // 0.96" SSD1306 128x64  (OLED SPI 7-pin)
};


void init();
bool load();
bool save();
void reset();
uint32_t computeCrc(const CalBlob& blob);

DisplayType getDisplayType();
bool setDisplayType(DisplayType type);
const char* getDisplayTypeName(DisplayType type);
const char* getDisplaySourceName();

// OLED Auto-Detection Info
bool isOledDetected();
int getOledSdaPin();
int getOledSclPin();
uint8_t getOledAddr();
void setOledConfig(int sda, int scl, uint8_t addr);
}

