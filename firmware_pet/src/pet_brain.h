#pragma once
#include <stdint.h>
#include "pet_emotions.h"

namespace pet_brain {

enum class PetPersona {
  CAT,       // Mèo lười, thích cuộn tròn, kêu rừ rừ
  PUPPY,     // Cún cưng, năng động, thích đập tay vẫy đuôi
  MECHA      // Robot chiến binh tương lai
};

struct PetStats {
  uint8_t affection;  // Mức độ gắn bó, quấn chủ (0 - 100)
  uint8_t stress;     // Mức độ căng thẳng, bực bội (0 - 100)
  uint8_t energy;     // Mức năng lượng (0 - 100)
  pet_emotions::PetMood currentMood;
  PetPersona persona;
};

// Khởi tạo bộ não và máy trạng thái cảm xúc thú cưng
void init();

// Đổi lốt tính cách (Persona)
void setPersona(PetPersona p);
PetPersona getPersona();

// Chu kỳ cập nhật bộ não (xử lý cảm biến -> chuyển đổi cảm xúc -> xuất phản xạ)
void update();

// Lấy thông số tâm trạng hiện tại
const PetStats& getStats();

// Giả lập kích hoạt sự kiện (phục vụ kiểm thử hoặc điều khiển từ xa)
void simulateEvent(const char* eventName);

} // namespace pet_brain
