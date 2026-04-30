#include "SDL3pp/SDL3pp_log.h"
#include "doctest.h"

TEST_CASE("Logging function")
{
  struct Data
  {
    SDL::LogCategory lastCategory;
    SDL::LogPriority lastPriority;
    std::string lastMessage;
  };
  Data data;

  SDL::SetLogOutputFunction(
    [&data](auto category, auto priority, auto message) {
      data.lastCategory = SDL::LogCategory(category);
      data.lastPriority = priority;
      data.lastMessage = message;
    });

  SUBCASE("Unformatted Log")
  {
    SDL::LogUnformatted("Test unformatted");
    CHECK(data.lastCategory == SDL::LOG_CATEGORY_APPLICATION);
    CHECK(data.lastPriority == SDL::LOG_PRIORITY_INFO);
    CHECK(data.lastMessage == "Test unformatted");
  }
  SUBCASE("formatted Log")
  {
    SDL::Log("Test {}, formatted: {}", 2, true);
    CHECK(data.lastCategory == SDL::LOG_CATEGORY_APPLICATION);
    CHECK(data.lastPriority == SDL::LOG_PRIORITY_INFO);
    CHECK(data.lastMessage == "Test 2, formatted: true");
  }

  SDL::ResetLogPriorities();
  SDL::ResetLogOutputFunction();
}
