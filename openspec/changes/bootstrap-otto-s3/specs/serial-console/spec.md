## ADDED Requirements

### Requirement: Lệnh dạng dòng, phản hồi JSON
Firmware SHALL trả về đúng một dòng JSON chứa "cmd" và "ok" cho mỗi lệnh trong design §2.

#### Scenario: Lệnh hợp lệ
- **WHEN** gửi `ping`
- **THEN** nhận `{"cmd":"ping","ok":true,"fw":"..."}` trong 500ms

#### Scenario: Lệnh sai
- **WHEN** gửi `abc`
- **THEN** nhận `{"cmd":"abc","ok":false,"err":"unknown"}`
