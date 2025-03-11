#include <iostream>
#include <SDL3/SDL_main.h>
#include "SDL3pp/SDL3pp.h"

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
  SDL::SDL init(SDL::INIT_VIDEO);
  if (!init) {
    SDL_Log("%s", SDL::GetError());
    return 1;
  }
  auto [window, renderer] = SDL::CreateWindowAndRenderer("Test", {400, 400});
  if (!window) {
    SDL_Log("%s", SDL::GetError());
    return 1;
  }
  bool running = true;
  while (running) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_EVENT_QUIT) { running = false; }
    }
    renderer.SetDrawColor(SDL::FColor{.5f, .5f, .5f, 1.f});
    renderer.RenderClear();
    renderer.SetDrawColor(SDL::FColor{0.f, 1.f, 0.f, 1.f});
    renderer.RenderFillRect(SDL::FRect{10, 10, 128, 128});

    renderer.Present();
    SDL::Delay(1ns);
  }

  return 0;
}
