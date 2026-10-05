#pragma once
#include <stddef.h>
#include <stdint.h>
#include "pins.h"

namespace display_policy {

enum class Type : uint8_t {
  ST7789_154 = 0,
  GC9A01_128 = 1,
  ST7735_180 = 2,
  ST7735_096 = 3,
  SSD1306_096 = 4,
  SSD1306_096_SPI = 5
};

enum class OledSeq : uint8_t { BringUp, AllPixelsOn };

struct PinPair {
  int sda;
  int scl;
};

struct I2cFrame {
  uint8_t data[16];
  uint8_t len;
};

enum class Source : uint8_t { Saved, AutoOled, AutoDefault, AutoSpi };

struct Choice {
  Type type;
  Source source;
};

// A stored id (0..5) wins, except a stale I2C OLED choice (id 4) when the
// command probe failed and the SPI display header is powered.
inline Choice resolveChoice(uint8_t saved_raw, bool oled_detected, bool spi_header = false) {
  Choice choice;
  if (saved_raw == (uint8_t)Type::SSD1306_096 && !oled_detected && spi_header) {
    choice.type = Type::SSD1306_096_SPI;
    choice.source = Source::AutoSpi;
    return choice;
  }
  if (saved_raw <= (uint8_t)Type::SSD1306_096_SPI) {
    choice.type = (Type)saved_raw;
    choice.source = Source::Saved;
    return choice;
  }
  if (oled_detected) {
    choice.type = Type::SSD1306_096;
    choice.source = Source::AutoOled;
  } else {
    choice.type = Type::ST7735_096;
    choice.source = Source::AutoDefault;
  }
  return choice;
}

inline Type resolveType(uint8_t saved_raw, bool oled_detected, bool spi_header = false) {
  return resolveChoice(saved_raw, oled_detected, spi_header).type;
}

// At least four of MOSI, SCLK, DC, RST, CS pulled high means a powered SPI module.
inline bool spiHeaderPresent(bool mosi, bool sclk, bool dc, bool rst, bool cs) {
  int pulled = (mosi ? 1 : 0) + (sclk ? 1 : 0) + (dc ? 1 : 0) + (rst ? 1 : 0) + (cs ? 1 : 0);
  return pulled >= 4;
}

inline bool isValidId(int id) {
  return id >= 0 && id <= (int)Type::SSD1306_096_SPI;
}

inline const char* sourceName(Source source) {
  switch (source) {
    case Source::Saved: return "saved";
    case Source::AutoOled: return "auto_oled";
    case Source::AutoDefault: return "auto_default";
    case Source::AutoSpi: return "auto_spi";
  }
  return "auto_default";
}

// Address ACK is not enough: the controller must accept a command,
// and a foreign address must NACK (rejects a stuck-ACK bus).
inline bool acceptOledProbe(bool addr_ack, bool cmd_ack, bool foreign_ack) {
  return addr_ack && cmd_ack && !foreign_ack;
}

// Silk on the display header is SDA=IO41, SCL=IO42. Try that before the swap.
inline PinPair oledPinCandidate(int index) {
  switch (index) {
    case 0: return {pins::TFT_MOSI, pins::TFT_SCLK};
    case 1: return {pins::TFT_SCLK, pins::TFT_MOSI};
    case 2: return {pins::I2C_SDA, pins::I2C_SCL};
    case 3: return {pins::I2C_SCL, pins::I2C_SDA};
    default: return {-1, -1};
  }
}

enum { kOledPinCandidateCount = 4 };

// On-demand scan: known headers only, both directions. No I2S pins.
inline PinPair oledHeaderCandidate(int index) {
  switch (index) {
    case 0: return {pins::I2C_SDA, pins::I2C_SCL};
    case 1: return {pins::I2C_SCL, pins::I2C_SDA};
    case 2: return {pins::TFT_MOSI, pins::TFT_SCLK};
    case 3: return {pins::TFT_SCLK, pins::TFT_MOSI};
    case 4: return {pins::IR_L, pins::IR_R};
    case 5: return {pins::IR_R, pins::IR_L};
    case 6: return {pins::US_TRIG, pins::US_ECHO};
    case 7: return {pins::US_ECHO, pins::US_TRIG};
    default: return {-1, -1};
  }
}

enum { kOledHeaderCandidateCount = 8 };

struct Cmd {
  uint8_t n;
  uint8_t b[3];
};

inline const Cmd* bringUpCommands(size_t* count) {
  static const Cmd k[] = {
    {1, {0xAE, 0, 0}},
    {2, {0xD5, 0x80, 0}},
    {2, {0xA8, 0x3F, 0}},
    {2, {0xD3, 0x00, 0}},
    {1, {0x40, 0, 0}},
    {2, {0x8D, 0x14, 0}},
    {2, {0xAD, 0x8B, 0}},
    {2, {0x20, 0x00, 0}},
    {1, {0xA1, 0, 0}},
    {1, {0xC8, 0, 0}},
    {2, {0xDA, 0x12, 0}},
    {2, {0x81, 0xFF, 0}},
    {2, {0xD9, 0xF1, 0}},
    {2, {0xDB, 0x40, 0}},
    {1, {0xA4, 0, 0}},
    {1, {0xA6, 0, 0}},
    {1, {0xAF, 0, 0}},
  };
  *count = sizeof(k) / sizeof(k[0]);
  return k;
}

inline size_t buildOledFrames(OledSeq seq, I2cFrame* out, size_t max_out) {
  if (out == nullptr || max_out == 0) return 0;
  if (seq == OledSeq::AllPixelsOn) {
    out[0].data[0] = 0x00;
    out[0].data[1] = 0xA5;
    out[0].len = 2;
    return 1;
  }
  size_t count = 0;
  const Cmd* cmds = bringUpCommands(&count);
  size_t n = 0;
  for (size_t i = 0; i < count && n < max_out; i++) {
    out[n].data[0] = 0x00;
    for (uint8_t b = 0; b < cmds[i].n; b++) out[n].data[1 + b] = cmds[i].b[b];
    out[n].len = (uint8_t)(1 + cmds[i].n);
    n++;
  }
  return n;
}

}
