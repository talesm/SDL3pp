#include "SDL3pp/SDL3pp_surface.h"
#include "doctest.h"

SCENARIO("Locking surface")
{
  GIVEN("A surface")
  {
    auto surface = SDL::Surface({400, 400}, SDL_PIXELFORMAT_RGBA32);
    REQUIRE(surface);
    WHEN("Check if must lock")
    {
      THEN("Results is false") { CHECK(surface.MustLock() == false); }
    }
  }
}
