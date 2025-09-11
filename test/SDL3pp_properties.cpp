#include "SDL3pp/SDL3pp_properties.h"
#include "doctest.h"

TEST_CASE("Properties")
{
  SDL::Properties props = SDL::Properties::Create();
  CHECK(SDL::CountProperties(props) == 0);
  CHECK(SDL::HasProperty(props, "potato") == false);
  CHECK(SDL::GetPropertyType(props, "potato") == SDL::PROPERTY_TYPE_INVALID);
  SDL::SetNumberProperty(props, "potato", 42);
  CHECK(SDL::HasProperty(props, "potato") == true);
  CHECK(SDL::GetPropertyType(props, "potato") == SDL_PROPERTY_TYPE_NUMBER);
  CHECK(SDL::GetNumberProperty(props, "potato", 0) == 42);
  CHECK(SDL::GetPointerProperty(props, "potato", nullptr) == nullptr);
  CHECK(SDL::CountProperties(props) == 1);

  std::string element;
  SDL::EnumerateProperties(props,
                           [&](auto props, const char* key) { element = key; });
  CHECK(element == "potato");
}
