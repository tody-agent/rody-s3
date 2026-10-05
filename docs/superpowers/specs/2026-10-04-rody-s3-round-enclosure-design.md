# 📐 Thiết Kế Chi Tiết Vỏ Robot Rody S3 Round Edition (Astro-Pod & Modular Pet)

> **Mã thiết kế:** `RODY-S3-ENC-V1`  
> **Ngày phê duyệt:** 04/10/2026  
> **Phiên bản:** `v1.0-spec`  
> **Màn hình:** 1.28" Round GC9A01 SPI (240x240)  
> **Kiến trúc:** Dạng quả cầu vát nghiêng $65^\circ$ (Astro-Pod) tích hợp ngàm gắn tai thú cưng nam châm tháo rời (Modular Mecha-Cat Ears).

---

## 1. Tổng Quan Hình Khối & Kích Thước Tổng Thể

Thiết kế kết hợp sự tinh tế của **Concept 1 (Astro-Pod)** và nét đáng yêu của **Concept 3 (Chibi-Mecha Cat)** thông qua giải pháp **thân vỏ tròn nguyên khối + ngàm gắn tai nam châm tháo rời (Modular Magnetic Ears)**:

- **Chiều cao tổng thể:** $78.0\text{ mm}$ (không tính tai) / $94.0\text{ mm}$ (khi gắn tai silicone).
- **Đường kính thân lớn nhất:** $\varnothing 72.0\text{ mm}$.
- **Đường kính chân đế tiếp xúc bàn:** $\varnothing 46.0\text{ mm}$.
- **Góc nghiêng mặt màn hình:** $65^\circ$ so với mặt phẳng ngang (tối ưu góc nhìn cho người ngồi làm việc cách $40\text{ cm} - 80\text{ cm}$).
- **Độ dày thành vỏ (Wall Thickness):** $2.0\text{ mm}$ (đồng đều, chống cong vênh khi in FDM hoặc đúc ép nhựa).

---

## 2. Bố Trí Khoang & Xếp Chồng Linh Kiện (Internal Packaging Architecture)

Vỏ được chia thành **3 phần lắp ráp chính** bằng khớp gài (snap-fit) kết hợp vít chìm M2:

```
          [ Top Touch Pocket & Magnetic Ear Slots ]
                      |
        +-------------+-------------+
        |                           |
  [ Front Bezel:              [ Main Body:
    - GC9A01 1.28" Display       - Vertical Spine Bracket for ESP32-S3
    - Front Acrylic 2.5D Lens    - Top Capacitive Touch Foil (GPIO 2)
    - INMP441 Mic Acoustic Port] - Rear USB-C Female Port Cutout
                                 - Reset / BOOT button actuator ]
                                    |
                       [ Bottom Acoustic Base:
                         - 28mm 4Ω Speaker in Sealed Chamber
                         - MAX98357A I2S Amplifier Board
                         - MPU6050 Accelerometer in Center Tray
                         - 360° Downward Acoustic Grille
                         - Circular RGB Halo Ring Light Guide
                         - Anti-vibration Silicone Ring ]
```

### 2.1. Cụm Mặt Trước (Front Bezel & Display Assembly)
- **Màn hình GC9A01:** Được giữ chặt bằng gờ định vị $\varnothing 38.2\text{ mm}$ và vòng đệm silicon $\varnothing 32.5\text{ mm}$ chống lọt sáng và chống bụi.
- **Kính bảo vệ:** Mặt kính mica/acrylic tròn $\varnothing 36.0\text{ mm}$ dày $1.5\text{ mm}$ bo cong 2.5D ở mép, tạo độ sâu và bảo vệ màn hình IPS khỏi va đập.
- **Lỗ Mic INMP441:** Đặt tại góc $6\text{ giờ}$ ngay dưới viền màn hình, góc nghiêng $45^\circ$ hướng xuống. Đường dẫn âm thanh (Acoustic Waveguide) đường kính $\varnothing 1.5\text{ mm}$ có màng lưới lọc bụi chống dội âm trực tiếp từ loa đáy.

### 2.2. Thân Chính & Khung Xương (Main Body & Vertical Spine)
- **Khung giữ ESP32-S3-DevKitC:** Khung xương dọc (Vertical Spine) trượt vào rãnh thân vỏ, cố định bo mạch bằng 2 vít M2x4mm. Bo mạch đặt thẳng đứng giúp tối ưu hóa luồng đối lưu nhiệt tự nhiên từ đáy lên đỉnh.
- **Vị trí Cảm Biến Chạm (GPIO 2):** Ngăn chứa (pocket) kích thước $18\text{ mm} \times 18\text{ mm} \times 0.8\text{ mm}$ sát trần đỉnh đầu. Dán lá đồng/nhôm cảm ứng điện dung trực tiếp vào mặt trong vỏ nhựa dày $1.5\text{ mm}$, nhận diện độ nhạy chạm cách $3\text{ mm} - 5\text{ mm}$.
- **Ngàm Tai Thú Cưng Nam Châm:** 2 hốc chìm $\varnothing 6.0\text{ mm} \times 2.5\text{ mm}$ chứa nam châm Neodymium N52 ở hai bên đỉnh đầu. Khi muốn biến thành mèo mecha, chỉ cần hít 2 tai silicone vào; khi muốn phong cách tối giản, tháo tai ra bề mặt vẫn liền lạc trơn tru.
- **Cổng Type-C:** Khoét lỗ chuẩn $10.0\text{ mm} \times 4.5\text{ mm}$ ở phía sau đáy thân, cho phép cắm mọi loại cáp sạc Type-C thông dụng mà không bị cấn viền.

