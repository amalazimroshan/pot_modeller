#include "mesh.h"
#include <iostream>
#include <fstream>
#include <sstream>

Mesh Mesh::createCube() {
  Mesh mesh;
  mesh.vertices = {
      {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
      {0.5, 0.5, -0.5},   {-0.5, 0.5, -0.5},  // Back face
      {-0.5, -0.5, 0.5},  {0.5, -0.5, 0.5},
      {0.5, 0.5, 0.5},    {-0.5, 0.5, 0.5}  // Front face
  };

  mesh.faces = {
      {0, 1, 2}, {2, 3, 0},  // Back face
      {4, 5, 6}, {6, 7, 4},  // Front face
      {0, 1, 5}, {5, 4, 0},  // Bottom face
      {2, 3, 7}, {7, 6, 2},  // Top face
      {0, 3, 7}, {7, 4, 0},  // Left face
      {1, 2, 6}, {6, 5, 1}   // Right face
  };
  return mesh;
}

Mesh Mesh::createOBJ(char* filename){
  Mesh mesh;
  std::cout<<"Opening file:"<< filename<<std::endl;
  std::ifstream file(filename);
  if(!file.is_open()){
     std::cerr << "Error: Could not open the file." << std::endl;
        return mesh;
  }
  std::string line;
  while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;
    if(prefix == "v"){
      Vec3f vertex;
      ss>>vertex.x >> vertex.y >>vertex.z;
      mesh.vertices.push_back(vertex);
    }
    else if(prefix == "f"){
      Vec3f face;
      ss>>face.x >>face.y>>face.z;
      mesh.faces.push_back(face);
    }
  }
  
  file.close();
  return mesh;
}

  Matrix Mesh::createTranslateMatrix(const Vec3f& t){
  Matrix m =  Matrix::identity();
  for(int i=3;i--;){
    m[i][3] = t[i];
  }
  return m;
}

Matrix Mesh::createScaleMatrix(const Vec3f& s){
  Matrix m =  Matrix::identity();
  for(int i=3;i--;){
    m[i][i] = s[i];
  }
  return m;
}


Matrix Mesh::createRotateMatrix(const Vec3f& angles) {
    // Convert angles from degrees to radians
    float x_angle = angles.x * M_PI / 180.0f;
    float y_angle = angles.y * M_PI / 180.0f;
    float z_angle = angles.z * M_PI / 180.0f;

    // Create rotation matrices for each axis
    Matrix rx = Matrix::identity();
    rx[1][1] = std::cos(x_angle);
    rx[1][2] = -std::sin(x_angle);
    rx[2][1] = std::sin(x_angle);
    rx[2][2] = std::cos(x_angle);

    Matrix ry = Matrix::identity();
    ry[0][0] = std::cos(y_angle);
    ry[0][2] = std::sin(y_angle);
    ry[2][0] = -std::sin(y_angle);
    ry[2][2] = std::cos(y_angle);

    Matrix rz = Matrix::identity();
    rz[0][0] = std::cos(z_angle);
    rz[0][1] = -std::sin(z_angle);
    rz[1][0] = std::sin(z_angle);
    rz[1][1] = std::cos(z_angle);

    return rz * ry * rx;
}