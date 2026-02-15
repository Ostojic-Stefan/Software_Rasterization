#include "event.hpp"

namespace event {
static inline std::vector<Callback> callbacks;

void register_callback(Callback cb) { callbacks.push_back(cb); }

void trigger_event(const Event &e) {
  for (const auto &cb : callbacks)
    cb(e);
}
} // namespace event
