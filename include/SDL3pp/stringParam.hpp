#ifndef SDL3PP_STRINGWRAPPER_HPP_
#define SDL3PP_STRINGWRAPPER_HPP_

#include <string>
#include <string_view>
#include <SDL3/SDL_stdinc.h>

namespace SDL {

#ifndef SDLPP_ENABLE_STRING_PARAM

#ifndef SDLPP_DISABLE_STRING_PARAM
#define SDLPP_ENABLE_STRING_PARAM
#endif // SDLPP_DISABLE_STRING_PARAM

#endif // SDLPP_ENABLE_STRING_PARAM

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
public:
  constexpr StringParamImpl(std::nullptr_t = nullptr) {}
  constexpr StringParamImpl(const char* str)
    : str(str)
  {
  }
  StringParamImpl(const std::string& str)
    : StringParamImpl(str.c_str())
  {
  }
  StringParamImpl(std::string_view str)
  {
    auto sz = str.size();
    char* data;
    if (sz < sizeof(smallStr)) {
      mode = 2;
      data = smallStr;
    } else {
      mode = 1;
      data = new char[sz + 1];
      this->str = data;
    }
    SDL_memcpy(data, str.data(), sz);
    data[sz] = 0;
  }

  StringParamImpl(const StringParamImpl&) = delete;
  StringParamImpl(StringParamImpl&& lhs)
    : mode(lhs.mode)
  {
    if (mode == 2) {
      SDL_memcpy(smallStr, lhs.smallStr, sizeof(smallStr));
    } else {
      str = lhs.str;
    }
    lhs.mode = 0;
  }
  ~StringParamImpl()
  {
    if (mode == 1) delete[] str;
  }
  StringParamImpl& operator=(const StringParamImpl&) = delete;
  StringParamImpl& operator=(StringParamImpl&& lhs) = delete;

  operator const char*() const { return mode == 2 ? smallStr : str; }

private:
  union
  {
    const char* str = nullptr;
    char smallStr[sizeof(size_t) * 2 - 1];
  };
  // 0: not owning, 1: owning, 2: small
  int mode = 0;
};
#ifdef SDLPP_ENABLE_STRING_PARAM
using StringParam = StringParamImpl;
#else  // SDLPP_ENABLE_STRING_PARAM
using StringParam = const char*;
#endif // SDLPP_ENABLE_STRING_PARAM
} // namespace SDL

#endif /* SDL3PP_STRINGWRAPPER_HPP_ */
