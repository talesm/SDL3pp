/*
 * This example creates an SDL window and renderer, and draws a
 * rotating texture to it, reads back the rendered pixels, converts them to
 * black and white, and then draws the converted image to a corner of the
 * screen.
 *
 * This isn't necessarily an efficient thing to do--in real life one might
 * want to do this sort of thing with a render target--but it's just a visual
 * example of how to use SDL_RenderReadPixels().
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from read-pixels.c on SDL's examples
 */
#include <SDL3pp/SDL3pp.h>

using namespace std::chrono_literals;

struct Main
{
  static SDL::AppResult Init(Main** state, SDL::AppArgs args)
  {
    SDL::SetAppMetadata("Example Renderer Read Pixels",
                        "1.0",
                        "com.example.renderer-read-pixels");
    return SDL::DefaultCreateClass(state, args);
  }

  static constexpr SDL::Point windowSz = {640, 480};

  SDL::SDL init{SDL::INIT_VIDEO};
  SDL::Window window{"examples/renderer/read-pixels", windowSz};
  SDL::Renderer renderer{window};

  /* Textures are pixel data that we upload to the video hardware for fast
    drawing. Lots of 2D engines refer to these as "sprites." We'll do a static
    texture (upload once, draw many times) with data from a bitmap file. */
  SDL::Texture texture{renderer,
                       std::format("{}/assets/sample.bmp", SDL::GetBasePath())};
  SDL::Point textureSz = texture.GetSize();
  SDL::Texture converted_texture;
  SDL::Point converted_textureSz;

  SDL::AppResult Iterate()
  {
    const float now = SDL::ToSeconds(SDL::GetTicks());
    // we'll have a texture rotate around over 2 seconds (2000 milliseconds).
    // 360 degrees in a circle!
    const float rotation = SDL::fmod(now, 2.f) * 360.f;

    // as you can see, rendering draws over what was drawn before it.
    renderer.SetDrawColor(SDL::Color{0, 0, 0}); // black
    renderer.RenderClear();                     // start with a blank canvas.

    // Center this one, and draw it with some rotation so it spins!
    SDL::FRect dst_rect{(windowSz - textureSz) / 2.f, textureSz};
    SDL::FPoint center = textureSz / 2.f;
    renderer.RenderTextureRotated(texture, {}, dst_rect, rotation, center);

    // this next whole thing is _super_ expensive. Seriously, don't do this in
    // real life.

    /* Download the pixels of what has just been rendered. This has to wait for
       the GPU to finish rendering it and everything before it, and then make an
       expensive copy from the GPU to system RAM! */
    SDL::Surface surface = renderer.ReadPixels();

    if (surface && (surface.GetFormat() != SDL::PIXELFORMAT_RGBA8888)) {
      surface = surface.Convert(SDL::PIXELFORMAT_RGBA8888);
    }

    if (surface) {
      // Rebuild converted_texture if the dimensions have changed (window
      // resized, etc).
      if (surface.GetSize() != converted_textureSz) {
        converted_textureSz = surface.GetSize();
        converted_texture = SDL::Texture(renderer,
                                         SDL::PIXELFORMAT_RGBA8888,
                                         SDL::TEXTUREACCESS_STREAMING,
                                         converted_textureSz);
      }

      /* Turn each pixel into either black or white. This is a lousy technique
         but it works here. In real life, something like Floyd-Steinberg
         dithering might work better:
         https://en.wikipedia.org/wiki/Floyd%E2%80%93Steinberg_dithering*/
      for (int y = 0; y < surface->h; y++) {
        Uint32* pixels =
          (Uint32*)(((Uint8*)surface->pixels) + (y * surface->pitch));
        for (int x = 0; x < surface->w; x++) {
          Uint8* p = (Uint8*)(&pixels[x]);
          const Uint32 average =
            (((Uint32)p[1]) + ((Uint32)p[2]) + ((Uint32)p[3])) / 3;
          if (average == 0) {
            p[0] = p[3] = 0xFF;
            p[1] = p[2] = 0; /* make pure black pixels red. */
          } else {
            p[1] = p[2] = p[3] =
              (average > 50)
                ? 0xFF
                : 0x00; /* make everything else either black or white. */
          }
        }
      }
      // upload the processed pixels back into a texture.
      converted_texture.Update({}, surface->pixels, surface->pitch);

      // draw the texture to the top-left of the screen
      renderer.RenderTexture(
        converted_texture, {}, SDL::FRect({0, 0}, windowSz / 4.f));
    }

    renderer.Present();       // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

#define SDL3PP_MAIN_USE_THIS_CLASS Main
#include <SDL3pp/SDL3pp_main.h>
