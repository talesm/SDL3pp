#include "SDL3pp/SDL3pp_spanRef.h"
#include <vector>
#include "doctest.h"

struct Base
{
  int value;
};

struct Derived : Base
{};

struct Independent
{};

struct NonWrapperDerived : Base
{
  int otherValue;
};

TEST_CASE("DerivedWrapper")
{
  REQUIRE(SDL::DerivedWrapper<Base, Base>);
  REQUIRE(SDL::DerivedWrapper<Derived, Base>);
  REQUIRE_FALSE(SDL::DerivedWrapper<Independent, Base>);
  REQUIRE_FALSE(SDL::DerivedWrapper<NonWrapperDerived, Base>);
}

TEST_CASE("SpanRef")
{
  SUBCASE("Empty ctor")
  {
    SDL::SpanRef<const Base> constSpan;
    SDL::SpanRef<Base> mutSpan;
    REQUIRE(constSpan.size() == 0);
    REQUIRE(mutSpan.size() == 0);
  }

  SUBCASE("Base array ctor")
  {
    Base baseArray[] = {1, 2, 3};
    SDL::SpanRef<const Base> constSpan{baseArray};
    SDL::SpanRef<Base> mutSpan{baseArray};
    REQUIRE(constSpan.size() == 3);
    REQUIRE(mutSpan.size() == 3);
  }

  SUBCASE("Const Base array ctor")
  {
    const Base baseArray[] = {1, 2, 3};
    SDL::SpanRef<const Base> constSpan{baseArray};
    REQUIRE(constSpan.size() == 3);
  }

  SUBCASE("Derived array ctor")
  {
    Derived derivedArray[] = {1, 2, 3};
    SDL::SpanRef<const Base> constSpan{derivedArray};
    SDL::SpanRef<Base> mutSpan{derivedArray};
    REQUIRE(constSpan.size() == 3);
    REQUIRE(mutSpan.size() == 3);
  }

  SUBCASE("Const Derived array ctor")
  {
    const Derived derivedArray[] = {1, 2, 3};
    SDL::SpanRef<const Base> constSpan{derivedArray};
    REQUIRE(constSpan.size() == 3);
  }

  SUBCASE("span ctor")
  {
    Base baseArray[] = {1, 2, 3};
    std::span<Base> baseSpan{baseArray};
    SDL::SpanRef<const Base> constSpan{baseSpan};
    SDL::SpanRef<Base> mutSpan{baseSpan};
    REQUIRE(constSpan.size() == 3);
    REQUIRE(mutSpan.size() == 3);
  }

  SUBCASE("Iterator Derived array ctor")
  {
    Derived derivedArray[] = {1, 2, 3};
    SDL::SpanRef<const Base> constSpan{std::begin(derivedArray),
                                       std::end(derivedArray)};
    SDL::SpanRef<Base> mutSpan{std::begin(derivedArray),
                               std::end(derivedArray)};
    REQUIRE(constSpan.size() == 3);
    REQUIRE(mutSpan.size() == 3);
  }

  SUBCASE("std::array ctor")
  {
    std::array<Derived, 3> derivedArray = {1, 2, 3};
    SDL::SpanRef<const Base> constSpan{derivedArray};
    SDL::SpanRef<Base> mutSpan{derivedArray};
    REQUIRE(constSpan.size() == 3);
    REQUIRE(mutSpan.size() == 3);
  }

  SUBCASE("std::vector ctor")
  {
    std::vector<Derived> derivedArray{{1}, {2}, {3}};
    SDL::SpanRef<const Base> constSpan{derivedArray};
    SDL::SpanRef<Base> mutSpan{derivedArray};
    REQUIRE(constSpan.size() == 3);
    REQUIRE(mutSpan.size() == 3);
  }
}
