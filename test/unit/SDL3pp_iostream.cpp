#include "SDL3pp/SDL3pp_iostream.h"
#include "doctest.h"

namespace SDL {

TEST_CASE("SDL3pp_iostream")
{
  IOStream streamOwn;
  IOStreamRef streamRef(streamOwn);
  IOStreamRaw streamRaw = streamOwn.get();

  SUBCASE("Copy Own")
  {
    IOStreamParam param{streamOwn};
    REQUIRE(param.closeio == false);
  }
  SUBCASE("Move Own")
  {
    IOStreamParam param{std::move(streamOwn)};
    REQUIRE(param.closeio == true);
  }
  SUBCASE("Copy Ref")
  {
    IOStreamParam param{streamRef};
    REQUIRE(param.closeio == false);
  }
  SUBCASE("Move Ref")
  {
    IOStreamParam param{std::move(streamRef)};
    REQUIRE(param.closeio == false);
  }
  SUBCASE("Copy Raw")
  {
    IOStreamParam param{streamRaw};
    REQUIRE(param.closeio == false);
  }
  SUBCASE("Move Raw")
  {
    IOStreamParam param{std::move(streamRaw)};
    REQUIRE(param.closeio == false);
  }
}
}