### 2.3. Cụm Đáy Âm Học & Tản Nhiệt (Acoustic Chamber & Base)
- **Buồng âm kín (Sealed Acoustic Enclosure):** Loa $28\text{ mm}$ được dán đệm mút EVA và bắt vít vào khoang kín thể tích $\approx 22\text{ cm}^3$. Âm thanh đánh xuống mặt bàn (Down-firing) qua 8 khe rãnh hướng tâm góc $360^\circ$, tạo hiệu ứng cộng hưởng âm trầm tự nhiên.
- **Cảm biến MPU6050:** Bắt vít cố định vào sàn đế ngay trọng tâm hình học, định hướng trục Z vuông góc mặt bàn để thuật toán nhận diện gia tốc rơi và lắc rung đạt độ chính xác cao nhất.
- **Vòng Dẫn Sáng Halo Ring:** Vòng tròn nhựa mờ (Translucent Light Guide Ring) bao quanh chân đế, gom ánh sáng từ LED WS2812 onboard (hoặc dải LED đáy) tạo quầng sáng hắt gầm (underglow halo) đổi màu theo tâm trạng robot.
- **Đế cao su chống trượt:** Vòng đệm cao su silicone chữ O (O-ring $\varnothing 42\text{ mm} \times 2.5\text{ mm}$) triệt tiêu độ rung của loa khi phát âm lượng lớn, giữ robot đứng vững trên bàn kính/gỗ.

---

## 3. Quản Lý Khí Động Học & Tản Nhiệt (Thermal Management)

ESP32-S3 phát Wi-Fi liên tục và khuếch đại âm thanh Class-D có thể tỏa nhiệt $0.8\text{ W} - 1.5\text{ W}$:
- **Hiệu ứng ống khói (Chimney Convection):** Không khí mát đi vào từ các khe rãnh đáy $\varnothing 46\text{ mm}$, đối lưu dọc theo hai mặt phiến tản nhiệt của ESP32-S3 và thoát ra qua 2 khe thoát khí vi mô ẩn dưới ngàm tai ở đỉnh đầu.
- Nhiệt độ chip duy trì $\le 48^\circ\text{C}$ khi hoạt động liên tục ở nhiệt độ phòng $28^\circ\text{C}$.

---

## 4. Hướng Dẫn In 3D & Chế Tạo (Fabrication & 3D Printing)

| Chi Tiết | Vật Liệu Đề Xuất | Công Nghệ In / Gia Công | Cài Đặt Khuyên Dùng |
| :--- | :--- | :--- | :--- |
| **Thân Vỏ (Main Shell)** | PLA+ / PETG / ABS Matte | FDM hoặc SLA Resin | Layer $0.16\text{ mm}$, Infill 25% Gyroid, 3 perimeters |
| **Viền Mặt Trước (Bezel)** | Nhựa Resin cứng hoặc Nhôm CNC | SLA Resin 8K / CNC Anodized | Layer $0.05\text{ mm}$, sơn phủ Space Gray mờ |
| **Đáy Âm Thanh (Base)** | ABS / PETG | FDM | Infill 100% (đặc) để tăng khối lượng và chống cộng hưởng rè |
| **Vòng Dẫn Sáng (Light Guide)** | Nhựa Resin trong mờ (Frosted Clear) | SLA Resin | Phun mờ hoặc chà nhám mịn 1000 grit |
| **Đôi Tai Mèo (Modular Ears)** | Silicone đúc hoặc TPU 85A | Đúc khuôn silicone / In TPU mềm | Màu Pastel Mint hoặc Pastel Orange |

---

## 5. Bảng Vật Tư Lắp Ráp Cơ Khí (Mechanical BOM)

1. Vỏ in 3D (3 chi tiết: Thân chính, Bezel trước, Đáy loa).
2. Kính bảo vệ màn hình mica/acrylic tròn $\varnothing 36\text{ mm} \times 1.5\text{ mm}$ (1 cái).
3. Vít tự ren inox đầu chìm M2 $\times 5\text{ mm}$ (4 cái giữ nắp đáy, 2 cái giữ bo mạch).
4. Nam châm Neodymium N52 $\varnothing 6\text{ mm} \times 2\text{ mm}$ (4 viên: 2 viên trong thân, 2 viên trong tai).
5. Vòng đệm cao su silicone $\varnothing 42\text{ mm}$ chân đế (1 cái).
6. Băng keo xốp đệm kín loa EVA $1.0\text{ mm}$ (1 dải).
7. Miếng dán lá đồng cảm ứng điện dung $15\text{ mm} \times 15\text{ mm}$ có dây nối (1 cái).
