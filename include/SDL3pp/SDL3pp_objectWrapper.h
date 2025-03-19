#ifndef SDL3PP_OBJECT_WRAPPER_H_
#define SDL3PP_OBJECT_WRAPPER_H_

#include <concepts>
#include <memory>

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

template<class T, class POINTER = T*>
class ObjectRef
{
  POINTER value;

public:
  using pointer = POINTER;

  constexpr ObjectRef(pointer value = nullptr)
    : value(value)
  {
  }

  template<ObjectBox<POINTER> BOX>
  ObjectRef(BOX&& box)
    : value(std::is_reference_v<BOX> ? box.get() : box.release())
  {
  }

  constexpr ObjectRef(std::nullptr_t)
    : value{0}
  {
  }

  constexpr pointer get() const { return value; }

  constexpr operator bool() const { return bool(get()); }

  const pointer operator->() const { return get(); }
  pointer operator->() { return get(); }
  const T& operator*() const { return *get(); }
  T& operator*() { return *get(); }

  constexpr bool operator==(std::nullptr_t) const { return !value; }

  pointer release()
  {
    pointer p;
    std::swap(value, p);
    return p;
  }
  void reset(pointer other = nullptr) { return std::swap(value, other); }
  void swap(ObjectRef& other) { return std::swap(value, other.value); }
};

template<class T>
struct ObjectDeleter;

template<class T, class DELETER = ObjectDeleter<T>>
class ObjectUnique
{
  std::unique_ptr<T, DELETER> value;

public:
  using pointer = std::unique_ptr<T, DELETER>::pointer;

  explicit ObjectUnique(pointer value = nullptr)
    : value(value)
  {
  }

  constexpr ObjectUnique(std::nullptr_t)
    : value{}
  {
  }

  pointer get() const { return value.get(); }
  operator bool() const { return bool(get()); }

  const pointer operator->() const { return get(); }
  pointer operator->() { return get(); }
  const T& operator*() const { return *value; }
  T& operator*() { return *value; }

  constexpr bool operator==(std::nullptr_t) const { return !value; }

  pointer release() { return value.release(); }
  void reset(pointer other = nullptr) { return value.reset(other); }
  void swap(ObjectUnique& other) { return std::swap(value, other.value); }
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

#ifndef DOXYGEN_SHOULD_SKIP_THIS

template<class T, class POINTER>
void std::swap(SDL::ObjectRef<T, POINTER>& left,
               SDL::ObjectRef<T, POINTER>& right)
{
  left.swap(right);
}

template<class T, class DELETER>
void std::swap(SDL::ObjectUnique<T, DELETER> left,
               SDL::ObjectUnique<T, DELETER>& right)
{
  left.swap(right);
}

#endif // DOXYGEN_SHOULD_SKIP_THIS

#endif /* SDL3PP_OBJECT_WRAPPER_H_ */
