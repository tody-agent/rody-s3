#include "store.h"
#include <Arduino.h>
#include <Preferences.h>
#include <string.h>

namespace store {
CalBlob gCal;
bool gCalValid = false;

static Preferences prefs;

uint32_t computeCrc(const CalBlob& blob) {
  // CRC-32 (IEEE 802.3)
  const uint8_t* data = reinterpret_cast<const uint8_t*>(&blob);
  size_t length = sizeof(CalBlob) - sizeof(uint32_t); // exclude crc field itself
  uint32_t crc = 0xFFFFFFFF;
  for (size_t i = 0; i < length; i++) {
    crc ^= data[i];
    for (int j = 0; j < 8; j++) {
      if (crc & 1) {
        crc = (crc >> 1) ^ 0xEDB88320;
      } else {
        crc >>= 1;
      }
    }
  }
  return ~crc;
}

void reset() {
  memset(&gCal, 0, sizeof(CalBlob));
  gCal.ver = CAL_VERSION;
  gCal.wheelbase_cm = 10.0f;
  gCal.l.db_lo = 1500;
  gCal.l.db_hi = 1500;
  gCal.l.fwd_sign = 0;
  gCal.l.trim = 1.0f;
  gCal.r.db_lo = 1500;
  gCal.r.db_hi = 1500;
  gCal.r.fwd_sign = 0;
  gCal.r.trim = 1.0f;
  gCal.crc = computeCrc(gCal);
  gCalValid = false;
}

void init() {
  reset();
  load();
}

bool load() {
  bool opened = prefs.begin("rody", true);
  if (!opened || prefs.getBytesLength("cal") != sizeof(CalBlob)) {
    if (opened) prefs.end();
    // Fallback to legacy "otto" namespace if present
    opened = prefs.begin("otto", true);
  }
  if (!opened) {
    Serial.println("# [store] Failed to open NVS namespace readonly");
    gCalValid = false;
    return false;
  }
  size_t len = prefs.getBytesLength("cal");
  if (len != sizeof(CalBlob)) {
    prefs.end();
    Serial.println("# [store] No valid calibration blob found in NVS");
    gCalValid = false;
    return false;
  }
  CalBlob temp;
  prefs.getBytes("cal", &temp, sizeof(CalBlob));
  prefs.end();

  if (temp.ver != CAL_VERSION) {
    Serial.println("# [store] Calibration version mismatch");
    gCalValid = false;
    return false;
  }
  uint32_t expectedCrc = computeCrc(temp);
  if (temp.crc != expectedCrc) {
    Serial.println("# [store] Calibration CRC mismatch");
    gCalValid = false;
    return false;
  }
  gCal = temp;
  gCalValid = true;
  Serial.println("# [store] Calibration loaded successfully from NVS");
  return true;
}

bool save() {
  gCal.ver = CAL_VERSION;
  gCal.crc = computeCrc(gCal);
  if (!prefs.begin("rody", false)) {
    Serial.println("# [store] Failed to open NVS namespace read-write");
    return false;
  }
  size_t written = prefs.putBytes("cal", &gCal, sizeof(CalBlob));
  prefs.end();
  if (written == sizeof(CalBlob)) {
    gCalValid = true;
    Serial.println("# [store] Calibration saved to NVS successfully");
    return true;
  }
  Serial.println("# [store] Failed to write complete calibration blob to NVS");
  return false;
}

}
