#include "SDL3pp/SDL3pp_callbackWrapper.h"
#include "doctest.h"

TEST_CASE("UniqueWrapper no params")
{
  using CallbackType = std::function<void()>;
  int counter = 0;
  auto addCounterCB = [&]() { counter++; };
  using UniqueWrapper = SDL::UniqueWrapper<CallbackType>;
  void* addCounterHandle = UniqueWrapper::Wrap(addCounterCB);
  CHECK(addCounterHandle != nullptr);
  SUBCASE("contains")
  {
    CHECK(UniqueWrapper::contains(addCounterHandle));
    CHECK_FALSE(UniqueWrapper::contains((void*)2));
    void* otherHandler = UniqueWrapper::Wrap(addCounterCB);
    CHECK(UniqueWrapper::contains(otherHandler));
    CHECK_FALSE(UniqueWrapper::contains(addCounterHandle));
  }
  SUBCASE("Call")
  {
    UniqueWrapper::at(addCounterHandle)();
    CHECK(counter == 1);
    CHECK(UniqueWrapper::contains(addCounterHandle));
  }
  SUBCASE("CallOnce")
  {
    UniqueWrapper::release(addCounterHandle)();
    CHECK(counter == 1);
    CHECK_FALSE(UniqueWrapper::contains(addCounterHandle));
  }
}
TEST_CASE("UniqueWrapper with params")
{
  using CallbackType = std::function<int(int)>;
  auto addTwoCB = [](int v) { return v + 2; };
  using UniqueWrapper = SDL::UniqueWrapper<CallbackType>;
  void* addTwoHandle = UniqueWrapper::Wrap(addTwoCB);
  CHECK(addTwoHandle != nullptr);
  SUBCASE("Call")
  {
    CHECK(UniqueWrapper::at(addTwoHandle)(42) == 44);
    CHECK(UniqueWrapper::contains(addTwoHandle));
  }
  SUBCASE("CallOnce")
  {
    CHECK(UniqueWrapper::release(addTwoHandle)(13) == 15);
    CHECK_FALSE(UniqueWrapper::contains(addTwoHandle));
  }
}
TEST_CASE("Size_t indexed KeyValueWrapper no params")
{
  using CallbackType = std::function<void()>;
  int counter = 0;
  auto addCounterCB = [&]() { counter++; };
  using KeyValueWrapper = SDL::KeyValueWrapper<size_t, CallbackType>;
  void* addCounterHandle = KeyValueWrapper::Wrap(1, addCounterCB);
  CHECK(addCounterHandle != nullptr);
  SUBCASE("contains")
  {
    CHECK(KeyValueWrapper::contains(addCounterHandle));
    CHECK_FALSE(KeyValueWrapper::contains((void*)2));
    void* otherHandle = KeyValueWrapper::Wrap(2, addCounterCB);
    CHECK(KeyValueWrapper::contains(otherHandle));
    CHECK(addCounterHandle != otherHandle);
  }
  SUBCASE("Call")
  {
    KeyValueWrapper::at(addCounterHandle)();
    CHECK(counter == 1);
    CHECK(KeyValueWrapper::contains(addCounterHandle));
  }
  SUBCASE("CallOnce")
  {
    KeyValueWrapper::release(addCounterHandle)();
    CHECK(counter == 1);
    CHECK_FALSE(KeyValueWrapper::contains(addCounterHandle));
  }
}
