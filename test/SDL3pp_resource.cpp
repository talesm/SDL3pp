#include "SDL3pp/SDL3pp_resource.h"
#include "doctest.h"

TEST_CASE("Int resource")
{
  struct IntResource : SDL::Resource<int>
  {
    using Resource<int>::Resource;
  };

  IntResource res{10};
  REQUIRE(res);
  REQUIRE(res.get() == 10);
  REQUIRE(res == res);
  REQUIRE(res != nullptr);

  IntResource none;
  REQUIRE_FALSE(none);
  REQUIRE(none == none);
  REQUIRE(none != res);
  REQUIRE(none == nullptr);

  SUBCASE("release")
  {
    REQUIRE(res);
    REQUIRE(res.release() == 10);
    REQUIRE_FALSE(res);
    REQUIRE(res == none);
    REQUIRE(res == nullptr);
  }
}
