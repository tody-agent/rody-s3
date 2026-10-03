#pragma once
#include <stdint.h>

namespace pet_audio {

enum class PetSound {
  BOOT_HELLO,          // Âm thanh chào mừng khởi động
  PURR_CONTENT,        // Tiếng rừ rừ thỏa mãn khi được vuốt ve
  CRY_HURT,            // Tiếng kêu đau / thút thít khi bị ngã
  BELLY_UP_ALARM,      // Tiếng còi bíp bíp cầu cứu khi bị lật ngửa bụng
  DIZZY_CHIRP,         // Tiếng hoa mắt chóng mặt xoắn ốc
  GROWL_ANGRY,         // Tiếng gầm gừ cáu kỉnh / tức giận
  GASP_STARTLED,       // Tiếng giật mình thảng thốt khi bị gõ mạnh
  ANNOYED_SIGH,        // Tiếng thở dài khó chịu khi môi trường quá ồn
  DISCO_GROOVE,        // Nhạc nhảy funky 8-bit disco chiptune
  SNEEZE_BURST,        // Tiếng hít sâu và bùng nổ hắt xì hơi
  AIRPLANE_ENGINE,     // Tiếng động cơ cánh quạt máy bay lượn
  TICKLE_GIGGLE,       // Tiếng cười khúc khích khi bị cù lét
  VICTORY_FANFARE,     // Khúc khải hoàn đập tay chiến thắng
  CUDDLE_NUDGE,        // Tiếng kêu nũng nịu đòi xoa đầu
  THUG_LIFE,           // Giai điệu kính đen ngầu Thug Life
  HAPPY_CHIRP,         // Tiếng kêu vui vẻ ríu rít arpeggio
  LISTENING_PING,      // Tiếng ping radar lắng nghe
  THINKING_TINKLE,     // Âm thanh tò mò suy nghĩ
  SPEAKING_BABBLE,     // Âm thanh bập bẹ đang nói
  OBSTACLE_ALERT,      // Tiếng cảnh báo vật cản đụng phải
  SLEEPY_SNORE         // Tiếng ngáy ngủ / hát ru êm dịu
};

// Khởi tạo I2S1 cho Loa MAX98357A
bool init();

// Phát âm thanh thú cưng tương ứng
void play(PetSound sound);

// Đang phát âm thanh
bool isPlaying();

} // namespace pet_audio
