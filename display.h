#pragma once
#include <SDL2/SDL.h>

#include <cstdint>

#include "geometry.h"
#include "image_view.hpp"

namespace rasterizer {
void clear(rasterizer::image_view const& color_buffer, color4ub const& color);
void draw(image_view& color_buffer, Vec3f v0, Vec3f v01, Vec3f v2,
          color4ub color);
}  // namespace rasterizer
class Renderer {
 public:
  Renderer();
  ~Renderer();

  bool initialize(int windowWidth, int windowheight);
  void shutdown();

  void present();

  void drawGrid(void);
  void drawLine(Vec3f p1, Vec3f p2);

  SDL_Window* mWindow;

 private:
  SDL_Renderer* mSDLRenderer;

  int windowWidth;
  int windowHeight;
};