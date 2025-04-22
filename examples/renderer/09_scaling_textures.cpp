/*
 * This example creates an SDL window and renderer, and then draws some
 * textures to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from scalling-textures.c on SDL's examples
 */
#include <SDL3pp/SDL3pp.h>

using namespace std::chrono_literals;

struct Main
{
  static SDL::AppResult Init(Main** state, SDL::AppArgs args)
  {
    SDL::SetAppMetadata("Example Renderer Scaling Textures",
                        "1.0",
                        "com.example.renderer-scaling-textures");
    return SDL::DefaultCreateClass(state, args);
  }

  static constexpr SDL::Point windowSz = {640, 480};

  SDL::SDL init{SDL::INIT_VIDEO};
  SDL::Window window{"examples/renderer/scaling-textures", windowSz};
  SDL::Renderer renderer{window};

  /* Textures are pixel data that we upload to the video hardware for fast
    drawing. Lots of 2D engines refer to these as "sprites." We'll do a static
    texture (upload once, draw many times) with data from a bitmap file. */
  SDL::Texture texture{renderer,
                       std::format("{}/assets/sample.bmp", SDL::GetBasePath())};

  SDL::AppResult Iterate()
  {
    const float now = SDL::ToSeconds(SDL::GetTicks());
    // we'll have the texture grow and shrink over a few seconds.
    const float direction = (SDL::fmod(now, 2.f) >= 1) ? 1.f : -1.f;
    const float scale = (SDL::fmod(now, 1.f) - 0.5f) * direction + 1.f;

    // as you can see, rendering draws over what was drawn before it.
    renderer.SetDrawColor(SDL::Color{0, 0, 0}); // black
    renderer.RenderClear();                     // start with a blank canvas.

    const SDL::FPoint textureSz = texture.GetSize() * scale;
    // Center this one and make it grow and shrink.
    SDL::FRect dst_rect = {(SDL::FPoint(windowSz) - textureSz) / 2.f,
                           textureSz};

    // rotate it around the center of the texture; you can rotate it from a
    // different point, too!
    SDL::FPoint center = textureSz / 2.f;
    renderer.RenderTexture(texture, {}, dst_rect);

    renderer.Present();       // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

#define SDL3PP_MAIN_USE_THIS_CLASS Main
#include <SDL3pp/SDL3pp_main.h>
