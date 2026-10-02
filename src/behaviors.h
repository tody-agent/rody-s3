#pragma once

namespace behaviors {
enum class Mode { MANUAL, AVOID, LINE };

void init();
bool setMode(const char* modeStr);
const char* getModeStr();
Mode getMode();
void update();
}
