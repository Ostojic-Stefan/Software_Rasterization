#include "event.hpp"
#include "framebuffer.hpp"
#include "timer.hpp"
#include "window.hpp"
#include <print>

static b8 running = true;

int main(int argc, char *argv[]) {

  window wnd;
  b8 ok = wnd.init("Software Rasterizer", 800, 600);

  if (!ok) {
    std::println("Failed to Initialize Window!");
    return 1;
  }

  event::register_callback([&](event::Event event) {
    if (event.type == event::EventType::AppQuit) {
      running = false;
    }
  });

  framebuffer fb(800, 600);

  struct timer timer;

  while (running) {
    wnd.process_events();

    // update
    std::println("FPS = {}", 1.f / timer.get_elapsed_s());

    // render

    fb.clear_color(colors::black);
    for (i32 y = 0; y < fb.get_height(); ++y) {
      for (i32 x = 0; x < fb.get_width(); ++x) {
        math::vec4 color = {x / (f32)fb.get_width(), y / (f32)fb.get_height(),
                            0.f, 1.f};
        fb.put_pixel(x, y, to_color(color));
      }
    }
    wnd.display_framebuffer(fb);
  }

  return 0;
}