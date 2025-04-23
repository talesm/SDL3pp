/*
 * This example creates an SDL window and renderer, and then draws a streaming
 * texture to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from streaming-textures.c on SDL's examples
 */

#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_THIS_CLASS Main
#define SDL3PP_MAIN_USE_THIS_APPNAME "Example Renderer Streaming Textures"
#define SDL3PP_MAIN_USE_THIS_APPID "com.example.renderer-streaming-textures"

struct Main
{
  // Window size
  static constexpr SDL::Point windowSz = {640, 480};
  static constexpr SDL::Point textureSz = {150, 150};

  // Init library
  SDL::SDL init{SDL::INIT_VIDEO};

  // We will use this renderer to draw into this window every frame.
  SDL::Window window{"examples/renderer/streaming-textures", windowSz};
  SDL::Renderer renderer{window};
  SDL::Texture texture{renderer,
                       SDL::PIXELFORMAT_RGBA8888,
                       SDL::TEXTUREACCESS_STREAMING,
                       textureSz};

  SDL::AppResult Iterate()
  {
    const float now = SDL::ToSeconds(SDL::GetTicks());

    // we'll have some textures move around over a few seconds.
    const float direction = SDL::fmod(now, 2.0f) > 1.f ? 1.0f : -1.0f;
    const float scale = (SDL::fmod(now, 1.0f) - 0.5f) / 0.5f * direction;

    /* To update a streaming texture, you need to lock it first. This gets you
       access to the pixels. Note that this is considered a _write-only_
       operation: the buffer you get from locking might not actually have the
       existing contents of the texture, and you have to write to every locked
       pixel! */

    /* The texture lock is a surface but at same time it
     *
     */
    if (SDL::TextureLock surface = texture.Lock()) {
      surface.Fill(SDL::Color(0, 0, 0));
      SDL::Rect r{0,
                  int(((float)(textureSz.y * 0.9f)) * ((scale + 1.0f) / 2.0f)),
                  textureSz.x,
                  textureSz.y / 10};
      /* make a strip of the surface green */
      surface.FillRect(r, SDL::Color{0, 255, 0});
    }

    // as you can see, rendering draws over what was drawn before it.
    renderer.SetDrawColor(SDL::Color{66, 66, 66}); // gray
    renderer.RenderClear();                        // start with a blank canvas.

    SDL::FRect dst_rect{(windowSz.x - textureSz.x) / 2.f,
                        (windowSz.y - textureSz.y) / 2.f,
                        float(textureSz.x),
                        float(textureSz.y)};
    renderer.RenderTexture(texture, std::nullopt, dst_rect);

    renderer.Present();       // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

#include <SDL3pp/SDL3pp_main.h>
