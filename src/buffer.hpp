#pragma once

#include "types.hpp"
#include <vector>

// struct buffer {
//   void *data;
//   size element_size;
//   size num_elements;
// };

// struct vertex_attrib {
//   size element_count;
//   u32 offset;
//   size slot;
// };

struct vertex_buffer {
  vertex_buffer(const void *data, size stride)
      : data((u8 *)data), stride(stride) {}

  u8 *data;
  size stride;
};