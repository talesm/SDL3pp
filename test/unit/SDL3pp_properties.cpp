#include "SDL3pp/SDL3pp_properties.h"
#include "doctest.h"

namespace SDL {

TEST_CASE("Properties")
{
  Properties props;
  CHECK(CountProperties(props) == 0);
  CHECK(HasProperty(props, "potato") == false);
  CHECK(GetPropertyType(props, "potato") == PROPERTY_TYPE_INVALID);
  SetNumberProperty(props, "potato", 42);
  CHECK(HasProperty(props, "potato") == true);
  CHECK(GetPropertyType(props, "potato") == SDL_PROPERTY_TYPE_NUMBER);
  CHECK(GetNumberProperty(props, "potato", 0) == 42);
  CHECK(GetPointerProperty(props, "potato", nullptr) == nullptr);
  CHECK(CountProperties(props) == 1);

  std::string element;
  EnumerateProperties(props,
                      [&element](auto, const char* key) { element = key; });
  CHECK(element == "potato");
}

TEST_CASE("Property .Set()")
{
  Properties props;

  CHECK_NOTHROW(props.Set("Hey", "YOU"));
  CHECK(props.GetPropertyType("Hey") == PROPERTY_TYPE_STRING);
  CHECK(props.GetPropertyType("HEY") == PROPERTY_TYPE_INVALID);
  CHECK_NOTHROW(props.Set("Hey2", std::string("YOU")));
  CHECK(props.GetPropertyType("Hey2") == PROPERTY_TYPE_STRING);
  CHECK_NOTHROW(props.Set("Hey3", true));
  CHECK(props.GetPropertyType("Hey3") == PROPERTY_TYPE_BOOLEAN);
  CHECK_NOTHROW(props.Set("HeyInt", 20));
  CHECK(props.GetPropertyType("HeyInt") == PROPERTY_TYPE_NUMBER);
  CHECK_NOTHROW(props.Set("HeyFloat", 2.5f));
  CHECK(props.GetPropertyType("HeyFloat") == PROPERTY_TYPE_FLOAT);

  int a = 42;
  CHECK_NOTHROW(props.Set("HeyPointer", &a));
  CHECK(props.GetPropertyType("HeyPointer") == PROPERTY_TYPE_POINTER);
}

} // namespace SDL
