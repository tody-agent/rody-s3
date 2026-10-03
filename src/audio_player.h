#pragma once
#include <stdint.h>

namespace audio_player {

enum class SoundEffect {
  BOOT,
  BEEP_CONFIRM,
  HAPPY_CHIRP,
  LISTEN_START,
  THINKING_PULSE,
  COMMAND_ACK,
  OBSTACLE_ALARM,
  ERROR_BUZZ
};

void init();
void playSfx(SoundEffect sfx);
bool playSfxByName(const char* name);
void playTone(float freqHz, uint32_t durationMs, float volume = 0.5f);
void update();

}
