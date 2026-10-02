# Otto S3 – Wiring Standard v1.0

## 1. BOM
| # | Linh kiện | SL | Ghi chú |
|---|-----------|----|---------|
| 1 | ESP32-S3 N16R8 DevKit 40 pin, 2 USB-C | 1 | |
| 2 | PCA9685 16 kênh | 1 | địa chỉ mặc định 0x40, KHÔNG hàn jumper địa chỉ |
| 3 | MG90S 360° | 2 | |
| 4 | HC-SR05 | 1 | chân OUT bỏ trống |
| 5 | Module IR (DO/AO) | 2 | chỉ dùng DO |
| 6 | Module chuyển mức logic 4 kênh 3.3V↔5V | 1 | |
| 7 | Module đo áp 0–25V | 1 | hệ số chia 5 |
| 8 | TP4056 có bảo vệ (cổng B+/B−/OUT+/OUT−) | 1 | |
| 9 | Boost 5V ≥2A, chỉnh được áp | 1 | |
| 10 | Diode Schottky 1N5819 (hoặc SS14/SS34) | 1 | OR nguồn ESP32 |
| 11 | Tụ hóa 470–1000µF ≥10V | 1 | nếu PCA9685 chưa có sẵn tụ |
| 12 | Công tắc gạt | 1 | |
| 13 | Buzzer active/passive 3.3V (tùy chọn) | 1 | |
| 14 | Pin Li-ion 3.7V 700mAh | 1 | bỏ nếu phồng hoặc <2.5V khi chưa sạc |

## 2. Quy ước màu dây
ĐỎ = 5V servo (SERVO_5V) · CAM = 5V logic (LOGIC_5V) · TRẮNG = 3V3 · ĐEN = GND ·
VÀNG = I2C · XANH LÁ = tín hiệu cảm biến · XANH DƯƠNG = Trig/Echo.

## 3. Cây nguồn
```
 Pin 3.7V ──B+/B−──► TP4056 ──OUT+──► [CÔNG TẮC] ──► VBAT_SW ──┬──► Module đo áp (VCC/GND)
                         ▲                                    │
                     USB sạc                                  └──► Boost IN+ / IN−
                                                                     │
                                         Boost OUT+ (5.1V) ──┬───────┴──► SERVO_5V ──► PCA9685 V+ (cọc vít)
                                                             │                         + tụ 470–1000µF
                                                             └──►|── 1N5819 ──► LOGIC_5V = chân 5V của ESP32
                                                                (vạch diode về phía ESP32)
 ESP32 3V3 ──► 3V3 rail: PCA9685 VCC, module IR VCC, level shifter LV
 LOGIC_5V  ──► HC-SR05 VCC, level shifter HV
 TẤT CẢ GND NỐI CHUNG (sao về chân GND của boost)
```
Diode chặn dòng đi từ USB ngược sang boost và ngược lại, nên được phép cắm USB khi
công tắc đang bật. Servo KHÔNG BAO GIỜ lấy nguồn từ USB.

## 4. Bảng chân & Sơ đồ vật lý ESP32-S3 (40 pin)

### 4.1 Bố trí chân thực tế trên bo mạch (Nhìn từ mặt trước, 2 cổng USB quay xuống)
```
                      +-------------------+
                      |     [ ANTENNA ]   |
                      |   ESP32-S3 N16R8  |
                      |    (WROOM-1/2)    |
                      +-------------------+
                      |                   |
 (3.3V Bus)   [ 1] 3V3| *               * |GND [21] (GND Bus)
 (EN / Reset) [ 2]  EN| *               * |TX  [22] (GPIO43 - UART0)
 (US_TRIG)    [ 3] IO4| *               * |RX  [23] (GPIO44 - UART0)
 (US_ECHO)    [ 4] IO5| *               * |IO1 [24] (BAT_ADC - Module đo pin S)
 (IR_L Tach)  [ 5] IO6| *               * |IO2 [25] (ADC1 / Touch)
 (IR_R Tach)  [ 6] IO7| *               * |IO42[26] (JTAG MTMS)
              [ 7]IO15| *               * |IO41[27] (JTAG MTDI)
              [ 8]IO16| *               * |IO40[28] (JTAG MTDO)
              [ 9]IO17| *               * |IO39[29] (JTAG MTCK)
              [10]IO18| *               * |IO38[30] (RGB LED / FSPI)
 (I2C SDA)    [11] IO8| *               * |IO37[31] (CẤM: PSRAM Octal)
 (CẤM DÙNG)   [12] IO3| *               * |IO36[32] (CẤM: PSRAM Octal)
 (CẤM DÙNG)   [13]IO46| *               * |IO35[33] (CẤM: PSRAM Octal)
 (I2C SCL)    [14] IO9| *               * |IO0 [34] (Nút BOOT)
 (Buzzer +)   [15]IO10| *               * |IO45[35] (CẤM: Strapping)
              [16]IO11| *               * |IO48[36] (RGB LED trên board)
              [17]IO12| *               * |IO47[37] (SPICLK)
              [18]IO13| *               * |IO21[38] (RTC GPIO)
 (LOGIC_5V)   [19]  5V| *               * |IO20[39] (CẤM: USB D+)
 (GND Bus)    [20] GND| *               * |IO19[40] (CẤM: USB D-)
                      |   [BOOT]   [RST]  |
                      |   [USB]    [UART] |
                      +-------------------+
```

