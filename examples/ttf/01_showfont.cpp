#include <SDL3pp/SDL3pp.h>

using namespace std::chrono_literals;

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  SDL::SDL init{SDL::INIT_VIDEO, SDL::INIT_TTF};
  SDL::Window window{"Test", windowSz};
  SDL::Renderer renderer{window};
  SDL::Font font;
  std::string testString = "the quick brown fox jumps over the lazy dog\n"
                           "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG\n"
                           "0123456789!@#$%&*(){}[]\"'";
  SDL::Texture testTexture;
  SDL::FRect testRect;
  int ptSize = 20;
  int testQuality = 1;
  static constexpr int testQualityCount = 2;
  SDL::Color testColor{0, 0, 0};

  SDL::AppResult Iterate()
  {
    renderer.SetDrawColor(SDL::FColor{.75f, .75f, .75f, 1.f});
    renderer.RenderClear();

    if (testTexture) renderer.RenderTexture(testTexture, {}, testRect);

    renderer.Present();
    return SDL::APP_CONTINUE;
  }

  SDL::AppResult Event(const SDL::Event& ev)
  {
    switch (ev.type) {
    case SDL::EVENT_QUIT: return SDL::APP_SUCCESS;
    case SDL::EVENT_KEY_DOWN: {
      switch (ev.key.key) {
      case SDL::KEYCODE_COMMA:
        if (ptSize > 1) {
          --ptSize;
          if (font) font.SetSize(ptSize);
          refreshText();
        }
        break;
      case SDL::KEYCODE_PERIOD:
        if (ptSize < 50) {
          ++ptSize;
          if (font) font.SetSize(ptSize);
          refreshText();
        }
        break;
      case SDL::KEYCODE_SPACE:
        testQuality = (testQuality + 1) % testQualityCount;
        refreshText();
        break;
      default: break;
      }
      break;
    }
    case SDL::EVENT_DROP_FILE:
      font = SDL::Font(ev.drop.data, ptSize);
      refreshText();
      break;
    default: break;
    }
    return SDL::APP_CONTINUE;
  }

  void refreshText()
  {
    if (!font) return;
    SDL::Surface surface;
    switch (testQuality) {
    case 0:
      surface =
        font.RenderText_Solid_Wrapped(testString, testColor, windowSz.x);
      break;
    case 1:
      surface =
        font.RenderText_Blended_Wrapped(testString, testColor, windowSz.x);
      break;
    default: break;
    }
    testTexture = SDL::Texture{renderer, surface};
    testRect.w = testTexture.GetWidth();
    testRect.h = testTexture.GetHeight();
  }
};

#define SDL3PP_MAIN_USE_THIS_CLASS Main
#include <SDL3pp/SDL3pp_main.h>
