# ESP32 IoT & Robotics: Essential Hardware Components Catalog (110+ Modules)

Cẩm nang tra cứu thông số kỹ thuật, sơ đồ chân (pinout), điện áp hoạt động, quy tắc an toàn (DOs & DON'Ts) và các lưu ý đặc thù khi kết nối với vi điều khiển ESP32 / ESP32-S3.

---

## Mục lục danh mục linh kiện

1. [MCU & Development Boards (7 linh kiện)](#mcu--development-boards)
2. [Actuators & Motors (9 linh kiện)](#actuators--motors)
3. [Motor & Servo Drivers (10 linh kiện)](#motor--servo-drivers)
4. [Power & Battery Management (10 linh kiện)](#power--battery-management)
5. [Displays & Visual Indicators (10 linh kiện)](#displays--visual-indicators)
6. [Distance, Optical & Tracking Sensors (11 linh kiện)](#distance-optical--tracking-sensors)
7. [IMU, Orientation & Motion (7 linh kiện)](#imu-orientation--motion)
8. [Environmental & Gas Sensors (10 linh kiện)](#environmental--gas-sensors)
9. [User Input & HMI Controls (8 linh kiện)](#user-input--hmi-controls)
10. [Relays, MOSFETs & Power Switches (8 linh kiện)](#relays-mosfets--power-switches)
11. [Audio, Sound & Buzzer (7 linh kiện)](#audio-sound--buzzer)
12. [Logic Converters, Bus & Protection (8 linh kiện)](#logic-converters-bus--protection)
13. [Wireless, RFID, GPS & Communication (8 linh kiện)](#wireless-rfid-gps--communication)

---

## MCU & Development Boards

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **ESP32-S3 DevKit 40-Pin (N16R8)** | `3.0V - 3.6V (VDD), 5V via USB-C or VBUS pin`<br>Logic: `3.3V strictly` | `GPIO, ADC, I2C, SPI, UART, PWM, I2S, USB OTG` | ⚠️ NEVER use GPIO 26 through 37 on N16R8 boards (causes instant crash due to Flash/PSRAM bus corruption). | `esp32-s3 n16r8 devkit 40 pin type c dual usb` |
| 2 | **ESP32 DevKit V1 30-Pin (ESP-WROOM-32)** | `3.0V - 3.6V (VDD), 5V via Micro-USB/VIN`<br>Logic: `3.3V strictly` | `GPIO, ADC, I2C, SPI, UART, PWM, DAC` | ⚠️ NEVER connect GPIO 6, 7, 8, 9, 10, 11 (connected to flash memory chip). | `esp32 nodemcu devkit v1 30 pin cp2102 ch340` |
| 3 | **ESP32 NodeMCU-32S 38-Pin** | `3.0V - 3.6V, 5V via USB/VIN`<br>Logic: `3.3V strictly` | `GPIO, ADC, I2C, SPI, UART, PWM, DAC` | ⚠️ NEVER assume pin layout matches 30-pin version. | `esp32 38 pin nodemcu 32s` |
| 4 | **ESP32-CAM AI-Thinker (OV2640)** | `5V strictly to 5V pin (current spikes > 350mA during capture)`<br>Logic: `3.3V strictly` | `UART, MicroSD SPI, DVP Camera` | ⚠️ NEVER power solely from cheap 3.3V USB-TTL adapters (causes brownout reset brownout loops). | `esp32 cam ov2640 module kem de nạp` |
| 5 | **ESP32-C3 SuperMini** | `3.3V or 5V via USB-C / VBUS`<br>Logic: `3.3V strictly` | `RISC-V single core, ADC, I2C, SPI, UART, PWM` | ⚠️ NEVER apply >3.3V to GPIO pins. | `esp32-c3 supermini sieu nho type-c` |
| 6 | **Seeed Studio XIAO ESP32-S3** | `3.3V or 5V via USB-C, onboard Li-ion battery charging`<br>Logic: `3.3V strictly` | `GPIO, ADC, I2C, SPI, UART, I2S, Battery solder pads` | ⚠️ NEVER reverse polarity on back battery solder pads (no reverse diode protection). | `seeed xiao esp32-s3 module tiny` |
| 7 | **ESP32-WROVER-IB DevKit (with 8MB PSRAM & IPEX)** | `3.0V - 3.6V, 5V via Micro-USB`<br>Logic: `3.3V strictly` | `GPIO, ADC, I2C, SPI, UART, DAC, External Antenna IPEX` | ⚠️ NEVER connect peripherals to GPIO16 or GPIO17 (PSRAM conflict causes crash). | `esp32 wrover b devkit psram 8mb ipex` |


### 🔹 ESP32-S3 DevKit 40-Pin (N16R8) (`esp32_s3_devkit_40p`)

- **Danh mục:** MCU & Development Boards
- **Điện áp hoạt động:** 3.0V - 3.6V (VDD), 5V via USB-C or VBUS pin
- **Mức logic (Logic Level):** 3.3V strictly
- **Dòng tiêu thụ điển hình:** ~150mA
- **Giao diện / Bus:** GPIO, ADC, I2C, SPI, UART, PWM, I2S, USB OTG
- **Từ khóa tìm mua:** `esp32-s3 n16r8 devkit 40 pin type c dual usb`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `3V3` | `pwr` | 3.3V Output from onboard LDO (max ~500mA) |
| `5V` | `pwr` | 5V In/Out directly connected to USB VBUS |
| `GND` | `gnd` | Common Ground reference |
| `GPIO0` | `sig` | Strapping pin (LOW on boot = download mode). Has pull-up |
| `GPIO1` | `sig` | ADC1_CH0, Touch 1, General purpose I/O |
| `GPIO2` | `sig` | ADC1_CH1, Touch 2, General purpose I/O |
| `GPIO3` | `sig` | Strapping pin (JTAG signal). Avoid pull-down on boot |
| `GPIO4` | `sig` | ADC1_CH3, General I/O, Safe default |
| `GPIO5` | `sig` | ADC1_CH4, General I/O, Safe default |
| `GPIO6` | `sig` | ADC1_CH5, General I/O, Safe default |
| `GPIO7` | `sig` | ADC1_CH6, General I/O, Safe default |
| `GPIO8` | `sig` | Default I2C SDA, General I/O |
| `GPIO9` | `sig` | Default I2C SCL, General I/O |
| `GPIO10` | `sig` | SPI CS/MOSI, General I/O |
| `GPIO19` | `sig` | USB D- (Native USB). Do not use if USB OTG active |
| `GPIO20` | `sig` | USB D+ (Native USB). Do not use if USB OTG active |
| `GPIO26-37` | `na` | RESERVED for internal Octal SPI Flash & PSRAM! DO NOT USE |
| `GPIO43` | `sig` | UART0 TX (Boot log and serial flashing) |
| `GPIO44` | `sig` | UART0 RX (Boot log and serial flashing) |
| `GPIO45` | `sig` | Strapping pin (SPI voltage 1.8V vs 3.3V). NEVER pull HIGH on boot |
| `GPIO46` | `sig` | Strapping pin (ROM messages print / boot log) |


#### ✅ ĐƯỢC LÀM (DOs)
- Supply 5V to 5V pin through a Schottky diode if dual-powering with USB.
- Use ADC1 pins (GPIO 1-10) when WiFi/BLE is enabled to prevent ADC2 conflicts.
- Power heavy loads (servos, motors) from separate external power supply.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER use GPIO 26 through 37 on N16R8 boards (causes instant crash due to Flash/PSRAM bus corruption).**
- 🛑 **NEVER pull GPIO45 HIGH at boot (sets flash voltage to 1.8V and bricks bootloader).**
- 🛑 **NEVER feed > 3.6V into any GPIO pin (ESP32-S3 is strictly NOT 5V tolerant).**


#### ⚡ Lưu ý khi dùng với ESP32
> Native USB CDC requires -DARDUINO_USB_CDC_ON_BOOT=1 in PlatformIO. Enter download mode with BOOT(GPIO0) + RST.

---

### 🔹 ESP32 DevKit V1 30-Pin (ESP-WROOM-32) (`esp32_devkit_v1_30p`)

- **Danh mục:** MCU & Development Boards
- **Điện áp hoạt động:** 3.0V - 3.6V (VDD), 5V via Micro-USB/VIN
- **Mức logic (Logic Level):** 3.3V strictly
- **Dòng tiêu thụ điển hình:** ~160mA
- **Giao diện / Bus:** GPIO, ADC, I2C, SPI, UART, PWM, DAC
- **Từ khóa tìm mua:** `esp32 nodemcu devkit v1 30 pin cp2102 ch340`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VIN` | `pwr` | 5V In/Out from USB |
| `3V3` | `pwr` | 3.3V Output from onboard LDO (max 300mA) |
| `GND` | `gnd` | Ground |
| `GPIO0` | `sig` | Strapping pin (Bootloader switch, internal pullup) |
| `GPIO2` | `sig` | Strapping pin & Onboard Blue LED. Must be floating/low at boot |
| `GPIO12` | `sig` | Strapping pin (MTDI flash voltage). Boot fails if pulled HIGH |
| `GPIO15` | `sig` | Strapping pin (MTDO debug log). Internal pullup |
| `GPIO21` | `sig` | Default I2C SDA |
| `GPIO22` | `sig` | Default I2C SCL |
| `GPIO34-39` | `sig` | GPI: INPUT ONLY, NO internal pullup/pulldown resistors! |
| `GPIO6-11` | `na` | RESERVED for internal SPI Flash. NEVER USE |


#### ✅ ĐƯỢC LÀM (DOs)
- Use external pull-up resistors (4.7k - 10k) when reading buttons on GPIO 34-39.
- Use ADC1 (GPIO 32-39) when using WiFi.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect GPIO 6, 7, 8, 9, 10, 11 (connected to flash memory chip).**
- 🛑 **NEVER configure GPIO 34, 35, 36, 39 as OUTPUT (they are physically input-only).**


#### ⚡ Lưu ý khi dùng với ESP32
> ADC2 channels cannot be read reliably while WiFi is active. Common 30-pin board has CP2102 or CH340.

---

### 🔹 ESP32 NodeMCU-32S 38-Pin (`esp32_devkit_38p`)

- **Danh mục:** MCU & Development Boards
- **Điện áp hoạt động:** 3.0V - 3.6V, 5V via USB/VIN
- **Mức logic (Logic Level):** 3.3V strictly
- **Dòng tiêu thụ điển hình:** ~160mA
- **Giao diện / Bus:** GPIO, ADC, I2C, SPI, UART, PWM, DAC
- **Từ khóa tìm mua:** `esp32 38 pin nodemcu 32s`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VIN` | `pwr` | 5V In/Out |
| `3V3` | `pwr` | 3.3V LDO Output |
| `GND` | `gnd` | Ground (multiple pins) |
| `GPIO34-39` | `sig` | Input Only pins |


#### ✅ ĐƯỢC LÀM (DOs)
- Confirm pinout from manufacturer silk screen before wiring as 38-pin pinouts vary slightly between vendors.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER assume pin layout matches 30-pin version.**


#### ⚡ Lưu ý khi dùng với ESP32
> Broader breadboard footprint: leaves only 1 pin row free on standard breadboard.

---

### 🔹 ESP32-CAM AI-Thinker (OV2640) (`esp32_cam`)

- **Danh mục:** MCU & Development Boards
- **Điện áp hoạt động:** 5V strictly to 5V pin (current spikes > 350mA during capture)
- **Mức logic (Logic Level):** 3.3V strictly
- **Dòng tiêu thụ điển hình:** ~310mA
- **Giao diện / Bus:** UART, MicroSD SPI, DVP Camera
- **Từ khóa tìm mua:** `esp32 cam ov2640 module kem de nạp`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `5V` | `pwr` | Power input (MUST provide >= 2A capability) |
| `3V3` | `pwr` | Output only (weak LDO) |
| `GND` | `gnd` | Ground |
| `U0TXD` | `sig` | GPIO1 UART TX to USB-TTL RX |
| `U0RXD` | `sig` | GPIO3 UART RX to USB-TTL TX |
| `GPIO0` | `sig` | Connect to GND to flash firmware, leave open to run |
| `GPIO4` | `sig` | High power Flash LED (also SD Card Data 1) |
| `GPIO33` | `sig` | Small red indicator LED on back |


#### ✅ ĐƯỢC LÀM (DOs)
- Use external 5V 2A power supply with minimum 100uF capacitor near 5V/GND pins.
- Short GPIO0 to GND before applying power when uploading code.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER power solely from cheap 3.3V USB-TTL adapters (causes brownout reset brownout loops).**
- 🛑 **NEVER use GPIO 12, 13, 14, 15, 2, 4 for other peripherals if MicroSD card is active.**


#### ⚡ Lưu ý khi dùng với ESP32
> No onboard USB chip. Requires external FTDI or CP2102 USB-to-UART adapter for flashing.

---

### 🔹 ESP32-C3 SuperMini (`esp32_c3_supermini`)

- **Danh mục:** MCU & Development Boards
- **Điện áp hoạt động:** 3.3V or 5V via USB-C / VBUS
- **Mức logic (Logic Level):** 3.3V strictly
- **Dòng tiêu thụ điển hình:** ~100mA
- **Giao diện / Bus:** RISC-V single core, ADC, I2C, SPI, UART, PWM
- **Từ khóa tìm mua:** `esp32-c3 supermini sieu nho type-c`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `5V` | `pwr` | 5V In from USB |
| `3V3` | `pwr` | 3.3V LDO Output |
| `GND` | `gnd` | Ground |
| `GPIO8` | `sig` | Onboard blue LED (active LOW) & Strapping pin |
| `GPIO9` | `sig` | Boot button (LOW = download mode) |


#### ✅ ĐƯỢC LÀM (DOs)
- Ultra compact form factor ideal for miniature drones and wearable sensors.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER apply >3.3V to GPIO pins.**


#### ⚡ Lưu ý khi dùng với ESP32
> Single-core RISC-V architecture. Uses Native USB-CDC on GPIO 18/19.

---

### 🔹 Seeed Studio XIAO ESP32-S3 (`seeed_xiao_esp32_s3`)

- **Danh mục:** MCU & Development Boards
- **Điện áp hoạt động:** 3.3V or 5V via USB-C, onboard Li-ion battery charging
- **Mức logic (Logic Level):** 3.3V strictly
- **Dòng tiêu thụ điển hình:** ~110mA
- **Giao diện / Bus:** GPIO, ADC, I2C, SPI, UART, I2S, Battery solder pads
- **Từ khóa tìm mua:** `seeed xiao esp32-s3 module tiny`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `5V` | `pwr` | 5V USB |
| `3V3` | `pwr` | 3.3V output |
| `GND` | `gnd` | Ground |
| `BAT+` | `pwr` | Solder pad on back for 3.7V Li-ion battery |
| `D4 / D5` | `sig` | Default I2C SDA / SCL (GPIO 5 / GPIO 6) |


#### ✅ ĐƯỢC LÀM (DOs)
- Use onboard battery charge management circuit for ultra-low-power IoT wearables.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER reverse polarity on back battery solder pads (no reverse diode protection).**


#### ⚡ Lưu ý khi dùng với ESP32
> Thumbnail-sized board with dual-core LX7 and integrated antenna connector.

---

### 🔹 ESP32-WROVER-IB DevKit (with 8MB PSRAM & IPEX) (`esp32_wrover_ib`)

- **Danh mục:** MCU & Development Boards
- **Điện áp hoạt động:** 3.0V - 3.6V, 5V via Micro-USB
- **Mức logic (Logic Level):** 3.3V strictly
- **Dòng tiêu thụ điển hình:** ~180mA
- **Giao diện / Bus:** GPIO, ADC, I2C, SPI, UART, DAC, External Antenna IPEX
- **Từ khóa tìm mua:** `esp32 wrover b devkit psram 8mb ipex`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `GPIO16` | `na` | USED INTERNALLY for PSRAM CS! DO NOT USE |
| `GPIO17` | `na` | USED INTERNALLY for PSRAM CLK! DO NOT USE |


#### ✅ ĐƯỢC LÀM (DOs)
- Use for RAM-intensive tasks: audio streaming, speech recognition, LVGL buffers.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect peripherals to GPIO16 or GPIO17 (PSRAM conflict causes crash).**


#### ⚡ Lưu ý khi dùng với ESP32
> Requires board_build.partitions and PSRAM enabled in PlatformIO.

---

## Actuators & Motors

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **MG90S 180° Micro Metal Gear Servo** | `4.8V - 6.0V (optimal 5.0V - 5.5V)`<br>Logic: `3.3V - 5.0V PWM signal` | `PWM (50Hz, 1ms - 2ms pulse width)` | ⚠️ NEVER power servo directly from ESP32 3.3V or 5V regulator pin (induces brownout reset). | `servo mg90s 180 do banh rang kim loai` |
| 2 | **MG90S 360° Continuous Rotation Servo** | `4.8V - 6.0V`<br>Logic: `3.3V - 5.0V PWM` | `PWM (50Hz, 1.5ms neutral stop)` | ⚠️ NEVER expect absolute angle feedback (has no internal encoder). | `servo mg90s 360 do quay lien tuc otto` |
| 3 | **SG90 9g Micro Plastic Gear Servo** | `4.8V - 5.5V`<br>Logic: `3.3V - 5V PWM` | `PWM (50Hz)` | ⚠️ NEVER overload (plastic gears strip easily under mechanical shock). | `servo sg90 9g banh rang nhua` |
| 4 | **MG996R High Torque Metal Gear Servo** | `4.8V - 7.2V (optimal 6.0V)`<br>Logic: `3.3V - 5V PWM` | `PWM (50Hz)` | ⚠️ NEVER power via thin breadboard jumper wires (copper resistance causes massive voltage drop). | `servo mg996r luc keo lon banh rang kim loai` |
| 5 | **28BYJ-48 5V Unipolar Stepper Motor** | `5V DC (coil resistance ~50 ohm)`<br>Logic: `5V driven via ULN2003` | `4-phase stepper sequence (Blue, Pink, Yellow, Orange, Red VCC)` | ⚠️ NEVER connect coils directly to ESP32 pins (back-EMF will instantly destroy MCU). | `dong co buoc 28byj-48 5v kem uln2003` |
| 6 | **N20 Micro Metal Gear Motor (6V)** | `3.0V - 6.0V DC`<br>Logic: `Driven via H-bridge PWM` | `2-pin DC motor terminals` | ⚠️ NEVER run directly from microcontroller pin. | `dong co giam toc n20 6v banh rang kim loai` |
| 7 | **TT DC Gearbox Motor (Yellow Dual Shaft 3-6V)** | `3.0V - 6.0V DC`<br>Logic: `Driven via H-bridge PWM` | `2-pin DC motor terminals` | ⚠️ NEVER leave unsuppressed (noisy DC brush noise can reset ESP32 WiFi module). | `dong co giam toc vang tt motor 2 truc kem banh xe` |
| 8 | **Coreless DC Motor 8520 (3.7V)** | `3.2V - 4.2V DC`<br>Logic: `Driven via N-channel MOSFET` | `2 wires` | ⚠️ NEVER run dry without propeller for prolonged duration (overheats coreless windings). | `dong co khong loi coreless 8520 3.7v` |
| 9 | **Mini Solenoid Electromagnetic Door Lock (12V)** | `9V - 12V DC`<br>Logic: `Driven via Relay or Power MOSFET` | `2 wires` | ⚠️ NEVER drive without flyback diode (inductive voltage spike can reach >100V). | `khoa dien tu mini solenoid lock 12v` |


### 🔹 MG90S 180° Micro Metal Gear Servo (`mg90s_180`)

- **Danh mục:** Actuators & Motors
- **Điện áp hoạt động:** 4.8V - 6.0V (optimal 5.0V - 5.5V)
- **Mức logic (Logic Level):** 3.3V - 5.0V PWM signal
- **Dòng tiêu thụ điển hình:** ~200mA
- **Giao diện / Bus:** PWM (50Hz, 1ms - 2ms pulse width)
- **Từ khóa tìm mua:** `servo mg90s 180 do banh rang kim loai`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | Red wire: 4.8V - 6V from dedicated power supply |
| `GND` | `gnd` | Brown wire: Common ground |
| `SIGNAL` | `sig` | Orange/Yellow wire: PWM signal (50Hz) |


#### ✅ ĐƯỢC LÀM (DOs)
- Use external 5V regulator/battery capable of >= 1A stall current per servo.
- Always attach a 470uF - 1000uF decoupling capacitor across servo power rails.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER power servo directly from ESP32 3.3V or 5V regulator pin (induces brownout reset).**
- 🛑 **NEVER force the servo horn by hand beyond 0° - 180° mechanical stops.**


#### ⚡ Lưu ý khi dùng với ESP32
> Accepts 3.3V logic PWM directly from ESP32 GPIO or via PCA9685.

---

### 🔹 MG90S 360° Continuous Rotation Servo (`mg90s_360`)

- **Danh mục:** Actuators & Motors
- **Điện áp hoạt động:** 4.8V - 6.0V
- **Mức logic (Logic Level):** 3.3V - 5.0V PWM
- **Dòng tiêu thụ điển hình:** ~220mA
- **Giao diện / Bus:** PWM (50Hz, 1.5ms neutral stop)
- **Từ khóa tìm mua:** `servo mg90s 360 do quay lien tuc otto`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | Red wire: 5V motor power |
| `GND` | `gnd` | Brown wire: Common ground |
| `SIGNAL` | `sig` | Orange wire: PWM signal |


#### ✅ ĐƯỢC LÀM (DOs)
- Calibrate exact neutral stop pulse width (usually ~1500us ± 40us) in software.
- Use deadband threshold of ±15us to ensure complete motor standstill.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER expect absolute angle feedback (has no internal encoder).**
- 🛑 **NEVER drive directly without external current reserve.**


#### ⚡ Lưu ý khi dùng với ESP32
> Ideal for wheeled micro-robots like Otto DIY. Stop = ~1500us, Forward = >1500us, Reverse = <1500us.

---

### 🔹 SG90 9g Micro Plastic Gear Servo (`sg90_180`)

- **Danh mục:** Actuators & Motors
- **Điện áp hoạt động:** 4.8V - 5.5V
- **Mức logic (Logic Level):** 3.3V - 5V PWM
- **Dòng tiêu thụ điển hình:** ~150mA
- **Giao diện / Bus:** PWM (50Hz)
- **Từ khóa tìm mua:** `servo sg90 9g banh rang nhua`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | Red wire: 5V power |
| `GND` | `gnd` | Brown wire: Ground |
| `SIGNAL` | `sig` | Orange wire: PWM |


#### ✅ ĐƯỢC LÀM (DOs)
- Use for lightweight robotic arms, ultrasonic panning brackets, pan-tilt kits.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER overload (plastic gears strip easily under mechanical shock).**


#### ⚡ Lưu ý khi dùng với ESP32
> Low cost starter servo. Stall current ~650mA.

---

### 🔹 MG996R High Torque Metal Gear Servo (`mg996r`)

- **Danh mục:** Actuators & Motors
- **Điện áp hoạt động:** 4.8V - 7.2V (optimal 6.0V)
- **Mức logic (Logic Level):** 3.3V - 5V PWM
- **Dòng tiêu thụ điển hình:** ~500mA
- **Giao diện / Bus:** PWM (50Hz)
- **Từ khóa tìm mua:** `servo mg996r luc keo lon banh rang kim loai`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | Red wire: 6V high current supply |
| `GND` | `gnd` | Brown wire: Ground |
| `SIGNAL` | `sig` | Orange wire: PWM |


#### ✅ ĐƯỢC LÀM (DOs)
- Provide minimum 2.5A peak current capacity per servo (stall current is 2.5A at 6V).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER power via thin breadboard jumper wires (copper resistance causes massive voltage drop).**


#### ⚡ Lưu ý khi dùng với ESP32
> Use PCA9685 driver with independent terminal block power and thick AWG20 wire.

---

### 🔹 28BYJ-48 5V Unipolar Stepper Motor (`stepper_28byj_48`)

- **Danh mục:** Actuators & Motors
- **Điện áp hoạt động:** 5V DC (coil resistance ~50 ohm)
- **Mức logic (Logic Level):** 5V driven via ULN2003
- **Dòng tiêu thụ điển hình:** ~240mA
- **Giao diện / Bus:** 4-phase stepper sequence (Blue, Pink, Yellow, Orange, Red VCC)
- **Từ khóa tìm mua:** `dong co buoc 28byj-48 5v kem uln2003`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | Red: +5V Common |
| `Coil 1-4` | `sig` | Blue, Pink, Yellow, Orange coils |


#### ✅ ĐƯỢC LÀM (DOs)
- Drive using ULN2003 Darlington driver board or 4x NPN transistors.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect coils directly to ESP32 pins (back-EMF will instantly destroy MCU).**


#### ⚡ Lưu ý khi dùng với ESP32
> Gear ratio ~64:1 (4096 steps per full output shaft revolution in half-step mode).

---

### 🔹 N20 Micro Metal Gear Motor (6V) (`n20_gear_motor`)

- **Danh mục:** Actuators & Motors
- **Điện áp hoạt động:** 3.0V - 6.0V DC
- **Mức logic (Logic Level):** Driven via H-bridge PWM
- **Dòng tiêu thụ điển hình:** ~120mA
- **Giao diện / Bus:** 2-pin DC motor terminals
- **Từ khóa tìm mua:** `dong co giam toc n20 6v banh rang kim loai`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `M+` | `pwr` | Positive terminal |
| `M-` | `pwr` | Negative terminal |


#### ✅ ĐƯỢC LÀM (DOs)
- Use DRV8833 or TB6612FNG motor driver.
- Solder a 100nF ceramic capacitor across the motor terminals to suppress brush spark EMI.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER run directly from microcontroller pin.**


#### ⚡ Lưu ý khi dùng với ESP32
> Stall current ~600mA at 6V. Perfect for mini line-following robots and micro sumos.

---

### 🔹 TT DC Gearbox Motor (Yellow Dual Shaft 3-6V) (`tt_motor_yellow`)

- **Danh mục:** Actuators & Motors
- **Điện áp hoạt động:** 3.0V - 6.0V DC
- **Mức logic (Logic Level):** Driven via H-bridge PWM
- **Dòng tiêu thụ điển hình:** ~250mA
- **Giao diện / Bus:** 2-pin DC motor terminals
- **Từ khóa tìm mua:** `dong co giam toc vang tt motor 2 truc kem banh xe`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `M1` | `pwr` | Motor pin 1 |
| `M2` | `pwr` | Motor pin 2 |


#### ✅ ĐƯỢC LÀM (DOs)
- Pair with L298N, L9110S, or TB6612 driver.
- Add optical encoder slotted wheel on rear shaft.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER leave unsuppressed (noisy DC brush noise can reset ESP32 WiFi module).**


#### ⚡ Lưu ý khi dùng với ESP32
> Ubiquitous yellow motor used in 2WD/4WD DIY smart car chassis.

---

### 🔹 Coreless DC Motor 8520 (3.7V) (`coreless_motor_8520`)

- **Danh mục:** Actuators & Motors
- **Điện áp hoạt động:** 3.2V - 4.2V DC
- **Mức logic (Logic Level):** Driven via N-channel MOSFET
- **Dòng tiêu thụ điển hình:** ~800mA
- **Giao diện / Bus:** 2 wires
- **Từ khóa tìm mua:** `dong co khong loi coreless 8520 3.7v`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `V+` | `pwr` | Positive power |
| `V-` | `pwr` | MOSFET drain |


#### ✅ ĐƯỢC LÀM (DOs)
- Always install a flyback diode (1N5819) across motor terminals when switching via MOSFET.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER run dry without propeller for prolonged duration (overheats coreless windings).**


#### ⚡ Lưu ý khi dùng với ESP32
> High RPM (>35,000 RPM). Commonly used in micro quadcopters and hovercraft.

---

### 🔹 Mini Solenoid Electromagnetic Door Lock (12V) (`solenoid_lock_12v`)

- **Danh mục:** Actuators & Motors
- **Điện áp hoạt động:** 9V - 12V DC
- **Mức logic (Logic Level):** Driven via Relay or Power MOSFET
- **Dòng tiêu thụ điển hình:** ~600mA
- **Giao diện / Bus:** 2 wires
- **Từ khóa tìm mua:** `khoa dien tu mini solenoid lock 12v`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `V+` | `pwr` | +12V Supply |
| `GND` | `gnd` | Switched Ground |


#### ✅ ĐƯỢC LÀM (DOs)
- Limit activation duration to < 10 seconds to avoid overheating the solenoid coil.
- Add 1N4007 flyback diode.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER drive without flyback diode (inductive voltage spike can reach >100V).**


#### ⚡ Lưu ý khi dùng với ESP32
> Drive gate of logic-level N-MOSFET (e.g. LR7843) from ESP32 GPIO with 10k pulldown.

---

## Motor & Servo Drivers

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **PCA9685 16-Channel 12-bit PWM I2C Driver** | `VCC: 3.0V - 5.5V (Logic), V+: 4.8V - 6.0V (Servo Power)`<br>Logic: `3.3V / 5.0V I2C compliant` | `I2C (Fast Mode 400kHz)`<br>I2C: `0x40 (configurable up to 0x7F via A0-A5 solder pads)` | ⚠️ NEVER connect external 5V/6V battery power to logic VCC pin (will blow ESP32 I2C bus). | `mach dieu khien servo pca9685 16 kenh i2c` |
| 2 | **L298N Dual H-Bridge Motor Driver Module** | `Power: 5V - 35V, Logic: 5V`<br>Logic: `5V logic (accepts 3.3V signals)` | `ENA, IN1, IN2, IN3, IN4, ENB (PWM + Direction)` | ⚠️ NEVER connect L298N onboard 5V output directly to ESP32 3.3V pin. | `mach cau h l298n dieu khien dong co dc` |
| 3 | **TB6612FNG Dual DC Motor Driver** | `VM: 4.5V - 13.5V (Motor), VCC: 2.7V - 5.5V (Logic)`<br>Logic: `3.3V / 5V compatible` | `PWMA, AIN1, AIN2, STBY, BIN1, BIN2, PWMB` | ⚠️ NEVER leave STBY floating (defaults to shutdown mode). | `mach tb6612fng dieu khien dong co mosfet` |
| 4 | **DRV8833 Dual H-Bridge Motor Driver Module** | `2.7V - 10.8V DC`<br>Logic: `3.3V / 5.0V logic` | `IN1, IN2, IN3, IN4, EEP (Sleep)` | ⚠️ NEVER exceed 10.8V VM voltage limit. | `mach cau h drv8833 2 dong co dc mini` |
| 5 | **L9110S Dual Channel DC Motor Driver** | `2.5V - 12V DC`<br>Logic: `3.3V - 5V compatible` | `A-IA, A-IB, B-IA, B-IB` | ⚠️ NEVER pull both IA and IB HIGH at the same time (causes motor brake). | `mach dieu khien dong co l9110s mini` |
| 6 | **ULN2003 Darlington Transistor Array Stepper Driver** | `5V - 12V DC`<br>Logic: `3.3V / 5V input trigger` | `IN1, IN2, IN3, IN4, 5-pin JST stepper connector` | ⚠️ NEVER power 5V stepper motor from ESP32 3.3V pin. | `mach uln2003 dieu khien dong co buoc 28byj` |
| 7 | **A4988 Microstepping Stepper Motor Driver** | `VMOT: 8V - 35V, VDD: 3.0V - 5.5V`<br>Logic: `3.3V / 5V compatible` | `STEP, DIR, ENABLE, MS1, MS2, MS3` | ⚠️ NEVER connect or disconnect motor wires while VMOT is powered (burns IC instantly). | `mach a4988 driver dong co buoc kem tan nhiet` |
| 8 | **TMC2209 Ultra-Silent Stepper Motor Driver** | `VM: 4.75V - 29V, VIO: 3.3V - 5V`<br>Logic: `3.3V / 5V compliant` | `STEP, DIR, UART single-wire, StealthChop2, StallGuard4` | ⚠️ NEVER run at high currents without heat sink and active cooling airflow. | `driver tmc2209 sieu em stepper motor` |
| 9 | **BTS7960 43A High Power H-Bridge Driver** | `6V - 27V DC`<br>Logic: `3.3V - 5V logic input` | `RPWM, LPWM, R_EN, L_EN, R_IS, L_IS` | ⚠️ NEVER reverse polarity on battery input terminals (causes explosive MOSFET destruction). | `mach cau h cong suat lon bts7960 43a` |
| 10 | **MX1508 Mini Dual DC Motor Driver** | `2.0V - 10.0V DC`<br>Logic: `1.8V - 7.0V compliant` | `IN1, IN2, IN3, IN4` | ⚠️ NEVER short motor outputs to GND (no short-circuit protection). | `mach dieu khien dong co mini mx1508` |


### 🔹 PCA9685 16-Channel 12-bit PWM I2C Driver (`pca9685`)

- **Danh mục:** Motor & Servo Drivers
- **Điện áp hoạt động:** VCC: 3.0V - 5.5V (Logic), V+: 4.8V - 6.0V (Servo Power)
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C compliant
- **Dòng tiêu thụ điển hình:** ~15mA
- **Giao diện / Bus:** I2C (Fast Mode 400kHz)
- **Địa chỉ mặc định:** `0x40 (configurable up to 0x7F via A0-A5 solder pads)`
- **Từ khóa tìm mua:** `mach dieu khien servo pca9685 16 kenh i2c`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `GND` | `gnd` | Logic Ground |
| `OE` | `sig` | Output Enable (active LOW, internal pull-down) |
| `SCL` | `sig` | I2C Clock line (connect to ESP32 SCL) |
| `SDA` | `sig` | I2C Data line (connect to ESP32 SDA) |
| `VCC` | `pwr` | Logic Power (connect to ESP32 3.3V) |
| `V+` | `pwr` | Terminal block screw power for servos (4.8V - 6V) |


#### ✅ ĐƯỢC LÀM (DOs)
- Power logic VCC from ESP32 3.3V, and servo V+ from external battery/boost.
- Install a 470uF - 1000uF 10V electrolytic capacitor on the dedicated C1 capacitor pads.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect external 5V/6V battery power to logic VCC pin (will blow ESP32 I2C bus).**
- 🛑 **NEVER leave ground between ESP32 and PCA9685 disconnected.**


#### ⚡ Lưu ý khi dùng với ESP32
> Hardware PWM offloader. Free 16 channels with only 2 I2C pins. Adafruit_PWMServoDriver library standard.

---

### 🔹 L298N Dual H-Bridge Motor Driver Module (`l298n_driver`)

- **Danh mục:** Motor & Servo Drivers
- **Điện áp hoạt động:** Power: 5V - 35V, Logic: 5V
- **Mức logic (Logic Level):** 5V logic (accepts 3.3V signals)
- **Dòng tiêu thụ điển hình:** ~35mA
- **Giao diện / Bus:** ENA, IN1, IN2, IN3, IN4, ENB (PWM + Direction)
- **Từ khóa tìm mua:** `mach cau h l298n dieu khien dong co dc`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `12V` | `pwr` | Motor power input (7V - 35V) |
| `GND` | `gnd` | Common Ground |
| `5V` | `pwr` | 5V output (if 5V enable jumper fitted) or input |
| `IN1-IN4` | `sig` | Direction control logic |
| `ENA/ENB` | `sig` | Speed control PWM (remove jumper to control) |


#### ✅ ĐƯỢC LÀM (DOs)
- Remove the 5V regulator jumper if motor supply exceeds 12V.
- Ensure common GND between L298N and ESP32.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect L298N onboard 5V output directly to ESP32 3.3V pin.**
- 🛑 **Expect significant ~2V voltage drop across internal bipolar transistors.**


#### ⚡ Lưu ý khi dùng với ESP32
> Bipolar technology has low efficiency. TB6612FNG or DRV8833 is preferred for low-voltage battery projects.

---

### 🔹 TB6612FNG Dual DC Motor Driver (`tb6612fng_driver`)

- **Danh mục:** Motor & Servo Drivers
- **Điện áp hoạt động:** VM: 4.5V - 13.5V (Motor), VCC: 2.7V - 5.5V (Logic)
- **Mức logic (Logic Level):** 3.3V / 5V compatible
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** PWMA, AIN1, AIN2, STBY, BIN1, BIN2, PWMB
- **Từ khóa tìm mua:** `mach tb6612fng dieu khien dong co mosfet`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VM` | `pwr` | Motor power supply (4.5V - 13.5V) |
| `VCC` | `pwr` | Logic power supply (3.3V from ESP32) |
| `GND` | `gnd` | Common ground |
| `STBY` | `sig` | Standby pin (MUST be pulled HIGH to enable driver) |
| `AIN1/AIN2` | `sig` | Motor A direction |
| `PWMA` | `sig` | Motor A speed PWM |


#### ✅ ĐƯỢC LÀM (DOs)
- Tie STBY pin to ESP32 3.3V or a control GPIO to wake up the driver.
- Efficient MOSFET bridge with low Rdson.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER leave STBY floating (defaults to shutdown mode).**


#### ⚡ Lưu ý khi dùng với ESP32
> Much higher efficiency than L298N. Virtually zero heat at 1.2A continuous current.

---

### 🔹 DRV8833 Dual H-Bridge Motor Driver Module (`drv8833_driver`)

- **Danh mục:** Motor & Servo Drivers
- **Điện áp hoạt động:** 2.7V - 10.8V DC
- **Mức logic (Logic Level):** 3.3V / 5.0V logic
- **Dòng tiêu thụ điển hình:** ~2mA
- **Giao diện / Bus:** IN1, IN2, IN3, IN4, EEP (Sleep)
- **Từ khóa tìm mua:** `mach cau h drv8833 2 dong co dc mini`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VM` | `pwr` | Motor Power (2.7V - 10.8V, perfect for 1S/2S Li-ion) |
| `GND` | `gnd` | Ground |
| `IN1/IN2` | `sig` | Motor A PWM / Direction |
| `ULT` | `sig` | Fault output pin (open drain) |


#### ✅ ĐƯỢC LÀM (DOs)
- Ideal for battery-operated robots running on a single 3.7V 18650 Li-ion cell.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER exceed 10.8V VM voltage limit.**


#### ⚡ Lưu ý khi dùng với ESP32
> Ultra compact, built-in current limit sensing and thermal shutdown.

---

### 🔹 L9110S Dual Channel DC Motor Driver (`l9110s_driver`)

- **Danh mục:** Motor & Servo Drivers
- **Điện áp hoạt động:** 2.5V - 12V DC
- **Mức logic (Logic Level):** 3.3V - 5V compatible
- **Dòng tiêu thụ điển hình:** ~5mA
- **Giao diện / Bus:** A-IA, A-IB, B-IA, B-IB
- **Từ khóa tìm mua:** `mach dieu khien dong co l9110s mini`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 2.5V - 12V Power |
| `GND` | `gnd` | Ground |
| `A-IA / A-IB` | `sig` | Motor A PWM forward / reverse inputs |


#### ✅ ĐƯỢC LÀM (DOs)
- Simplest 4-wire control: drive A-IA with PWM and A-IB LOW for forward rotation.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER pull both IA and IB HIGH at the same time (causes motor brake).**


#### ⚡ Lưu ý khi dùng với ESP32
> Rated 800mA continuous per channel. Great for low-cost mini rovers.

---

### 🔹 ULN2003 Darlington Transistor Array Stepper Driver (`uln2003_driver`)

- **Danh mục:** Motor & Servo Drivers
- **Điện áp hoạt động:** 5V - 12V DC
- **Mức logic (Logic Level):** 3.3V / 5V input trigger
- **Dòng tiêu thụ điển hình:** ~10mA
- **Giao diện / Bus:** IN1, IN2, IN3, IN4, 5-pin JST stepper connector
- **Từ khóa tìm mua:** `mach uln2003 dieu khien dong co buoc 28byj`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | +5V to +12V power supply |
| `GND` | `gnd` | Common ground |
| `IN1-IN4` | `sig` | Inputs from ESP32 GPIOs |


#### ✅ ĐƯỢC LÀM (DOs)
- Has onboard indicator LEDs showing active coil step sequence.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER power 5V stepper motor from ESP32 3.3V pin.**


#### ⚡ Lưu ý khi dùng với ESP32
> Built-in clamping flyback diodes protect inputs against inductive spikes.

---

### 🔹 A4988 Microstepping Stepper Motor Driver (`a4988_driver`)

- **Danh mục:** Motor & Servo Drivers
- **Điện áp hoạt động:** VMOT: 8V - 35V, VDD: 3.0V - 5.5V
- **Mức logic (Logic Level):** 3.3V / 5V compatible
- **Dòng tiêu thụ điển hình:** ~8mA
- **Giao diện / Bus:** STEP, DIR, ENABLE, MS1, MS2, MS3
- **Từ khóa tìm mua:** `mach a4988 driver dong co buoc kem tan nhiet`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VMOT` | `pwr` | Motor power (8V - 35V with 100uF cap!) |
| `VDD` | `pwr` | Logic power (3.3V from ESP32) |
| `STEP` | `sig` | Pulse to advance step |
| `DIR` | `sig` | Direction HIGH/LOW |


#### ✅ ĐƯỢC LÀM (DOs)
- MUST calibrate current limit via onboard Vref trimpot BEFORE attaching motor.
- Install aluminum heat sink on top of driver IC.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect or disconnect motor wires while VMOT is powered (burns IC instantly).**


#### ⚡ Lưu ý khi dùng với ESP32
> Supports up to 1/16 microstepping. Standard driver for 3D printers and CNCs.

---

### 🔹 TMC2209 Ultra-Silent Stepper Motor Driver (`tmc2209_driver`)

- **Danh mục:** Motor & Servo Drivers
- **Điện áp hoạt động:** VM: 4.75V - 29V, VIO: 3.3V - 5V
- **Mức logic (Logic Level):** 3.3V / 5V compliant
- **Dòng tiêu thụ điển hình:** ~15mA
- **Giao diện / Bus:** STEP, DIR, UART single-wire, StealthChop2, StallGuard4
- **Từ khóa tìm mua:** `driver tmc2209 sieu em stepper motor`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VM` | `pwr` | Motor power |
| `VIO` | `pwr` | Logic 3.3V |
| `STEP/DIR` | `sig` | Step & Direction |
| `DIAG` | `sig` | Sensorless homing output interrupt |


#### ✅ ĐƯỢC LÀM (DOs)
- Use StealthChop2 for whisper-quiet robotic joints and 3D printing.
- Enable StallGuard for sensorless endstops.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER run at high currents without heat sink and active cooling airflow.**


#### ⚡ Lưu ý khi dùng với ESP32
> Can be configured dynamically over single-wire UART from ESP32.

---

### 🔹 BTS7960 43A High Power H-Bridge Driver (`bts7960_driver`)

- **Danh mục:** Motor & Servo Drivers
- **Điện áp hoạt động:** 6V - 27V DC
- **Mức logic (Logic Level):** 3.3V - 5V logic input
- **Dòng tiêu thụ điển hình:** ~20mA
- **Giao diện / Bus:** RPWM, LPWM, R_EN, L_EN, R_IS, L_IS
- **Từ khóa tìm mua:** `mach cau h cong suat lon bts7960 43a`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `B+/B-` | `pwr` | High current DC battery power |
| `M+/M-` | `pwr` | Motor screw terminals |
| `VCC` | `pwr` | 5V logic supply (3.3V or 5V) |
| `RPWM/LPWM` | `sig` | Forward / Reverse PWM signals |


#### ✅ ĐƯỢC LÀM (DOs)
- Use for heavy robots, electric wheelchairs, lawnmowers requiring up to 43A peak.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER reverse polarity on battery input terminals (causes explosive MOSFET destruction).**


#### ⚡ Lưu ý khi dùng với ESP32
> Huge aluminum heatsink integrated. Has current alarm feedback pins.

---

### 🔹 MX1508 Mini Dual DC Motor Driver (`mx1508_driver`)

- **Danh mục:** Motor & Servo Drivers
- **Điện áp hoạt động:** 2.0V - 10.0V DC
- **Mức logic (Logic Level):** 1.8V - 7.0V compliant
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** IN1, IN2, IN3, IN4
- **Từ khóa tìm mua:** `mach dieu khien dong co mini mx1508`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `MOTOR-A / B` | `pwr` | Dual motor outputs |
| `Power +/-` | `pwr` | 2V - 10V battery power |
| `IN1-IN4` | `sig` | PWM inputs |


#### ✅ ĐƯỢC LÀM (DOs)
- Ultra compact size (25x21mm), peak 2.5A, low standby current.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER short motor outputs to GND (no short-circuit protection).**


#### ⚡ Lưu ý khi dùng với ESP32
> Cheapest and smallest dual H-bridge module for budget toy hacking.

---

## Power & Battery Management

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **TP4056 1S Li-ion Battery Charger with Protection (Type-C / Micro-USB)** | `Input: 4.5V - 5.5V USB, Output: 4.2V CC/CV`<br>Logic: `Power module` | `IN+, IN-, B+, B-, OUT+, OUT-` | ⚠️ NEVER connect loads directly across B+/B- (bypasses under-voltage and short-circuit protection). | `mach sac pin 1s tp4056 co bao ve type-c` |
| 2 | **MT3608 2A DC-DC Step-Up Boost Converter** | `VIN: 2.0V - 24V, VOUT: 5.0V - 28V (Adjustable)`<br>Logic: `Power module` | `VIN+, VIN-, VOUT+, VOUT-` | ⚠️ NEVER connect loads before measuring output voltage (factory setting can be >20V!). | `mach tang ap mini mt3608 2a dc-dc` |
| 3 | **LM2596 DC-DC Step-Down Buck Converter (3A)** | `VIN: 4.0V - 40V, VOUT: 1.25V - 35V`<br>Logic: `Power module` | `IN+, IN-, OUT+, OUT-` | ⚠️ NEVER use if VIN is less than VOUT + 1.5V (it is step-down only). | `mach ha ap lm2596 3a dc-dc co bien tro` |
| 4 | **MP1584EN Ultra-Small DC-DC Step-Down Buck (3A)** | `VIN: 4.5V - 28V, VOUT: 0.8V - 20V`<br>Logic: `Power module` | `IN+, IN-, OUT+, OUT-` | ⚠️ Potentiometer is tiny and delicate; turn gently with miniature ceramic screwdriver. | `mach ha ap mini mp1584en 3a` |
| 5 | **XL6009 DC-DC Step-Up Boost Converter (4A)** | `VIN: 3.0V - 32V, VOUT: 5.0V - 35V`<br>Logic: `Power module` | `IN+, IN-, OUT+, OUT-` | ⚠️ NEVER short-circuit output (XL6009 has no output short circuit protection). | `mach tang ap xl6009 4a dc-dc` |
| 6 | **18650 Battery Shield V3 (with 5V / 3V outputs)** | `3.7V 18650 battery to 5V (USB-A) and 3.3V pin headers`<br>Logic: `Power module` | `USB-A output, 5V/GND headers, 3V/GND headers, MicroUSB/Type-C charging` | ⚠️ NEVER insert 18650 in reverse (destroys charging controller IC immediately). | `de pin 18650 shield v3 kem mach sac va tang ap` |
| 7 | **2S 7.4V / 8.4V Li-ion BMS Protection Board (20A)** | `7.4V nominal (8.4V fully charged)`<br>Logic: `Battery protection` | `B+, B-, BM (Middle tap), P+, P-` | ⚠️ NEVER mix old and new battery cells of different capacities. | `mach bao ve pin 2s 8.4v 20a bms` |
| 8 | **1N5819 / SS14 / SS34 Schottky Barrier Diode (Power OR-ing)** | `Up to 40V Reverse Voltage`<br>Logic: `Passive Semiconductor` | `Anode, Cathode (Silver band)` | ⚠️ NEVER use standard 1N4007 silicon diode (0.7V - 1.0V drop causes ESP32 brownouts). | `diode schottky 1n5819 ss14 ss34 chong nguoc nguon` |
| 9 | **AMS1117-3.3V Step-Down LDO Voltage Regulator Module** | `VIN: 4.5V - 12V, VOUT: 3.3V (800mA max)`<br>Logic: `Power regulation` | `VIN, GND, VOUT` | ⚠️ NEVER input >12V (linear regulator dissipates excess voltage as heat and overheats). | `mach nguon ha ap ams1117 3.3v mini` |
| 10 | **AP2112K-3.3 Ultra-Low Dropout 3.3V LDO (600mA)** | `VIN: 3.5V - 6.0V, VOUT: 3.3V (Dropout only 250mV at 600mA)`<br>Logic: `Power regulation` | `VIN, GND, EN, VOUT` | ⚠️ NEVER exceed 6V max input rating. | `ap2112k-3.3 sot23-5 ldo low dropout` |


### 🔹 TP4056 1S Li-ion Battery Charger with Protection (Type-C / Micro-USB) (`tp4056_protection`)

- **Danh mục:** Power & Battery Management
- **Điện áp hoạt động:** Input: 4.5V - 5.5V USB, Output: 4.2V CC/CV
- **Mức logic (Logic Level):** Power module
- **Dòng tiêu thụ điển hình:** ~1000mA
- **Giao diện / Bus:** IN+, IN-, B+, B-, OUT+, OUT-
- **Từ khóa tìm mua:** `mach sac pin 1s tp4056 co bao ve type-c`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `IN+ / IN-` | `pwr` | 5V DC input from USB or external adapter |
| `B+` | `pwr` | Connect directly to Li-ion 3.7V Cell Positive terminal |
| `B-` | `pwr` | Connect directly to Li-ion 3.7V Cell Negative terminal |
| `OUT+` | `pwr` | Protected Power Output Positive (to power switch/boost) |
| `OUT-` | `pwr` | Protected Power Output Negative (to common GND) |


#### ✅ ĐƯỢC LÀM (DOs)
- Always choose the module with 2 ICs (TP4056 + DW01A + FS8205A) with OUT+/OUT- terminals.
- Wire battery directly to B+/B- and loads to OUT+/OUT-.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect loads directly across B+/B- (bypasses under-voltage and short-circuit protection).**
- 🛑 **NEVER reverse battery polarity (instantly burns DW01 protection IC).**


#### ⚡ Lưu ý khi dùng với ESP32
> Provides 2.5V cut-off protection preventing Li-ion over-discharge. Standard charge current 1A.

---

### 🔹 MT3608 2A DC-DC Step-Up Boost Converter (`mt3608_boost`)

- **Danh mục:** Power & Battery Management
- **Điện áp hoạt động:** VIN: 2.0V - 24V, VOUT: 5.0V - 28V (Adjustable)
- **Mức logic (Logic Level):** Power module
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** VIN+, VIN-, VOUT+, VOUT-
- **Từ khóa tìm mua:** `mach tang ap mini mt3608 2a dc-dc`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VIN+` | `pwr` | Input positive from battery OUT+ |
| `VIN-` | `pwr` | Input negative GND |
| `VOUT+` | `pwr` | Boosted output positive (adjusted to 5.1V) |
| `VOUT-` | `gnd` | Output negative GND (internally common with VIN-) |


#### ✅ ĐƯỢC LÀM (DOs)
- Turn multi-turn potentiometer counter-clockwise 10-20 turns if output voltage does not rise initially.
- Use multimeter to verify and lock output to exactly 5.1V BEFORE connecting any servos or ESP32.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect loads before measuring output voltage (factory setting can be >20V!).**
- 🛑 **NEVER exceed 2A peak current (add heatsink if continuous current > 1.2A).**


#### ⚡ Lưu ý khi dùng với ESP32
> Ideal to step up 3.7V Li-ion to 5.1V to power ESP32 and SG90/MG90S servos.

---

### 🔹 LM2596 DC-DC Step-Down Buck Converter (3A) (`lm2596_buck`)

- **Danh mục:** Power & Battery Management
- **Điện áp hoạt động:** VIN: 4.0V - 40V, VOUT: 1.25V - 35V
- **Mức logic (Logic Level):** Power module
- **Dòng tiêu thụ điển hình:** ~5mA
- **Giao diện / Bus:** IN+, IN-, OUT+, OUT-
- **Từ khóa tìm mua:** `mach ha ap lm2596 3a dc-dc co bien tro`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `IN+` | `pwr` | DC Input positive (e.g. 12V adapter, 3S LiPo) |
| `IN-` | `gnd` | DC Input negative |
| `OUT+` | `pwr` | Regulated output (e.g. set to 5.0V) |
| `OUT-` | `gnd` | Regulated output ground |


#### ✅ ĐƯỢC LÀM (DOs)
- Check output with voltmeter before connecting electronics.
- Use for 12V/24V robotic power supplies.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER use if VIN is less than VOUT + 1.5V (it is step-down only).**


#### ⚡ Lưu ý khi dùng với ESP32
> Rock-solid 3A output. Can easily power ESP32, cameras, and multiple servos from a 3S 11.1V LiPo.

---

### 🔹 MP1584EN Ultra-Small DC-DC Step-Down Buck (3A) (`mp1584en_buck`)

- **Danh mục:** Power & Battery Management
- **Điện áp hoạt động:** VIN: 4.5V - 28V, VOUT: 0.8V - 20V
- **Mức logic (Logic Level):** Power module
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** IN+, IN-, OUT+, OUT-
- **Từ khóa tìm mua:** `mach ha ap mini mp1584en 3a`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `IN+ / IN-` | `pwr` | Input terminals |
| `OUT+ / OUT-` | `pwr` | Output terminals |


#### ✅ ĐƯỢC LÀM (DOs)
- Super miniature size (22x17mm). High 1.5MHz switching frequency.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Potentiometer is tiny and delicate; turn gently with miniature ceramic screwdriver.**


#### ⚡ Lưu ý khi dùng với ESP32
> Great space-saving replacement for LM2596 in compact combat bots and drones.

---

### 🔹 XL6009 DC-DC Step-Up Boost Converter (4A) (`xl6009_boost_buck`)

- **Danh mục:** Power & Battery Management
- **Điện áp hoạt động:** VIN: 3.0V - 32V, VOUT: 5.0V - 35V
- **Mức logic (Logic Level):** Power module
- **Dòng tiêu thụ điển hình:** ~10mA
- **Giao diện / Bus:** IN+, IN-, OUT+, OUT-
- **Từ khóa tìm mua:** `mach tang ap xl6009 4a dc-dc`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `IN+ / IN-` | `pwr` | Power in |
| `OUT+ / OUT-` | `pwr` | Power out |


#### ✅ ĐƯỢC LÀM (DOs)
- Capable of higher 4A switch current for heavier robotic drive systems.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER short-circuit output (XL6009 has no output short circuit protection).**


#### ⚡ Lưu ý khi dùng với ESP32
> Upgraded replacement for older LM2577 boost modules.

---

### 🔹 18650 Battery Shield V3 (with 5V / 3V outputs) (`battery_shield_18650`)

- **Danh mục:** Power & Battery Management
- **Điện áp hoạt động:** 3.7V 18650 battery to 5V (USB-A) and 3.3V pin headers
- **Mức logic (Logic Level):** Power module
- **Dòng tiêu thụ điển hình:** ~15mA
- **Giao diện / Bus:** USB-A output, 5V/GND headers, 3V/GND headers, MicroUSB/Type-C charging
- **Từ khóa tìm mua:** `de pin 18650 shield v3 kem mach sac va tang ap`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `5V Header` | `pwr` | Regulated 5V output (max 2A) |
| `3V Header` | `pwr` | Regulated 3.3V output (max 1A) |
| `GND` | `gnd` | Common ground |


#### ✅ ĐƯỢC LÀM (DOs)
- Check battery polarity marked inside the plastic holder (+ and -) carefully before insertion.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER insert 18650 in reverse (destroys charging controller IC immediately).**


#### ⚡ Lưu ý khi dùng với ESP32
> All-in-one power solution for portable ESP32 robots. Has physical ON/OFF switch.

---

### 🔹 2S 7.4V / 8.4V Li-ion BMS Protection Board (20A) (`bms_2s_20a`)

- **Danh mục:** Power & Battery Management
- **Điện áp hoạt động:** 7.4V nominal (8.4V fully charged)
- **Mức logic (Logic Level):** Battery protection
- **Dòng tiêu thụ điển hình:** ~0.05mA
- **Giao diện / Bus:** B+, B-, BM (Middle tap), P+, P-
- **Từ khóa tìm mua:** `mach bao ve pin 2s 8.4v 20a bms`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `B+` | `pwr` | Battery 2 Positive (+8.4V) |
| `BM` | `pwr` | Mid-point tap (+4.2V between cell 1 & 2) |
| `B-` | `pwr` | Battery 1 Negative (0V) |
| `P+ / P-` | `pwr` | Charge input & Load output |


#### ✅ ĐƯỢC LÀM (DOs)
- Solder BM wire accurately to the series junction between two 18650 cells.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER mix old and new battery cells of different capacities.**


#### ⚡ Lưu ý khi dùng với ESP32
> Provides high discharge current for high-torque servos (MG996R) and high-speed DC motors.

---

### 🔹 1N5819 / SS14 / SS34 Schottky Barrier Diode (Power OR-ing) (`schottky_1n5819`)

- **Danh mục:** Power & Battery Management
- **Điện áp hoạt động:** Up to 40V Reverse Voltage
- **Mức logic (Logic Level):** Passive Semiconductor
- **Dòng tiêu thụ điển hình:** ~0mA
- **Giao diện / Bus:** Anode, Cathode (Silver band)
- **Từ khóa tìm mua:** `diode schottky 1n5819 ss14 ss34 chong nguoc nguon`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `Anode` | `pwr` | Input side (connected to Boost 5V output) |
| `Cathode (Silver Band)` | `pwr` | Output side (connected to ESP32 5V pin) |


#### ✅ ĐƯỢC LÀM (DOs)
- Use Schottky diode for power OR-ing between USB 5V and external Boost 5.1V.
- Orient silver/white cathode line facing directly toward ESP32 5V pin.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER use standard 1N4007 silicon diode (0.7V - 1.0V drop causes ESP32 brownouts).**


#### ⚡ Lưu ý khi dùng với ESP32
> Ultra low forward voltage drop (~0.25V - 0.35V). Safely allows simultaneous USB logging & battery operation.

---

### 🔹 AMS1117-3.3V Step-Down LDO Voltage Regulator Module (`ams1117_3v3`)

- **Danh mục:** Power & Battery Management
- **Điện áp hoạt động:** VIN: 4.5V - 12V, VOUT: 3.3V (800mA max)
- **Mức logic (Logic Level):** Power regulation
- **Dòng tiêu thụ điển hình:** ~5mA
- **Giao diện / Bus:** VIN, GND, VOUT
- **Từ khóa tìm mua:** `mach nguon ha ap ams1117 3.3v mini`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VIN` | `pwr` | 5V DC Input |
| `GND` | `gnd` | Ground |
| `VOUT` | `pwr` | Clean 3.3V Output |


#### ✅ ĐƯỢC LÀM (DOs)
- Add 10uF tantalum or ceramic capacitors at input and output for stability.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER input >12V (linear regulator dissipates excess voltage as heat and overheats).**


#### ⚡ Lưu ý khi dùng với ESP32
> Standard LDO found on most ESP32 DevKits. Dropout voltage is ~1.1V at full load.

---

### 🔹 AP2112K-3.3 Ultra-Low Dropout 3.3V LDO (600mA) (`ap2112k_3v3`)

- **Danh mục:** Power & Battery Management
- **Điện áp hoạt động:** VIN: 3.5V - 6.0V, VOUT: 3.3V (Dropout only 250mV at 600mA)
- **Mức logic (Logic Level):** Power regulation
- **Dòng tiêu thụ điển hình:** ~0.05mA
- **Giao diện / Bus:** VIN, GND, EN, VOUT
- **Từ khóa tìm mua:** `ap2112k-3.3 sot23-5 ldo low dropout`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VIN` | `pwr` | Input from 3.7V Li-ion battery directly |
| `GND` | `gnd` | Ground |
| `VOUT` | `pwr` | Clean 3.3V output for ESP32 |


#### ✅ ĐƯỢC LÀM (DOs)
- Use for ultra-low power ESP32 battery projects (can run directly down to 3.55V battery voltage).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER exceed 6V max input rating.**


#### ⚡ Lưu ý khi dùng với ESP32
> Featured on Adafruit and SparkFun ESP32 boards. Outstandingly low quiescent current (55uA).

---

## Displays & Visual Indicators

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **0.96" I2C OLED Display (128x64 SSD1306)** | `3.3V - 5.0V`<br>Logic: `3.3V / 5.0V compatible` | `I2C (400kHz Fast Mode)`<br>I2C: `0x3C (or 0x3D via back resistor jumper)` | ⚠️ NEVER plug in before verifying GND vs VCC silkscreen (reverse polarity burns OLED instantly). | `man hinh oled 0.96 inch i2c 128x64 ssd1306` |
| 2 | **1.3" I2C OLED Display (128x64 SH1106)** | `3.3V - 5.0V`<br>Logic: `3.3V / 5.0V compatible` | `I2C`<br>I2C: `0x3C` | ⚠️ NEVER confuse with SSD1306. | `man hinh oled 1.3 inch i2c sh1106` |
| 3 | **1.8" SPI TFT LCD Display (128x160 ST7735)** | `3.3V - 5.0V (VCC), Backlight: 3.3V`<br>Logic: `3.3V strictly on SPI signal pins` | `SPI (up to 27MHz)` | ⚠️ NEVER connect BL directly to 5V without current-limiting resistor. | `man hinh tft lcd 1.8 inch st7735 spi` |
| 4 | **2.8" SPI TFT LCD Display (240x320 ILI9341 with Touch XPT2046)** | `VCC: 3.3V - 5.0V, Logic: 3.3V strictly`<br>Logic: `3.3V strictly` | `SPI (Display up to 40MHz, Touch up to 2.5MHz)` | ⚠️ NEVER run SPI bus clock over 2.5MHz when reading Touch controller. | `man hinh 2.8 inch spi ili9341 cam ung xpt2046` |
| 5 | **1.28" Round SPI TFT IPS Display (240x240 GC9A01)** | `3.3V - 5.0V`<br>Logic: `3.3V strictly` | `SPI (4-wire)` | ⚠️ Remember that rectangular coordinate system (240x240) has cropped round corners. | `man hinh tron 1.28 inch gc9a01 ips spi 240x240` |
| 6 | **LCD 1602 Character Display with PCF8574 I2C Backpack** | `4.5V - 5.5V (optimal 5V for crisp character contrast)`<br>Logic: `5V logic (accepts 3.3V I2C with pullups to 3.3V)` | `I2C (Standard Mode 100kHz)`<br>I2C: `0x27 or 0x3F (A0, A1, A2 jumpers on backpack)` | ⚠️ NEVER power LCD VCC from 3.3V (characters will be invisible or extremely faint). | `man hinh lcd 1602 kem module i2c pcf8574` |
| 7 | **LCD 2004 20x4 Character Display with I2C Backpack** | `5.0V`<br>Logic: `5V logic / 3.3V I2C` | `I2C`<br>I2C: `0x27 or 0x3F` | ⚠️ NEVER power with less than 4.8V. | `man hinh lcd 2004 i2c xanh duong 20x4` |
| 8 | **MAX7219 8x8 Dot Matrix LED Display Module** | `4.5V - 5.5V (5.0V)`<br>Logic: `5V / 3.3V compatible SPI-like shift register` | `DIN, CS, CLK (Daisy-chainable)` | ⚠️ NEVER connect long matrix chains without auxiliary 5V power feeds. | `module led ma tran max7219 8x8 4 trong 1` |
| 9 | **WS2812B NeoPixel Addressable RGB LED (Strip / Ring / 8-LED Stick)** | `3.5V - 5.3V (optimal 5.0V)`<br>Logic: `High level input min 0.7 * VCC (~3.5V when VCC is 5V)` | `1-Wire high speed timing (800kHz NZR)` | ⚠️ NEVER power a strip of >8 LEDs directly from ESP32 board pins (each pixel draws up to 60mA at full white!). | `led rgb ws2812b neopixel 5v thanh 8 led vong tron` |
| 10 | **TM1637 4-Digit 7-Segment Display Module** | `3.3V - 5.0V`<br>Logic: `3.3V / 5.0V compatible` | `2-Wire custom synchronous (CLK, DIO)` | ⚠️ NEVER confuse TM1637 2-wire protocol with standard I2C (cannot share bus with I2C devices). | `module led 7 doan 4 so tm1637` |


### 🔹 0.96" I2C OLED Display (128x64 SSD1306) (`ssd1306_oled_096`)

- **Danh mục:** Displays & Visual Indicators
- **Điện áp hoạt động:** 3.3V - 5.0V
- **Mức logic (Logic Level):** 3.3V / 5.0V compatible
- **Dòng tiêu thụ điển hình:** ~20mA
- **Giao diện / Bus:** I2C (400kHz Fast Mode)
- **Địa chỉ mặc định:** `0x3C (or 0x3D via back resistor jumper)`
- **Từ khóa tìm mua:** `man hinh oled 0.96 inch i2c 128x64 ssd1306`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `GND` | `gnd` | Ground |
| `VCC` | `pwr` | Power (3.3V recommended) |
| `SCL` | `sig` | I2C Clock (ESP32 SCL) |
| `SDA` | `sig` | I2C Data (ESP32 SDA) |


#### ✅ ĐƯỢC LÀM (DOs)
- Check pin order on PCB silkscreen: some batches have VCC first, others have GND first!
- Use Adafruit_SSD1306 or U8g2 library for rich graphics and Vietnamese unicode support.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER plug in before verifying GND vs VCC silkscreen (reverse polarity burns OLED instantly).**


#### ⚡ Lưu ý khi dùng với ESP32
> Very sharp display, high contrast, zero backlight bleed. Draws very low power (~15mA).

---

### 🔹 1.3" I2C OLED Display (128x64 SH1106) (`sh1106_oled_13`)

- **Danh mục:** Displays & Visual Indicators
- **Điện áp hoạt động:** 3.3V - 5.0V
- **Mức logic (Logic Level):** 3.3V / 5.0V compatible
- **Dòng tiêu thụ điển hình:** ~25mA
- **Giao diện / Bus:** I2C
- **Địa chỉ mặc định:** `0x3C`
- **Từ khóa tìm mua:** `man hinh oled 1.3 inch i2c sh1106`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V - 5V |
| `GND` | `gnd` | Ground |
| `SCL` | `sig` | I2C Clock |
| `SDA` | `sig` | I2C Data |


#### ✅ ĐƯỢC LÀM (DOs)
- Select SH1106 driver in software (not SSD1306, which causes shifted 2-pixel static border).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER confuse with SSD1306.**


#### ⚡ Lưu ý khi dùng với ESP32
> Larger viewing area than 0.96". Compatible with U8g2 library (U8G2_SH1106_128X64_NONAME_F_HW_I2C).

---

### 🔹 1.8" SPI TFT LCD Display (128x160 ST7735) (`st7735_tft_18`)

- **Danh mục:** Displays & Visual Indicators
- **Điện áp hoạt động:** 3.3V - 5.0V (VCC), Backlight: 3.3V
- **Mức logic (Logic Level):** 3.3V strictly on SPI signal pins
- **Dòng tiêu thụ điển hình:** ~50mA
- **Giao diện / Bus:** SPI (up to 27MHz)
- **Từ khóa tìm mua:** `man hinh tft lcd 1.8 inch st7735 spi`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V or 5V |
| `GND` | `gnd` | Ground |
| `CS` | `sig` | Chip Select |
| `RESET` | `sig` | Hardware Reset |
| `A0 / DC` | `sig` | Data / Command select |
| `SDA / MOSI` | `sig` | SPI MOSI |
| `SCK` | `sig` | SPI Clock |
| `LED / BL` | `pwr` | Backlight 3.3V (use 100 ohm resistor if connecting to 5V) |


#### ✅ ĐƯỢC LÀM (DOs)
- Use TFT_eSPI library for ultra-fast DMA rendering on ESP32.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect BL directly to 5V without current-limiting resistor.**


#### ⚡ Lưu ý khi dùng với ESP32
> Color 16-bit RGB565. Great for retro gaming and UI dashboards.

---

### 🔹 2.8" SPI TFT LCD Display (240x320 ILI9341 with Touch XPT2046) (`ili9341_tft_28`)

- **Danh mục:** Displays & Visual Indicators
- **Điện áp hoạt động:** VCC: 3.3V - 5.0V, Logic: 3.3V strictly
- **Mức logic (Logic Level):** 3.3V strictly
- **Dòng tiêu thụ điển hình:** ~120mA
- **Giao diện / Bus:** SPI (Display up to 40MHz, Touch up to 2.5MHz)
- **Từ khóa tìm mua:** `man hinh 2.8 inch spi ili9341 cam ung xpt2046`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND` | `pwr` | Power & Ground |
| `CS/DC/MOSI/SCK/RST` | `sig` | Display SPI bus |
| `T_CLK/T_CS/T_DIN/T_DO/T_IRQ` | `sig` | Resistive Touch Controller pins |


#### ✅ ĐƯỢC LÀM (DOs)
- Use separate CS pins for Display (TFT_CS) and Touch (TOUCH_CS) sharing the same SPI bus.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER run SPI bus clock over 2.5MHz when reading Touch controller.**


#### ⚡ Lưu ý khi dùng với ESP32
> Defacto standard for rich LVGL GUI on ESP32. Configure via User_Setup.h in TFT_eSPI.

---

### 🔹 1.28" Round SPI TFT IPS Display (240x240 GC9A01) (`gc9a01_round_tft`)

- **Danh mục:** Displays & Visual Indicators
- **Điện áp hoạt động:** 3.3V - 5.0V
- **Mức logic (Logic Level):** 3.3V strictly
- **Dòng tiêu thụ điển hình:** ~40mA
- **Giao diện / Bus:** SPI (4-wire)
- **Từ khóa tìm mua:** `man hinh tron 1.28 inch gc9a01 ips spi 240x240`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND` | `pwr` | Power |
| `SCL` | `sig` | SPI Clock |
| `SDA` | `sig` | SPI MOSI |
| `RES` | `sig` | Reset |
| `DC` | `sig` | Data/Command |
| `CS` | `sig` | Chip Select |
| `BLK` | `sig` | Backlight control (PWM dimmable) |


#### ✅ ĐƯỢC LÀM (DOs)
- Perfect circular display for animated robotic eyes, smart gauge dials, and smartwatch UI.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Remember that rectangular coordinate system (240x240) has cropped round corners.**


#### ⚡ Lưu ý khi dùng với ESP32
> Vibrant IPS 178° viewing angle. Supported in TFT_eSPI and LovyanGFX.

---

### 🔹 LCD 1602 Character Display with PCF8574 I2C Backpack (`lcd_1602_i2c`)

- **Danh mục:** Displays & Visual Indicators
- **Điện áp hoạt động:** 4.5V - 5.5V (optimal 5V for crisp character contrast)
- **Mức logic (Logic Level):** 5V logic (accepts 3.3V I2C with pullups to 3.3V)
- **Dòng tiêu thụ điển hình:** ~30mA
- **Giao diện / Bus:** I2C (Standard Mode 100kHz)
- **Địa chỉ mặc định:** `0x27 or 0x3F (A0, A1, A2 jumpers on backpack)`
- **Từ khóa tìm mua:** `man hinh lcd 1602 kem module i2c pcf8574`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `GND` | `gnd` | Ground |
| `VCC` | `pwr` | 5V Power (contrast drops severely if powered by 3.3V) |
| `SDA` | `sig` | I2C Data |
| `SCL` | `sig` | I2C Clock |


#### ✅ ĐƯỢC LÀM (DOs)
- Adjust the blue contrast potentiometer on the backpack with a flat screwdriver until blocks appear.
- Run an I2C scanner sketch if screen is blank (determines if address is 0x27 or 0x3F).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER power LCD VCC from 3.3V (characters will be invisible or extremely faint).**


#### ⚡ Lưu ý khi dùng với ESP32
> Use LiquidCrystal_I2C library. ESP32 3.3V I2C pins handle PCF8574 safely.

---

### 🔹 LCD 2004 20x4 Character Display with I2C Backpack (`lcd_2004_i2c`)

- **Danh mục:** Displays & Visual Indicators
- **Điện áp hoạt động:** 5.0V
- **Mức logic (Logic Level):** 5V logic / 3.3V I2C
- **Dòng tiêu thụ điển hình:** ~45mA
- **Giao diện / Bus:** I2C
- **Địa chỉ mặc định:** `0x27 or 0x3F`
- **Từ khóa tìm mua:** `man hinh lcd 2004 i2c xanh duong 20x4`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/SDA/SCL` | `pwr` | 4-pin standard I2C header |


#### ✅ ĐƯỢC LÀM (DOs)
- Use for telemetry dashboards displaying 4 rows of 20 characters.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER power with less than 4.8V.**


#### ⚡ Lưu ý khi dùng với ESP32
> Drop-in code replacement for LCD1602 by changing constructor to lcd(0x27, 20, 4).

---

### 🔹 MAX7219 8x8 Dot Matrix LED Display Module (`max7219_matrix_8x8`)

- **Danh mục:** Displays & Visual Indicators
- **Điện áp hoạt động:** 4.5V - 5.5V (5.0V)
- **Mức logic (Logic Level):** 5V / 3.3V compatible SPI-like shift register
- **Dòng tiêu thụ điển hình:** ~200mA
- **Giao diện / Bus:** DIN, CS, CLK (Daisy-chainable)
- **Từ khóa tìm mua:** `module led ma tran max7219 8x8 4 trong 1`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 5V Power |
| `GND` | `gnd` | Ground |
| `DIN` | `sig` | Data In (from ESP32 MOSI or GPIO) |
| `CS` | `sig` | Chip Select |
| `CLK` | `sig` | Clock |
| `DOUT` | `sig` | Data Out (to next matrix DIN) |


#### ✅ ĐƯỢC LÀM (DOs)
- Cascade 4 modules in series (4-in-1 matrix) for scrolling text signs and robot mouths.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect long matrix chains without auxiliary 5V power feeds.**


#### ⚡ Lưu ý khi dùng với ESP32
> Driven via MD_MAX72xx and MD_Parola libraries for smooth text ticker animations.

---

### 🔹 WS2812B NeoPixel Addressable RGB LED (Strip / Ring / 8-LED Stick) (`ws2812b_neopixel`)

- **Danh mục:** Displays & Visual Indicators
- **Điện áp hoạt động:** 3.5V - 5.3V (optimal 5.0V)
- **Mức logic (Logic Level):** High level input min 0.7 * VCC (~3.5V when VCC is 5V)
- **Dòng tiêu thụ điển hình:** ~60mA
- **Giao diện / Bus:** 1-Wire high speed timing (800kHz NZR)
- **Từ khóa tìm mua:** `led rgb ws2812b neopixel 5v thanh 8 led vong tron`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `5V / VDD` | `pwr` | +5V Power rail (add 1000uF capacitor across +/-) |
| `GND` | `gnd` | Ground rail |
| `DIN` | `sig` | Data In (connect through 330 - 470 ohm resistor) |
| `DOUT` | `sig` | Data Out to next pixel DIN |


#### ✅ ĐƯỢC LÀM (DOs)
- Put a 330 - 470 ohm resistor in series between ESP32 GPIO and the DIN pin.
- Use Level Shifter (74AHCT125) if driving 5V strips from 3.3V ESP32 over long wires (>30cm).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER power a strip of >8 LEDs directly from ESP32 board pins (each pixel draws up to 60mA at full white!).**
- 🛑 **NEVER connect data wire before power and ground are securely connected.**


#### ⚡ Lưu ý khi dùng với ESP32
> Use Adafruit_NeoPixel or FastLED (RMT hardware peripheral based).

---

### 🔹 TM1637 4-Digit 7-Segment Display Module (`tm1637_display`)

- **Danh mục:** Displays & Visual Indicators
- **Điện áp hoạt động:** 3.3V - 5.0V
- **Mức logic (Logic Level):** 3.3V / 5.0V compatible
- **Dòng tiêu thụ điển hình:** ~40mA
- **Giao diện / Bus:** 2-Wire custom synchronous (CLK, DIO)
- **Từ khóa tìm mua:** `module led 7 doan 4 so tm1637`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V or 5V |
| `GND` | `gnd` | Ground |
| `CLK` | `sig` | Clock line |
| `DIO` | `sig` | Data I/O line |


#### ✅ ĐƯỢC LÀM (DOs)
- Use for digital clocks, countdown timers, scoreboards, and sensor readouts.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER confuse TM1637 2-wire protocol with standard I2C (cannot share bus with I2C devices).**


#### ⚡ Lưu ý khi dùng với ESP32
> Use TM1637Display library. Simple, readable in direct sunlight.

---

## Distance, Optical & Tracking Sensors

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **HC-SR04 Ultrasonic Distance Sensor (5V Only)** | `5.0V strictly`<br>Logic: `Trig: 3.3V/5V, Echo: 5V OUT (BURNS ESP32 WITHOUT LEVEL SHIFTER!)` | `Trig (10us pulse), Echo (Pulse width proportional to distance)` | ⚠️ NEVER connect Echo pin directly to ESP32 GPIO (5V signal will permanently damage ESP32 input pin!). | `cam bien sieu am hc-sr04 5v` |
| 2 | **HC-SR05 / RCWL-1601 Multi-Mode Ultrasonic Sensor (3.3V - 5V)** | `3.0V - 5.5V wide input range`<br>Logic: `3.3V native when powered by 3.3V, 5V when powered by 5V` | `GPIO / 1-Wire / UART / I2C configurable via solder jumpers on back` | ⚠️ NEVER leave OUT shorted unless intentionally selecting UART/I2C mode. | `cam bien sieu am rcwl-1601 hc-sr05 3.3v 5v` |
| 3 | **VL53L0X Time-of-Flight (ToF) Laser Distance Sensor** | `2.8V - 5.0V (onboard 2.8V LDO & level shift)`<br>Logic: `3.3V / 5.0V I2C compliant` | `I2C (Fast Mode 400kHz)`<br>I2C: `0x29 (dynamically readdressable via XSHUT pin)` | ⚠️ NEVER touch or scratch the miniature laser transmitter/receiver glass. | `cam bien khoang cach laser vl53l0x tof i2c` |
| 4 | **VL53L1X Long Range ToF Laser Distance Sensor (up to 4m)** | `3.3V - 5.0V`<br>Logic: `3.3V / 5.0V I2C` | `I2C`<br>I2C: `0x29` | ⚠️ Performance drops in direct outdoor midday sunlight (ambient infrared interference). | `cam bien khoang cach vl53l1x 4m tof` |
| 5 | **TCRT5000 Infrared Reflective Line Tracking Sensor Module** | `3.3V - 5.0V DC`<br>Logic: `3.3V / 5.0V compatible DO (Digital Out)` | `DO (Digital Comparator LM393) and AO (Analog Output)` | ⚠️ NEVER mount too close (<2mm) or too far (>25mm) from surface. | `cam bien do line tcrt5000 do ao` |
| 6 | **Active Infrared Obstacle Avoidance Sensor Module** | `3.3V - 5.0V`<br>Logic: `3.3V / 5.0V logic` | `DO (Digital Output, Active LOW)` | ⚠️ Avoid using under direct halogen or incandescent light sources (heavy IR emission triggers false alarms). | `cam bien vat can hong ngoai ir tranh vat can` |
| 7 | **RCWL-0516 Microwave Radar Doppler Motion Sensor** | `4.0V - 28V DC (VIN)`<br>Logic: `3.3V OUT strictly (safe for ESP32)` | `OUT (3.3V active HIGH pulse for ~2s on motion detection)` | ⚠️ NEVER mount directly against metal plates (shields microwave 3.18GHz radar signals). | `cam bien radar vi song rcwl-0516 chuyen dong` |
| 8 | **HC-SR501 PIR Passive Infrared Motion Detector** | `4.5V - 20V DC (VCC)`<br>Logic: `3.3V OUT strictly (3.3V High / 0V Low)` | `OUT (3.3V Digital High on heat motion)` | ⚠️ NEVER point directly at air conditioners, heaters, or open sunny windows. | `cam bien chuyen dong hong ngoai hc-sr501 pir` |
| 9 | **AM312 Mini Low-Power PIR Motion Sensor** | `2.7V - 12V DC (runs natively on 3.3V!)`<br>Logic: `3.3V digital output` | `OUT (3.3V digital)` | ⚠️ Has fixed delay (~2-3s) without manual adjustment potentiometer. | `cam bien chuyen dong mini am312 tiet kiem pin` |
| 10 | **APDS-9960 RGB, Gesture, Proximity & Ambient Light Sensor** | `2.4V - 3.6V (optimal 3.3V strictly)`<br>Logic: `3.3V I2C` | `I2C (400kHz)`<br>I2C: `0x39` | ⚠️ NEVER power with 5V (destroys photodiode array). | `cam bien cu chi apds-9960 rgb gesture i2c` |
| 11 | **BH1750 Ambient Light Sensor Module (Lux Meter)** | `3.0V - 5.0V (onboard 3.3V LDO)`<br>Logic: `3.3V / 5.0V I2C compliant` | `I2C`<br>I2C: `0x23 (default, ADDR pin low) or 0x5C (ADDR pin high)` | ⚠️ NEVER leave ADDR floating in noisy electrical environments. | `cam bien anh sang gy-302 bh1750 lux i2c` |


### 🔹 HC-SR04 Ultrasonic Distance Sensor (5V Only) (`hc_sr04`)

- **Danh mục:** Distance, Optical & Tracking Sensors
- **Điện áp hoạt động:** 5.0V strictly
- **Mức logic (Logic Level):** Trig: 3.3V/5V, Echo: 5V OUT (BURNS ESP32 WITHOUT LEVEL SHIFTER!)
- **Dòng tiêu thụ điển hình:** ~15mA
- **Giao diện / Bus:** Trig (10us pulse), Echo (Pulse width proportional to distance)
- **Từ khóa tìm mua:** `cam bien sieu am hc-sr04 5v`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | +5V Power input |
| `Trig` | `sig` | Trigger input (active HIGH 10us pulse from ESP32) |
| `Echo` | `sig` | Echo output (5V PULSE! MUST USE VOLTAGE DIVIDER / LEVEL SHIFTER) |
| `GND` | `gnd` | Common Ground |


#### ✅ ĐƯỢC LÀM (DOs)
- MUST use a voltage divider (e.g. 1k resistor in series + 2k to GND) or Logic Level Shifter on Echo pin.
- Supply clean 5V to VCC (range degrades severely below 4.7V).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect Echo pin directly to ESP32 GPIO (5V signal will permanently damage ESP32 input pin!).**
- 🛑 **NEVER try to measure closer than 2cm or farther than 400cm.**


#### ⚡ Lưu ý khi dùng với ESP32
> Calculation: distance_cm = (duration_us * 0.0343) / 2. Use pulseIn() or ESP32 RMT peripheral.

---

### 🔹 HC-SR05 / RCWL-1601 Multi-Mode Ultrasonic Sensor (3.3V - 5V) (`hc_sr05_rcwl1601`)

- **Danh mục:** Distance, Optical & Tracking Sensors
- **Điện áp hoạt động:** 3.0V - 5.5V wide input range
- **Mức logic (Logic Level):** 3.3V native when powered by 3.3V, 5V when powered by 5V
- **Dòng tiêu thụ điển hình:** ~12mA
- **Giao diện / Bus:** GPIO / 1-Wire / UART / I2C configurable via solder jumpers on back
- **Từ khóa tìm mua:** `cam bien sieu am rcwl-1601 hc-sr05 3.3v 5v`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V or 5V Power |
| `Trig` | `sig` | Trigger input |
| `Echo` | `sig` | Echo output (3.3V safe if powered by 3.3V) |
| `OUT` | `na` | Mode select / Serial out (leave floating for standard GPIO) |
| `GND` | `gnd` | Ground |


#### ✅ ĐƯỢC LÀM (DOs)
- Power from 3.3V rail if connecting directly to ESP32 without level shifter.
- If powered from 5V for maximum sound range, Echo MUST still go through level shifter.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER leave OUT shorted unless intentionally selecting UART/I2C mode.**


#### ⚡ Lưu ý khi dùng với ESP32
> Superior upgrade to HC-SR04 with true 3.3V MCU support and blind zone down to 1.5cm.

---

### 🔹 VL53L0X Time-of-Flight (ToF) Laser Distance Sensor (`vl53l0x_tof`)

- **Danh mục:** Distance, Optical & Tracking Sensors
- **Điện áp hoạt động:** 2.8V - 5.0V (onboard 2.8V LDO & level shift)
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C compliant
- **Dòng tiêu thụ điển hình:** ~19mA
- **Giao diện / Bus:** I2C (Fast Mode 400kHz)
- **Địa chỉ mặc định:** `0x29 (dynamically readdressable via XSHUT pin)`
- **Từ khóa tìm mua:** `cam bien khoang cach laser vl53l0x tof i2c`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VIN` | `pwr` | 3.3V or 5V |
| `GND` | `gnd` | Ground |
| `SCL` | `sig` | I2C SCL |
| `SDA` | `sig` | I2C SDA |
| `XSHUT` | `sig` | Shutdown pin (pull LOW to put in reset). Essential for multi-sensor! |
| `GPIO1` | `sig` | Interrupt output (optional) |


#### ✅ ĐƯỢC LÀM (DOs)
- Peel off the protective orange/yellow protective tape from optical lenses before first use!
- To use 2 or more sensors, connect each XSHUT to a distinct ESP32 GPIO to assign unique I2C addresses on boot.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER touch or scratch the miniature laser transmitter/receiver glass.**


#### ⚡ Lưu ý khi dùng với ESP32
> Millimeter accuracy, true 940nm laser light time-of-flight (up to 2 meters). Unaffected by target color.

---

### 🔹 VL53L1X Long Range ToF Laser Distance Sensor (up to 4m) (`vl53l1x_tof`)

- **Danh mục:** Distance, Optical & Tracking Sensors
- **Điện áp hoạt động:** 3.3V - 5.0V
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C
- **Dòng tiêu thụ điển hình:** ~20mA
- **Giao diện / Bus:** I2C
- **Địa chỉ mặc định:** `0x29`
- **Từ khóa tìm mua:** `cam bien khoang cach vl53l1x 4m tof`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VIN/GND/SCL/SDA/XSHUT/INT` | `sig` | Pinout identical to VL53L0X |


#### ✅ ĐƯỢC LÀM (DOs)
- Programmable Region of Interest (ROI) allowing wide or narrow laser field of view.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Performance drops in direct outdoor midday sunlight (ambient infrared interference).**


#### ⚡ Lưu ý khi dùng với ESP32
> Extends range up to 400cm (twice the range of VL53L0X). Pololu VL53L1X library recommended.

---

### 🔹 TCRT5000 Infrared Reflective Line Tracking Sensor Module (`tcrt5000_line_sensor`)

- **Danh mục:** Distance, Optical & Tracking Sensors
- **Điện áp hoạt động:** 3.3V - 5.0V DC
- **Mức logic (Logic Level):** 3.3V / 5.0V compatible DO (Digital Out)
- **Dòng tiêu thụ điển hình:** ~15mA
- **Giao diện / Bus:** DO (Digital Comparator LM393) and AO (Analog Output)
- **Từ khóa tìm mua:** `cam bien do line tcrt5000 do ao`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | Connect to 3.3V (recommended) or 5V |
| `GND` | `gnd` | Ground |
| `DO` | `sig` | Digital output (LOW over white/reflective, HIGH over black/tape) |
| `AO` | `sig` | Analog raw voltage from phototransistor |


#### ✅ ĐƯỢC LÀM (DOs)
- Adjust the onboard blue potentiometer until the indicator LED flips precisely at black/white transition.
- Maintain optimal optical distance of 5mm - 15mm from floor.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER mount too close (<2mm) or too far (>25mm) from surface.**


#### ⚡ Lưu ý khi dùng với ESP32
> Attach DO to ESP32 interrupt pin (attachInterrupt) for tachometer wheel speed measurement or line tracking.

---

### 🔹 Active Infrared Obstacle Avoidance Sensor Module (`ir_obstacle_avoidance`)

- **Danh mục:** Distance, Optical & Tracking Sensors
- **Điện áp hoạt động:** 3.3V - 5.0V
- **Mức logic (Logic Level):** 3.3V / 5.0V logic
- **Dòng tiêu thụ điển hình:** ~18mA
- **Giao diện / Bus:** DO (Digital Output, Active LOW)
- **Từ khóa tìm mua:** `cam bien vat can hong ngoai ir tranh vat can`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V - 5V |
| `GND` | `gnd` | Ground |
| `OUT` | `sig` | Digital output (LOW when obstacle detected within 2 - 30cm) |


#### ✅ ĐƯỢC LÀM (DOs)
- Calibrate detection threshold potentiometer under actual room lighting.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Avoid using under direct halogen or incandescent light sources (heavy IR emission triggers false alarms).**


#### ⚡ Lưu ý khi dùng với ESP32
> Has dual LEDs (IR transmitter and IR photodiode). Simplest collision sensor for mini rovers.

---

### 🔹 RCWL-0516 Microwave Radar Doppler Motion Sensor (`rcwl_0516_microwave`)

- **Danh mục:** Distance, Optical & Tracking Sensors
- **Điện áp hoạt động:** 4.0V - 28V DC (VIN)
- **Mức logic (Logic Level):** 3.3V OUT strictly (safe for ESP32)
- **Dòng tiêu thụ điển hình:** ~3mA
- **Giao diện / Bus:** OUT (3.3V active HIGH pulse for ~2s on motion detection)
- **Từ khóa tìm mua:** `cam bien radar vi song rcwl-0516 chuyen dong`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VIN` | `pwr` | 4V - 28V input (connect to ESP32 5V) |
| `GND` | `gnd` | Ground |
| `OUT` | `sig` | 3.3V logic output (HIGH when human/object moves) |
| `3V3` | `pwr` | 3.3V reference out (max 100mA, do not power heavy loads) |
| `CDS` | `sig` | Optional light-sensor disable pin |


#### ✅ ĐƯỢC LÀM (DOs)
- Can penetrate thin wooden doors, plastic enclosures, glass, and drywall (360° detection bubble).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER mount directly against metal plates (shields microwave 3.18GHz radar signals).**


#### ⚡ Lưu ý khi dùng với ESP32
> Far superior to PIR in high-temperature environments where human body temp matches ambient room temp.

---

### 🔹 HC-SR501 PIR Passive Infrared Motion Detector (`hc_sr501_pir`)

- **Danh mục:** Distance, Optical & Tracking Sensors
- **Điện áp hoạt động:** 4.5V - 20V DC (VCC)
- **Mức logic (Logic Level):** 3.3V OUT strictly (3.3V High / 0V Low)
- **Dòng tiêu thụ điển hình:** ~0.05mA
- **Giao diện / Bus:** OUT (3.3V Digital High on heat motion)
- **Từ khóa tìm mua:** `cam bien chuyen dong hong ngoai hc-sr501 pir`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 5V power input (has internal 3.3V regulator) |
| `OUT` | `sig` | 3.3V High signal output directly safe for ESP32 GPIO |
| `GND` | `gnd` | Ground |


#### ✅ ĐƯỢC LÀM (DOs)
- Allow 30 - 60 seconds stabilization warmup time after applying power before reading outputs.
- Adjust time delay (Tx) and sensitivity distance (Sx) via the two orange potentiometers.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER point directly at air conditioners, heaters, or open sunny windows.**


#### ⚡ Lưu ý khi dùng với ESP32
> Output is native 3.3V logic level, making it 100% safe to wire directly to any ESP32 GPIO.

---

### 🔹 AM312 Mini Low-Power PIR Motion Sensor (`am312_mini_pir`)

- **Danh mục:** Distance, Optical & Tracking Sensors
- **Điện áp hoạt động:** 2.7V - 12V DC (runs natively on 3.3V!)
- **Mức logic (Logic Level):** 3.3V digital output
- **Dòng tiêu thụ điển hình:** ~0.015mA
- **Giao diện / Bus:** OUT (3.3V digital)
- **Từ khóa tìm mua:** `cam bien chuyen dong mini am312 tiet kiem pin`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V from ESP32 |
| `OUT` | `sig` | 3.3V output |
| `GND` | `gnd` | Ground |


#### ✅ ĐƯỢC LÀM (DOs)
- Best PIR sensor for battery-powered deep-sleep wake-up circuits (draws only 15uA quiescent current).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Has fixed delay (~2-3s) without manual adjustment potentiometer.**


#### ⚡ Lưu ý khi dùng với ESP32
> Can wake ESP32 from deep sleep via ext0/ext1 GPIO wake-up interrupt.

---

### 🔹 APDS-9960 RGB, Gesture, Proximity & Ambient Light Sensor (`apds9960_gesture`)

- **Danh mục:** Distance, Optical & Tracking Sensors
- **Điện áp hoạt động:** 2.4V - 3.6V (optimal 3.3V strictly)
- **Mức logic (Logic Level):** 3.3V I2C
- **Dòng tiêu thụ điển hình:** ~10mA
- **Giao diện / Bus:** I2C (400kHz)
- **Địa chỉ mặc định:** `0x39`
- **Từ khóa tìm mua:** `cam bien cu chi apds-9960 rgb gesture i2c`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VL` | `pwr` | Optional power for internal IR LED (connect to 3.3V) |
| `GND` | `gnd` | Ground |
| `VCC` | `pwr` | 3.3V Power strictly |
| `SDA` | `sig` | I2C SDA |
| `SCL` | `sig` | I2C SCL |
| `INT` | `sig` | Interrupt output |


#### ✅ ĐƯỢC LÀM (DOs)
- Detects 4-directional hand swipes: UP, DOWN, LEFT, RIGHT, and NEAR/FAR proximity.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER power with 5V (destroys photodiode array).**


#### ⚡ Lưu ý khi dùng với ESP32
> SparkFun_APDS9960 library provides simple non-blocking gesture state machine.

---

### 🔹 BH1750 Ambient Light Sensor Module (Lux Meter) (`bh1750_light`)

- **Danh mục:** Distance, Optical & Tracking Sensors
- **Điện áp hoạt động:** 3.0V - 5.0V (onboard 3.3V LDO)
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C compliant
- **Dòng tiêu thụ điển hình:** ~0.15mA
- **Giao diện / Bus:** I2C
- **Địa chỉ mặc định:** `0x23 (default, ADDR pin low) or 0x5C (ADDR pin high)`
- **Từ khóa tìm mua:** `cam bien anh sang gy-302 bh1750 lux i2c`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V or 5V |
| `GND` | `gnd` | Ground |
| `SCL` | `sig` | I2C Clock |
| `SDA` | `sig` | I2C Data |
| `ADDR` | `sig` | Address select pin (pull to GND for 0x23) |


#### ✅ ĐƯỢC LÀM (DOs)
- Outputs direct, calibrated light intensity in Lux (1 - 65535 lx) matching human eye response.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER leave ADDR floating in noisy electrical environments.**


#### ⚡ Lưu ý khi dùng với ESP32
> Replaces crude photoresistor with lab-grade optical lux precision.

---

## IMU, Orientation & Motion

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **MPU6050 6-Axis Gyroscope & Accelerometer (GY-521)** | `3.0V - 5.0V (onboard 3.3V LDO)`<br>Logic: `3.3V / 5.0V I2C` | `I2C (400kHz)`<br>I2C: `0x68 (default with AD0 grounded) or 0x69 (AD0 tied to 3.3V)` | ⚠️ NEVER leave AD0 floating (causes intermittent I2C connection dropouts). | `cam bien goc nghieng gy-521 mpu6050 6 truc` |
| 2 | **MPU9250 9-Axis MotionTracking Device (Accel + Gyro + Magnetometer)** | `3.0V - 5.0V`<br>Logic: `3.3V I2C / SPI` | `I2C or SPI`<br>I2C: `0x68 (MPU) + 0x0C (AK8963 Magnetometer)` | ⚠️ NEVER mount near high-current motor wires or magnetic battery holders. | `cam bien mpu9250 9 truc gy-9250` |
| 3 | **QMC5883L 3-Axis Digital Compass / Magnetometer** | `3.0V - 5.0V`<br>Logic: `3.3V / 5.0V I2C` | `I2C`<br>I2C: `0x0D (Notice: HMC5883L was 0x1E, QMC5883L is 0x0D!)` | ⚠️ NEVER use HMC5883L library on QMC5883L modules (different register map and address). | `cam bien la ban so gy-271 qmc5883l i2c` |
| 4 | **BNO055 9-Axis Absolute Orientation Sensor with Sensor Fusion** | `3.0V - 5.0V`<br>Logic: `3.3V / 5.0V I2C / UART` | `I2C (clock stretching required) or UART`<br>I2C: `0x28 (default) or 0x29` | ⚠️ Notice that BNO055 uses I2C clock stretching; configure ESP32 I2C clock frequency appropriately. | `cam bien goc bno055 9 truc tu tinh toan` |
| 5 | **ADXL345 3-Axis Digital Accelerometer (SPI / I2C)** | `3.0V - 5.0V (GY-291 module has 3.3V regulator)`<br>Logic: `3.3V / 5.0V compatible` | `I2C or SPI`<br>I2C: `0x53 (default) or 0x1D (ALT ADDRESS pin tied to VCC)` | ⚠️ NEVER leave CS floating in I2C mode (tie CS to 3.3V to enable I2C mode). | `cam bien gia toc adxl345 gy-291 i2c spi` |
| 6 | **BMI160 6-Axis Inertial Measurement Unit (Bosch)** | `3.0V - 5.0V`<br>Logic: `3.3V / 5.0V I2C` | `I2C (400kHz) or SPI`<br>I2C: `0x69 (default) or 0x68` | ⚠️ Notice default address is 0x69, which is the alternate address of MPU6050. | `cam bien bmi160 6 truc bosch i2c` |
| 7 | **SW-420 Normally Closed Vibration Sensor Module** | `3.3V - 5.0V DC`<br>Logic: `3.3V / 5.0V DO` | `DO (Digital Output)` | ⚠️ Do not use for measuring continuous vibration frequency (binary impact switch only). | `cam bien rung sw-420 chong trom` |


### 🔹 MPU6050 6-Axis Gyroscope & Accelerometer (GY-521) (`mpu6050_imu`)

- **Danh mục:** IMU, Orientation & Motion
- **Điện áp hoạt động:** 3.0V - 5.0V (onboard 3.3V LDO)
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C
- **Dòng tiêu thụ điển hình:** ~4mA
- **Giao diện / Bus:** I2C (400kHz)
- **Địa chỉ mặc định:** `0x68 (default with AD0 grounded) or 0x69 (AD0 tied to 3.3V)`
- **Từ khóa tìm mua:** `cam bien goc nghieng gy-521 mpu6050 6 truc`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V (preferred) or 5V |
| `GND` | `gnd` | Ground |
| `SCL` | `sig` | I2C SCL |
| `SDA` | `sig` | I2C Data |
| `XDA/XCL` | `sig` | Auxiliary I2C bus (leave floating) |
| `AD0` | `sig` | Address pin (GND = 0x68, 3.3V = 0x69) |
| `INT` | `sig` | Data ready interrupt |


#### ✅ ĐƯỢC LÀM (DOs)
- Tie AD0 to GND firmly with a resistor or jumper wire to prevent random address flipping.
- Run DMP (Digital Motion Processor) or Madgwick/Mahony filter on ESP32 to compute drift-free pitch and roll.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER leave AD0 floating (causes intermittent I2C connection dropouts).**


#### ⚡ Lưu ý khi dùng với ESP32
> Foundation of self-balancing robots and drone flight controllers. Adafruit_MPU6050 or ElectronicCats library.

---

### 🔹 MPU9250 9-Axis MotionTracking Device (Accel + Gyro + Magnetometer) (`mpu9250_imu`)

- **Danh mục:** IMU, Orientation & Motion
- **Điện áp hoạt động:** 3.0V - 5.0V
- **Mức logic (Logic Level):** 3.3V I2C / SPI
- **Dòng tiêu thụ điển hình:** ~6mA
- **Giao diện / Bus:** I2C or SPI
- **Địa chỉ mặc định:** `0x68 (MPU) + 0x0C (AK8963 Magnetometer)`
- **Từ khóa tìm mua:** `cam bien mpu9250 9 truc gy-9250`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/SCL/SDA/EDA/ECL/AD0/INT/NCS` | `sig` | Dual I2C/SPI header |


#### ✅ ĐƯỢC LÀM (DOs)
- Perform 8-figure calibration on magnetometer to eliminate hard and soft iron distortion.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER mount near high-current motor wires or magnetic battery holders.**


#### ⚡ Lưu ý khi dùng với ESP32
> Provides absolute compass heading (Yaw) in addition to Pitch and Roll.

---

### 🔹 QMC5883L 3-Axis Digital Compass / Magnetometer (`qmc5883l_compass`)

- **Danh mục:** IMU, Orientation & Motion
- **Điện áp hoạt động:** 3.0V - 5.0V
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** I2C
- **Địa chỉ mặc định:** `0x0D (Notice: HMC5883L was 0x1E, QMC5883L is 0x0D!)`
- **Từ khóa tìm mua:** `cam bien la ban so gy-271 qmc5883l i2c`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V or 5V |
| `GND` | `gnd` | Ground |
| `SCL` | `sig` | I2C SCL |
| `SDA` | `sig` | I2C SDA |
| `DRDY` | `sig` | Data ready interrupt |


#### ✅ ĐƯỢC LÀM (DOs)
- Verify chip marking: 'DA5883' means QMC5883L (use QMC5883L compass library, address 0x0D).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER use HMC5883L library on QMC5883L modules (different register map and address).**


#### ⚡ Lưu ý khi dùng với ESP32
> Most modern blue breakout boards labeled GY-271 contain QMC5883L, not discontinued Honeywell HMC.

---

### 🔹 BNO055 9-Axis Absolute Orientation Sensor with Sensor Fusion (`bno055_sensor_fusion`)

- **Danh mục:** IMU, Orientation & Motion
- **Điện áp hoạt động:** 3.0V - 5.0V
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C / UART
- **Dòng tiêu thụ điển hình:** ~12mA
- **Giao diện / Bus:** I2C (clock stretching required) or UART
- **Địa chỉ mặc định:** `0x28 (default) or 0x29`
- **Từ khóa tìm mua:** `cam bien goc bno055 9 truc tu tinh toan`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VIN/GND/SDA/SCL/RST/INT` | `sig` | Standard I2C pinout |


#### ✅ ĐƯỢC LÀM (DOs)
- Directly outputs Euler angles (Yaw, Pitch, Roll) and Quaternions calculated on internal ARM Cortex-M0 MCU!


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Notice that BNO055 uses I2C clock stretching; configure ESP32 I2C clock frequency appropriately.**


#### ⚡ Lưu ý khi dùng với ESP32
> The king of robotic IMUs. Zero math required on ESP32 main cores.

---

### 🔹 ADXL345 3-Axis Digital Accelerometer (SPI / I2C) (`adxl345_accel`)

- **Danh mục:** IMU, Orientation & Motion
- **Điện áp hoạt động:** 3.0V - 5.0V (GY-291 module has 3.3V regulator)
- **Mức logic (Logic Level):** 3.3V / 5.0V compatible
- **Dòng tiêu thụ điển hình:** ~0.1mA
- **Giao diện / Bus:** I2C or SPI
- **Địa chỉ mặc định:** `0x53 (default) or 0x1D (ALT ADDRESS pin tied to VCC)`
- **Từ khóa tìm mua:** `cam bien gia toc adxl345 gy-291 i2c spi`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/CS/INT1/INT2/SDO/SDA/SCL` | `sig` | I2C/SPI pins |


#### ✅ ĐƯỢC LÀM (DOs)
- Has built-in hardware tap detection (single/double tap), freefall detection, and activity sensing.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER leave CS floating in I2C mode (tie CS to 3.3V to enable I2C mode).**


#### ⚡ Lưu ý khi dùng với ESP32
> Extremely low power. High resolution measurement (up to ±16g).

---

### 🔹 BMI160 6-Axis Inertial Measurement Unit (Bosch) (`bmi160_imu`)

- **Danh mục:** IMU, Orientation & Motion
- **Điện áp hoạt động:** 3.0V - 5.0V
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** I2C (400kHz) or SPI
- **Địa chỉ mặc định:** `0x69 (default) or 0x68`
- **Từ khóa tìm mua:** `cam bien bmi160 6 truc bosch i2c`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/SCL/SDA/INT1/INT2/SDO/CS` | `sig` | Bosch IMU pinout |


#### ✅ ĐƯỢC LÀM (DOs)
- Ultra low power consumption (~950uA in full gyro+accel mode). Built-in step counter hardware.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Notice default address is 0x69, which is the alternate address of MPU6050.**


#### ⚡ Lưu ý khi dùng với ESP32
> Modern Bosch replacement for MPU6050 with lower thermal gyro drift.

---

### 🔹 SW-420 Normally Closed Vibration Sensor Module (`sw420_vibration`)

- **Danh mục:** IMU, Orientation & Motion
- **Điện áp hoạt động:** 3.3V - 5.0V DC
- **Mức logic (Logic Level):** 3.3V / 5.0V DO
- **Dòng tiêu thụ điển hình:** ~8mA
- **Giao diện / Bus:** DO (Digital Output)
- **Từ khóa tìm mua:** `cam bien rung sw-420 chong trom`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V - 5V |
| `GND` | `gnd` | Ground |
| `DO` | `sig` | Digital output (normally LOW, pulses HIGH on vibration shock) |


#### ✅ ĐƯỢC LÀM (DOs)
- Attach to ESP32 interrupt to trigger anti-theft alarms and collision alerts.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Do not use for measuring continuous vibration frequency (binary impact switch only).**


#### ⚡ Lưu ý khi dùng với ESP32
> Onboard LM393 comparator with sensitivity potentiometer.

---

## Environmental & Gas Sensors

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **DHT11 Temperature & Relative Humidity Sensor** | `3.3V - 5.5V DC`<br>Logic: `3.3V / 5.0V single-bus digital` | `1-Wire proprietary single-bus` | ⚠️ NEVER poll faster than 2 seconds (causes internal heating and stale/corrupt readings). | `cam bien nhiet do do am dht11` |
| 2 | **DHT22 / AM2302 High Accuracy Temperature & Humidity Sensor** | `3.3V - 5.5V DC`<br>Logic: `3.3V / 5.0V compatible` | `1-Wire proprietary single-bus` | ⚠️ Sampling frequency is max 0.5Hz (read once every 2 seconds). | `cam bien dht22 am2302 do am chinh xac` |
| 3 | **BME280 Temperature, Humidity & Barometric Pressure Sensor** | `1.8V - 3.6V (onboard regulator accepts up to 5V on GY-BME280)`<br>Logic: `3.3V I2C / SPI` | `I2C (up to 3.4MHz) or SPI`<br>I2C: `0x76 (default on purple GY-BME280) or 0x77 (SDO tied to VCC)` | ⚠️ NEVER confuse with BMP280 (BMP280 has no humidity sensor!). | `cam bien bme280 nhiet do do am ap suat gy-bme280 i2c` |
| 4 | **BMP280 Barometric Pressure & Temperature Sensor** | `3.3V - 5.0V`<br>Logic: `3.3V / 5.0V I2C` | `I2C or SPI`<br>I2C: `0x76 (SDO low) or 0x77 (SDO high)` | ⚠️ Does NOT measure humidity (temperature and pressure only). | `cam bien ap suat khi quyen bmp280 do cao` |
| 5 | **SHT30 / SHT31 High Precision Digital Temp & Humidity Sensor** | `2.4V - 5.5V`<br>Logic: `3.3V / 5.0V I2C` | `I2C`<br>I2C: `0x44 (default) or 0x45 (ADR pin high)` | ⚠️ Avoid exposure to volatile organic solvents and condensing water. | `cam bien sht30 sht31 chinh xac cao sensirion` |
| 6 | **DS18B20 Waterproof 1-Wire Digital Temperature Sensor (Probe)** | `3.0V - 5.5V DC`<br>Logic: `3.3V / 5.0V Dallas 1-Wire` | `Dallas 1-Wire (unique 64-bit ROM address per chip)` | ⚠️ NEVER forget the 4.7k pullup resistor (sensor will not be detected on bus). | `cam bien nhiet do ds18b20 chong nuoc kem tro 4.7k` |
| 7 | **MQ-2 Smoke, Flammable Gas & LPG Sensor Module** | `5.0V strictly (heater coil requires 5V ±0.1V)`<br>Logic: `Heater: 5V, AO: 0-5V Analog, DO: 0-5V Comparator` | `AO (Analog Out) and DO (Digital Out via LM393)` | ⚠️ NEVER connect AO directly to ESP32 ADC pin (5V output can blow the ADC input!). | `cam bien khi gas khoi mq-2 5v` |
| 8 | **MQ-135 Air Quality & Hazardous Gas Sensor** | `5.0V strictly`<br>Logic: `5V power, AO requires level divider for ESP32` | `AO and DO` | ⚠️ Requires 5V heater power; will not function accurately if powered by 3.3V. | `cam bien chat luong khong khi mq-135` |
| 9 | **Capacitive Soil Moisture Sensor v1.2 (Corrosion Resistant)** | `3.3V - 5.5V DC (optimal 3.3V)`<br>Logic: `Analog Output (approx 1.2V - 3.0V)` | `Analog Voltage (VOUT)` | ⚠️ NEVER submerge the upper electronics board and wire connector in soil or water! | `cam bien do am dat dien dung capacitive v1.2 chong an mon` |
| 10 | **Raindrop / Water Droplet Detection Board with LM393** | `3.3V - 5.0V`<br>Logic: `3.3V / 5.0V DO & AO` | `DO (Digital) & AO (Analog)` | ⚠️ Power via a GPIO pin to turn on only when reading (avoids continuous electrolysis corrosion). | `cam bien mua raindrop sensor kem mach lm393` |


### 🔹 DHT11 Temperature & Relative Humidity Sensor (`dht11_sensor`)

- **Danh mục:** Environmental & Gas Sensors
- **Điện áp hoạt động:** 3.3V - 5.5V DC
- **Mức logic (Logic Level):** 3.3V / 5.0V single-bus digital
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** 1-Wire proprietary single-bus
- **Từ khóa tìm mua:** `cam bien nhiet do do am dht11`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V or 5V power |
| `DATA` | `sig` | Bi-directional single wire (requires 4.7k - 10k pullup) |
| `NC` | `na` | Not connected |
| `GND` | `gnd` | Ground |


#### ✅ ĐƯỢC LÀM (DOs)
- Use 3-pin breakout module (already has onboard 4.7k pullup resistor).
- Read at intervals no faster than once every 2 seconds (1Hz max sampling rate).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER poll faster than 2 seconds (causes internal heating and stale/corrupt readings).**


#### ⚡ Lưu ý khi dùng với ESP32
> Range: 0-50°C (±2°C), 20-90% RH (±5%). DHT sensor library by Adafruit.

---

### 🔹 DHT22 / AM2302 High Accuracy Temperature & Humidity Sensor (`dht22_sensor`)

- **Danh mục:** Environmental & Gas Sensors
- **Điện áp hoạt động:** 3.3V - 5.5V DC
- **Mức logic (Logic Level):** 3.3V / 5.0V compatible
- **Dòng tiêu thụ điển hình:** ~1.5mA
- **Giao diện / Bus:** 1-Wire proprietary single-bus
- **Từ khóa tìm mua:** `cam bien dht22 am2302 do am chinh xac`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/DATA/NC/GND` | `sig` | Standard 4-pin DHT pinout |


#### ✅ ĐƯỢC LÀM (DOs)
- Much higher accuracy than DHT11: -40 to +80°C (±0.5°C) and 0-100% RH (±2%).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Sampling frequency is max 0.5Hz (read once every 2 seconds).**


#### ⚡ Lưu ý khi dùng với ESP32
> White casing. Ideal upgrade to DHT11 for home weather stations.

---

### 🔹 BME280 Temperature, Humidity & Barometric Pressure Sensor (`bme280_sensor`)

- **Danh mục:** Environmental & Gas Sensors
- **Điện áp hoạt động:** 1.8V - 3.6V (onboard regulator accepts up to 5V on GY-BME280)
- **Mức logic (Logic Level):** 3.3V I2C / SPI
- **Dòng tiêu thụ điển hình:** ~0.5mA
- **Giao diện / Bus:** I2C (up to 3.4MHz) or SPI
- **Địa chỉ mặc định:** `0x76 (default on purple GY-BME280) or 0x77 (SDO tied to VCC)`
- **Từ khóa tìm mua:** `cam bien bme280 nhiet do do am ap suat gy-bme280 i2c`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V (preferred) |
| `GND` | `gnd` | Ground |
| `SCL` | `sig` | I2C SCL |
| `SDA` | `sig` | I2C SDA |


#### ✅ ĐƯỢC LÀM (DOs)
- Check default address: Adafruit library defaults to 0x77, but 99% of AliExpress/Shopee modules use 0x76! Pass 0x76 to bme.begin(0x76).
- Calculate accurate relative altitude from barometric pressure changes (resolution 1 meter).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER confuse with BMP280 (BMP280 has no humidity sensor!).**


#### ⚡ Lưu ý khi dùng với ESP32
> The gold standard environmental sensor. Draws only 3.6uA at 1Hz sampling.

---

### 🔹 BMP280 Barometric Pressure & Temperature Sensor (`bmp280_sensor`)

- **Danh mục:** Environmental & Gas Sensors
- **Điện áp hoạt động:** 3.3V - 5.0V
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C
- **Dòng tiêu thụ điển hình:** ~0.3mA
- **Giao diện / Bus:** I2C or SPI
- **Địa chỉ mặc định:** `0x76 (SDO low) or 0x77 (SDO high)`
- **Từ khóa tìm mua:** `cam bien ap suat khi quyen bmp280 do cao`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/SCL/SDA/CSB/SDO` | `sig` | Standard 6-pin BMP280 header |


#### ✅ ĐƯỢC LÀM (DOs)
- Use for altimeters in model rockets, quadcopter height hold, and weather trend barometers.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Does NOT measure humidity (temperature and pressure only).**


#### ⚡ Lưu ý khi dùng với ESP32
> Very low cost compared to BME280. Adafruit_BMP280 library.

---

### 🔹 SHT30 / SHT31 High Precision Digital Temp & Humidity Sensor (`sht30_sensor`)

- **Danh mục:** Environmental & Gas Sensors
- **Điện áp hoạt động:** 2.4V - 5.5V
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C
- **Dòng tiêu thụ điển hình:** ~0.8mA
- **Giao diện / Bus:** I2C
- **Địa chỉ mặc định:** `0x44 (default) or 0x45 (ADR pin high)`
- **Từ khóa tìm mua:** `cam bien sht30 sht31 chinh xac cao sensirion`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VIN/GND/SCL/SDA/AL/ADR` | `sig` | Sensirion I2C pinout |


#### ✅ ĐƯỢC LÀM (DOs)
- Fastest response time among humidity sensors. High chemical resistance.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Avoid exposure to volatile organic solvents and condensing water.**


#### ⚡ Lưu ý khi dùng với ESP32
> Industrial-grade Sensirion silicon. Extremely stable long-term calibration.

---

### 🔹 DS18B20 Waterproof 1-Wire Digital Temperature Sensor (Probe) (`ds18b20_waterproof`)

- **Danh mục:** Environmental & Gas Sensors
- **Điện áp hoạt động:** 3.0V - 5.5V DC
- **Mức logic (Logic Level):** 3.3V / 5.0V Dallas 1-Wire
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** Dallas 1-Wire (unique 64-bit ROM address per chip)
- **Từ khóa tìm mua:** `cam bien nhiet do ds18b20 chong nuoc kem tro 4.7k`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC (Red)` | `pwr` | 3.3V or 5V Power |
| `GND (Black)` | `gnd` | Ground |
| `DATA (Yellow/Blue)` | `sig` | 1-Wire bus (MUST have 4.7k pullup resistor to VCC!) |


#### ✅ ĐƯỢC LÀM (DOs)
- MUST add a 4.7k ohm pull-up resistor between DATA and VCC.
- Connect dozens of sensors on the same single wire pin (each has a unique laser-etched 64-bit ID).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER forget the 4.7k pullup resistor (sensor will not be detected on bus).**


#### ⚡ Lưu ý khi dùng với ESP32
> Stainless steel probe is waterproof. Ideal for aquarium, soil, and liquids (-55°C to +125°C). OneWire + DallasTemperature library.

---

### 🔹 MQ-2 Smoke, Flammable Gas & LPG Sensor Module (`mq2_smoke_gas`)

- **Danh mục:** Environmental & Gas Sensors
- **Điện áp hoạt động:** 5.0V strictly (heater coil requires 5V ±0.1V)
- **Mức logic (Logic Level):** Heater: 5V, AO: 0-5V Analog, DO: 0-5V Comparator
- **Dòng tiêu thụ điển hình:** ~160mA
- **Giao diện / Bus:** AO (Analog Out) and DO (Digital Out via LM393)
- **Từ khóa tìm mua:** `cam bien khi gas khoi mq-2 5v`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 5V Power strictly (heater coil draws ~150mA) |
| `GND` | `gnd` | Ground |
| `DO` | `sig` | Digital output threshold |
| `AO` | `sig` | Analog output (0 - 5V. MUST use voltage divider to ESP32 ADC!) |


#### ✅ ĐƯỢC LÀM (DOs)
- Allow 24-48 hours preheating burn-in time for new sensors before calibrating.
- Use resistor divider (e.g. 10k + 15k) to scale AO down from 5V to max 3.0V for ESP32 ADC.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect AO directly to ESP32 ADC pin (5V output can blow the ADC input!).**
- 🛑 **NEVER touch sensor metal casing during operation (internal heater coil operates at ~200°C and gets warm).**


#### ⚡ Lưu ý khi dùng với ESP32
> Detects Methane, Butane, LPG, Smoke. Connect to ADC1 pins on ESP32.

---

### 🔹 MQ-135 Air Quality & Hazardous Gas Sensor (`mq135_air_quality`)

- **Danh mục:** Environmental & Gas Sensors
- **Điện áp hoạt động:** 5.0V strictly
- **Mức logic (Logic Level):** 5V power, AO requires level divider for ESP32
- **Dòng tiêu thụ điển hình:** ~160mA
- **Giao diện / Bus:** AO and DO
- **Từ khóa tìm mua:** `cam bien chat luong khong khi mq-135`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/DO/AO` | `sig` | Standard MQ series pinout |


#### ✅ ĐƯỢC LÀM (DOs)
- Detects Ammonia (NH3), NOx, Alcohol, Benzene, Smoke, and CO2 in indoor environments.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Requires 5V heater power; will not function accurately if powered by 3.3V.**


#### ⚡ Lưu ý khi dùng với ESP32
> Use MQ135 library with temperature and humidity compensation from BME280.

---

### 🔹 Capacitive Soil Moisture Sensor v1.2 (Corrosion Resistant) (`capacitive_soil_moisture`)

- **Danh mục:** Environmental & Gas Sensors
- **Điện áp hoạt động:** 3.3V - 5.5V DC (optimal 3.3V)
- **Mức logic (Logic Level):** Analog Output (approx 1.2V - 3.0V)
- **Dòng tiêu thụ điển hình:** ~5mA
- **Giao diện / Bus:** Analog Voltage (VOUT)
- **Từ khóa tìm mua:** `cam bien do am dat dien dung capacitive v1.2 chong an mon`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V Power from ESP32 |
| `GND` | `gnd` | Ground |
| `AOUT` | `sig` | Analog Output directly safe for ESP32 ADC1 pin |


#### ✅ ĐƯỢC LÀM (DOs)
- Power directly from ESP32 3.3V rail (makes analog output directly compatible with ESP32 3.3V ADC).
- Calibrate in air (Dry value ~2.8V) and submerged in water up to white line (Wet value ~1.3V).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER submerge the upper electronics board and wire connector in soil or water!**
- 🛑 **Do NOT use cheap resistive fork sensors (they corrode to dust in weeks via electrolysis; capacitive does NOT corrode).**


#### ⚡ Lưu ý khi dùng với ESP32
> Notice inverse reading: HIGHER voltage = DRIER soil, LOWER voltage = WETTER soil.

---

### 🔹 Raindrop / Water Droplet Detection Board with LM393 (`raindrop_sensor`)

- **Danh mục:** Environmental & Gas Sensors
- **Điện áp hoạt động:** 3.3V - 5.0V
- **Mức logic (Logic Level):** 3.3V / 5.0V DO & AO
- **Dòng tiêu thụ điển hình:** ~15mA
- **Giao diện / Bus:** DO (Digital) & AO (Analog)
- **Từ khóa tìm mua:** `cam bien mua raindrop sensor kem mach lm393`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/DO/AO` | `sig` | 4-pin sensor module header |


#### ✅ ĐƯỢC LÀM (DOs)
- Use for automatic smart clothesline retraction and smart irrigation controllers.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Power via a GPIO pin to turn on only when reading (avoids continuous electrolysis corrosion).**


#### ⚡ Lưu ý khi dùng với ESP32
> Nickel-coated tracks detect rain drops touching adjacent interdigitated fingers.

---

## User Input & HMI Controls

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **KY-040 Rotary Encoder Module (with Push Button)** | `3.3V - 5.0V DC`<br>Logic: `3.3V / 5.0V compliant` | `Quadrature CLK, DT, and Pushbutton SW` | ⚠️ NEVER use blocking delay() loops in encoder polling code. | `module rotary encoder ky-040 chiet ap xoay vo han` |
| 2 | **2-Axis Analog Thumb Joystick (PS2 Style)** | `3.3V - 5.0V DC (optimal 3.3V)`<br>Logic: `Analog Voltage (0V - VCC), Digital SW` | `VRX (Analog), VRY (Analog), SW (Pushbutton)` | ⚠️ NEVER power with 5V if connecting VRX/VRY directly to ESP32 ADC. | `module joystick 2 truc ps2 analog` |
| 3 | **Tactile Push Button Module with 10k Pull-up Resistor** | `3.3V - 5.0V`<br>Logic: `Digital HIGH / LOW` | `Digital GPIO` | ⚠️ NEVER leave button pins floating without internal or external pullup/pulldown. | `module nut nhan tactile button co tro keo` |
| 4 | **4x4 Matrix Membrane Keypad (16 Buttons)** | `Passive switch matrix (3.3V compatible)`<br>Logic: `3.3V logic` | `8-pin header (4 Row pins, 4 Column pins)` | ⚠️ Avoid using strapping pins for keypad matrix lines. | `ban phim ma tran 4x4 membrane keypad 16 phim` |
| 5 | **TTP223 Single Capacitive Touch Switch Module** | `2.0V - 5.5V DC`<br>Logic: `3.3V / 5.0V digital output` | `I/O (Active HIGH default, configurable via solder pads A & B)` | ⚠️ NEVER place touching metal plates directly against the capacitive sensor pad. | `cam bien cham dien dung ttp223 touch switch` |
| 6 | **Linear Slide Potentiometer Module (10k Dual Output)** | `3.3V - 5.0V (optimal 3.3V)`<br>Logic: `Analog Voltage (0V - VCC)` | `OTA, OTB (Dual linear analog wiper outputs)` | ⚠️ Do not power with 5V when connecting to ESP32 ADC. | `module bien tro truot linear slide potentiometer 10k` |
| 7 | **EC11 Incremental Rotary Encoder with Push Switch (Bare Component)** | `Passive mechanical contacts (up to 5V)`<br>Logic: `Depends on pull-up resistors` | `A, B (Quadrature), C (Common Ground), D, E (Switch)` | ⚠️ NEVER forget pull-up resistors (bare contacts only switch to GND). | `chiet ap xoay ec11 rotary encoder 5 chan kem cong tac` |
| 8 | **8-Position DIP Switch Module** | `Passive contacts`<br>Logic: `Depends on pull-up/pull-down` | `8 independent SPST switches` | ⚠️ NEVER leave switches floating without pull-ups. | `cong tac gat dip switch 8 vi tri` |


### 🔹 KY-040 Rotary Encoder Module (with Push Button) (`ky040_rotary_encoder`)

- **Danh mục:** User Input & HMI Controls
- **Điện áp hoạt động:** 3.3V - 5.0V DC
- **Mức logic (Logic Level):** 3.3V / 5.0V compliant
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** Quadrature CLK, DT, and Pushbutton SW
- **Từ khóa tìm mua:** `module rotary encoder ky-040 chiet ap xoay vo han`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `GND` | `gnd` | Ground |
| `+` | `pwr` | 3.3V Power (for onboard 10k pullup resistors) |
| `SW` | `sig` | Pushbutton switch (active LOW when knob is pressed) |
| `DT` | `sig` | Quadrature Channel B (Data) |
| `CLK` | `sig` | Quadrature Channel A (Clock) |


#### ✅ ĐƯỢC LÀM (DOs)
- Attach CLK and DT to interrupt-capable GPIOs on ESP32.
- Use ESP32 PCNT (Pulse Counter) hardware peripheral for 100% glitch-free rotary tracking.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER use blocking delay() loops in encoder polling code.**


#### ⚡ Lưu ý khi dùng với ESP32
> 20 detents/pulses per 360° turn. The best UI control for menu scrolling and volume adjustment.

---

### 🔹 2-Axis Analog Thumb Joystick (PS2 Style) (`joystick_ps2_analog`)

- **Danh mục:** User Input & HMI Controls
- **Điện áp hoạt động:** 3.3V - 5.0V DC (optimal 3.3V)
- **Mức logic (Logic Level):** Analog Voltage (0V - VCC), Digital SW
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** VRX (Analog), VRY (Analog), SW (Pushbutton)
- **Từ khóa tìm mua:** `module joystick 2 truc ps2 analog`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `GND` | `gnd` | Ground |
| `+5V` | `pwr` | Connect to ESP32 3.3V rail (NOT 5V) to match ADC range! |
| `VRX` | `sig` | X-axis analog voltage (center ~1.65V) |
| `VRY` | `sig` | Y-axis analog voltage (center ~1.65V) |
| `SW` | `sig` | Center push button switch (connect to GPIO with INPUT_PULLUP) |


#### ✅ ĐƯỢC LÀM (DOs)
- Power from 3.3V so that analog voltage output never exceeds ESP32 3.3V ADC limit.
- Implement a software deadzone (e.g. ±100 ADC counts around center) to prevent robot drifting.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER power with 5V if connecting VRX/VRY directly to ESP32 ADC.**


#### ⚡ Lưu ý khi dùng với ESP32
> Dual 10k potentiometers + tactile momentary switch. Standard for robot remote controllers.

---

### 🔹 Tactile Push Button Module with 10k Pull-up Resistor (`tactile_push_button`)

- **Danh mục:** User Input & HMI Controls
- **Điện áp hoạt động:** 3.3V - 5.0V
- **Mức logic (Logic Level):** Digital HIGH / LOW
- **Dòng tiêu thụ điển hình:** ~0.3mA
- **Giao diện / Bus:** Digital GPIO
- **Từ khóa tìm mua:** `module nut nhan tactile button co tro keo`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V |
| `GND` | `gnd` | Ground |
| `OUT` | `sig` | Signal output (LOW when pressed, HIGH when released) |


#### ✅ ĐƯỢC LÀM (DOs)
- Implement 20ms - 50ms software debounce in code to prevent contact chatter.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER leave button pins floating without internal or external pullup/pulldown.**


#### ⚡ Lưu ý khi dùng với ESP32
> Use pinMode(pin, INPUT_PULLUP) if using bare 2-pin buttons without module board.

---

### 🔹 4x4 Matrix Membrane Keypad (16 Buttons) (`matrix_keypad_4x4`)

- **Danh mục:** User Input & HMI Controls
- **Điện áp hoạt động:** Passive switch matrix (3.3V compatible)
- **Mức logic (Logic Level):** 3.3V logic
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** 8-pin header (4 Row pins, 4 Column pins)
- **Từ khóa tìm mua:** `ban phim ma tran 4x4 membrane keypad 16 phim`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `R1 - R4` | `sig` | Row 1 to Row 4 pins |
| `C1 - C4` | `sig` | Column 1 to Column 4 pins |


#### ✅ ĐƯỢC LÀM (DOs)
- Use Keypad library or PCF8574 I2C expander to save 6 GPIO pins.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Avoid using strapping pins for keypad matrix lines.**


#### ⚡ Lưu ý khi dùng với ESP32
> Thin adhesive backing, 16 keys (0-9, *, #, A, B, C, D). Great for security locks.

---

### 🔹 TTP223 Single Capacitive Touch Switch Module (`ttp223_touch_switch`)

- **Danh mục:** User Input & HMI Controls
- **Điện áp hoạt động:** 2.0V - 5.5V DC
- **Mức logic (Logic Level):** 3.3V / 5.0V digital output
- **Dòng tiêu thụ điển hình:** ~0.005mA
- **Giao diện / Bus:** I/O (Active HIGH default, configurable via solder pads A & B)
- **Từ khóa tìm mua:** `cam bien cham dien dung ttp223 touch switch`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V or 5V Power |
| `GND` | `gnd` | Ground |
| `I/O` | `sig` | Digital output (HIGH on touch) |


#### ✅ ĐƯỢC LÀM (DOs)
- Can sense touch through non-metallic surfaces (acrylic, glass, wood, plastic enclosures up to 3mm).
- Solder jumper A to toggle active LOW, jumper B to enable latching/toggle mode.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER place touching metal plates directly against the capacitive sensor pad.**


#### ⚡ Lưu ý khi dùng với ESP32
> Alternatively, ESP32 has 10 built-in capacitive touch pins (touchRead(T0)).

---

### 🔹 Linear Slide Potentiometer Module (10k Dual Output) (`slide_potentiometer_10k`)

- **Danh mục:** User Input & HMI Controls
- **Điện áp hoạt động:** 3.3V - 5.0V (optimal 3.3V)
- **Mức logic (Logic Level):** Analog Voltage (0V - VCC)
- **Dòng tiêu thụ điển hình:** ~0.3mA
- **Giao diện / Bus:** OTA, OTB (Dual linear analog wiper outputs)
- **Từ khóa tìm mua:** `module bien tro truot linear slide potentiometer 10k`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/OTA/OTB` | `sig` | Dual channel slider header |


#### ✅ ĐƯỢC LÀM (DOs)
- Smooth 60mm stroke slider ideal for robotic throttle and audio mixers.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Do not power with 5V when connecting to ESP32 ADC.**


#### ⚡ Lưu ý khi dùng với ESP32
> Linear B10K resistance curve.

---

### 🔹 EC11 Incremental Rotary Encoder with Push Switch (Bare Component) (`ec11_encoder_bare`)

- **Danh mục:** User Input & HMI Controls
- **Điện áp hoạt động:** Passive mechanical contacts (up to 5V)
- **Mức logic (Logic Level):** Depends on pull-up resistors
- **Dòng tiêu thụ điển hình:** ~0.1mA
- **Giao diện / Bus:** A, B (Quadrature), C (Common Ground), D, E (Switch)
- **Từ khóa tìm mua:** `chiet ap xoay ec11 rotary encoder 5 chan kem cong tac`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `A & B` | `sig` | Quadrature outputs (need pullup resistors to 3.3V) |
| `C` | `gnd` | Common center pin tied to GND |


#### ✅ ĐƯỢC LÀM (DOs)
- Add 100nF filtering capacitors on A and B lines to GND for hardware debouncing.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER forget pull-up resistors (bare contacts only switch to GND).**


#### ⚡ Lưu ý khi dùng với ESP32
> Industrial grade encoder with knurled D-shaft for metal volume knobs.

---

### 🔹 8-Position DIP Switch Module (`dip_switch_8p`)

- **Danh mục:** User Input & HMI Controls
- **Điện áp hoạt động:** Passive contacts
- **Mức logic (Logic Level):** Depends on pull-up/pull-down
- **Dòng tiêu thụ điển hình:** ~0mA
- **Giao diện / Bus:** 8 independent SPST switches
- **Từ khóa tìm mua:** `cong tac gat dip switch 8 vi tri`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `1-8 In/Out` | `sig` | 8 switch positions |


#### ✅ ĐƯỢC LÀM (DOs)
- Use for hardware mode selection, robot team ID, and WiFi channel configuration.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER leave switches floating without pull-ups.**


#### ⚡ Lưu ý khi dùng với ESP32
> Connect to PCF8574 I2C expander to read all 8 switches over 2 I2C pins.

---

## Relays, MOSFETs & Power Switches

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **1-Channel 5V Relay Module with Optocoupler Isolation** | `VCC: 5.0V (Relay coil), IN: 3.3V - 5.0V trigger`<br>Logic: `Active LOW (usually) or selectable via jumper` | `IN (Digital trigger), COM, NO, NC screw terminals` | ⚠️ NEVER power relay coil (DC+) from ESP32 3.3V rail (5V relay coil will not trigger reliably and may brown out ESP32). | `module relay 1 kenh 5v cach ly quang opto` |
| 2 | **2-Channel 5V Relay Module with Optocoupler Isolation** | `5V DC coil power`<br>Logic: `3.3V / 5V trigger` | `IN1, IN2, Dual COM/NO/NC terminals` | ⚠️ Coils draw ~70mA each (2 coils = 140mA). Do NOT power from USB-TTL converters. | `module relay 2 kenh 5v opto cach ly` |
| 3 | **4-Channel 5V Relay Module with Optocoupler Isolation** | `5V DC coil power`<br>Logic: `3.3V / 5V trigger` | `IN1 - IN4` | ⚠️ NEVER leave relay coils unpowered. | `module relay 4 kenh 5v opto songle` |
| 4 | **IRF520 MOSFET Driver Module** | `VIN: 0 - 24V DC, Load current: max 5A (with heatsink)`<br>Logic: `3.3V - 5V PWM/Digital input` | `SIG, VCC, GND, VIN, VOUT terminals` | ⚠️ CAUTION: IRF520 is NOT a true logic-level MOSFET (Vgs threshold is 2V - 4V). At 3.3V gate drive, it does NOT turn fully ON and can overheat if driving >1.5A loads! | `module mosfet irf520 dieu khien pwm dc` |
| 5 | **LR7843 High-Current Isolated MOSFET Module (30V 161A, 3.3V Logic Trigger)** | `Load: 0V - 30V DC, max 30A continuous (with heatsink)`<br>Logic: `True 3.3V / 5.0V Logic level trigger with onboard optocoupler` | `PWM/Digital input, Heavy duty screw terminals` | ⚠️ NEVER switch AC loads (DC only!). | `module mosfet lr7843 cach ly quang cong suat lon 30v` |
| 6 | **SSR-40DA 40A DC to AC Solid State Relay** | `Input: 3V - 32V DC, Output: 24V - 380V AC`<br>Logic: `3.3V - 32V DC trigger` | `Screw terminals (Input 3-4, Output 1-2)` | ⚠️ NEVER use to switch DC loads (SCR/Triac only turns off when AC current crosses zero; stays stuck ON with DC!). | `ro le ban dan ssr-40da 40a fotek` |
| 7 | **Micro Limit Switch / Roller Lever Endstop** | `Up to 250V AC / 30V DC`<br>Logic: `Passive mechanical contact` | `COM, NO, NC` | ⚠️ Do not bend lever beyond mechanical spring limit. | `cong tac hanh trinh micro limit switch co banh xe` |
| 8 | **SS12D00 / SS12F15 SPDT Miniature Slide Switch** | `Up to 50V DC 0.5A`<br>Logic: `Power switch` | `3 pins (Common center, Left throw, Right throw)` | ⚠️ NEVER switch currents > 1A directly through tiny sub-miniature slide switches (use rocker switch for heavy bots). | `cong tac gat ss12d00 3 chan spdt` |


### 🔹 1-Channel 5V Relay Module with Optocoupler Isolation (`relay_1ch_5v_opto`)

- **Danh mục:** Relays, MOSFETs & Power Switches
- **Điện áp hoạt động:** VCC: 5.0V (Relay coil), IN: 3.3V - 5.0V trigger
- **Mức logic (Logic Level):** Active LOW (usually) or selectable via jumper
- **Dòng tiêu thụ điển hình:** ~70mA
- **Giao diện / Bus:** IN (Digital trigger), COM, NO, NC screw terminals
- **Từ khóa tìm mua:** `module relay 1 kenh 5v cach ly quang opto`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `DC+` | `pwr` | +5V Power for relay coil |
| `DC-` | `gnd` | Ground |
| `IN` | `sig` | Trigger input (draws only ~3mA through optocoupler LED) |
| `COM` | `pwr` | Common switch terminal |
| `NO` | `pwr` | Normally Open terminal (closes when relay activates) |
| `NC` | `pwr` | Normally Closed terminal (opens when relay activates) |


#### ✅ ĐƯỢC LÀM (DOs)
- Use optical isolation jumper (remove VCC-JDVCC jumper to completely isolate noisy coils from MCU power).
- Switches AC 250V 10A or DC 30V 10A loads safely.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER power relay coil (DC+) from ESP32 3.3V rail (5V relay coil will not trigger reliably and may brown out ESP32).**
- 🛑 **NEVER touch 220V AC screw terminals while plugged into mains power!**


#### ⚡ Lưu ý khi dùng với ESP32
> Most 5V modules trigger reliably from ESP32 3.3V GPIO because the optocoupler LED threshold is ~1.8V.

---

### 🔹 2-Channel 5V Relay Module with Optocoupler Isolation (`relay_2ch_5v_opto`)

- **Danh mục:** Relays, MOSFETs & Power Switches
- **Điện áp hoạt động:** 5V DC coil power
- **Mức logic (Logic Level):** 3.3V / 5V trigger
- **Dòng tiêu thụ điển hình:** ~140mA
- **Giao diện / Bus:** IN1, IN2, Dual COM/NO/NC terminals
- **Từ khóa tìm mua:** `module relay 2 kenh 5v opto cach ly`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/IN1/IN2` | `sig` | 4-pin input header |


#### ✅ ĐƯỢC LÀM (DOs)
- Provide dedicated 5V power supply when triggering multiple coils simultaneously.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Coils draw ~70mA each (2 coils = 140mA). Do NOT power from USB-TTL converters.**


#### ⚡ Lưu ý khi dùng với ESP32
> Songle SRD-05VDC-SL-C relays onboard.

---

### 🔹 4-Channel 5V Relay Module with Optocoupler Isolation (`relay_4ch_5v_opto`)

- **Danh mục:** Relays, MOSFETs & Power Switches
- **Điện áp hoạt động:** 5V DC coil power
- **Mức logic (Logic Level):** 3.3V / 5V trigger
- **Dòng tiêu thụ điển hình:** ~280mA
- **Giao diện / Bus:** IN1 - IN4
- **Từ khóa tìm mua:** `module relay 4 kenh 5v opto songle`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/IN1-IN4/JD-VCC` | `sig` | Multi-channel isolated header |


#### ✅ ĐƯỢC LÀM (DOs)
- Remove JD-VCC jumper and feed independent 5V power directly to JD-VCC and GND.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER leave relay coils unpowered.**


#### ⚡ Lưu ý khi dùng với ESP32
> Standard for smart home AC outlet automation (lights, fans, water pumps).

---

### 🔹 IRF520 MOSFET Driver Module (`irf520_mosfet`)

- **Danh mục:** Relays, MOSFETs & Power Switches
- **Điện áp hoạt động:** VIN: 0 - 24V DC, Load current: max 5A (with heatsink)
- **Mức logic (Logic Level):** 3.3V - 5V PWM/Digital input
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** SIG, VCC, GND, VIN, VOUT terminals
- **Từ khóa tìm mua:** `module mosfet irf520 dieu khien pwm dc`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `SIG` | `sig` | Gate trigger from ESP32 GPIO |
| `VCC` | `pwr` | Logic reference |
| `GND` | `gnd` | Ground |


#### ✅ ĐƯỢC LÀM (DOs)
- Use for switching DC LED strips, high-power solenoids, and DC motors via PWM.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **CAUTION: IRF520 is NOT a true logic-level MOSFET (Vgs threshold is 2V - 4V). At 3.3V gate drive, it does NOT turn fully ON and can overheat if driving >1.5A loads!**


#### ⚡ Lưu ý khi dùng với ESP32
> For loads > 2A with 3.3V ESP32, use LR7843 or AO3400 logic-level MOSFET modules instead.

---

### 🔹 LR7843 High-Current Isolated MOSFET Module (30V 161A, 3.3V Logic Trigger) (`lr7843_mosfet_isolated`)

- **Danh mục:** Relays, MOSFETs & Power Switches
- **Điện áp hoạt động:** Load: 0V - 30V DC, max 30A continuous (with heatsink)
- **Mức logic (Logic Level):** True 3.3V / 5.0V Logic level trigger with onboard optocoupler
- **Dòng tiêu thụ điển hình:** ~5mA
- **Giao diện / Bus:** PWM/Digital input, Heavy duty screw terminals
- **Từ khóa tìm mua:** `module mosfet lr7843 cach ly quang cong suat lon 30v`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `PWM / SIG` | `sig` | Gate trigger from ESP32 (accepts 3.3V directly) |
| `GND` | `gnd` | Signal ground |
| `+ / - (Load)` | `pwr` | Heavy current load screw terminals |


#### ✅ ĐƯỢC LÀM (DOs)
- The BEST MOSFET module for ESP32: fully saturates at 3.3V gate drive with ultra-low Rdson (3.3 mOhm!).
- Switches high-speed PWM (up to 20kHz) for silent motor and LED dimming.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER switch AC loads (DC only!).**


#### ⚡ Lưu ý khi dùng với ESP32
> Far superior to IRF520. Runs completely cold at 5A - 10A currents without heatsink.

---

### 🔹 SSR-40DA 40A DC to AC Solid State Relay (`ssr_40da_solid_state`)

- **Danh mục:** Relays, MOSFETs & Power Switches
- **Điện áp hoạt động:** Input: 3V - 32V DC, Output: 24V - 380V AC
- **Mức logic (Logic Level):** 3.3V - 32V DC trigger
- **Dòng tiêu thụ điển hình:** ~15mA
- **Giao diện / Bus:** Screw terminals (Input 3-4, Output 1-2)
- **Từ khóa tìm mua:** `ro le ban dan ssr-40da 40a fotek`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `Terminal 3 (+)` | `sig` | Positive DC trigger from ESP32 GPIO |
| `Terminal 4 (-)` | `gnd` | Ground |
| `Terminal 1 & 2` | `pwr` | AC Mains load switch (in series with AC live wire) |


#### ✅ ĐƯỢC LÀM (DOs)
- Completely silent operation (no mechanical clicking). No electrical contact arcing.
- MUST mount on aluminum heatsink if switching loads > 5A (heats up due to 1.6V triac drop).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER use to switch DC loads (SCR/Triac only turns off when AC current crosses zero; stays stuck ON with DC!).**
- 🛑 **NEVER touch metal base plate without grounding heatsink.**


#### ⚡ Lưu ý khi dùng với ESP32
> Zero-crossing switching avoids EMI noise. Perfect for heating elements and coffee makers.

---

### 🔹 Micro Limit Switch / Roller Lever Endstop (`micro_limit_switch`)

- **Danh mục:** Relays, MOSFETs & Power Switches
- **Điện áp hoạt động:** Up to 250V AC / 30V DC
- **Mức logic (Logic Level):** Passive mechanical contact
- **Dòng tiêu thụ điển hình:** ~0mA
- **Giao diện / Bus:** COM, NO, NC
- **Từ khóa tìm mua:** `cong tac hanh trinh micro limit switch co banh xe`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `COM` | `gnd` | Common terminal (connect to ESP32 GND) |
| `NO` | `sig` | Normally Open (connect to GPIO with INPUT_PULLUP) |
| `NC` | `sig` | Normally Closed |


#### ✅ ĐƯỢC LÀM (DOs)
- Wire between GPIO and GND using internal INPUT_PULLUP for fail-safe collision detection.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Do not bend lever beyond mechanical spring limit.**


#### ⚡ Lưu ý khi dùng với ESP32
> Essential physical endstops for 3D printers, CNC axes, and robot bumpers.

---

### 🔹 SS12D00 / SS12F15 SPDT Miniature Slide Switch (`slide_switch_spdt`)

- **Danh mục:** Relays, MOSFETs & Power Switches
- **Điện áp hoạt động:** Up to 50V DC 0.5A
- **Mức logic (Logic Level):** Power switch
- **Dòng tiêu thụ điển hình:** ~0mA
- **Giao diện / Bus:** 3 pins (Common center, Left throw, Right throw)
- **Từ khóa tìm mua:** `cong tac gat ss12d00 3 chan spdt`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `Pin 1` | `pwr` | Input from Battery OUT+ |
| `Pin 2 (Center)` | `pwr` | Switched Output to Boost IN+ |
| `Pin 3` | `na` | Off state / unconnected |


#### ✅ ĐƯỢC LÀM (DOs)
- Install on battery OUT+ line to cleanly power off entire robot when not in use.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER switch currents > 1A directly through tiny sub-miniature slide switches (use rocker switch for heavy bots).**


#### ⚡ Lưu ý khi dùng với ESP32
> Standard power switch used on Otto DIY robot chassis.

---

## Audio, Sound & Buzzer

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **Active Buzzer Module (3.3V - 5V)** | `3.3V - 5.0V DC`<br>Logic: `Digital HIGH turns tone ON, LOW turns tone OFF` | `Digital GPIO (on/off)` | ⚠️ NEVER attempt to play musical melodies (active buzzer can only produce 1 fixed single pitch). | `coi chip active buzzer 3.3v 5v keu chu dong` |
| 2 | **Passive Buzzer Module (Requires PWM)** | `3.3V - 5.0V`<br>Logic: `PWM frequency driven` | `PWM (ESP32 ledcWriteTone)` | ⚠️ Applying constant DC HIGH will only produce a tiny click sound, NOT a continuous tone. | `coi chip bi dong passive buzzer phat nhac pwm` |
| 3 | **MAX98357A I2S 3.2W Class-D Mono Audio Amplifier** | `2.5V - 5.5V DC (optimal 5V for 3.2W output into 4 ohm speaker)`<br>Logic: `3.3V I2S digital audio bus` | `I2S (BCLK, LRC, DIN)` | ⚠️ NEVER connect speaker - terminal to ground (Class-D bridge tied load output!). | `mach khuech dai am thanh i2s max98357a dac 3w` |
| 4 | **INMP441 Omnidirectional Digital I2S Microphone Module** | `1.8V - 3.3V strictly`<br>Logic: `3.3V I2S digital audio` | `I2S (SCK, WS, SD, L/R)` | ⚠️ NEVER supply 5V to VDD (instantly burns MEMS microphone element). | `micro i2s inmp441 thu am mems esp32` |
| 5 | **PAM8403 2x3W Miniature Stereo Audio Amplifier (5V)** | `2.5V - 5.5V DC (optimal 5.0V)`<br>Logic: `Analog Audio In (L, R, GND)` | `L-In, Ground, R-In, L-Out (+/-), R-Out (+/-)` | ⚠️ NEVER connect L- and R- speaker grounds together (independent bridge tied outputs). | `mach khuech dai pam8403 2x3w co chiet ap` |
| 6 | **DFPlayer Mini MP3 Player Module** | `3.3V - 5.0V DC (optimal 4.2V - 5.0V for full speaker loudness)`<br>Logic: `UART Serial 9600 baud (RX requires 1k resistor from 3.3V ESP32 TX)` | `UART (9600 baud, 8-N-1)` | ⚠️ NEVER connect directly without 1k series resistor on RX line (causes speaker popping/hum). | `module mp3 dfplayer mini kem khe the nho` |
| 7 | **Sound Detection Sensor with Microphone & LM393** | `3.3V - 5.0V`<br>Logic: `3.3V / 5.0V DO and AO` | `DO (Digital Out), AO (Analog Out)` | ⚠️ Cannot record speech or audio waveforms (electret condenser + comparator only). | `cam bien am thanh microphone lm393` |


### 🔹 Active Buzzer Module (3.3V - 5V) (`buzzer_active_3v3`)

- **Danh mục:** Audio, Sound & Buzzer
- **Điện áp hoạt động:** 3.3V - 5.0V DC
- **Mức logic (Logic Level):** Digital HIGH turns tone ON, LOW turns tone OFF
- **Dòng tiêu thụ điển hình:** ~30mA
- **Giao diện / Bus:** Digital GPIO (on/off)
- **Từ khóa tìm mua:** `coi chip active buzzer 3.3v 5v keu chu dong`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC / +` | `pwr` | 3.3V Power |
| `GND / -` | `gnd` | Ground |
| `I/O / S` | `sig` | Control pin (HIGH = beep, LOW = silent) |


#### ✅ ĐƯỢC LÀM (DOs)
- Identify active buzzer: has white protective sticker on top and green PCB base with built-in oscillator.
- Drive directly with digitalWrite(pin, HIGH) to produce loud 2.5kHz alarm tone.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER attempt to play musical melodies (active buzzer can only produce 1 fixed single pitch).**


#### ⚡ Lưu ý khi dùng với ESP32
> Simplest audio feedback for robot startup, button clicks, and low battery alarms.

---

### 🔹 Passive Buzzer Module (Requires PWM) (`buzzer_passive_pwm`)

- **Danh mục:** Audio, Sound & Buzzer
- **Điện áp hoạt động:** 3.3V - 5.0V
- **Mức logic (Logic Level):** PWM frequency driven
- **Dòng tiêu thụ điển hình:** ~25mA
- **Giao diện / Bus:** PWM (ESP32 ledcWriteTone)
- **Từ khóa tìm mua:** `coi chip bi dong passive buzzer phat nhac pwm`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/S` | `sig` | PWM signal pin |


#### ✅ ĐƯỢC LÀM (DOs)
- Identify passive buzzer: open black casing exposing internal green PCB and coil.
- Use tone() or ledcWriteTone(channel, freq) to play full 8-octave musical notes, RTTTL ringtones, and Mario songs.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Applying constant DC HIGH will only produce a tiny click sound, NOT a continuous tone.**


#### ⚡ Lưu ý khi dùng với ESP32
> Allows Otto robot to make R2-D2 expressive bleeps, whirs, and sad sounds.

---

### 🔹 MAX98357A I2S 3.2W Class-D Mono Audio Amplifier (`max98357a_i2s_dac`)

- **Danh mục:** Audio, Sound & Buzzer
- **Điện áp hoạt động:** 2.5V - 5.5V DC (optimal 5V for 3.2W output into 4 ohm speaker)
- **Mức logic (Logic Level):** 3.3V I2S digital audio bus
- **Dòng tiêu thụ điển hình:** ~400mA
- **Giao diện / Bus:** I2S (BCLK, LRC, DIN)
- **Từ khóa tìm mua:** `mach khuech dai am thanh i2s max98357a dac 3w`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VIN` | `pwr` | 5V Power supply |
| `GND` | `gnd` | Ground |
| `BCLK` | `sig` | Bit Clock (ESP32 I2S BCLK) |
| `LRC` | `sig` | Left/Right Word Select Clock (ESP32 I2S WS) |
| `DIN` | `sig` | Serial Audio Data (ESP32 I2S DOUT) |
| `GAIN` | `sig` | Gain select (floating = 9dB, tie to GND for 3dB, to VIN for 15dB) |
| `SD` | `sig` | Shutdown / Channel mix |


#### ✅ ĐƯỢC LÀM (DOs)
- Use ESP32-audioI2S or Phil Schatzmann's arduino-audio-tools library for internet radio and MP3 playback.
- Connect directly to 4 ohm or 8 ohm 3W speaker.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect speaker - terminal to ground (Class-D bridge tied load output!).**


#### ⚡ Lưu ý khi dùng với ESP32
> Pristine digital audio quality directly from ESP32 I2S DMA. No analog noise or hum.

---

### 🔹 INMP441 Omnidirectional Digital I2S Microphone Module (`inmp441_i2s_mic`)

- **Danh mục:** Audio, Sound & Buzzer
- **Điện áp hoạt động:** 1.8V - 3.3V strictly
- **Mức logic (Logic Level):** 3.3V I2S digital audio
- **Dòng tiêu thụ điển hình:** ~1.4mA
- **Giao diện / Bus:** I2S (SCK, WS, SD, L/R)
- **Từ khóa tìm mua:** `micro i2s inmp441 thu am mems esp32`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VDD` | `pwr` | 3.3V Power strictly from ESP32 (NEVER 5V!) |
| `GND` | `gnd` | Ground |
| `SD` | `sig` | Serial Data Output to ESP32 I2S DIN |
| `SCK` | `sig` | Serial Clock from ESP32 I2S BCLK |
| `WS` | `sig` | Word Select from ESP32 I2S WS |
| `L/R` | `sig` | Left/Right channel select (tie to GND for Left channel) |


#### ✅ ĐƯỢC LÀM (DOs)
- Keep small sound port hole on bottom of module open to the air.
- Use for on-device voice recognition (wake words) and AI speech recording streaming to OpenAI Whisper.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER supply 5V to VDD (instantly burns MEMS microphone element).**


#### ⚡ Lưu ý khi dùng với ESP32
> Studio grade 24-bit digital MEMS microphone. High 61dBA SNR. Zero analog ADC hiss.

---

### 🔹 PAM8403 2x3W Miniature Stereo Audio Amplifier (5V) (`pam8403_amp`)

- **Danh mục:** Audio, Sound & Buzzer
- **Điện áp hoạt động:** 2.5V - 5.5V DC (optimal 5.0V)
- **Mức logic (Logic Level):** Analog Audio In (L, R, GND)
- **Dòng tiêu thụ điển hình:** ~600mA
- **Giao diện / Bus:** L-In, Ground, R-In, L-Out (+/-), R-Out (+/-)
- **Từ khóa tìm mua:** `mach khuech dai pam8403 2x3w co chiet ap`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `+5V / GND` | `pwr` | 5V Power |
| `L / G / R` | `sig` | Analog stereo audio input |
| `L+/L- and R+/R-` | `pwr` | Speaker outputs |


#### ✅ ĐƯỢC LÀM (DOs)
- Install a 470uF filter capacitor near power terminals to eliminate hum.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect L- and R- speaker grounds together (independent bridge tied outputs).**


#### ⚡ Lưu ý khi dùng với ESP32
> Tiny size with integrated potentiometer volume knob and power switch.

---

### 🔹 DFPlayer Mini MP3 Player Module (`dfplayer_mini_mp3`)

- **Danh mục:** Audio, Sound & Buzzer
- **Điện áp hoạt động:** 3.3V - 5.0V DC (optimal 4.2V - 5.0V for full speaker loudness)
- **Mức logic (Logic Level):** UART Serial 9600 baud (RX requires 1k resistor from 3.3V ESP32 TX)
- **Dòng tiêu thụ điển hình:** ~200mA
- **Giao diện / Bus:** UART (9600 baud, 8-N-1)
- **Từ khóa tìm mua:** `module mp3 dfplayer mini kem khe the nho`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 5V Power |
| `GND` | `gnd` | Ground |
| `RX` | `sig` | UART Receive (put 1k resistor in series with ESP32 TX pin!) |
| `TX` | `sig` | UART Transmit to ESP32 RX |
| `SPK_1 / SPK_2` | `pwr` | Direct 3W speaker drive outputs |
| `BUSY` | `sig` | Active LOW while track is playing |


#### ✅ ĐƯỢC LÀM (DOs)
- Format MicroSD card as FAT32, create a folder named 'mp3' and name files '0001.mp3', '0002.mp3'.
- Put a 1k ohm resistor between ESP32 TX and DFPlayer RX to suppress serial noise/glitches.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect directly without 1k series resistor on RX line (causes speaker popping/hum).**


#### ⚡ Lưu ý khi dùng với ESP32
> Offloads MP3 decoding completely from ESP32. DFPlayerMini_Fast library recommended.

---

### 🔹 Sound Detection Sensor with Microphone & LM393 (`sound_sensor_lm393`)

- **Danh mục:** Audio, Sound & Buzzer
- **Điện áp hoạt động:** 3.3V - 5.0V
- **Mức logic (Logic Level):** 3.3V / 5.0V DO and AO
- **Dòng tiêu thụ điển hình:** ~5mA
- **Giao diện / Bus:** DO (Digital Out), AO (Analog Out)
- **Từ khóa tìm mua:** `cam bien am thanh microphone lm393`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/DO/AO` | `sig` | 4-pin sensor module header |


#### ✅ ĐƯỢC LÀM (DOs)
- Detects clapping hands, loud bangs, whistles, and voice triggers.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Cannot record speech or audio waveforms (electret condenser + comparator only).**


#### ⚡ Lưu ý khi dùng với ESP32
> Great for clap-activated lamps and acoustic disturbance triggers.

---

## Logic Converters, Bus & Protection

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **4-Channel Bi-Directional Logic Level Shifter (BSS138 Based)** | `LV: 1.8V - 3.3V, HV: 3.3V - 5.5V`<br>Logic: `Bi-directional level translation between LV and HV` | `LV1-LV4 (3.3V side), HV1-HV4 (5V side)` | ⚠️ NEVER reverse LV and HV (HV must ALWAYS be at higher potential than LV). | `mach chuyen doi muc logic 4 kenh 3.3v 5v level shifter` |
| 2 | **TXS0108E 8-Channel High Speed Bi-Directional Level Converter** | `VCCA: 1.2V - 3.6V, VCCB: 1.65V - 5.5V`<br>Logic: `Automatic direction sensing` | `A1 - A8 (LV), B1 - B8 (HV), OE (Output Enable)` | ⚠️ Not recommended for very heavy capacitive loads or strong pull-up resistors (<4.7k). | `mach chuyen muc logic 8 kenh txs0108e` |
| 3 | **PCF8574 8-Bit I2C I/O Expander Module** | `2.5V - 6.0V DC`<br>Logic: `3.3V / 5.0V I2C` | `I2C (100kHz Standard)`<br>I2C: `0x20 to 0x27 (or 0x38 to 0x3F for PCF8574A)` | ⚠️ Output drive capability: Strong sink (25mA to GND), but very weak source (100uA to VCC). Pull to GND to turn on LEDs! | `module mo rong 8 chan i2c pcf8574` |
| 4 | **MCP23017 16-Bit I2C I/O Expander with Interrupts** | `1.8V - 5.5V DC`<br>Logic: `3.3V / 5.0V I2C compliant` | `I2C (up to 1.7MHz High-Speed)`<br>I2C: `0x20 to 0x27 (configured via A0, A1, A2 pins)` | ⚠️ NEVER leave RESET pin floating. | `ic mo rong 16 chan i2c mcp23017` |
| 5 | **ADS1115 16-Bit 4-Channel Precision ADC with PGA (I2C)** | `2.0V - 5.5V DC`<br>Logic: `3.3V / 5.0V I2C` | `I2C (Fast Mode 400kHz)`<br>I2C: `0x48 (ADDR pin to GND), 0x49 (to VDD), 0x4A (to SDA), 0x4B (to SCL)` | ⚠️ NEVER feed analog voltage higher than VDD + 0.3V into any analog pin. | `mach adc 16 bit 4 kenh ads1115 i2c` |
| 6 | **0-25V Voltage Divider Sensor Module (5:1 Resistor Divider)** | `Input: 0V - 25V DC (when using 5V MCU) or 0V - 16.5V DC (with 3.3V ESP32)`<br>Logic: `Analog Output scaled by 1/5` | `VCC (+), GND (-) screw terminal; S (+), - (GND) pin header` | ⚠️ NEVER measure input voltages exceeding 16.5V when connected to ESP32 3.3V ADC (3.3V * 5 = 16.5V max!). | `module do dien ap 0-25v mach chia ap 5 lan` |
| 7 | **ACS712 5A / 20A / 30A Hall Effect Current Sensor** | `5.0V strictly (VCC)`<br>Logic: `Analog output centered at VCC/2 (2.5V at 0A)` | `VIOUT (Analog), Heavy duty screw terminals` | ⚠️ NEVER connect OUT pin directly to ESP32 without attenuation (can output up to 5V). | `cam bien dong acs712 5a 20a 30a hall effect` |
| 8 | **INA219 Bi-Directional High-Side DC Voltage & Current Monitor (I2C)** | `3.0V - 5.5V (Logic), Bus Voltage: 0V - 26V DC`<br>Logic: `3.3V / 5.0V I2C compliant` | `I2C (400kHz)`<br>I2C: `0x40 (default) up to 0x4F (via A0, A1 solder pads)` | ⚠️ NEVER exceed 26V bus voltage limit on Vin+/Vin-. | `module do dong ap i2c ina219 26v 3.2a` |


### 🔹 4-Channel Bi-Directional Logic Level Shifter (BSS138 Based) (`level_shifter_4ch_bss138`)

- **Danh mục:** Logic Converters, Bus & Protection
- **Điện áp hoạt động:** LV: 1.8V - 3.3V, HV: 3.3V - 5.5V
- **Mức logic (Logic Level):** Bi-directional level translation between LV and HV
- **Dòng tiêu thụ điển hình:** ~0.5mA
- **Giao diện / Bus:** LV1-LV4 (3.3V side), HV1-HV4 (5V side)
- **Từ khóa tìm mua:** `mach chuyen doi muc logic 4 kenh 3.3v 5v level shifter`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `LV` | `pwr` | Low Voltage reference: MUST connect to ESP32 3.3V |
| `HV` | `pwr` | High Voltage reference: MUST connect to 5V rail |
| `GND` | `gnd` | Common Ground: MUST connect to common system GND |
| `LV1 - LV4` | `sig` | 3.3V signal channels to ESP32 GPIOs |
| `HV1 - HV4` | `sig` | 5.0V signal channels to 5V sensors/actuators |


#### ✅ ĐƯỢC LÀM (DOs)
- Always connect LV to 3.3V, HV to 5V, and GND to common GND.
- Use for I2C, UART, SPI, and ultrasonic Trig/Echo bidirectional signals.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER reverse LV and HV (HV must ALWAYS be at higher potential than LV).**
- 🛑 **NEVER leave LV or HV unpowered while passing signals through channels.**


#### ⚡ Lưu ý khi dùng với ESP32
> Essential bridge protecting delicate ESP32 3.3V inputs when interfacing with 5V Arduino sensors.

---

### 🔹 TXS0108E 8-Channel High Speed Bi-Directional Level Converter (`txs0108e_level_shifter`)

- **Danh mục:** Logic Converters, Bus & Protection
- **Điện áp hoạt động:** VCCA: 1.2V - 3.6V, VCCB: 1.65V - 5.5V
- **Mức logic (Logic Level):** Automatic direction sensing
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** A1 - A8 (LV), B1 - B8 (HV), OE (Output Enable)
- **Từ khóa tìm mua:** `mach chuyen muc logic 8 kenh txs0108e`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCCA` | `pwr` | Connect to 3.3V |
| `VCCB` | `pwr` | Connect to 5.0V (VCCA <= VCCB!) |
| `OE` | `sig` | Output enable (pull HIGH to VCCA to enable) |


#### ✅ ĐƯỢC LÀM (DOs)
- Pull OE HIGH to VCCA to activate channels.
- Supports high-speed SPI up to 20Mbps.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Not recommended for very heavy capacitive loads or strong pull-up resistors (<4.7k).**


#### ⚡ Lưu ý khi dùng với ESP32
> Translates 8 lines simultaneously. Perfect for 8-bit parallel TFT screens or SD cards.

---

### 🔹 PCF8574 8-Bit I2C I/O Expander Module (`pcf8574_io_expander`)

- **Danh mục:** Logic Converters, Bus & Protection
- **Điện áp hoạt động:** 2.5V - 6.0V DC
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C
- **Dòng tiêu thụ điển hình:** ~2.5mA
- **Giao diện / Bus:** I2C (100kHz Standard)
- **Địa chỉ mặc định:** `0x20 to 0x27 (or 0x38 to 0x3F for PCF8574A)`
- **Từ khóa tìm mua:** `module mo rong 8 chan i2c pcf8574`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/SDA/SCL` | `sig` | I2C bus |
| `P0 - P7` | `sig` | 8 quasi-bidirectional I/O pins |
| `INT` | `sig` | Interrupt output pin (active LOW on any input change) |


#### ✅ ĐƯỢC LÀM (DOs)
- Use INT pin connected to ESP32 GPIO interrupt to detect keypad presses instantly without polling.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Output drive capability: Strong sink (25mA to GND), but very weak source (100uA to VCC). Pull to GND to turn on LEDs!**


#### ⚡ Lưu ý khi dùng với ESP32
> Saves pins: control 8 buttons, relays, or LEDs with only 2 I2C pins.

---

### 🔹 MCP23017 16-Bit I2C I/O Expander with Interrupts (`mcp23017_io_expander`)

- **Danh mục:** Logic Converters, Bus & Protection
- **Điện áp hoạt động:** 1.8V - 5.5V DC
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C compliant
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** I2C (up to 1.7MHz High-Speed)
- **Địa chỉ mặc định:** `0x20 to 0x27 (configured via A0, A1, A2 pins)`
- **Từ khóa tìm mua:** `ic mo rong 16 chan i2c mcp23017`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `GPA0 - GPA7` | `sig` | Port A 8-bit I/O |
| `GPB0 - GPB7` | `sig` | Port B 8-bit I/O |
| `INTA / INTB` | `sig` | Dual configurable interrupt pins |
| `RESET` | `sig` | Active LOW reset (must tie to VCC for normal operation!) |


#### ✅ ĐƯỢC LÀM (DOs)
- Tie RESET pin to VCC firmly (leaving it floating causes random chip resets).
- Has true push-pull 25mA source and sink outputs, plus built-in 100k internal pullup resistors.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER leave RESET pin floating.**


#### ⚡ Lưu ý khi dùng với ESP32
> The ultimate I/O expander for complex robots with dozens of limit switches, LEDs, and buttons. Adafruit_MCP23017 library.

---

### 🔹 ADS1115 16-Bit 4-Channel Precision ADC with PGA (I2C) (`ads1115_16bit_adc`)

- **Danh mục:** Logic Converters, Bus & Protection
- **Điện áp hoạt động:** 2.0V - 5.5V DC
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C
- **Dòng tiêu thụ điển hình:** ~0.15mA
- **Giao diện / Bus:** I2C (Fast Mode 400kHz)
- **Địa chỉ mặc định:** `0x48 (ADDR pin to GND), 0x49 (to VDD), 0x4A (to SDA), 0x4B (to SCL)`
- **Từ khóa tìm mua:** `mach adc 16 bit 4 kenh ads1115 i2c`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VDD/GND/SCL/SDA` | `sig` | I2C power & bus |
| `ADDR` | `sig` | Address pin |
| `ALERT/RDY` | `sig` | Conversion ready interrupt |
| `A0 - A3` | `sig` | 4 Single-Ended or 2 Differential Analog inputs |


#### ✅ ĐƯỢC LÀM (DOs)
- Use ADS1115 whenever ESP32 internal ADC is too noisy, non-linear, or unavailable (e.g. ADC2 while WiFi is on).
- Has internal programmable gain amplifier (PGA) measuring down to microvolts (±0.256V up to ±6.144V).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER feed analog voltage higher than VDD + 0.3V into any analog pin.**


#### ⚡ Lưu ý khi dùng với ESP32
> 16-bit resolution (65536 steps) versus ESP32's noisy 12-bit ADC. Indispensable for precision load cells and current shunts.

---

### 🔹 0-25V Voltage Divider Sensor Module (5:1 Resistor Divider) (`voltage_divider_module`)

- **Danh mục:** Logic Converters, Bus & Protection
- **Điện áp hoạt động:** Input: 0V - 25V DC (when using 5V MCU) or 0V - 16.5V DC (with 3.3V ESP32)
- **Mức logic (Logic Level):** Analog Output scaled by 1/5
- **Dòng tiêu thụ điển hình:** ~0.8mA
- **Giao diện / Bus:** VCC (+), GND (-) screw terminal; S (+), - (GND) pin header
- **Từ khóa tìm mua:** `module do dien ap 0-25v mach chia ap 5 lan`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `Terminal VCC` | `pwr` | Positive battery/load voltage to measure (0 - 16.5V) |
| `Terminal GND` | `gnd` | Negative battery terminal / Ground |
| `Pin S` | `sig` | Scaled analog output (Vin / 5.0). Connect directly to ESP32 ADC pin |
| `Pin -` | `gnd` | Ground to ESP32 GND |
| `Pin +` | `na` | Not connected internally on most modules |


#### ✅ ĐƯỢC LÀM (DOs)
- Use to measure 1S (3.7V - 4.2V), 2S (7.4V - 8.4V), or 3S (11.1V - 12.6V) battery packs safely on ESP32.
- Software calculation: V_battery = V_adc_measured * 5.0 * calibration_factor.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER measure input voltages exceeding 16.5V when connected to ESP32 3.3V ADC (3.3V * 5 = 16.5V max!).**


#### ⚡ Lưu ý khi dùng với ESP32
> Contains precision 30k ohm and 7.5k ohm resistors. Connect to ADC1 pin (e.g. GPIO1 on S3).

---

### 🔹 ACS712 5A / 20A / 30A Hall Effect Current Sensor (`acs712_current_sensor`)

- **Danh mục:** Logic Converters, Bus & Protection
- **Điện áp hoạt động:** 5.0V strictly (VCC)
- **Mức logic (Logic Level):** Analog output centered at VCC/2 (2.5V at 0A)
- **Dòng tiêu thụ điển hình:** ~13mA
- **Giao diện / Bus:** VIOUT (Analog), Heavy duty screw terminals
- **Từ khóa tìm mua:** `cam bien dong acs712 5a 20a 30a hall effect`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 5V Power strictly |
| `GND` | `gnd` | Ground |
| `OUT` | `sig` | Analog Output (2.5V center. Scale via voltage divider to ESP32 ADC!) |


#### ✅ ĐƯỢC LÀM (DOs)
- Use voltage divider on OUT pin to drop 0-5V down to 0-3.3V before connecting to ESP32 ADC.
- Full galvanic isolation (2.1kV RMS) between high current line and MCU.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect OUT pin directly to ESP32 without attenuation (can output up to 5V).**


#### ⚡ Lưu ý khi dùng với ESP32
> Measures both AC and DC currents. Sensitivity: 185mV/A (5A model), 100mV/A (20A model), 66mV/A (30A model).

---

### 🔹 INA219 Bi-Directional High-Side DC Voltage & Current Monitor (I2C) (`ina219_power_monitor`)

- **Danh mục:** Logic Converters, Bus & Protection
- **Điện áp hoạt động:** 3.0V - 5.5V (Logic), Bus Voltage: 0V - 26V DC
- **Mức logic (Logic Level):** 3.3V / 5.0V I2C compliant
- **Dòng tiêu thụ điển hình:** ~1mA
- **Giao diện / Bus:** I2C (400kHz)
- **Địa chỉ mặc định:** `0x40 (default) up to 0x4F (via A0, A1 solder pads)`
- **Từ khóa tìm mua:** `module do dong ap i2c ina219 26v 3.2a`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/SCL/SDA` | `sig` | I2C interface |
| `Vin+` | `pwr` | High side power positive in (from power supply) |
| `Vin-` | `pwr` | High side power load out (to robot/motor load) |


#### ✅ ĐƯỢC LÀM (DOs)
- FAR superior to ACS712: measures bus voltage (0-26V), shunt current (up to ±3.2A), and power (Watts) digitally over I2C!
- 1% precision 0.1 ohm shunt resistor onboard.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER exceed 26V bus voltage limit on Vin+/Vin-.**


#### ⚡ Lưu ý khi dùng với ESP32
> Must-have telemetry tool for robotics: logs real-time battery drain, motor stall currents, and power consumption. Adafruit_INA219 library.

---

## Wireless, RFID, GPS & Communication

| # | Linh kiện | Điện áp / Logic | Giao tiếp | Lưu ý cốt lõi | Từ khóa Shopee |
|---|-----------|-----------------|-----------|---------------|----------------|
| 1 | **RC522 13.56MHz RFID Reader/Writer Module** | `2.5V - 3.3V strictly (VCC)`<br>Logic: `3.3V SPI strictly (NOT 5V tolerant!)` | `SPI (up to 10MHz)` | ⚠️ NEVER connect 5V to VCC pin (instantly destroys MFRC522 silicon). | `module doc the rfid rc522 13.56mhz kem the tu` |
| 2 | **PN532 NFC RFID Module (I2C / SPI / HSU UART Selectable)** | `3.3V - 5.0V`<br>Logic: `3.3V / 5.0V compliant` | `I2C, SPI, or High Speed UART (configured via dual onboard DIP switches)`<br>I2C: `0x24 (I2C mode)` | ⚠️ NEVER flip DIP switches while power is connected. | `module nfc pn532 v3 doc the tu dien thoai` |
| 3 | **NEO-6M / NEO-8M GPS Satellite Receiver Module** | `3.0V - 5.0V (VCC has onboard 3.3V LDO)`<br>Logic: `UART 3.3V strictly (TX and RX)` | `UART (Default 9600 baud, 8-N-1, NMEA sentences)` | ⚠️ NEVER expect satellite lock indoors inside dense reinforced concrete buildings. | `module dinh vi gps neo-6m neo-8m kem anten gom` |
| 4 | **NRF24L01+ 2.4GHz Wireless Transceiver Module** | `1.9V - 3.6V strictly (VCC MUST NEVER EXCEED 3.6V!)`<br>Logic: `SPI pins are 5V tolerant, but VCC IS NOT!` | `SPI (up to 10Mbps)` | ⚠️ NEVER connect VCC to 5V (instantly destroys NRF24L01 chip!). | `module nrf24l01 2.4ghz truyen nhan khong day kem de nguon` |
| 5 | **HC-05 / HC-06 Bluetooth Classic Serial SPP Module** | `3.6V - 6.0V (VCC has onboard 3.3V regulator)`<br>Logic: `RX pin is 3.3V strictly (needs voltage divider from 5V MCU, safe with 3.3V ESP32)` | `UART (Default 9600 or 38400 baud in AT command mode)` | ⚠️ Remember ESP32 already has built-in Bluetooth/BLE! Only use HC-05 if offloading stack or interfacing legacy devices. | `module bluetooth hc-05 hc-06 truyen thong noi tiep` |
| 6 | **MAX485 RS-485 Half-Duplex Transceiver Module** | `4.75V - 5.25V DC (optimal 5.0V)`<br>Logic: `5V logic (accepts 3.3V TX/RX on most modern clones)` | `UART (RO, DI) + Direction control (DE, RE) + Differential A/B bus` | ⚠️ NEVER leave bus unterminated over long cable distances (causes signal reflections). | `module max485 rs485 modbus rtu` |
| 7 | **MCP2515 CAN Bus Controller with TJA1050 Transceiver (SPI)** | `5.0V strictly (TJA1050 CAN transceiver requires 5.0V for CANH/CANL differential voltages)`<br>Logic: `SPI inputs are 3.3V/5V compatible, VCC must be 5V` | `SPI (up to 10MHz)` | ⚠️ NEVER power solely from 3.3V (SPI will respond, but CAN transceiver CANNOT transmit packets!). | `module can bus mcp2515 tja1050 spi giao tiep o to` |
| 8 | **SX1278 Ra-02 433MHz LoRa Long Range Wireless Module** | `2.5V - 3.7V (optimal 3.3V strictly)`<br>Logic: `3.3V SPI strictly (NOT 5V tolerant!)` | `SPI (NSS, MOSI, MISO, SCK, DIO0)` | ⚠️ NEVER power on or transmit without antenna attached! | `module lora ra-02 sx1278 433mhz kem anten` |


### 🔹 RC522 13.56MHz RFID Reader/Writer Module (`rc522_rfid_reader`)

- **Danh mục:** Wireless, RFID, GPS & Communication
- **Điện áp hoạt động:** 2.5V - 3.3V strictly (VCC)
- **Mức logic (Logic Level):** 3.3V SPI strictly (NOT 5V tolerant!)
- **Dòng tiêu thụ điển hình:** ~26mA
- **Giao diện / Bus:** SPI (up to 10MHz)
- **Từ khóa tìm mua:** `module doc the rfid rc522 13.56mhz kem the tu`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `3.3V` | `pwr` | 3.3V Power from ESP32 strictly (NEVER 5V!) |
| `RST` | `sig` | Reset pin |
| `GND` | `gnd` | Ground |
| `IRQ` | `sig` | Interrupt pin (leave floating or connect to GPIO) |
| `MISO` | `sig` | SPI MISO |
| `MOSI` | `sig` | SPI MOSI |
| `SCK` | `sig` | SPI Clock |
| `SDA / NSS` | `sig` | SPI Chip Select (CS) |


#### ✅ ĐƯỢC LÀM (DOs)
- Always power strictly from 3.3V rail.
- Reads Mifare Classic 1K cards, S50 key fobs, and NFC Type 2 tags.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect 5V to VCC pin (instantly destroys MFRC522 silicon).**


#### ⚡ Lưu ý khi dùng với ESP32
> Popular choice for smart door locks, robot game tokens, and attendance systems. MFRC522 library.

---

### 🔹 PN532 NFC RFID Module (I2C / SPI / HSU UART Selectable) (`pn532_nfc_module`)

- **Danh mục:** Wireless, RFID, GPS & Communication
- **Điện áp hoạt động:** 3.3V - 5.0V
- **Mức logic (Logic Level):** 3.3V / 5.0V compliant
- **Dòng tiêu thụ điển hình:** ~50mA
- **Giao diện / Bus:** I2C, SPI, or High Speed UART (configured via dual onboard DIP switches)
- **Địa chỉ mặc định:** `0x24 (I2C mode)`
- **Từ khóa tìm mua:** `module nfc pn532 v3 doc the tu dien thoai`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC/GND/SDA(TX)/SCL(RX)/RSTOUT/IRQ` | `sig` | Multi-mode pinout |


#### ✅ ĐƯỢC LÀM (DOs)
- Set DIP switches: [0, 1] for I2C, [1, 0] for SPI, [0, 0] for HSU UART.
- Can emulate NFC tags and read Apple/Google Pay NFC UID.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER flip DIP switches while power is connected.**


#### ⚡ Lưu ý khi dùng với ESP32
> Much more powerful than RC522. Supports peer-to-peer NFC phone communication. Adafruit_PN532 library.

---

### 🔹 NEO-6M / NEO-8M GPS Satellite Receiver Module (`neo6m_gps_module`)

- **Danh mục:** Wireless, RFID, GPS & Communication
- **Điện áp hoạt động:** 3.0V - 5.0V (VCC has onboard 3.3V LDO)
- **Mức logic (Logic Level):** UART 3.3V strictly (TX and RX)
- **Dòng tiêu thụ điển hình:** ~50mA
- **Giao diện / Bus:** UART (Default 9600 baud, 8-N-1, NMEA sentences)
- **Từ khóa tìm mua:** `module dinh vi gps neo-6m neo-8m kem anten gom`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 3.3V or 5V Power |
| `GND` | `gnd` | Ground |
| `TX` | `sig` | UART TX out (connect to ESP32 HardwareSerial RX pin) |
| `RX` | `sig` | UART RX in (connect to ESP32 HardwareSerial TX pin) |
| `PPS` | `sig` | Pulse Per Second time synchronization pin (blinks when GPS locks) |


#### ✅ ĐƯỢC LÀM (DOs)
- Test outdoors with clear line of sight to open sky (first satellite fix takes 1 - 3 minutes).
- Use TinyGPSPlus library to easily parse Latitude, Longitude, Altitude, Speed, and Atomic Time.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER expect satellite lock indoors inside dense reinforced concrete buildings.**


#### ⚡ Lưu ý khi dùng với ESP32
> Blinking PPS LED indicates valid satellite fix. Ceramic patch antenna must face upward toward sky.

---

### 🔹 NRF24L01+ 2.4GHz Wireless Transceiver Module (`nrf24l01_wireless`)

- **Danh mục:** Wireless, RFID, GPS & Communication
- **Điện áp hoạt động:** 1.9V - 3.6V strictly (VCC MUST NEVER EXCEED 3.6V!)
- **Mức logic (Logic Level):** SPI pins are 5V tolerant, but VCC IS NOT!
- **Dòng tiêu thụ điển hình:** ~115mA
- **Giao diện / Bus:** SPI (up to 10Mbps)
- **Từ khóa tìm mua:** `module nrf24l01 2.4ghz truyen nhan khong day kem de nguon`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `GND` | `gnd` | Ground (Pin 1 top left looking at back) |
| `VCC` | `pwr` | 3.3V Power STRICTLY! (Add 10uF - 47uF capacitor across GND/VCC!) |
| `CE` | `sig` | Chip Enable (RX/TX mode control) |
| `CSN` | `sig` | SPI Chip Select |
| `SCK` | `sig` | SPI Clock |
| `MOSI` | `sig` | SPI MOSI |
| `MISO` | `sig` | SPI MISO |
| `IRQ` | `sig` | Interrupt pin (optional) |


#### ✅ ĐƯỢC LÀM (DOs)
- CRITICAL: Solder a 10uF - 47uF electrolytic or tantalum capacitor directly across VCC and GND pins on the module!
- Power from a dedicated 3.3V LDO or adapter board with onboard AMS1117 (ESP32 3.3V pin often has power spikes that reset NRF24).


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER connect VCC to 5V (instantly destroys NRF24L01 chip!).**
- 🛑 **NEVER attempt reliable transmission without decoupling capacitor across power pins.**


#### ⚡ Lưu ý khi dùng với ESP32
> Low cost long-range remote control (PA/LNA version reaches >1km). RF24 library by TMRh20.

---

### 🔹 HC-05 / HC-06 Bluetooth Classic Serial SPP Module (`hc05_bluetooth`)

- **Danh mục:** Wireless, RFID, GPS & Communication
- **Điện áp hoạt động:** 3.6V - 6.0V (VCC has onboard 3.3V regulator)
- **Mức logic (Logic Level):** RX pin is 3.3V strictly (needs voltage divider from 5V MCU, safe with 3.3V ESP32)
- **Dòng tiêu thụ điển hình:** ~40mA
- **Giao diện / Bus:** UART (Default 9600 or 38400 baud in AT command mode)
- **Từ khóa tìm mua:** `module bluetooth hc-05 hc-06 truyen thong noi tiep`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 5V Power from ESP32 |
| `GND` | `gnd` | Ground |
| `TXD` | `sig` | UART TX (3.3V level to ESP32 RX) |
| `RXD` | `sig` | UART RX (3.3V from ESP32 TX) |
| `STATE` | `sig` | Connection status output (HIGH when paired) |
| `EN / KEY` | `sig` | AT Command mode trigger (hold HIGH on power-up for 38400 baud AT mode) |


#### ✅ ĐƯỢC LÀM (DOs)
- HC-05 can be configured as Master or Slave; HC-06 is factory fixed as Slave only.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **Remember ESP32 already has built-in Bluetooth/BLE! Only use HC-05 if offloading stack or interfacing legacy devices.**


#### ⚡ Lưu ý khi dùng với ESP32
> Bluetooth 2.0 + EDR SPP. Easy serial terminal communication with Android smartphones.

---

### 🔹 MAX485 RS-485 Half-Duplex Transceiver Module (`max485_rs485`)

- **Danh mục:** Wireless, RFID, GPS & Communication
- **Điện áp hoạt động:** 4.75V - 5.25V DC (optimal 5.0V)
- **Mức logic (Logic Level):** 5V logic (accepts 3.3V TX/RX on most modern clones)
- **Dòng tiêu thụ điển hình:** ~30mA
- **Giao diện / Bus:** UART (RO, DI) + Direction control (DE, RE) + Differential A/B bus
- **Từ khóa tìm mua:** `module max485 rs485 modbus rtu`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 5V Power |
| `GND` | `gnd` | Ground |
| `RO` | `sig` | Receiver Output to ESP32 RX |
| `DI` | `sig` | Driver Input from ESP32 TX |
| `DE / RE` | `sig` | Driver / Receiver Enable (tie together to 1 ESP32 GPIO: HIGH to send, LOW to receive) |
| `A / B` | `sig` | RS-485 Differential twisted pair bus |


#### ✅ ĐƯỢC LÀM (DOs)
- Use twisted pair cable with 120 ohm termination resistors at both ends of long cable runs (>1000m).
- Tie DE and RE together to a single direction GPIO on ESP32.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER leave bus unterminated over long cable distances (causes signal reflections).**


#### ⚡ Lưu ý khi dùng với ESP32
> Standard physical layer for industrial Modbus RTU PLC communication and solar inverters.

---

### 🔹 MCP2515 CAN Bus Controller with TJA1050 Transceiver (SPI) (`mcp2515_can_bus`)

- **Danh mục:** Wireless, RFID, GPS & Communication
- **Điện áp hoạt động:** 5.0V strictly (TJA1050 CAN transceiver requires 5.0V for CANH/CANL differential voltages)
- **Mức logic (Logic Level):** SPI inputs are 3.3V/5V compatible, VCC must be 5V
- **Dòng tiêu thụ điển hình:** ~20mA
- **Giao diện / Bus:** SPI (up to 10MHz)
- **Từ khóa tìm mua:** `module can bus mcp2515 tja1050 spi giao tiep o to`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `VCC` | `pwr` | 5V Power strictly (TJA1050 will not transmit on 3.3V!) |
| `GND` | `gnd` | Ground |
| `CS/SO/SI/SCK/INT` | `sig` | SPI bus pins to ESP32 |
| `CANH / CANL` | `sig` | Differential CAN Bus lines (connect to automotive OBD-II / robot bus) |


#### ✅ ĐƯỢC LÀM (DOs)
- Supply 5V to VCC (the TJA1050 transceiver chip REQUIRES 5V to generate the ISO 11898 CAN bus differential voltage).
- Enable 120 ohm termination resistor jumper (J2) if this is an endpoint device.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER power solely from 3.3V (SPI will respond, but CAN transceiver CANNOT transmit packets!).**


#### ⚡ Lưu ý khi dùng với ESP32
> Notice ESP32 has built-in TWAI (Two-Wire Automotive Interface / CAN). Alternatively, just use SN65HVD230 3.3V transceiver directly with ESP32 TWAI.

---

### 🔹 SX1278 Ra-02 433MHz LoRa Long Range Wireless Module (`lora_ra02_sx1278`)

- **Danh mục:** Wireless, RFID, GPS & Communication
- **Điện áp hoạt động:** 2.5V - 3.7V (optimal 3.3V strictly)
- **Mức logic (Logic Level):** 3.3V SPI strictly (NOT 5V tolerant!)
- **Dòng tiêu thụ điển hình:** ~120mA
- **Giao diện / Bus:** SPI (NSS, MOSI, MISO, SCK, DIO0)
- **Từ khóa tìm mua:** `module lora ra-02 sx1278 433mhz kem anten`

#### Sơ đồ chân (Pinout)
| Chân pin | Loại | Chức năng mô tả |
|----------|------|-----------------|
| `3.3V` | `pwr` | 3.3V Power from ESP32 strictly |
| `GND` | `gnd` | Ground |
| `NSS` | `sig` | SPI Chip Select |
| `MOSI/MISO/SCK` | `sig` | SPI bus |
| `DIO0` | `sig` | Packet Rx/Tx interrupt pin (connect to ESP32 GPIO) |
| `RESET` | `sig` | Reset pin |


#### ✅ ĐƯỢC LÀM (DOs)
- MUST ALWAYS connect external 433MHz spring or SMA whip antenna BEFORE powering on (transmitting without antenna damages RF power amplifier!).
- Extreme range: up to 3km - 10km in rural line-of-sight environments.


#### ❌ KHÔNG ĐƯỢC LÀM (DON'Ts - Tránh cháy hỏng)
- 🛑 **NEVER power on or transmit without antenna attached!**
- 🛑 **NEVER connect 5V to power or signal lines.**


#### ⚡ Lưu ý khi dùng với ESP32
> Chirp Spread Spectrum (CSS) modulation. Sandeep Mistry's LoRa library or RadioLib standard.

---
