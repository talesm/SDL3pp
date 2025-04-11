#ifndef SDL3PP_RESOURCE_H_
#define SDL3PP_RESOURCE_H_

#include <optional>

namespace SDL {

/**
 * @brief A SDL managed resource.
 *
 * @tparam T the underlying resource type.
 */
template<class T>
class Resource
{
  T m_resource;

public:
  /// Constructs the underlying resource.
  constexpr Resource(T resource = {})
    : m_resource(std::move(resource))
  {
  }

  /// Equivalent to default ctor
  constexpr Resource(std::nullptr_t)
    : m_resource{0}
  {
  }

  /// Equivalent to default ctor
  constexpr Resource(std::nullopt_t)
    : m_resource{0}
  {
  }

  Resource(const Resource& other) = delete;
  Resource(Resource&& other) = delete;
  Resource& operator=(const Resource& other) = delete;
  Resource& operator=(Resource&& other) = delete;

  /// True if contains a valid resource
  constexpr explicit operator bool() const { return m_resource; }

  /// Comparison
  constexpr bool operator==(const Resource& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullopt_t) const { return !m_resource; }

  /// Comparison
  constexpr bool operator==(std::nullptr_t) const { return !m_resource; }

  /// Return contained resource;
  constexpr T get() const { return m_resource; }

  /// Return contained resource and empties or replace value
  constexpr T release(T newResource = {})
  {
    T result = std::move(m_resource);
    m_resource = newResource;
    return result;
  }

  /// Access to fields
  constexpr const T operator->() const { return get(); }

  /// Access to fields
  constexpr T operator->() { return get(); }
};

/**
 * A optional reference to resource.
 *
 * This is meant to be aliased like this:
 *
 * ```cpp
 * using OptionalTexture = OptionalResource<TextureRef, Texture>;
 * ```
 *
 * @tparam REF the *Type*Ref.
 * @tparam UNIQUE the *Type*.
 */
template<class REF, class UNIQUE>
class OptionalResource : public REF
{
  bool m_owning = false;

public:
  using REF::REF;

  /// Constructs from a reference
  constexpr OptionalResource(const REF& other)
    : REF(other)
  {
  }

  /// Constructs from a reference
  constexpr OptionalResource(const UNIQUE& other)
    : REF(other)
  {
  }

  /// Constructs from a moved from unique
  constexpr OptionalResource(UNIQUE&& other)
    : REF(other.release())
    , m_owning(true)
  {
  }

  OptionalResource(const OptionalResource& other) = delete;

  /// Move ctor
  OptionalResource(OptionalResource&& other)
    : REF(other.release())
    , m_owning(other.m_owning)
  {
    other.m_owning = false;
  }

  OptionalResource& operator=(const OptionalResource& other) = delete;

  /// Assignment operator.
  OptionalResource& operator=(OptionalResource&& other)
  {
    REF::operator=(REF(other.release()));
    m_owning = other.m_owning;
    other.m_owning = false;
    return *this;
  }

  /// Destructor
  ~OptionalResource()
  {
    if (m_owning) REF::reset();
  }
};

/**
 * A detached reference to resource that might be transformed into an owned
 * handle.
 *
 * This is meant to be aliased like this:
 *
 * ```cpp
 * using DetachedTrayEntry = DetachedResource<TrayEntryRef, TrayEntry>;
 * ```
 *
 * @tparam REF the *Type*Ref.
 * @tparam UNIQUE the *Type*.
 */
template<class REF, class UNIQUE>
struct DetachedResource : REF
{
  using REF::REF;

  DetachedResource(const DetachedResource& other) = delete;

  /// Move ctor
  constexpr DetachedResource(DetachedResource&& other)
    : REF(other.release())
  {
  }

  DetachedResource& operator=(const DetachedResource& other) = delete;

  /// Move assignment
  constexpr DetachedResource& operator=(DetachedResource&& other)
  {
    release(other.release());
    return *this;
  }

  /// Converts to UNIQUE
  operator UNIQUE() { return UNIQUE{REF::release()}; }
};

} // namespace SDL

#endif /* SDL3PP_RESOURCE_H_ */
