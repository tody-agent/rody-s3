# Canvas Design & Visual Blueprint Standard

The interactive schematic canvas supports dual themes (**Dark CAD** `#090d16` and **Light Blueprint** `#f8fafc`) with rich, authentic component solder-mask colors, high-contrast non-colliding wire inks, and professional CAD navigation.

---

## 1. Color Architecture & Dual Theme Tokens

All color tokens live in `:root` (Dark CAD) and `body.theme-light` (Light Blueprint).

### Core Theme Backgrounds & Surfaces

| Token | Dark CAD Mode | Light Blueprint Mode | Function |
| :--- | :--- | :--- | :--- |
| `--bg` | `#090d16` (Obsidian) | `#f8fafc` (Engineering Paper) | Canvas background |
| `--surface` | `#111827` (Dark Slate) | `#ffffff` (Pure White Card) | Toolbars & modal dialogs |
| `--border` | `#1e293b` (Subtle Slate) | `#cbd5e1` (Crisp Blueprint Border) | Structural boundaries |
| `--fg` | `#f8fafc` (Luminous White) | `#0f172a` (Charcoal Slate-900) | Primary typography |
| `--wire-casing` | `#090d16` (Dark Separator) | `#ffffff` (White Casing Sleeve) | Ribbon bus isolation sleeve |

### Harmonious, Non-Colliding Wire Palette (WCAG AA ≥ 4.5:1)

Every functional subsystem possesses an instantly distinguishable color signature so makers never confuse power rails with signal buses:

| Wire Bus / Net | Light Blueprint Ink | Dark CAD Ink | Visual Role |
| :--- | :--- | :--- | :--- |
| **5V / 5.1V Power** | `#dc2626` (Red-600) | `#ef4444` (Bright Red) | Main servo & amp power rail |
| **3.3V Logic Rail** | `#d97706` (Amber-600) | `#f59e0b` (Warm Amber) | Clean regulated 3.3V IC power |
| **VBAT (Battery 3.7V–4.2V)** | `#ea580c` (Orange-600) | `#f97316` (Coral Orange) | Li-ion battery rail before boost |
| **GND (Mass)** | `#0f172a` (Slate-900) | `#0f172a` (Stroke `#475569`) | Common star ground |
| **I2C Bus SDA** | `#0284c7` (Sky-600) | `#eab308` (Gold) | I2C Serial Data (PCA9685, MPU6050) |
| **I2C Bus SCL** | `#7c3aed` (Violet-600) | `#8b5cf6` (Royal Violet) | I2C Serial Clock |
| **Display SPI SCL** | `#2563eb` (Blue-600) | `#0284c7` (Sky Blue) | ST7789 High-speed Clock |
| **Display SPI SDA** | `#059669` (Emerald-600) | `#059669` (Emerald) | ST7789 MOSI Pixel Data |
| **Display SPI DC** | `#4f46e5` (Indigo-600) | `#38bdf8` (Cyan) | Data / Command selector |
| **Display SPI RES** | `#e11d48` (Rose-600) | `#f97316` (Orange) | Hardware Reset line |
| **Display SPI CS** | `#9333ea` (Purple-600) | `#c084fc` (Light Purple) | Chip Select |
| **Display SPI BLK** | `#db2777` (Pink-600) | `#f43f5e` (Rose Pink) | Backlight PWM dimming |
| **Servo Left PWM** | `#f97316` (Orange-500) | `#f97316` (Orange) | Left drive wheel control |
| **Servo Right PWM** | `#c026d3` (Fuchsia-600) | `#d946ef` (Fuchsia) | Right drive wheel control |
| **Audio Mic SCK / WS / SD** | `#0891b2` / `#10b981` / `#4f46e5` | `#0891b2` / `#10b981` / `#38bdf8` | INMP441 24-bit I2S recording |
| **Audio Amp DIN / LRC / BCLK**| `#2563eb` / `#8b5cf6` / `#0284c7` | `#f97316` / `#8b5cf6` / `#0891b2` | MAX98357A I2S playback |
| **Speaker SPK+ / SPK-** | `#dc2626` / `#475569` | `#ef4444` / `#64748b` | Differential audio output |
| **IR Tachometer Left / Right** | `#16a34a` (Green) / `#0d9488` (Teal) | `#22c55e` / `#14b8a6` | Left & right wheel interrupt pulses |
| **Pet Touch (TTP223)** | `#ec4899` (Hot Pink) | `#ec4899` (Pink) | Head stroked tactile input |

