#include <iostream>
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
  SDL::SDL init(SDL::INIT_VIDEO);
  constexpr SDL::Point WINDOW_SZ = {400, 400};
  auto [window, renderer] = SDL::CreateWindowAndRenderer("Test", WINDOW_SZ);

  SDL::Texture characterTexture{
    renderer, std::format("{}../assets/smiley.png", SDL::GetBasePath())};
  SDL::FRect characterRect(SDL::FPoint(WINDOW_SZ) / 2 - SDL::FPoint{64, 64},
                           {128, 128});

  bool running = true;
  while (running) {
    while (auto ev = SDL::PollEvent()) {
      if (ev->type == SDL::EVENT_QUIT) { running = false; }
    }
    renderer.SetDrawColor(SDL::FColor{.5f, .5f, .5f, 1.f});
    renderer.RenderClear();
    renderer.SetDrawColor(SDL::FColor{0.f, 0.725f, 0.f, 1.f});
    renderer.RenderFillRect(SDL::FRect{10, 10, 380, 380});
    renderer.RenderTexture(characterTexture, {}, characterRect);

    renderer.Present();
    SDL::Delay(1ns);
  }

  return 0;
}
