#include "oled_diag.h"
#include "store.h"
#include "../include/display_policy.h"
#include "../include/pins.h"
#include <Arduino.h>
#include <Wire.h>

namespace oled_diag {

static bool probePair(int sda, int scl, uint8_t addr) {
  Wire1.end();
  delay(2);
  if (!Wire1.begin(sda, scl, 100000)) {
    return false;
  }
  Wire1.setTimeOut(80);

  Wire1.beginTransmission(addr);
  bool addrAck = (Wire1.endTransmission() == 0);

  Wire1.beginTransmission(0x55);
  bool foreignAck = (Wire1.endTransmission() == 0);

  bool cmdAck = false;
  if (addrAck && !foreignAck) {
    Wire1.beginTransmission(addr);
    Wire1.write(0x00);
    Wire1.write(0xAE);
    cmdAck = (Wire1.endTransmission() == 0);
  }
  Wire1.end();

  bool ok = display_policy::acceptOledProbe(addrAck, cmdAck, foreignAck);
  Serial.printf("# [oled_diag] probe SDA=%d SCL=%d addr=0x%02X addrAck=%d cmdAck=%d foreign=%d -> %s\n",
                sda, scl, addr, addrAck ? 1 : 0, cmdAck ? 1 : 0, foreignAck ? 1 : 0,
                ok ? "ACCEPT" : "reject");
  return ok;
}

static ScanResult scanPairs(int count, display_policy::PinPair (*at)(int)) {
  ScanResult res;
  for (int i = 0; i < count; i++) {
    display_policy::PinPair pair = at(i);
    if (pair.sda < 0 || pair.scl < 0) continue;
    for (uint8_t addr : { (uint8_t)0x3C, (uint8_t)0x3D }) {
      if (probePair(pair.sda, pair.scl, addr)) {
        res.found = true;
        res.sdaPin = pair.sda;
        res.sclPin = pair.scl;
        res.address = addr;
        Serial.printf("# [oled_diag] OLED accepted at 0x%02X SDA=%d SCL=%d\n", addr, pair.sda, pair.scl);
        return res;
      }
    }
  }
  return res;
}

ScanResult scanAllCandidatePins() {
  return scanPairs(display_policy::kOledPinCandidateCount, display_policy::oledPinCandidate);
}

ScanResult scanKnownHeaders() {
  return scanPairs(display_policy::kOledHeaderCandidateCount, display_policy::oledHeaderCandidate);
}

static bool sendFrame(uint8_t addr, const display_policy::I2cFrame& frame) {
  Wire1.beginTransmission(addr);
  for (uint8_t i = 0; i < frame.len; i++) {
    Wire1.write(frame.data[i]);
  }
  uint8_t err = Wire1.endTransmission();
  if (err != 0) {
    Serial.printf("# [oled_diag] I2C frame len=%u FAILED err=%u\n", frame.len, err);
  }
  return err == 0;
}

static bool sendSequence(uint8_t addr, display_policy::OledSeq seq) {
  display_policy::I2cFrame frames[24];
  size_t n = display_policy::buildOledFrames(seq, frames, 24);
  bool ok = n > 0;
  for (size_t i = 0; i < n; i++) {
    if (!sendFrame(addr, frames[i])) ok = false;
  }
  return ok;
}

// Fills all 8 pages of 128x64 display memory with a byte pattern
static bool fillDisplayRam(uint8_t addr, uint8_t pattern) {
  bool allOk = true;

  display_policy::I2cFrame horiz;
  horiz.len = 3;
  horiz.data[0] = 0x00;
  horiz.data[1] = 0x20;
  horiz.data[2] = 0x00;
  sendFrame(addr, horiz);

  for (uint8_t page = 0; page < 8; page++) {
    display_policy::I2cFrame pageCmds;
    pageCmds.len = 4;
    pageCmds.data[0] = 0x00;
    pageCmds.data[1] = (uint8_t)(0xB0 + page);
    pageCmds.data[2] = 0x00;
    pageCmds.data[3] = 0x10;
    sendFrame(addr, pageCmds);

    // Send 128 bytes in 4 chunks of 32 bytes (fitting standard Wire 128-byte buffer)
    for (int chunk = 0; chunk < 4; chunk++) {
      Wire1.beginTransmission(addr);
      Wire1.write(0x40); // Co=0, D/C=1: Data byte stream
      for (int i = 0; i < 32; i++) {
        Wire1.write(pattern);
      }
      if (Wire1.endTransmission() != 0) {
        allOk = false;
      }
    }
  }
  return allOk;
}

bool forceHardwareLightUp(int sda, int scl, uint8_t addr) {
  Serial.printf("# [oled_diag] forceHardwareLightUp on SDA=%d, SCL=%d, Addr=0x%02X...\n", sda, scl, addr);

  Wire1.end();
  delay(10);
  if (!Wire1.begin(sda, scl, 100000)) {
    Serial.println("# [oled_diag] Wire1.begin() FAILED!");
    return false;
  }
  Wire1.setTimeOut(100);

  bool initOk = sendSequence(addr, display_policy::OledSeq::BringUp);
  delay(10);
  display_policy::I2cFrame onAgain;
  onAgain.len = 2;
  onAgain.data[0] = 0x00;
  onAgain.data[1] = 0xAF;
  bool onOk = sendFrame(addr, onAgain);
  Serial.printf("# [oled_diag] Bring-up %s, display-on %s\n",
                initOk ? "ACK" : "NACK", onOk ? "ACK" : "NACK");
  Wire1.end();
  return initOk && onOk;
}

bool runVisualTest(int sda, int scl, uint8_t addr) {
  Serial.printf("# [oled_diag] Starting Visual Self-Test on SDA=%d, SCL=%d, 0x%02X...\n", sda, scl, addr);

  Wire1.end();
  delay(10);
  if (!Wire1.begin(sda, scl, 100000)) {
    Serial.println("# [oled_diag] Wire1.begin() FAILED during visual test!");
    return false;
  }
  Wire1.setTimeOut(100);

  bool lit = sendSequence(addr, display_policy::OledSeq::BringUp);
  delay(10);
  lit = sendSequence(addr, display_policy::OledSeq::AllPixelsOn) && lit;
  Serial.printf("# [oled_diag] Stage 1 all-pixels-on: %s\n", lit ? "ACK" : "NACK");
  if (!lit) {
    Wire1.end();
    return false;
  }
  delay(1500);

  Serial.println("# [oled_diag] Stage 2: Solid White Framebuffer (0xFF) - 1s...");
  display_policy::I2cFrame ramOn;
  ramOn.len = 2;
  ramOn.data[0] = 0x00;
  ramOn.data[1] = 0xA4;
  sendFrame(addr, ramOn);
  fillDisplayRam(addr, 0xFF);
  delay(1000);

  // 4. STAGE 3: Alternating Horizontal Stripes (0xAA)
  Serial.println("# [oled_diag] Stage 3: Alternating Stripes (0xAA) - 800ms...");
  fillDisplayRam(addr, 0xAA);
  delay(800);

  // 5. STAGE 4: Inverse Stripes (0x55)
  Serial.println("# [oled_diag] Stage 4: Inverse Stripes (0x55) - 800ms...");
  fillDisplayRam(addr, 0x55);
  delay(800);

  // 6. STAGE 5: Solid Frame with Clear Center (Border Test)
  Serial.println("# [oled_diag] Stage 5: Screen Border Test...");
  for (uint8_t page = 0; page < 8; page++) {
    display_policy::I2cFrame pageCmds;
    pageCmds.len = 4;
    pageCmds.data[0] = 0x00;
    pageCmds.data[1] = (uint8_t)(0xB0 + page);
    pageCmds.data[2] = 0x00;
    pageCmds.data[3] = 0x10;
    sendFrame(addr, pageCmds);

    for (int chunk = 0; chunk < 4; chunk++) {
      Wire1.beginTransmission(addr);
      Wire1.write(0x40);
      for (int c = 0; c < 32; c++) {
        int col = chunk * 32 + c;
        uint8_t byteVal = 0x00;
        if (page == 0) byteVal |= 0x01; // Top border
        if (page == 7) byteVal |= 0x80; // Bottom border
        if (col == 0 || col == 127) byteVal = 0xFF; // Left / Right border
        Wire1.write(byteVal);
      }
      Wire1.endTransmission();
    }
  }
  delay(1000);

  sendSequence(addr, display_policy::OledSeq::AllPixelsOn);
  Wire1.end();
  Serial.println("# [oled_diag] Visual self-test finished. Panel left all-pixels-on.");
  return true;
}

String getPinDiagnosticsReport() {
  String out = "";
  struct PinCheck {
    int pin;
    const char* name;
  };
  const PinCheck checks[] = {
    { pins::I2C_SDA, "IO8 (I2C SDA)" },
    { pins::I2C_SCL, "IO9 (I2C SCL)" },
    { pins::TFT_MOSI, "IO41 (Display MOSI / SCL/SDA)" },
    { pins::TFT_SCLK, "IO42 (Display SCLK / SDA/SCL)" },
    { pins::TFT_DC,   "IO40 (Display DC)" },
    { pins::TFT_RST,  "IO39 (Display RST)" },
    { pins::TFT_CS,   "IO38 (Display CS)" },
    { pins::TFT_BLK,  "IO21 (Display BLK)" },
    { pins::IR_L, "IO10 (IR Left)" },
    { pins::IR_R, "IO11 (IR Right)" },
    { pins::US_TRIG, "IO12 (Ultrasonic Trig)" },
    { pins::US_ECHO, "IO13 (Ultrasonic Echo)" },
    { 17, "IO17 (Pin 9)" },
    { 18, "IO18 (Pin 10)" },
    { 1, "IO1 (ADC)" },
    { 2, "IO2 (Touch)" }
  };

  out += "=== GPIO Electrical State Audit (Pull-Down vs Pull-Up) ===\n";
  for (const auto& c : checks) {
    pinMode(c.pin, INPUT_PULLDOWN);
    delayMicroseconds(200);
    int pd = digitalRead(c.pin);

    pinMode(c.pin, INPUT_PULLUP);
    delayMicroseconds(200);
    int pu = digitalRead(c.pin);

    pinMode(c.pin, INPUT);

    String status;
    if (pd == 1) {
      status = "[+] ACTIVE 3.3V PULL-UP DETECTED! (Device is connected and powered!)";
    } else if (pu == 0) {
      status = "[-] GROUNDED / SHORTED TO GND! (Pin is clamped to 0V!)";
    } else {
      status = "[o] Floating / No external pull-up";
    }

    out += String("  ") + c.name + ": " + status + "\n";
  }
  return out;
}

static bool pinHasExternalPullUp(int pin) {
  pinMode(pin, INPUT_PULLDOWN);
  delayMicroseconds(200);
  int pulledHigh = digitalRead(pin);
  pinMode(pin, INPUT);
  return pulledHigh == 1;
}

bool spiHeaderConnected() {
  return display_policy::spiHeaderPresent(
      pinHasExternalPullUp(pins::TFT_MOSI),
      pinHasExternalPullUp(pins::TFT_SCLK),
      pinHasExternalPullUp(pins::TFT_DC),
      pinHasExternalPullUp(pins::TFT_RST),
      pinHasExternalPullUp(pins::TFT_CS));
}

static void spiShiftByte(uint8_t value) {
  for (int bit = 7; bit >= 0; --bit) {
    digitalWrite(pins::TFT_MOSI, (value & (1 << bit)) ? HIGH : LOW);
    digitalWrite(pins::TFT_SCLK, HIGH);
    digitalWrite(pins::TFT_SCLK, LOW);
  }
}

bool forceSpiPanelOn() {
  const int lines[] = {
    pins::TFT_SCLK, pins::TFT_MOSI, pins::TFT_DC,
    pins::TFT_RST, pins::TFT_CS, pins::TFT_BLK
  };
  for (int pin : lines) {
    gpio_reset_pin((gpio_num_t)pin);
    pinMode(pin, OUTPUT);
  }
  digitalWrite(pins::TFT_SCLK, LOW);
  digitalWrite(pins::TFT_MOSI, LOW);
  digitalWrite(pins::TFT_DC, LOW);
  digitalWrite(pins::TFT_CS, HIGH);
  digitalWrite(pins::TFT_BLK, HIGH);
  digitalWrite(pins::TFT_RST, HIGH);
  delay(5);
  digitalWrite(pins::TFT_RST, LOW);
  delay(20);
  digitalWrite(pins::TFT_RST, HIGH);
  delay(50);

  size_t count = 0;
  const display_policy::Cmd* cmds = display_policy::bringUpCommands(&count);
  for (size_t i = 0; i < count; i++) {
    digitalWrite(pins::TFT_CS, LOW);
    digitalWrite(pins::TFT_DC, LOW);
    spiShiftByte(cmds[i].b[0]);
    if (cmds[i].n > 1) spiShiftByte(cmds[i].b[1]);
    digitalWrite(pins::TFT_CS, HIGH);
  }
  digitalWrite(pins::TFT_CS, LOW);
  spiShiftByte(0xA5);
  digitalWrite(pins::TFT_CS, HIGH);
  Serial.println("# [oled_diag] SPI header lamp: bring-up + 0xA5 on IO41/IO42");
  return true;
}

bool pulseAllPossibleOledHeaders() {
  auto scan = scanAllCandidatePins();
  if (scan.found) {
    forceHardwareLightUp(scan.sdaPin, scan.sclPin, scan.address);
    store::setOledConfig(scan.sdaPin, scan.sclPin, scan.address);
    return true;
  }
  return false;
}

} // namespace oled_diag
