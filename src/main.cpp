#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "../include/pins.h"
#include "store.h"
#include "sensors.h"
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
  delay(100);
  Serial.println("# =========================================");
  Serial.println("# [boot] Rody S3 (ESP32-S3 N16R8) Initializing");
  Serial.println("# [boot] Mochi/Xiaozhi TFT + I2S Audio Active");
  Serial.println("# =========================================");

  // Status LED
  statusLed.begin();
  statusLed.setBrightness(40);
  setLedColor(0, 50, 200); // Blue during boot

  // Mochi / Xiaozhi TFT Display
  emotion_gfx::init();
  emotion_gfx::setEmotion(emotion_gfx::Emotion::HAPPY);

  // Audio Output (MAX98357A)
  audio_player::init();
  audio_player::playSfx(audio_player::SoundEffect::BOOT);

  // Audio Input (INMP441)
  voice_control::init();

  // NVS Storage
  store::init();

  // Sensors & Tachometer
  sensors::init();

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
}

void loop() {
  console::process();
  drive::update();
  behaviors::update();
  web::update();

  // Update TFT graphics and audio
  emotion_gfx::update();
  audio_player::update();
  voice_control::update();

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
