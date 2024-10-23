#pragma once
#include <vector>

#include "geometry.h"

class Mesh {
 public:
  std::vector<Vec3f> vertices;
  std::vector<std::vector<int>> faces;

  static Mesh addCube(Mesh &mesh);
  void scale(float x, float y, float z);
  void rotateX(float angle);
  void rotateY(float angle);
  void rotateZ(float angle);
  void translate(float x, float y, float z);

 private:
  Vec3f scalingFactor;
  Vec3f rotation;
  Vec3f translation;
};
