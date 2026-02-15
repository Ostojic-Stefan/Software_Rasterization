#pragma once

#include "types.hpp"
#include <cassert>
#include <cmath>
#include <immintrin.h>
#include <initializer_list>

namespace math {
template <typename T> struct vector2d;
template <typename T> struct vector3d;
template <typename T> struct vector4d;

template <typename T> struct vector2d {
  union {
    struct {
      T x, y;
    };
    T values[2];
  };

  constexpr vector2d() : x(T(0)), y(T(0)) {}
  constexpr explicit vector2d(T val) : x(val), y(val) {}
  constexpr vector2d(T _x, T _y) : x(_x), y(_y) {}

  constexpr vector2d(std::initializer_list<T> list) {
    assert(list.size() == 2 && "vector2d must be initialized with 2 elements.");
    auto it = list.begin();
    x = *it++;
    y = *it++;
  }

  template <typename U>
  explicit constexpr vector2d(vector2d<U> const &o)
      : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)) {}

  vector2d &operator+=(vector2d const &o) {
    x += o.x;
    y += o.y;
    return *this;
  }
  vector2d &operator-=(vector2d const &o) {
    x -= o.x;
    y -= o.y;
    return *this;
  }
  vector2d &operator*=(T s) {
    x *= s;
    y *= s;
    return *this;
  }
  vector2d &operator/=(T s) {
    x /= s;
    y /= s;
    return *this;
  }
};

template <typename T>
inline vector2d<T> operator+(vector2d<T> a, vector2d<T> const &b) {
  return {a.x + b.x, a.y + b.y};
}
template <typename T>
inline vector2d<T> operator-(vector2d<T> a, vector2d<T> const &b) {
  return {a.x - b.x, a.y - b.y};
}
template <typename T> inline vector2d<T> operator-(vector2d<T> v) {
  return {-v.x, -v.y};
}
template <typename T> inline vector2d<T> operator*(vector2d<T> v, T s) {
  return {v.x * s, v.y * s};
}
template <typename T> inline vector2d<T> operator*(T s, vector2d<T> v) {
  return v * s;
}
template <typename T> inline vector2d<T> operator/(vector2d<T> v, T s) {
  return {v.x / s, v.y / s};
}
template <typename T> inline T dot(vector2d<T> const &a, vector2d<T> const &b) {
  return a.x * b.x + a.y * b.y;
}
template <typename T> inline T length(vector2d<T> const &v) {
  return std::sqrt(dot(v, v));
}
template <typename T> inline vector2d<T> normalize(vector2d<T> const &v) {
  T len = length(v);
  assert(len != T(0) && "Cannot normalize zero-length vector2d");
  return v / len;
}

template <typename T> struct vector3d {
  union {
    struct {
      T x, y, z;
    };
    T values[3];
  };

  constexpr vector3d() : x(T(0)), y(T(0)), z(T(0)) {}
  constexpr explicit vector3d(T val) : x(val), y(val), z(val) {}
  constexpr vector3d(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

  constexpr vector3d(std::initializer_list<T> list) {
    assert(list.size() == 3 && "vector3d must be initialized with 3 elements.");
    auto it = list.begin();
    x = *it++;
    y = *it++;
    z = *it++;
  }

  template <typename U>
  explicit constexpr vector3d(vector4d<U> const &o)
      : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)), z(static_cast<T>(o.z)) {
  }

  template <typename U>
  constexpr vector3d(vector2d<U> const &o, T _z)
      : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)), z(_z) {}

  template <typename U>
  explicit constexpr vector3d(vector3d<U> const &o)
      : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)), z(static_cast<T>(o.z)) {
  }

  vector3d &operator+=(vector3d const &o) {
    x += o.x;
    y += o.y;
    z += o.z;
    return *this;
  }
  vector3d &operator-=(vector3d const &o) {
    x -= o.x;
    y -= o.y;
    z -= o.z;
    return *this;
  }
  vector3d &operator*=(T s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }
  vector3d &operator/=(T s) {
    x /= s;
    y /= s;
    z /= s;
    return *this;
  }
};

template <typename T>
inline vector3d<T> operator+(vector3d<T> a, vector3d<T> const &b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}
template <typename T>
inline vector3d<T> operator-(vector3d<T> a, vector3d<T> const &b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
}
template <typename T> inline vector3d<T> operator-(vector3d<T> v) {
  return {-v.x, -v.y, -v.z};
}
template <typename T> inline vector3d<T> operator*(vector3d<T> v, T s) {
  return {v.x * s, v.y * s, v.z * s};
}
template <typename T> inline vector3d<T> operator*(T s, vector3d<T> v) {
  return v * s;
}

