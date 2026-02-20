#pragma once

#include "buffer.hpp"
#include "types.hpp"
#include "vector.hpp"
#include <array>
#include <cassert>

inline void interpolate_vars(const void *src, void *dst, size_t size,
                             math::vec3 w) {
  size_t count = size / sizeof(f32);

  const f32 *v0 = (const f32 *)src;
  const f32 *v1 = (const f32 *)((const u8 *)src + size);
  const f32 *v2 = (const f32 *)((const u8 *)src + 2 * size);

  f32 *out = (f32 *)dst;

  for (size_t i = 0; i < count; ++i) {
    out[i] = v0[i] * w.x + v1[i] * w.y + v2[i] * w.z;
  }
}

// struct vs_input {
//   void set(size_t slot, buffer buf) {
//     assert(slot < MAX_ATTRIBS);
//     attribs[slot] = buf;
//   }

//   buffer get(size_t slot) const {
//     assert(slot < MAX_ATTRIBS);
//     return attribs[slot];
//   }

// private:
//   static constexpr size MAX_ATTRIBS = 16;
//   std::array<buffer, MAX_ATTRIBS> attribs;
// };

// struct vs_out {

//   template <typename V> void set_varying(u32 location, V var) {
//     assert(location < MAX_VARYINGS);
//     assert(location == used && "location must be the next unused slot");

//     if constexpr (std::is_same_v<V, math::vec2>) {
//       buffer val = {};
//       val.
//     }
//   }

//   size get_size() const { return vars_used; }

// public:
//   math::vec4 position;
//   size vars_used = 0;
//   static constexpr size MAX_VARYINGS = 6;
//   std::array<buffer, MAX_VARYINGS> varyings = {0};
// };

// template <> inline math::vec2 vs_input::get<math::vec2>(size_t slot) const {
//   assert(slot < MAX_ATTRIBS);
//   buffer attrib = attribs[slot];
//   assert(attrib.count == 2);

//   return {attrib.vals[0], attrib.vals[1]};
// }

// template <> inline math::vec3 vs_input::get<math::vec3>(size_t slot) const {
//   assert(slot < MAX_ATTRIBS);
//   buffer attrib = attribs[slot];
//   assert(attrib.count == 3);

//   return {attrib.vals[0], attrib.vals[1], attrib.vals[2]};
// }

// template <> inline math::vec4 vs_input::get<math::vec4>(size_t slot) const {
//   assert(slot < MAX_ATTRIBS);
//   buffer attrib = attribs[slot];
//   assert(attrib.count == 4);

//   return {attrib.vals[0], attrib.vals[1], attrib.vals[2], attrib.vals[3]};
// }