#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "SDL3pp/objectWrapper.hpp"
#include "doctest.h"

struct Dummy
{
  int content = 0;
};

TEST_CASE("ObjectBox")
{
  struct DummyWrapper
  {
    Dummy dummy;
    Dummy* Get() const { return const_cast<Dummy*>(&dummy); }
  };
  CHECK(SDL::ObjectBox<DummyWrapper, Dummy>);
  CHECK(SDL::ObjectBox<DummyWrapper, const Dummy>);
  struct ConstDummyWrapper
  {
    Dummy dummy;
    const Dummy* Get() const { return &dummy; }
  };
  CHECK(SDL::ObjectBox<ConstDummyWrapper, const Dummy>);
  CHECK_FALSE(SDL::ObjectBox<ConstDummyWrapper, Dummy>);
}

namespace SDL {
template<class T>
struct ObjectBase<T, Dummy>
{
  Dummy& operator*() { return *Get<T>(this); }
  Dummy* operator->() { return Get<T>(this); }
};
};

template<class T>
using DummyBase = SDL::ObjectBase<T, Dummy>;

TEST_CASE("ObjectBase")
{
  struct DummyWrapper : DummyBase<DummyWrapper>
  {
    Dummy dummy;
    Dummy* Get() const { return const_cast<Dummy*>(&dummy); }
  };
  CHECK(SDL::ObjectBox<DummyWrapper, Dummy>);
  DummyWrapper wrapper;
  wrapper.dummy.content = 42;
  CHECK(wrapper->content == 42);
};
