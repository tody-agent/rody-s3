## ADDED Requirements

### Requirement: Cấu hình N16R8
Firmware SHALL build cho ESP32-S3 với flash 16MB QIO và PSRAM 8MB OPI.

#### Scenario: Kiểm tra bộ nhớ sau khi nạp
- **WHEN** gửi lệnh `info`
- **THEN** flash = 16777216 và psram > 7000000

### Requirement: Test logic trên PC
Thư viện otto_calib MUST không phụ thuộc Arduino và SHALL có test Unity chạy bằng `pio test -e native`.

#### Scenario: Test native
- **WHEN** agent chạy `pio test -e native`
- **THEN** mọi test PASS mà không cần board
