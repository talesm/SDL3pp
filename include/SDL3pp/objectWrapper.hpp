#ifndef SDL3PP_OBJECT_WRAPPER_HPP
#define SDL3PP_OBJECT_WRAPPER_HPP

#include <concepts>
#include <memory>

namespace SDL {

template<class T, class OBJ>
concept ObjectBox = requires(const T a) {
  { a.Get() } -> std::convertible_to<OBJ*>;
};

template<class T, class OBJ>
struct ObjectBase;

// Helper to access the wrapped value from ObjectBase
template<class T, class BASE>
auto Get(const BASE* base)
{
  return static_cast<const T*>(base)->Get();
}

/**
 * @brief A non owning wrapper for a managed object
 * @tparam T the managed object
 */
template<class T>
class ObjectWrapper : public ObjectBase<ObjectWrapper<T>, T>
{
public:
  constexpr ObjectWrapper(T* value = nullptr)
    : value(value)
  {
  }

  template<ObjectBox<T> BOX>
  ObjectWrapper(const BOX& box)
    : value(box.Get())
  {
  }

  T* Get() const { return value; }
  operator bool() const { return value != nullptr; }

  T* Release()
  {
    T* result = value;
    value = nullptr;
    return result;
  }
  void Reset(T* other = nullptr) { return value = other; }

private:
  T* value = nullptr;
};

/**
 * @brief An uniquely owning RAII wrapper for a managed object
 * @tparam T the managed object
 */
template<class T>
class ObjectUnique : public ObjectBase<ObjectUnique<T>, T>
{
public:
  ObjectUnique() = default;

  explicit ObjectUnique(T* value)
    : value(value)
  {
  }

  template<class... ARGS>
  ObjectUnique(ARGS&&... args)
    : value(super::Create(std::forward<ARGS>(args)...))
  {
  }

  T* Get() const { return value.get(); }
  operator bool() const { return value; }

  T* Release() { return value.release(); }
  void Reset(T* other = nullptr) { return value.reset(other); }
  void Swap(ObjectUnique& other) { return std::swap(value, other.value); }

private:
  using super = ObjectBase<ObjectUnique<T>, T>;

  struct Deleter
  {
    void operator()(T* value) { super::Destroy(value); }
  };

  using PointerType = std::unique_ptr<T, Deleter>;
  PointerType value;
};

} // namespace SDL

template<class T>
void std::swap(SDL::ObjectUnique<T>& left, SDL::ObjectUnique<T>& right)
{
  left.Swap(right);
}

#endif /* SDL3PP_OBJECT_WRAPPER_HPP */
