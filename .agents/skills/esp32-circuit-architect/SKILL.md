---
name: esp32-circuit-architect
description: Use when designing circuits, wiring sensors, actuators, or power modules with ESP32/microcontrollers, or when verifying and cataloging hardware components
allowed-tools:
  - bash
  - read
  - write
  - glob
---

# ESP32 Circuit Architect & Hardware Design Lab

## Overview

**Core principle:** NEVER wire, code, or power an embedded circuit without verified pinouts, voltage levels, and safety bounds. Guesswork burns silicon.

Every hardware component introduced to a project MUST undergo systematic research, unambiguous pinout mapping, strict electrical validation, and visual interactive design before any physical wire is touched.

---

## When to Use

Use this skill whenever:
- Designing a new circuit or pinout mapping with ESP32 / ESP32-S3 / microcontrollers.
- Adding a new sensor, actuator, motor driver, display, or power management module to a robotics/IoT project.
- Creating visual, interactive wiring diagrams for makers, students, or vibe coders.
- Performing electrical sanity audits (verifying pin safety, logic levels, strapping pins, and power budgets).
- Querying or expanding the built-in database of 110+ curated IoT/Robotics components.

**When NOT to use:**
- Pure software algorithms with no microcontroller or peripheral hardware interface.
- Standard high-level web frontend/backend work unrelated to IoT devices.

---

## The Iron Laws of Embedded Hardware

```
1. NEVER FEED 5V DIRECTLY INTO AN ESP32 GPIO (STRICTLY 3.3V MAX).
2. NEVER POWER INDUCTIVE LOADS (MOTORS/SERVOS) FROM ESP32 3.3V/5V LDO RAILS.
3. NEVER USE RESERVED FLASH/PSRAM PINS (GPIO 26-37 ON ESP32-S3 N16R8, GPIO 6-11 ON ESP32).
4. NEVER GUESS PINOUTS WHEN COMPONENT VARIANTS ARE AMBIGUOUS — STOP AND ASK.
5. NO SCHEMATIC IS COMPLETE WITHOUT JUMPER HOPS, PIN LABELS, AND PASSED AUDITS.
6. NEVER COLLAPSE MULTIPLE WIRES ONTO A SINGLE MIDPOINT TRUNK — USE CHANNEL RIBBON BUS (PITCH 12-14PX).
7. NEVER ALLOW AMBIGUOUS WIRE CROSSINGS — ALL CROSSINGS MUST RENDER SEMICIRCULAR JUMPER HOPS (R=6PX).
```

---

## The 4-Phase Engineering Protocol

You MUST follow all four phases sequentially:

```
[Phase 1: Research & Cataloging]
               │
               ▼
[Phase 2: Ambiguity Clarification Gate] (Stop if underspecified!)
               │
               ▼
[Phase 3: Interactive Visual Schematic Generation]
               │
               ▼
[Phase 4: Self-Verification & Electrical Sanity Audit]
```

---

### Phase 1: Deep-Dive Component Research & Cataloging

When a new component or module is requested:

1. **Query the Local Catalog First:**
   Check if the component already exists in the built-in database:
   ```bash
   python3 scripts/component_lookup.py "<component_name_or_keyword>"
   ```
   Reference file: `references/esp32_100_components.json` (110+ modules cataloged).

2. **If New / Uncataloged, Perform Mandatory Research:**
   Extract and document the following 7 parameters:
   - **Exact Model & Revision:** (e.g. ESP32 DevKit V1 30-pin vs NodeMCU 38-pin vs S3 40-pin; HC-SR04 5V vs HC-SR05 multi-mode; PCA9685 default 0x40).
   - **Operating Voltage & Logic Level:** (3.3V, 5V, or external motor rail 6-12V).
   - **Current Consumption:** Quiescent current vs peak/stall current (mA).
   - **Interface Bus & Protocol:** (I2C address, SPI mode, UART baud, PWM 50Hz, Analog ADC, 1-Wire).
   - **Pinout Definitions:** Name, pin type (`pwr`, `gnd`, `sig`, `na`), and function.
   - **DOs (Được làm):** Safe usage best practices, decoupling capacitors, pullups.
   - **DON'Ts (Cấm làm):** Prohibitions that cause latch-up, overvoltage damage, or boot-loops.

3. **Persist the Knowledge:**
   Append new verified components into the catalog so future sessions benefit:
   ```bash
   python3 scripts/component_lookup.py --add-json new_component.json
   ```

---

### Phase 2: Ambiguity Clarification Gate (STRICT STOPPING CONDITION)

**You MUST STOP and ask the user for clarification if any of the following occur:**

