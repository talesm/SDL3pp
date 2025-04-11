#ifndef SDL3PP_OWN_PTR_H_
#define SDL3PP_OWN_PTR_H_

#include <memory>
#include <span>

namespace SDL {

/**
 * @defgroup CategoryOwnPtr Pointer wrapper to SDL::free()
 *
 * Wraps SDL generated pointers to automatically freeing them.
 * @{
 */

/// Calls SDL_free()
struct PtrDeleter
{
  void operator()(void* ptr) const;
};

/**
 * Handle to an owned SDL memory allocated pointer
 *
 * @cat resource
 *
 * @sa resource
 * @sa PtrBase
 * @sa RefPtr
 */
template<class T>
using OwnPtr = std::unique_ptr<T, PtrDeleter>;

/**
 * Base class for SDL memory allocated array wrap
 *
 * @tparam T the wrapped array type, without the []
 *
 * @cat resource
 */
template<class T>
class OwnArray
{
  OwnPtr<T> m_ptr;
  size_t m_size = 0;

public:
  constexpr OwnArray(std::nullptr_t = nullptr) {}

  constexpr explicit OwnArray(T* ptr, size_t size)
    : m_ptr(ptr)
    , m_size(size)
  {
  }

  constexpr explicit OwnArray(T* ptr)
    : m_ptr(ptr)
  {
    if (ptr) {
      auto endPtr = ptr;
      while (*endPtr) ++endPtr;
      m_size = endPtr - ptr;
    }
  }

  /// True if not empty
  constexpr explicit operator bool() const { return bool(m_ptr); }

  /// True if size() == 0
  constexpr bool empty() const { return !m_ptr; }

  /// Data
  constexpr T* data() { return m_ptr.get(); }

  /// Data
  constexpr const T* data() const { return m_ptr.get(); }

  /// Size
  constexpr size_t size() const { return m_size; }

  /// Access index
  constexpr T& operator[](size_t i) { return m_ptr.get()[i]; }

  /// Access index
  constexpr const T& operator[](size_t i) const { return m_ptr.get()[i]; }

  /**
   * @{
   *
   * Get iterator to first element
   */
  T* begin() { return data(); }
  const T* begin() const { return data(); }
  const T* cbegin() const { return begin(); }
  /// @}

  /**
   * @{
   *
   * Get iterator to one past end element
   */
  T* end() { return begin() + size(); }
  const T* end() const { return begin() + size(); }
  const T* cend() const { return begin() + size(); }
  /// @}
};

/**
 * Handle to an owned SDL memory allocated array
 *
 * @tparam T the wrapped array type, without the []
 *
 * @cat resource
 *
 * @sa resource
 * @sa ArrayBase
 * @sa OwnArray
 * @sa RefPtr
 */
template<class T>
using RefArray = std::span<T>;

/// @}

} // namespace SDL
#endif /* SDL3PP_OWN_PTR_H_ */

#include "SDL3pp_stdinc.h"