### 4.2 Bảng kết nối chi tiết giữa ESP32-S3 và các linh kiện
| Vị trí Pin ESP32 | Chân Board | Hướng | Nối tới linh kiện | Chân linh kiện | Màu dây |
|---|---|---|---|---|---|
| **Hàng Trái, Pin 1** | **3V3** | OUT | PCA9685, Level Shifter, 2x IR | VCC (logic) | Trắng |
| **Hàng Trái, Pin 3** | **IO4** | OUT | Level Shifter | LV1 (-> HV1 -> HC-SR05 Trig) | Xanh dương |
| **Hàng Trái, Pin 4** | **IO5** | IN | Level Shifter | LV2 (<- HV2 <- HC-SR05 Echo) | Xanh dương |
| **Hàng Trái, Pin 5** | **IO6** | IN (ngắt) | Module IR Trái | DO | Xanh lá |
| **Hàng Trái, Pin 6** | **IO7** | IN (ngắt) | Module IR Phải | DO | Xanh lá |
| **Hàng Trái, Pin 11** | **IO8** | I2C SDA | PCA9685 | SDA | Vàng |
| **Hàng Trái, Pin 14** | **IO9** | I2C SCL | PCA9685 | SCL | Vàng |
| **Hàng Trái, Pin 15** | **IO10**| OUT | Buzzer 3.3V (tùy chọn) | Cực dương (+) | Tím / Cam |
| **Hàng Trái, Pin 19** | **5V** | IN | Diode 1N5819 (Cathode vạch trắng) | Chân có vạch (<- Anode <- Boost 5.1V) | Cam |
| **Hàng Trái, Pin 20** | **GND**| POWER | Điểm GND chung (Star GND) | GND chung toàn hệ thống | Đen |
| **Hàng Phải, Pin 21** | **GND**| POWER | Điểm GND chung (Star GND) | GND chung toàn hệ thống | Đen |
| **Hàng Phải, Pin 24** | **IO1** | ADC1 | Module đo áp 0–25V | Chân tín hiệu S | Xanh lá |

### 4.3 Sơ đồ đấu nối chi tiết các module vệ tinh

#### A. PCA9685 (Mạch PWM Servo)
- **Cọc vít cấp nguồn động cơ (Terminal 2 chân màu xanh dương):**
  - Cọc `V+`: Dây ĐỎ từ Boost `OUT+` (5.1V)
  - Cọc `GND`: Dây ĐEN về mass chung
  - **Tụ hóa 470–1000µF (≥10V):** Cực dương (+) cắm vào `V+`, cực âm (-) có sọc trắng cắm vào `GND`.
- **Hàng 6 chân tín hiệu bên hông:**
  - `VCC` -> ESP32 `3V3` (chỉ nuôi chip PCA9685)
  - `GND` -> `GND` chung
  - `SDA` -> ESP32 `IO8`
  - `SCL` -> ESP32 `IO9`
  - `OE`  -> Bỏ trống
  - `V+`  -> Bỏ trống (đã cấp qua cọc vít)
- **Cắm 2 Servo MG90S 360°:**
  - **Kênh 0 (Servo Bánh Trái):** Dây Cam (PWM) ở trên, Đỏ (V+) ở giữa, Nâu (GND) ở dưới.
  - **Kênh 1 (Servo Bánh Phải):** Dây Cam (PWM) ở trên, Đỏ (V+) ở giữa, Nâu (GND) ở dưới.

