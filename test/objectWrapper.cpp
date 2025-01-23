#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "SDL3pp/objectWrapper.hpp"
#include "doctest.h"

struct Dummy
{
  int content;
};

TEST_CASE("ObjectBox")
{
  struct DummyWrapper
  {
    Dummy dummy;
    Dummy* Get() { return &dummy; }
  };
  CHECK(SDL::ObjectBox<DummyWrapper, Dummy>);
  CHECK(SDL::ObjectBox<DummyWrapper, const Dummy>);
  struct ConstDummyWrapper
  {
    Dummy dummy;
    const Dummy* Get() { return &dummy; }
  };
  CHECK(SDL::ObjectBox<ConstDummyWrapper, const Dummy>);
  CHECK_FALSE(SDL::ObjectBox<ConstDummyWrapper, Dummy>);
}
