#include "SDL3pp/freeWrapper.hpp"
#include "doctest.h"

struct Dummy
{
  int value;
};

TEST_CASE("FreeWrapper")
{
  SDL::FreeWrapper<Dummy> dummy{static_cast<Dummy*>(SDL_malloc(sizeof(Dummy)))};
  REQUIRE(dummy);
  dummy.reset();
  REQUIRE(!dummy);
}

TEST_CASE("wrapArray")
{
  SDL::FreeWrapper<Dummy[]> dummies{
    SDL::wrapArray(static_cast<Dummy*>(SDL_calloc(3, sizeof(Dummy))))};
  REQUIRE(dummies);
  dummies.reset();
  REQUIRE(!dummies);
}
