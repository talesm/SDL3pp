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

TEST_CASE("SourceBytes")
{
  SUBCASE("empty")
  {
    SDL::SourceBytes sb{};
    CHECK(sb.data() == nullptr);
    CHECK(sb.size_bytes() == 0);
  }
  SUBCASE("nullptr")
  {
    SDL::SourceBytes sb{nullptr};
    CHECK(sb.data() == nullptr);
    CHECK(sb.size_bytes() == 0);
  }
  SUBCASE("char pointer + size")
  {
    const char bytes[] = "Test";
    SDL::SourceBytes sb{bytes, 4};
    CHECK(sb.data() == bytes);
    CHECK(sb.size_bytes() == 4);
  }
  SUBCASE("pointer but 0 size")
  {
    const char bytes[] = "Test";
    SDL::SourceBytes sb{bytes, 0};
    CHECK(sb.data() == nullptr);
    CHECK(sb.size_bytes() == 0);
  }
  SUBCASE("nullptr pointer with non 0 size")
  {
    SDL::SourceBytes sb{(const char*)(nullptr), 5};
    CHECK(sb.data() == nullptr);
    CHECK(sb.size_bytes() == 0);
  }
  SUBCASE("Sint32 array")
  {
    const Sint32 bytes[] = {1, 2, 3, 4};
    SDL::SourceBytes sb{bytes};
    CHECK(sb.data_as<Sint32>() == bytes);
    CHECK(sb.size_bytes() == 16);
  }
  SUBCASE("Sint32 span")
  {
    const Sint32 bytes[] = {1, 2, 3, 4};
    std::span span{bytes};
    SDL::SourceBytes sb{span};
    CHECK(sb.data_as<Sint32>() == bytes);
    CHECK(sb.size_bytes() == 16);
  }
  SUBCASE("Sint32 pointer + size")
  {
    const Sint32 bytes[] = {1, 2, 3, 4};
    SDL::SourceBytes sb{bytes, 16};
    CHECK(sb.data_as<Sint32>() == bytes);
    CHECK(sb.size_bytes() == 16);
  }
  SUBCASE("Sint32 vector")
  {
    std::vector<Sint32> bytes = {1, 2, 3, 4};
    SDL::SourceBytes sb{bytes};
    CHECK(sb.data_as<Sint32>() == bytes.data());
    CHECK(sb.size_bytes() == 16);
  }
}

TEST_CASE("TargetBytes")
{
  SUBCASE("empty")
  {
    SDL::TargetBytes sb{};
    CHECK(sb.data() == nullptr);
    CHECK(sb.size_bytes() == 0);
  }
  SUBCASE("nullptr")
  {
    SDL::TargetBytes sb{nullptr};
    CHECK(sb.data() == nullptr);
    CHECK(sb.size_bytes() == 0);
  }
  SUBCASE("char pointer + size")
  {
    char bytes[] = "Test";
    SDL::TargetBytes sb{bytes, 4};
    CHECK(sb.data() == bytes);
    CHECK(sb.size_bytes() == 4);
  }
  SUBCASE("pointer but 0 size")
  {
    char bytes[] = "Test";
    SDL::TargetBytes sb{bytes, 0};
    CHECK(sb.data() == nullptr);
    CHECK(sb.size_bytes() == 0);
  }
  SUBCASE("nullptr pointer with non 0 size")
  {
    SDL::TargetBytes sb{(char*)(nullptr), 5};
    CHECK(sb.data() == nullptr);
    CHECK(sb.size_bytes() == 0);
  }
  SUBCASE("Sint32 array")
  {
    Sint32 bytes[] = {1, 2, 3, 4};
    SDL::TargetBytes sb{bytes};
    CHECK(sb.data_as<Sint32>() == bytes);
    CHECK(sb.size_bytes() == 16);
  }
  SUBCASE("Sint32 span")
  {
    Sint32 bytes[] = {1, 2, 3, 4};
    std::span span{bytes};
    SDL::TargetBytes sb{span};
    CHECK(sb.data_as<Sint32>() == bytes);
    CHECK(sb.size_bytes() == 16);
  }
  SUBCASE("Sint32 pointer + size")
  {
    Sint32 bytes[] = {1, 2, 3, 4};
    SDL::TargetBytes sb{bytes, 16};
    CHECK(sb.data_as<Sint32>() == bytes);
    CHECK(sb.size_bytes() == 16);
  }
  SUBCASE("Sint32 vector")
  {
    std::vector<Sint32> bytes = {1, 2, 3, 4};
    SDL::TargetBytes sb{bytes};
    CHECK(sb.data_as<Sint32>() == bytes.data());
    CHECK(sb.size_bytes() == 16);
  }
}

#endif // SDL3PP_ENABLE_STRING_PARAM
