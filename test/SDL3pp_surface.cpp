#include "SDL3pp/SDL3pp_surface.h"
#include "doctest.h"

SCENARIO("Locking surface")
{
  GIVEN("A surface")
  {
    SDL::Surface surface{400, 400, SDL_PIXELFORMAT_RGBA32};
    REQUIRE(surface);
    WHEN("Check if must lock")
    {
      THEN("Results is false") { CHECK(surface.MustLock() == false); }
    }
    WHEN("RLE is active")
    {
      surface.SetRLE(true);
      AND_WHEN("Check if must lock")
      {
        THEN("Results is true") { CHECK(surface.MustLock() == true); }
      }
      AND_WHEN("We Lock the surface")
      {
        SDL::SurfaceLock guard{surface.Lock()};
      }
    }
  }
}
