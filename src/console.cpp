#include "console.h"
#include "../include/pins.h"
#include "store.h"
#include "sensors.h"
#include "drive.h"
#include "calib_run.h"
#include "calib.h"
#include "behaviors.h"
#include "emotion_gfx.h"
#include "audio_player.h"
#include "voice_control.h"
#include <Arduino.h>
#include <Wire.h>
#include <vector>
#include <string>

#ifndef FW_VERSION
#define FW_VERSION "0.1.0"
#endif

namespace console {

static String lineBuffer = "";

static const char* getResetReasonStr() {
  esp_reset_reason_t r = esp_reset_reason();
  switch (r) {
    case ESP_RST_POWERON:   return "POWERON";
    case ESP_RST_BROWNOUT:  return "BROWNOUT";
    case ESP_RST_SW:        return "SW";
    case ESP_RST_PANIC:     return "PANIC";
    case ESP_RST_INT_WDT:
    case ESP_RST_TASK_WDT:
    case ESP_RST_WDT:       return "WDT";
    default:                return "OTHER";
  }
}

static void handleCommand(const String& line) {
  String trimmed = line;
  trimmed.trim();
  if (trimmed.length() == 0) return;

  // Split tokens
  std::vector<String> tokens;
  int start = 0;
  while (start < trimmed.length()) {
    while (start < trimmed.length() && (trimmed[start] == ' ' || trimmed[start] == '\t')) start++;
    if (start >= trimmed.length()) break;
    int end = start;
    while (end < trimmed.length() && trimmed[end] != ' ' && trimmed[end] != '\t') end++;
    tokens.push_back(trimmed.substring(start, end));
    start = end;
  }
  if (tokens.empty()) return;

  const String& cmd = tokens[0];

  if (cmd == "ping") {
    Serial.printf("{\"cmd\":\"ping\",\"ok\":true,\"fw\":\"%s\"}\n", FW_VERSION);
  }
  else if (cmd == "info") {
    uint32_t flash = ESP.getFlashChipSize();
    uint32_t psram = ESP.getPsramSize();
    uint32_t heap = ESP.getFreeHeap();
    uint32_t uptime = millis();
    const char* reason = getResetReasonStr();
    bool calValid = store::gCalValid;
    Serial.printf("{\"cmd\":\"info\",\"ok\":true,\"fw\":\"%s\",\"flash\":%u,\"psram\":%u,\"heap\":%u,\"uptime_ms\":%u,\"reset_reason\":\"%s\",\"cal_valid\":%s}\n",
                  FW_VERSION, flash, psram, heap, uptime, reason, calValid ? "true" : "false");
  }
  else if (cmd == "i2c") {
    std::vector<int> found;
    for (uint8_t addr = 1; addr < 127; addr++) {
      Wire.beginTransmission(addr);
      if (Wire.endTransmission() == 0) {
        found.push_back(addr);
      }
    }
    Serial.print("{\"cmd\":\"i2c\",\"ok\":true,\"devices\":[");
    for (size_t i = 0; i < found.size(); i++) {
      Serial.print(found[i]);
      if (i + 1 < found.size()) Serial.print(",");
    }
    Serial.println("]}");
  }
  else if (cmd == "bat") {
    float v = sensors::readBatteryVoltage();
    Serial.printf("{\"cmd\":\"bat\",\"ok\":true,\"v\":%.2f}\n", v);
  }
  else if (cmd == "us") {
    size_t n = 5;
    if (tokens.size() > 1) {
      n = tokens[1].toInt();
      if (n < 1) n = 1;
      if (n > 10) n = 10;
    }
    float results[10];
    sensors::readUltrasonicN(n, results, 10);
    int validCount = 0;
    Serial.print("{\"cmd\":\"us\",\"ok\":true,\"cm\":[");
    for (size_t i = 0; i < n; i++) {
      Serial.printf("%.1f", results[i]);
      if (i + 1 < n) Serial.print(",");
      if (results[i] >= 2.0f && results[i] <= 400.0f) validCount++;
    }
    Serial.printf("],\"valid\":%d}\n", validCount);
  }
  else if (cmd == "ir") {
    int l = 0, r = 0;
    sensors::readLine(l, r);
    Serial.printf("{\"cmd\":\"ir\",\"ok\":true,\"l\":%d,\"r\":%d}\n", l, r);
  }
  else if (cmd == "tach") {
    uint32_t ms = 1000;
    if (tokens.size() > 1) {
      ms = tokens[1].toInt();
      if (ms == 0) ms = 1000;
    }
    uint32_t eL, eR;
    float rpmL, rpmR;
    sensors::sampleTach(ms, eL, eR, rpmL, rpmR);
    Serial.printf("{\"cmd\":\"tach\",\"ok\":true,\"l\":%u,\"r\":%u,\"rpm_l\":%.1f,\"rpm_r\":%.1f}\n",
                  eL, eR, rpmL, rpmR);
  }
  else if (cmd == "pwm") {
    if (tokens.size() < 3) {
      Serial.println("{\"cmd\":\"pwm\",\"ok\":false,\"err\":\"missing_args\"}");
      return;
    }
    char w = tokens[1][0];
    int ch = (w == 'L' || w == 'l') ? CH_L : ((w == 'R' || w == 'r') ? CH_R : -1);
    if (ch < 0) {
      Serial.println("{\"cmd\":\"pwm\",\"ok\":false,\"err\":\"invalid_wheel\"}");
      return;
    }
    uint16_t us = 0;
    if (tokens[2] != "off") {
      us = tokens[2].toInt();
    }
    if (drive::setPwmRaw(ch, us)) {
      Serial.println("{\"cmd\":\"pwm\",\"ok\":true}");
    } else {
      Serial.println("{\"cmd\":\"pwm\",\"ok\":false,\"err\":\"drive_failed\"}");
    }
  }
  else if (cmd == "drive") {
    if (tokens.size() < 4) {
      Serial.println("{\"cmd\":\"drive\",\"ok\":false,\"err\":\"missing_args\"}");
      return;
    }
    float speedL = tokens[1].toFloat();
    float speedR = tokens[2].toFloat();
    uint32_t ms = tokens[3].toInt();
    const char* err = nullptr;
    if (drive::drive(speedL, speedR, ms, err)) {
      Serial.println("{\"cmd\":\"drive\",\"ok\":true}");
    } else {
      Serial.printf("{\"cmd\":\"drive\",\"ok\":false,\"err\":\"%s\"}\n", err ? err : "failed");
    }
  }
  else if (cmd == "stop") {
    drive::stop();
    Serial.println("{\"cmd\":\"stop\",\"ok\":true}");
  }
  else if (cmd == "meas") {
    if (tokens.size() < 2) {
      Serial.println("{\"cmd\":\"meas\",\"ok\":false,\"err\":\"missing_args\"}");
      return;
    }
    float speed = tokens[1].toFloat();
    float target, rpmL, rpmR, errPct;
    if (calib_run::runMeas(speed, target, rpmL, rpmR, errPct)) {
      Serial.printf("{\"cmd\":\"meas\",\"ok\":true,\"target\":%.1f,\"rpm_l\":%.1f,\"rpm_r\":%.1f,\"err_pct\":%.1f}\n",
                    target, rpmL, rpmR, errPct);
    } else {
      Serial.println("{\"cmd\":\"meas\",\"ok\":false,\"err\":\"uncalibrated\"}");
    }
  }
  else if (cmd == "cal") {
    if (tokens.size() < 2) {
      Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"missing_subcmd\"}");
      return;
    }
    const String& sub = tokens[1];

    if (sub == "deadband") {
      if (tokens.size() < 3) {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"missing_wheel\"}");
        return;
      }
      char wheel = tokens[2][0];
      uint16_t lo, hi;
      if (calib_run::runDeadband(wheel, lo, hi)) {
        Serial.printf("{\"cmd\":\"cal\",\"ok\":true,\"lo\":%u,\"hi\":%u}\n", lo, hi);
      } else {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"deadband_failed\"}");
      }
    }
    else if (sub == "spin") {
      if (tokens.size() < 3) {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"missing_wheel\"}");
        return;
      }
      char wheel = tokens[2][0];
      if (calib_run::runSpin(wheel)) {
        Serial.println("{\"cmd\":\"cal\",\"ok\":true}");
      } else {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"spin_failed\"}");
      }
    }
    else if (sub == "dir") {
      if (tokens.size() < 4) {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"missing_args\"}");
        return;
      }
      char wheel = tokens[2][0];
      int8_t dir = tokens[3].toInt();
      if (calib_run::setDir(wheel, dir)) {
        Serial.println("{\"cmd\":\"cal\",\"ok\":true}");
      } else {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"dir_failed\"}");
      }
    }
    else if (sub == "sweep") {
      if (tokens.size() < 3) {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"missing_wheel\"}");
        return;
      }
      char wheel = tokens[2][0];
      float fwd[calib::NPTS];
      float rev[calib::NPTS];
      if (calib_run::runSweep(wheel, fwd, rev, calib::NPTS)) {
        Serial.print("{\"cmd\":\"cal\",\"ok\":true,\"fwd\":[");
        for (size_t i = 0; i < calib::NPTS; i++) {
          Serial.printf("%.1f", fwd[i]);
          if (i + 1 < calib::NPTS) Serial.print(",");
        }
        Serial.print("],\"rev\":[");
        for (size_t i = 0; i < calib::NPTS; i++) {
          Serial.printf("%.1f", rev[i]);
          if (i + 1 < calib::NPTS) Serial.print(",");
        }
        Serial.println("]}");
      } else {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"sweep_failed\"}");
      }
    }
    else if (sub == "wheelbase") {
      if (tokens.size() < 3) {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"missing_cm\"}");
        return;
      }
      float cm = tokens[2].toFloat();
      if (calib_run::setWheelbase(cm)) {
        Serial.println("{\"cmd\":\"cal\",\"ok\":true}");
      } else {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"wheelbase_failed\"}");
      }
    }
    else if (sub == "drift") {
      if (tokens.size() < 4) {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"missing_args\"}");
        return;
      }
      float d = tokens[2].toFloat();
      float D = tokens[3].toFloat();
      float trimL, trimR;
      if (calib_run::runDrift(d, D, trimL, trimR)) {
        Serial.printf("{\"cmd\":\"cal\",\"ok\":true,\"trim_l\":%.4f,\"trim_r\":%.4f}\n", trimL, trimR);
      } else {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"drift_failed\"}");
      }
    }
    else if (sub == "show") {
      Serial.printf("{\"cmd\":\"cal\",\"ok\":true,"
                    "\"l\":{\"fwd_sign\":%d,\"db_lo\":%u,\"db_hi\":%u,\"trim\":%.4f},"
                    "\"r\":{\"fwd_sign\":%d,\"db_lo\":%u,\"db_hi\":%u,\"trim\":%.4f},"
                    "\"wheelbase_cm\":%.2f}\n",
                    store::gCal.l.fwd_sign, store::gCal.l.db_lo, store::gCal.l.db_hi, store::gCal.l.trim,
                    store::gCal.r.fwd_sign, store::gCal.r.db_lo, store::gCal.r.db_hi, store::gCal.r.trim,
                    store::gCal.wheelbase_cm);
    }
    else if (sub == "save") {
      if (store::save()) {
        Serial.println("{\"cmd\":\"cal\",\"ok\":true}");
      } else {
        Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"save_failed\"}");
      }
    }
    else if (sub == "reset") {
      store::reset();
      Serial.println("{\"cmd\":\"cal\",\"ok\":true}");
    }
    else {
      Serial.println("{\"cmd\":\"cal\",\"ok\":false,\"err\":\"unknown_subcmd\"}");
    }
  }
  else if (cmd == "mode") {
    if (tokens.size() < 2) {
      Serial.printf("{\"cmd\":\"mode\",\"ok\":true,\"mode\":\"%s\"}\n", behaviors::getModeStr());
      return;
    }
    const String& m = tokens[1];
    if (behaviors::setMode(m.c_str())) {
      Serial.printf("{\"cmd\":\"mode\",\"ok\":true,\"mode\":\"%s\"}\n", m.c_str());
    } else {
      Serial.println("{\"cmd\":\"mode\",\"ok\":false,\"err\":\"invalid_mode\"}");
    }
  }
  else if (cmd == "face") {
    if (tokens.size() < 2) {
      Serial.printf("{\"cmd\":\"face\",\"ok\":true,\"emotion\":\"%s\"}\n", emotion_gfx::getEmotionStr());
      return;
    }
    const String& emo = tokens[1];
    if (emotion_gfx::setEmotionByName(emo.c_str())) {
      Serial.printf("{\"cmd\":\"face\",\"ok\":true,\"emotion\":\"%s\"}\n", emotion_gfx::getEmotionStr());
    } else {
      Serial.println("{\"cmd\":\"face\",\"ok\":false,\"err\":\"invalid_emotion\"}");
    }
  }
  else if (cmd == "sfx") {
    if (tokens.size() < 2) {
      Serial.println("{\"cmd\":\"sfx\",\"ok\":false,\"err\":\"missing_sfx_name\"}");
      return;
    }
    const String& name = tokens[1];
    if (audio_player::playSfxByName(name.c_str())) {
      Serial.printf("{\"cmd\":\"sfx\",\"ok\":true,\"played\":\"%s\"}\n", name.c_str());
    } else {
      Serial.println("{\"cmd\":\"sfx\",\"ok\":false,\"err\":\"invalid_sfx\"}");
    }
  }
  else if (cmd == "voice") {
    if (tokens.size() < 2) {
      Serial.printf("{\"cmd\":\"voice\",\"ok\":true,\"last\":\"%s\",\"level\":%u,\"listening\":%s}\n",
                    voice_control::getCommandStr(voice_control::getLastCommand()),
                    voice_control::getAudioLevel(),
                    voice_control::isListening() ? "true" : "false");
      return;
    }
    const String& vcmd = tokens[1];
    if (vcmd == "listen_on") {
      voice_control::setListening(true);
      Serial.println("{\"cmd\":\"voice\",\"ok\":true,\"listening\":true}");
    } else if (vcmd == "listen_off") {
      voice_control::setListening(false);
      Serial.println("{\"cmd\":\"voice\",\"ok\":true,\"listening\":false}");
    } else {
      if (voice_control::triggerCommandByName(vcmd.c_str())) {
        Serial.printf("{\"cmd\":\"voice\",\"ok\":true,\"triggered\":\"%s\"}\n", vcmd.c_str());
      } else {
        Serial.println("{\"cmd\":\"voice\",\"ok\":false,\"err\":\"invalid_voice_command\"}");
      }
    }
  }
  else {
    Serial.printf("{\"cmd\":\"%s\",\"ok\":false,\"err\":\"unknown\"}\n", cmd.c_str());
  }
}

void init() {
  Serial.begin(115200);
  lineBuffer.reserve(128);
}

void process() {
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') {
      if (lineBuffer.length() > 0) {
        handleCommand(lineBuffer);
        lineBuffer = "";
      }
    } else {
      if (lineBuffer.length() < 256) {
        lineBuffer += c;
      }
    }
  }
}

}
