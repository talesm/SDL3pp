/*
 * This example creates an SDL window and renderer, and then draws some
 * rectangles to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from rectangles.c on SDL's examples
 */

#include <SDL3pp/SDL3pp.h>

struct Main
{
  static SDL::AppResult Init(Main** state, SDL::AppArgs args)
  {
    SDL::SetAppMetadata(
      "Example Renderer Rectangles", "1.0", "com.example.renderer-rectangles");
    return SDL::DefaultCreateClass(state, args);
  }

  // Window size
  static constexpr SDL::Point windowSz = {640, 480};

  // Init library
  SDL::SDL init{SDL::INIT_VIDEO};

  // We will use this renderer to draw into this window every frame.
  SDL::Window window{"examples/renderer/rectangles", windowSz};
  SDL::Renderer renderer{window};

  // This function runs once at startup.
  Main()
  {
    if (!renderer) throw std::runtime_error{SDL::GetError()};
  }

  SDL::AppResult Iterate()
  {
    std::array<SDL::FRect, 16> rects;
    const float now = SDL::ToSeconds(SDL::GetTicks());

    // we'll have the rectangles grow and shrink over a few seconds.
    const float direction = SDL::fmod(now, 2.0f) > 1.f ? 1.0f : -1.0f;
    const float scale = (SDL::fmod(now, 1.0f) - 0.5f) / 0.5f * direction;

    // as you can see, rendering draws over what was drawn before it.
    renderer.SetDrawColor(SDL::Color{0, 0, 0}); // black
    renderer.RenderClear();                     // start with a blank canvas.

    /* Rectangles are comprised of set of X and Y coordinates, plus width and
      height. (0, 0) is the top left of the window, and larger numbers go
      down and to the right. This isn't how geometry works, but this is
      pretty standard in 2D graphics. */

    // Let's draw a single rectangle (square, really).
    rects[0].x = rects[0].y = 100;
    rects[0].w = rects[0].h = 100 + (100 * scale);
    renderer.SetDrawColor(SDL::Color{255, 0, 0}); // red
    renderer.RenderRect(rects[0]);

    // Now let's draw several rectangles with one function call.
    for (int i = 0; i < 3; i++) {
      const float size = (i + 1) * 50.0f;
      rects[i].w = rects[i].h = size + (size * scale);
      rects[i].x = (windowSz.x - rects[i].w) / 2; /* center it. */
      rects[i].y = (windowSz.y - rects[i].h) / 2; /* center it. */
    }
    renderer.SetDrawColor(SDL::Color{0, 255, 0}); // green
    renderer.RenderRects(std::span{rects}.subspan(0, 3));

    // those were rectangle _outlines_, really. You can also draw _filled_
    // rectangles!
    rects[0].x = 400;
    rects[0].y = 50;
    rects[0].w = 100 + (100 * scale);
    rects[0].h = 50 + (50 * scale);
    renderer.SetDrawColor(SDL::Color{0, 0, 255}); // blue
    renderer.RenderFillRect(rects[0]);

    // ...and also fill a bunch of rectangles at once...
    for (int i = 0; i < rects.size(); i++) {
      const float w = float(windowSz.x) / rects.size();
      const float h = i * 8.0f;
      rects[i].x = i * w;
      rects[i].y = windowSz.y - h;
      rects[i].w = w;
      rects[i].h = h;
    }
    renderer.SetDrawColor(SDL::Color{255, 255, 255}); // white
    renderer.RenderFillRects(rects);

    renderer.Present();       // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

#define SDL3PP_MAIN_USE_THIS_CLASS Main
#include <SDL3pp/SDL3pp_main.h>
