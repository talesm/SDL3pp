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

protected:
  static Dummy* Create(int value = 0) { return new Dummy{value}; }
  static void Destroy(Dummy* dummy) { delete dummy; }
};
};

template<class T>
using DummyBase = SDL::ObjectBase<T, Dummy>;

using DummyUnique = SDL::ObjectUnique<Dummy>;

TEST_CASE("ObjectUnique")
{
  CHECK(SDL::ObjectBox<DummyUnique, Dummy>);
  DummyUnique wrapper{42};
  CHECK(wrapper->content == 42);
}

using DummyWrapper = SDL::ObjectWrapper<Dummy>;

TEST_CASE("ObjectWrapper")
{
  CHECK(SDL::ObjectBox<DummyWrapper, Dummy>);
  DummyUnique owner{42};
  DummyWrapper wrapper{owner};
  CHECK(wrapper->content == 42);
}
