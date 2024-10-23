#include "mesh.h"

#include <iostream>
Mesh Mesh::addCube(Mesh &mesh) {
  std::vector<Vec3f> cubeVertices = {
      {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
      {0.5, 0.5, -0.5},   {-0.5, 0.5, -0.5},  // Back face
      {-0.5, -0.5, 0.5},  {0.5, -0.5, 0.5},
      {0.5, 0.5, 0.5},    {-0.5, 0.5, 0.5}  // Front face
  };

  std::vector<std::vector<int>> cubeFaces = {
      {0, 1, 2}, {2, 3, 0},  // Back face
      {4, 5, 6}, {6, 7, 4},  // Front face
      {0, 1, 5}, {5, 4, 0},  // Bottom face
      {2, 3, 7}, {7, 6, 2},  // Top face
      {0, 3, 7}, {7, 4, 0},  // Left face
      {1, 2, 6}, {6, 5, 1}   // Right face
  };

  mesh.vertices = cubeVertices;
  mesh.faces = cubeFaces;
  return mesh;
}
