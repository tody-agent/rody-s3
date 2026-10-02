#!/usr/bin/env python3
"""
Automated Circuit & Pinout Safety Verifier for ESP32 and ESP32-S3
Validates schematic connections against electrical limits, strapping pins, flash buses, and I2C collisions.
"""

import sys
import os
import json
import argparse

ESP32_CONSTRAINTS = {
    "esp32_classic": {
        "forbidden_flash_gpios": [6, 7, 8, 9, 10, 11],
        "input_only_gpios": [34, 35, 36, 37, 38, 39],
        "strapping_gpios": [0, 2, 12, 15],
        "adc2_gpios": [0, 2, 4, 12, 13, 14, 15, 25, 26, 27],
        "adc1_gpios": [32, 33, 34, 35, 36, 37, 38, 39],
        "max_gpio_voltage": 3.6,
        "max_ldo_3v3_ma": 300
    },
    "esp32_s3": {
        "forbidden_flash_psram_gpios": list(range(26, 38)), # 26-37
        "forbidden_strapping_high_on_boot": [45], # Pin 45 HIGH sets flash voltage to 1.8V -> bricks!
        "strapping_gpios": [0, 3, 45, 46],
        "native_usb_gpios": [19, 20],
        "uart0_gpios": [43, 44],
        "adc1_gpios": [1, 2, 3, 4, 5, 6, 7, 8, 9, 10],
        "adc2_gpios": [11, 12, 13, 14, 15, 16, 17, 18, 19, 20],
        "max_gpio_voltage": 3.6,
        "max_ldo_3v3_ma": 500
    }
}

