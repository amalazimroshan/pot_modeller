#pragma once

#include <array>
#include <iostream>

// General template for Vec<N>
template <typename T, size_t N>
struct Vec {
  std::array<T, N> data;

  Vec() { data.fill(T()); }

  Vec(std::initializer_list<T> list) {
    size_t i = 0;
    for (auto& val : list) {
      if (i < N) data[i++] = val;
    }
  }

  T& operator[](size_t index) { return data[index]; }
  const T& operator[](size_t index) const { return data[index]; }

  Vec<T, N> operator+(const Vec<T, N>& v) const {
    Vec<T, N> result;
    for (size_t i = 0; i < N; ++i) {
      result[i] = data[i] + v[i];
    }
    return result;
  }

  Vec<T, N> operator-(const Vec<T, N>& v) const {
    Vec<T, N> result;
    for (size_t i = 0; i < N; ++i) {
      result[i] = data[i] - v[i];
    }
    return result;
  }

  Vec<T, N> operator*(const T& scalar) const {
    Vec<T, N> result;
    for (size_t i = 0; i < N; ++i) {
      result[i] = data[i] * scalar;
    }
    return result;
  }

  Vec<T, N> operator/(const T& scalar) const {
    Vec<T, N> result;
    for (size_t i = 0; i < N; ++i) {
      result[i] = data[i] / scalar;
    }
    return result;
  }

  friend std::ostream& operator<<(std::ostream& os, const Vec<T, N>& v) {
    os << "(";
    for (size_t i = 0; i < N; ++i) {
      os << v[i];
      if (i < N - 1) os << ", ";
    }
    os << ")";
    return os;
  }
};

// Specialization for Vec<2>
template <typename T>
struct Vec<T, 2> {
  union {
    std::array<T, 2> data;
    struct {
      T x, y;
    };
  };

  Vec() : x(T()), y(T()) {}
  Vec(T x, T y) : x(x), y(y) {}

  T& operator[](size_t index) { return data[index]; }
  const T& operator[](size_t index) const { return data[index]; }

  Vec<T, 2> operator+(const Vec<T, 2>& v) const {
    return Vec<T, 2>(x + v.x, y + v.y);
  }

  Vec<T, 2> operator-(const Vec<T, 2>& v) const {
    return Vec<T, 2>(x - v.x, y - v.y);
  }

  Vec<T, 2> operator*(const T& scalar) const {
    return Vec<T, 2>(x * scalar, y * scalar);
  }

  Vec<T, 2> operator/(const T& scalar) const {
    return Vec<T, 2>(x / scalar, y / scalar);
  }

  friend std::ostream& operator<<(std::ostream& os, const Vec<T, 2>& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
  }
};

// Specialization for Vec<3>
template <typename T>
struct Vec<T, 3> {
  union {
    std::array<T, 3> data;
    struct {
      T x, y, z;
    };
  };

  Vec() : x(T()), y(T()), z(T()) {}
  Vec(T x, T y, T z) : x(x), y(y), z(z) {}

  T& operator[](size_t index) { return data[index]; }
  const T& operator[](size_t index) const { return data[index]; }

  Vec<T, 3> operator+(const Vec<T, 3>& v) const {
    return Vec<T, 3>(x + v.x, y + v.y, z + v.z);
  }

  Vec<T, 3> operator-(const Vec<T, 3>& v) const {
    return Vec<T, 3>(x - v.x, y - v.y, z - v.z);
  }

  Vec<T, 3> operator*(const T& scalar) const {
    return Vec<T, 3>(x * scalar, y * scalar, z * scalar);
  }

  Vec<T, 3> operator/(const T& scalar) const {
    return Vec<T, 3>(x / scalar, y / scalar, z / scalar);
  }

  friend std::ostream& operator<<(std::ostream& os, const Vec<T, 3>& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
  }
};

// Cross product for Vec3
template <typename T>
Vec<T, 3> cross(const Vec<T, 3>& v1, const Vec<T, 3>& v2) {
  return Vec<T, 3>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                   v1.x * v2.y - v1.y * v2.x);
}

// Dot product for Vec3
template <typename T>
T dot(const Vec<T, 3>& v1, const Vec<T, 3>& v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Type aliases for convenience
template <typename T>
using Vec2 = Vec<T, 2>;

template <typename T>
using Vec3 = Vec<T, 3>;
