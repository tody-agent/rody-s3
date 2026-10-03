#include "pet_brain.h"
#include "imu_sensor.h"
#include "touch_sensor.h"
#include "acoustic_monitor.h"
#include "pet_emotions.h"
#include "pet_audio.h"
#include "../include/pet_pins.h"
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

namespace pet_brain {

static PetStats stats = {
  .affection = 60,
  .stress = 0,
  .energy = 90,
  .currentMood = pet_emotions::PetMood::IDLE
};

static Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(pet_pins::PET_PCA_ADDR);
static bool pcaReady = false;

static uint32_t stateEnterTime = 0;
static uint32_t lastDecayTime = 0;
static uint32_t lastPurrSoundTime = 0;
static uint32_t lastBellyAlarmTime = 0;

static void setMotors(float leftSpeed, float rightSpeed) {
  if (!pcaReady) return;
  // Convert -100..100 to pulse 1000..2000 us (where ~1500 is stop)
  int lPulse = (int)(1500 + leftSpeed * 4.0f);
  int rPulse = (int)(1500 - rightSpeed * 4.0f);
  if (lPulse < 1000) lPulse = 1000; if (lPulse > 2000) lPulse = 2000;
  if (rPulse < 1000) rPulse = 1000; if (rPulse > 2000) rPulse = 2000;

  pca.writeMicroseconds(pet_pins::PET_CH_L, lPulse);
  pca.writeMicroseconds(pet_pins::PET_CH_R, rPulse);
}

static void stopMotors() {
  if (!pcaReady) return;
  pca.writeMicroseconds(pet_pins::PET_CH_L, 1500);
  pca.writeMicroseconds(pet_pins::PET_CH_R, 1500);
}

void init() {
  Wire.begin(pet_pins::I2C_SDA, pet_pins::I2C_SCL, 400000);
  pcaReady = pca.begin();
  if (pcaReady) {
    pca.setPWMFreq(50);
    stopMotors();
    Serial.println("# [pet_brain] PCA9685 motor driver ready");
  } else {
    Serial.println("# [pet_brain] PCA9685 not found (running in sensor-only mode)");
  }

  stats.affection = 65;
  stats.stress = 0;
  stats.energy = 90;
  stats.currentMood = pet_emotions::PetMood::IDLE;
  pet_emotions::setMood(pet_emotions::PetMood::IDLE);
  stateEnterTime = millis();
  lastDecayTime = millis();
}

static void transitionTo(pet_emotions::PetMood newMood) {
  if (stats.currentMood == newMood) return;
  stats.currentMood = newMood;
  stateEnterTime = millis();
  pet_emotions::setMood(newMood);
  Serial.printf("# [pet_brain] Mood changed to: %s (Aff:%d, Str:%d, Ene:%d)\n",
                pet_emotions::getMoodStr(newMood), stats.affection, stats.stress, stats.energy);
}

void update() {
  uint32_t now = millis();
  const auto& imu = imu_sensor::getState();
  bool petting = touch_sensor::isPetting();
  bool loudNoise = acoustic_monitor::isLoudNoiseSustained();
  bool audioSpike = acoustic_monitor::checkAndClearAudioSpike();

  // Mood decay & natural settling every 1 second
  if (now - lastDecayTime >= 1000) {
    lastDecayTime = now;
    if (stats.stress > 0) stats.stress -= 1;
    if (stats.affection > 20 && !petting) stats.affection -= 1;
    if (stats.energy > 0 && stats.currentMood != pet_emotions::PetMood::SLEEPY) {
      if ((now / 1000) % 2 == 0) stats.energy -= 1;
    }

    // Tự động ngáp ngủ nếu bị bỏ rơi lâu và năng lượng thấp
    if (stats.energy <= 20 && stats.currentMood == pet_emotions::PetMood::IDLE && (now - stateEnterTime > 25000)) {
      transitionTo(pet_emotions::PetMood::SLEEPY);
      pet_audio::play(pet_audio::PetSound::SLEEPY_SNORE);
    }
  }

  // Tỉnh giấc khi đang ngủ nếu được xoa đầu hoặc có tiếng động/rung lắc
  if (stats.currentMood == pet_emotions::PetMood::SLEEPY) {
    if (petting || audioSpike || imu.isKnocked || imu.isShaking) {
      stats.energy = 85;
      transitionTo(pet_emotions::PetMood::HAPPY);
      pet_audio::play(pet_audio::PetSound::HAPPY_CHIRP);
      return;
    }
  }

  // =========================================================================
  // STATE MACHINE PRIORITY:
  // 1. BELLY UP (Highest emergency: inverted orientation)
  // 2. FALLEN / HURT (Impact or tilted on side)
  // 3. KNOCKED / STARTLED (Sudden shock or sharp sound)
  // 4. SHAKEN / DIZZY (Continuous rapid shaking)
  // 5. PETTING / PURRING (Loving contact on head)
  // 6. LOUD NOISE (Annoyed by continuous ambient noise)
  // 7. IDLE / SLEEPY (Default calm state)
  // =========================================================================

  // 1. BELLY UP (Lật ngửa bụng lên)
  if (imu.isBellyUp) {
    if (stats.currentMood != pet_emotions::PetMood::BELLY_UP) {
      transitionTo(pet_emotions::PetMood::BELLY_UP);
      stats.stress = (stats.stress + 35 > 100) ? 100 : stats.stress + 35;
      pet_audio::play(pet_audio::PetSound::BELLY_UP_ALARM);
      lastBellyAlarmTime = now;
    }
    // 2 Bánh xe quẫy đạp liên hồi để đòi lật lại
    float flail = (sinf((float)(now - stateEnterTime) * 0.015f) > 0) ? 40.0f : -40.0f;
    setMotors(flail, -flail);

    if (now - lastBellyAlarmTime > 1800) {
      pet_audio::play(pet_audio::PetSound::BELLY_UP_ALARM);
      lastBellyAlarmTime = now;
    }
    return;
  }

  // 2. FALLEN / HURT (Bị ngã / té)
  if (imu.isImpact || imu.isFallen) {
    stopMotors(); // Ngắt xung servo ngay lập tức bảo vệ cơ khí
    if (stats.currentMood != pet_emotions::PetMood::HURT) {
      transitionTo(pet_emotions::PetMood::HURT);
      stats.stress = (stats.stress + 30 > 100) ? 100 : stats.stress + 30;
      pet_audio::play(pet_audio::PetSound::CRY_HURT);
    }
    imu_sensor::clearTransientFlags();
    return;
  }

  // 3. KNOCKED / STARTLED (Bị gõ mạnh đột ngột)
  if (imu.isKnocked || (audioSpike && imu.totalAccel > 1.6f)) {
    if (stats.currentMood != pet_emotions::PetMood::STARTLED) {
      transitionTo(pet_emotions::PetMood::STARTLED);
      stats.stress = (stats.stress + 20 > 100) ? 100 : stats.stress + 20;
      pet_audio::play(pet_audio::PetSound::GASP_STARTLED);
      // Lùi giật mình 2cm
      setMotors(-30.0f, -30.0f);
      delay(120);
      stopMotors();
    }
    imu_sensor::clearTransientFlags();
    return;
  }

  // 4. SHAKEN / DIZZY (Bị lắc lắc liên tục)
  if (imu.isShaking) {
    stopMotors();
    if (stats.currentMood != pet_emotions::PetMood::SHAKEN_DIZZY) {
      transitionTo(pet_emotions::PetMood::SHAKEN_DIZZY);
      stats.stress = (stats.stress + 25 > 100) ? 100 : stats.stress + 25;
      pet_audio::play(pet_audio::PetSound::DIZZY_CHIRP);
    }
    return;
  }

  // 5. PETTING / PURRING (Được vuốt ve đầu)
  if (petting) {
    if (stats.currentMood != pet_emotions::PetMood::PURRING) {
      transitionTo(pet_emotions::PetMood::PURRING);
      stats.affection = (stats.affection + 15 > 100) ? 100 : stats.affection + 15;
      stats.stress = (stats.stress >= 20) ? stats.stress - 20 : 0;
      pet_audio::play(pet_audio::PetSound::PURR_CONTENT);
      lastPurrSoundTime = now;
    }
    // Lắc lư nhẹ nhàng thân xe sang 2 bên
    float wiggle = sinf((float)(now - stateEnterTime) * 0.008f) * 18.0f;
    setMotors(wiggle, -wiggle);

    if (now - lastPurrSoundTime > 2200) {
      pet_audio::play(pet_audio::PetSound::PURR_CONTENT);
      lastPurrSoundTime = now;
    }
    return;
  }

  // 6. CÙ LÉT NHỘT (Rapid multi-tap trên đầu)
  if (touch_sensor::checkAndClearTickle()) {
    transitionTo(pet_emotions::PetMood::TICKLE);
    stats.stress = (stats.stress >= 25) ? stats.stress - 25 : 0;
    stats.affection = (stats.affection + 15 > 100) ? 100 : stats.affection + 15;
    pet_audio::play(pet_audio::PetSound::TICKLE_GIGGLE);
    // Lắc hông giật giật
    setMotors(35.0f, -35.0f); delay(80);
    setMotors(-35.0f, 35.0f); delay(80);
    stopMotors();
    return;
  }

  // 7. VỖ TAY NHẢY DISCO (Clap-Clap qua Mic)
  if (acoustic_monitor::checkAndClearClap()) {
    transitionTo(pet_emotions::PetMood::DISCO);
    stats.energy = 100;
    stats.affection = (stats.affection + 20 > 100) ? 100 : stats.affection + 20;
    pet_audio::play(pet_audio::PetSound::DISCO_GROOVE);
    // Moonwalk lùi nhẹ rồi lắc lư
    setMotors(-30.0f, -30.0f); delay(150);
    setMotors(30.0f, -30.0f);  delay(120);
    stopMotors();
    return;
  }

  // 8. THỔI HƠI HẮT XÌ (Blowing air vào Mic)
  if (acoustic_monitor::isBlowingAir()) {
    transitionTo(pet_emotions::PetMood::SNEEZE);
    pet_audio::play(pet_audio::PetSound::SNEEZE_BURST);
    // Giật lùi vì hắt xì mạnh
    setMotors(-40.0f, -40.0f); delay(160);
    stopMotors();
    return;
  }

  // 9. NHẮC BỔNG BAY LƯỢN (Airplane mode trên IMU)
  if (imu.isAirplaneMode) {
    if (stats.currentMood != pet_emotions::PetMood::AIRPLANE) {
      transitionTo(pet_emotions::PetMood::AIRPLANE);
      pet_audio::play(pet_audio::PetSound::AIRPLANE_ENGINE);
    }
    // Bánh xe quay tít như cánh quạt
    setMotors(40.0f, 40.0f);
    return;
  }

  // 10. TRÒ CHƠI ĐẬP TAY (High-Five response)
  if (stats.currentMood == pet_emotions::PetMood::HIGH_FIVE) {
    if (touch_sensor::checkAndClearTap()) {
      // Đập tay thành công!
      transitionTo(pet_emotions::PetMood::COOL_GLASSES);
      stats.affection = (stats.affection + 25 > 100) ? 100 : stats.affection + 25;
      pet_audio::play(pet_audio::PetSound::VICTORY_FANFARE);
      // Xoay 360 độ ăn mừng
      setMotors(50.0f, -50.0f); delay(500);
      stopMotors();
      return;
    }
  }

  // 11. LOUD NOISE / ANNOYED (Môi trường quá ồn ào)
  if (loudNoise) {
    stopMotors();
    if (stats.currentMood != pet_emotions::PetMood::ANNOYED) {
      transitionTo(pet_emotions::PetMood::ANNOYED);
      stats.stress = (stats.stress + 15 > 100) ? 100 : stats.stress + 15;
      pet_audio::play(pet_audio::PetSound::ANNOYED_SIGH);
    }
    return;
  }

  // =========================================================================
  // RETURN TO IDLE / CALM
  // =========================================================================
  stopMotors();

  if (stats.currentMood == pet_emotions::PetMood::SHAKEN_DIZZY && (now - stateEnterTime > 2500)) {
    // Sau khi bị lắc, chuyển sang càu nhàu tức giận 2 giây rồi mới bình thường
    transitionTo(pet_emotions::PetMood::ANGRY);
    pet_audio::play(pet_audio::PetSound::GROWL_ANGRY);
  } else if ((stats.currentMood == pet_emotions::PetMood::STARTLED ||
              stats.currentMood == pet_emotions::PetMood::ANGRY ||
              stats.currentMood == pet_emotions::PetMood::HURT ||
              stats.currentMood == pet_emotions::PetMood::ANNOYED ||
              stats.currentMood == pet_emotions::PetMood::PURRING ||
              stats.currentMood == pet_emotions::PetMood::DISCO ||
              stats.currentMood == pet_emotions::PetMood::SNEEZE ||
              stats.currentMood == pet_emotions::PetMood::AIRPLANE ||
              stats.currentMood == pet_emotions::PetMood::TICKLE ||
              stats.currentMood == pet_emotions::PetMood::HIGH_FIVE ||
              stats.currentMood == pet_emotions::PetMood::NUDGE ||
              stats.currentMood == pet_emotions::PetMood::COOL_GLASSES ||
              stats.currentMood == pet_emotions::PetMood::HAPPY ||
              stats.currentMood == pet_emotions::PetMood::LISTENING ||
              stats.currentMood == pet_emotions::PetMood::THINKING ||
              stats.currentMood == pet_emotions::PetMood::SPEAKING ||
              stats.currentMood == pet_emotions::PetMood::DRIVE_FWD ||
              stats.currentMood == pet_emotions::PetMood::DRIVE_REV ||
              stats.currentMood == pet_emotions::PetMood::TURN_LEFT ||
              stats.currentMood == pet_emotions::PetMood::TURN_RIGHT ||
              stats.currentMood == pet_emotions::PetMood::OBSTACLE) &&
             (now - stateEnterTime > 2600)) {
    transitionTo(pet_emotions::PetMood::IDLE);
  }
}

void setPersona(PetPersona p) {
  stats.persona = p;
  Serial.printf("# [pet_brain] Persona set to: %d\n", (int)p);
}

PetPersona getPersona() {
  return stats.persona;
}

const PetStats& getStats() {
  return stats;
}

void simulateEvent(const char* eventName) {
  // 12 Biểu cảm Chuẩn Mochi (Classic)
  if (strcmp(eventName, "happy") == 0) {
    transitionTo(pet_emotions::PetMood::HAPPY);
    pet_audio::play(pet_audio::PetSound::HAPPY_CHIRP);
  } else if (strcmp(eventName, "listen") == 0 || strcmp(eventName, "listening") == 0) {
    transitionTo(pet_emotions::PetMood::LISTENING);
    pet_audio::play(pet_audio::PetSound::LISTENING_PING);
  } else if (strcmp(eventName, "think") == 0 || strcmp(eventName, "thinking") == 0) {
    transitionTo(pet_emotions::PetMood::THINKING);
    pet_audio::play(pet_audio::PetSound::THINKING_TINKLE);
  } else if (strcmp(eventName, "speak") == 0 || strcmp(eventName, "speaking") == 0) {
    transitionTo(pet_emotions::PetMood::SPEAKING);
    pet_audio::play(pet_audio::PetSound::SPEAKING_BABBLE);
  } else if (strcmp(eventName, "fwd") == 0 || strcmp(eventName, "forward") == 0 || strcmp(eventName, "drive_fwd") == 0) {
    transitionTo(pet_emotions::PetMood::DRIVE_FWD);
    setMotors(35.0f, 35.0f); delay(200); stopMotors();
  } else if (strcmp(eventName, "rev") == 0 || strcmp(eventName, "backward") == 0 || strcmp(eventName, "drive_rev") == 0) {
    transitionTo(pet_emotions::PetMood::DRIVE_REV);
    setMotors(-30.0f, -30.0f); delay(200); stopMotors();
  } else if (strcmp(eventName, "left") == 0 || strcmp(eventName, "turn_left") == 0) {
    transitionTo(pet_emotions::PetMood::TURN_LEFT);
    setMotors(-30.0f, 30.0f); delay(180); stopMotors();
  } else if (strcmp(eventName, "right") == 0 || strcmp(eventName, "turn_right") == 0) {
    transitionTo(pet_emotions::PetMood::TURN_RIGHT);
    setMotors(30.0f, -30.0f); delay(180); stopMotors();
  } else if (strcmp(eventName, "obstacle") == 0) {
    transitionTo(pet_emotions::PetMood::OBSTACLE);
    pet_audio::play(pet_audio::PetSound::OBSTACLE_ALERT);
    setMotors(-35.0f, -35.0f); delay(150); stopMotors();
  } else if (strcmp(eventName, "sleep") == 0 || strcmp(eventName, "sleepy") == 0) {
    transitionTo(pet_emotions::PetMood::SLEEPY);
    pet_audio::play(pet_audio::PetSound::SLEEPY_SNORE);
  } else if (strcmp(eventName, "dizzy") == 0) {
    transitionTo(pet_emotions::PetMood::SHAKEN_DIZZY);
    pet_audio::play(pet_audio::PetSound::DIZZY_CHIRP);

  // Phản xạ Thú cưng Độc quyền (Pet Edition)
  } else if (strcmp(eventName, "pet") == 0 || strcmp(eventName, "purr") == 0) {
    transitionTo(pet_emotions::PetMood::PURRING);
    pet_audio::play(pet_audio::PetSound::PURR_CONTENT);
  } else if (strcmp(eventName, "fall") == 0 || strcmp(eventName, "hurt") == 0) {
    transitionTo(pet_emotions::PetMood::HURT);
    pet_audio::play(pet_audio::PetSound::CRY_HURT);
  } else if (strcmp(eventName, "belly_up") == 0) {
    transitionTo(pet_emotions::PetMood::BELLY_UP);
    pet_audio::play(pet_audio::PetSound::BELLY_UP_ALARM);
  } else if (strcmp(eventName, "shake") == 0) {
    transitionTo(pet_emotions::PetMood::SHAKEN_DIZZY);
    pet_audio::play(pet_audio::PetSound::DIZZY_CHIRP);
  } else if (strcmp(eventName, "knock") == 0) {
    transitionTo(pet_emotions::PetMood::STARTLED);
    pet_audio::play(pet_audio::PetSound::GASP_STARTLED);
  } else if (strcmp(eventName, "noise") == 0) {
    transitionTo(pet_emotions::PetMood::ANNOYED);
    pet_audio::play(pet_audio::PetSound::ANNOYED_SIGH);
  } else if (strcmp(eventName, "disco") == 0) {
    transitionTo(pet_emotions::PetMood::DISCO);
    pet_audio::play(pet_audio::PetSound::DISCO_GROOVE);
  } else if (strcmp(eventName, "achoo") == 0 || strcmp(eventName, "sneeze") == 0) {
    transitionTo(pet_emotions::PetMood::SNEEZE);
    pet_audio::play(pet_audio::PetSound::SNEEZE_BURST);
    setMotors(-40.0f, -40.0f); delay(140); stopMotors();
  } else if (strcmp(eventName, "fly") == 0 || strcmp(eventName, "airplane") == 0) {
    transitionTo(pet_emotions::PetMood::AIRPLANE);
    pet_audio::play(pet_audio::PetSound::AIRPLANE_ENGINE);
  } else if (strcmp(eventName, "tickle") == 0) {
    transitionTo(pet_emotions::PetMood::TICKLE);
    pet_audio::play(pet_audio::PetSound::TICKLE_GIGGLE);
  } else if (strcmp(eventName, "highfive") == 0 || strcmp(eventName, "high_five") == 0) {
    transitionTo(pet_emotions::PetMood::HIGH_FIVE);
  } else if (strcmp(eventName, "nudge") == 0) {
    transitionTo(pet_emotions::PetMood::NUDGE);
    pet_audio::play(pet_audio::PetSound::CUDDLE_NUDGE);
    setMotors(25.0f, 25.0f); delay(180); stopMotors();
  } else if (strcmp(eventName, "cool") == 0 || strcmp(eventName, "cool_glasses") == 0) {
    transitionTo(pet_emotions::PetMood::COOL_GLASSES);
    pet_audio::play(pet_audio::PetSound::THUG_LIFE);
  } else if (strcmp(eventName, "cat") == 0) {
    setPersona(PetPersona::CAT);
  } else if (strcmp(eventName, "dog") == 0) {
    setPersona(PetPersona::PUPPY);
  } else if (strcmp(eventName, "mecha") == 0) {
    setPersona(PetPersona::MECHA);
  } else if (strcmp(eventName, "idle") == 0) {
    transitionTo(pet_emotions::PetMood::IDLE);
  }
}

} // namespace pet_brain
