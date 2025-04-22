#include "SDL3pp/SDL3pp_strings.h"
#include "SDL3pp/SDL3pp_stdinc.h"
#include "doctest.h"

#ifdef SDL3PP_ENABLE_STRING_PARAM

TEST_CASE("StringParam")
{
  std::string result;
  auto test = [&](SDL::StringParam str) { result = str; };
  REQUIRE_NOTHROW(test("c array"));
  CHECK(result == "c array");
  REQUIRE_NOTHROW(test(std::string_view("string view")));
  CHECK(result == "string view");
  REQUIRE_NOTHROW(test(std::string("string")));
  CHECK(result == "string");
}

TEST_CASE("StringParam const string & optimization")
{
  std::string test = "test";

  REQUIRE(SDL::StringParam(test) == test.c_str());
  REQUIRE(SDL::StringParam(std::string(test)) != test.c_str());
}

#endif // SDL3PP_ENABLE_STRING_PARAM
