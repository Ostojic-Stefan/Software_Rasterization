#pragma once

#include "vector.hpp"
#include <algorithm>
#include <cmath>
#include <initializer_list>

namespace math {
struct mat4 {
  f32 values[16];

  mat4() = default;

  mat4(std::initializer_list<f32> list) {
    std::copy(list.begin(), list.end(), values);
  }

  static mat4 identity() {
    return mat4{1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};
  }

  static mat4 scale(const vec3 &s) {
    return mat4{s.x, 0.f, 0.f, 0.f, 0.f, s.y, 0.f, 0.f,
                0.f, 0.f, s.z, 0.f, 0.f, 0.f, 0.f, 1.f};
  }

  static mat4 scale(f32 s) { return scale(vec3{s, s, s}); }

  static mat4 translate(const vec3 &t) {
    return mat4{1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f, t.x, t.y, t.z, 1.f};
  }

  static mat4 rotation_x(f32 angle) {
    f32 c = std::cos(angle);
    f32 s = std::sin(angle);
    return mat4{1.f, 0.f, 0.f, 0.f, 0.f, c,   s,   0.f,
                0.f, -s,  c,   0.f, 0.f, 0.f, 0.f, 1.f};
  }

  static mat4 rotation_y(f32 angle) {
    f32 c = std::cos(angle);
    f32 s = std::sin(angle);
    return mat4{c, 0.f, -s, 0.f, 0.f, 1.f, 0.f, 0.f,
                s, 0.f, c,  0.f, 0.f, 0.f, 0.f, 1.f};
  }

  static mat4 rotation_z(f32 angle) {
    f32 c = std::cos(angle);
    f32 s = std::sin(angle);
    return mat4{c,   s,   0.f, 0.f, -s,  c,   0.f, 0.f,
                0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};
  }

  static mat4 rotation(const vec3 &axis, f32 angle) {
    vec3 u = normalize(axis);
    f32 x = u.x, y = u.y, z = u.z;

    f32 c = std::cos(angle);
    f32 s = std::sin(angle);
    f32 t = 1 - c;

    return mat4{t * x * x + c,
                t * x * y - s * z,
                t * x * z + s * y,
                0.f,
                t * x * y + s * z,
                t * y * y + c,
                t * y * z - s * x,
                0.f,
                t * x * z - s * y,
                t * y * z + s * x,
                t * z * z + c,
                0.f,
                0.f,
                0.f,
                0.f,
                1.f};
  }

  static mat4 perspective(f32 near, f32 far, f32 fovy, f32 aspect_ratio) {
    mat4 result{};

    float tanHalfFovy = std::tan(fovy / 2.0f);

    f32 f = 1.0f / tanHalfFovy;
    result.values[0] = f / aspect_ratio;
    result.values[5] = f;

    result.values[10] = (far + near) / (near - far);
    result.values[11] = -1.0f;

    result.values[14] = (2.0f * far * near) / (near - far);

    return result;
  }

  static inline mat4 look_at(const vec3 &eye, const vec3 &center,
                             const vec3 &up) {
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f, up));
    vec3 u = cross(s, f);

    mat4 result = identity();

    result.values[0] = s.x;
    result.values[4] = s.y;
    result.values[8] = s.z;

    result.values[1] = u.x;
    result.values[5] = u.y;
    result.values[9] = u.z;

    result.values[2] = -f.x;
    result.values[6] = -f.y;
    result.values[10] = -f.z;

    result.values[12] = -dot(s, eye);
    result.values[13] = -dot(u, eye);
    result.values[14] = dot(f, eye);
    result.values[15] = 1.0f;

    return result;
  }

  static mat4 orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near,
                           f32 far) {
    mat4 result{}; // zero-initialize

    result.values[0] = 2.f / (right - left);
    result.values[5] = 2.f / (top - bottom);
    result.values[10] = -2.f / (far - near);

    result.values[12] = -(right + left) / (right - left);
    result.values[13] = -(top + bottom) / (top - bottom);
    result.values[14] = -(far + near) / (far - near);
    result.values[15] = 1.f;

    return result;
  }
};

static inline vec4 operator*(const mat4 &m, const vec4 &v) {
  vec4 result;
  result.x = m.values[0] * v.x + m.values[4] * v.y + m.values[8] * v.z +
             m.values[12] * v.w;
  result.y = m.values[1] * v.x + m.values[5] * v.y + m.values[9] * v.z +
             m.values[13] * v.w;
  result.z = m.values[2] * v.x + m.values[6] * v.y + m.values[10] * v.z +
             m.values[14] * v.w;
  result.w = m.values[3] * v.x + m.values[7] * v.y + m.values[11] * v.z +
             m.values[15] * v.w;
  return result;
}

static inline mat4 operator*(const mat4 &A, const mat4 &B) {
  mat4 R{};
  for (i32 col = 0; col < 4; ++col) {
    for (i32 row = 0; row < 4; ++row) {
      f32 sum = 0.f;
      for (i32 k = 0; k < 4; ++k) {
        sum += A.values[k * 4 + row] * B.values[col * 4 + k];
      }
      R.values[col * 4 + row] = sum;
    }
  }
  return R;
}
} // namespace math