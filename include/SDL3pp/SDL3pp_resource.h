#ifndef SDL3PP_RESOURCE_H_
#define SDL3PP_RESOURCE_H_

#include <memory>
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
template<class RESOURCE>
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

protected:
  /// Constructs from raw type.
  constexpr ResourcePtrBase(value_type value = {})
    : m_value(value)
  {
  }

  /// Get reference
  reference& get() { return m_value; }

public:
  /// Check if not null
  constexpr operator bool() const { return bool(m_value); }

  /// Comparison
  constexpr bool operator==(const ResourcePtrBase& other) const
  {
    return m_value == other.m_value;
  };

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
};

/**
 * @brief Base class for resource pointer-like owner objects.
 *
 * @tparam RESOURCE
 */
template<class RESOURCE, class DELETER>
class ResourceOwnerBase : public ResourcePtrBase<RESOURCE>
{
  using base = ResourcePtrBase<RESOURCE>;
  DELETER m_deleter;

protected:
  /// Constructs from raw type.
  constexpr ResourceOwnerBase(base::value_type value = {}, DELETER deleter = {})
    : base(value)
    , m_deleter(std::move(deleter))
  {
  }

  /// Frees resource
  void free() { m_deleter(base::get()); }

public:
  /// The deleter type
  using deleter = DELETER;

  /// Returns reference and reset this
  RESOURCE release()
  {
    RESOURCE value = base::get();
    base::get() = {};
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
class ResourceUnsafe : public ResourceOwnerBase<RESOURCE, DELETER>
{
  using base = ResourceOwnerBase<RESOURCE, DELETER>;

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
class ResourceUnique : public ResourceOwnerBase<RESOURCE, DELETER>
{
  using base = ResourceOwnerBase<RESOURCE, DELETER>;

public:
  /// Default constructor.
  constexpr ResourceUnique(std::nullptr_t = nullptr) {};

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

// Forward decl
template<class UNIQUE>
class ResourceWeak;

/**
 * @brief Implement shared ownership for a resource.
 *
 * @tparam UNIQUE
 */
template<class UNIQUE>
class ResourceShared : public ResourcePtrBase<typename UNIQUE::reference>
{
  using base = ResourcePtrBase<typename UNIQUE::reference>;
  std::shared_ptr<UNIQUE> m_shared;

public:
  /// Default constructor
  constexpr ResourceShared(std::nullptr_t = nullptr) {};

  /// Constructs from unique type
  constexpr ResourceShared(UNIQUE&& value)
    : base(*value)
    , m_shared(std::make_shared<UNIQUE>(std::move(value)))
  {
  }

  /// Constructs from raw type.
  constexpr explicit ResourceShared(base::value_type value,
                                    typename UNIQUE::deleter deleter = {})
    : ResourceShared(UNIQUE(value, std::move(deleter)))
  {
  }

  /// True if this is the only shared instance
  constexpr bool unique() const { return m_shared.unique(); }

  /// Reset this instance
  void reset() { *this = {}; }

  friend class ResourceWeak<UNIQUE>;
};

/**
 * @brief Implement weak ownership for a resource.
 *
 * @tparam UNIQUE
 */
template<class UNIQUE>
class ResourceWeak
{
  std::weak_ptr<UNIQUE> m_shared;

public:
  /// Default constructor
  constexpr ResourceWeak() = default;

  /// Constructs from ResourceShared
  constexpr ResourceWeak(const ResourceShared<UNIQUE>& shared)
    : m_shared(shared)
  {
  }

  /// Compares.
  constexpr bool operator==(const ResourceWeak& other) const = default;

  /// True if expired.
  constexpr bool expired() const { return m_shared.expired(); }

  /// True if not expired.
  constexpr operator bool() const { return !expired(); }

  /// Lock back to ResourceShared
  ResourceShared<UNIQUE> lock() const { return m_shared.lock(); }
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
  : public ResourceOwnerBase<RESOURCE, DefaultDeleter<RESOURCE>>
{
  using base = ResourceOwnerBase<RESOURCE, DefaultDeleter<RESOURCE>>;

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
class LockBase : public ResourceOwnerBase<RESOURCE, DefaultDeleter<RESOURCE>>
{
  using base = ResourceOwnerBase<RESOURCE, DefaultDeleter<RESOURCE>>;

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

/**
 * Get hash for resource type
 */
template<class T>
std::size_t hash(const SDL::Resource<T>& s) noexcept
{
  return std::hash<T>{}(s.get());
}

/**
 * Get hash for resource type
 */
template<class T>
std::size_t hash(const SDL::ResourcePtrBase<T>& s) noexcept
{
  return hash(s.get());
}

/**
 * @brief Utility class to help creating std::hash.
 *
 */
struct Hash
{
  /// Calculate hash.
  template<class T>
  std::size_t operator()(const T& s) const noexcept
  {
    return hash(s);
  }
};

} // namespace SDL

#endif /* SDL3PP_RESOURCE_H_ */
