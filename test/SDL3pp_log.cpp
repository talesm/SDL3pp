#include "SDL3pp/SDL3pp_log.h"
#include "doctest.h"

TEST_CASE("Logging function")
{
  SDL::LogCategory lastCategory;
  SDL::LogPriority lastPriority;
  std::string lastMessage;

  SDL::SetLogOutputFunction([&](auto category, auto priority, auto message) {
    lastCategory = category;
    lastPriority = priority;
    lastMessage = message;
  });

  SUBCASE("Unformatted Log")
  {
    SDL::LogUnformatted("Test unformatted");
    CHECK(lastCategory == SDL::LOG_CATEGORY_APPLICATION);
    CHECK(lastPriority == SDL::LOG_PRIORITY_INFO);
    CHECK(lastMessage == "Test unformatted");
  }
  SUBCASE("formatted Log")
  {
    SDL::Log("Test {}, formatted: {}", 2, true);
    CHECK(lastCategory == SDL::LOG_CATEGORY_APPLICATION);
    CHECK(lastPriority == SDL::LOG_PRIORITY_INFO);
    CHECK(lastMessage == "Test 2, formatted: true");
  }

  SDL::ResetLogPriorities();
  SDL::ResetLogOutputFunction();
}
