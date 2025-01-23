#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "SDL3pp/stringWrapper.hpp"
#include "doctest.h"

TEST_CASE("StringWrapper")
{
  std::string result;
  auto test = [&](SDL::StringWrapper str) { result = str.Get(); };
  REQUIRE_NOTHROW(test("c array"));
  CHECK(result == "c array");
  REQUIRE_NOTHROW(test(std::string_view("string view")));
  CHECK(result == "string view");
  REQUIRE_NOTHROW(test(std::string("string")));
  CHECK(result == "string");
}
