#pragma once
#include <SDL2/SDL.h>

#include "geometry.h"
class Renderer {
 public:
  Renderer();
  ~Renderer();

  bool initialize();
  void shutdown();
  void clear();
  void present();

  void drawGrid(void);
  void drawLine(Vec3f p1, Vec3f p2);

 private:
  SDL_Window* mWindow;
  SDL_Renderer* mSDLRenderer;

  int windowWidth;
  int windowHeight;
};