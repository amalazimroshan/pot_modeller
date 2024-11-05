#pragma once
#include <SDL2/SDL.h>
#include "image_view.hpp"
#include "geometry.h"

namespace rasterizer{
  void clear(rasterizer::image_view const& color_buffer, color4ub const& color);
}
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