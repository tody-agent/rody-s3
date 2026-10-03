#include "pet_audio.h"
#include "../include/pet_pins.h"
#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>

namespace pet_audio {

static constexpr i2s_port_t SPK_I2S_PORT = I2S_NUM_1;
static constexpr int SAMPLE_RATE = 22050;
static constexpr size_t DMA_BUF_LEN = 256;

static bool initialized = false;

static void playToneInternal(float freqHz, uint32_t durationMs, float volume = 0.5f, int waveType = 0) {
  if (!initialized || freqHz <= 0.0f) {
    delay(durationMs);
    return;
  }

  size_t totalSamples = (SAMPLE_RATE * durationMs) / 1000;
  int16_t buffer[DMA_BUF_LEN * 2]; // Stereo L+R
  float phase = 0.0f;
  float phaseInc = (2.0f * (float)M_PI * freqHz) / (float)SAMPLE_RATE;
  int16_t maxAmp = (int16_t)(32767.0f * volume);

  size_t samplesGenerated = 0;
  while (samplesGenerated < totalSamples) {
    size_t chunk = DMA_BUF_LEN;
    if (samplesGenerated + chunk > totalSamples) {
      chunk = totalSamples - samplesGenerated;
    }

    for (size_t i = 0; i < chunk; i++) {
      float sample = 0.0f;
      if (waveType == 0) {
        // Sine
        sample = sinf(phase);
      } else if (waveType == 1) {
        // Triangle
        sample = (phase < (float)M_PI) ? (-1.0f + 2.0f * (phase / (float)M_PI)) : (3.0f - 2.0f * (phase / (float)M_PI));
      } else {
        // Sawtooth / Purr pulse
        sample = (phase / (float)M_PI) - 1.0f;
      }

      int16_t val = (int16_t)(sample * maxAmp);
      buffer[i * 2]     = val; // L
      buffer[i * 2 + 1] = val; // R

      phase += phaseInc;
      if (phase >= 2.0f * (float)M_PI) phase -= 2.0f * (float)M_PI;
    }

    size_t bytesWritten = 0;
    i2s_write(SPK_I2S_PORT, buffer, chunk * 2 * sizeof(int16_t), &bytesWritten, portMAX_DELAY);
    samplesGenerated += chunk;
  }
}

bool init() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 3,
    .dma_buf_len = DMA_BUF_LEN,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = pet_pins::SPK_BCLK, // GPIO 16
    .ws_io_num = pet_pins::SPK_LRC,   // GPIO 15
    .data_out_num = pet_pins::SPK_DIN,// GPIO 7
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  esp_err_t err = i2s_driver_install(SPK_I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("# [pet_audio] Failed to install I2S speaker driver: %d\n", err);
    return false;
  }

  err = i2s_set_pin(SPK_I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("# [pet_audio] Failed to set I2S speaker pins: %d\n", err);
    return false;
  }

  initialized = true;
  Serial.println("# [pet_audio] MAX98357A Pet Sound Engine initialized on I2S1");
  return true;
}

