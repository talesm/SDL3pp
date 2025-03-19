#include "SDL3pp/SDL3pp_ownPtr.h"
#include "doctest.h"

TEST_CASE("Empty OwnPtr")
{
  SDL::OwnPtr<int> ptr;
  CHECK_FALSE(ptr);
  CHECK(ptr == nullptr);
  CHECK(ptr.get() == nullptr);
}

TEST_CASE("Scalar OwnPtr")
{
  SDL::OwnPtr<int> ptr{static_cast<int*>(SDL::malloc(sizeof(int)))};
  CHECK(ptr == true);
  CHECK(ptr != nullptr);
  CHECK(ptr.get() != nullptr);
  REQUIRE_NOTHROW(*ptr = 42);
  CHECK(*ptr == 42);
}
