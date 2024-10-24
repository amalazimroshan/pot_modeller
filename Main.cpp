#include <SDL2/SDL.h>
#include <geometry.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

#include "display.h"
#include "mesh.h"

int screen_width = 1200, screen_height = 800, depth = 255;
Vec3f camera(0, 0, 3);

Matrix viewPort(int x, int y, int w, int h) {
  Matrix m = Matrix::identity(4);
  m[0][3] = x + w / 2.f;
  m[1][3] = y + h / 2.f;
  m[2][3] = depth / 2.f;

  m[0][0] = w / 2.f;
  m[1][1] = h / 2.f;
  m[2][2] = depth / 2.f;

  return m;
}

Matrix projection(float aspectRatio) {
  Matrix m = Matrix::identity(4);
  if (aspectRatio > 1.0f) {
    m[0][0] = 1.0f / aspectRatio;
  } else {
    m[1][1] = aspectRatio;
  }
  m[3][2] = -1.f / camera.z;
  return m;
}

int main() {
  Renderer renderer;
  renderer.initialize(screen_width, screen_height);
  bool isRunning = true;
  SDL_Event event;

  while (isRunning) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) isRunning = false;
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
      }
    }

    renderer.clear();

    float aspectRatio =
        static_cast<float>(screen_width) / static_cast<float>(screen_height);
    Matrix Projection = projection(aspectRatio);

    Matrix ViewPort = viewPort(0, 0, screen_width, screen_height);

    Mesh cube;
    cube.addCube(cube);

    float angle = SDL_GetTicks() / 1000.0f;  // Rotation based on time
    Matrix rotation = Matrix::identity(4);
    rotation[0][0] = cos(angle);
    rotation[0][2] = sin(angle);
    rotation[2][0] = -sin(angle);
    rotation[2][2] = cos(angle);

    for (const auto& face : cube.faces) {
      for (int i = 0; i < 3; i++) {
        Vec3f v0 = Vec3f(ViewPort * Projection * rotation *
                         Matrix(cube.vertices[face[i]]));
        Vec3f v1 = Vec3f(ViewPort * Projection * rotation *
                         Matrix(cube.vertices[face[(i + 1) % 3]]));
        renderer.drawLine(v0, v1);
      }
    }

    renderer.present();
  }

  renderer.shutdown();
  return 0;
}