#pragma once
#include <stdint.h>

namespace pet_emotions {

enum class PetMood {
  IDLE,             // Bình thường, chớp mắt ngẫu nhiên
  PURRING,          // Thỏa mãn, tim bay, mắt cười trăng khuyết (khi vuốt ve)
  HURT,             // Đau đớn, khóc, ngấn lệ rơi (khi té ngã)
  BELLY_UP,         // Lật ngửa bụng, mắt trợn to chớp đỏ cầu cứu
  SHAKEN_DIZZY,     // Hoa mắt xoắn ốc (khi bị lắc lắc)
  ANGRY,            // Tức giận, mắt xếch tam giác đỏ (khi bị làm phiền)
  STARTLED,         // Giật mình hoảng hốt, mắt mở to (khi bị gõ mạnh)
  ANNOYED,          // Khó chịu, nhắm tịt mắt nhăn mày (khi quá ồn)
  SLEEPY,           // Buồn ngủ, lim dim, chữ Zzz bay
  DISCO,            // Nhảy theo nhịp: Mắt kính râm disco nhấp nháy đèn nhảy beat
  SNEEZE,           // Hắt xì hơi: Mắt nhắm tịt hít sâu rồi bùng nổ Achoo!
  AIRPLANE,         // Bay lượn: Kính phi công cổ điển lướt mây đón gió
  TICKLE,           // Bị cù lét: Mắt nhắm tít > < má ửng hồng cười ngặt nghẽo
  HIGH_FIVE,        // Đập tay phản xạ: Bàn tay mở mời gọi hoặc pháo hoa ăn mừng
  NUDGE,            // Húc nịnh đòi xoa đầu: Mắt cún long lanh nũng nịu
  COOL_GLASSES      // Kính đen ngầu Thug Life khi hoàn thành thử thách
};

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

} // namespace pet_emotions
