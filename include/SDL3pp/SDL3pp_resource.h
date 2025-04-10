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
  /// Constructs from value
  constexpr Resource(T resource = {})
    : m_resource(std::move(resource))
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

  /// Return contained resource and empties this
  constexpr T release()
  {
    T result = std::move(m_resource);
    m_resource = {};
    return result;
  }
};

} // namespace SDL

#endif /* SDL3PP_RESOURCE_H_ */
