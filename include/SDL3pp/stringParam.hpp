#ifndef SDL3PP_STRINGWRAPPER_HPP_
#define SDL3PP_STRINGWRAPPER_HPP_

#include <string>
#include <string_view>
#include <SDL3/SDL_stdinc.h>

namespace SDL {

/**
 * @brief A safe and mostly efficient wrapper to std::string and
 * std::string_view parameters
 *
 * This should only be declared in [function
 * parameters](https://en.cppreference.com/w/cpp/language/expressions#Full-expressions),
 * using it otherwise is to ask for undefined behavior
 */
class StringParam
{
public:
  constexpr StringParam(std::nullptr_t = nullptr) {}
  constexpr StringParam(const char* str)
    : str(str)
  {
  }
  StringParam(const std::string& str)
    : StringParam(str.c_str())
  {
  }
  StringParam(std::string_view str)
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

  StringParam(const StringParam&) = delete;
  StringParam(StringParam&& lhs)
    : mode(lhs.mode)
  {
    if (mode == 2) {
      SDL_memcpy(smallStr, lhs.smallStr, sizeof(smallStr));
    } else {
      str = lhs.str;
    }
    lhs.mode = 0;
  }
  ~StringParam()
  {
    if (mode == 1) delete[] str;
  }
  StringParam& operator=(const StringParam&) = delete;
  StringParam& operator=(StringParam&& lhs) = delete;

  const char* Get() const { return mode == 2 ? smallStr : str; }

private:
  union
  {
    const char* str = nullptr;
    char smallStr[sizeof(size_t) * 2 - 1];
  };
  // 0: not owning, 1: owning, 2: small
  int mode = 0;
};

} // namespace SDL

#endif /* SDL3PP_STRINGWRAPPER_HPP_ */
