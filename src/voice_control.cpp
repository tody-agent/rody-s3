#include "voice_control.h"
#include "../include/pins.h"
#include "emotion_gfx.h"
#include "audio_player.h"
#include "drive.h"
#include <Arduino.h>
#include <driver/i2s.h>
#include <string.h>
#include <math.h>

namespace voice_control {

static constexpr i2s_port_t MIC_I2S_PORT = I2S_NUM_0;
static constexpr int SAMPLE_RATE = 16000;
static constexpr size_t DMA_BUF_LEN = 256;

static bool micInitialized = false;
static bool listeningActive = true;
static VoiceCommand lastCommand = VoiceCommand::NONE;
static uint8_t currentAudioLevel = 0; // 0-100%

// Buffer for audio processing
static int32_t rawSampleBuffer[DMA_BUF_LEN];

void init() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT, // INMP441 outputs 24-bit data in 32-bit slot
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,  // L/R tied to GND -> Left channel
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = DMA_BUF_LEN,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = pins::MIC_SCK,    // GPIO 4
    .ws_io_num = pins::MIC_WS,      // GPIO 5
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = pins::MIC_SD     // GPIO 6
  };

  esp_err_t err = i2s_driver_install(MIC_I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("# [mic] Failed to install I2S mic driver: %d\n", err);
    return;
  }

  err = i2s_set_pin(MIC_I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("# [mic] Failed to set I2S mic pins: %d\n", err);
    return;
  }

  micInitialized = true;
  Serial.println("# [mic] INMP441 I2S microphone initialized on I2S0 (16kHz)");
}

void setListening(bool enable) {
  listeningActive = enable;
  if (enable) {
    emotion_gfx::setEmotion(emotion_gfx::Emotion::LISTENING);
    audio_player::playSfx(audio_player::SoundEffect::LISTEN_START);
  } else {
    emotion_gfx::setEmotion(emotion_gfx::Emotion::IDLE);
  }
}

bool isListening() {
  return listeningActive;
}

uint8_t getAudioLevel() {
  return currentAudioLevel;
}

VoiceCommand getLastCommand() {
  return lastCommand;
}

const char* getCommandStr(VoiceCommand cmd) {
  switch (cmd) {
    case VoiceCommand::FORWARD:  return "forward";
    case VoiceCommand::BACKWARD: return "backward";
    case VoiceCommand::LEFT:     return "left";
    case VoiceCommand::RIGHT:    return "right";
    case VoiceCommand::STOP:     return "stop";
    case VoiceCommand::SPIN:     return "spin";
    case VoiceCommand::HAPPY:    return "happy";
    default:                     return "none";
  }
}

bool triggerCommand(VoiceCommand cmd) {
  lastCommand = cmd;
  const char* err = nullptr;

  switch (cmd) {
    case VoiceCommand::FORWARD:
      Serial.println("# [voice] Command Recognized: TIẾN / FORWARD");
      emotion_gfx::setEmotion(emotion_gfx::Emotion::DRIVE_FWD);
      audio_player::playSfx(audio_player::SoundEffect::COMMAND_ACK);
      drive::drive(50.0f, 50.0f, 1500, err);
      break;

    case VoiceCommand::BACKWARD:
      Serial.println("# [voice] Command Recognized: LÙI / BACKWARD");
      emotion_gfx::setEmotion(emotion_gfx::Emotion::DRIVE_REV);
      audio_player::playSfx(audio_player::SoundEffect::COMMAND_ACK);
      drive::drive(-40.0f, -40.0f, 1200, err);
      break;

    case VoiceCommand::LEFT:
      Serial.println("# [voice] Command Recognized: TRÁI / LEFT");
      emotion_gfx::setEmotion(emotion_gfx::Emotion::TURN_LEFT);
      audio_player::playSfx(audio_player::SoundEffect::COMMAND_ACK);
      drive::drive(-35.0f, 35.0f, 700, err);
      break;

    case VoiceCommand::RIGHT:
      Serial.println("# [voice] Command Recognized: PHẢI / RIGHT");
      emotion_gfx::setEmotion(emotion_gfx::Emotion::TURN_RIGHT);
      audio_player::playSfx(audio_player::SoundEffect::COMMAND_ACK);
      drive::drive(35.0f, -35.0f, 700, err);
      break;

    case VoiceCommand::STOP:
      Serial.println("# [voice] Command Recognized: DỪNG / STOP");
      drive::stop();
      emotion_gfx::setEmotion(emotion_gfx::Emotion::IDLE);
      audio_player::playSfx(audio_player::SoundEffect::BEEP_CONFIRM);
      break;

    case VoiceCommand::SPIN:
      Serial.println("# [voice] Command Recognized: QUAY / SPIN");
      emotion_gfx::setEmotion(emotion_gfx::Emotion::DIZZY);
      audio_player::playSfx(audio_player::SoundEffect::HAPPY_CHIRP);
      drive::drive(60.0f, -60.0f, 1800, err);
      break;

    case VoiceCommand::HAPPY:
      Serial.println("# [voice] Command Recognized: VUI VẺ / HAPPY");
      emotion_gfx::setEmotion(emotion_gfx::Emotion::HAPPY);
      audio_player::playSfx(audio_player::SoundEffect::HAPPY_CHIRP);
      break;

    default:
      return false;
  }
  return true;
}

