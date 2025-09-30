#include "SDL3pp/SDL3pp_version.h"
#include "doctest.h"

TEST_CASE("Version check")
{
  int dynamicVersion = SDL::GetVersion();
  REQUIRE(dynamicVersion >= SDL_VERSION);
}
