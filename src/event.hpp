#include "input.hpp"
#include "types.hpp"
#include <functional>
#include <variant>

namespace event {
enum class EventType : u8 { Key, MouseMove, MousePress, Resize, AppQuit };

struct KeyData {
  input::key_code key;
  bool pressed;
};

struct MousePressData {
  input::mouse_btn btn;
};

struct MouseMoveData {
  math::vec2 pos;
};

struct Event {
  EventType type;
  union {
    KeyData key;
    MousePressData mouse_press;
    MouseMoveData mouse_move;
  } data;
};

using Callback = std::function<void(const Event &)>;

void register_callback(Callback cb);
void trigger_event(const Event &e);
} // namespace  event