#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "../include/pins.h"
#include "store.h"
#include "sensors.h"
#include "imu_sensor.h"
#include "drive.h"
#include "console.h"
#include "behaviors.h"
#include "web.h"
#include "emotion_gfx.h"
#include "audio_player.h"
#include "voice_control.h"

static Adafruit_NeoPixel statusLed(1, pins::RGB, NEO_GRB + NEO_KHZ800);

static void setLedColor(uint8_t r, uint8_t g, uint8_t b) {
  statusLed.setPixelColor(0, statusLed.Color(r, g, b));
  statusLed.show();
}

void setup() {
  console::init();
#if ARDUINO_USB_CDC_ON_BOOT
  Serial.setTxTimeoutMs(10);
#endif
  delay(150);

  Serial.println("# =========================================");
  Serial.println("# [boot] Rody S3 (ESP32-S3 N16R8) Initializing");
  Serial.println("# [boot] Multi-Display Auto-Detection & Self-Test Active");
  Serial.println("# =========================================");
  Serial.flush();

  // Status LED
  statusLed.begin();
  statusLed.setBrightness(50);
  setLedColor(0, 100, 255); // Cyan-Blue during boot

  // NVS Storage & OLED Auto-Probe
  store::init();

  // Screen Hardware Initialization & Visual Splash
  emotion_gfx::init();
  emotion_gfx::setEmotion(emotion_gfx::Emotion::HAPPY);
  setLedColor(0, 255, 60); // Bright Green: Screen initialized!

  // Audio Output (MAX98357A)
  audio_player::init();
  audio_player::playSfx(audio_player::SoundEffect::BOOT);


  // Audio Input (INMP441)
  voice_control::init();

  // Sensors & Tachometer
  sensors::init();

  // IMU Accelerometer / Gyro (MPU6050 / GY-6500 / GY-9250)
  imu_sensor::init();
  if (imu_sensor::isAvailable()) {
    Serial.printf("# [boot] IMU Active: %s at 0x%02X\n", imu_sensor::getChipName(), imu_sensor::getActiveAddress());
  } else {
    Serial.println("# [boot] IMU: Not detected (optional)");
  }

  // PCA9685 Motor Driver
  drive::init();

  // Behaviors
  behaviors::init();

  // Web Server & SoftAP
  web::init();

  // Check battery
  float v = sensors::readBatteryVoltage();
  if (v < 1.0f) {
    Serial.println("# [boot] Battery monitor: Not detected / minimal setup (bypass enabled)");
  } else {
    Serial.printf("# [boot] Battery Voltage: %.2f V\n", v);
  }

  if (sensors::isBatteryLow()) {
    Serial.println("# [boot] WARNING: Battery voltage low (< 3.4V)!");
    setLedColor(200, 0, 0); // Red
  } else if (store::gCalValid) {
    Serial.println("# [boot] Calibration is VALID. Ready to drive.");
    setLedColor(0, 200, 50); // Green
  } else {
    Serial.println("# [boot] Uncalibrated. Please run calibration procedures.");
    setLedColor(200, 150, 0); // Amber
  }

  Serial.println("# [boot] Setup complete. Listening for commands...");
  Serial.flush();
}

void loop() {
  console::process();
  imu_sensor::update();
  drive::update();
  behaviors::update();
  web::update();

  // Update TFT graphics and audio
  emotion_gfx::update();
  audio_player::update();
  voice_control::update();

  // Periodic heartbeat log every 3s
  static uint32_t lastHb = 0;
  if (millis() - lastHb > 3000) {
    lastHb = millis();
    Serial.printf("# [hb] Active Display: %s (%dx%d), Heap=%u\n",
                  store::getDisplayTypeName(store::getDisplayType()),
                  emotion_gfx::getWidth(), emotion_gfx::getHeight(),
                  (unsigned int)ESP.getFreeHeap());
    Serial.flush();
  }

  // Transition from boot HAPPY to IDLE after 3.5s
  static bool bootHappyDone = false;
  if (!bootHappyDone && millis() > 3500) {
    bootHappyDone = true;
    if (emotion_gfx::getEmotion() == emotion_gfx::Emotion::HAPPY) {
      emotion_gfx::setEmotion(emotion_gfx::Emotion::IDLE);
    }
  }

  // Update LED status periodically
  static uint32_t lastLedCheck = 0;
  if (millis() - lastLedCheck > 1000) {
    lastLedCheck = millis();
    if (sensors::isBatteryLow()) {
      setLedColor(200, 0, 0); // Red: Low battery
    } else if (drive::isMoving()) {
      setLedColor(255, 120, 0); // Orange: Moving
    } else if (store::gCalValid) {
      setLedColor(0, 180, 40); // Green: Calibrated idle
    } else {
      setLedColor(100, 100, 0); // Yellow/Amber: Uncalibrated idle
    }
  }

  delay(2); // Yield to FreeRTOS scheduler
}