---

## 2. Solder-Mask Realism (Strict No-Bleaching Rule)

- **NEVER bleach or convert component SVGs to monochrome (`recolorArtwork` is strictly forbidden).**
- Real-world hardware modules MUST look like physical circuit boards:
  - **ESP32-S3 DevKit:** Matte black PCB (`#131b2e` / `#182234`), silver metallic RF shield (`#94a3b8`), gold antenna trace (`#f59e0b`).
  - **MPU6050 (GY-521):** Industrial royal blue PCB (`#1d4ed8` / `#2563eb`), gold solder pads (`#ca8a04`).
  - **ST7789 TFT:** Crimson red bezel (`#991b1b`), glossy black LCD panel (`#030712`), expressive cyan cyber eyes (`#38bdf8`).
  - **PCA9685:** Royal blue PCB (`#1e3a8a`), vibrant green screw terminal block (`#16a34a`).
  - **TTP223 Touch:** Ruby red PCB (`#991b1b`) with gold concentric sensor pad (`#f59e0b`).
  - **INMP441 Mic:** Deep purple/black PCB (`#1e1b4b`), gold acoustic aperture ring (`#eab308`).
  - **MAX98357A DAC:** Deep indigo PCB (`#312e81`), cyan speaker terminal (`#0284c7`).
  - **Li-ion Battery:** Industrial green battery wrap (`#059669`).
  - **TP4056 Charger:** Cobalt blue PCB (`#1d4ed8`) with metallic silver Type-C receptacle.
- **Pin Silkscreen Legibility:** Because boards have dark solder-mask colors, pin names on cards must use crisp white text (`#ffffff`, `font-weight: 700`, `text-shadow: 0 1px 3px rgba(0,0,0,0.95)`). Unused pins use `#94a3b8`, warning strapping pins use `#fde047`, and forbidden pins use `#fca5a5`.

---

## 3. SVG Path Fill Safety & Layered Architecture

To prevent browser stylesheet resets or unclosed CSS media queries from painting solid black polygons across the canvas:

1. **Every SVG `<path>` element MUST have `fill="none"`:**
   - Both in JavaScript DOM creation: `casing.setAttribute('fill', 'none')`, `path.setAttribute('fill', 'none')`, `glow.setAttribute('fill', 'none')`.
   - And in CSS: `.wire-casing, .wire-path, .wire-glow { fill: none !important; }`.

2. **Strict Layer Ordering (Eliminates White Cut-Through Artifacts):**
   Wires are rendered in two dedicated SVG group layers:
   - `<g id="wire-casings-layer"></g>` (Appended first: all casing sleeves render underneath).
   - `<g id="wire-paths-layer"></g>` (Appended second: all core colored wire paths render on top).
   This guarantees that casing sleeves never slice or chop through horizontal wires at crossings!

---

## 4. Interactive Tooling & Controls

Makers and vibe coders require interactive controls to inspect circuits:
- **`[🏷️ Tên Chân]` Button (Key `L`):** Toggle monospace pin names next to all solder pads.
- **`[☀️/🌙 Nền Sáng/Tối]` Button (Key `T`):** Instantaneously toggle between Dark CAD and Light Blueprint. Preference is synced via `localStorage` and URL parameters (`?theme=light`).
- **`[🛒 Mua Linh Kiện (BOM)]` Button:** Complete bill of materials with quantities, specs, and Shopee search shortcuts.
- **`[Ẩn/Hiện Dây]` Filter Modal & Subsystem Bar:** Filter wires by power, drive, audio, display, or sensory subsystems.
- **`[Gập Vuông / Dây Cong / Dây Thẳng]` Mode Selector:** Switch between orthogonal jumper hops, smooth organic Bezier curves, or point-to-point lines.
- **Dual Endpoint Badges:** Hovering any wire spawns glowing callouts at both endpoints (`📍 [Component A]: [Pin A]` and `📍 [Component B]: [Pin B]`).

---

