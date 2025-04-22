#ifndef SDL3PP_STRINGS_H_
#define SDL3PP_STRINGS_H_

#include <string>
#include <string_view>
#include <variant>

namespace SDL {

/**
 * @defgroup CategoryStrings Helpers to use C++ strings with SDL
 *
 * It has StringParam to use on parameters and StringResult to have
 * a simple std::string-like interface for SDL allocated strings.
 * @{
 */

#ifndef SDL3PP_ENABLE_STRING_PARAM

#ifndef SDL3PP_DISABLE_STRING_PARAM
#define SDL3PP_ENABLE_STRING_PARAM
#endif // SDL3PP_DISABLE_STRING_PARAM

#endif // SDL3PP_ENABLE_STRING_PARAM

#ifdef SDL3PP_ENABLE_STRING_PARAM

/**
 * @brief Helpers to use C++ strings parameters
 *
 * This wraps std::string and std::string_view to the SDL's C interfaces.
 *
 * This should only be declared in [function
 * parameters](https://en.cppreference.com/w/cpp/language/expressions#Full-expressions),
 * using it otherwise is to ask for undefined behavior
 *
 * @ingroup CategoriesCppSupport
 */
class StringParam
{
  std::variant<const char*, std::string> data;

public:
  /// Default ctor
  constexpr StringParam(std::nullptr_t = nullptr)
    : data("")
  {
  }

  /**
   * Constructs from a C string.
   *
   * This assumes the parameter object lifetime is larger than this object,
   * which is always true on parameters, so it just stores the pointer, no copy
   * is done.
   *
   * @param str the string to store. This parameter must outlive this object.
   */
  constexpr StringParam(const char* str)
    : data(str ?: "")
  {
  }

  /**
   * Constructs from std::string reference.
   *
   * This assumes the parameter object lifetime is larger than this object,
   * which is always true on parameters, so it just stores the pointer to
   * internal string data, no copy is done.
   *
   * @param str the reference to a string. This parameter must outlive this
   *            object.
   */
  constexpr StringParam(const std::string& str)
    : StringParam(str.c_str())
  {
  }

  /**
   * Constructs from std::string object.
   *
   * This case we assume the ownership for the string and will properly call
   * destructor automatically.
   *
   * @param str the string to store
   */
  constexpr StringParam(std::string&& str)
    : data(std::move(str))
  {
  }

  /**
   * Constructs from std::string_view object
   *
   * String view are very usefull on C++, but they don't have the null
   * terminator expected by most string SDL APIs, so we always copy its content
   * to a stored std::string.
   *
   * @param str the string_view to store
   */
  StringParam(std::string_view str)
    : StringParam(std::string{str})
  {
  }

  StringParam(const StringParam&) = delete;
  StringParam& operator=(const StringParam&) = delete;

  /// Move ctor
  StringParam(StringParam&&) = default;

  /// Move assignment
  StringParam& operator=(StringParam&&) = default;

  /**
   * Converts to a null terminated C string.
   *
   * @return the C string representation. We guarantee it to be null terminated
   * unless the objects it was constructed from are corrupted.
   */
  constexpr const char* c_str() const
  {
    struct Visitor
    {
      const char* operator()(const char* a) const { return a; }
      const char* operator()(const std::string& s) const { return s.c_str(); }
    };
    return std::visit(Visitor{}, data);
  }

  /**
   * Converts to a null terminated C string.
   *
   * @return the C string representation. We guarantee it to be null terminated
   * unless the objects it was constructed from are corrupted.
   */
  constexpr operator const char*() const { return c_str(); }
};

#else // SDL3PP_ENABLE_STRING_PARAM

using StringParam = const char*;

#endif // SDL3PP_ENABLE_STRING_PARAM

/// @}

} // namespace SDL

#endif /* SDL3PP_STRINGS_H_ */
