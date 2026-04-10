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

/// Base class for resources.
template<typename RAW_POINTER, typename RAW_CONST_POINTER = RAW_POINTER>
class ResourceBase
{
public:
  /// The underlying raw pointer type.
  using RawPointer = RAW_POINTER;

  /// The underlying const raw pointer type.
  using RawConstPointer = RAW_CONST_POINTER;

  /// Constructs from resource pointer.
  constexpr ResourceBase(RawPointer resource)
    : m_resource(resource)
  {
  }

  /// Constructs null/invalid
  constexpr ResourceBase(std::nullptr_t = nullptr)
    : m_resource{}
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Comparison
  constexpr auto operator<=>(const ResourceBase& other) const = default;

  /// member access to underlying resource pointer.
  constexpr RawConstPointer operator->() const noexcept { return m_resource; }

  /// member access to underlying resource pointer.
  constexpr RawPointer operator->() noexcept { return m_resource; }

  /// Retrieves underlying resource pointer.
  constexpr RawPointer get() const noexcept { return m_resource; }

  /// Retrieves underlying resource pointer and clear this.
  constexpr RawPointer release() noexcept
  {
    auto r = m_resource;
    m_resource = {};
    return r;
  }

  friend constexpr void swap(ResourceBase& lhs, ResourceBase& rhs) noexcept
  {
    std::swap(lhs.m_resource, rhs.m_resource);
  }

private:
  RawPointer m_resource; ///< parameter's RawPointer
};

/// Reference wrapper for a given resource,
template<typename RAW_POINTER>
class ResourceLegacyRef
{
public:
  /// The underlying raw pointer type.
  using RawPointer = RAW_POINTER;

  /// Constructs from RawPointer
  constexpr ResourceLegacyRef(RawPointer resource)
    : m_resource(resource)
  {
  }

  /// Constructs null/invalid
  constexpr ResourceLegacyRef(std::nullptr_t = nullptr)
    : m_resource(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Comparison
  constexpr auto operator<=>(const ResourceLegacyRef& other) const = default;

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

/// A non-owning reference wrapper for a given resource
template<typename RESOURCE>
struct ResourceRef : RESOURCE
{
  using RESOURCE::RESOURCE;

  /// The underlying raw pointer type.
  using RawPointer = RESOURCE::RawPointer;

  /// The underlying const raw pointer type.
  using RawConstPointer = RESOURCE::RawConstPointer;

  /**
   * Constructs from raw resource.
   *
   * @param resource a raw pointer.
   *
   * This does not takes ownership!
   */
  constexpr ResourceRef(RawPointer resource) noexcept
    : RESOURCE(resource)
  {
  }

  /**
   * Constructs from resource.
   *
   * @param resource a RESOURCE.
   *
   * This does not takes ownership!
   */
  constexpr ResourceRef(const RESOURCE& resource) noexcept
    : RESOURCE(resource.get())
  {
  }

  /**
   * Constructs from RESOURCE.
   *
   * @param resource a RESOURCE.
   *
   * This will release the ownership from resource!
   */
  constexpr ResourceRef(RESOURCE&& resource) noexcept
    : RESOURCE(std::move(resource).release())
  {
  }

  /// Copy constructor.
  constexpr ResourceRef(const ResourceRef& other) noexcept
    : RESOURCE(other.get())
  {
  }

  /// Move constructor.
  constexpr ResourceRef(ResourceRef&& other) noexcept
    : RESOURCE(other.get())
  {
  }

  /// Destructor
  ~ResourceRef() { this->release(); }

  /// Assignment operator.
  ResourceRef& operator=(const ResourceRef& other) noexcept
  {
    this->release();
    RESOURCE::operator=(RESOURCE(other.get()));
    return *this;
  }

  /// Converts to raw pointer.
  constexpr operator RawPointer() const noexcept { return this->get(); }
};

/// @}

} // namespace SDL

#endif /* SDL3PP_RESOURCE_H_ */
