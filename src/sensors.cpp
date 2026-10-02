#include "sensors.h"
#include "../include/pins.h"
#include "calib.h"
#include <Arduino.h>

namespace sensors {

static volatile uint32_t gEdges[2] = {0, 0};
static volatile uint32_t gLastUs[2] = {0, 0};

void IRAM_ATTR isrTach(void* arg) {
  int i = (int)(intptr_t)arg;
  uint32_t t = micros();
  if (t - gLastUs[i] > 2000) { // 2ms debounce
    gEdges[i]++;
    gLastUs[i] = t;
  }
}

void init() {
  // Ultrasonic pins
  pinMode(pins::US_TRIG, OUTPUT);
  digitalWrite(pins::US_TRIG, LOW);
  pinMode(pins::US_ECHO, INPUT);

  // Line / Tachometer IR pins
  pinMode(pins::IR_L, INPUT);
  pinMode(pins::IR_R, INPUT);

  // Attach interrupts for tachometer
  attachInterruptArg(digitalPinToInterrupt(pins::IR_L), isrTach, (void*)0, CHANGE);
  attachInterruptArg(digitalPinToInterrupt(pins::IR_R), isrTach, (void*)1, CHANGE);

  // Battery ADC
  analogReadResolution(12);
}

float readBatteryVoltage() {
  // ESP32 ADC1 calibration read
  uint32_t mv = analogReadMilliVolts(pins::BAT_ADC);
  return (float)mv * BAT_DIV / 1000.0f;
}

float measureDistanceCmOnce() {
  digitalWrite(pins::US_TRIG, LOW);
  delayMicroseconds(4);
  digitalWrite(pins::US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(pins::US_TRIG, LOW);

  // 30ms timeout = 30000us (~500cm max range)
  unsigned long duration = pulseIn(pins::US_ECHO, HIGH, 30000UL);
  if (duration == 0) {
    return 999.0f;
  }
  float cm = (float)duration / 58.2f;
  if (cm < 2.0f || cm > 400.0f) {
    return 999.0f;
  }
  return cm;
}

bool readUltrasonicN(size_t n, float* results, size_t maxN) {
  if (n > maxN) n = maxN;
  for (size_t i = 0; i < n; i++) {
    results[i] = measureDistanceCmOnce();
    delay(20); // allow echo reverberations to settle
  }
  return true;
}

void readLine(int& l, int& r) {
  // DO outputs: active low or high depending on module, return 1 if line/object detected
  l = digitalRead(pins::IR_L) == LOW ? 1 : 0;
  r = digitalRead(pins::IR_R) == LOW ? 1 : 0;
}

void resetTach() {
  noInterrupts();
  gEdges[0] = 0;
  gEdges[1] = 0;
  interrupts();
}

void getTachCounts(uint32_t& l, uint32_t& r) {
  noInterrupts();
  l = gEdges[0];
  r = gEdges[1];
  interrupts();
}

void sampleTach(uint32_t ms, uint32_t& edgesL, uint32_t& edgesR, float& rpmL, float& rpmR) {
  resetTach();
  delay(ms);
  getTachCounts(edgesL, edgesR);
  rpmL = calib::rpmFromEdges(edgesL, ms, TACH_EDGES_PER_REV);
  rpmR = calib::rpmFromEdges(edgesR, ms, TACH_EDGES_PER_REV);
}

}
