#include "event.hpp"
#include "framebuffer.hpp"
#include "renderer.hpp"
#include "timer.hpp"
#include "window.hpp"
#include <numbers>
#include <print>

static b8 running = true;

struct varying {
  math::vec4 color;
  math::vec2 tex_coord;
};

math::vec4 my_fragment_shader(const void *varyings) {
  varying vars = *(varying *)varyings;
  return math::vec4{0.f, 1.f, 0.f, 1.f};
  // return vars.color;
}

static void render(renderer &rnd) {
  math::vec4 coords[3] = {{100.f, 500.f, 1.f, 0.f},  // bottom-left
                          {500.f, 500.f, 1.f, 0.f},  // bottom-right
                          {300.f, 100.f, 1.f, 0.f}}; // middle-top
  varying vars[3] = {
      varying{.color = {0.f, 0.f, 1.f, 1.f}, .tex_coord = {0.f, 0.f}},
      varying{.color = {0.f, 1.f, 0.f, 1.f}, .tex_coord = {1.f, 0.f}},
      varying{.color = {1.f, 0.f, 0.f, 1.f}, .tex_coord = {0.f, 1.f}}};

  // const void *ptrs[3] = {&vars[0], &vars[1], &vars[2]};

  rnd.draw_triangle(coords, vars, sizeof(varying), my_fragment_shader);
}

static void update(f32 dt) { std::println("FPS = {}", 1.f / dt); }

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

  renderer rnd(fb);

  struct timer timer;

  while (running) {
    wnd.process_events();

    update(timer.get_elapsed_s());
    fb.clear_color(colors::black);
    render(rnd);
    wnd.display_framebuffer(fb);
  }

  return 0;
}