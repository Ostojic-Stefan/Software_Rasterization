#pragma once

#include "types.hpp"
#include <numbers>

namespace math {
static inline constexpr f32 pi32 = std::numbers::pi_v<f32>;
static inline constexpr f64 pi64 = std::numbers::pi_v<f64>;
static inline constexpr f32 half_pi32 = std::numbers::pi_v<f32> / 2.f;

template <typename T> static inline T lerp(const T &a, const T &b, f32 t) {
  return (1.f - t) * a + t * b;
}

template <typename T>
static inline T remap(T val, T in_min, T in_max, T out_min, T out_max) {
  T t = (val - in_min) / (in_max - in_min);
  return lerp(out_min, out_max, t);
}

template <typename TVec>
static inline f32 det_2d(const TVec &v0, const TVec &v1) {
  return v0.x * v1.y - v0.y * v1.x;
}

template <typename T> static inline f32 radians(const T &deg) {
  return deg * (180.0f / pi32);
}

// �� reflect for 2D ��
template <typename T>
inline vector2d<T> reflect(vector2d<T> const &I, vector2d<T> const &N) {
  // R = I - 2 * dot(N, I) * N
  return I - N * (static_cast<T>(2) * dot(N, I));
}

// �� reflect for 3D ��
template <typename T>
inline vector3d<T> reflect(vector3d<T> const &I, vector3d<T> const &N) {
  return I - N * (static_cast<T>(2) * dot(N, I));
}

// �� reflect for 4D ��
template <typename T>
inline vector4d<T> reflect(vector4d<T> const &I, vector4d<T> const &N) {
  return I - N * (static_cast<T>(2) * dot(N, I));
}
}; // namespace math