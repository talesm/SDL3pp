#include "SDL3pp/SDL3pp_surface.h"
#include "doctest.h"

namespace SDL {

SCENARIO("Creating surface")
{
  GIVEN("A size and a pixel format")
  {
    auto size = SDL::Point{400, 400};
    auto format = SDL::PIXELFORMAT_RGBA32;
    WHEN("Create surface")
    {
      auto surface = SDL::Surface(size, format);
      THEN("Surface is created")
      {
        CHECK(surface);
        CHECK(surface.GetSize() == size);
        CHECK(surface.GetFormat() == format);
      }
    }
  }
}

SCENARIO("Reference counting")
{
  GIVEN("A surface")
  {
    auto surface = SDL::Surface({400, 400}, SDL::PIXELFORMAT_RGBA32);
    REQUIRE(surface);
    WHEN("Copy surface")
    {
      auto copy = surface;
      THEN("Reference count is increased")
      {
        CHECK(surface->refcount == 2);
        CHECK(copy->refcount == 2);
      }
      AND_WHEN("Destroy copy")
      {
        copy = nullptr;
        THEN("Reference count is decreased") { CHECK(surface->refcount == 1); }
      }
      AND_WHEN("Assign copy to another surface")
      {
        Surface copy2 = surface;
        THEN("Reference count is increased")
        {
          CHECK(surface->refcount == 3);
          CHECK(copy2->refcount == 3);
        }
        AND_WHEN("Destroy copy2")
        {
          copy2 = nullptr;
          THEN("Reference count is decreased")
          {
            CHECK(surface->refcount == 2);
          }
        }
      }
      AND_WHEN("Assign move other to another surface")
      {
        copy = SDL::Surface({400, 400}, SDL::PIXELFORMAT_RGBA32);
        THEN("Reference count is unchanged")
        {
          CHECK(surface->refcount == 1);
          CHECK(copy->refcount == 1);
        }
      }
    }
    WHEN("Move surface")
    {
      auto moved = std::move(surface);
      THEN("Reference count is unchanged") { CHECK(moved->refcount == 1); }
    }
    WHEN("Copy to a SurfaceRef")
    {
      auto copy = SDL::SurfaceRef(surface);
      THEN("Reference count is not increased")
      {
        CHECK(surface->refcount == 1);
        CHECK(copy->refcount == 1);
      }
      AND_WHEN("Destroy copy")
      {
        copy = nullptr;
        THEN("Reference count is unchanged") { CHECK(surface->refcount == 1); }
      }
      AND_WHEN("Assign copy to another SurfaceRef")
      {
        copy = surface;
        THEN("Reference count is not increased")
        {
          CHECK(surface->refcount == 1);
          CHECK(copy->refcount == 1);
        }
        AND_WHEN("Destroy copy2")
        {
          copy = nullptr;
          THEN("Reference count is unchanged")
          {
            CHECK(surface->refcount == 1);
          }
        }
      }
      AND_WHEN("Copy SurfaceRef to another Surface")
      {
        Surface copy2(copy);
        THEN("Reference count is increased")
        {
          CHECK(surface->refcount == 2);
          CHECK(copy->refcount == 2);
          CHECK(copy2->refcount == 2);
        }
        AND_WHEN("Destroy copy2")
        {
          copy2 = nullptr;
          THEN("Reference count is unchanged")
          {
            CHECK(surface->refcount == 1);
            CHECK(copy->refcount == 1);
          }
        }
      }
    }
  }
}

SCENARIO("Locking surface")
{
  GIVEN("A surface")
  {
    auto surface = SDL::Surface({400, 400}, SDL_PIXELFORMAT_RGBA32);
    REQUIRE(surface);
    WHEN("Check if must lock")
    {
      THEN("Results is false") { CHECK(surface.MustLock() == false); }
    }
  }
}

} // namespace SDL
