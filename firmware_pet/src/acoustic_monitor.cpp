#include "acoustic_monitor.h"
#include "../include/pet_pins.h"
#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>

namespace acoustic_monitor {

static constexpr i2s_port_t MIC_I2S_PORT = I2S_NUM_0;
static constexpr int SAMPLE_RATE = 16000;
static constexpr size_t DMA_BUF_LEN = 256;

static bool initialized = false;
static int32_t sampleBuffer[DMA_BUF_LEN];
static uint8_t currentLevel = 0;
static uint8_t lastLevel = 0;

static uint32_t loudNoiseStartTime = 0;
static bool loudSustained = false;
static bool audioSpikeDetected = false;
static uint32_t lastSpikeTime = 0;
static bool clapDetected = false;
static uint32_t blowStartTime = 0;
static bool blowingDetected = false;

bool init() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = DMA_BUF_LEN,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = pet_pins::MIC_SCK,
    .ws_io_num = pet_pins::MIC_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = pet_pins::MIC_SD
  };

  esp_err_t err = i2s_driver_install(MIC_I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("# [acoustic] Failed to install I2S driver: %d\n", err);
    return false;
  }

  err = i2s_set_pin(MIC_I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("# [acoustic] Failed to set I2S pins: %d\n", err);
    return false;
  }

  initialized = true;
  Serial.println("# [acoustic] INMP441 acoustic monitor initialized on I2S0");
  return true;
}

void update() {
  if (!initialized) return;

  size_t bytesRead = 0;
  esp_err_t res = i2s_read(MIC_I2S_PORT, sampleBuffer, sizeof(sampleBuffer), &bytesRead, 10);
  if (res != ESP_OK || bytesRead == 0) return;

  size_t samples = bytesRead / sizeof(int32_t);
  int64_t sumSquares = 0;

  for (size_t i = 0; i < samples; i++) {
    int32_t s = sampleBuffer[i] >> 14;
    sumSquares += (int64_t)s * s;
  }

  int32_t rms = (int32_t)sqrtf((float)(sumSquares / (samples > 0 ? samples : 1)));
  int level = rms / 400;
  if (level > 100) level = 100;
  currentLevel = (uint8_t)level;

  uint32_t now = millis();

  // Audio spike & Clap-Clap detection
  if (currentLevel >= 70 && (currentLevel - lastLevel) >= 40) {
    audioSpikeDetected = true;
    if (lastSpikeTime > 0 && (now - lastSpikeTime >= 150) && (now - lastSpikeTime <= 550)) {
      clapDetected = true;
      lastSpikeTime = 0;
    } else {
      lastSpikeTime = now;
    }
  } else if (lastSpikeTime > 0 && (now - lastSpikeTime > 600)) {
    lastSpikeTime = 0;
  }
  lastLevel = currentLevel;

  // Sustained loud noise detection (> 70% continuous for > 2500ms)
  if (currentLevel >= 70) {
    if (loudNoiseStartTime == 0) {
      loudNoiseStartTime = now;
    } else if (now - loudNoiseStartTime >= 2500) {
      loudSustained = true;
    }
  } else if (currentLevel < 55) {
    loudNoiseStartTime = 0;
    loudSustained = false;
  }

  // Blowing air detection (50-88% level sustained for > 350ms)
  if (currentLevel >= 50 && currentLevel <= 88) {
    if (blowStartTime == 0) {
      blowStartTime = now;
    } else if (now - blowStartTime >= 350) {
      blowingDetected = true;
    }
  } else {
    blowStartTime = 0;
    blowingDetected = false;
  }
}

uint8_t getCurrentRmsLevel() {
  return currentLevel;
}

bool isLoudNoiseSustained() {
  return loudSustained;
}

bool checkAndClearAudioSpike() {
  if (audioSpikeDetected) {
    audioSpikeDetected = false;
    return true;
  }
  return false;
}

bool checkAndClearClap() {
  if (clapDetected) {
    clapDetected = false;
    return true;
  }
  return false;
}

bool isBlowingAir() {
  return blowingDetected;
}

uint32_t getNoiseDurationMs() {
  if (loudNoiseStartTime == 0) return 0;
  return (millis() - loudNoiseStartTime);
}

} // namespace acoustic_monitor
