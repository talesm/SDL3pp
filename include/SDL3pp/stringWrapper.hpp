#ifndef SDL3PP_STRING_WRAPPER_HPP
#define SDL3PP_STRING_WRAPPER_HPP

#include <string>
#include <string_view>
#include <SDL3/SDL_stdinc.h>

namespace SDL {

/**
 * @brief A safe and mostly efficient wrapper to std::string and
 * std::string_view
 *
 * This should only be declared in function parameters, using it
 * otherwise is to ask for undefined behavior
 */
class StringWrapper
{
public:
  constexpr StringWrapper(std::nullptr_t = nullptr) {}
  constexpr StringWrapper(const char* str)
    : str(str)
  {
  }
  StringWrapper(const std::string& str)
    : StringWrapper(str.c_str())
  {
  }
  StringWrapper(std::string_view str)
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

  StringWrapper(const StringWrapper&) = delete;
  StringWrapper(StringWrapper&& lhs)
    : mode(lhs.mode)
  {
    if (mode == 2) {
      SDL_memcpy(smallStr, lhs.smallStr, sizeof(smallStr));
    } else {
      str = lhs.str;
    }
    lhs.mode = 0;
  }
  ~StringWrapper()
  {
    if (mode == 1) delete[] str;
  }
  StringWrapper& operator=(const StringWrapper&) = delete;
  StringWrapper& operator=(StringWrapper&& lhs) = delete;

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

#endif /* SDL3PP_STRING_WRAPPER_HPP */
