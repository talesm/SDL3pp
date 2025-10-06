#ifndef SDL3PP_SPAN_REF_H_
#define SDL3PP_SPAN_REF_H_

#include <concepts>
#include <ranges>
#include <span>

namespace SDL {

template<class T, class BASE>
concept DerivedWrapper =
  std::derived_from<T, BASE> && sizeof(T) == sizeof(BASE);

/**
 * span-like for empty-derived structs
 *
 * This wraps around std::span that works for out derived-wrapper pattern
 * (eg, Rect, Color)
 *
 * @ingroup CategoriesCppSupport
 */
template<class T>
class SpanRef
{
  std::span<T> value;

public:
  constexpr SpanRef() = default;

  /// Constructor
  template<DerivedWrapper<T> U, size_t N>
  constexpr SpanRef(U (&other)[N])
    : value(static_cast<T*>(other), N)
  {
  }

  /// Constructor
  template<DerivedWrapper<T> U>
  constexpr SpanRef(const std::span<U>& other)
    : value(other.data(), other.size())
  {
  }

  /// Constructor
  template<std::contiguous_iterator It>
    requires DerivedWrapper<std::iter_value_t<It>, T>
  constexpr SpanRef(It first, size_t count)
    : value((T*)(&*first), count)
  {
  }

  /// Constructor
  template<std::contiguous_iterator It, std::sized_sentinel_for<It> End>
    requires DerivedWrapper<std::iter_value_t<It>, T>
  constexpr SpanRef(It first, End last)
    : value((T*)(&*first), size_t(last - first))
  {
  }

  /// Constructor
  template<std::ranges::contiguous_range R>
    requires DerivedWrapper<std::iter_value_t<std::ranges::iterator_t<R>>, T>
  constexpr SpanRef(R&& range)
    : SpanRef(std::begin(range), std::end(range))
  {
  }

  /// Retrieves contained size
  constexpr size_t size() const { return value.size(); }

  /// Retrieves contained data
  constexpr T* data() const { return value.data(); }
};

} // namespace SDL

#endif /* SDL3PP_SPAN_REF_H_ */
