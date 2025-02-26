#ifndef SDL3PP_OBJECT_WRAPPER_HPP_
#define SDL3PP_OBJECT_WRAPPER_HPP_

#include <concepts>
#include <memory>

namespace SDL {

template<class T, class POINTER>
concept ObjectBox = requires(const T a) {
  { a.get() } -> std::convertible_to<POINTER>;
};

template<class T, class POINTER = T*>
class ObjectRef
{
  POINTER value;

public:
  using pointer = POINTER;

  constexpr ObjectRef(pointer value)
    : value(value)
  {
  }

  template<ObjectBox<POINTER> BOX>
  ObjectRef(const BOX& box)
    : value(box.Get())
  {
  }

  constexpr ObjectRef(std::nullptr_t)
    : value{0}
  {
  }

  constexpr pointer Get() const { return value; }
  constexpr pointer get() const { return value; }

  constexpr operator bool() const { return bool(Get()); }

  const pointer operator->() const { return Get(); }
  pointer operator->() { return Get(); }
  const T& operator*() const { return *Get(); }
  T& operator*() { return *Get(); }

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

  constexpr ObjectUnique() = default;

  explicit ObjectUnique(pointer value)
    : value(value)
  {
  }

  pointer Get() const { return value.get(); }
  pointer get() const { return value.get(); }
  operator bool() const { return bool(Get()); }

  const pointer operator->() const { return Get(); }
  pointer operator->() { return Get(); }
  const T& operator*() const { return *value; }
  T& operator*() { return *value; }

  pointer release() { return value.release(); }
  void reset(pointer other = nullptr) { return value.reset(other); }
  void swap(ObjectUnique& other) { return std::swap(value, other.value); }
};

template<class T, T defaultValue = 0>
struct FancyPointer
{
  T value;

  auto operator<=>(const FancyPointer& other) const = default;

  operator bool() const { return value != defaultValue; }

  bool operator==(nullptr_t) const { return bool(*this); }

  operator T() const { return value; }
  T operator*() const { return value; }
  T& operator*() { return value; }

  T* operator->() { return &value; }
};

} // namespace SDL

template<class T>
void std::swap(SDL::ObjectRef<T>& left, SDL::ObjectRef<T>& right)
{
  left.swap(right);
}

template<class T>
void std::swap(SDL::ObjectUnique<T>& left, SDL::ObjectUnique<T>& right)
{
  left.swap(right);
}

#endif /* SDL3PP_OBJECT_WRAPPER_HPP_ */
