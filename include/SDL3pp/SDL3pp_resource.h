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

/// Base class for non owned resources.
template<typename RAW_POINTER, typename RAW_CONST_POINTER = RAW_POINTER>
class NonOwnedResourceBase
{
public:
  /// The underlying raw pointer type.
  using RawPointer = RAW_POINTER;

  /// The underlying const raw pointer type.
  using RawConstPointer = RAW_CONST_POINTER;

  /// Constructs from resource pointer.
  constexpr NonOwnedResourceBase(RawPointer resource)
    : m_resource(resource)
  {
  }

  /// Constructs null/invalid
  constexpr NonOwnedResourceBase(std::nullptr_t = nullptr)
    : m_resource(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Comparison
  constexpr auto operator<=>(const NonOwnedResourceBase& other) const = default;

  /// Converts to underlying resource pointer.
  constexpr operator RawPointer() const noexcept { return m_resource; }

  /// Retrieves underlying resource pointer.
  constexpr RawPointer get() const noexcept { return m_resource; }

  /// Retrieves underlying resource pointer and clear this.
  constexpr RawPointer release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

private:
  RawPointer m_resource; ///< parameter's RawPointer
};

/// Reference wrapper for a given resource,
template<typename RAW_POINTER>
class ResourceRef
{
public:
  /// The underlying raw pointer type.
  using RawPointer = RAW_POINTER;

  /// Constructs from RawPointer
  constexpr ResourceRef(RawPointer resource)
    : m_resource(resource)
  {
  }

  /// Constructs null/invalid
  constexpr ResourceRef(std::nullptr_t = nullptr)
    : m_resource(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Comparison
  constexpr auto operator<=>(const ResourceRef& other) const = default;

  /// Converts to underlying RawPointer
  constexpr operator RawPointer() const { return m_resource; }

  /// member access to underlying type.
  constexpr auto operator->() const { return m_resource; }

private:
  RawPointer m_resource; ///< parameter's RawPointer
};

/// Const reference wrapper for a given resource,
template<typename RAW_POINTER, typename RAW_CONST_POINTER>
class ResourceConstRef
{
public:
  /// The underlying raw pointer type.
  using RawPointer = RAW_POINTER;

  /// The underlying const raw pointer type.
  using RawConstPointer = RAW_CONST_POINTER;

  /// Constructs from const pointer.
  constexpr ResourceConstRef(RawConstPointer resource)
    : m_resource(resource)
  {
  }

  /// Constructs null/invalid
  constexpr ResourceConstRef(std::nullptr_t = nullptr)
    : m_resource(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Comparison.
  constexpr auto operator<=>(const ResourceConstRef& other) const = default;

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