| Ambiguity Trigger | Why Guessing Fails | Action Required |
| :--- | :--- | :--- |
| **DevKit Pin Count Unknown** | 30-pin, 38-pin, and 40-pin boards have totally different pin layouts. | Ask user for photo, board markings, or chip model (`esp32-wroom` vs `esp32-s3`). |
| **Sensor Variant Ambiguity** | HC-SR04 outputs 5V (damages ESP32); HC-SR05/RCWL-1601 runs on 3.3V. | Inquire whether user has logic level shifter, resistor divider, or 3.3V version. |
| **Buzzer Type Ambiguity** | Active buzzers need digital HIGH/LOW; Passive buzzers need PWM frequency. | Ask if module has black resin seal with sticker (active) or open coil (passive). |
| **I2C Address Collision** | Default addresses clash (e.g. MPU6050 & BMI160 both on 0x68/0x69). | Inquire if AD0 address jumper is soldered or multiplexer is used. |
| **Power Source Unspecified** | Servos running on USB power will brown out ESP32. | Confirm battery type (1S Li-ion + Boost vs 2S 7.4V vs 5V adapter). |

---

### Phase 3: Interactive Visual Schematic Generation

When generating circuit documentation, NEVER provide flat ASCII text, messy overlapping SVG lines, or midpoint-collapsed trunks. Build a rich, interactive HTML/SVG canvas based on `templates/wiring_canvas_template.html`.

**Mandatory Visual Design Rules:**
1. **Clear Pin Labels & Toggle Mode (Chế Độ Hiện Tên Chân Pin):** Display legible monospace text labels right next to every component pin dot (`3V3`, `IO4`, `SDA`, `SCL`, `GND`, `DIN`...). Distinct visual badges for Power (`red`), Ground (`black`), Signal (`blue/cyan/gold`), and Reserved/Forbidden (`gray/red outline`). Provide a prominent top toolbar toggle button `[🏷️ Tên Chân]` (Keyboard Shortcut `L`) so makers can view all pin labels at a glance without hover guessing.
2. **Channel Ribbon Bus Routing (Phân Luồng Dây Băng Ribbon - Pitch 12-14px):**
   - NEVER collapse wires between two components into a single midpoint X (`(from.x + to.x) / 2`). That creates an unreadable clump.
   - Bundle wires by connected component pairs (`[c1, c2].sort().join('--')`).
   - **Monotonic Pin Ordering:** Sort pins within each bundle monotonically by destination Y coordinate (`b.to.y - a.to.y`) so parallel wires flow naturally without internal crossing.
   - Allocate discrete vertical trunks spaced by a pitch of 12-14px: $X_{\text{trunk}} = X_{\text{start}} + \text{idx} \times \text{pitch}$.
3. **Global Vertical Trunk Conflict Resolver (Bộ Giải Quyết Xung Đột Đường Thân):**
   - Perform a multi-pass sweep (up to 6 passes) scanning all vertical trunks across different wire bundles.
   - If two vertical segments share nearly the same X coordinate ($|x_1 - x_2| < 8\text{px}$) and their vertical Y spans overlap by $> 6\text{px}$, push the second trunk outward by $+14\text{px}$ until 0 vertical trunk overlaps remain.
4. **Directional Semicircular Crossing Bridge Hops (Cầu Nhảy Dây Bán Nguyệt R=6px):**
   - Wires MUST NEVER cross as ambiguous intersecting lines. Wires only connect when a junction dot (`●`) is present.
   - Any vertical trunk crossing another wire's horizontal segment must insert a semicircular arch ($R=6\text{px}$):
     - Downward flow: `L x (cy - 6) A 6 6 0 0 1 x (cy + 6)`
     - Upward flow: `L x (cy + 6) A 6 6 0 0 0 x (cy - 6)`
     *(Both bulge outward to +X for clean visual rhythm).*
5. **Filleted Manhattan Corners (Bo Góc 90° Mềm Mại R=8px):**
   - Replace harsh 90° corners with smooth quadratic bezier curves ($R=8\text{px}$): `Q c.currX c.currY, c.outX c.outY`.
6. **Obstacle-Avoidance Wire Routing & Z-Index Overlay (Thuật Giải Né Linh Kiện & Lớp Nổi):**
   - Wires must NEVER cut through or dive behind component cards. All routes must navigate through designated wiring corridors around component bounding boxes.
   - Active/hovered wires must be elevated to a top SVG overlay (`#wire-top-overlay`) above all component cards.
7. **Dual-Endpoint Interactive Callouts (Huy Hiệu 2 Đầu Chân Nối):**
   - When hovering or clicking any wire, immediately spawn floating glowing callout badges (`.endpoint-callout`) attached directly at both endpoints (`📍 [Component A]: [Pin A]` and `📍 [Component B]: [Pin B]`), accompanied by a pulsating neon halo (`.pin-pulse-active`) on both pin dots so users immediately know exactly which two pins to plug.
8. **Distinct Wire Colors per Component:** Prevent visual spaghetti by assigning dedicated color palettes to each peripheral (e.g. Left Servo: Orange/Pink/Brown; Right Servo: Magenta/Crimson/Dark Brown; I2S Mic: Yellow/Green/SkyBlue; I2S Amp: Orange/Purple/Cyan; Touch: Pink/Rose).
9. **Desktop Trackpad CAD Navigation & Mobile Touch Gestures:**
   - Desktop: Two-finger scroll to pan infinitely; two-finger pinch to zoom anchored at cursor; Spacebar + Drag to pan.
   - Mobile: 1-Finger drag to pan canvas; 2-Finger pinch to zoom smoothly centered at midpoint; Double-tap to auto-fit (`fitToScreen()`).
   - Bottom Carousel / Drawer with slide-up sheet on mobile viewports ($\le 768\text{px}$). Touch targets $\ge 44\times 44\text{px}$.
