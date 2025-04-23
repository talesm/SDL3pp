/*
 * This example creates an SDL window and renderer, and then draws some
 * textures to it every frame, adjusting the viewport.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from viewport.c on SDL's examples
 */
#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_THIS_CLASS Main
#define SDL3PP_MAIN_USE_THIS_APPNAME "Example Renderer Viewport"
#define SDL3PP_MAIN_USE_THIS_APPID "com.example.renderer-viewport"

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  SDL::SDL init{SDL::INIT_VIDEO};
  SDL::Window window{"examples/renderer/viewport", windowSz};
  SDL::Renderer renderer{window};

  /* Textures are pixel data that we upload to the video hardware for fast
    drawing. Lots of 2D engines refer to these as "sprites." We'll do a static
    texture (upload once, draw many times) with data from a bitmap file. */
  SDL::Texture texture{renderer,
                       std::format("{}/assets/sample.bmp", SDL::GetBasePath())};

  SDL::AppResult Iterate()
  {
    /* Setting a viewport has the effect of limiting the area that rendering
       can happen, and making coordinate (0, 0) live somewhere else in the
       window. It does _not_ scale rendering to fit the viewport. */

    // as you can see, rendering draws over what was drawn before it.
    renderer.SetDrawColor(SDL::Color{0, 0, 0}); // black
    renderer.RenderClear();                     // start with a blank canvas.

    SDL::FRect dst_rect = {{0, 0}, texture.GetSize()};

    // Draw once with the whole window as the viewport.
    renderer.ResetViewport();
    renderer.RenderTexture(texture, {}, dst_rect);

    // top right quarter of the window.
    renderer.SetViewport(SDL::Rect{windowSz / 2, windowSz / 2});
    renderer.RenderTexture(texture, {}, dst_rect);

    // bottom 20% of the window. Note it clips the width!
    renderer.SetViewport(
      SDL::Rect{{0, windowSz.y - (windowSz.y / 5)}, windowSz / 5});
    renderer.RenderTexture(texture, {}, dst_rect);

    // what happens if you try to draw above the viewport? It should clip!
    renderer.SetViewport(SDL::Rect{{100, 200}, windowSz});
    dst_rect.y -= 50;
    renderer.RenderTexture(texture, {}, dst_rect);

    renderer.Present();       // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

#include <SDL3pp/SDL3pp_main.h>
