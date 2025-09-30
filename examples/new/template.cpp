#define SDL3PP_MAIN_USE_CALLBACKS 1
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::literals;

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  static SDL::AppResult Init(Main** m, SDL::AppArgs args)
  {
    SDL::SetAppMetadata("Example Template", "1.0", "com.example.template");
    SDL::Init(SDL::INIT_VIDEO);
    *m = new Main();
    return SDL::APP_CONTINUE;
  }

  SDL::Window window = SDL::Window("examples/demo/template", windowSz);
  SDL::Renderer renderer = SDL::Renderer(window);

  SDL::AppResult Iterate()
  {
    renderer.SetDrawColorFloat(SDL::FColor{.75f, .75f, .75f, 1.f});
    renderer.RenderClear();

    renderer.Present();
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main)
