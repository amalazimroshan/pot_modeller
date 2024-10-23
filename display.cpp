#include "display.h"

#include <cmath>

Renderer::Renderer()
    : mWindow(nullptr),
      mSDLRenderer(nullptr),
      windowWidth(1200),
      windowHeight(800) {}

Renderer::~Renderer() {}

bool Renderer::initialize() {
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

void Renderer::clear() {
  SDL_SetRenderDrawColor(mSDLRenderer, 0, 0, 0, 255);
  SDL_RenderClear(mSDLRenderer);
}

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