#pragma once
#include "pins.h"
#include <stddef.h>
#include <stdint.h>

namespace pin_catalog {

enum class Bus : uint8_t {
  GpioIn = 0,
  Gpio = 1,
  I2c = 2,
  I2s = 3,
  Spi = 4,
  Adc = 5,
  Led = 6
};

struct Entry {
  int gpio;
  const char* name;
  Bus bus;
};

inline const Entry* entries(size_t* count) {
  static const Entry kRows[] = {
    {pins::I2C_SDA, "I2C SDA", Bus::I2c},
    {pins::I2C_SCL, "I2C SCL", Bus::I2c},
    {pins::IR_L, "IR trai", Bus::GpioIn},
    {pins::IR_R, "IR phai", Bus::GpioIn},
    {pins::MIC_SCK, "Mic SCK", Bus::I2s},
    {pins::MIC_WS, "Mic WS", Bus::I2s},
    {pins::MIC_SD, "Mic SD", Bus::I2s},
    {pins::SPK_DIN, "Loa DIN", Bus::I2s},
    {pins::SPK_LRC, "Loa LRC", Bus::I2s},
    {pins::SPK_BCLK, "Loa BCLK", Bus::I2s},
    {pins::TFT_SCLK, "TFT SCLK", Bus::Spi},
    {pins::TFT_MOSI, "TFT MOSI", Bus::Spi},
    {pins::TFT_DC, "TFT DC", Bus::Spi},
    {pins::TFT_RST, "TFT RST", Bus::Spi},
    {pins::TFT_CS, "TFT CS", Bus::Spi},
    {pins::TFT_BLK, "TFT BLK", Bus::Spi},
    {pins::US_TRIG, "Sieu am TRIG", Bus::Gpio},
    {pins::US_ECHO, "Sieu am ECHO", Bus::Gpio},
    {pins::BAT_ADC, "ADC pin", Bus::Adc},
    {pins::BTN, "Nut BOOT", Bus::GpioIn},
    {pins::RGB, "LED RGB", Bus::Led},
  };
  if (count) *count = sizeof(kRows) / sizeof(kRows[0]);
  return kRows;
}

inline const Entry* find(int gpio) {
  size_t count = 0;
  const Entry* rows = entries(&count);
  for (size_t i = 0; i < count; i++) {
    if (rows[i].gpio == gpio) return &rows[i];
  }
  return nullptr;
}

inline const char* busName(Bus bus) {
  switch (bus) {
    case Bus::GpioIn: return "gpio_in";
    case Bus::Gpio: return "gpio";
    case Bus::I2c: return "i2c";
    case Bus::I2s: return "i2s";
    case Bus::Spi: return "spi";
    case Bus::Adc: return "adc";
    case Bus::Led: return "led";
  }
  return "gpio";
}

inline bool readsLevel(Bus bus) {
  return bus == Bus::GpioIn || bus == Bus::Gpio;
}

}
