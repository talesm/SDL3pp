#include <SDL3pp/SDL3pp.h>

using namespace std::chrono_literals;

struct Main
{
  static constexpr SDL::Point WINDOW_SZ = {400, 400};
  static constexpr SDL::FRect characterRect{SDL::FPoint(WINDOW_SZ) / 2 -
                                              SDL::FPoint{64, 64},
                                            {128, 128}};

  SDL::SDL init;
  SDL::Window window;
  SDL::Renderer renderer;
  SDL::Texture characterTexture;

  Main(int, char**)
    : init(SDL::INIT_VIDEO)
  {
    if (!init) throw std::runtime_error{SDL::GetError()};
    std::tie(window, renderer) =
      SDL::CreateWindowAndRenderer("Test", {400, 400});
    if (!window) throw std::runtime_error{SDL::GetError()};
    characterTexture = {renderer, "assets/smiley.png"};
    if (!characterTexture) throw std::runtime_error{SDL::GetError()};
  }

  // SDL::AppResult Init(int argc, char* argv[]) {}

  SDL::AppResult Iterate()
  {
    renderer.SetDrawColor(SDL::FColor{.5f, .5f, .5f, 1.f});
    renderer.RenderClear();
    renderer.SetDrawColor(SDL::FColor{0.f, 0.725f, 0.f, 1.f});
    renderer.RenderFillRect(SDL::FRect{10, 10, 380, 380});
    renderer.RenderTexture(characterTexture, {}, characterRect);

    renderer.Present();
    return SDL::APP_CONTINUE;
  }

  SDL::AppResult Event(const SDL::Event& ev)
  {
    if (ev.type == SDL::EVENT_QUIT) return SDL::APP_SUCCESS;
    return SDL::APP_CONTINUE;
  }

  void Quit(SDL::AppResult) { SDL::Log("Bye"); }
};

#define SDL3PP_MAIN_USE_THIS_CLASS Main
#include <SDL3pp/SDL3pp_main.h>
