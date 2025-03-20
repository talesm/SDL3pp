#ifndef SDL3PP_OBJECT_WRAPPER_H_
#define SDL3PP_OBJECT_WRAPPER_H_

#include <concepts>
#include <memory>
#include <type_traits>

namespace SDL {

/**
 * @defgroup CategoryObjectWrapper Helpers for Resource type wrapping
 *
 * @sa Resource
 * @{
 */

template<class T, class POINTER>
concept ObjectBox = requires(const T a, T b) {
  { a.get() } -> std::convertible_to<POINTER>;
  { b.release() } -> std::convertible_to<POINTER>;
};

template<class T, class POINTER = std::remove_extent_t<T>*>
class ObjectRef
{
  POINTER m_value;

public:
  using pointer = POINTER;

  constexpr ObjectRef(pointer value = nullptr)
    : m_value(value)
  {
  }

  template<ObjectBox<POINTER> BOX>
  ObjectRef(BOX&& box)
    : m_value(std::is_reference_v<BOX> ? box.get() : box.release())
  {
  }

  constexpr ObjectRef(std::nullptr_t)
    : m_value{0}
  {
  }

  constexpr pointer get() const { return m_value; }

  constexpr operator bool() const { return bool(get()); }

  const pointer operator->() const { return get(); }
  pointer operator->() { return get(); }
  const T& operator*() const { return *get(); }
  T& operator*() { return *get(); }

  constexpr bool operator==(std::nullptr_t) const { return !m_value; }

  pointer release()
  {
    pointer p;
    std::swap(m_value, p);
    return p;
  }
  void reset(pointer other = nullptr) { return std::swap(m_value, other); }
  void swap(ObjectRef& other) { return std::swap(m_value, other.m_value); }

  auto& operator[](ptrdiff_t index)
  {
    static_assert(std::is_array_v<T>, "T must be an array");
    return m_value[index];
  }
  const auto& operator[](ptrdiff_t index) const
  {
    static_assert(std::is_array_v<T>, "T must be an array");
    return m_value[index];
  }
};

template<class T>
struct ObjectDeleter;

template<class T, class DELETER = ObjectDeleter<T>>
class ObjectUnique
{
  std::unique_ptr<T, DELETER> m_value;

public:
  using pointer = std::unique_ptr<T, DELETER>::pointer;

  explicit ObjectUnique(pointer value = nullptr)
    : m_value(value)
  {
  }

  constexpr ObjectUnique(std::nullptr_t)
    : m_value{}
  {
  }

  pointer get() const { return m_value.get(); }
  operator bool() const { return bool(get()); }

  const pointer operator->() const { return get(); }
  pointer operator->() { return get(); }
  const auto& operator*() const { return *m_value; }
  auto& operator*() { return *m_value; }

  constexpr bool operator==(std::nullptr_t) const { return !m_value; }

  pointer release() { return m_value.release(); }
  void reset(pointer other = nullptr) { return m_value.reset(other); }
  void swap(ObjectUnique& other) { return std::swap(m_value, other.m_value); }

  auto& operator[](ptrdiff_t index)
  {
    static_assert(std::is_array_v<T>, "T must be an array");
    return m_value[index];
  }
  const auto& operator[](ptrdiff_t index) const
  {
    static_assert(std::is_array_v<T>, "T must be an array");
    return m_value[index];
  }
};

template<class T, T defaultValue = 0>
class FancyPointer
{
  T value;

public:
  constexpr FancyPointer(T value = defaultValue)
    : value(value)
  {
  }

  constexpr auto operator<=>(const FancyPointer& other) const = default;

  constexpr operator bool() const { return value != defaultValue; }

  bool operator==(nullptr_t) const { return bool(*this); }

  constexpr operator T() const { return value; }

  T operator*() const { return value; }
  T& operator*() { return value; }

  T* operator->() { return &value; }
};

/// @}

} // namespace SDL

#ifndef SDL3PP_DOC

template<class T, class POINTER>
void std::swap(SDL::ObjectRef<T, POINTER>& left,
               SDL::ObjectRef<T, POINTER>& right)
{
  left.swap(right);
}

template<class T, class DELETER>
void std::swap(SDL::ObjectUnique<T, DELETER>& left,
               SDL::ObjectUnique<T, DELETER>& right)
{
  left.swap(right);
}

#endif // SDL3PP_DOC

#endif /* SDL3PP_OBJECT_WRAPPER_H_ */
