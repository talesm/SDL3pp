#ifndef SDL3PP_OBJECT_REFERENCE_HPP
#define SDL3PP_OBJECT_REFERENCE_HPP

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
auto
Get(const BASE* base)
{
  return static_cast<const T*>(base)->Get();
}

} // namespace SDL

#endif /* SDL3PP_OBJECT_REFERENCE_HPP */
