#include "SDL3pp/SDL3pp_callbackWrapper.h"
#include "doctest.h"

TEST_CASE("CallbackWrapper no params")
{
  using CallbackType = std::function<void()>;
  int counter = 0;
  auto addCounterCB = [&]() { counter++; };
  using CallbackWrapper = SDL::CallbackWrapper<CallbackType>;
  void* addCounterHandle = CallbackWrapper::Wrap(addCounterCB);
  CHECK(addCounterHandle != nullptr);
  SUBCASE("contains")
  {
    CHECK(CallbackWrapper::contains(addCounterHandle));
    CHECK_FALSE(CallbackWrapper::contains((void*)2));
    void* otherHandle = CallbackWrapper::Wrap(addCounterCB);
    CHECK(CallbackWrapper::contains(otherHandle));
    CHECK(addCounterHandle != otherHandle);
  }
  SUBCASE("Call")
  {
    CallbackWrapper::Call(addCounterHandle);
    CHECK(counter == 1);
    CHECK(CallbackWrapper::contains(addCounterHandle));
  }
  SUBCASE("CallOnce")
  {
    CallbackWrapper::CallOnce(addCounterHandle);
    CHECK(counter == 1);
    CHECK_FALSE(CallbackWrapper::contains(addCounterHandle));
  }
}
TEST_CASE("CallbackWrapper with params")
{
  using CallbackType = std::function<int(int)>;
  auto addTwoCB = [](int v) { return v + 2; };
  using CallbackWrapper = SDL::CallbackWrapper<CallbackType>;
  void* addTwoHandle = CallbackWrapper::Wrap(addTwoCB);
  CHECK(addTwoHandle != nullptr);
  SUBCASE("Call")
  {
    CHECK(CallbackWrapper::Call(addTwoHandle, 42) == 44);
    CHECK(CallbackWrapper::contains(addTwoHandle));
  }
  SUBCASE("CallOnce")
  {
    CHECK(CallbackWrapper::CallOnce(addTwoHandle, 13) == 15);
    CHECK_FALSE(CallbackWrapper::contains(addTwoHandle));
  }
}
