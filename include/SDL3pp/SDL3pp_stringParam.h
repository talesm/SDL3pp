#ifndef SDL3PP_STRING_PARAM_H_
#define SDL3PP_STRING_PARAM_H_

#include <string>
#include <string_view>
#include <variant>

namespace SDL {

#ifndef SDLPP_ENABLE_STRING_PARAM

#ifndef SDLPP_DISABLE_STRING_PARAM
#define SDLPP_ENABLE_STRING_PARAM
#endif // SDLPP_DISABLE_STRING_PARAM

#endif // SDLPP_ENABLE_STRING_PARAM''

/**
 * @brief A safe and mostly efficient wrapper to std::string and
 * std::string_view parameters
 *
 * This should only be declared in [function
 * parameters](https://en.cppreference.com/w/cpp/language/expressions#Full-expressions),
 * using it otherwise is to ask for undefined behavior
 */
class StringParamImpl
{
  std::variant<std::monostate, const char*, std::string> data;

public:
  constexpr StringParamImpl(std::nullptr_t = nullptr) {}
  constexpr StringParamImpl(const char* str)
    : data(str)
  {
  }

  StringParamImpl(const std::string& str)
    : StringParamImpl(str.c_str())
  {
  }

  StringParamImpl(std::string&& str)
    : data(std::move(str))
  {
  }

  StringParamImpl(std::string_view str)
    : StringParamImpl(std::string{str})
  {
  }

  StringParamImpl(const StringParamImpl&) = delete;
  StringParamImpl(StringParamImpl&&) = default;
  StringParamImpl& operator=(const StringParamImpl&) = delete;
  StringParamImpl& operator=(StringParamImpl&&) = default;

  operator const char*() const
  {
    struct Visitor
    {
      const char* operator()(const char* a) const { return a; }
      const char* operator()(const std::string& s) const { return s.c_str(); }
      const char* operator()(std::monostate) const { return ""; }
    };
    return std::visit(Visitor{}, data);
  }
};
#ifdef SDLPP_ENABLE_STRING_PARAM
using StringParam = StringParamImpl;
#else  // SDLPP_ENABLE_STRING_PARAM
using StringParam = const char*;
#endif // SDLPP_ENABLE_STRING_PARAM
} // namespace SDL

#endif /* SDL3PP_STRING_PARAM_H_ */
