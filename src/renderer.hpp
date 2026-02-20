#pragma once

#include "buffer.hpp"
#include "framebuffer.hpp"
#include "math_util.hpp"
#include "shader_program.hpp"
#include "varying.hpp"
#include "vector.hpp"
#include <array>
#include <memory>

struct viewport {
  std::int32_t xmin, ymin, xmax, ymax;

  /// <summary>
  /// Transforms the point specified in NDC Space (0,0) is middle,
  /// to the range specified by the bounds (xmin, ymin, xmax, ymax)
  /// </summary>
  /// <param name="pt">The point that will be transformed</param>
  /// <returns>The Transformed point</returns>
  math::vec4 transform(math::vec4 pt) const {
    pt.x = xmin + (xmax - xmin) * (0.5f + 0.5f * pt.x);
    pt.y = ymin + (ymax - ymin) * (0.5f - 0.5f * pt.y);
    return pt;
  }

  f32 get_aspect_wh() const { return (xmax - xmin) / f32(ymax - ymin); }
  f32 get_aspect_hw() const { return (ymax - ymin) / f32(xmax - xmin); }
};

static void draw_triangle(framebuffer &fb, shader_program *program,
                          math::vec4 positions[3],
                          void *varyings) // packed: v0|v1|v2
{
  f32 area = math::det_2d(math::vec4{positions[1].x - positions[0].x,
                                     positions[1].y - positions[0].y, 0, 0},
                          math::vec4{positions[2].x - positions[0].x,
                                     positions[2].y - positions[0].y, 0, 0});

  if (area == 0.0f)
    return;

  f32 inv_area = 1.f / area;

  i32 xmin = (i32)fminf(fminf(positions[0].x, positions[1].x), positions[2].x);
  i32 xmax = (i32)fmaxf(fmaxf(positions[0].x, positions[1].x), positions[2].x);
  i32 ymin = (i32)fminf(fminf(positions[0].y, positions[1].y), positions[2].y);
  i32 ymax = (i32)fmaxf(fmaxf(positions[0].y, positions[1].y), positions[2].y);

  u8 interp_buffer[256];
  if (program->varying_size > sizeof(interp_buffer))
    return;

  for (i32 y = ymin; y <= ymax; ++y) {
    for (i32 x = xmin; x <= xmax; ++x) {
      math::vec4 p = {x + 0.5f, y + 0.5f, 0, 0};

      f32 w0 = math::det_2d(
          (math::vec4){positions[2].x - positions[1].x,
                       positions[2].y - positions[1].y, 0, 0},
          (math::vec4){p.x - positions[1].x, p.y - positions[1].y, 0, 0});

      f32 w1 = math::det_2d(
          (math::vec4){positions[0].x - positions[2].x,
                       positions[0].y - positions[2].y, 0, 0},
          (math::vec4){p.x - positions[2].x, p.y - positions[2].y, 0, 0});

      f32 w2 = math::det_2d(
          (math::vec4){positions[1].x - positions[0].x,
                       positions[1].y - positions[0].y, 0, 0},
          (math::vec4){p.x - positions[0].x, p.y - positions[0].y, 0, 0});

      if (w0 > 0 || w1 > 0 || w2 > 0)
        continue;

      math::vec3 bary = {w0 * inv_area, w1 * inv_area, w2 * inv_area};

      interpolate_vars(varyings, interp_buffer, program->varying_size, bary);

      math::vec4 color = program->fragment_shader(interp_buffer);

      fb.put_pixel(x, y, to_color(color));
    }
  }
}

struct rendering_pipeline {

  rendering_pipeline(framebuffer &fb) : fb(fb) {
    // todo: un-hardcode
    vp = {0, 0, 800, 600};
  }

  void execute_pipeline(shader_program *program, vertex_buffer vbuf,
                        i32 vertex_count) {

    assert(vertex_count % 3 == 0);
    size n_triangles = vertex_count / 3;

    // for each triangle...
    for (size tri = 0; tri < n_triangles; ++tri) {
      u8 vars[3 * 256];
      std::array<math::vec4, 3> positions;

      assert(program->varying_size < 256);

      for (size v = 0; v < 3; ++v) {
        void *vtx_ptr = vbuf.data + (tri * 3 + v) * vbuf.stride;

        void *out_vars = vars + v * program->varying_size;

        program->vertex_shader(vtx_ptr, &positions[v], out_vars);

        positions[v].x *= vp.get_aspect_hw();
        positions[v] = vp.transform(positions[v]);
      }

      draw_triangle(fb, program, positions.data(), vars);
    }
  }

private:
  framebuffer &fb;
  viewport vp;
};
