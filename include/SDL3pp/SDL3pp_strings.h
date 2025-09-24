#ifndef SDL3PP_STRINGS_H_
#define SDL3PP_STRINGS_H_

#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <SDL3/SDL_stdinc.h>
#include "SDL3pp_ownPtr.h"

namespace SDL {

/**
 * @defgroup CategoryStrings Helpers to use C++ strings and byte arrays.
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

/**
 * A simple std::string-like interface for SDL allocated strings.
 */
struct StringResult : OwnArray<char>
{

  /// Use parent ctors
  using OwnArray::OwnArray;

  /// Copy ctor
  StringResult(const StringResult& other)
    : StringResult(std::string_view(other))
  {
  }

  /// Move ctor
  constexpr StringResult(StringResult&& other)
    : OwnArray(other.release(), other.size())
  {
  }

  /// Constructs from string view
  StringResult(std::string_view other)
    : OwnArray(other.empty() ? nullptr
                             : SDL_strndup(other.data(), other.size()))
  {
  }

  /// Convert to StringParam
  constexpr operator StringParam() const { return std::string_view{*this}; }

  /// Convert to std::string_view
  constexpr operator std::string_view() const
  {
    return std::string_view{data(), size()};
  }

  /// Append string.
  StringResult& operator+=(std::string_view other)
  {
    if (empty()) {
      reset(StringResult(other).release());
    } else if (!other.empty()) {
      size_t lhsSz = size();
      size_t rhsSz = other.size();
      size_t finalSize = lhsSz + lhsSz + 1;
      auto newBuf = static_cast<char*>(SDL_realloc(data(), finalSize));
      newBuf += lhsSz;
      SDL_memcpy(newBuf, other.data(), rhsSz);
      newBuf += rhsSz;
      *newBuf = 0;
      reset(newBuf, finalSize - 1);
    }
    return *this;
  }

  /// Append char.
  StringResult& operator+=(char ch)
  {
    return (*this) += std::string_view{&ch, 1};
  }

  /// Append string.
  StringResult operator+(std::string_view other) const
  {
    StringResult t{*this};
    t += other;
    return t;
  }

  /// Append char.
  StringResult operator+(char ch) { return (*this) + std::string_view{&ch, 1}; }

  /// Convert to string.
  std::string str() const { return std::string{data(), size()}; }

  /// Convert to c-string.
  const char* c_str() const
  {
    if (empty()) return "";
    return data();
  }
};

/**
 * Source byte stream.
 *
 * Source byte stream, tipically used as source where bytes are copied from.
 */
struct SourceBytes
{
  const void* data;  ///< The data copied from
  size_t size_bytes; ///< The size in bytes

  /// Default ctor
  constexpr SourceBytes() = default;

  /// Disambiguate between multiple pointer types.
  constexpr SourceBytes(std::nullptr_t)
    : SourceBytes()
  {
  }

  /**
   * From span
   *
   * @param span the data.
   *
   */
  template<class T, size_t N>
  constexpr SourceBytes(std::span<T, N> span)
    : SourceBytes(span.data(), span.size_bytes())
  {
  }

  /**
   * From container-like
   *
   * @param data the data.
   */
  template<class T>
  constexpr SourceBytes(const T& data)
    : SourceBytes(std::span{data.data(), data.size()})
  {
  }
  /**
   * From array
   *
   * @param data the data.
   */
  template<class T, size_t N>
  constexpr SourceBytes(T (&data)[N])
    : SourceBytes(std::span<T, N>{static_cast<T*>(data), N})
  {
  }

  /**
   * From data + size in bytes.
   *
   * @param data the data.
   * @param size_bytes size in @b bytes!
   *
   */
  constexpr SourceBytes(const void* data, size_t size_bytes)
    : data(size_bytes > 0 ? data : nullptr)
    , size_bytes(data != nullptr ? size_bytes : 0)
  {
  }
};

/**
 * Target byte stream.
 *
 * Target byte stream, tipically used as target where bytes are copied to.
 */
struct TargetBytes
{
  void* data = nullptr;  ///< The address to have data copied to it
  size_t size_bytes = 0; ///< The size in bytes

  /// Default ctor
  constexpr TargetBytes() = default;

  /// Disambiguate between multiple pointer types.
  constexpr TargetBytes(std::nullptr_t)
    : TargetBytes()
  {
  }

  constexpr TargetBytes(const TargetBytes& other) = default;

  constexpr TargetBytes(TargetBytes&& other) = default;

  constexpr TargetBytes& operator=(const TargetBytes& other) = default;

  constexpr TargetBytes& operator=(TargetBytes&& other) = default;

  /// Just to have better error message
  template<class T, size_t N>
  constexpr TargetBytes(std::span<const T, N> span)
  {
    static_assert(false, "Non-const type is required");
  }

  /**
   * From span
   *
   * @param span the data.
   *
   */
  template<class T, size_t N>
  constexpr TargetBytes(std::span<T, N> span)
    : TargetBytes(span.data(), span.size_bytes())
  {
  }

  /**
   * From container-like
   *
   * @param data the data.
   */
  template<class T>
  constexpr TargetBytes(T& data)
    : TargetBytes(std::span{data.data(), data.size()})
  {
  }
  /**
   * From array
   *
   * @param data the data.
   */
  template<class T, size_t N>
  constexpr TargetBytes(T (&data)[N])
    : TargetBytes(std::span<T, N>{static_cast<T*>(data), N})
  {
  }

  /**
   * From data + size in bytes.
   *
   * @param data the data.
   * @param size_bytes size in @b bytes!
   *
   */
  constexpr TargetBytes(void* data, size_t size_bytes)
    : data(size_bytes > 0 ? data : nullptr)
    , size_bytes(data != nullptr ? size_bytes : 0)
  {
  }
};

/// @}

} // namespace SDL

#endif /* SDL3PP_STRINGS_H_ */