10. **Component Inspector & BOM Shopping Guide:**
    - Click any component to reveal full electrical ratings, pin table, and DOs/DON'Ts.
    - Top-bar `🛒 BOM Shopping Guide` button displaying exact commercial names, quantities, and 1-click Shopee search keywords.

---

### Phase 4: Self-Verification & Electrical Sanity Audit

Before claiming any circuit design is ready:

1. **Run Automated Circuit Verification:**
   Validate the circuit schema against silicon constraints:
   ```bash
   python3 scripts/circuit_checker.py <circuit_definition.json>
   ```
   **Verification Checklist:**
   - [ ] No 5V signal feeds directly into any ESP32 input pin without level shifter or divider.
   - [ ] No pins assigned to ESP32-S3 Flash/PSRAM bus (GPIO 26-37 on N16R8) or ESP32 Flash (GPIO 6-11).
   - [ ] Strapping pins are safe: GPIO 45 NOT pulled high on S3 (prevents 1.8V flash brick); GPIO 0/2/12/15 safe.
   - [ ] No motors or servos powered directly from ESP32 3.3V or 5V regulator.
   - [ ] Common ground (GND) tied together across all power rails.
   - [ ] Reverse protection / Schottky OR-ing diode present if USB and external battery coexist.
   - [ ] I2C bus devices have distinct non-colliding hex addresses.
   - [ ] Analog sensors on ESP32 use ADC1 channels if WiFi/Bluetooth will be active.

2. **Visual & Accessibility UX Audit:**
   For HTML schematics, run headless visual inspection:
   ```bash
   python3 scripts/ux_inspect.py --target docs/hardware/wiring_interactive.html --screenshots
   ```
   Target: **0 Blocker, 0 Major, 0 Minor defects**.

---

## Red Flags & Rationalization Table

| Rationalization (Excuse) | Reality & Hard Truth |
| :--- | :--- |
| *"The sensor runs on 5V but Echo probably won't hurt the ESP32 for a quick test."* | **5V onto a 3.3V unbuffered CMOS gate causes parasitic latch-up and burns the input pin in seconds.** |
| *"I'll just power the micro servo from the ESP32 5V/VIN pin since it's only one servo."* | **Servo motor start current spikes exceed 800mA, pulling VIN below 4.2V and triggering Brownout Reset loops.** |
| *"GPIO 34 works fine as an LED output on Arduino, so it should work on ESP32."* | **GPIO 34-39 on ESP32 are physically Input-Only (GPI). Writing HIGH does nothing; hardware cannot output current.** |
| *"I don't need a diode because the user won't plug in USB while the battery is ON."* | **Users WILL plug in USB for debugging while the battery is ON. Without a Schottky diode, back-feeding burns USB ports.** |
| *"The schematic looks fine even though wire lines cross without hops."* | **Crossing lines without hops cause beginners to solder shorts, confusing cross-overs with shared junctions.** |

---

## Quick Reference: ESP32 Pin Selection Cheat-Sheet

### ESP32-S3 (DevKit 40-Pin N16R8)
- **Safe General Purpose I/O:** GPIO 1, 2, 4, 5, 6, 7, 8 (SDA), 9 (SCL), 10, 11, 12, 13, 14, 15, 16, 17, 18, 21, 38, 39, 40, 41, 42.
- **Native USB OTG:** GPIO 19 (D-), GPIO 20 (D+). Do not use for external hardware if USB-CDC is active.
- **Boot & Flash Reserved (DO NOT TOUCH):** GPIO 26 through 37 (Octal SPI Flash & PSRAM bus).
- **Dangerous Strapping Pin:** GPIO 45 (MUST NOT be pulled HIGH at boot: switches VDD_SPI to 1.8V and bricks flash).
- **UART0 Boot Log:** GPIO 43 (TX), GPIO 44 (RX).
- **ADC Channels:** ADC1 = GPIO 1 - 10 (WiFi safe). ADC2 = GPIO 11 - 20 (disabled during WiFi transmission).

### Classic ESP32 (WROOM-32 30-Pin / 38-Pin)
- **Safe General Purpose I/O:** GPIO 4, 5, 13, 14, 16, 17, 18, 19, 21 (SDA), 22 (SCL), 23, 25, 27, 32, 33.
- **Input ONLY (No Output, No Internal Pullup):** GPIO 34, 35, 36 (VP), 39 (VN). Must use external 10k pullup/pulldown.
- **Internal SPI Flash (FORBIDDEN):** GPIO 6, 7, 8, 9, 10, 11.
- **Strapping Pins (Avoid external pull during boot):** GPIO 0, 2 (Onboard Blue LED), 12 (MTDI), 15 (MTDO).
- **ADC Channels:** ADC1 = GPIO 32 - 39 (WiFi safe). ADC2 = GPIO 0, 2, 4, 12-15, 25-27 (fails when WiFi is active).
