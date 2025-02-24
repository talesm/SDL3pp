#ifndef SDL3PP_OBJECT_WRAPPER_HPP_
#define SDL3PP_OBJECT_WRAPPER_HPP_

#include <concepts>
#include <memory>

namespace SDL {

template<class T, class OBJ>
concept ObjectBox = requires(const T a) {
  { a.Get() } -> std::convertible_to<OBJ*>;
};

// Helper to access the wrapped value from ObjectBase
template<class T, class BASE>
auto Get(const BASE* base)
{
  auto maybeConstValue = static_cast<const T*>(base)->Get();
  return const_cast<std::remove_const_t<decltype(maybeConstValue)>>(
    maybeConstValue);
}

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

  template<ObjectBox<T> BOX>
  ObjectRef(const BOX& box)
    : value(box.Get())
  {
  }

  constexpr ObjectRef(std::nullptr_t)
    : value{0}
  {
  }

  constexpr pointer Get() const { return value; }

  constexpr operator bool() const { return bool(Get()); }
  constexpr bool Valid() const { return bool(*this); }

  const pointer operator->() const { return Get(); }
  pointer operator->() { return Get(); }
  const T& operator*() const { return *Get(); }
  T& operator*() { return *Get(); }
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
  operator bool() const { return bool(Get()); }
  bool Valid() const { return bool(*this); }

  const pointer operator->() const { return Get(); }
  pointer operator->() { return Get(); }
  const T& operator*() const { return *value; }
  T& operator*() { return *value; }

  pointer Release() { return value.release(); }
  void Reset(pointer other = nullptr) { return value.reset(other); }
  void Swap(ObjectUnique& other) { return std::swap(value, other.value); }
};

template<class T, T defaultValue = 0>
struct FancyPointer
{
  T value;

  auto operator<=>(const FancyPointer& other) const = default;

  operator bool() const { return value != defaultValue; }

  bool operator==(nullptr_t) const { return bool(*this); }

  T Get() const { return value; }
  T& Get() { return value; }

  T operator*() const { return Get(); }
  T& operator*() { return Get(); }

  FancyPointer& operator->() { return *this; }
};

} // namespace SDL

template<class T>
void std::swap(SDL::ObjectUnique<T>& left, SDL::ObjectUnique<T>& right)
{
  left.Swap(right);
}

#endif /* SDL3PP_OBJECT_WRAPPER_HPP_ */
