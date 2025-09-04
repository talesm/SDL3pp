#define SDL3PP_APPCLASS_LOG_PRIORITY LOG_PRIORITY_TRACE

#include "SDL3pp/SDL3pp_init.h"
#include "doctest.h"

namespace SDL {

}

TEST_CASE("InitClass()")
{
  SUBCASE("DefaultClassCreate with default ctor")
  {
    struct Dummy
    {
      int cookie;
      constexpr Dummy()
        : cookie(42)
      {
      }

      constexpr Dummy(int _)
        : cookie(13)
      {
      }
    }* dummy = nullptr;
    CHECK(SDL::InitClass(&dummy, {}) == SDL::APP_CONTINUE);
    REQUIRE(dummy != nullptr);
    CHECK(dummy->cookie == 42);
    delete dummy;
  }

  SUBCASE("DefaultClassCreate with throwing default ctor")
  {
    struct Dummy
    {
      Dummy() { throw std::runtime_error{"Error"}; }

    }* dummy = nullptr;
    CHECK(SDL::InitClass(&dummy, {}) == SDL::APP_FAILURE);
    REQUIRE(dummy == nullptr);
  }

  SUBCASE("DefaultClassCreate with AppArgs ctor")
  {
    struct Dummy
    {
      int cookie;
      constexpr Dummy()
        : cookie(42)
      {
      }

      constexpr Dummy(SDL::AppArgs _)
        : cookie(13)
      {
      }
    }* dummy = nullptr;
    CHECK(SDL::InitClass(&dummy, {}) == SDL::APP_CONTINUE);
    REQUIRE(dummy != nullptr);
    CHECK(dummy->cookie == 13);
    delete dummy;
  }

  SUBCASE("InitClass non-initialized success")
  {
    struct Dummy
    {
      static SDL::AppResult Init(Dummy** state, SDL::AppArgs _)
      {
        return SDL::APP_SUCCESS;
      }
    }* dummy;
    CHECK(SDL::InitClass(&dummy, {}) == SDL::APP_SUCCESS);
    REQUIRE(dummy == nullptr);
  }

  SUBCASE("InitClass non-initialized continue")
  {
    struct Dummy
    {
      static SDL::AppResult Init(Dummy** state, SDL::AppArgs _)
      {
        return SDL::APP_CONTINUE;
      }
    }* dummy;
    CHECK(SDL::InitClass(&dummy, {}) == SDL::APP_SUCCESS);
    REQUIRE(dummy == nullptr);
  }

  SUBCASE("InitClass non-initialized failure")
  {
    struct Dummy
    {
      static SDL::AppResult Init(Dummy** state, SDL::AppArgs _)
      {
        return SDL::APP_FAILURE;
      }
    }* dummy;
    CHECK(SDL::InitClass(&dummy, {}) == SDL::APP_FAILURE);
    REQUIRE(dummy == nullptr);
  }

  SUBCASE("InitClass non-initialized throws")
  {
    struct Dummy
    {
      static SDL::AppResult Init(Dummy** state, SDL::AppArgs _)
      {
        throw std::runtime_error{"Error"};
      }
    }* dummy;
    CHECK(SDL::InitClass(&dummy, {}) == SDL::APP_FAILURE);
    REQUIRE(dummy == nullptr);
  }

  SUBCASE("InitClass initialized success")
  {
    struct Dummy
    {
      int cookie;
      static SDL::AppResult Init(Dummy** state, SDL::AppArgs _)
      {
        *state = new Dummy{42};
        return SDL::APP_SUCCESS;
      }
    }* dummy;
    CHECK(SDL::InitClass(&dummy, {}) == SDL::APP_SUCCESS);
    REQUIRE(dummy != nullptr);
    CHECK(dummy->cookie == 42);
    delete dummy;
  }

  SUBCASE("InitClass initialized continue")
  {
    struct Dummy
    {
      int cookie;
      static SDL::AppResult Init(Dummy** state, SDL::AppArgs _)
      {
        *state = new Dummy{42};
        return SDL::APP_CONTINUE;
      }
    }* dummy;
    CHECK(SDL::InitClass(&dummy, {}) == SDL::APP_CONTINUE);
    REQUIRE(dummy != nullptr);
    CHECK(dummy->cookie == 42);
    delete dummy;
  }

  SUBCASE("InitClass initialized failure")
  {
    struct Dummy
    {
      int cookie;
      static SDL::AppResult Init(Dummy** state, SDL::AppArgs _)
      {
        *state = new Dummy{42};
        return SDL::APP_FAILURE;
      }
    }* dummy;
    CHECK(SDL::InitClass(&dummy, {}) == SDL::APP_FAILURE);
    REQUIRE(dummy != nullptr);
    CHECK(dummy->cookie == 42);
    delete dummy;
  }

  SUBCASE("InitClass initialized failure")
  {
    struct Dummy
    {
      int cookie;
      static SDL::AppResult Init(Dummy** state, SDL::AppArgs _)
      {
        *state = new Dummy{42};
        throw std::runtime_error{"Error"};
      }
    }* dummy;
    CHECK(SDL::InitClass(&dummy, {}) == SDL::APP_FAILURE);
    REQUIRE(dummy != nullptr);
    CHECK(dummy->cookie == 42);
    delete dummy;
  }
}

TEST_CASE("QuitClass()")
{
  SUBCASE("QuitClass Default Destroy")
  {
    bool called = false;
    struct Dummy
    {
      bool* called;

      ~Dummy() { *called = true; }
    }* dummy;
    dummy = new Dummy{&called};

    SDL::QuitClass(dummy, SDL_APP_SUCCESS);
    CHECK(called == true);
  }
  SUBCASE("QuitClass Quit")
  {
    SDL::AppResult result = SDL::APP_CONTINUE;
    struct Dummy
    {
      SDL::AppResult* result;

      static void Quit(Dummy* dummy, SDL::AppResult result)
      {
        *dummy->result = result;
      }
    }* dummy;
    dummy = new Dummy{&result};

    SDL::QuitClass(dummy, SDL::APP_SUCCESS);
    REQUIRE(result == SDL::APP_SUCCESS);
    SDL::QuitClass(dummy, SDL::APP_FAILURE);
    REQUIRE(result == SDL::APP_FAILURE);
    delete dummy;
  }
}
