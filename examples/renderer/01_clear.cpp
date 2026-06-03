/**
 * @file 01_clear.cpp
 *
 * Based on SDL3's clear.c
 *
 * This example code creates an SDL window and renderer, and then clears the
 * window to a different color every frame, so you'll effectively get a window
 * that's smoothly fading between colors.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CLASS_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main : SDL::AppInterface
{
  // Window size
  static constexpr SDL::Point windowSz = {640, 480};

  // We will use this renderer to draw into this window every frame.
  SDL::Window window = SDL::CreateWindowAndRenderer("examples/renderer/clear",
                                                    windowSz,
                                                    0,
                                                    nullptr);
  SDL::RendererRef renderer = window.GetRenderer();

  SDL::AppResult Iterate() final
  {
    // convert from milliseconds to seconds.
    const double now = SDL::ToSeconds(SDL::GetTicks());

    // choose the color for the frame we will draw. The sine wave trick makes it
    // fade between colors smoothly.
    renderer.SetDrawColorFloat({
      float(0.5f + 0.5f * SDL::sin(now)),
      float(0.5f + 0.5f * SDL::sin(now + SDL::PI_D * 2 / 3)),
      float(0.5f + 0.5f * SDL::sin(now + SDL::PI_D * 4 / 3)),
      SDL::ALPHA_OPAQUE_FLOAT,
    });
    renderer.RenderClear();

    renderer.Present();
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CLASS_CALLBACKS(Main,
                              SDL::INIT_VIDEO,
                              "Example Renderer Clear",
                              "1.0",
                              "com.example.renderer-clear")