void play(PetSound sound) {
  switch (sound) {
    case PetSound::BOOT_HELLO:
      playToneInternal(440, 80, 0.4f);
      playToneInternal(659, 100, 0.45f);
      playToneInternal(880, 180, 0.5f);
      break;

    case PetSound::PURR_CONTENT:
      // Rừ rừ thỏa mãn: Xung nhịp thấp nhấp nhô 90Hz - 130Hz
      for (int k = 0; k < 4; k++) {
        playToneInternal(95, 45, 0.35f, 2);
        playToneInternal(120, 35, 0.4f, 2);
        delay(15);
      }
      playToneInternal(880, 90, 0.25f, 0); // tiếng hót nhỏ nũng nịu
      break;

    case PetSound::CRY_HURT:
      // Tiếng kêu đau ré lên rồi thút thít trầm dần
      for (float f = 1600; f > 450; f -= 120) {
        playToneInternal(f, 22, 0.55f, 0);
      }
      delay(40);
      playToneInternal(320, 140, 0.4f, 1);
      delay(30);
      playToneInternal(280, 180, 0.35f, 1);
      break;

    case PetSound::BELLY_UP_ALARM:
      // Còi bíp bíp cầu cứu dồn dập
      for (int i = 0; i < 3; i++) {
        playToneInternal(1318, 70, 0.5f, 0);
        delay(30);
        playToneInternal(1760, 90, 0.55f, 0);
        delay(40);
      }
      break;

    case PetSound::DIZZY_CHIRP:
      // Tiếng hoa mắt chóng mặt xoắn ốc (vibrato lặp lại)
      for (int i = 0; i < 3; i++) {
        playToneInternal(800, 30, 0.35f);
        playToneInternal(1200, 40, 0.4f);
        playToneInternal(600, 30, 0.35f);
        playToneInternal(1000, 40, 0.4f);
      }
      break;

    case PetSound::GROWL_ANGRY:
      // Tiếng gầm gừ càu nhàu giận dữ (sawtooth trầm 140-180Hz gắt gao)
      for (int i = 0; i < 3; i++) {
        playToneInternal(160, 90, 0.5f, 2);
        playToneInternal(140, 110, 0.55f, 2);
      }
      break;

    case PetSound::GASP_STARTLED:
      // Tiếng giật mình "Gasp!" thảng thốt
      playToneInternal(1400, 40, 0.6f, 0);
      playToneInternal(900, 80, 0.45f, 0);
      break;

    case PetSound::ANNOYED_SIGH:
      // Tiếng thở dài ngao ngán khi quá ồn
      playToneInternal(520, 120, 0.35f, 1);
      playToneInternal(380, 160, 0.3f, 1);
      playToneInternal(260, 220, 0.25f, 1);
      break;

    case PetSound::DISCO_GROOVE:
      // Funky 8-bit disco chiptune beat
      for (int rep = 0; rep < 2; rep++) {
        playToneInternal(262, 70, 0.45f, 2);
        playToneInternal(330, 70, 0.45f, 2);
        playToneInternal(392, 70, 0.5f, 2);
        playToneInternal(523, 100, 0.55f, 0);
        delay(30);
        playToneInternal(440, 70, 0.5f, 2);
        playToneInternal(392, 90, 0.45f, 2);
        delay(40);
      }
      break;

    case PetSound::SNEEZE_BURST:
      // Hít sâu rồi bùng nổ Achoo!
      playToneInternal(600, 60, 0.35f, 0);
      playToneInternal(800, 80, 0.45f, 0);
      playToneInternal(1100, 120, 0.55f, 0);
      delay(120);
      for (float f = 2200; f > 300; f -= 250) {
        playToneInternal(f, 20, 0.7f, 2);
      }
      playToneInternal(180, 160, 0.5f, 1);
      break;

    case PetSound::AIRPLANE_ENGINE:
      // Tiếng rền vang động cơ máy bay lượn
      for (int rep = 0; rep < 4; rep++) {
        playToneInternal(110 + rep * 15, 60, 0.45f, 2);
        playToneInternal(140 + rep * 20, 60, 0.5f, 2);
        delay(15);
      }
      break;

    case PetSound::TICKLE_GIGGLE:
      // Tiếng cười khúc khích khi bị cù lét
      playToneInternal(880, 50, 0.45f, 0);
      playToneInternal(1175, 60, 0.5f, 0);
      playToneInternal(1046, 50, 0.45f, 0);
      playToneInternal(1318, 90, 0.55f, 0);
      delay(30);
      playToneInternal(988, 50, 0.4f, 0);
      playToneInternal(1318, 120, 0.5f, 0);
      break;

    case PetSound::VICTORY_FANFARE:
      // Khúc khải hoàn đập tay chiến thắng
      playToneInternal(523, 90, 0.5f, 0);
      playToneInternal(659, 90, 0.55f, 0);
      playToneInternal(784, 90, 0.6f, 0);
      playToneInternal(1046, 260, 0.7f, 0);
      break;

    case PetSound::CUDDLE_NUDGE:
      // Tiếng thút thít nũng nịu đòi xoa đầu
      playToneInternal(988, 70, 0.35f, 0);
      playToneInternal(1318, 90, 0.4f, 0);
      playToneInternal(1175, 140, 0.45f, 0);
      break;

    case PetSound::THUG_LIFE:
      // Giai điệu kính đen Thug Life
      playToneInternal(392, 100, 0.5f, 2);
      delay(30);
      playToneInternal(392, 100, 0.5f, 2);
      playToneInternal(523, 160, 0.6f, 2);
      playToneInternal(466, 140, 0.55f, 2);
      playToneInternal(392, 220, 0.65f, 2);
      break;

    case PetSound::HAPPY_CHIRP:
      // Hợp âm vui vẻ ríu rít arpeggio
      playToneInternal(523, 70, 0.45f, 0);
      playToneInternal(659, 70, 0.5f, 0);
      playToneInternal(784, 80, 0.55f, 0);
      playToneInternal(1046, 160, 0.6f, 0);
      break;

    case PetSound::LISTENING_PING:
      // Tiếng ping sonar lắng nghe
      playToneInternal(1760, 60, 0.4f, 0);
      break;

    case PetSound::THINKING_TINKLE:
      // Âm thanh tò mò suy nghĩ
      playToneInternal(880, 80, 0.35f, 0);
      playToneInternal(1175, 120, 0.4f, 0);
      break;

    case PetSound::SPEAKING_BABBLE:
      // Âm thanh bập bẹ đang nói
      for (int i = 0; i < 3; i++) {
        playToneInternal(600 + (i % 2) * 200, 50, 0.4f, 1);
        delay(20);
      }
      break;

    case PetSound::OBSTACLE_ALERT:
      // Tiếng cảnh báo vật cản đụng phải (âm kép trầm giật mình)
      playToneInternal(220, 80, 0.6f, 2);
      playToneInternal(180, 140, 0.65f, 2);
      break;

    case PetSound::SLEEPY_SNORE:
      // Khúc hát ru đi xuống êm dịu
      playToneInternal(392, 140, 0.35f, 0);
      playToneInternal(330, 160, 0.3f, 0);
      playToneInternal(262, 240, 0.25f, 0);
      break;
  }
}

bool isPlaying() {
  return false; // synchronous block during tone generation
}

} // namespace pet_audio
