#pragma once
#include <stdint.h>

namespace pins {
// I2C Bus for PCA9685 Motor Driver
constexpr int I2C_SDA = 8,  I2C_SCL = 9;

// Cảm biến Line / Tachometer IR (chuyển sang 10 và 11 để nhường 6 và 7 cho I2S Audio)
constexpr int IR_L    = 10, IR_R    = 11;

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

// Ultrasonic / Laser (Dự phòng)
constexpr int US_TRIG = 12, US_ECHO = 13;

constexpr int BAT_ADC = 1;                 // ADC1
constexpr int BTN     = 0;                 // BOOT, input only
constexpr int RGB     = 48;                // WS2812 onboard
}

constexpr uint8_t PCA_ADDR = 0x40, CH_L = 0, CH_R = 1;
constexpr uint8_t MPU6050_ADDR = 0x68, MPU_ADDR_ALT = 0x69, AK8963_MAG_ADDR = 0x0C;
constexpr float   BAT_DIV = 2.0f, BAT_LOW_V = 3.4f;
constexpr uint8_t TACH_EDGES_PER_REV = 8;
constexpr int     OBSTACLE_CM = 20;
