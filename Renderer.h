#pragma once
#include <SDL2/SDL.h>

class Renderer {
 public:
  Renderer();
  ~Renderer();

  bool Initialize();
  void Shutdown();

  void Clear();
  void Present();
  void DrawPoint(int x, int y);
  void DrawCircle(int centerX, int centerY, int radius);
  void DrawLine(int x1, int y1, int x2, int y2);

 private:
  SDL_Window *mWindow;
  SDL_Renderer *mSDLRenderer;
};