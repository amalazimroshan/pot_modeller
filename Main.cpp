#include <SDL2/SDL.h>
#include <geometry.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <image_view.hpp>
#include <iostream>
#include <limits>

#include "display.h"
#include "mesh.h"

int screen_width = 1200, screen_height = 800, depth = 255;
Vec3f eye(0, 0, 20);
Vec3f center(0, 0, 0);

Matrix viewPort(int x, int y, int w, int h) {
  Matrix m = Matrix::identity();
  m[0][3] = x + w / 2.f;
  m[1][3] = y + h / 2.f;
  m[2][3] = depth / 2.f;

  m[0][0] = w / 2.f;
  m[1][1] = h / 2.f;
  m[2][2] = depth / 2.f;

  return m;
}

Matrix lookAt(Vec3f eye, Vec3f center, Vec3f up) {
  Vec3f z = (eye - center).normalize();
  Vec3f x = cross(up, z).normalize();
  Vec3f y = cross(z, x).normalize();
  Matrix res = Matrix::identity();
  for (int i = 0; i < 3; i++) {
    res[0][i] = x[i];
    res[1][i] = y[i];
    res[2][i] = z[i];
    res[i][3] = -eye[i];
  }
  return res;
}

Matrix projection(float aspectRatio) {
  Matrix m = Matrix::identity();
  if (aspectRatio > 1.0f) {
    m[0][0] = 1.0f / aspectRatio;
  } else {
    m[1][1] = aspectRatio;
  }
  m[3][2] = -1.f / (eye - center).norm();
  return m;
}

int main(int argc, char* argv[]) {
  Mesh mesh;
  if (argc < 2) {
    mesh = Mesh::createCube();
  } else {
    mesh = Mesh::createOBJ(argv[argc - 1]);
  }
  // Renderer renderer;
  // renderer.initialize(screen_width, screen_height);
  SDL_Window* window = SDL_CreateWindow(
      "tiny rasterizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      screen_width, screen_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
  SDL_Surface* draw_surface = nullptr;

  bool isRunning = true;
  SDL_Event event;

  Matrix ModelView = lookAt(eye, center, Vec3f(0, 1, 0));
  float aspectRatio =
      static_cast<float>(screen_width) / static_cast<float>(screen_height);
  Matrix Projection = projection(aspectRatio);
  Matrix ViewPort = viewPort(0, 0, screen_width, screen_height);

  using clock = std::chrono::high_resolution_clock;
  auto last_frame_start = clock::now();

  while (isRunning) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) isRunning = false;
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
      }
    }

    if (!draw_surface) {
      draw_surface = SDL_CreateRGBSurfaceWithFormat(
          0, screen_width, screen_height, 32, SDL_PIXELFORMAT_RGBA32);
      SDL_SetSurfaceBlendMode(draw_surface, SDL_BLENDMODE_NONE);
    }
    auto now = clock::now();
    float dt = std::chrono::duration_cast<std::chrono::duration<float>>(
                   now - last_frame_start)
                   .count();
    last_frame_start = now;
    std::cout << dt << std::endl;

    rasterizer::image_view color_buffer{
        .pixels = (color4ub*)draw_surface->pixels,
        .width = (std::uint32_t)screen_width,
        .height = (std::uint32_t)screen_height,
    };

    auto ptr = color_buffer.pixels;
    auto size = color_buffer.height * color_buffer.width;
    std::fill(ptr, ptr + size, color4ub{0, 28, 73, 255});

    float angle = SDL_GetTicks() / 100.0f;  // Rotation based on time
    Matrix scale = Mesh::createScaleMatrix(Vec3f(0.7, 0.7, 0.7));
    Matrix rotation = Mesh::createRotateMatrix(Vec3f(angle, angle, angle));
    Matrix translate = Mesh::createTranslateMatrix(Vec3f(0, -1, 0));
    Matrix Model = scale * rotation * translate;

    Matrix MVP = ViewPort * Projection * ModelView * Model;
    // for (const auto& face : mesh.faces) {
    for (std::size_t i = 0; i < mesh.faces.size(); ++i) {
      const auto& face = mesh.faces[i];
      Vec3f v0 = Vec3f(MVP * Matrix(mesh.vertices[face[0]]));
      Vec3f v1 = Vec3f(MVP * Matrix(mesh.vertices[face[1]]));
      Vec3f v2 = Vec3f(MVP * Matrix(mesh.vertices[face[2]]));
      const color4ub color = color4ub(120, i * 20, i * 20);
      rasterizer::draw(color_buffer, v0, v1, v2, color);
    }

    SDL_Rect rect{.x = 0, .y = 0, .w = screen_width, .h = screen_height};
    SDL_BlitSurface(draw_surface, &rect, SDL_GetWindowSurface(window), &rect);
    SDL_UpdateWindowSurface(window);
    // renderer.present();
  }
  // renderer.shutdown();
  return 0;
}