#pragma once
#include <stdint.h>
namespace pins {
constexpr int I2C_SDA = 8,  I2C_SCL = 9;
constexpr int US_TRIG = 4,  US_ECHO = 5;
constexpr int IR_L    = 6,  IR_R    = 7;   // line sensor / tachometer
constexpr int BUZZER  = 10;
constexpr int BAT_ADC = 1;                 // ADC1
constexpr int BTN     = 0;                 // BOOT, input only
constexpr int RGB     = 48;                // verify: some boards use 38
}
constexpr uint8_t PCA_ADDR = 0x40, CH_L = 0, CH_R = 1;
constexpr float   BAT_DIV = 5.0f, BAT_LOW_V = 3.4f;
constexpr uint8_t TACH_EDGES_PER_REV = 8;
constexpr int     OBSTACLE_CM = 20;
