#pragma once
#include <vector>

#include "geometry.h"

class Mesh {
 public:
  std::vector<Vec3f> vertices;
  std::vector<std::vector<int>> faces;

  static Mesh createCube();

  static Matrix createScaleMatrix (const Vec3f& s);
  static Matrix createRotateMatrix (const Vec3f& angles);
  static Matrix createTranslateMatrix (const Vec3f& t);

  void applyTransform(const Matrix& transform);
};
