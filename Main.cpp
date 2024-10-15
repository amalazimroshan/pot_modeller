#include <SDL2/SDL.h>
#include <geometry.h>

#include <algorithm>
#include <cmath>
#include <limits>

#include "Renderer.h"

struct Face {
  Vec3f v1, v2, v3;
};

int main() {
  int screen_width = 800, screen_height = 600;
  Renderer renderer;
  renderer.Initialize();

  bool isRunning = true;
  SDL_Event event;

  while (isRunning) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) isRunning = false;

      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
      }
    }
    renderer.Clear();
    renderer.Present();
  }

  renderer.Shutdown();
  return 0;
}