def verify_circuit(circuit_data):
    issues = []
    chip = circuit_data.get("chip", "esp32_s3")
    rules = ESP32_CONSTRAINTS.get(chip, ESP32_CONSTRAINTS["esp32_s3"])
    
    components = circuit_data.get("components", [])
    wires = circuit_data.get("connections", [])
    wifi_enabled = circuit_data.get("wifi_enabled", False)

    print(f"\n🔍 Running Hardware & Circuit Sanity Audit for [{chip.upper()}]...")

    # 1. Check I2C Address Collisions
    i2c_addresses = {}
    for c in components:
        addr = c.get("i2c_address")
        if addr:
            addr_norm = addr.lower()
            if addr_norm in i2c_addresses:
                issues.append({
                    "severity": "BLOCKER",
                    "type": "I2C_COLLISION",
                    "message": f"I2C Address Collision: Both '{i2c_addresses[addr_norm]}' and '{c['name']}' use address {addr}. Use I2C Multiplexer (TCA9548A) or change address jumper."
                })
            else:
                i2c_addresses[addr_norm] = c["name"]

    # 2. Check Power Budget & Heavy Loads on 3.3V
    total_3v3_current = 0
    total_5v_current = 0
    for c in components:
        v_in = str(c.get("power_source", "")).upper()
        current = c.get("current_ma", 0)
        
        # Check if motors/servos are placed on 3.3V rail
        if ("SERVO" in c.get("type", "").upper() or "MOTOR" in c.get("type", "").upper()):
            if "3V3" in v_in or "3.3V" in v_in or "ESP32" in v_in:
                issues.append({
                    "severity": "BLOCKER",
                    "type": "INDUCTIVE_LOAD_ON_LOGIC",
                    "message": f"CRITICAL: Inductive motor '{c['name']}' is powered from logic 3.3V/ESP32 rail! This will cause brownout resets and fried LDO. Must use external 5V/6V supply."
                })

        if "3.3V" in v_in or "3V3" in v_in:
            total_3v3_current += current
        else:
            total_5v_current += current

    max_3v3 = rules.get("max_ldo_3v3_ma", 500)
    if total_3v3_current > max_3v3:
        issues.append({
            "severity": "MAJOR",
            "type": "OVERCURRENT_3V3",
            "message": f"Total 3.3V load ({total_3v3_current}mA) exceeds onboard LDO capacity ({max_3v3}mA). Add external 3.3V regulator."
        })

    # 3. Check Individual Wire Connections to ESP32 Pins
    used_gpios = {}
    for w in wires:
        esp_pin = w.get("esp_pin")
        if esp_pin is None:
            continue
            
        # Parse GPIO number if given as string "GPIO8" or int 8
        try:
            gpio_num = int(str(esp_pin).replace("GPIO", "").replace("IO", "").strip())
        except ValueError:
            continue

        direction = w.get("direction", "IN") # IN, OUT, BIDIR, PWR
        target_comp = w.get("target_component", "Unknown")
        target_signal = w.get("target_signal", "SIG")
        target_voltage = w.get("target_voltage", 3.3)

        # Check for duplicate GPIO usage
        if gpio_num in used_gpios:
            prev = used_gpios[gpio_num]
            # Unless it's I2C SDA/SCL, sharing is an issue
            if not ("SDA" in target_signal or "SCL" in target_signal):
                issues.append({
                    "severity": "MAJOR",
                    "type": "GPIO_CONFLICT",
                    "message": f"GPIO {gpio_num} is assigned to both '{prev}' and '{target_comp} ({target_signal})'."
                })
        else:
            used_gpios[gpio_num] = f"{target_comp} ({target_signal})"

        # ESP32-S3 Flash/PSRAM Reserved Pins
        if chip == "esp32_s3":
            if gpio_num in rules["forbidden_flash_psram_gpios"]:
                issues.append({
                    "severity": "BLOCKER",
                    "type": "FORBIDDEN_FLASH_GPIO",
                    "message": f"GPIO {gpio_num} used by '{target_comp}' is RESERVED for Octal SPI Flash/PSRAM on ESP32-S3 (N16R8)! Using it will crash the MCU."
                })
            if gpio_num in rules.get("forbidden_strapping_high_on_boot", []):
                if w.get("pulled_high_at_boot", False):
                    issues.append({
                        "severity": "BLOCKER",
                        "type": "STRAPPING_BRICK_RISK",
                        "message": f"GPIO {gpio_num} is pulled HIGH at boot by '{target_comp}'. On ESP32-S3, GPIO45 HIGH sets flash voltage to 1.8V and BRICKS bootloader!"
                    })
            if gpio_num in rules["native_usb_gpios"]:
                if circuit_data.get("usb_cdc_enabled", True):
                    issues.append({
                        "severity": "MAJOR",
                        "type": "USB_CONFLICT",
                        "message": f"GPIO {gpio_num} is used by Native USB OTG/CDC. Avoid using for '{target_comp}'."
                    })

        # Classic ESP32 Rules
        elif chip == "esp32_classic":
            if gpio_num in rules["forbidden_flash_gpios"]:
                issues.append({
                    "severity": "BLOCKER",
                    "type": "FORBIDDEN_FLASH_GPIO",
                    "message": f"GPIO {gpio_num} used by '{target_comp}' is physically connected to internal SPI Flash! Cannot be used."
                })
            if gpio_num in rules["input_only_gpios"] and direction == "OUT":
                issues.append({
                    "severity": "BLOCKER",
                    "type": "INPUT_ONLY_AS_OUTPUT",
                    "message": f"GPIO {gpio_num} used by '{target_comp}' is INPUT ONLY on ESP32! It cannot drive output signals."
                })

        # Voltage level check: 5V into ESP32 input pin
        if direction in ["IN", "BIDIR"] and target_voltage > 3.6:
            has_level_shifter = w.get("level_shifter", False) or w.get("voltage_divider", False)
            if not has_level_shifter:
                issues.append({
                    "severity": "BLOCKER",
                    "type": "OVERVOLTAGE_PIN",
                    "message": f"5V signal from '{target_comp} ({target_signal})' is connected directly to GPIO {gpio_num} without Level Shifter or Voltage Divider! ESP32 inputs are strictly 3.3V!"
                })

        # ADC2 and WiFi conflict
        if wifi_enabled and gpio_num in rules.get("adc2_gpios", []):
            if "ADC" in target_signal or "ANALOG" in target_signal:
                issues.append({
                    "severity": "MAJOR",
                    "type": "ADC2_WIFI_CONFLICT",
                    "message": f"Analog sensor '{target_comp}' is connected to GPIO {gpio_num} (ADC2). ADC2 cannot be read while WiFi is active. Move to ADC1 (GPIO {rules['adc1_gpios'][:4]})."
                })

    # Summary Report
    blockers = [i for i in issues if i["severity"] == "BLOCKER"]
    majors = [i for i in issues if i["severity"] == "MAJOR"]
    minors = [i for i in issues if i["severity"] == "MINOR"]

    print(f"\n📊 Verification Result:")
    print(f"   Blockers: {len(blockers)} | Majors: {len(majors)} | Minors: {len(minors)}")
    
    if issues:
        for idx, i in enumerate(issues, 1):
            badge = "🛑 [BLOCKER]" if i["severity"] == "BLOCKER" else "⚠️ [MAJOR]"
            print(f"   {idx}. {badge} ({i['type']}): {i['message']}")
    else:
        print("   ✅ ALL ELECTRICAL & PINOUT CHECKS PASSED PERFECTLY!")

    return len(blockers) == 0

def main():
    parser = argparse.ArgumentParser(description="Circuit and Pinout Safety Verifier")
    parser.add_argument("circuit_json", help="Path to circuit configuration JSON")
    args = parser.parse_args()

    if not os.path.exists(args.circuit_json):
        print(f"Error: File {args.circuit_json} not found.")
        sys.exit(1)

    with open(args.circuit_json, "r", encoding="utf-8") as f:
        data = json.load(f)

    passed = verify_circuit(data)
    sys.exit(0 if passed else 1)

if __name__ == "__main__":
    main()
