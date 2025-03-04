#include <iostream>
#include <SDL3/SDL_main.h>
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_timer.h"
#include "SDL3pp/init.hpp"
#include "SDL3pp/timer.hpp"
#include "SDL3pp/video.hpp"

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
  SDL::SDL init(SDL_INIT_VIDEO);
  if (!init) {
    SDL_Log("%s", SDL::GetError());
    return 1;
  }
  SDL::Window window{"Test", 400, 400};
  if (!window) {
    SDL_Log("%s", SDL::GetError());
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
    SDL::Delay(1ns);
  }

  return 0;
}
