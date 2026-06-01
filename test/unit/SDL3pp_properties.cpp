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

class PropertyProxy
{
  PropertiesRef m_props;
  StringParam m_name;

public:
  constexpr PropertyProxy(PropertiesRef props, StringParam name)
    : m_props(props)
    , m_name(std::move(name))
  {
  }

  bool IsValid() { return m_props.HasProperty(m_name.c_str()); }

  PropertyType GetType() { return m_props.GetPropertyType(m_name.c_str()); }

  const char* GetName() const { return m_name; }

  operator void*()
  {
    return m_props.GetPointerProperty(m_name.c_str(), nullptr);
  }
  operator const char*()
  {
    return m_props.GetStringProperty(m_name.c_str(), "");
  }
  template<std::integral T>
  operator T()
  {
    return T(m_props.GetNumberProperty(m_name.c_str(), 0));
  }
  operator float() { return m_props.GetFloatProperty(m_name.c_str(), 0); }
  operator bool() { return m_props.GetBooleanProperty(m_name.c_str(), false); }
};

TEST_CASE("PropertyProxy")
{
  Properties props;
  PropertyProxy pp{props, "key"};
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
}

} // namespace SDL
