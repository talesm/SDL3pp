/*
 * This example creates an SDL window and renderer, and then draws some points
 * to it every frame.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally from points.c on SDL's examples
 */

#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main
{
  // Window size
  static constexpr SDL::Point windowSz = {640, 480};

  // Init library
  static SDL::Window InitAndCreateWindow()
  {
    SDL::SetAppMetadata(
      "Example Renderer Points", "1.0", "com.example.renderer-points");
    SDL::Init(SDL::INIT_VIDEO);
    return SDL::CreateWindowAndRenderer(
      "examples/renderer/points", windowSz, 0, nullptr);
  }

  // We will use this renderer to draw into this window every frame.
  SDL::Window window{InitAndCreateWindow()};
  SDL::RendererRef renderer{window.GetRenderer()};

  std::chrono::nanoseconds last_time;

  static constexpr int NUM_POINTS = 500;

  // move at least this many pixels per second.
  static constexpr float MIN_PIXELS_PER_SECOND = 30;

  // move this many pixels per second at most.
  static constexpr float MAX_PIXELS_PER_SECOND = 60;

  /* (track everything as parallel arrays instead of a array of structs,
     so we can pass the coordinates to the renderer in a single function call.)
   */

  /* Points are plotted as a set of X and Y coordinates.
     (0, 0) is the top left of the window, and larger numbers go down
     and to the right. This isn't how geometry works, but this is pretty
     standard in 2D graphics. */
  std::array<SDL::FPoint, NUM_POINTS> points;
  std::array<float, NUM_POINTS> point_speeds;

  // This function runs once at startup.
  Main()
  {
    // set up the data for a bunch of points.
    for (int i = 0; i < points.size(); ++i) {
      points[i] = {SDL::randf() * windowSz.GetX(),
                   SDL::randf() * windowSz.GetY()};
      point_speeds[i] =
        MIN_PIXELS_PER_SECOND +
        SDL::randf() * (MAX_PIXELS_PER_SECOND - MIN_PIXELS_PER_SECOND);
    }

    last_time = SDL::GetTicks();
  }

  SDL::AppResult Iterate()
  {
    const auto now = SDL::GetTicks();
    // Seconds since last iteration
    const auto elapsed = SDL::ToSeconds(now - last_time);

    // let's move all our points a little for a new frame.
    for (int i = 0; i < points.size(); i++) {
      const float distance = elapsed * point_speeds[i];
      points[i] += SDL::FPoint{distance, distance};
      if ((points[i].x >= windowSz.x) || (points[i].y >= windowSz.y)) {
        /* off the screen; restart it elsewhere! */
        if (SDL_rand(2)) {
          points[i].x = SDL::randf() * float(windowSz.x);
          points[i].y = 0.0f;
        } else {
          points[i].x = 0.0f;
          points[i].y = SDL::randf() * float(windowSz.y);
        }
        point_speeds[i] =
          MIN_PIXELS_PER_SECOND +
          (SDL::randf() * (MAX_PIXELS_PER_SECOND - MIN_PIXELS_PER_SECOND));
      }
    }

    last_time = now;

    // as you can see, rendering draws over what was drawn before it.
    renderer.SetDrawColor(SDL::Color{0, 0, 0}); // black
    renderer.RenderClear();                     // start with a blank canvas.
    renderer.SetDrawColor(SDL_Color{255, 255, 255}); // white
    renderer.RenderPoints(points);                   // Draw all the points

    /* You can also draw single points with SDL_RenderPoint(), but it's
       cheaper (sometimes significantly so) to do them all at once. */

    renderer.Present();       // put it all on the screen!
    return SDL::APP_CONTINUE; // carry on with the program!
  }
};

SDL3PP_DEFINE_CALLBACKS(Main)
