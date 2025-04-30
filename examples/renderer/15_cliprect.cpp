/*
 * This example creates an SDL window and renderer, and then draws a scene
 * to it every frame, while sliding around a clipping rectangle.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from cliprect.c on SDL's examples
 */
#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};
  static constexpr int cliprect_size = 250;
  static constexpr float cliprect_speed = 200; // pixels per second

  SDL::SDL init{SDL::INIT_VIDEO};
  SDL::Window window{"examples/renderer/cliprect", windowSz};
  SDL::Renderer renderer{window};

  /* Textures are pixel data that we upload to the video hardware for fast
    drawing. Lots of 2D engines refer to these as "sprites." We'll do a static
    texture (upload once, draw many times) with data from a bitmap file. */
  SDL::Texture texture{
    renderer,
    std::format("{}../assets/sample.bmp", SDL::GetBasePath())};

  SDL::FPoint cliprect_position;
  SDL::FPoint cliprect_direction{1, 1};
  SDL::Nanoseconds last_time = SDL::GetTicks();

  // This function runs once per frame, and is the heart of the program.
  SDL::AppResult Iterate()
  {
    const SDL::Rect cliprect{SDL::Point(cliprect_position),
                             SDL::Point{cliprect_size, cliprect_size}};
    const SDL::Nanoseconds now = SDL::GetTicks();
    const float elapsed = SDL::ToSeconds(now - last_time);
    const float distance = elapsed * cliprect_speed;

    // Set a new clipping rectangle position
    renderer.SetClipRect(cliprect);
    cliprect_position += cliprect_direction * distance;
    if (cliprect_position.x < 0.0f) {
      cliprect_position.x = 0.0f;
      cliprect_direction.x = 1.0f;
    } else if (cliprect_position.x >= (windowSz.x - cliprect_size)) {
      cliprect_position.x = (windowSz.x - cliprect_size) - 1;
      cliprect_direction.x = -1.0f;
    }
    if (cliprect_position.y < 0.0f) {
      cliprect_position.y = 0.0f;
      cliprect_direction.y = 1.0f;
    } else if (cliprect_position.y >= (windowSz.y - cliprect_size)) {
      cliprect_position.y = (windowSz.y - cliprect_size) - 1;
      cliprect_direction.y = -1.0f;
    }
    last_time = now;

    // okay, now draw!

    // Note that SDL_RenderClear is _not_ affected by the clipping rectangle!
    renderer.SetDrawColor(SDL::Color{33, 33, 33}); // grey, full alpha
    renderer.RenderClear();                        // start with a blank canvas.

    SDL::FRect dst_rect = {{0, 0}, texture.GetSize()};

    // stretch the texture across the entire window. Only the piece in the
    // clipping rectangle will actually render, though!
    renderer.RenderTexture(texture, {}, {});

    renderer.Present();       // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

SDL3PP_DEFINE_CALLBACKS(Main,
                        "Example Renderer Clipping Rectangle",
                        "1.0",
                        "com.example.renderer-cliprect")
