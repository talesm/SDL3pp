#define SDL3PP_MAIN_USE_CLASS_CALLBACKS
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::literals;

struct Main : SDL::AppInterface
{
  static constexpr SDL::Point windowSz = {640, 480};

  SDL::Window window{SDL::CreateWindowAndRenderer("examples/demo/template",
                                                  windowSz,
                                                  0,
                                                  nullptr)};
  SDL::RendererRef renderer{window.GetRenderer()};

  SDL::AppResult Iterate() final
  {
    renderer.SetDrawColorFloat(SDL::FColor{.75f, .75f, .75f, 1.f});
    renderer.RenderClear();

    renderer.Present();
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CLASS_CALLBACKS(Main,
                              SDL::INIT_VIDEO,
                              "Example Template",
                              "1.0",
                              "com.example.template")
