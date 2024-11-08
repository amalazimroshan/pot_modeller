#include <algorithm>
#include <cmath>
#include <renderlite/display.hpp>
#include <renderlite/image_view.hpp>

Renderer::Renderer()
    : mWindow(nullptr),
      mSDLRenderer(nullptr),
      windowWidth(1200),
      windowHeight(800) {}

Renderer::~Renderer() {}

bool Renderer::initialize(int windowWidth, int windowHeight) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }
  mWindow = SDL_CreateWindow("Physics Simulation", 100, 100, windowWidth,
                             windowHeight, 0);
  if (!mWindow) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  mSDLRenderer = SDL_CreateRenderer(
      mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!mSDLRenderer) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }

  return true;
}

void Renderer::shutdown() {
  SDL_DestroyRenderer(mSDLRenderer);
  SDL_DestroyWindow(mWindow);
}

void clear(rasterizer::image_view const& color_buffer, color4ub const& color) {
  auto ptr = color_buffer.pixels;
  auto size = color_buffer.height * color_buffer.width;
  std::fill(ptr, ptr + size, color);
}

namespace rasterizer {
void draw(image_view& color_buffer, float* zbuffer, Vec3f v0, Vec3f v1,
          Vec3f v2, color4ub color) {
  std::int32_t xmin = std::max<float>(
      0, std::min({std::floor(v0.x), std::floor(v1.x), std::floor(v2.x)}));
  std::int32_t xmax = std::min<float>(
      color_buffer.width - 1,
      std::max({std::floor(v0.x), std::floor(v1.x), std::floor(v2.x)}));
  std::int32_t ymin = std::max<float>(
      0, std::min({std::floor(v0.y), std::floor(v1.y), std::floor(v2.y)}));
  std::int32_t ymax = std::min<float>(
      color_buffer.height - 1,
      std::max({std::floor(v0.y), std::floor(v1.y), std::floor(v2.y)}));

  float det012 = det2D(v1 - v0, v2 - v0);
  bool const ccw = det012 < 0.f;
  if (ccw) {
    std::swap(v1, v2);
    det012 = -det012;
  }
  for (std::int32_t y = ymin; y <= ymax; ++y) {
    for (std::int32_t x = xmin; x <= xmax; ++x) {
      Vec3f p{x + .5f, y + .5f, 0.f};

      float det01p = det2D(v1 - v0, p - v0);
      float det12p = det2D(v2 - v1, p - v1);
      float det20p = det2D(v0 - v2, p - v2);

      if (det01p >= 0.f && det12p >= 0.f && det20p >= 0) {
        // float z = det01p * v0.z + det12p * v1.z + det20p * v2.z;
        float z = (det01p * v0.z + det12p * v1.z + det20p * v2.z) / det012;
        int index = int(x + y * 1200);
        if (z < zbuffer[index]) {
          zbuffer[index] = z;
          color_buffer.at(x, y) = color;
        }
      }
    }
  }
}
}  // namespace rasterizer

void Renderer::present() { SDL_RenderPresent(mSDLRenderer); }

void Renderer::drawGrid(void) {
  int gridSize = 10;
  uint32_t gridColor = 0xFF444444;

  for (int y = 0; y < windowHeight; y += gridSize) {
    for (int x = 0; x < windowWidth; x += gridSize) {
      SDL_SetRenderDrawColor(mSDLRenderer, 40, 40, 40, 255);
      SDL_RenderDrawPoint(mSDLRenderer, x, y);
    }
  }
}

void Renderer::drawLine(Vec3f p1, Vec3f p2) {
  SDL_SetRenderDrawColor(mSDLRenderer, 255, 255, 255, 255);
  SDL_RenderDrawLine(mSDLRenderer, p1.x, p1.y, p2.x, p2.y);
}