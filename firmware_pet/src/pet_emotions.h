#pragma once
#include <stdint.h>

namespace pet_emotions {

enum class PetMood {
  // 12 Biểu cảm Chuẩn Mochi (Classic)
  IDLE,             // 1. Bình thường, chớp mắt ngẫu nhiên
  HAPPY,            // 2. Vui sướng, mắt cười trăng khuyết nhún nhảy ^ _ ^, má hồng
  LISTENING,        // 3. Lắng nghe, mắt mở to + dải sóng âm equalizer ở đáy
  THINKING,         // 4. Suy nghĩ, mắt liếc lên góc phải + chấm quỹ đạo xoay tròn
  SPEAKING,         // 5. Đang nói, mắt nhìn thẳng + miệng mấp máy hoạt họa
  DRIVE_FWD,        // 6. Tiến tới, mắt nghiêng lao về trước + tia tốc độ
  DRIVE_REV,        // 7. Lùi lại, mắt cam thận trọng liếc góc sau
  TURN_LEFT,        // 8. Rẽ trái, mắt liếc trái
  TURN_RIGHT,       // 9. Rẽ phải, mắt liếc phải
  SHAKEN_DIZZY,     // 10. Chóng mặt xoắn ốc (DIZZY)
  OBSTACLE,         // 11. Đụng vật cản / va chạm, mắt chữ X _ X đỏ rực
  SLEEPY,           // 12. Buồn ngủ / ngủ say, mắt lim dim + chữ Zzz bay bổng

  // Phản xạ Thú cưng Độc quyền (Pet Edition)
  PURRING,          // Thỏa mãn, tim bay, mắt cười trăng khuyết (khi vuốt ve)
  HURT,             // Đau đớn, khóc, ngấn lệ rơi (khi té ngã)
  BELLY_UP,         // Lật ngửa bụng, mắt trợn to chớp đỏ cầu cứu
  ANGRY,            // Tức giận, mắt xếch tam giác đỏ (khi bị làm phiền)
  STARTLED,         // Giật mình hoảng hốt, mắt mở to (khi bị gõ mạnh)
  ANNOYED,          // Khó chịu, nhắm tịt mắt nhăn mày (khi quá ồn)
  DISCO,            // Nhảy theo nhịp: Mắt kính râm disco nhấp nháy đèn nhảy beat
  SNEEZE,           // Hắt xì hơi: Mắt nhắm tịt hít sâu rồi bùng nổ Achoo!
  AIRPLANE,         // Bay lượn: Kính phi công cổ điển lướt mây đón gió
  TICKLE,           // Bị cù lét: Mắt nhắm tít > < má ửng hồng cười ngặt nghẽo
  HIGH_FIVE,        // Đập tay phản xạ: Bàn tay mở mời gọi hoặc pháo hoa ăn mừng
  NUDGE,            // Húc nịnh đòi xoa đầu: Mắt cún long lanh nũng nịu
  COOL_GLASSES      // Kính đen ngầu Thug Life khi hoàn thành thử thách
};

// Bí danh thuận tiện cho DIZZY
constexpr PetMood DIZZY = PetMood::SHAKEN_DIZZY;

// Khởi tạo màn hình ST7789 và bộ đệm Sprite 60 FPS
bool init();

// Thiết lập tâm trạng biểu cảm hiện tại
void setMood(PetMood mood);

// Lấy tâm trạng hiện tại
PetMood getMood();

// Cập nhật hoạt họa 60 FPS (gọi liên tục trong loop)
void update();

// Tên chuỗi của tâm trạng
const char* getMoodStr(PetMood mood);

// Thiết lập tâm trạng theo tên chuỗi (hỗ trợ cả 12 cảm xúc bản cũ và các phản xạ bản pet)
bool setMoodByName(const char* name);

} // namespace pet_emotions
