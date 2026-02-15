#pragma once

#include "color.hpp"
#include "types.hpp"
#include <memory>

struct framebuffer {
  framebuffer(u32 width, u32 height)
      : width{width}, height{height},
        color_buffer{std::make_unique<color[]>(width * height)},
        depth_buffer{std::make_unique<f32[]>(width * height)} {}

  void put_pixel(u32 x, u32 y, const color &c) {
    // todo: assertions
    assert(x < width);
    assert(y < height);
    assert(x >= 0);
    assert(y >= 0);

    color_buffer[y * width + x] = c;
  }

  color get_pixel(u32 x, u32 y, const color &c) {
    // todo: assertions
    assert(x < width);
    assert(y < height);
    assert(x >= 0);
    assert(y >= 0);

    return color_buffer[y * width + x];
  }

  void clear_color(const color &c) {
    std::fill(color_buffer.get(), color_buffer.get() + (width * height), c);
  }

  inline void reset(u32 width, u32 height) {
    this->width = width;
    this->height = height;

    color_buffer = std::make_unique<color[]>(width * height);
    depth_buffer = std::make_unique<f32[]>(width * height);
  }

  inline u32 get_width() const { return width; }
  inline u32 get_height() const { return height; }

  inline math::vec2i get_dimensions() const {
    return math::vec2i{(i32)width, (i32)height};
  }

  friend struct window;

private:
  u32 width, height;
  std::unique_ptr<color[]> color_buffer;
  std::unique_ptr<f32[]> depth_buffer;
};