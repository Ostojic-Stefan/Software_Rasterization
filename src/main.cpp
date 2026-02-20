#include "event.hpp"
#include "framebuffer.hpp"
#include "matrix.hpp"
#include "renderer.hpp"
#include "timer.hpp"
#include "window.hpp"
#include <numbers>
#include <print>

static b8 running = true;
static f32 total_time;

typedef struct {
  math::vec3 pos;
  math::vec4 col;
} vertex;

typedef struct {
  math::vec4 col;
} varying;

void my_vertex_shader(const void *in, math::vec4 *out_pos, void *out_var) {
  const vertex *v = (const vertex *)in;
  varying *var = (varying *)out_var;

  math::mat4 rot = math::mat4::rotation_z(total_time);
  math::vec4 world_pos = rot * math::vec4(v->pos, 1.f);

  memcpy(out_pos, &world_pos, sizeof(math::vec4));
  var->col = v->col;
}

math::vec4 my_fragment_shader(void *in_var) {
  varying *var = (varying *)in_var;

  return var->col;
}

static void render(rendering_pipeline &pipeline) {
  shader_program program = {.varying_size = sizeof(varying),
                            .vertex_shader = my_vertex_shader,
                            .fragment_shader = my_fragment_shader};

  vertex mesh[] = {
      {math::vec3{-0.5f, -0.5f, 0.f}, math::vec4{1, 0, 0, 1}}, // bottom-left
      {math::vec3{0.5f, -0.5f, 0.f}, math::vec4{0, 1, 0, 1}},  // bottom-right
      {math::vec3{-0.5f, 0.5f, 0.f}, math::vec4{0, 0, 1, 1}},  // top-left
      {math::vec3{0.5f, -0.5f, 0.f}, math::vec4{0, 1, 0, 1}},  // bottom-right
      {math::vec3{0.5f, 0.5f, 0.f}, math::vec4{0, 0, 1, 1}},   // top-right
      {math::vec3{-0.5f, 0.5f, 0.f}, math::vec4{0, 0, 1, 1}},  // top-left
  };

  vertex_buffer vbuf(mesh, sizeof(vertex));

  pipeline.execute_pipeline(&program, vbuf, 6);
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

  // renderer rnd(fb);
  rendering_pipeline pipeline(fb);

  struct timer timer;

  while (running) {
    wnd.process_events();
    f32 dt = timer.get_elapsed_s();
    total_time += dt;
    update(dt);
    fb.clear_color(colors::black);
    render(pipeline);
    wnd.display_framebuffer(fb);
  }

  return 0;
}