## 5. Contextual Progressive Disclosure, Slide-Over Menu Drawer & Mobile Touch Ergonomics

### Minimalist Zen 3-Button Header
To prevent header crowding, title truncation, and cognitive fatigue on compact screens:
- **Header holds ONLY 3 primary actions:**
  1. `[☀️/🌙 Nền Sáng / Tối]` (`#btn-theme-toggle`): Quick theme toggle (shortcut `T`).
  2. `[🛒 Shopee BOM]` (`.bom-btn`): Direct access to parts list and 1-click buy keywords.
  3. `[☰ Menu]` (`#btn-open-menu`): Slide-over drawer toggle for all secondary configuration.

### Slide-Over Menu Drawer (`#menu-drawer`)
Secondary CAD controls are housed in a sliding drawer (`right: 0`, `z-index: 2500`) with a dark backdrop overlay (`#menu-drawer-backdrop`):
1. **Chế Độ Đi Dây CAD:** Orthogonal Manhattan (cầu nhảy bán nguyệt), Curved (Bezier), Straight (P2P).
2. **Hiển Thị & Thao Tác:** Bật/tắt tên chân pin (`L`), Căn vừa màn hình (`F`), Ẩn/hiện khối linh kiện, Khôi phục vị trí mặc định.
3. **Tài Liệu Hướng Dẫn:** Danh mục Shopee BOM, Sổ tay lắp ráp & đo kiểm M1–M8.

### Contextual Progressive Disclosure & Non-Intrusive Wire Isolation ("Ấn thiết bị hiện dây - Giữ hoặc ấn ⓘ mới hiện Chi Tiết")
- **Single Tap / Click on Component:** MUST NOT open a popup or bottom sheet! When a user taps or clicks any component card, it only highlights and isolates connected wires (`toggleIsolateComponent(compId)`), dimming unrelated wires and displaying a compact floating capsule `[🎯 Device Name | ⓘ Chi Tiết | ✕]` (`#active-device-capsule`) at the bottom. This leaves 100% of the canvas visible so users can trace wires unobstructed.
- **Triggering Detailed Bottom Sheet / Drawer:** Detailed specs and pinout tables are revealed ONLY when the user:
  1. Long-presses on the component card for $\ge 450\text{ms}$ (with haptic feedback vibration).
  2. OR taps the explicit `ⓘ` button in the component card header (`.comp-info-btn`).
  3. OR taps `ⓘ Chi Tiết` on the floating active device capsule.
- **Inside the Drawer / Bottom Sheet:**
  - 🎯 `[Cô Lập Dây Thiết Bị / Focus Wires]`: Toggle wire isolation state with `background: var(--surface-elevated); color: var(--fg);`.
  - 🛒 `[Mua Shopee]`: Distinctive vibrant gradient button (`.shopee-btn`: `#ff5722` to `#ea580c`) with 1-click clipboard copy of exact commercial search keyword.
  - **High-Contrast Pin Table:** Pin names MUST use `color: var(--fg);` (`.pin-name-cell`). NEVER hardcode `color: #fff;` on table cells (which becomes invisible white-on-white in Light Blueprint Mode).
- **Zero Duplicate Icons:** In `#subsystem-bar` and `#menu-drawer`, never place an emoji next to an SVG icon or dot indicator (keep clean, professional SVG icons).

### Mobile Touch Ergonomics (Zero Collision Architecture)
- **Zero Screen Clutter on Mobile:** Hide `#quick-jump-bar` on mobile viewports ($\le 768\text{px}$) with `display: none !important;` to reclaim valuable vertical screen real estate. The floating `#active-device-capsule` only appears when a component is actively isolated, providing a non-intrusive action bar (`[🎯 Name | ⓘ Chi Tiết | ✕]`).
- **Eliminate Floating Zoom Widget:** On mobile viewports ($\le 768\text{px}$), `#nav-controls` is hidden (`display: none !important;`). Touchscreen users have intuitive multi-touch:
  - 1-Finger drag to pan.
  - 2-Finger pinch to zoom centered at touch midpoint.
  - Double-tap empty canvas to auto-fit screen (`fitView()` / `fitToScreen()`).
  - Single-tap empty canvas to clear selection, close bottom sheet, un-isolate wires, and hide the capsule.