template <typename T>
inline vector3d<T> operator*(vector3d<T> v1, vector3d<T> v2) {
  return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}
template <typename T> inline vector3d<T> operator/(vector3d<T> v, T s) {
  return {v.x / s, v.y / s, v.z / s};
}

template <typename T>
inline bool operator==(vector3d<T> const &a, vector3d<T> const &b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}
template <typename T>
inline bool operator!=(vector3d<T> const &a, vector3d<T> const &b) {
  return !(a == b);
}

template <typename T> inline T dot(vector3d<T> const &a, vector3d<T> const &b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
template <typename T> inline T length(vector3d<T> const &v) {
  return std::sqrt(dot(v, v));
}
template <typename T> inline vector3d<T> normalize(vector3d<T> const &v) {
  T len = length(v);
  assert(len != T(0) && "Cannot normalize zero-length vector3d");
  return v / len;
}
template <typename T>
inline vector3d<T> cross(vector3d<T> const &a, vector3d<T> const &b) {
  return {(a.y * b.z - a.z * b.y), (a.z * b.x - a.x * b.z),
          (a.x * b.y - a.y * b.x)};
}

template <typename T> struct vector4d {
  union {
    struct {
      T x, y, z, w;
    };
    T values[4];
    __m128 _v;
  };

  template <typename U>
  explicit constexpr vector4d(vector3d<U> const &o, T _w = T(0))
      : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)), z(static_cast<T>(o.z)),
        w(_w) {}

  /// explicit construct from any U-typed vec2, plus your own z and w
  template <typename U>
  constexpr vector4d(vector2d<U> const &o, T _z, T _w)
      : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)), z(_z), w(_w) {}

  constexpr vector4d() : x(T(0)), y(T(0)), z(T(0)), w(T(0)) {}
  constexpr explicit vector4d(T val) : x(val), y(val), z(val), w(val) {}
  constexpr vector4d(std::initializer_list<T> list) {
    assert(list.size() == 4 && "vector4d must be initialized with 4 elements.");
    auto it = list.begin();
    x = *it++;
    y = *it++;
    z = *it++;
    w = *it++;
  }
  constexpr vector4d(vector3d<T> const &v, T _w)
      : x(v.x), y(v.y), z(v.z), w(_w) {}

  template <typename U>
  explicit constexpr vector4d(vector4d<U> const &o)
      : x(static_cast<T>(o.x)), y(static_cast<T>(o.y)), z(static_cast<T>(o.z)),
        w(static_cast<T>(o.w)) {}

  vector4d &operator+=(vector4d const &o) {
    x += o.x;
    y += o.y;
    z += o.z;
    w += o.w;
    return *this;
  }
  vector4d &operator-=(vector4d const &o) {
    x -= o.x;
    y -= o.y;
    z -= o.z;
    w -= o.w;
    return *this;
  }
  vector4d &operator*=(T s) {
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
  }
  vector4d &operator/=(T s) {
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
  }
};

template <typename T>
inline vector4d<T> operator+(vector4d<T> a, vector4d<T> const &b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}
template <typename T>
inline vector4d<T> operator-(vector4d<T> a, vector4d<T> const &b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}
template <typename T> inline vector4d<T> operator-(vector4d<T> v) {
  return {-v.x, -v.y, -v.z, -v.w};
}
template <typename T> inline vector4d<T> operator*(vector4d<T> v, T s) {
  return {v.x * s, v.y * s, v.z * s, v.w * s};
}
template <typename T> inline vector4d<T> operator*(T s, vector4d<T> v) {
  return v * s;
}
template <typename T> inline vector4d<T> operator/(vector4d<T> v, T s) {
  return {v.x / s, v.y / s, v.z / s, v.w / s};
}
template <typename T> inline T dot(vector4d<T> const &a, vector4d<T> const &b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

using vec2 = vector2d<f32>;
using vec2d = vector2d<f64>;
using vec2i = vector2d<i32>;

using vec3 = vector3d<f32>;
using vec3d = vector3d<f64>;
using vec3i = vector3d<i32>;

using vec4 = vector4d<f32>;
using vec4d = vector4d<f64>;
using vec4i = vector4d<i32>;
} // namespace math
