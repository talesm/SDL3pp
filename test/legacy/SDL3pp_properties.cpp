#include "SDL3pp/SDL3pp_properties.h"
#include "doctest.h"

TEST_CASE("Properties")
{
  SDL::Properties props = SDL::Properties::Create();
  CHECK(props->GetCount() == 0);
  CHECK(props->Has("potato") == false);
  CHECK(props->GetType("potato") == SDL_PROPERTY_TYPE_INVALID);
  props->SetNumber("potato", 42);
  CHECK(props->Has("potato") == true);
  CHECK(props->GetType("potato") == SDL_PROPERTY_TYPE_NUMBER);
  CHECK(props->GetNumber("potato", 0) == 42);
  CHECK(props->GetPointer("potato", nullptr) == nullptr);
  CHECK(props->GetCount() == 1);

  std::string element;
  props->Enumerate([&](auto props, const char* key) { element = key; });

  CHECK(element == "potato");
}

TEST_CASE("PropertiesID compat")
{
  SDL::PropertiesRef propRef(42);
  SDL_PropertiesID propRaw = propRef.get();
  REQUIRE(propRaw == 42);
}
