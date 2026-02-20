#pragma once

#include "vector.hpp"

typedef void (*vertex_shader_fn)(const void *in_vertex,
                                 math::vec4 *out_position, void *out_varying);

typedef math::vec4 (*fragment_shader_fn)(void *varying);

typedef struct {
  // size_t vertex_stride;
  size_t varying_size;

  vertex_shader_fn vertex_shader;
  fragment_shader_fn fragment_shader;
} shader_program;
