#include "store.h"
#include "oled_diag.h"
#include "../include/debug_log.h"
#include "../include/display_policy.h"
#include <Arduino.h>
#include <Preferences.h>
#include <string.h>
#ifdef ESP32
#include <Wire.h>
#include "../include/pins.h"
#endif

namespace store {
CalBlob gCal;
bool gCalValid = false;

static Preferences prefs;
static DisplayType gDisplayType = DisplayType::ST7735_096;
static display_policy::Source gDisplaySource = display_policy::Source::AutoDefault;
static int gOledSda = 8;
static int gOledScl = 9;
static uint8_t gOledAddr = 0x3C;
static bool gOledDetected = false;

uint32_t computeCrc(const CalBlob& blob) {

  // CRC-32 (IEEE 802.3)
  const uint8_t* data = reinterpret_cast<const uint8_t*>(&blob);
  size_t length = sizeof(CalBlob) - sizeof(uint32_t); // exclude crc field itself
  uint32_t crc = 0xFFFFFFFF;
  for (size_t i = 0; i < length; i++) {
    crc ^= data[i];
    for (int j = 0; j < 8; j++) {
      if (crc & 1) {
        crc = (crc >> 1) ^ 0xEDB88320;
      } else {
        crc >>= 1;
      }
    }
  }
  return ~crc;
}

void reset() {
  memset(&gCal, 0, sizeof(CalBlob));
  gCal.ver = CAL_VERSION;
  gCal.wheelbase_cm = 10.0f;
  gCal.l.db_lo = 1500;
  gCal.l.db_hi = 1500;
  gCal.l.fwd_sign = 0;
  gCal.l.trim = 1.0f;
  gCal.r.db_lo = 1500;
  gCal.r.db_hi = 1500;
  gCal.r.fwd_sign = 0;
  gCal.r.trim = 1.0f;
  gCal.crc = computeCrc(gCal);
  gCalValid = false;
}

void init() {
  reset();
  load();

  gOledDetected = false;
  bool spiHeader = false;
#ifdef ESP32
  // Auto-scan all candidate GPIO pins for I2C OLED (0x3C / 0x3D)
  auto scan = oled_diag::scanAllCandidatePins();
  if (scan.found) {
    gOledDetected = true;
    gOledAddr = scan.address;
    gOledSda = scan.sdaPin;
    gOledScl = scan.sclPin;
  } else {
    // Fallback to standard I2C pins
    gOledDetected = false;
    gOledAddr = 0x3C;
    gOledSda = pins::I2C_SDA;
    gOledScl = pins::I2C_SCL;
  }

  // Ensure TFT SPI pins are cleanly reset and detached from any peripheral
  gpio_reset_pin((gpio_num_t)pins::TFT_MOSI);
  gpio_reset_pin((gpio_num_t)pins::TFT_SCLK);
  gpio_reset_pin((gpio_num_t)pins::TFT_DC);
  gpio_reset_pin((gpio_num_t)pins::TFT_RST);
  gpio_reset_pin((gpio_num_t)pins::TFT_CS);
  gpio_reset_pin((gpio_num_t)pins::TFT_BLK);
  spiHeader = oled_diag::spiHeaderConnected();
#endif

  // Saved display id wins. OLED detection only supplies the first-boot default.
  static_assert((int)DisplayType::SSD1306_096 == (int)display_policy::Type::SSD1306_096, "oled id");
  static_assert((int)DisplayType::SSD1306_096_SPI == (int)display_policy::Type::SSD1306_096_SPI, "oled spi id");
  Preferences p;
  if (p.begin("rody", false)) {
    if (gOledDetected) {
      p.putUChar("oled_sda", (uint8_t)gOledSda);
      p.putUChar("oled_scl", (uint8_t)gOledScl);
      p.putUChar("oled_addr", gOledAddr);
    } else {
      uint8_t sda = p.getUChar("oled_sda", 255);
      uint8_t scl = p.getUChar("oled_scl", 255);
      uint8_t addr = p.getUChar("oled_addr", 0);
      if (sda != 255 && scl != 255 && addr != 0) {
        gOledSda = sda;
        gOledScl = scl;
        gOledAddr = addr;
      }
    }
    uint8_t saved = p.getUChar("disp_type", 255);
    display_policy::Choice choice = display_policy::resolveChoice(saved, gOledDetected, spiHeader);
    gDisplayType = (DisplayType)choice.type;
    gDisplaySource = choice.source;
    if ((uint8_t)gDisplayType != saved) {
      p.putUChar("disp_type", (uint8_t)gDisplayType);
    }
    p.end();
  } else {
    display_policy::Choice choice = display_policy::resolveChoice(255, gOledDetected, spiHeader);
    gDisplayType = (DisplayType)choice.type;
    gDisplaySource = choice.source;
  }

  Serial.printf("# [store] Display type: %s (%d) source=%s spi_header=%s [OLED on I2C: %s at 0x%02X, SDA=%d, SCL=%d]\n",
                getDisplayTypeName(gDisplayType), (int)gDisplayType, getDisplaySourceName(),
                spiHeader ? "YES" : "NO",
                gOledDetected ? "YES" : "NO", gOledAddr, gOledSda, gOledScl);
  char bootLine[96];
  snprintf(bootLine, sizeof(bootLine), "Display %s source %s OLED %s SDA %d SCL %d",
           getDisplayTypeName(gDisplayType), getDisplaySourceName(),
           gOledDetected ? "ACK" : "none", gOledSda, gOledScl);
  debug_log::push(bootLine);
}

bool load() {
  bool opened = prefs.begin("rody", false);
  if (!opened || prefs.getBytesLength("cal") != sizeof(CalBlob)) {
    if (opened) prefs.end();
    // Fallback to legacy "otto" namespace if present
    opened = prefs.begin("otto", false);
  }
  if (!opened) {
    Serial.println("# [store] Failed to open NVS namespace");
    gCalValid = false;
    return false;
  }
  size_t len = prefs.getBytesLength("cal");
  if (len != sizeof(CalBlob)) {
    prefs.end();
    Serial.println("# [store] No valid calibration blob found in NVS (first boot)");
    gCalValid = false;
    return false;
  }
  CalBlob temp;
  prefs.getBytes("cal", &temp, sizeof(CalBlob));
  prefs.end();

  if (temp.ver != CAL_VERSION) {
    Serial.println("# [store] Calibration version mismatch");
    gCalValid = false;
    return false;
  }
  uint32_t expectedCrc = computeCrc(temp);
  if (temp.crc != expectedCrc) {
    Serial.println("# [store] Calibration CRC mismatch");
    gCalValid = false;
    return false;
  }
  gCal = temp;
  gCalValid = true;
  Serial.println("# [store] Calibration loaded successfully from NVS");
  return true;
}

bool save() {
  gCal.ver = CAL_VERSION;
  gCal.crc = computeCrc(gCal);
  if (!prefs.begin("rody", false)) {
    Serial.println("# [store] Failed to open NVS namespace read-write");
    return false;
  }
  size_t written = prefs.putBytes("cal", &gCal, sizeof(CalBlob));
  prefs.end();
  if (written == sizeof(CalBlob)) {
    gCalValid = true;
    Serial.println("# [store] Calibration saved to NVS successfully");
    return true;
  }
  Serial.println("# [store] Failed to write complete calibration blob to NVS");
  return false;
}

DisplayType getDisplayType() {
  return gDisplayType;
}

bool setDisplayType(DisplayType type) {
  Preferences p;
  if (!p.begin("rody", false)) {
    Serial.println("# [store] Failed to open NVS to write disp_type");
    return false;
  }
  p.putUChar("disp_type", (uint8_t)type);
  uint8_t stored = p.getUChar("disp_type", 255);
  p.end();
  if (stored != (uint8_t)type) {
    Serial.println("# [store] disp_type readback mismatch");
    return false;
  }
  gDisplayType = type;
  gDisplaySource = display_policy::Source::Saved;
  Serial.printf("# [store] Display type saved to NVS: %s (%d)\n", getDisplayTypeName(type), (int)type);
  return true;
}

const char* getDisplaySourceName() {
  return display_policy::sourceName(gDisplaySource);
}

const char* getDisplayTypeName(DisplayType type) {
  switch (type) {
    case DisplayType::ST7789_154:  return "1.54\" ST7789 (240x240)";
    case DisplayType::GC9A01_128:  return "1.28\" GC9A01 (240x240 Round)";
    case DisplayType::ST7735_180:  return "1.8\" ST7735 (160x128)";
    case DisplayType::ST7735_096:      return "0.96\" ST7735 (160x80 IPS)";
    case DisplayType::SSD1306_096:     return "0.96\" SSD1306 (128x64 OLED I2C)";
    case DisplayType::SSD1306_096_SPI: return "0.96\" SSD1306 (128x64 OLED SPI)";
    default:                           return "Unknown";

  }
}

bool isOledDetected() {
  return gOledDetected;
}

int getOledSdaPin() {
  return gOledSda;
}

int getOledSclPin() {
  return gOledScl;
}

uint8_t getOledAddr() {
  return gOledAddr;
}

void setOledConfig(int sda, int scl, uint8_t addr) {
  gOledSda = sda;
  gOledScl = scl;
  gOledAddr = addr;
  gOledDetected = true;
  Preferences p;
  if (!p.begin("rody", false)) return;
  p.putUChar("oled_sda", (uint8_t)sda);
  p.putUChar("oled_scl", (uint8_t)scl);
  p.putUChar("oled_addr", addr);
  p.end();
}

}

