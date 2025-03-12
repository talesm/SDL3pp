#include "SDL3pp/SDL3pp_callbackWrapper.h"
#include "doctest.h"

TEST_CASE("Unique KeyValueWrapper no params")
{
  using CallbackType = std::function<void()>;
  int counter = 0;
  auto addCounterCB = [&]() { counter++; };
  using KeyValueWrapper = SDL::KeyValueWrapper<void, CallbackType>;
  void* addCounterHandle = KeyValueWrapper::Wrap(addCounterCB);
  CHECK(addCounterHandle != nullptr);
  SUBCASE("contains")
  {
    CHECK(KeyValueWrapper::contains(addCounterHandle));
    CHECK_FALSE(KeyValueWrapper::contains((void*)2));
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
TEST_CASE("Unique KeyValueWrapper with params")
{
  using CallbackType = std::function<int(int)>;
  auto addTwoCB = [](int v) { return v + 2; };
  using KeyValueWrapper = SDL::KeyValueWrapper<void, CallbackType>;
  void* addTwoHandle = KeyValueWrapper::Wrap(addTwoCB);
  CHECK(addTwoHandle != nullptr);
  SUBCASE("Call")
  {
    CHECK(KeyValueWrapper::at(addTwoHandle)(42) == 44);
    CHECK(KeyValueWrapper::contains(addTwoHandle));
  }
  SUBCASE("CallOnce")
  {
    CHECK(KeyValueWrapper::release(addTwoHandle)(13) == 15);
    CHECK_FALSE(KeyValueWrapper::contains(addTwoHandle));
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
