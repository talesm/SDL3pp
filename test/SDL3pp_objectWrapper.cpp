#include "SDL3pp/SDL3pp_objectWrapper.h"
#include "doctest.h"

struct Dummy
{
  int content = 0;
};

TEST_CASE("ObjectBox")
{
  struct DummyBox
  {
    Dummy* dummy;
    Dummy* get() const;
    Dummy* release() const;
    void free();
  };
  CHECK(SDL::ObjectBox<DummyBox, Dummy*>);
  CHECK(SDL::ObjectBox<DummyBox, const Dummy*>);
  struct ConstDummyBox
  {
    const Dummy* dummy;
    const Dummy* get() const;
    const Dummy* release() const;
    void free();
  };
  CHECK(SDL::ObjectBox<ConstDummyBox, const Dummy*>);
  CHECK_FALSE(SDL::ObjectBox<ConstDummyBox, Dummy*>);
}

template<class T>
struct DummyConstBase : T
{
  using T::T;

  int GetContent() const { return T::get()->content; }
};

template<class T>
struct DummyBase : DummyConstBase<T>
{
  using DummyConstBase<T>::DummyConstBase;

  DummyBase(int content)
    : DummyConstBase<T>(new Dummy(content))
  {
  }

  void SetContent(int content) const { T::get()->content = content; }
};

template<>
void SDL::ObjectRef<Dummy>::doFree(Dummy* dummy)
{
  delete dummy;
}

using DummyUnique = DummyBase<SDL::ObjectUnique<Dummy>>;

TEST_CASE("ObjectUnique")
{
  CHECK(SDL::ObjectBox<DummyUnique, Dummy*>);
  DummyUnique wrapper{42};
  auto result = wrapper->content;
  CHECK(result == 42);
}

using DummyRef = DummyBase<SDL::ObjectRef<Dummy>>;
using DummyConstRef = DummyConstBase<SDL::ObjectRef<const Dummy>>;

TEST_CASE("ObjectRef")
{
  CHECK(SDL::ObjectBox<DummyRef, Dummy*>);

  SUBCASE("Non-const")
  {
    SUBCASE("From stack")
    {
      Dummy dummy{42};
      DummyRef wrapper = &dummy;
      CHECK(wrapper.get() == &dummy);
      CHECK(wrapper.GetContent() == 42);
      CHECK(wrapper->content == 42);
      dummy.content = 13;
      CHECK(wrapper.GetContent() == 13);
      CHECK(wrapper->content == 13);
      wrapper.SetContent(42);
      CHECK(wrapper.GetContent() == 42);
      CHECK(wrapper->content == 42);
    }
    SUBCASE("From unique")
    {
      DummyUnique owner{new Dummy{42}};
      DummyRef wrapper{owner};
      CHECK(wrapper->content == 42);
    }
  }
  SUBCASE("Const")
  {
    SUBCASE("From stack")
    {
      Dummy dummy{42};
      DummyConstRef wrapper = &dummy;
      CHECK(wrapper.get() == &dummy);
      CHECK(wrapper.GetContent() == 42);
      CHECK(wrapper->content == 42);
      dummy.content = 13;
      CHECK(wrapper.GetContent() == 13);
      CHECK(wrapper->content == 13);
    }
    SUBCASE("From unique")
    {
      DummyUnique owner{new Dummy{42}};
      DummyConstRef wrapper{owner};
      CHECK(wrapper->content == 42);
    }
  }
}
