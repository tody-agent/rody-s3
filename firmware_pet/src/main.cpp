#include <Arduino.h>
#include "../include/pet_pins.h"
#include "imu_sensor.h"
#include "touch_sensor.h"
#include "acoustic_monitor.h"
#include "pet_emotions.h"
#include "pet_audio.h"
#include "pet_brain.h"

void setup() {
  Serial.begin(115200);
  delay(200);

  Serial.println("# =========================================");
  Serial.println("# [boot] Rody S3 Pet Edition (v0.3.0-pet)");
  Serial.println("# [boot] IMU 6-DOF + Capacitive Touch Active");
  Serial.println("# =========================================");

  // 1. Màn hình ST7789 60 FPS
  pet_emotions::init();
  pet_emotions::setMood(pet_emotions::PetMood::IDLE);

  // 2. Âm thanh MAX98357A I2S1
  pet_audio::init();
  pet_audio::play(pet_audio::PetSound::BOOT_HELLO);

  // 3. Gia tốc kế IMU (MPU6050 / GY-6500 / GY-9250) I2C (GPIO 8 & 9)
  if (!imu_sensor::init()) {
    Serial.println("# [boot] Warning: IMU (MPU6050/6500/9250) not detected. Continuing with Touch & Audio.");
  }

  // 4. Cảm biến chạm GPIO 2
  touch_sensor::init(touch_sensor::TouchMode::DIGITAL_TTP223);

  // 5. Cảm biến âm thanh INMP441 I2S0
  acoustic_monitor::init();

  // 6. Bộ não thú cưng Pet Brain
  pet_brain::init();

  Serial.println("# [boot] Rody S3 Pet Edition Ready! Pet me or shake me!");
}

void loop() {
  // Cập nhật cảm biến
  imu_sensor::update();
  touch_sensor::update();
  acoustic_monitor::update();

  // Xử lý bộ não & máy trạng thái phản xạ
  pet_brain::update();

  // Vẽ chuyển động mắt 60 FPS
  pet_emotions::update();

  // Lắng nghe lệnh Serial CLI điều khiển / test nhanh
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd == "stats") {
      const auto& st = pet_brain::getStats();
      Serial.printf("{\"cmd\":\"stats\",\"ok\":true,\"mood\":\"%s\",\"affection\":%d,\"stress\":%d,\"energy\":%d}\n",
                    pet_emotions::getMoodStr(st.currentMood), st.affection, st.stress, st.energy);
    } else if (cmd == "ping") {
      Serial.println("{\"cmd\":\"ping\",\"ok\":true,\"fw\":\"0.3.0-pet\"}");
    } else if (cmd.length() > 0) {
      pet_brain::simulateEvent(cmd.c_str());
      const auto& st = pet_brain::getStats();
      Serial.printf("{\"cmd\":\"sim\",\"event\":\"%s\",\"mood\":\"%s\",\"ok\":true}\n",
                    cmd.c_str(), pet_emotions::getMoodStr(st.currentMood));
    }
  }
}
