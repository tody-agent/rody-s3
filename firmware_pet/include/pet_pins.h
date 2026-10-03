#pragma once
#include <stdint.h>

namespace pet_pins {
// I2C Bus: Chia sẻ chung giữa PCA9685 (0x40) và Gia tốc kế MPU6050 (0x68)
constexpr int I2C_SDA = 8;
constexpr int I2C_SCL = 9;

// Cảm biến Gia tốc MPU6050
constexpr uint8_t MPU6050_ADDR = 0x68;

// Cảm biến Chạm (Touch Sensor trên đỉnh đầu / trán)
// GPIO 2: Tương thích cả module rời TTP223 (digitalRead) và cảm ứng điện dung tích hợp (touchRead)
constexpr int TOUCH_PIN = 2;

// Cảm biến Line / Tachometer IR
constexpr int IR_L = 10;
constexpr int IR_R = 11;

// I2S Microphone (INMP441)
constexpr int MIC_SCK = 4;
constexpr int MIC_WS  = 5;
constexpr int MIC_SD  = 6;

// I2S Speaker Amplifier (MAX98357A)
constexpr int SPK_DIN  = 7;
constexpr int SPK_LRC  = 15;
constexpr int SPK_BCLK = 16;

// TFT 1.54" ST7789 SPI Display (240x240)
constexpr int TFT_SCLK = 42;
constexpr int TFT_MOSI = 41;
constexpr int TFT_DC   = 40;
constexpr int TFT_RST  = 39;
constexpr int TFT_CS   = 38;
constexpr int TFT_BLK  = 21;

// Ultrasonic / Laser
constexpr int US_TRIG = 12;
constexpr int US_ECHO = 13;

// Pin, Button, LED
constexpr int BAT_ADC = 1;  // ADC1
constexpr int BTN     = 0;  // BOOT button
constexpr int RGB     = 48; // WS2812 onboard
// PCA9685 & Peripherals
constexpr uint8_t PET_PCA_ADDR = 0x40;
constexpr uint8_t PET_CH_L = 0;
constexpr uint8_t PET_CH_R = 1;
constexpr float   PET_BAT_DIV = 5.0f;
constexpr float   PET_BAT_LOW_V = 3.4f;
constexpr uint8_t PET_TACH_EDGES_PER_REV = 8;
constexpr int     PET_OBSTACLE_CM = 20;
}
