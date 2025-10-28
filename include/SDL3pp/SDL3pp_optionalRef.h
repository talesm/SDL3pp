#ifndef SDL3PP_OPTIONAL_REF_H_
#define SDL3PP_OPTIONAL_REF_H_

#include <optional>

namespace SDL {

/**
 * @brief Optional-like shim for references
 *
 * This allows us explicitly annotate optional parameters that would otherwise
 * be references.
 *
 * @tparam T the underlying type
 *
 * @ingroup CategoriesCppSupport
 */
template<class T>
class OptionalRef
{
  T* ptr;

public:
  /// Constructor.
  constexpr OptionalRef(std::nullopt_t = std::nullopt)
    : ptr(nullptr)
  {
  }

  /// Constructor.
  constexpr OptionalRef(T& value)
    : ptr(&value)
  {
  }

  /// Constructor.
  constexpr auto operator<=>(const OptionalRef&) const = default;

  /// true if has a value.
  constexpr bool has_value() const { return ptr != nullptr; }

  /// Returns contained value.
  constexpr const T& value() const { return *ptr; }

  /// Returns contained value.
  constexpr T& value() { return *ptr; }

  /// returns pointer if has value, nullptr otherwise.
  constexpr operator T*() const { return ptr; }

  /// Comparison operator
  constexpr bool operator==(std::nullopt_t) const { return ptr == nullptr; }

  /// Equivalent to value()
  constexpr T& operator*() { return *ptr; }

  /// Equivalent to value()
  constexpr const T& operator*() const { return *ptr; }

  /// Equivalent to value()
  constexpr T* operator->() { return ptr; }

  /// Equivalent to value()
  constexpr const T* operator->() const { return ptr; }
};

/**
 * Create OptionalRef from a nullable pointer.
 */
template<class T>
constexpr OptionalRef<T> fromNullable(T* ptr)
{
  if (ptr) return {*ptr};
  return std::nullopt;
}

} // namespace SDL

#endif /* SDL3PP_OPTIONAL_REF_H_ */
