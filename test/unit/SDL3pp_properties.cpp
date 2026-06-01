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

  bool IsValid() const { return m_props.HasProperty(m_name.c_str()); }

  PropertyType GetType() const
  {
    return m_props.GetPropertyType(m_name.c_str());
  }

  const char* GetName() const { return m_name; }

  operator void*() const { return m_props.GetPointerProperty(m_name.c_str()); }

  operator const char*() const
  {
    return m_props.GetStringProperty(m_name.c_str());
  }

  template<std::integral T>
  operator T() const
  {
    return T(m_props.GetNumberProperty(m_name.c_str()));
  }

  operator float() const { return m_props.GetFloatProperty(m_name.c_str()); }

  operator bool() const { return m_props.GetBooleanProperty(m_name.c_str()); }

  template<class T>
  auto visit(T visitor) const
  {
    switch (GetType()) {
    case SDL_PROPERTY_TYPE_POINTER: return visitor((void*)(*this));
    case SDL_PROPERTY_TYPE_STRING: return visitor((const char*)(*this));
    case SDL_PROPERTY_TYPE_NUMBER: return visitor(Sint64(*this));
    case SDL_PROPERTY_TYPE_FLOAT: return visitor(float(*this));
    case SDL_PROPERTY_TYPE_BOOLEAN: return visitor(bool(*this));
    default: return visitor(std::nullopt);
    }
  }
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

  auto r = pp.visit([](auto v) { return std::any(v); });
  CHECK_EQ(std::any_cast<float>(r), 10.5f);
}

} // namespace SDL
