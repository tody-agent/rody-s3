# 🛠️ Cẩm Nang Cài Đặt, Debug & Xử Lý Sự Cố Firmware Robot Rody S3 (ESP32-S3 N16R8)

Tài liệu kỹ thuật chuyên sâu dành cho lập trình viên, kỹ sư nhúng và người phát triển hệ thống **Robot Rody S3 (Mochi AI Edition & Pet Edition)**. Hướng dẫn toàn diện từ thiết lập toolchain, nạp firmware, kiểm thử tự động (Unit Test / HIL Test), kỹ thuật debug thời gian thực, giải mã crash dump đến sổ tay bắt bệnh và khắc phục lỗi chi tiết.

---

## 📑 Mục Lục

1. [Kiến Trúc Firmware & Bản Đồ Bộ Nhớ](#1-kiến-trúc-firmware--bản-đồ-bộ-nhớ)
2. [Cài Đặt Môi Trường & Toolchain PlatformIO](#2-cài-đặt-môi-trường--toolchain-platformio)
3. [Quy Trình Biên Dịch & Nạp Firmware](#3-quy-trình-biên-dịch--nạp-firmware)
4. [Kiểm Thử Tự Động: Unit Test & HIL Test](#4-kiểm-thử-tự-động-unit-test--hil-test)
5. [Kỹ Thuật & Công Cụ Debug Firmware Chuyên Sâu](#5-kỹ-thuật--công-cụ-debug-firmware-chuyên-sâu)
6. [Sổ Tay Chẩn Đoán & Sửa Lỗi Firmware (Troubleshooting Matrix)](#6-sổ-tay-chẩn-đoán--sửa-lỗi-firmware-troubleshooting-matrix)
7. [Tiêu Chuẩn Lập Trình & Best Practices Cho ESP32-S3](#7-tiêu-chuẩn-lập-trình--best-practices-cho-esp32-s3)

---

## 1. Kiến Trúc Firmware & Bản Đồ Bộ Nhớ

### 1.1. Sơ Đồ Khối Phân Tầng Firmware

Firmware Rody S3 được tổ chức theo mô hình phân tầng mô-đun hóa (Layered Architecture), độc lập giữa phần cứng (HAL) và logic điều khiển cấp cao:

```
+--------------------------------------------------------------------------+
|                       APPLICATION & BEHAVIOR LAYER                       |
|   - Behaviors State Machine (Avoid, Line, Manual, Disco, High-Five)      |
|   - Pet Brain Dynamic Mood (Affection, Stress, Energy, Personas)         |
|   - Serial JSON Interactive Console (115200 Baud)                        |
|   - Web Server UI & SoftAP (192.168.4.1)                                 |
+--------------------------------------------------------------------------+
                                    │
                                    ▼
+--------------------------------------------------------------------------+
|                         SERVICE & DRIVER LAYER                           |
|   - Emotion GFX: Double-buffered Sprite Render (ST7789 SPI DMA)          |
|   - Audio Player: I2S Class-D DAC (MAX98357A) - SFX & Voice Feedback    |
|   - Voice Control & Acoustic Monitor: I2S Mic (INMP441) - RMS Energy     |
|   - Motion Control & Calib: Deadband, Trim Balance, Drift (PCA9685 PWM)  |
|   - Sensory Engine: MPU6050 (6-DOF IMU), TCRT5000 IR, TTP223 Touch, HC-SR05 |
|   - Store: NVS Flash Preferences (Calib, Calibration Validity, Personas) |
+--------------------------------------------------------------------------+
                                    │
                                    ▼
+--------------------------------------------------------------------------+
|                      HARDWARE ABSTRACTION & OS (HAL)                     |
|   - ESP32-S3 Xtensa Dual-Core 240MHz (FreeRTOS Kernel, Core 0 & Core 1) |
|   - 16MB Quad-SPI Flash (Partition default_16MB.csv)                     |
|   - 8MB Octal PSRAM (OPI Mode, Bus 8-bit 120MHz)                         |
|   - ESP-IDF Drivers: I2C, SPI DMA, I2S Master, RMT, ADC1, HW Timer       |
+--------------------------------------------------------------------------+
```

### 1.2. Các Phiên Bản Môi Trường (Environments) trong `platformio.ini`

Dự án cấu hình 4 môi trường độc lập:

| Môi trường | Thư mục mã nguồn | Mục đích | Đặc tính kỹ thuật |
| :--- | :--- | :--- | :--- |
| `env:rody` (Default) | `src/` | Firmware xe tự hành Mochi AI | TFT ST7789, I2S Audio, Web SoftAP, PCA9685, IR Tachometer. Cổng UART (`CDC_ON_BOOT=0`). |
| `env:otto` | `src/` | Alias tương thích ngược với rody | Cấu hình giống hệt `env:rody`. |
| `env:rody-pet` | `firmware_pet/src/` | Firmware thú cưng để bàn AI Pet | Tích hợp IMU MPU6050, Touch Sensor GPIO 2, Âm học Acoustic, USB CDC Native (`CDC_ON_BOOT=1`). |
| `env:native` | `test/` | Kiểm thử Unit Test trên PC/Mac | Unity test runner, không cần phần cứng, build bằng GCC/Clang máy chủ. |

### 1.3. Bản Đồ Phân Vùng Bộ Nhớ 16MB Flash (`default_16MB.csv`)

```
0x000000 ┌──────────────────────────────────────────┐ (0 KB)
         │ Bootloader (32 KB)                       │
0x008000 ├──────────────────────────────────────────┤ (32 KB)
         │ Partition Table (4 KB)                   │
0x009000 ├──────────────────────────────────────────┤ (36 KB)
         │ NVS (Non-Volatile Storage) (20 KB)       │ -> Lưu calib motor, Wi-Fi config
0x00E000 ├──────────────────────────────────────────┤ (56 KB)
         │ OTA Data (8 KB)                          │
0x010000 ├──────────────────────────────────────────┤ (64 KB)
         │ App0 (OTA Primary App) (6.5 MB)          │ -> Chứa Firmware thực thi hiện tại
0x690000 ├──────────────────────────────────────────┤ (6.625 MB)
         │ App1 (OTA Secondary App) (6.5 MB)        │ -> Dự phòng nâng cấp Firmware không dây
0xD10000 ├──────────────────────────────────────────┤ (13.125 MB)
         │ SPIFFS / LittleFS (2.875 MB)             │ -> Lưu web UI, audio sample, asset hình
0x1000000└──────────────────────────────────────────┘ (16 MB)
```

### 1.4. Vùng Cấm & Quy Tắc An Toàn GPIO trên ESP32-S3 N16R8

> [!CAUTION]
> **VÙNG CẤM TUYỆT ĐỐI KHÔNG ĐƯỢC CHẠM VÀO:**
> Bo mạch sử dụng bản **N16R8 (16MB Flash + 8MB Octal PSRAM)**. Việc cấu hình nhầm chân GPIO vào vùng này sẽ làm **crash bộ nhớ ngay khi boot** hoặc cháy chip:
> 1. **GPIO 26 đến GPIO 37**: Dành riêng cho giao tiếp Octal PSRAM và SPI Flash tốc độ cao. Tuyệt đối không khai báo làm INPUT hay OUTPUT.
> 2. **GPIO 19 và GPIO 20**: Chân USB D- và D+ phần cứng của cổng USB-OTG Native.
> 3. **GPIO 43 và GPIO 44**: Cổng UART0 Tx/Rx mặc định nối với chip nạp CH340/CP2102.
> 4. **Strapping Pins**:
>    - **GPIO 0**: Giữ LOW khi khởi động để vào chế độ Download Bootloader.
>    - **GPIO 3, 45, 46**: Chân thiết lập điện áp và boot ROM, không treo điện trở kéo ngoài không chuẩn.

Bảng phân bổ chân chuẩn xác tại [`include/pins.h`](file:///Volumes/Builder/Arduino/OtooRobot/include/pins.h):

| Chức năng | Chân ESP32-S3 | Giao thức | Ghi chú an toàn |
| :--- | :---: | :---: | :--- |
| **I2C Bus** (Chung PCA9685 & IMU) | **GPIO 8 (SDA), GPIO 9 (SCL)** | I2C (400kHz) | Cần trở kéo pull-up 4.7kΩ (trên bo PCA đã có sẵn) |
| **SPI TFT 1.54" ST7789** | **SCLK: 42, MOSI: 41, DC: 40, RST: 39, CS: 38, BLK: 21** | SPI DMA | Tần số SPI 40MHz - 80MHz |
| **I2S Microphone INMP441** | **SCK: 4, WS: 5, SD: 6** | I2S Rx | Chân L/R của Mic bắt buộc nối GND |
| **I2S Amply Loa MAX98357A** | **DIN: 7, LRC: 15, BCLK: 16** | I2S Tx | Ngõ ra loa SPK- cấm nối chung GND |
| **Cảm biến Line / Tachometer** | **GPIO 10 (Trái), GPIO 11 (Phải)** | GPIO IRQ | Cảm ứng ngắt cạnh xuống/lên |
| **Cảm biến Chạm Vuốt Ve** | **GPIO 2** | Digital / Touch | TTP223 hoặc cảm ứng điện dung tích hợp |
| **Cảm biến Siêu âm HC-SR05** | **GPIO 12 (Trig), GPIO 13 (Echo)** | Pulse | Echo 5V **phải** qua chia áp xuống 3.3V |
| **Đo điện áp pin ADC** | **GPIO 1** | ADC1_CH0 | Qua mạch chia áp tỉ lệ (phần mềm có bypass) |
| **Onboard RGB LED** | **GPIO 48** | WS2812B RMT | Đèn tín hiệu trạng thái hệ thống |

---

## 2. Cài Đặt Môi Trường & Toolchain PlatformIO

### 2.1. Yêu Cầu Hệ Thống

- **Hệ điều hành**: macOS (Sonoma/Sequoia), Linux (Ubuntu 22.04+ / Debian), Windows 10/11 (64-bit).
- **Phần mềm bắt buộc**:
  - [Visual Studio Code](https://code.visualstudio.com/) bản mới nhất.
  - Extension **PlatformIO IDE** (ID: `platformio.platformio-ide`).
  - **Python 3.9+** (kèm `pip` và `virtualenv`).
  - **Git** để clone và quản lý phiên bản.

### 2.2. Cài Đặt PlatformIO CLI

Nếu bạn thích làm việc qua dòng lệnh (Terminal):
```bash
# Cài đặt PlatformIO Core qua pip
python3 -m pip install -U platformio

# Kiểm tra phiên bản cài đặt thành công
pio --version
# Kết quả mong đợi: PlatformIO Core, version 6.x.x
```

### 2.3. Cài Đặt Trình Điều Khiển (USB Drivers)

Bo ESP32-S3 DevKitC-1 thường có **2 cổng Type-C**:
1. **Cổng COM / UART** (Qua chip cầu nạp CH340, CP2102, hoặc CH9102):
   - **macOS**: Thường nhận diện tự động dạng `/dev/cu.wchusbserial*` hoặc `/dev/cu.usbserial-*`. Nếu không nhận, cài [WCH CH34x Driver cho macOS](http://www.wch-ic.com/downloads/CH341SER_MAC_ZIP.html).
   - **Windows**: Cài đặt [CH341SER.EXE](http://www.wch-ic.com/downloads/CH341SER_EXE.html) hoặc [CP210x Universal Windows Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers). Nhận cổng dạng `COM3`, `COM5`,...
   - **Linux**: Kernel đã tích hợp driver sẵn. Cần cấp quyền truy cập cổng Serial cho user:
     ```bash
     sudo usermod -a -G dialout $USER
     # Khởi động lại hoặc đăng xuất rồi đăng nhập lại để cập nhật group
     ```

2. **Cổng USB Native (OTG)**:
   - Kết nối trực tiếp vào phần cứng USB Controller của ESP32-S3 (nhận dạng USB CDC / JTAG).
   - Tên cổng hiển thị: `/dev/cu.usbmodem*` (macOS), `/dev/ttyACM0` (Linux).

### 2.4. Lưu Ý Quan Trọng Cho macOS (Lỗi Xcode CommandLineTools)

Khi chạy test hoặc build native trên macOS, bạn có thể gặp lỗi:
```
xcrun: error: missing DEVELOPER_DIR path: /Applications/Xcode.app/Contents/Developer
```
**Khắc phục**: Xuất biến môi trường trỏ về Command Line Tools độc lập:
```bash
export DEVELOPER_DIR=/Library/Developer/CommandLineTools
```
*(Bạn có thể thêm dòng này vào file `~/.zshrc` để không phải nhập lại).*

---

## 3. Quy Trình Biên Dịch & Nạp Firmware

### 3.1. Các Lệnh Biên Dịch Cơ Bản

Mở thư mục gốc dự án `/Volumes/Builder/Arduino/OtooRobot`:

```bash
# 1. Biên dịch Firmware bản Xe Tự Hành (Mochi AI):
pio run -e rody

# 2. Biên dịch Firmware bản Thú Cưng Cảm Xúc (Pet Edition):
pio run -e rody-pet

# 3. Dọn dẹp bản build cũ khi đổi cấu hình cờ hoặc thư viện:
pio run -t clean
```

### 3.2. Cấu Hình Cổng Nạp & Nạp Firmware

Xác định cổng USB đang cắm bằng lệnh:
```bash
pio device list
```

Thiết lập cổng nạp tương ứng:
```bash
# Trên macOS:
export RODY_PORT=/dev/cu.wchusbserial110   # hoặc cổng usbmodem

# Trên Linux:
export RODY_PORT=/dev/ttyUSB0             # hoặc /dev/ttyACM0

# Trên Windows (PowerShell):
$env:RODY_PORT="COM5"
```

Nạp firmware vào vi điều khiển:
```bash
# Nạp bản xe tự hành:
pio run -e rody -t upload --upload-port $RODY_PORT

# Nạp bản thú cưng AI Pet:
pio run -e rody-pet -t upload --upload-port $RODY_PORT
```

### 3.3. Chế Độ Nạp Cứu Hộ (Manual Download Boot Mode)

Nếu nạp code bị báo lỗi `"A fatal error occurred: Failed to connect to ESP32-S3: No serial data received"`:
1. Nhấn và **giữ nút BOOT** (GPIO 0).
2. Nhấn và nhả **nút RST (Reset)** một lần.
3. **Nhả nút BOOT**.
4. Chạy lại lệnh nạp `pio run -e rody -t upload --upload-port $RODY_PORT`. Khi nạp xong 100%, nhấn nút **RST** một lần để robot khởi động vào chương trình mới.

### 3.4. Mở Cổng Giám Sát Serial Monitor

```bash
# Mở monitor trực tiếp bằng PlatformIO (115200 baud, tự decode exception)
pio device monitor -e rody --port $RODY_PORT
```
> [!TIP]
> Phím tắt thoát khỏi PlatformIO Monitor: **`Ctrl + ]`** hoặc **`Ctrl + C`**.

---

## 4. Kiểm Thử Tự Động: Unit Test & HIL Test

Hệ sinh thái Rody S3 áp dụng nguyên tắc kiểm thử nghiêm ngặt theo **Test-Driven Development (TDD)** và **Hardware-in-the-Loop (HIL)**.

### 4.1. Chạy Native Unit Test Trên Máy Tính (Không Cần Phần Cứng)

Bộ Unit Test kiểm thử thuật toán điều khiển, xử lý bộ đệm âm thanh, máy trạng thái cảm xúc và parser lệnh mà không cần cắm bo mạch:

```bash
# Chạy toàn bộ 17 test cases của bản xe tự hành & firmware:
DEVELOPER_DIR=/Library/Developer/CommandLineTools pio test -e native
```
Danh sách các bài test được tự động thẩm định:
- `test_deadband`: Kiểm tra thuật toán phát hiện dải chết servo đối xứng.
- `test_rpm` & `test_monotonic`: Kiểm tra hàm chuyển đổi tốc độ đơn điệu.
- `test_battery_bypass_when_disconnected`: Kiểm tra cơ chế tự động bypass khi chạy nguồn USB.
- `test_emotion_name_parser`: Kiểm tra nhận diện 12 trạng thái mắt Mochi.
- `test_voice_command_vietnamese_aliases`: Kiểm tra bộ phân tích từ khóa tiếng Việt (*"tiến lên"*, *"lùi lại"*,...).
- `test_audio_level_calculation`: Kiểm tra công thức đo năng lượng âm thanh RMS.
- `test_display_type_validation_and_names`: Kiểm tra an toàn bộ đệm hiển thị ST7789.

```bash
# Chạy toàn bộ 11 test cases của bản Thú Cưng (Pet Brain):
DEVELOPER_DIR=/Library/Developer/CommandLineTools pio test -d firmware_pet -e native
```
Kiểm tra phản xạ rơi tự do (`freefall`), lật ngửa bụng (`belly_up`), vuốt ve (`petting`), cù lét (`tickle`), và bay lượn máy bay (`airplane`).

---

### 4.2. Chạy Hardware-in-the-Loop (HIL) Tests

Các bài test giao tiếp trực tiếp với robot qua cổng Serial bằng Python `pytest`. Tuân thủ các **Safety Gates** trong [`AGENTS.md`](file:///Volumes/Builder/Arduino/OtooRobot/AGENTS.md):

```
       [G0: Đã đo dây an toàn M1-M8]
                    │
                    ▼
     [Test Bench: pytest tools/hil -m bench]
                    │
                    ▼
       [G1: Kê bánh xe khỏi mặt bàn]
                    │
                    ▼
    [Test Lifted: pytest tools/hil -m lifted]
                    │
                    ▼
   [G2: Robot đặt trên sàn phẳng dán keo]
                    │
                    ▼
     [Test Floor: pytest tools/hil -m floor -s]
```

```bash
# Cài đặt thư viện test Python
pip install pytest pyserial

# 1. Chạy test tĩnh trên bàn làm việc (Bench Test - Không quay bánh)
pytest tools/hil -m bench -v

# 2. Chạy test nâng bánh (Lifted Test - Sau khi đã kê bánh xe)
pytest tools/hil -m lifted -v

# 3. Chạy test chạy sàn thực tế (Floor Test - Sau khi đặt trên vạch thẳng)
pytest tools/hil -m floor -v -s
```

---

## 5. Kỹ Thuật & Công Cụ Debug Firmware Chuyên Sâu

### 5.1. Khung Điều Khiển Serial JSON Interactive Console

Firmware Rody S3 tích hợp một giao diện điều khiển Console chuẩn JSON hai chiều qua UART/USB CDC với tốc độ **115200 Baud**.

#### Cú Pháp Lệnh & Ví Dụ:

| Mục đích | Lệnh gửi (`\n`) | JSON Phản Hồi Mong Đợi | Ý Nghĩa Kỹ Thuật |
| :--- | :--- | :--- | :--- |
| **Kiểm tra liveness** | `ping` | `{"cmd":"ping","ok":true,"fw":"0.2.0-rody"}` | Kiểm tra luồng CPU chính không bị treo |
| **Kiểm tra tài nguyên** | `info` | `{"cmd":"info","ok":true,"flash":16777216,"psram":8388608,"heap":298400}` | Kiểm tra dung lượng RAM, PSRAM, Flash thực |
| **Quét bus I2C** | `i2c` | `{"cmd":"i2c","ok":true,"devices":[64,104]}` | 64 = 0x40 (PCA9685), 104 = 0x68 (MPU6050) |
| **Đọc điện áp pin** | `bat` | `{"cmd":"bat","ok":true,"v":3.92,"raw":2450}` | Đọc ADC1 chân GPIO 1 |
| **Đo khoảng cách** | `us 3` | `{"cmd":"us","ok":true,"cm":[18.2,18.1,18.3]}` | 3 lần đo siêu âm chân GPIO 12/13 |
| **Đọc cảm biến bánh** | `tach 1000` | `{"cmd":"tach","ok":true,"l":16,"r":16,"ms":1000}` | Đếm xung encoder bánh xe trong 1 giây |
| **Thử nghiệm servo** | `drive 40 40 500` | `{"cmd":"drive","ok":true,"l":40.0,"r":40.0,"ms":500}` | Chạy tiến 40% lực trong 500 mili-giây |
| **Dừng khẩn cấp** | `stop` | `{"cmd":"stop","ok":true}` | Tắt hẳn xung PWM của PCA9685 |
| **Đổi mắt Mochi** | `face happy` | `{"cmd":"face","ok":true,"emotion":"happy"}` | Đổi hoạt ảnh màn hình TFT ST7789 |
| **Phát âm thanh** | `sfx boot` | `{"cmd":"sfx","ok":true,"played":"boot"}` | Phát mẫu âm thanh I2S ra loa MAX98357A |
| **Xem bảng Calib** | `cal show` | `{"cmd":"cal show","ok":true,"valid":true,"trim_l":1.02}` | Đọc thông số bù sai lệch từ NVS Flash |

---

### 5.2. Giải Mã Crash Dump Bằng ESP32 Exception Decoder

Khi xảy ra lỗi nghiêm trọng (Guru Meditation Error / Core Panic), ESP32-S3 sẽ in ra một đoạn Call Stack mã hex dạng:

```
Guru Meditation Error: Core 1 panic'ed (LoadProhibited). Exception was unhandled.
Core 1 register dump:
PC      : 0x42008abc  PS      : 0x00060030  A0      : 0x82009120  A1      : 0x3fca1fe0  
...
Backtrace: 0x42008abc:0x3fca1fe0 0x4200911d:0x3fca2010 0x42004218:0x3fca2040 0x4037ba52:0x3fca2060
```

#### Phương Pháp 1: Tự Động Decode Qua PlatformIO Monitor
Trong [`platformio.ini`](file:///Volumes/Builder/Arduino/OtooRobot/platformio.ini), bộ lọc đã được kích hoạt sẵn:
```ini
monitor_filters = esp32_exception_decoder, time
```
Khi mở Serial Monitor qua `pio device monitor`, PlatformIO sẽ tự động khớp file ELF `.pio/build/rody/firmware.elf` và in trực tiếp tên hàm, tên file và số dòng gây lỗi:
```
Backtrace:
0x42008abc: emotion_gfx::drawEye at src/emotion_gfx.cpp:142
0x4200911d: emotion_gfx::update at src/emotion_gfx.cpp:285
0x42004218: loop at src/main.cpp:107
```

#### Phương Pháp 2: Dùng `addr2line` Thủ Công
Nếu bạn có một địa chỉ PC hoặc Backtrace từ log cũ:
```bash
~/.platformio/packages/toolchain-xtensa-esp32s3/bin/xtensa-esp32s3-elf-addr2line -pfiaC -e .pio/build/rody/firmware.elf 0x42008abc
```
Lệnh sẽ trả về chính xác: `emotion_gfx::drawEye() at /Volumes/Builder/Arduino/OtooRobot/src/emotion_gfx.cpp:142`.

---

### 5.3. Giám Sát & Phòng Chống Tràn Bộ Nhớ (Heap & PSRAM)

ESP32-S3 có **320KB Internal SRAM** và **8MB Octal PSRAM**:

```cpp
// 1. Kiểm tra bộ nhớ SRAM nội tuyến
uint32_t freeSram = ESP.getFreeHeap();
uint32_t minEverSram = ESP.getMinFreeHeap(); // Điểm thấp nhất từng chạm tới

// 2. Kiểm tra bộ nhớ Octal PSRAM ngoài (Dùng cho Framebuffer màn hình)
uint32_t freePsram = ESP.getFreePsram();

Serial.printf("# [MEM] SRAM Free: %u bytes (Min: %u) | PSRAM Free: %u bytes\n", 
              freeSram, minEverSram, freePsram);
```

> [!WARNING]
> **Quy Tắc Quản Lý Màn Hình ST7789**: Màn hình màu 240x240 RGB565 cần `240 * 240 * 2 = 115,200 bytes` (~112.5 KB) cho một Framebuffer.
> Nếu tạo 2 bộ đệm Double-buffer trong SRAM nội thì SRAM sẽ cạn kiệt, gây crash ngay lập tức. Thư viện `LovyanGFX` trong Rody S3 được cấu hình tự động phân bổ sprite đệm vào **PSRAM** (`psram = true`).

---

### 5.4. Xử Lý Watchdog Timer (WDT) Trong FreeRTOS

ESP32-S3 có 2 bộ giám sát thời gian:
1. **Interrupt Watchdog Timer (IWDT)**: Kích hoạt khi một ngắt ISR chạy quá 300ms.
2. **Task Watchdog Timer (TWDT)**: Kích hoạt khi Task ưu tiên cao (như `loopTask`) chiếm dụng CPU liên tục quá 5 giây mà không trả quyền kiểm soát.

#### Cách Khắc Phục:
- Không bao giờ dùng `while(1)` hoặc vòng lặp chờ dài mà không có hàm nhả CPU.
- Luôn chèn `delay(1)` hoặc `vTaskDelay(1 / portTICK_PERIOD_MS)` trong các vòng lặp xử lý logic nặng để bộ lập lịch FreeRTOS chuyển quyền sang luồng IDLE:
```cpp
// ĐÚNG:
while (!deviceReady) {
  delay(5); // Nhả quyền điều khiển cho FreeRTOS scheduler & Watchdog
}

// SAI (Gây Watchdog Reset sau 5s):
while (!deviceReady) {
  // Không có delay
}
```

---

### 5.5. Cơ Chế Khôi Phục Treo Bus I2C (I2C Bus Lockup Recovery)

Khi vi điều khiển bị reset đột ngột đúng thời điểm module slave (PCA9685 hoặc MPU6050) đang kéo chân **SDA xuống mức LOW**, bus I2C sẽ bị treo cứng vĩnh viễn (SDA mãi ở mức 0V, ESP32 không thể gửi xung START).

Firmware tích hợp hàm tự phục hồi bus I2C (I2C Bus Clear) trước khi gọi `Wire.begin()`:

```cpp
void recoverI2CBus(int sdaPin, int sclPin) {
  pinMode(sdaPin, INPUT_PULLUP);
  pinMode(sclPin, OUTPUT_OPEN_DRAIN);
  digitalWrite(sclPin, HIGH);
  delayMicroseconds(10);

  // Nếu SDA bị kéo thấp bởi thiết bị ngoại vi
  if (digitalRead(sdaPin) == LOW) {
    // Phát 9 xung nhịp Clock trên chân SCL để ép Slave nhả đường SDA
    for (int i = 0; i < 9; i++) {
      digitalWrite(sclPin, LOW);
      delayMicroseconds(5);
      digitalWrite(sclPin, HIGH);
      delayMicroseconds(5);
      if (digitalRead(sdaPin) == HIGH) break;
    }
  }
  // Gửi tín hiệu STOP để hoàn tất reset bus
  pinMode(sdaPin, OUTPUT_OPEN_DRAIN);
  digitalWrite(sdaPin, LOW);
  delayMicroseconds(5);
  digitalWrite(sclPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(sdaPin, HIGH);
  delayMicroseconds(5);
}
```

---

## 6. Sổ Tay Chẩn Đoán & Sửa Lỗi Firmware (Troubleshooting Matrix)

Bảng phân loại chi tiết các triệu chứng, nguyên nhân và cách khắc phục dứt điểm:

### 6.1. Nhóm Lỗi Nạp & Kết Nối (Upload / Bootloader)

| Hiện tượng | Nguyên nhân gốc rễ | Cách xử lý dứt điểm |
| :--- | :--- | :--- |
| `A fatal error occurred: Failed to connect to ESP32-S3: No serial data received` | Chip đang chạy firmware crash liên tục hoặc chưa kích hoạt Bootloader ROM. | Giữ nút **BOOT** $\to$ Bấm nhả nút **RST** $\to$ Nhả nút **BOOT**. Chạy lại lệnh upload. |
| `serial.serialutil.SerialException: [Errno 16] Resource busy` | Cổng Serial đang bị chiếm dụng bởi cửa sổ Serial Monitor hoặc ứng dụng khác (Cura, Arduino IDE). | Đóng tất cả các tab Terminal đang chạy `pio device monitor`, tắt các ứng dụng 3D/Serial khác rồi nạp lại. |
| `Permission denied: '/dev/ttyUSB0'` (Linux) | Tài khoản người dùng hiện tại chưa thuộc nhóm quản lý cổng nối tiếp `dialout`. | Chạy: `sudo usermod -a -G dialout $USER` rồi đăng xuất và đăng nhập lại. |
| Nạp thành công nhưng mở Serial Monitor thấy hoàn toàn im lặng, không có chữ nào | Cấu hình cờ USB CDC không khớp với cổng vật lý đang cắm cáp. | - Cắm cổng **UART** (chip CH340): sửa cờ thành `-DARDUINO_USB_CDC_ON_BOOT=0`.<br>- Cắm cổng **Native USB**: sửa cờ thành `-DARDUINO_USB_CDC_ON_BOOT=1`. |

---

### 6.2. Nhóm Lỗi Khởi Động & Bộ Nhớ (Boot / PSRAM / Brownout)

| Hiện tượng | Nguyên nhân gốc rễ | Cách xử lý dứt điểm |
| :--- | :--- | :--- |
| Bootloop lặp vô hạn, log in: `E (xxx) psram: PSRAM ID read error: 0xffffffff` | Cấu hình bộ nhớ PSRAM sai chế độ (Quad thay vì Octal). Bo N16R8 bắt buộc chạy Octal SPI. | Kiểm tra [`platformio.ini`](file:///Volumes/Builder/Arduino/OtooRobot/platformio.ini) phải có:<br>`board_build.arduino.memory_type = qio_opi`<br>`board_build.psram_type = opi`<br>`board_build.extra_flags = -DBOARD_HAS_PSRAM` |
| Robot tự reset ngay khi 2 bánh xe bắt đầu chuyển động, log báo: `rst:0xc (BROWNOUT_RST)` | Sụt áp nguồn đột ngột khi Servo ăn dòng tải đỉnh (Peak Current có thể lên 1.5A). | 1. Tuyệt đối không lấy chân 3.3V hoặc 5V của ESP32 cấp cho Servo.<br>2. Cấp nguồn động cơ từ pin qua mạch tăng áp hoặc cọc riêng.<br>3. Hàn thêm tụ hóa $1000\mu\text{F}\ / 10\text{V}-16\text{V}$ vào 2 cực nguồn V+ và GND của mạch PCA9685. |
| Log báo `info.flash != 16777216` | Chọn nhầm board mặc định 8MB Flash trong PlatformIO. | Khai báo trong `platformio.ini`:<br>`board_upload.flash_size = 16MB`<br>`board_build.partitions = default_16MB.csv` |

---

### 6.3. Nhóm Lỗi Ngoại Vi & Giao Tiếp (Hardware Peripherals)

| Hiện tượng | Nguyên nhân gốc rễ | Cách xử lý dứt điểm |
| :--- | :--- | :--- |
| **Màn hình ST7789 sáng trắng hoặc tối đen** | 1. Chân BLK (Đèn nền) bị bỏ trống.<br>2. Nhầm giao thức I2C thay vì SPI.<br>3. Chân SPI chạm chập. | 1. Nối chân **BLK** lên 3.3V hoặc GPIO 21.<br>2. Màn hình ST7789 dùng chuẩn **SPI DMA**: SCLK (42), MOSI (41), DC (40), RST (39), CS (38).<br>3. Chạy lệnh console `face happy` để kiểm tra driver. |
| **Lệnh `i2c` không tìm thấy địa chỉ `0x40` của PCA9685** | 1. Đảo lộn chân SDA và SCL.<br>2. Cọc VCC của PCA9685 chưa được cấp 3.3V.<br>3. Chân OE bị kéo lên HIGH. | 1. Đo kiểm: GPIO 8 nối SDA, GPIO 9 nối SCL.<br>2. Chân OE trên bo PCA9685 phải bỏ trống (tự kéo LOW nội) hoặc nối GND.<br>3. Đo điện áp chân VCC của PCA9685 phải đủ 3.3V. |
| **Servo 360° tự quay từ từ dù tốc độ đặt bằng 0** | Điểm dừng (Deadband Center) của biến trở cơ bên trong servo bị lệch so với xung chuẩn 1500µs. | Chạy quy trình hiệu chuẩn qua console:<br>`cal deadband l`<br>`cal deadband r`<br>`cal save` |
| **Loa MAX98357A phát tiếng xè xè hoặc không có tiếng** | 1. Kênh cấp nguồn 5V bị nhiễu do chung với motor.<br>2. Cực âm loa SPK- bị đấu nhầm xuống GND chung. | 1. Amply MAX98357A là vi sai (Bridge-Tied Load), hai dây ra loa **SPK+ và SPK- tuyệt đối không nối GND**.<br>2. Kiểm tra chân I2S: DIN (GPIO 7), LRC (GPIO 15), BCLK (GPIO 16). |
| **Mic INMP441 không thu được tiếng (RMS luôn bằng 0)** | Chân lựa chọn kênh L/R đang bị hở lơ lửng (Floating) làm mất đồng bộ chu kỳ xung I2S. | Nối chắc chắn chân **L/R của INMP441 xuống GND** để chọn thu kênh Trái (Left Channel Mono). |
| **Cảm biến siêu âm trả về `us = 999` liên tục** | Mạch chuyển mức logic thiếu 5V ở phía HV hoặc chân Echo cắm lỏng. | Đo chân HV của mạch chuyển mức phải đủ 5V, LV đủ 3.3V. Chân Trig (GPIO 12), Echo (GPIO 13). |
| **Đĩa mã hóa Tachometer đếm xung nhảy vọt bất thường** | Cảm biến hồng ngoại bị nhiễu ánh sáng đèn phòng hoặc chưa chỉnh cữ biến trở so sánh LM393. | Vặn chiết áp nhỏ trên module TCRT5000 sao cho đèn LED DO chỉ sáng khi gặp vạch phản xạ và tắt khi gặp vạch đen mờ. Che chắn ánh sáng mặt trời chiếu trực tiếp. |

---

## 7. Tiêu Chuẩn Lập Trình & Best Practices Cho ESP32-S3

Khi phát triển thêm tính năng mới cho firmware, hãy tuân thủ 5 nguyên tắc vàng sau:

### 7.1. Không Dùng `delay()` Dài – Dùng State Machine & `millis()`
```cpp
// Chuẩn thiết kế Non-blocking cho luồng chính:
void updateBlinkLed() {
  static uint32_t lastToggle = 0;
  static bool ledState = false;
  
  if (millis() - lastToggle >= 500) { // 500ms một lần
    lastToggle = millis();
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}
```

### 7.2. Tận Dụng Hai Nhân Xử Lý (Dual-Core FreeRTOS)
- **Core 0**: Dành riêng cho các tác vụ truyền thông, Wi-Fi SoftAP, Web Server, giải mã Audio I2S.
- **Core 1**: Dành riêng cho vòng lặp điều khiển thời gian thực (PID motor, đọc IMU, cảm biến IR, vẽ màn hình ST7789).

```cpp
// Tạo task chạy chuyên biệt trên Core 0
xTaskCreatePinnedToCore(
    audioTaskFunction,   // Hàm thực thi task
    "AudioTask",         // Tên task
    4096,                // Stack size (bytes)
    NULL,                // Tham số
    2,                   // Độ ưu tiên (Priority)
    NULL,                // Task handle
    0                    // Pin vào Core 0!
);
```

### 7.3. Ghi Dữ Liệu NVS Thông Minh (Chống Hỏng Bộ Nhớ Flash)
Bộ nhớ Flash có giới hạn chu kỳ ghi (~100.000 lần). Tuyệt đối không gọi `Preferences.putBytes()` hay ghi flash liên tục trong hàm `loop()`:
- Chỉ ghi khi giá trị thực sự thay đổi.
- Áp dụng cơ chế gom cụm (Debounce save) sau khi hiệu chuẩn xong mới bấm lệnh `cal save`.

---

## 8. Danh Mục Liên Kết Tài Liệu Hữu Ích

- 📖 [Sổ Tay Lắp Ráp & Hướng Dẫn Sử Dụng Rody S3](file:///Volumes/Builder/Arduino/OtooRobot/docs/HUONG_DAN_SU_DUNG.md)
- 📐 [Tiêu Chuẩn Đấu Dây Chi Tiết v2.0 (WIRING.md)](file:///Volumes/Builder/Arduino/OtooRobot/docs/hardware/WIRING.md)
- 🐾 [Hướng Dẫn Đấu Dây Bản Pet Thú Cưng (WIRING_PET_S3.md)](file:///Volumes/Builder/Arduino/OtooRobot/docs/hardware/WIRING_PET_S3.md)
- 📺 [Hướng Dẫn Màn Hình TFT & Âm Thanh (WIRING_XIAOZHI_TFT.md)](file:///Volumes/Builder/Arduino/OtooRobot/docs/hardware/WIRING_XIAOZHI_TFT.md)
- 📋 [Quy Tắc An Toàn Dành Cho AI Agent & HIL Gates (AGENTS.md)](file:///Volumes/Builder/Arduino/OtooRobot/AGENTS.md)
