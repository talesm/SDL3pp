#ifndef SDL3PP_RESOURCE_H_
#define SDL3PP_RESOURCE_H_

#include <cstdlib>

namespace SDL {

/**
 * @defgroup CategoryResource Helpers to wrap SDL Resources.
 *
 * A resource is any SDL object that is created and destroyed by SDL, such as a
 * Window, a Renderer, a Texture, etc.
 *
 * @{
 */

/// Reference wrapper for a given resource,
template<typename RAW_POINTER>
class ObjParam
{
public:
  using RawPointer = RAW_POINTER;

  /// Constructs from RawPointer
  constexpr ObjParam(RawPointer resource)
    : m_resource(resource)
  {
  }

  /// Constructs null/invalid
  constexpr ObjParam(std::nullptr_t = nullptr)
    : m_resource(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Comparison
  constexpr auto operator<=>(const ObjParam& other) const = default;

  /// Converts to underlying RawPointer
  constexpr operator RawPointer() const { return m_resource; }

  /// member access to underlying type.
  constexpr auto operator->() const { return m_resource; }

private:
  RawPointer m_resource; ///< parameter's RawPointer
};

/// Const reference wrapper for a given resource,
template<typename RAW_POINTER, typename RAW_CONST_POINTER>
class ObjConstParam
{
public:
  using RawPointer = RAW_POINTER;
  using RawConstPointer = RAW_CONST_POINTER;

  /// Constructs from const pointer.
  constexpr ObjConstParam(RawConstPointer resource)
    : m_resource(resource)
  {
  }

  /// Constructs null/invalid
  constexpr ObjConstParam(std::nullptr_t = nullptr)
    : m_resource(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Comparison.
  constexpr auto operator<=>(const ObjConstParam& other) const = default;

  /// Converts to underlying type.
  constexpr operator RawConstPointer() const { return m_resource; }

  /// Converts to underlying type.
  constexpr operator RawPointer() const
  {
    return const_cast<RawPointer>(m_resource);
  }

  /// member access to underlying type.
  constexpr auto operator->() const { return m_resource; }

private:
  RawConstPointer m_resource; ///< parameter's Surface
};

/// @}

} // namespace SDL

#endif /* SDL3PP_RESOURCE_H_ */
