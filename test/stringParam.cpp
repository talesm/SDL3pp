#include "SDL3pp/stringParam.hpp"
#include "doctest.h"

TEST_CASE("StringParam")
{
  std::string result;
  auto test = [&](SDL::StringParam str) { result = str.Get(); };
  REQUIRE_NOTHROW(test("c array"));
  CHECK(result == "c array");
  REQUIRE_NOTHROW(test(std::string_view("string view")));
  CHECK(result == "string view");
  REQUIRE_NOTHROW(test(std::string("string")));
  CHECK(result == "string");
}
