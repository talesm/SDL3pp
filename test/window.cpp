#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "SDL3pp/init.hpp"

int
main(int argc, char** argv)
{
  SDL::Init init(SDL_INIT_VIDEO);
  if (!init) {
    SDL_Log("%s", SDL_GetError());
    return 1;
  }
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  if (!SDL_CreateWindowAndRenderer("Test", 400, 400, 0, &window, &renderer)) {
    SDL_Log("%s", SDL_GetError());
    return 1;
  }

  bool running = true;
  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_EVENT_QUIT) { running = false; }
    }
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  return 0;
}
