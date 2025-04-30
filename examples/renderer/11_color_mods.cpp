/*
 * This example creates an SDL window and renderer, and then draws some
 * textures to it every frame, adjusting their color.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from color-mods.c on SDL's examples
 */
#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  SDL::SDL init{SDL::INIT_VIDEO};
  SDL::Window window{"examples/renderer/color-mods", windowSz};
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
    // choose the modulation values for the center texture. The sine wave trick
    // makes it fade between colors smoothly.
    const float red = 0.5 + 0.5 * SDL::sin(now);
    const float green = 0.5 + 0.5 * SDL::sin(now + SDL_PI_F * 2 / 3);
    const float blue = 0.5 + 0.5 * SDL::sin(now + SDL_PI_F * 4 / 3);

    // as you can see, rendering draws over what was drawn before it.
    renderer.SetDrawColor(SDL::Color{0, 0, 0}); // black
    renderer.RenderClear();                     // start with a blank canvas.

    // Just draw the static texture a few times. You can think of it like a
    // stamp, there isn't a limit to the number of times you can draw with it.

    // Color modulation multiplies each pixel's red, green, and blue intensities
    // by the mod values, so multiplying by 1.0f will leave a color intensity
    // alone, 0.0f will shut off that color completely, etc.

    // top left; let's make this one blue!
    SDL::FRect dst_rect{{0, 0}, texture.GetSize()};
    texture.SetColorMod(0.f, 0.f, 1.f);
    renderer.RenderTexture(texture, {}, dst_rect);

    // center this one, and have it cycle through red/green/blue modulations.
    dst_rect.x = (windowSz.x - dst_rect.w) / 2.f;
    dst_rect.y = (windowSz.y - dst_rect.h) / 2.f;
    texture.SetColorMod(red, green, blue);
    renderer.RenderTexture(texture, {}, dst_rect);

    // bottom right; let's make this one red!
    dst_rect.x = windowSz.x - dst_rect.w;
    dst_rect.y = windowSz.y - dst_rect.h;
    texture.SetColorMod(1.f, 0.f, 0.f);
    renderer.RenderTexture(texture, {}, dst_rect);

    renderer.Present();       // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

SDL3PP_DEFINE_CALLBACKS(Main,
                        "Example Renderer Color Mods",
                        "1.0",
                        "com.example.renderer-color-mods")
