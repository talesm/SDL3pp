#include <SDL3pp/SDL3pp.h>

using namespace std::chrono_literals;

struct Main
{
  static constexpr SDL::Point windowSz = {400, 400};

  SDL::SDL init;
  SDL::Window window;
  SDL::Renderer renderer;

  Main(int, char**)
    : init(SDL::INIT_VIDEO)
  {
    if (!init) throw std::runtime_error{SDL::GetError()};
    std::tie(window, renderer) = SDL::CreateWindowAndRenderer("Test", windowSz);
    if (!window) throw std::runtime_error{SDL::GetError()};
  }

  SDL::AppResult Iterate()
  {
    renderer.SetDrawColor(SDL::FColor{.75f, .75f, .75f, 1.f});
    renderer.RenderClear();
    renderer.SetDrawColor(SDL::FColor{0.f, 0.725f, 0.f, 1.f});
    renderer.RenderFillRect(SDL::FRect{10, 10, 380, 380});

    renderer.Present();
    return SDL::APP_CONTINUE;
  }

  SDL::AppResult Event(const SDL::Event& ev)
  {
    switch (ev.type) {
    case SDL::EVENT_QUIT: return SDL::APP_SUCCESS;

    default: break;
    }
    return SDL::APP_CONTINUE;
  }
};

#define SDL3PP_MAIN_USE_THIS_CLASS Main
#include <SDL3pp/SDL3pp_main.h>
