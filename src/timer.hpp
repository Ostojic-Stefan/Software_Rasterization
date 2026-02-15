#pragma once

#include "types.hpp"
#include <chrono>

struct timer {
public:
  timer() { prev_time = std::chrono::steady_clock::now(); }
  f32 get_elapsed_s() const;
  f32 get_elapsed_ms() const;

private:
  mutable std::chrono::steady_clock::time_point prev_time;
};
