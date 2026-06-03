/*
 * This example creates an SDL window and renderer, and then draws some
 * textures to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from textures.c on SDL's examples
 */

#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CLASS_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main : SDL::AppInterface
{
  // Window size
  static constexpr SDL::Point windowSz = {640, 480};

  // We will use this renderer to draw into this window every frame.
  SDL::Window window{SDL::CreateWindowAndRenderer("examples/renderer/textures",
                                                  windowSz,
                                                  0,
                                                  nullptr)};
  SDL::RendererRef renderer = window.GetRenderer();

  /* Textures are pixel data that we upload to the video hardware for fast
     drawing. Lots of 2D engines refer to these as "sprites." We'll do a static
     texture (upload once, draw many times) with data from a bitmap file. */
  SDL::Texture texture{
    renderer,
    std::format("{}../assets/sample.png", SDL::GetBasePath())};

  SDL::AppResult Iterate() final
  {
    const float now = SDL::ToSeconds(SDL::GetTicks());

    // we'll have some textures move around over a few seconds.
    const float direction = SDL::fmod(now, 2.0f) > 1.f ? 1.0f : -1.0f;
    const float scale = (SDL::fmod(now, 1.0f) - 0.5f) / 0.5f * direction;

    // as you can see, rendering draws over what was drawn before it.
    renderer.SetDrawColor(SDL::Color{0, 0, 0}); // black
    renderer.RenderClear();                     // start with a blank canvas.

    SDL::FPoint texture_size = texture.GetSizeFloat();

    /* Just draw the static texture a few times. You can think of it like a
       stamp, there isn't a limit to the number of times you can draw with it.
     */

    SDL::FRect dst_rect;

    // top left
    dst_rect.x = (100.0f * scale);
    dst_rect.y = 0.0f;
    dst_rect.w = texture_size.x;
    dst_rect.h = texture_size.y;
    renderer.RenderTexture(texture, std::nullopt, dst_rect);

    //  center this one.
    dst_rect.x = (windowSz.x - texture_size.x) / 2.0f;
    dst_rect.y = (windowSz.y - texture_size.y) / 2.0f;
    dst_rect.w = texture_size.x;
    dst_rect.h = texture_size.y;
    renderer.RenderTexture(texture, std::nullopt, dst_rect);

    // bottom right
    dst_rect.x = windowSz.x - texture_size.x - (100.0f * scale);
    dst_rect.y = windowSz.y - texture_size.y;
    dst_rect.w = texture_size.x;
    dst_rect.h = texture_size.y;
    renderer.RenderTexture(texture, std::nullopt, dst_rect);

    renderer.Present();       // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

SDL3PP_DEFINE_CLASS_CALLBACKS(Main,
                              SDL::INIT_VIDEO,
                              "Example Renderer Textures",
                              "1.0",
                              "com.example.renderer-textures")