#### B. Module Chuyển Mức Logic 4 Kênh (3.3V ↔ 5V) & HC-SR05
- **Bên Low Voltage (3.3V):**
  - `LV` -> ESP32 `3V3`
  - `GND` -> `GND` chung
  - `LV1` -> ESP32 `IO4` (Trig)
  - `LV2` -> ESP32 `IO5` (Echo)
- **Bên High Voltage (5V):**
  - `HV` -> Đường `5V` (chân 5V của ESP32 hoặc sau diode)
  - `GND` -> `GND` chung
  - `HV1` -> HC-SR05 `Trig`
  - `HV2` -> HC-SR05 `Echo`
- **HC-SR05:** `VCC` nối 5V, `GND` nối GND chung, `OUT` bỏ trống.

#### C. Module Đo Áp 0–25V (Cầu phân áp chia 5)
- **Cọc vít 2 chân (Đầu vào đo):**
  - Cọc `VCC`: Nối vào dây dương pin **SAU công tắc gạt** (`VBAT_SW`).
  - Cọc `GND`: Nối vào `GND` chung.
- **Hàng 3 chân Header (Đầu ra):**
  - Chân `S`: Nối vào ESP32 `IO1` (ADC1)
  - Chân `+`: Bỏ trống (NC)
  - Chân `-`: Nối vào `GND` chung

#### D. Hai Module Cảm Biến Hồng Ngoại IR (Tachometer & Dò Line)
- **Module IR Trái:** `VCC` -> 3V3, `GND` -> GND, `DO` -> ESP32 `IO6`, `AO` bỏ trống.
- **Module IR Phải:** `VCC` -> 3V3, `GND` -> GND, `DO` -> ESP32 `IO7`, `AO` bỏ trống.

#### E. Mạch Nguồn & Diode Schottky 1N5819
- Pin 3.7V -> TP4056 (`B+`/`B-`).
- TP4056 `OUT+` -> Chân vào Công tắc gạt.
- Sau Công tắc gạt (`VBAT_SW`) rẽ thành 2 nhánh:
  - Nhánh 1: Cấp vào Boost `IN+`.
  - Nhánh 2: Cấp vào cọc `VCC` của module đo áp.
- Boost `OUT+` (5.10V) rẽ thành 2 nhánh:
  - Nhánh A: Cấp vào cọc vít `V+` PCA9685 (`SERVO_5V`).
  - Nhánh B: Cấp vào chân **Anode** (phía trơn) của Diode 1N5819.
- Chân **Cathode** (phía có vạch trắng/bạc) của Diode 1N5819 -> Cấp vào chân `5V` của ESP32 (`LOGIC_5V`).
- Mọi dây `GND` gom về 1 điểm chung (Star Ground).

## 5. Chân cấm dùng
GPIO19/20 (USB), 26–32 (flash), 33–37 (PSRAM octal), 43/44 (UART0),
0/3/45/46 (strapping – chỉ dùng làm input sau boot).

## 6. Đồ gá cân chỉnh (bắt buộc cho Gate G1)
- In tools/tach_disc.svg (Ø30mm, 4 sọc đen / 4 trắng), dán vào mặt trong mỗi bánh.
- Gắn tạm mỗi module IR cách đĩa 3–8mm, vặn biến trở tới khi LED DO đổi trạng thái
  theo từng sọc khi quay bánh bằng tay.
- Kê thân robot sao cho 2 bánh quay tự do.

## 7. Checklist đo trước khi cấp nguồn (Gate G0)
| ID | Đo | Đạt |
|----|----|-----|
| M1 | Ω giữa SERVO_5V–GND, LOGIC_5V–GND, 3V3–GND (rút nguồn) | > 1kΩ, không chạm |
| M2 | Áp pin tại B+ | 3.0–4.2V |
| M3 | Boost OUT khi CHƯA nối tải (chỉnh biến trở) | 5.05–5.20V |
| M4 | LOGIC_5V sau diode (bật công tắc, chưa cắm USB) | 4.7–5.0V |
| M5 | 3V3 trên board | 3.25–3.35V |
| M6 | Chân S module đo áp | ≈ VBAT/5 (0.6–0.85V) |
| M7 | Phía LV của level shifter, kênh Echo, khi siêu âm hoạt động | ≤ 3.4V |
| M8 | Cực tính diode: vạch về phía ESP32 | đúng |
