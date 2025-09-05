#include "SDL3pp/SDL3pp_callbackWrapper.h"
#include "doctest.h"

TEST_CASE("UniqueCallbackWrapper no params")
{
  using CallbackType = std::function<void()>;
  int counter = 0;
  auto addCounterCB = [&]() { counter++; };
  using UniqueCallbackWrapper = SDL::UniqueCallbackWrapper<CallbackType>;
  void* addCounterHandle = UniqueCallbackWrapper::Wrap(addCounterCB);
  CHECK(addCounterHandle != nullptr);
  SUBCASE("contains")
  {
    CHECK(UniqueCallbackWrapper::contains(addCounterHandle));
    CHECK_FALSE(UniqueCallbackWrapper::contains(nullptr));
    UniqueCallbackWrapper::erase();
    CHECK_FALSE(UniqueCallbackWrapper::contains(addCounterHandle));
  }
  SUBCASE("Call")
  {
    UniqueCallbackWrapper::at(addCounterHandle)();
    CHECK(counter == 1);
    CHECK(UniqueCallbackWrapper::contains(addCounterHandle));
  }
  SUBCASE("CallOnce")
  {
    UniqueCallbackWrapper::release(addCounterHandle)();
    CHECK(counter == 1);
    CHECK_FALSE(UniqueCallbackWrapper::contains(addCounterHandle));
  }
}
TEST_CASE("UniqueCallbackWrapper with params")
{
  using CallbackType = std::function<int(int)>;
  auto addTwoCB = [](int v) { return v + 2; };
  using UniqueCallbackWrapper = SDL::UniqueCallbackWrapper<CallbackType>;
  void* addTwoHandle = UniqueCallbackWrapper::Wrap(addTwoCB);
  CHECK(addTwoHandle != nullptr);
  SUBCASE("Call")
  {
    CHECK(UniqueCallbackWrapper::at(addTwoHandle)(42) == 44);
    CHECK(UniqueCallbackWrapper::contains(addTwoHandle));
  }
  SUBCASE("CallOnce")
  {
    CHECK(UniqueCallbackWrapper::release(addTwoHandle)(13) == 15);
    CHECK_FALSE(UniqueCallbackWrapper::contains(addTwoHandle));
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
    CHECK(KeyValueWrapper::contains(1));
    CHECK_FALSE(KeyValueWrapper::contains(2));
    void* otherHandle = KeyValueWrapper::Wrap(2, addCounterCB);
    CHECK(KeyValueWrapper::contains(2));
  }
  SUBCASE("Call")
  {
    KeyValueWrapper::at(1)();
    CHECK(counter == 1);
    CHECK(KeyValueWrapper::contains(1));
  }
  SUBCASE("CallOnce")
  {
    KeyValueWrapper::release(1)();
    CHECK(counter == 1);
    CHECK_FALSE(KeyValueWrapper::contains(1));
  }
}
