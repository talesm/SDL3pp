#ifndef SDL3PP_OPTIONAL_REF_HPP_
#define SDL3PP_OPTIONAL_REF_HPP_

#include <optional>

namespace SDL {

/**
 * @brief Shim to get optional-like interface for references
 *
 * @tparam T
 */
template<class T>
class OptionalRef
{
  T* ptr;

public:
  constexpr OptionalRef(std::nullopt_t = std::nullopt)
    : ptr(nullptr)
  {
  }

  constexpr OptionalRef(T& value)
    : ptr(&value)
  {
  }

  constexpr auto operator<=>(const OptionalRef&) const = default;

  constexpr bool has_value() const { return ptr != nullptr; }

  constexpr const T& value() const { return *ptr; }
  constexpr T& value() { return *ptr; }

  constexpr operator T*() const { return ptr; }

  constexpr bool operator==(std::nullopt_t) const { return ptr == nullptr; }

  constexpr T& operator*() { return *ptr; }
  constexpr const T& operator*() const { return *ptr; }
  constexpr T* operator->() { return ptr; }
  constexpr const T* operator->() const { return ptr; }
};

template<class T>
constexpr OptionalRef<T> fromNullable(T* ptr)
{
  if (ptr) return {*ptr};
  return std::nullopt;
}

} // namespace SDL

#endif /* SDL3PP_OPTIONAL_REF_HPP_ */
