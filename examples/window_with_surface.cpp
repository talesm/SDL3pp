#include <iostream>
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::chrono_literals;

static SDL::Point makeRandomPoint()
{
  Sint32 magnitude = 4 + SDL::rand(5); //[4-8]
  Sint32 xx = SDL::rand(magnitude * 2 + 1) - magnitude;
  Sint32 yy = magnitude - SDL::abs(xx);
  if (yy > 0 && SDL::rand(2) == 1) yy *= -1;
  return {xx, yy};
}

int main(int argc, char** argv)
{
  SDL::SDL init(SDL::INIT_VIDEO);
  constexpr SDL::Point WINDOW_SZ = {400, 400};
  SDL::Window window = SDL::Window::Create("Test", WINDOW_SZ);
  SDL::SurfaceRef screen = window->GetSurface();
  auto smileyImg = SDL::Surface::Load(
    std::format("{}../assets/smiley.bmp", SDL::GetBasePath()));

  constexpr int WW = 64;

  SDL::Point smileyPos{WINDOW_SZ.x / 2 - smileyImg->GetWidth() / 2,
                       WINDOW_SZ.y / 2 - smileyImg->GetHeight() / 2};

  SDL::Rect rects[] = {
    {10, 10, WW, WW},
    {WINDOW_SZ.x - 10 - WW, WINDOW_SZ.y - 10 - WW, WW, WW},
    {10, WINDOW_SZ.y - 10 - WW, WW, WW},
    {WINDOW_SZ.x - 10 - WW, 10, WW, WW},
  };

  SDL::Point speed[] = {
    makeRandomPoint(),
    makeRandomPoint(),
    makeRandomPoint(),
    makeRandomPoint(),
  };

  bool running = true;
  while (running) {
    while (auto ev = SDL::PollEvent()) {
      if (ev->type == SDL::EVENT_QUIT) { running = false; }
    }
    for (int i = 0; i < 4; i++) {
      auto& xx = rects[i].x;
      auto& yy = rects[i].y;
      xx += speed[i].x;
      yy += speed[i].y;
      if (xx < 0 || xx + rects[i].w >= WINDOW_SZ.x) speed[i].x *= -1;
      if (yy < 0 || yy + rects[i].h >= WINDOW_SZ.y) speed[i].y *= -1;
    }

    screen.Fill(SDL::Color{127, 0, 127});

    screen.Blit(smileyImg, {}, smileyPos);
    screen.FillRects(rects, SDL::Color{0, 255, 0});

    window->UpdateSurface();
    SDL::Delay(10ms);
  }

  return 0;
}
