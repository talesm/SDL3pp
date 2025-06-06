#ifndef SDL3PP_RESOURCE_H_
#define SDL3PP_RESOURCE_H_

#include <optional>

namespace SDL {

/**
 * Concept representing a valid resource handle
 *
 * @tparam T
 * @tparam RESOURCE
 */
template<class T, class RESOURCE>
concept ResourceHandle = requires(const T& test) {
  { *test } -> std::convertible_to<RESOURCE>;
};

/**
 * @brief A SDL managed resource.
 *
 * @tparam T the underlying resource type.
 *
 * @ingroup CategoriesCppSupport
 */
template<class T>
class Resource
{
  T m_resource;

public:
  /// The raw resource type
  using value_type = T;

  /// Constructs from the underlying resource.
  constexpr Resource(T resource = {})
    : m_resource(std::move(resource))
  {
  }

  /// Constructs from pointer like.
  constexpr Resource(const ResourceHandle<Resource<T>> auto& resource)
    : Resource(*resource)
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

  /// True if contains a valid resource
  constexpr explicit operator bool() const { return m_resource; }

  /// Converts back to underlying type
  constexpr operator value_type() const { return m_resource; }

  /// Comparison
  constexpr bool operator==(const Resource& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullopt_t) const { return !m_resource; }

  /// Comparison
  constexpr bool operator==(std::nullptr_t) const { return !m_resource; }

  /// Return contained resource;
  constexpr T get() const { return m_resource; }

  /// Return contained resource and empties or replace value
  /// @deprecated
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

/// Default deleter
template<class RESOURCE>
struct DefaultDeleter
{
  /// Deletes resource
  void operator()(RESOURCE& ref) const { RESOURCE::reset(ref.get()); }
};

/**
 * @brief Base class for resource pointer-like objects.
 *
 * @tparam RESOURCE
 */
template<class RESOURCE, class DELETER>
class ResourcePtrBase
{
public:
  /// The reference resource type
  using reference = RESOURCE;

  /// The raw resource type
  using value_type = typename reference::value_type;

private:
  /// The resource.
  reference m_value;
  DELETER m_deleter;

protected:
  /// Constructs from raw type.
  constexpr ResourcePtrBase(value_type value = {}, DELETER deleter = {})
    : m_value(value)
    , m_deleter(std::move(deleter))
  {
  }

  /// Get reference
  reference& get() { return m_value; }

  /// Frees resource
  void free() { m_deleter(m_value); }

public:
  /// Check if not null
  constexpr operator bool() const { return bool(m_value); }

  /// Comparison
  constexpr bool operator==(const ResourcePtrBase&) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t) const { return !*this; }

  /// Comparison
  constexpr bool operator==(std::nullopt_t) const { return !*this; }

  /// Gets reference.
  constexpr reference operator*() const { return m_value; }

  /// Gets addressable reference.
  constexpr const reference* operator->() const { return &m_value; }

  /// Gets addressable reference.
  constexpr reference* operator->() { return &m_value; }

  /// Get reference
  reference get() const { return m_value; }

  /// Returns reference and reset this
  reference release()
  {
    reference value = m_value;
    m_value = {};
    return value;
  }
};

/**
 * A _dumb_ pointer to resource.
 *
 * This is not safe to generally use, but might be used to compose other
 * pointers.
 *
 * @tparam RESOURCE
 * @tparam DELETER
 */
template<class RESOURCE, class DELETER = DefaultDeleter<RESOURCE>>
class ResourceUnsafe : public ResourcePtrBase<RESOURCE, DELETER>
{
  using base = ResourcePtrBase<RESOURCE, DELETER>;

public:
  /// Default constructor.
  constexpr ResourceUnsafe() = default;

  /// Constructs pointer from anything compatible
  constexpr explicit ResourceUnsafe(RESOURCE other, DELETER deleter = {})
    : base(other, std::move(deleter))
  {
    other = nullptr;
  }

  /// Resets the value, destroying the resource if not nullptr
  void reset()
  {
    base::free();
    base::release();
  }
};

/**
 * @brief Implement unique ownership for a resource.
 *
 * @tparam RESOURCE
 */
template<class RESOURCE, class DELETER = DefaultDeleter<RESOURCE>>
class ResourceUnique : public ResourcePtrBase<RESOURCE, DELETER>
{
  using base = ResourcePtrBase<RESOURCE, DELETER>;

public:
  /// Default constructor.
  constexpr ResourceUnique() = default;

  /// Constructs from raw type.
  constexpr explicit ResourceUnique(base::value_type value,
                                    DELETER deleter = {})
    : base(value, std::move(deleter))
  {
  }

  /// Move constructor.
  constexpr ResourceUnique(ResourceUnique&& other)
    : base(other)
  {
    other.release();
  }

  ResourceUnique(const ResourceUnique& other) = delete;

  /**
   * Destructor.
   *
   * This frees up the resource.
   */
  ~ResourceUnique() { base::free(); }

  /// Assignment operator.
  constexpr ResourceUnique& operator=(ResourceUnique other)
  {
    base::operator=(other);
    other.release();
    return *this;
  }

  /// Resets the value, destroying the resource if not nullptr
  void reset()
  {
    base::free();
    base::release();
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
 * @tparam RESOURCE the *Type*Ref.
 * @tparam UNIQUE the *Type*.
 */
template<class RESOURCE, class UNIQUE>
class DetachedResource
  : public ResourcePtrBase<RESOURCE, DefaultDeleter<RESOURCE>>
{
  using base = ResourcePtrBase<RESOURCE, DefaultDeleter<RESOURCE>>;

public:
  /// Constructs pointer from anything compatible
  constexpr explicit DetachedResource(RESOURCE other)
    : base(other)
  {
    other = nullptr;
  }

  DetachedResource(const DetachedResource& other) = delete;

  /// Move ctor
  constexpr DetachedResource(DetachedResource&& other) = default;

  DetachedResource& operator=(const DetachedResource& other) = delete;

  /// Move assignment
  constexpr DetachedResource& operator=(DetachedResource&& other) = default;

  /// Converts to UNIQUE
  operator UNIQUE() && { return UNIQUE{base::release()}; }
};

/**
 * Base class for locks
 *
 * @tparam RESOURCE
 *
 * @see AudioLock
 * @see SurfaceLock
 */
template<class RESOURCE>
class LockBase : public ResourcePtrBase<RESOURCE, DefaultDeleter<RESOURCE>>
{
  using base = ResourcePtrBase<RESOURCE, DefaultDeleter<RESOURCE>>;

protected:
  /// Constructs initializing member
  constexpr LockBase(RESOURCE&& resource)
    : base(std::move(resource))
  {
  }

public:
  /// Default ctor
  constexpr LockBase() = default;

  LockBase(const LockBase& other) = delete;

  /// Move ctor
  LockBase(LockBase&& other)
    : base(std::move(other))
  {
    other.get() = nullptr;
  }

  /// Dtor
  constexpr ~LockBase() { SDL_assert_paranoid(!*this); }

  /// Move assignment
  LockBase& operator=(LockBase other)
  {
    base::operator=(other);
    other.get() = nullptr;
    return *this;
  }
};

} // namespace SDL

#endif /* SDL3PP_RESOURCE_H_ */
