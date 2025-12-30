#define SDL3PP_MAIN_USE_CALLBACKS 1
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::literals;

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  static SDL::Window InitAndCreateWindow()
  {
    SDL::SetAppMetadata("Example Template", "1.0", "com.example.template");
    SDL::Init(SDL::INIT_VIDEO);
    return SDL::CreateWindowAndRenderer(
      "examples/demo/template", windowSz, 0, nullptr);
  }

  SDL::Window window{InitAndCreateWindow()};
  SDL::RendererRef renderer{window.GetRenderer()};

  SDL::AppResult Iterate()
  {
    renderer.SetDrawColorFloat(SDL::FColor{.75f, .75f, .75f, 1.f});
    renderer.RenderClear();

    renderer.Present();
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main)
