#include "SDL3pp/SDL3pp_ownPtr.h"
#include <span>
#include "SDL3pp/SDL3pp_stdinc.h"
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
  REQUIRE(bool(ptr) == true);
  CHECK(ptr != nullptr);
  CHECK(ptr.get() != nullptr);
  REQUIRE_NOTHROW(*ptr = 42);
  CHECK(*ptr == 42);
}

TEST_CASE("Array OwnPtr")
{
  SDL::OwnPtr<int[]> ptr{static_cast<int*>(SDL::calloc(4, sizeof(int)))};
  REQUIRE(bool(ptr) == true);
  CHECK(ptr != nullptr);
  CHECK(ptr.get() != nullptr);
  REQUIRE_NOTHROW(ptr[0] = 42);
  CHECK(ptr[0] == 42);
  CHECK(ptr[1] == 0);
}

TEST_CASE("OwnArray")
{
  SDL::OwnArray<int> arr{static_cast<int*>(SDL::calloc(4, sizeof(int))), 3};
  REQUIRE_FALSE(arr.empty());
  REQUIRE_NOTHROW(arr[0] = 42);
  REQUIRE_NOTHROW(arr[1] = 13);
  REQUIRE_NOTHROW(arr[2] = 108);

  SUBCASE("sanity tests")
  {
    CHECK_FALSE(arr.empty());
    CHECK(arr.data() != nullptr);
    CHECK(arr[0] == 42);
    CHECK(arr[1] == 13);
    CHECK(arr[2] == 108);
    CHECK(arr[3] == 0);
    REQUIRE(arr.size() == 3);
    std::span sArr{arr};
    REQUIRE(sArr.size() == 3);
  }

  SUBCASE("Automatic counting")
  {
    SDL::RefArray<int> rArr{arr};
    REQUIRE_FALSE(rArr.empty());
    REQUIRE(rArr.size() == 3);
    std::span sArr{rArr};
    REQUIRE(sArr.size() == 3);
  }
}