bool triggerCommandByName(const char* name) {
  if (strcmp(name, "forward") == 0 || strcmp(name, "fwd") == 0 || strcmp(name, "tien") == 0) {
    return triggerCommand(VoiceCommand::FORWARD);
  }
  if (strcmp(name, "backward") == 0 || strcmp(name, "rev") == 0 || strcmp(name, "lui") == 0) {
    return triggerCommand(VoiceCommand::BACKWARD);
  }
  if (strcmp(name, "left") == 0 || strcmp(name, "trai") == 0) {
    return triggerCommand(VoiceCommand::LEFT);
  }
  if (strcmp(name, "right") == 0 || strcmp(name, "phai") == 0) {
    return triggerCommand(VoiceCommand::RIGHT);
  }
  if (strcmp(name, "stop") == 0 || strcmp(name, "dung") == 0) {
    return triggerCommand(VoiceCommand::STOP);
  }
  if (strcmp(name, "spin") == 0 || strcmp(name, "quay") == 0) {
    return triggerCommand(VoiceCommand::SPIN);
  }
  if (strcmp(name, "happy") == 0 || strcmp(name, "vui") == 0 || strcmp(name, "rody") == 0 || strcmp(name, "chao") == 0 || strcmp(name, "otto") == 0) {
    return triggerCommand(VoiceCommand::HAPPY);
  }
  return false;
}

void update() {
  if (!micInitialized || !listeningActive) return;

  size_t bytesRead = 0;
  esp_err_t res = i2s_read(MIC_I2S_PORT, rawSampleBuffer, sizeof(rawSampleBuffer), &bytesRead, 10);
  if (res != ESP_OK || bytesRead == 0) return;

  size_t samples = bytesRead / sizeof(int32_t);
  int64_t sumSquares = 0;

  for (size_t i = 0; i < samples; i++) {
    // INMP441 24-bit data in high bits of 32-bit slot
    int32_t s = rawSampleBuffer[i] >> 14; // scale down to 18-bit signed
    sumSquares += (int64_t)s * s;
  }

  int32_t rms = (int32_t)sqrtf((float)(sumSquares / (samples > 0 ? samples : 1)));
  // Normalize RMS to 0-100 scale
  int level = rms / 400;
  if (level > 100) level = 100;
  currentAudioLevel = (uint8_t)level;

  // Real-time acoustic energy detector:
  // Detect distinct voice pulse triggers
  static uint32_t highEnergyStart = 0;
  static bool voiceTriggerActive = false;

  if (currentAudioLevel > 45) {
    if (highEnergyStart == 0) {
      highEnergyStart = millis();
    } else if (millis() - highEnergyStart > 120 && !voiceTriggerActive) {
      // Voice burst detected! Animate listening face
      voiceTriggerActive = true;
      if (emotion_gfx::getEmotion() == emotion_gfx::Emotion::IDLE) {
        emotion_gfx::setEmotion(emotion_gfx::Emotion::LISTENING);
      }
    }
  } else {
    if (voiceTriggerActive && millis() - highEnergyStart > 800) {
      voiceTriggerActive = false;
      highEnergyStart = 0;
      // If was just an ambient sound, return to idle after 1.5s
      if (emotion_gfx::getEmotion() == emotion_gfx::Emotion::LISTENING) {
        emotion_gfx::setEmotion(emotion_gfx::Emotion::IDLE);
      }
    }
    if (currentAudioLevel < 20) {
      highEnergyStart = 0;
    }
  }
}

}
