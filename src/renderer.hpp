#pragma once

#include "framebuffer.hpp"
#include "math_util.hpp"
#include "vector.hpp"
#include <memory>

typedef math::vec4 (*fragment_shader)(const void *vars);

inline void interpolate_vars(const void *src_vars, void *dst_vars,
                             i32 vars_size, math::vec3 weights) {

  size num_floats_per_vertex = (vars_size) / sizeof(float);
  // size num_floats = vars_size / sizeof(float);
  f32 *src0 = (f32 *)src_vars;
  f32 *src1 = (f32 *)((u8 *)src_vars + vars_size);
  f32 *src2 = (f32 *)((u8 *)src_vars + 2 * vars_size);
  f32 *dst = (f32 *)dst_vars;

  for (i32 i = 0; i < num_floats_per_vertex; ++i) {
    f32 sum = src0[i] * weights.x + src1[i] * weights.y + src2[i] * weights.z;
    dst[i] = sum;
  }
}

struct renderer {
  renderer(framebuffer &fb) : fb(fb) {}

  void draw_triangle(math::vec4 clip_coords[3], const void *varyings,
                     size varyings_size, fragment_shader fs) {

    f32 area = math::det_2d(clip_coords[1] - clip_coords[0],
                            clip_coords[2] - clip_coords[0]);
    f32 rcp_area = 1.f / area;

    i32 xmin =
        (i32)std::min({clip_coords[0].x, clip_coords[1].x, clip_coords[2].x});
    i32 xmax =
        (i32)std::max({clip_coords[0].x, clip_coords[1].x, clip_coords[2].x});
    i32 ymin =
        (i32)std::min({clip_coords[0].y, clip_coords[1].y, clip_coords[2].y});
    i32 ymax =
        (i32)std::max({clip_coords[0].y, clip_coords[1].y, clip_coords[2].y});

    // void *out_vars = std::malloc(10 * sizeof(f32));

    for (i32 y = ymin; y <= ymax; ++y) {
      for (i32 x = xmin; x <= xmax; ++x) {
        math::vec4 p = {x + 0.5f, y + 0.5f, 0.f, 1.f};
        f32 det01p =
            math::det_2d(clip_coords[1] - clip_coords[0], p - clip_coords[0]);
        f32 det12p =
            math::det_2d(clip_coords[2] - clip_coords[1], p - clip_coords[1]);
        f32 det20p =
            math::det_2d(clip_coords[0] - clip_coords[2], p - clip_coords[2]);

        // todo: flip the signs when we flip the coordinate system to the
        // version
        if (det01p > 0.f || det12p > 0.f || det20p > 0.f)
          continue;

        f32 alpha = det12p * rcp_area;
        f32 beta = det20p * rcp_area;
        f32 gamma = det01p * rcp_area;

        char out_vars[512];
        interpolate_vars(varyings, out_vars, varyings_size,
                         math::vec3{alpha, beta, gamma});

        math::vec4 final_color = fs(out_vars);

        fb.put_pixel(x, y, to_color(final_color));
      }
    }
  }

private:
  framebuffer &fb;
};
