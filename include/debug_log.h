#pragma once
#include <stdint.h>
#include <string.h>

namespace debug_log {

constexpr int kCap = 24;
constexpr int kLen = 96;

struct View {
  uint32_t seq;
  char text[kLen];
};

struct Slot {
  uint32_t seq;
  char text[kLen];
};

inline Slot& slotAt(int index) {
  static Slot slots[kCap];
  return slots[index];
}

inline uint32_t& headSeq() {
  static uint32_t seq = 0;
  return seq;
}

inline void push(const char* msg) {
  uint32_t seq = ++headSeq();
  Slot& slot = slotAt((int)((seq - 1) % (uint32_t)kCap));
  slot.seq = seq;
  slot.text[0] = '\0';
  if (!msg) return;
  size_t n = 0;
  for (const char* p = msg; *p && n + 1 < (size_t)kLen; ++p) {
    char c = *p;
    if (c == '"' || c == '\\' || c == '\n' || c == '\r') c = ' ';
    slot.text[n++] = c;
  }
  slot.text[n] = '\0';
}

inline int copySince(uint32_t since, View* out, int maxOut) {
  uint32_t head = headSeq();
  if (!out || maxOut <= 0 || head == 0) return 0;
  uint32_t start = since + 1;
  if (head > (uint32_t)kCap && start < head - (uint32_t)kCap + 1) {
    start = head - (uint32_t)kCap + 1;
  }
  int copied = 0;
  for (uint32_t seq = start; seq <= head && copied < maxOut; ++seq) {
    const Slot& slot = slotAt((int)((seq - 1) % (uint32_t)kCap));
    if (slot.seq != seq) continue;
    out[copied].seq = seq;
    strncpy(out[copied].text, slot.text, kLen - 1);
    out[copied].text[kLen - 1] = '\0';
    ++copied;
  }
  return copied;
}

}
