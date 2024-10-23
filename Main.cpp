#include <SDL2/SDL.h>
#include <geometry.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

#include "display.h"
#include "mesh.h"

int screen_width = 800, screen_height = 600, depth = 255;
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

int main() {
  Renderer renderer;
  renderer.initialize();

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

    // renderer.drawGrid();
    Matrix Projection = Matrix::identity(4);
    Projection[3][2] = -1.f / camera.z;
    Matrix ViewPort = viewPort(screen_width / 8, screen_height / 8,
                               screen_width * 3 / 4, screen_height * 3 / 4);
    Mesh cube;
    cube.addCube(cube);
    for (const auto& face : cube.faces) {
      for (int i = 0; i < 3; i++) {
        Vec3f v0 =
            Vec3f(ViewPort * Projection * Matrix(cube.vertices[face[i]]));
        Vec3f v1 = Vec3f(ViewPort * Projection *
                         Matrix(cube.vertices[face[(i + 1) % 3]]));
        renderer.drawLine(v0, v1);
      }
    }
    renderer.present();
  }

  renderer.shutdown();
  return 0;
}