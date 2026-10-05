# 🛠️ Rody S3 Round Enclosure Fabrication & Assembly Implementation Plan

> **Mục tiêu:** Xây dựng bộ file thiết kế cơ khí 3D tham số (OpenSCAD/CAD), kiểm tra dung sai lắp ráp thực tế cho các module điện tử (GC9A01, ESP32-S3, INMP441, MAX98357A, MPU6050) và đóng gói hướng dẫn thi công lắp ráp sản phẩm hoàn chỉnh.  
> **Kiến trúc:** Dạng Astro-Pod vát nghiêng $65^\circ$ tích hợp ngàm nam châm gắn tai thú cưng tháo rời.  
> **Tài liệu đặc tả liên quan:** [`docs/superpowers/specs/2026-10-04-rody-s3-round-enclosure-design.md`](file:///Volumes/Builder/Arduino/OtooRobot/docs/superpowers/specs/2026-10-04-rody-s3-round-enclosure-design.md)  
> **Ngày lập:** 04/10/2026  

---

## Danh Mục Các File Sẽ Tạo & Chỉnh Sửa

| Đường Dẫn File | Trách Nhiệm / Mục Đích |
| :--- | :--- |
| `firmware_muse/cad/rody_s3_astropod.scad` | Mô hình 3D tham số đầy đủ bằng OpenSCAD (Thân chính, Bezel trước, Đáy âm thanh, Tai nam châm) |
| `firmware_muse/cad/generate_stl.py` | Script tự động xuất STL và kiểm tra va chạm / kích thước dung sai hình học |
| `firmware_muse/docs/ASSEMBLY_GUIDE.md` | Hướng dẫn thi công, hàn dây, chiều dài dây dẫn và quy trình lắp ráp từng bước |
| `firmware_muse/tests/test_enclosure_tolerances.py` | Unit test kiểm tra kích thước dung sai và khoảng hở cơ khí |

---

## Chi Tiết Các Tác Vụ (Bite-Sized Tasks)

### Task 1: Xây Dựng Script Kiểm Tra Dung Sai Cơ Khí (TDD)
- **Mục tiêu:** Tạo bài test tự động xác thực các thông số kích thước cơ khí không bị va chạm hoặc thiếu khoảng hở.
- **Tập tin:** `firmware_muse/tests/test_enclosure_tolerances.py`
- **Các bước thực hiện:**
  1. Viết test kiểm tra kích thước khay màn hình GC9A01: đường kính trong phải $\ge 38.2\text{mm}$, viền kính $\ge 36.0\text{mm}$.
  2. Viết test kiểm tra khoang chứa bo mạch ESP32-S3: chiều rộng $\ge 26.5\text{mm}$, chiều cao $\ge 52.0\text{mm}$.
  3. Viết test kiểm tra thể tích buồng âm học loa $\ge 18\text{cm}^3$.
  4. Chạy test để xác nhận kiểm thử hoạt động.

### Task 2: Tạo Mô Hình 3D Tham Số OpenSCAD (`rody_s3_astropod.scad`)
- **Mục tiêu:** Lập trình mô hình 3D tham số chính xác từng milimét cho 4 chi tiết lắp ghép:
  - Part A: `front_bezel()` - Vòng viền giữ màn hình tròn GC9A01 và kính mica 2.5D, tích hợp lỗ mic INMP441 chéo $45^\circ$.
  - Part B: `main_shell()` - Thân cầu vát nghiêng $65^\circ$, rãnh trượt cho ESP32-S3, pocket cảm biến chạm và 2 hốc nam châm cho tai.
  - Part C: `acoustic_base()` - Buồng kín loa $28\text{mm}$, khay bắt vít MPU6050, 8 khe thoát âm 360°, rãnh O-ring chống trượt và vòng dẫn sáng Halo Ring.
  - Part D: `modular_ears()` - Cặp tai mèo mecha gắn nam châm N52 $\varnothing 6\text{mm}$.
- **Tập tin:** `firmware_muse/cad/rody_s3_astropod.scad`
- **Các bước thực hiện:**
  1. Viết code SCAD với đầy đủ biến tham số (variables table).
  2. Tạo các module riêng biệt có thể render độc lập (`render_part = "all" | "bezel" | "shell" | "base" | "ears"`).

### Task 3: Viết Hướng Dẫn Lắp Ráp & Đi Dây Thực Tế (`ASSEMBLY_GUIDE.md`)
- **Mục tiêu:** Cung cấp tài liệu chỉ dẫn người dùng cách chuẩn bị vật tư, cắt dây, hàn dây ngắn chống nhiễu, cố định mạch và hoàn thiện sản phẩm.
- **Tập tin:** `firmware_muse/docs/ASSEMBLY_GUIDE.md`
- **Các bước thực hiện:**
  1. Bảng chiều dài dây điện tối ưu (Wire Length Schedule) cho SPI, I2S, I2C và nguồn.
  2. Sơ đồ thứ tự lắp ráp 6 bước kèm lưu ý cách nhiệt và chống rè âm học.

---

## Tiêu Chí Hoàn Thành (Definition of Done)
1. File OpenSCAD biên dịch trơn tru, không có lỗi mesh (non-manifold).
2. Toàn bộ unit tests dung sai cơ khí và các test firmware hiện hữu (168 tests) đều chạy đạt 100%.
3. Hướng dẫn lắp ráp chi tiết, rõ ràng, dễ hiểu.
