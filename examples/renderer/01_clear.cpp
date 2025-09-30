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

#define SDL3PP_MAIN_USE_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main
{
  // Window size
  static constexpr SDL::Point windowSz = {640, 480};

  // Init library
  static SDL::AppResult Init(Main** m, SDL::AppArgs args)
  {
    SDL::SetAppMetadata(
      "Example Renderer Clear", "1.0", "com.example.renderer-clear");
    SDL::Init(SDL::INIT_VIDEO);
    *m = new Main();
    return SDL::APP_CONTINUE;
  }

  // We will use this renderer to draw into this window every frame.
  SDL::Window window{"examples/renderer/clear", windowSz};
  SDL::Renderer renderer{window};

  SDL::AppResult Iterate()
  {
    // convert from milliseconds to seconds.
    const double now = ((double)SDL_GetTicks()) / 1000.0;

    // choose the color for the frame we will draw. The sine wave trick makes it
    // fade between colors smoothly.
    SDL::FColor color{
      float(0.5f + 0.5f * SDL::sin(now)),
      float(0.5f + 0.5f * SDL::sin(now + SDL_PI_D * 2 / 3)),
      float(0.5f + 0.5f * SDL::sin(now + SDL_PI_D * 4 / 3)),
    };
    renderer.SetDrawColorFloat(color);
    renderer.RenderClear();

    renderer.Present();
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main)
