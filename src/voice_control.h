#pragma once
#include <stdint.h>

namespace voice_control {

enum class VoiceCommand {
  NONE,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  STOP,
  SPIN,
  HAPPY
};

void init();
void update();
void setListening(bool enable);
bool isListening();
uint8_t getAudioLevel(); // 0 to 100 for visual reactive mouth/wave
VoiceCommand getLastCommand();
const char* getCommandStr(VoiceCommand cmd);
bool triggerCommand(VoiceCommand cmd);
bool triggerCommandByName(const char* name);

}
