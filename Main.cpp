#include <SDL2/SDL.h>

#include <algorithm>
#include <cmath>
#include <limits>

#include "Renderer.h"
#include "Vec.h"

struct Face {
  Vec3f v1, v2, v3;
};

Vec3f project(const Vec3f& v, float K1, float K2, int screen_width,
              int screen_height) {
  float x = ((v.x * K1) / (K2 + v.z)) + screen_width / 2.0f;
  float y = ((v.y * K1) / (K2 + v.z)) + screen_height / 2.0f;
  return Vec3f(x, y, v.z);
}

Vec3f barycentric(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2,
                  const Vec3f& p) {
  Vec3f s[2];
  for (int i = 2; i--;) {
    s[i][0] = v2[i] - v0[i];
    s[i][1] = v1[i] - v0[i];
    s[i][2] = v0[i] - p[i];
  }
  Vec3f u = cross(s[0], s[1]);
  if (std::abs(u[2]) > 1e-1)
    return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
  return Vec3f(-1, 1, 1);
}

void barycentric_trianglefill(Renderer* renderer, float* zbuffer,
                              int screen_width, int screen_height,
                              const Vec3f& v0, const Vec3f& v1, const Vec3f& v2,
                              Uint32 edgeColor, Uint32 faceColor) {
  Vec2i bboxmin = {
      static_cast<int>(std::max(0.0f, std::min({v0.x, v1.x, v2.x}))),
      static_cast<int>(std::max(0.0f, std::min({v0.y, v1.y, v2.y})))};
  Vec2i bboxmax = {
      static_cast<int>(std::min(static_cast<float>(screen_width - 1),
                                std::max({v0.x, v1.x, v2.x}))),
      static_cast<int>(std::min(static_cast<float>(screen_height - 1),
                                std::max({v0.y, v1.y, v2.y})))};

  Vec3f p;
  const float EPSILON = 0.03;
  for (p.x = bboxmin.x; p.x <= bboxmax.x; p.x++) {
    for (p.y = bboxmin.y; p.y <= bboxmax.y; p.y++) {
      Vec3f bc_screen = barycentric(v0, v1, v2, p);
      if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
      p.z = bc_screen.x * v0.z + bc_screen.y * v1.z + bc_screen.z * v2.z;
      int index = int(p.x + p.y * screen_width);
      if (zbuffer[index] < p.z) {
        zbuffer[index] = p.z;

        bool isEdge = (bc_screen.x < EPSILON || bc_screen.y < EPSILON ||
                       bc_screen.z < EPSILON);

        Uint32 color = isEdge ? edgeColor : faceColor;
        renderer->DrawPoint(static_cast<int>(p.x), static_cast<int>(p.y),
                            color);
      }
    }
  }
}

