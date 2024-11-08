#pragma once
#include <renderlite/geometry.hpp>
#include <vector>

class Mesh {
 public:
  std::vector<Vec3f> vertices;
  std::vector<Vec3f> faces;

  static Mesh createCube();
  static Mesh createOBJ(char* filepath);

  static Matrix createScaleMatrix(const Vec3f& s);
  static Matrix createRotateMatrix(const Vec3f& angles);
  static Matrix createTranslateMatrix(const Vec3f& t);
};
