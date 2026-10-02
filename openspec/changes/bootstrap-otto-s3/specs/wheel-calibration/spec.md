## ADDED Requirements

### Requirement: Deadband
Firmware SHALL tìm dải xung đứng yên của từng bánh.

#### Scenario: Deadband hợp lệ
- **WHEN** gửi `cal deadband L` (bánh kê lên)
- **THEN** 1350 ≤ lo ≤ hi ≤ 1650 và hi − lo < 150

### Requirement: Cân bằng tốc độ
Với cùng speed%, hai bánh SHALL quay cùng rpm.

#### Scenario: Đo sau hiệu chỉnh
- **WHEN** gửi `meas 30`, `meas 60`, `meas 90`
- **THEN** mỗi lần err_pct ≤ 5

### Requirement: Chạy thẳng trên sàn
Robot SHALL lệch ngang ≤ 2cm trên mỗi 100cm ở speed 60.

#### Scenario: Lặp trim
- **WHEN** chạy `drive 60 60 <ms>` dọc băng keo, người đo d và D, gửi `cal drift d D`
- **THEN** sau tối đa 3 vòng lặp, |d| ≤ 2cm/100cm và `cal save` thành công

### Requirement: Lưu bền
Dữ liệu hiệu chỉnh SHALL còn nguyên sau khi rút nguồn.

#### Scenario: Reboot
- **WHEN** rút nguồn rồi cắm lại
- **THEN** `info` cho cal_valid = true và `cal show` trùng giá trị trước đó
