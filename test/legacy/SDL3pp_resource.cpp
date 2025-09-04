#include "SDL3pp/SDL3pp_resource.h"
#include "doctest.h"

TEST_CASE("Int resource")
{
  struct IntResource : SDL::Resource<int>
  {
    using Resource<int>::Resource;

    static void reset(int v) {}
  };

  SUBCASE("Naked ref")
  {
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

    SUBCASE("copy")
    {
      REQUIRE(res);
      IntResource resCopy{res};
      REQUIRE(res == resCopy);
    }

    SUBCASE("move")
    {
      REQUIRE(res);
      IntResource resCopy{std::move(res)};
      REQUIRE(res == resCopy);
    }
  }

  SUBCASE("ResourceUnique")
  {
    SDL::ResourceUnique<IntResource> res{10};
    CHECK(res->get() == 10);
  }
  SUBCASE("ResourceShared")
  {
    SDL::ResourceShared<SDL::ResourceUnique<IntResource>> res{10};
    CHECK(res->get() == 10);
  }
}
