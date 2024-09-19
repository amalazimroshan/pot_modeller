#include <SDL2/SDL.h>

#include "Renderer.h"
#include "Vec.h"
struct Face {
  Vec3<float> v1, v2, v3;
};

Vec2<int> project(const Vec3<float> v, float K1, float K2, int screen_width,
                  int screen_height) {
  int x = static_cast<int>((v.x * K1) / (K2 + v.z)) + screen_width / 2;
  int y = static_cast<int>((v.y * K1) / (K2 + v.z)) + screen_height / 2;
  return Vec2<int>(x, y);
}

void drawTriangle(Renderer& renderer, float* zbuffer, const Face& face,
                  float K1, float K2, int screen_width, int screen_height) {
  Vec2<int> p1 = project(face.v1, K1, K2, screen_width, screen_height);
  Vec2<int> p2 = project(face.v2, K1, K2, screen_width, screen_height);
  Vec2<int> p3 = project(face.v3, K1, K2, screen_width, screen_height);

  renderer.DrawLine(p1.x, p1.y, p2.x, p2.y);
  renderer.DrawLine(p2.x, p2.y, p3.x, p3.y);
  renderer.DrawLine(p3.x, p3.y, p1.x, p1.y);
}

int main() {
  int screen_width = 800, screen_height = 600;
  Renderer renderer;
  renderer.Initialize();

  bool isRunning = true;
  SDL_Event event;

  float* zbuffer = new float[screen_width * screen_height];

  while (isRunning) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          isRunning = false;
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE) {
            isRunning = false;
          }
        default:
          break;
      }
    }
    renderer.Clear();

    const float R1 = 100.f;
    const float R2 = 200.f;
    const float K2 = 650;
    const float K1 = screen_width * K2 * 3 / (8 * (R1 + R2));

    const int num_circles = 40;
    const int num_segments = 20;

    Vec3<float> light_dir = {0.0f, -1.0f, -1.0f};

    for (int i = 0; i < num_circles; ++i) {
      float phi = 2.0f * M_PI * float(i) / float(num_circles);
      float phi1 = 2.0f * M_PI * float(i + 1) / float(num_circles);

      for (int j = 0; j < num_segments; ++j) {
        float theta = 2.0f * M_PI * float(j) / float(num_segments);
        float theta1 = 2.0f * M_PI * float(j + 1) / float(num_segments);

        // (i,j)
        Vec3<float> v1 = {(R1 * cos(theta) + R2) * cos(phi), R1 * sin(theta),
                          (R1 * cos(theta) + R2) * sin(phi)};

        //(i+1,j)
        Vec3<float> v2 = {(R1 * cos(theta) + R2) * cos(phi1), R1 * sin(theta),
                          (R1 * cos(theta) + R2) * sin(phi1)};

        //(i,j+1)
        Vec3<float> v3 = {(R1 * cos(theta1) + R2) * cos(phi), R1 * sin(theta1),
                          (R1 * cos(theta1) + R2) * sin(phi)};

        //(i+1,j+1)
        Vec3<float> v4 = {(R1 * cos(theta1) + R2) * cos(phi1), R1 * sin(theta1),
                          (R1 * cos(theta1) + R2) * sin(phi1)};

        Face f1 = {v1, v2, v3};
        Face f2 = {v2, v4, v3};

        drawTriangle(renderer, zbuffer, f1, K1, K2, screen_width,
                     screen_height);
        drawTriangle(renderer, zbuffer, f2, K1, K2, screen_width,
                     screen_height);
      }
    }
    renderer.Present();
  }
  renderer.Shutdown();
  return 0;
}