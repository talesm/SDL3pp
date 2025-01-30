#include <iostream>
#include <SDL3/SDL_main.h>
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_timer.h"
#include "SDL3pp/init.hpp"
#include "SDL3pp/video.hpp"

int main(int argc, char** argv)
{
  SDL::Init init(SDL_INIT_VIDEO);
  if (!init) {
    SDL_Log("%s", SDL_GetError());
    return 1;
  }
  SDL::Window window{"Test", 400, 400};
  if (!window) {
    SDL_Log("%s", SDL_GetError());
    return 1;
  }
  SDL::SurfaceRef screen = window.GetSurface();

  bool running = true;
  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_EVENT_QUIT) { running = false; }
    }
    screen.Fill(0);
    window.UpdateSurface();
    SDL_Delay(1);
  }

  return 0;
}