void drawTriangle(Renderer& renderer, float* zbuffer, const Face& face,
                  float K1, float K2, int screen_width, int screen_height) {
  Vec3f p1 = project(face.v1, K1, K2, screen_width, screen_height);
  Vec3f p2 = project(face.v2, K1, K2, screen_width, screen_height);
  Vec3f p3 = project(face.v3, K1, K2, screen_width, screen_height);

  barycentric_trianglefill(&renderer, zbuffer, screen_width, screen_height,
  p1,
                           p2, p3, 0xFFFFFF00, 0xFF023047);

  // renderer.DrawLine(static_cast<int>(p1.x), static_cast<int>(p1.y),
  //                   static_cast<int>(p2.x), static_cast<int>(p2.y));
  // renderer.DrawLine(static_cast<int>(p2.x), static_cast<int>(p2.y),
  //                   static_cast<int>(p3.x), static_cast<int>(p3.y));
  // renderer.DrawLine(static_cast<int>(p3.x), static_cast<int>(p3.y),
  //                   static_cast<int>(p1.x), static_cast<int>(p1.y));
}
Vec3f rotatePointZ(const Vec3f& p, float angle) {
  return Vec3f(p.x * cos(angle) - p.y * sin(angle),
               p.x * sin(angle) + p.y * cos(angle), p.z);
}
Vec3f rotatePointY(const Vec3f& p, float angle) {
  return Vec3f(p.x * cos(angle) + p.z * sin(angle), p.y,
               -p.x * sin(angle) + p.z * cos(angle));
}
Vec3f rotatePointX(const Vec3f& p, float angle) {
  return Vec3f(p.x, p.y * cos(angle) - p.z * sin(angle),
               p.y * sin(angle) + p.z * cos(angle));
}
Vec3f rotatePoint(const Vec3f& p, float rotateX, float rotateY, float rotateZ) {
  Vec3f rotated = rotatePointX(p, rotateX);
  rotated = rotatePointY(rotated, rotateY);
  rotated = rotatePointZ(rotated, rotateZ);
  return rotated;
}
int main() {
  int screen_width = 800, screen_height = 600;
  Renderer renderer;
  renderer.Initialize();

  bool isRunning = true;
  SDL_Event event;

  float* zbuffer = new float[screen_width * screen_height];
  float rotateX = 0.0f;
  float rotateY = 0.0f;
  float rotateZ = 0.0f;

  while (isRunning) {
    // Initialize z-buffer
    std::fill(zbuffer, zbuffer + screen_width * screen_height,
              std::numeric_limits<float>::lowest());

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) isRunning = false;

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
        if (event.key.keysym.sym == SDLK_a) rotateY += 0.02;
        if (event.key.keysym.sym == SDLK_s) rotateX -= 0.02;
        if (event.key.keysym.sym == SDLK_d) rotateZ -= 0.02;
      }
    }
    renderer.Clear();

    const float R1 = 100.f;
    const float R2 = 200.f;
    const float K2 = 650;
    const float K1 = screen_width * K2 * 3 / (8 * (R1 + R2));

    const int num_circles = 40;
    const int num_segments = 20;

    // rotateX += 0.01f;
    // rotateY += 0.01f;
    // rotateZ += 0.01f;

    for (int i = 0; i < num_circles; ++i) {
      float phi = 2.0f * M_PI  / float(num_circles);
      // float phi1 = 2.0f * M_PI * float(i + 1) / float(num_circles);

      for (int j = 0; j < num_segments; ++j) {
        float theta = 2.0f * M_PI  / float(num_segments);
        // float theta1 = 2.0f * M_PI * float(j + 1) / float(num_segments);

        Vec3f v1 = {(R1 * cos(theta* float(j)) + R2) * cos(phi* float(i)), R1 * sin(theta* float(j)),
                    (R1 * cos(theta* float(j)) + R2) * sin(phi* float(i))};
        Vec3f v2 = {(R1 * cos(theta* float(j)) + R2) * cos(phi* float(i+1)), R1 * sin(theta* float(j)),
                    (R1 * cos(theta* float(j)) + R2) * sin(phi* float(i+1))};
        Vec3f v3 = {(R1 * cos(theta* float(j+1)) + R2) * cos(phi* float(i)), R1 * sin(theta* float(j+1)),
                    (R1 * cos(theta* float(j+1)) + R2) * sin(phi* float(i))};
        Vec3f v4 = {(R1 * cos(theta* float(j+1)) + R2) * cos(phi* float(i+1)), R1 * sin(theta* float(j+1)),
                    (R1 * cos(theta* float(j+1)) + R2) * sin(phi* float(i+1))};

        v1 = rotatePoint(v1, rotateX, rotateY, rotateZ);
        v2 = rotatePoint(v2, rotateX, rotateY, rotateZ);
        v3 = rotatePoint(v3, rotateX, rotateY, rotateZ);
        v4 = rotatePoint(v4, rotateX, rotateY, rotateZ);

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

  delete[] zbuffer;
  renderer.Shutdown();
  return 0;
}