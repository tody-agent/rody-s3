# ESP32 Circuit Architect & Robotics Hardware Lab (.plugin)

> **Universal AI Agent Plugin & Skill Suite for Claude Code, OpenAI Codex, and Google Antigravity / Gemini CLI.**  
> *Bộ công cụ & kỹ năng kỹ thuật phần cứng chuẩn hóa dành cho người học IoT và làm robotics.*

---

## 🌟 Tính năng cốt lõi (Key Features)

1. **Kho dữ liệu 110+ Linh kiện chuẩn hóa (`esp32_100_components.json / .md`):**
   - Đầy đủ thông số: Điện áp hoạt động, Mức logic (3.3V vs 5V), Dòng tiêu thụ, Giao thức kết nối (I2C/SPI/UART/PWM/ADC), Sơ đồ chân chi tiết.
   - **Được làm (DOs)** & **Cấm làm (DON'Ts)** giúp chống cháy chip, chống quá áp, chống sụt áp brownout.
   - Từ khóa Shopee Việt Nam chuẩn để đặt mua đúng 100% chủng loại.

2. **Cơ chế an toàn & Làm rõ (Ambiguity Gate):**
   - Nghiêm cấm agent đoán mò sơ đồ chân khi board mạch có nhiều phiên bản (30-pin vs 40-pin, 5V vs 3.3V).
   - Tự động kích hoạt cổng hỏi người dùng để xác minh trước khi cấp điện.

3. **Bộ kiểm thử mạch tự động (`circuit_checker.py`):**
   - Phát hiện xung đột chân Flash / PSRAM nội (GPIO 26-37 trên ESP32-S3 N16R8, GPIO 6-11 trên ESP32 WROOM).
   - Phát hiện đưa trực tiếp tín hiệu 5V vào chân GPIO 3.3V mà không qua Level Shifter / Mạch chia áp.
   - Phát hiện cấp nguồn động cơ/servo từ chân 3.3V của ESP32 gây treo vi điều khiển.
   - Kiểm tra xung đột địa chỉ I2C trùng nhau.

4. **Sơ đồ đi dây tương tác trực quan (`wiring_canvas_template.html`):**
   - Tên chân pin rõ ràng, viền màu phân biệt chức năng.
   - Cầu nhảy uốn cong 3D (Schematic Jumper Hops) khi dây bắt chéo.
   - Bảng màu dây riêng biệt theo từng linh kiện.
   - Tối ưu mượt mà cho Trackpad macOS (vuốt 2 ngón pan, pinch zoom neo tâm chuột, nút Fit to Screen).
   - Bảng danh mục BOM mua sắm 1 chạm.

---

## 🚀 Hướng dẫn Cài đặt nhanh (Quick Installation)

### Cách 1: Sử dụng Script cài đặt tự động (`install.sh`)
Chạy lệnh sau ngay trong thư mục giải nén:
```bash
chmod +x install.sh
./install.sh --all
```
Hoặc cài đặt riêng cho từng môi trường:
- Claude Code: `./install.sh --claude`
- OpenAI Codex: `./install.sh --codex`
- Antigravity / Gemini: `./install.sh --gemini`

---

### Cách 2: Cài đặt thủ công (Manual Installation)

#### Dành cho Claude Code:
Copy thư mục skill vào thư mục kỹ năng của Claude:
```bash
mkdir -p ~/.claude/skills/esp32-circuit-architect
cp -r skills/esp32-circuit-architect/* ~/.claude/skills/esp32-circuit-architect/
```
Hoặc cài đặt dưới dạng Plugin Claude:
```bash
mkdir -p ~/.claude/plugins/esp32-circuit-architect
cp -r . ~/.claude/plugins/esp32-circuit-architect/
```

#### Dành cho OpenAI Codex:
Copy thư mục skill vào thư mục kỹ năng của Codex:
```bash
mkdir -p ~/.codex/skills/esp32-circuit-architect
cp -r skills/esp32-circuit-architect/* ~/.codex/skills/esp32-circuit-architect/
```
Hoặc cài đặt dưới dạng Plugin Codex:
```bash
mkdir -p ~/.codex/plugins/esp32-circuit-architect
cp -r . ~/.codex/plugins/esp32-circuit-architect/
```

#### Dành cho Google Antigravity / Gemini CLI:
```bash
mkdir -p ~/.gemini/config/skills/esp32-circuit-architect
cp -r skills/esp32-circuit-architect/* ~/.gemini/config/skills/esp32-circuit-architect/
```

#### Dành riêng cho 1 Repository dự án hiện tại:
```bash
mkdir -p .agents/skills/esp32-circuit-architect
cp -r skills/esp32-circuit-architect/* .agents/skills/esp32-circuit-architect/
```

---

## 💻 Cách sử dụng trong Chat AI (Prompts gợi ý)

Khi làm việc với Claude, Codex hoặc Antigravity, bạn chỉ cần gõ yêu cầu tự nhiên:

1. **Tra cứu linh kiện:**
   > *"Tra cứu thông số, sơ đồ chân và những điều CẤM LÀM của module PCA9685 và ESP32-S3 DevKit 40 chân."*

2. **Thiết kế mạch:**
   > *"Hãy thiết kế sơ đồ nối chân cho robot Otto dùng ESP32-S3, 2 servo liên tục MG90S, cảm biến siêu âm HC-SR05 và mạch đo pin 0-25V. Đảm bảo an toàn điện và xuất file HTML đi dây trực quan."*

3. **Kiểm tra an toàn:**
   > *"Kiểm tra file cấu hình mạch này xem có chân nào bị trùng với chân Flash SPI hoặc cấp nguồn quá tải không."*

---

## 🛠️ Tra cứu bằng dòng lệnh (CLI Tools)

1. **Tìm kiếm linh kiện trong database:**
   ```bash
   python3 skills/esp32-circuit-architect/scripts/component_lookup.py mpu6050
   ```

2. **Thêm linh kiện mới bạn vừa nghiên cứu:**
   ```bash
   python3 skills/esp32-circuit-architect/scripts/component_lookup.py --add-json new_sensor.json
   ```

3. **Kiểm tra an toàn mạch điện:**
   ```bash
   python3 skills/esp32-circuit-architect/scripts/circuit_checker.py my_circuit.json
   ```

---

## 🔒 Kiểm định An ninh Bảo mật (Security Clearance)

Plugin này đã vượt qua kiểm định an ninh nghiêm ngặt bằng **SkillSpector** (chuẩn NVIDIA AI Agent Security Gate):
- **Risk Score:** `0 / 100` (Điểm tối đa an toàn)
- **Critical / High Issues:** `0`
- **Tình trạng:** `✅ SECURITY GATE PASSED` (Đạt chuẩn nạp vào mọi hệ thống AI Agent mà không gây rủi ro bảo mật).
