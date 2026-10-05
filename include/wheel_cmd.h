#pragma once
#include <ctype.h>
#include <stdint.h>

namespace wheel_cmd {

enum class Side : uint8_t { Left, Right, Both };

inline bool equalFold(const char* a, const char* b) {
  if (!a || !b) return false;
  while (*a && *b) {
    unsigned char ca = (unsigned char)tolower((unsigned char)*a);
    unsigned char cb = (unsigned char)tolower((unsigned char)*b);
    if (ca != cb) return false;
    ++a;
    ++b;
  }
  return *a == '\0' && *b == '\0';
}

inline bool parse(const char* text, Side* out) {
  if (!text || !out) return false;
  if (equalFold(text, "left") || equalFold(text, "l")) {
    *out = Side::Left;
    return true;
  }
  if (equalFold(text, "right") || equalFold(text, "r")) {
    *out = Side::Right;
    return true;
  }
  if (equalFold(text, "both") || equalFold(text, "all")) {
    *out = Side::Both;
    return true;
  }
  return false;
}

}
