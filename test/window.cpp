#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "SDL3pp/init.hpp"

int main(int argc, char** argv)
{
  SDL::Init init(SDL_INIT_VIDEO);
  if (!init) {
    SDL_Log("%s", SDL_GetError());
    return 1;
  }
  SDL_Window* window = SDL_CreateWindow("Test", 400, 400, 0);
  if (!window) {
    SDL_Log("%s", SDL_GetError());
    return 1;
  }
  SDL_Surface* screen = SDL_GetWindowSurface(window);

  bool running = true;
  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_EVENT_QUIT) { running = false; }
    }
    SDL_FillSurfaceRect(screen, nullptr, 0);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(1);
  }
  SDL_DestroyWindow(window);

  return 0;
}
