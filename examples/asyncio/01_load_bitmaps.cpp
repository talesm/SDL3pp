#define SDL3PP_MAIN_USE_CALLBACKS 1
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::literals;

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  static SDL::AppResult Init(Main** m, SDL::AppArgs args)
  {
    SDL::SetAppMetadata("Example Asyncio load bitmaps",
                        "1.0",
                        "com.example.asyncio-load-bitmaps");
    SDL::Init(SDL::INIT_VIDEO);
    *m = new Main();
    return SDL::APP_CONTINUE;
  }

  SDL::Window window{"examples/asyncio/load-bitmaps", windowSz};
  SDL::Renderer renderer{window};
  SDL::AsyncIOQueue queue;

  static constexpr std::string_view bmps[] = {"sample.bmp",
                                              "gamepad_front.bmp",
                                              "speaker.bmp",
                                              "icon2x.bmp"};
  static constexpr size_t TOTAL_TEXTURES = std::size(bmps);
  std::array<SDL::Texture, TOTAL_TEXTURES> textures;
  SDL::FRect texture_rects[TOTAL_TEXTURES] = {
    {116, 156, 408, 167},
    {20, 200, 96, 60},
    {525, 180, 96, 96},
    {288, 375, 64, 64},
  };

  Main()
  {
    int i = 0;
    for (auto bmp : bmps) {
      auto path = std::format("{}../assets/{}", SDL::GetBasePath(), bmp);
      SDL::LoadFileAsync(path, queue, reinterpret_cast<void*>(i++));
    }
  }

  SDL::AppResult Iterate()
  {
    if (auto outcome = queue.GetResult()) {
      if (outcome->result == SDL::ASYNCIO_COMPLETE) {
        size_t i = reinterpret_cast<size_t>(outcome->userdata);
        if (i < TOTAL_TEXTURES) { // (just in case.)
          SDL::Surface surface{
            SDL::IOFromConstMem({outcome->buffer, outcome->bytes_transferred})};
          // the renderer is not multithreaded, so create the texture here once
          // the data loads
          textures[i] = SDL::CreateTextureFromSurface(renderer, surface);
        }
      }
      SDL::free(outcome->buffer);
    }

    renderer.SetDrawColor({0, 0, 0});
    renderer.RenderClear();

    for (size_t i = 0; i < TOTAL_TEXTURES; i++) {
      if (!textures[i]) continue;
      renderer.RenderTexture(textures[i], {}, texture_rects[i]);
    }

    renderer.Present();
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main)
