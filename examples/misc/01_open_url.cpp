#include <iostream>
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
  SDL::Init(SDL::INIT_VIDEO);
  constexpr SDL::Point WINDOW_SZ = {400, 400};
  auto [window, renderer] = SDL::CreateWindowAndRenderer("Test", {400, 400});
  const char clickString[] = "Click anywhere to open URL";
  SDL::FPoint stringPos =
    (SDL::FPoint(WINDOW_SZ) -
     SDL::FPoint{SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * sizeof(clickString),
                 SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE}) /
    2;

  bool running = true;
  while (running) {
    while (auto ev = SDL::PollEvent()) {
      switch (ev->type) {
      case SDL::EVENT_QUIT: running = false; break;
      case SDL::EVENT_MOUSE_BUTTON_DOWN:
        SDL::OpenURL("https://talesm.github.io/SDL3pp");
        break;
      default: break;
      }
    }
    renderer.SetDrawColorFloat({.5f, .5f, .5f, 1.f});
    renderer.RenderClear();
    renderer.SetDrawColorFloat({0.f, 0.725f, 0.f, 1.f});
    renderer.RenderFillRect(SDL::FRect{10, 10, 380, 380});
    renderer.SetDrawColorFloat({1.f, 0.125f, 1.f, 1.f});
    renderer.RenderDebugText(stringPos, clickString);

    renderer.Present();
    SDL::Delay(1ns);
  }

  return 0;
}
