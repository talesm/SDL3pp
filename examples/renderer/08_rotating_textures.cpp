/*
 * This example creates an SDL window and renderer, and then draws some
 * rotated textures to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from rotating_textures.c on SDL's examples
 */
#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  SDL::SDL init{SDL::INIT_VIDEO};
  SDL::Window window{"examples/renderer/rotating-textures", windowSz};
  SDL::Renderer renderer{window};

  /* Textures are pixel data that we upload to the video hardware for fast
    drawing. Lots of 2D engines refer to these as "sprites." We'll do a static
    texture (upload once, draw many times) with data from a bitmap file. */
  SDL::Texture texture{
    renderer,
    std::format("{}../assets/sample.bmp", SDL::GetBasePath())};

  SDL::AppResult Iterate()
  {
    const float now = SDL::ToSeconds(SDL::GetTicks());
    // we'll have a texture rotate around over 2 seconds (2000 milliseconds).
    // 360 degrees in a circle!
    const float rotation = SDL::fmod(now, 2.f) * 360.0f;

    // as you can see, rendering draws over what was drawn before it.
    renderer.SetDrawColor(SDL::Color{0, 0, 0}); // black
    renderer.RenderClear();                     // start with a blank canvas.

    // Center this one, and draw it with some rotation so it spins!
    SDL::FRect dst_rect = {(windowSz - texture.GetSize()) / 2.f,
                           texture.GetSize()};

    // rotate it around the center of the texture; you can rotate it from a
    // different point, too!
    SDL::FPoint center = texture.GetSize() / 2.f;
    renderer.RenderTextureRotated(texture, {}, dst_rect, rotation, center);

    renderer.Present();       // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

SDL3PP_DEFINE_CALLBACKS(Main,
                        "Example Renderer Rotating Textures",
                        "1.0",
                        "com.example.renderer-rotating-textures")
