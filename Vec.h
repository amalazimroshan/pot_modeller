#pragma once

#include <array>
#include <iostream>

template <typename T, size_t N>
struct Vec {
  std::array<T, N> data;

  // Default constructor
  Vec() { data.fill(T()); }

  // Constructor with initializer list
  Vec(std::initializer_list<T> list) {
    size_t i = 0;
    for (auto& val : list) {
      if (i < N) data[i++] = val;
    }
  }

  // Access by index
  T& operator[](size_t index) { return data[index]; }

  const T& operator[](size_t index) const { return data[index]; }

  // Operator overloading
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

  // Print for debugging purposes
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

// Specialization for Vec2 (2D vector) to access x and y
template <typename T>
struct Vec<T, 2> {
  std::array<T, 2> data;

  Vec() { data.fill(T()); }
  Vec(T x, T y) : data{x, y} {}

  T& operator[](size_t index) { return data[index]; }
  const T& operator[](size_t index) const { return data[index]; }

  T& x() { return data[0]; }
  T& y() { return data[1]; }

  const T& x() const { return data[0]; }
  const T& y() const { return data[1]; }

  // Operator overloading (same as above)
  // ...
};

// Specialization for Vec3 (3D vector) to access x, y, and z
template <typename T>
struct Vec<T, 3> {
  std::array<T, 3> data;

  Vec() { data.fill(T()); }
  Vec(T x, T y, T z) : data{x, y, z} {}

  T& operator[](size_t index) { return data[index]; }
  const T& operator[](size_t index) const { return data[index]; }

  T& x() { return data[0]; }
  T& y() { return data[1]; }
  T& z() { return data[2]; }

  const T& x() const { return data[0]; }
  const T& y() const { return data[1]; }
  const T& z() const { return data[2]; }

  // Operator overloading (same as above)
  // ...
  Vec<T, N> operator-(const Vec<T, N>& v) const {
    Vec<T, N> result;
    for (size_t i = 0; i < N; ++i) {
      result[i] = data[i] - v[i];
    }
    return result;
  }
};

// Cross product for Vec3
template <typename T>
Vec<T, 3> cross(const Vec<T, 3>& v1, const Vec<T, 3>& v2) {
  return Vec<T, 3>(v1.y() * v2.z() - v1.z() * v2.y(),
                   v1.z() * v2.x() - v1.x() * v2.z(),
                   v1.x() * v2.y() - v1.y() * v2.x());
}

// Specialization for Vec2 (2D vector)
template <typename T>
using Vec2 = Vec<T, 2>;

// Specialization for Vec3 (3D vector)
template <typename T>
using Vec3 = Vec<T, 3>;
