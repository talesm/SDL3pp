/**
 * @file 19_affine_textures.cpp
 *
 * Based on SDL3's affine_textures.c
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
  SDL::Window window =
    SDL::CreateWindowAndRenderer("examples/renderer/affine_textures",
                                 windowSz,
                                 0,
                                 nullptr);
  SDL::RendererRef renderer{window.GetRenderer()};

  /* Textures are pixel data that we upload to the video hardware for fast
     drawing. Lots of 2D engines refer to these as "sprites." We'll do a static
     texture (upload once, draw many times) with data from a bitmap file. */
  SDL::Texture texture{
    renderer,
    std::format("{}../assets/sample.png", SDL::GetBasePath())};

  SDL::AppResult Iterate() final
  {
    const float x0 = 0.5f * windowSz.x;
    const float y0 = 0.5f * windowSz.y;
    const float px = SDL::min(windowSz.x, windowSz.y) / SDL::sqrt(3.0f);

    const Uint64 now = SDL::GetTicksMS();
    const float rad = (((float)((int)(now % 2000))) / 2000.0f) * SDL_PI_F * 2;
    const float cos = SDL_cosf(rad);
    const float sin = SDL_sinf(rad);
    const float k[3] = {3.0f / SDL_sqrtf(50.0f),
                        4.0f / SDL_sqrtf(50.0f),
                        5.0f / SDL_sqrtf(50.0f)};
    float mat[9] = {
      // clang-format off
       cos      + (1.0f-cos)*k[0]*k[0], -sin*k[2] + (1.0f-cos)*k[0]*k[1],  sin*k[1] + (1.0f-cos)*k[0]*k[2], 
       sin*k[2] + (1.0f-cos)*k[0]*k[1],  cos      + (1.0f-cos)*k[1]*k[1], -sin*k[0] + (1.0f-cos)*k[1]*k[2], 
      -sin*k[1] + (1.0f-cos)*k[0]*k[2],  sin*k[0] + (1.0f-cos)*k[1]*k[2],  cos      + (1.0f-cos)*k[2]*k[2],
      // clang-format on
    };

    float corners[16];
    for (int i = 0; i < 8; i++) {
      const float x = (i & 1) ? -0.5f : 0.5f;
      const float y = (i & 2) ? -0.5f : 0.5f;
      const float z = (i & 4) ? -0.5f : 0.5f;
      corners[0 + 2 * i] = mat[0] * x + mat[1] * y + mat[2] * z;
      corners[1 + 2 * i] = mat[3] * x + mat[4] * y + mat[5] * z;
    }
    renderer.SetDrawColor(
      {0x42, 0x87, 0xf5, SDL::ALPHA_OPAQUE}); // light blue background.
    renderer.RenderClear();

    for (int i = 1; i < 7; i++) {
      const int dir = 3 & ((i & 4) ? ~i : i);
      const int odd = (i & 1) ^ ((i & 2) >> 1) ^ ((i & 4) >> 2);
      if (0 < (odd ? 1.0f : -1.0f) * mat[5 + dir]) continue;
      int origin_index = (1 << ((dir - 1) % 3));
      int right_index = (1 << ((dir + odd) % 3)) | origin_index;
      int down_index = (1 << ((dir + (odd ^ 1)) % 3)) | origin_index;
      if (!odd) {
        origin_index ^= 7;
        right_index ^= 7;
        down_index ^= 7;
      }
      SDL::FPoint origin{x0 + px * corners[0 + 2 * origin_index],
                         y0 + px * corners[1 + 2 * origin_index]};
      SDL::FPoint right{x0 + px * corners[0 + 2 * right_index],
                        y0 + px * corners[1 + 2 * right_index]};
      SDL::FPoint down{x0 + px * corners[0 + 2 * down_index],
                       y0 + px * corners[1 + 2 * down_index]};
      renderer.RenderTextureAffine(texture, {}, origin, right, down);
    }

    renderer.Present();
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CLASS_CALLBACKS(Main,
                              SDL::INIT_VIDEO,
                              "Example Renderer Affine Textures",
                              "1.0",
                              "com.example.renderer-affine-textures")
