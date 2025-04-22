/*
 * This example creates an SDL window and renderer, and then draws some text
 * using SDL_RenderDebugText() every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from debug-text.c on SDL's examples
 */
#include <SDL3pp/SDL3pp.h>

using namespace std::chrono_literals;

struct Main
{
  static SDL::AppResult Init(Main** state, SDL::AppArgs args)
  {
    SDL::SetAppMetadata("Example Renderer Renderer Debug Text",
                        "1.0",
                        "com.example.renderer-debug-text");
    return SDL::DefaultCreateClass(state, args);
  }

  static constexpr SDL::Point windowSz = {640, 480};

  SDL::SDL init{SDL::INIT_VIDEO};
  SDL::Window window{"examples/renderer/debug-text", windowSz};
  SDL::Renderer renderer{window};

  SDL::AppResult Iterate()
  {
    constexpr int charsize = SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;

    // as you can see, rendering draws over what was drawn before it.
    renderer.SetDrawColor(SDL::Color{0, 0, 0}); // black
    renderer.RenderClear();                     // start with a blank canvas.

    renderer.SetDrawColor(SDL::Color{255, 255, 255}); // white
    renderer.RenderDebugText({272, 100}, "Hello world!");
    renderer.RenderDebugText({224, 150}, "This is some debug text.");

    renderer.SetDrawColor(SDL::Color{51, 102, 255}); // light blue
    renderer.RenderDebugText({184, 200}, "You can do it in different colors.");
    renderer.SetDrawColor(SDL::Color{255, 255, 255}); // white

    renderer.SetScale({4, 4});
    renderer.RenderDebugText({14, 65}, "It can be scaled.");
    renderer.SetScale({1, 1});
    renderer.RenderDebugText(
      {64, 350},
      "This only does ASCII chars. So this laughing emoji won't draw: 🤣");

    renderer.RenderDebugTextFormat(
      SDL::FPoint{(windowSz.x - (charsize * 46)) / 2.f, 400.f},
      "(This program has been running for {} seconds.)",
      Sint64(SDL::ToSeconds(SDL::GetTicks())));

    renderer.Present();       // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

#define SDL3PP_MAIN_USE_THIS_CLASS Main
#include <SDL3pp/SDL3pp_main.h>
