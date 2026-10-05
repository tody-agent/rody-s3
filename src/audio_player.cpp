#include "audio_player.h"
#include "../include/pins.h"
#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>

namespace audio_player {

static constexpr i2s_port_t SPK_I2S_PORT = I2S_NUM_1;
static constexpr int SAMPLE_RATE = 16000;
static bool i2sInitialized = false;

void init() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 256,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = pins::SPK_BCLK,   // GPIO 16
    .ws_io_num = pins::SPK_LRC,     // GPIO 15
    .data_out_num = pins::SPK_DIN,  // GPIO 7
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  esp_err_t err = i2s_driver_install(SPK_I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("# [audio] Failed to install I2S speaker driver: %d\n", err);
    return;
  }

  err = i2s_set_pin(SPK_I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("# [audio] Failed to set I2S speaker pins: %d\n", err);
    return;
  }

  i2s_zero_dma_buffer(SPK_I2S_PORT);
  i2sInitialized = true;
  Serial.println("# [audio] MAX98357A I2S driver initialized on I2S1 (16kHz)");
}

void playTone(float freqHz, uint32_t durationMs, float volume) {
  if (!i2sInitialized || freqHz <= 10.0f) return;
  if (volume > 1.0f) volume = 1.0f;
  if (volume < 0.0f) volume = 0.0f;

  size_t totalSamples = (SAMPLE_RATE * durationMs) / 1000;
  constexpr size_t CHUNK = 128;
  int16_t buffer[CHUNK * 2]; // Stereo (L+R)

  float phase = 0.0f;
  float phaseStep = 2.0f * (float)M_PI * freqHz / (float)SAMPLE_RATE;
  int16_t amplitude = (int16_t)(volume * 16000.0f);

  size_t samplesRemaining = totalSamples;
  while (samplesRemaining > 0) {
    size_t n = samplesRemaining < CHUNK ? samplesRemaining : CHUNK;
    for (size_t i = 0; i < n; i++) {
      int16_t val = (int16_t)(sinf(phase) * amplitude);
      buffer[i * 2]     = val; // Left channel
      buffer[i * 2 + 1] = val; // Right channel
      phase += phaseStep;
      if (phase >= 2.0f * (float)M_PI) phase -= 2.0f * (float)M_PI;
    }
    size_t bytesWritten = 0;
    i2s_write(SPK_I2S_PORT, buffer, n * 2 * sizeof(int16_t), &bytesWritten, pdMS_TO_TICKS(50));
    samplesRemaining -= n;
  }

  // Soft silence flush to prevent speaker pop
  memset(buffer, 0, sizeof(buffer));
  size_t written = 0;
  i2s_write(SPK_I2S_PORT, buffer, 64 * sizeof(int16_t), &written, 50);
}

void playSfx(SoundEffect sfx) {
  if (!i2sInitialized) return;

  switch (sfx) {
    case SoundEffect::BOOT:
      playTone(440, 70, 0.4f);
      delay(30);
      playTone(659, 80, 0.5f);
      delay(30);
      playTone(880, 140, 0.6f);
      break;

    case SoundEffect::BEEP_CONFIRM:
      playTone(987, 60, 0.5f);
      delay(20);
      playTone(1318, 90, 0.6f);
      break;

    case SoundEffect::HAPPY_CHIRP:
      for (float f = 600; f < 1600; f += 80) {
        playTone(f, 15, 0.5f);
      }
      playTone(1760, 100, 0.6f);
      break;

    case SoundEffect::LISTEN_START:
      playTone(523, 70, 0.4f);
      delay(20);
      playTone(784, 110, 0.5f);
      break;

    case SoundEffect::THINKING_PULSE:
      playTone(700, 40, 0.3f);
      delay(40);
      playTone(740, 40, 0.3f);
      break;

    case SoundEffect::COMMAND_ACK:
      playTone(1046, 50, 0.5f);
      delay(25);
      playTone(1567, 80, 0.6f);
      break;

    case SoundEffect::OBSTACLE_ALARM:
      for (int i = 0; i < 2; i++) {
        playTone(1200, 80, 0.7f);
        playTone(600, 100, 0.7f);
        delay(40);
      }
      break;

    case SoundEffect::ERROR_BUZZ:
      playTone(220, 120, 0.7f);
      delay(50);
      playTone(180, 180, 0.7f);
      break;
  }
}

bool playSfxByName(const char* name) {
  if (strcmp(name, "boot") == 0)       { playSfx(SoundEffect::BOOT); return true; }
  if (strcmp(name, "beep") == 0)       { playSfx(SoundEffect::BEEP_CONFIRM); return true; }
  if (strcmp(name, "happy") == 0)      { playSfx(SoundEffect::HAPPY_CHIRP); return true; }
  if (strcmp(name, "listen") == 0)     { playSfx(SoundEffect::LISTEN_START); return true; }
  if (strcmp(name, "think") == 0)      { playSfx(SoundEffect::THINKING_PULSE); return true; }
  if (strcmp(name, "ack") == 0)        { playSfx(SoundEffect::COMMAND_ACK); return true; }
  if (strcmp(name, "obstacle") == 0)   { playSfx(SoundEffect::OBSTACLE_ALARM); return true; }
  if (strcmp(name, "error") == 0)      { playSfx(SoundEffect::ERROR_BUZZ); return true; }
  return false;
}

void update() {
  // Can be used for background async audio streaming if needed
}

}
