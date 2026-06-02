#include "SDL3pp/SDL3pp_properties.h"
#include "doctest.h"
#include <any>

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
  CHECK_NOTHROW(props.Set("HeyInt", 20));
  CHECK(props.GetPropertyType("HeyInt") == PROPERTY_TYPE_NUMBER);
  CHECK_NOTHROW(props.Set("HeyFloat", 2.5f));
  CHECK(props.GetPropertyType("HeyFloat") == PROPERTY_TYPE_FLOAT);
  CHECK_NOTHROW(props.Set("Hey3", true));
  CHECK(props.GetPropertyType("Hey3") == PROPERTY_TYPE_BOOLEAN);
  bool b = false;
  CHECK_NOTHROW(props.Set("HeyBoolean", b));
  CHECK(props.GetPropertyType("HeyBoolean") == PROPERTY_TYPE_BOOLEAN);
  bool& bref = b;
  CHECK_NOTHROW(props.Set("HeyBoolean2", bref));
  CHECK(props.GetPropertyType("HeyBoolean2") == PROPERTY_TYPE_BOOLEAN);

  int a = 42;
  CHECK_NOTHROW(props.Set("HeyPointer", &a));
  CHECK(props.GetPropertyType("HeyPointer") == PROPERTY_TYPE_POINTER);
}

TEST_CASE("Property .Get")
{
  Properties props;
  PropertyProxy pp = props.Get("key");
  CHECK(pp.IsValid() == false);

  props.Set("key", "value");
  CHECK(pp.IsValid() == true);
  CHECK(bool(pp) == true);
  CHECK(std::string(pp) == "value");
  CHECK(Sint64(pp) == 0);
  CHECK(int(pp) == 0);
  CHECK(float(pp) == 0);
  CHECK(double(pp) == 0);
  CHECK((void*)(pp) == nullptr);

  props.Set("key", true);
  CHECK(bool(pp) == true);
  CHECK(std::string(pp) == "true");
  CHECK(Sint64(pp) == 1);
  CHECK(float(pp) == 1);
  CHECK((void*)(pp) == nullptr);

  props.Set("key", 10);
  CHECK(bool(pp) == true);
  CHECK(std::string(pp) == "10");
  CHECK(Sint64(pp) == 10);
  CHECK(float(pp) == 10);
  CHECK((void*)(pp) == nullptr);

  props.Set("key", 10.5f);
  CHECK(bool(pp) == true);
  CHECK(std::string(pp) == "10.500000");
  CHECK(Sint64(pp) == 11);
  CHECK(float(pp) == 10.5f);
  CHECK((void*)(pp) == nullptr);

  auto r = pp.visit([](auto v) { return std::any(v); });
  CHECK_EQ(std::any_cast<float>(r), 10.5f);
}

TEST_CASE("Property.Set(PropertyProxy) ")
{
  Properties props;
  REQUIRE_NOTHROW(props.Set("key", props.Get("non-existing key")));
  CHECK_FALSE(props.HasProperty("key"));

  props.Set("key", 10);
  REQUIRE_NOTHROW(props.Set("key2", props.Get("key")));
  CHECK_EQ(Sint64(props.Get("key")), 10);
  CHECK_EQ(Sint64(props.Get("key2")), 10);
}

TEST_CASE("PropertiesRef.operator[]")
{
  Properties props;
  props.Set("key", 10);
  CHECK_EQ(Sint64(props["key"]), 10);
}

TEST_CASE("Properties ctor with initializer list")
{
  int aInt = 13;
  Properties props{{"key", 10},
                   {"key2", "value"},
                   {"key3", 2.25f},
                   {"key4", true},
                   {"key5", &aInt}};
  CHECK_EQ(Sint64(props["key"]), 10);
  CHECK_EQ(std::string(props["key2"]), "value");
  CHECK_EQ(float(props["key3"]), 2.25f);
  CHECK_EQ(bool(props["key4"]), true);
  CHECK_EQ((void*)(props["key5"]), &aInt);
}

} // namespace SDL
