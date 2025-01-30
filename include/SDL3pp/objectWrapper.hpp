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

template<class T>
class ObjectRef
{
  T* value;

public:
  ObjectRef(T* value)
    : value(value)
  {
  }

  template<ObjectBox<T> BOX>
  ObjectRef(const BOX& box)
    : value(box.Get())
  {
  }

  T* Get() const { return value; }

  operator bool() const { return Get() != nullptr; }

  const T* operator->() const { return Get(); }
  T* operator->() { return Get(); }
  const T& operator*() const { return *Get(); }
  T& operator*() { return *Get(); }
};

template<class T>
struct ObjectDeleter;

template<class T>
class ObjectUnique
{
  std::unique_ptr<T, ObjectDeleter<T>> value;

public:
  constexpr ObjectUnique() = default;

  explicit ObjectUnique(T* value)
    : value(value)
  {
  }

  T* Get() const { return value.get(); }
  operator bool() const { return value != nullptr; }

  const T* operator->() const { return Get(); }
  T* operator->() { return Get(); }
  const T& operator*() const { return *value; }
  T& operator*() { return *value; }

  T* Release() { return value.release(); }
  void Reset(T* other = nullptr) { return value.reset(other); }
  void Swap(ObjectUnique& other) { return std::swap(value, other.value); }
};

} // namespace SDL

template<class T>
void std::swap(SDL::ObjectUnique<T>& left, SDL::ObjectUnique<T>& right)
{
  left.Swap(right);
}

#endif /* SDL3PP_OBJECT_WRAPPER_HPP_ */
