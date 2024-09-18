#include <SDL2/SDL.h>

#include "Renderer.h"

int main() {
  int screen_width = 800, screen_height = 600;
  Renderer renderer;
  renderer.Initialize();

  bool isRunning = true;
  SDL_Event event;
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

    const int num_circles = 100;
    const int num_segments = 100;

    for (int i = 0; i < num_circles; ++i) {
      float phi = 2.0f * M_PI * float(i) / float(num_circles);

      for (int j = 0; j < num_segments; ++j) {
        float theta = 2.0f * M_PI * float(j) / float(num_segments);

        float x = (R1 * cos(theta) + R2) * cos(phi);
        float y = R1 * sin(theta);
        float z = (R1 * cos(theta) + R2) * sin(phi);

        int screenX = static_cast<int>((x * K1) / (K2 + z)) + screen_width / 2;
        int screenY = static_cast<int>((y * K1) / (K2 + z)) + screen_height / 2;

        renderer.DrawPoint(screenX, screenY);
      }
    }
    renderer.Present();
  }
  renderer.Shutdown();
  return 0;
}