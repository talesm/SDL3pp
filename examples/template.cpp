#include <SDL3pp/SDL3pp.h>

using namespace std::chrono_literals;

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  SDL::SDL init{SDL::INIT_VIDEO};
  SDL::Window window{"Test", windowSz};
  SDL::Renderer renderer{window};

  SDL::AppResult Iterate()
  {
    renderer.SetDrawColor(SDL::FColor{.75f, .75f, .75f, 1.f});
    renderer.RenderClear();
    renderer.SetDrawColor(SDL::FColor{0.f, 0.725f, 0.f, 1.f});
    renderer.RenderFillRect(SDL::FRect{10, 10, 620, 460});

    renderer.Present();
    return SDL::APP_CONTINUE;
  }
};

#define SDL3PP_MAIN_USE_THIS_CLASS Main
#include <SDL3pp/SDL3pp_main.h>
