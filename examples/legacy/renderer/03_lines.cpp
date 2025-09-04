/*
 * This example creates an SDL window and renderer, and then draws some lines
 * to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from lines.c on SDL's examples
 */

#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main
{
  // Window size
  static constexpr SDL::Point windowSz = {640, 480};

  // Init library
  SDL::SDL init{SDL::INIT_VIDEO};

  // We will use this renderer to draw into this window every frame.
  SDL::Window window = SDL::Window::Create("examples/renderer/lines", windowSz);
  SDL::Renderer renderer = SDL::Renderer::Create(window);

  SDL::AppResult Iterate()
  {
    /* Lines (line segments, really) are drawn in terms of points: a set of
       X and Y coordinates, one set for each end of the line.
       (0, 0) is the top left of the window, and larger numbers go down
       and to the right. This isn't how geometry works, but this is pretty
       standard in 2D graphics. */
    static const SDL_FPoint line_points[] = {
      {100, 354},
      {220, 230},
      {140, 230},
      {320, 100},
      {500, 230},
      {420, 230},
      {540, 354},
      {400, 354},
      {100, 354},
    };

    // as you can see, rendering draws over what was drawn before it.
    renderer->SetDrawColor(SDL::Color{100, 100, 100}); // grey
    renderer->RenderClear(); // start with a blank canvas.

    // You can draw lines, one at a time, like these brown ones...
    renderer->SetDrawColor(SDL::Color{127, 49, 32});
    renderer->RenderLine({240, 450}, {400, 450});
    renderer->RenderLine({240, 356}, {400, 356});
    renderer->RenderLine({240, 356}, {240, 450});
    renderer->RenderLine({400, 356}, {400, 450});

    // You can also draw a series of connected lines in a single batch...
    renderer->SetDrawColor(SDL::Color{0, 255, 0});
    renderer->RenderLines(line_points);

    // here's a bunch of lines drawn out from a center point in a circle.
    // we randomize the color of each line, so it functions as animation.
    for (float i = 0; i < 360; i++) {
      const float size = 30.0f;
      const float x = 320.0f;
      const float y = 95.0f - (size / 2.0f);
      renderer->SetDrawColor(
        SDL::Color(SDL::rand(256), SDL::rand(256), SDL::rand(256)));
      renderer->RenderLine({x, y},
                           {x + SDL::sin(i) * size, y + SDL::cos(i) * size});
    }

    renderer->Present();      // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

SDL3PP_DEFINE_CALLBACKS(Main,
                        "Example Renderer Lines",
                        "1.0",
                        "com.example.renderer-lines")
