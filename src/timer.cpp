#include "timer.hpp"

f32 timer::get_elapsed_s() const { return get_elapsed_ms() / 1000.f; }

f32 timer::get_elapsed_ms() const {
  auto now = std::chrono::steady_clock::now();
  auto diff = now - prev_time;
  prev_time = now;
  return std::chrono::duration<f32, std::milli>(diff).count();
}