/*
 * This example creates an SDL window and renderer, and then draws some
 * geometry (arbitrary polygons) to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from geometry.c on SDL's examples
 */
#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  SDL::SDL init{SDL::INIT_VIDEO};
  SDL::Window window{"examples/renderer/geometry", windowSz};
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
    // we'll have the texture grow and shrink over a few seconds.
    const float direction = (SDL::fmod(now, 2.f) >= 1) ? 1.f : -1.f;
    const float scale = (SDL::fmod(now, 1.f) - 0.5f) * direction + 1.f;
    const float size = 200.0f + (200.0f * scale);

    // as you can see, rendering draws over what was drawn before it.
    renderer.SetDrawColor(SDL::Color{0, 0, 0}); // black
    renderer.RenderClear();                     // start with a blank canvas.

    // Draw a single triangle with a different color at each vertex. Center this
    // one and make it grow and shrink. You always draw triangles with this, but
    // you can string triangles together to form polygons.
    renderer.RenderGeometry(
      {},
      std::array{
        SDL::Vertex{
          .position{windowSz.x / 2.f, (windowSz.y - size) / 2.f},
          .color{.r = 1.f, .a = 1.f},
        },
        SDL::Vertex{
          .position{(windowSz.x + size) / 2.f, (windowSz.y + size) / 2.f},
          .color{.g = 1.f, .a = 1.f},
        },
        SDL::Vertex{
          .position{(windowSz.x - size) / 2.f, (windowSz.y + size) / 2.f},
          .color{.b = 1.f, .a = 1.f},
        },
      });

    // you can also map a texture to the geometry! Texture coordinates go from
    // 0.0f to 1.0f. That will be the location in the texture bound to this
    // vertex.
    renderer.RenderGeometry(texture,
                            std::array{
                              SDL::Vertex{
                                .position{10, 10},
                                .color{1.f, 1.f, 1.f, 1.f},
                                .tex_coord{0, 0},
                              },
                              SDL::Vertex{
                                .position{150, 10},
                                .color{1.f, 1.f, 1.f, 1.f},
                                .tex_coord{1, 0},
                              },
                              SDL::Vertex{
                                .position{10, 150},
                                .color{1.f, 1.f, 1.f, 1.f},
                                .tex_coord{0, 1},
                              },
                            });
    // Did that only draw half of the texture? You can do multiple triangles
    // sharing some vertices, using indices, to get the whole thing on the
    // screen:
    renderer.RenderGeometry(texture,
                            // Vertices
                            std::array{
                              SDL::Vertex{
                                .position{460, 10},
                                .color{1.f, 1.f, 1.f, 1.f},
                                .tex_coord{0, 0},
                              },
                              SDL::Vertex{
                                .position{610, 10},
                                .color{1.f, 1.f, 1.f, 1.f},
                                .tex_coord{1, 0},
                              },
                              SDL::Vertex{
                                .position{460, 150},
                                .color{1.f, 1.f, 1.f, 1.f},
                                .tex_coord{0, 1},
                              },
                              SDL::Vertex{
                                .position{610, 150},
                                .color{1.f, 1.f, 1.f, 1.f},
                                .tex_coord{1, 1},
                              },
                            },
                            // Indices
                            std::array{0, 1, 2, 1, 2, 3});

    renderer.Present();       // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

SDL3PP_DEFINE_CALLBACKS(Main,
                        "Example Renderer Geometry",
                        "1.0",
                        "com.example.renderer-geometry")
