#include "SDL3pp/callbackWrapper.hpp"
#include "doctest.h"

TEST_CASE("CallbackWrapper no params")
{
  using CallbackType = void();
  int counter = 0;
  auto addCounterCB = [&]() { counter++; };
  using CallbackWrapper = SDL::CallbackWrapper<CallbackType>;
  void* addCounterHandle = CallbackWrapper::Wrap(addCounterCB);
  CHECK(addCounterHandle != nullptr);
  SUBCASE("Has")
  {
    CHECK(CallbackWrapper::Has(addCounterHandle));
    CHECK_FALSE(CallbackWrapper::Has((void*)2));
    void* otherHandle = CallbackWrapper::Wrap(addCounterCB);
    CHECK(CallbackWrapper::Has(otherHandle));
    CHECK(addCounterHandle != otherHandle);
  }
  SUBCASE("Call")
  {
    CallbackWrapper::Call(addCounterHandle);
    CHECK(counter == 1);
    CHECK(CallbackWrapper::Has(addCounterHandle));
  }
  SUBCASE("CallOnce")
  {
    CallbackWrapper::CallOnce(addCounterHandle);
    CHECK(counter == 1);
    CHECK_FALSE(CallbackWrapper::Has(addCounterHandle));
  }
}
TEST_CASE("CallbackWrapper with params")
{
  using CallbackType = int(int);
  auto addTwoCB = [](int v) { return v + 2; };
  using CallbackWrapper = SDL::CallbackWrapper<CallbackType>;
  void* addTwoHandle = CallbackWrapper::Wrap(addTwoCB);
  CHECK(addTwoHandle != nullptr);
  SUBCASE("Call")
  {
    CHECK(CallbackWrapper::Call(addTwoHandle, 42) == 44);
    CHECK(CallbackWrapper::Has(addTwoHandle));
  }
  SUBCASE("CallOnce")
  {
    CHECK(CallbackWrapper::CallOnce(addTwoHandle, 13) == 15);
    CHECK_FALSE(CallbackWrapper::Has(addTwoHandle));
  }
}
