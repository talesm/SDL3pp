#ifndef SDL3PP_OBJECT_REFERENCE_HPP
#define SDL3PP_OBJECT_REFERENCE_HPP

#include <concepts>
#include <memory>

namespace SDL {

template<class T, class OBJ>
concept ObjectBox = requires(T a) {
  { a.Get() } -> std::convertible_to<OBJ*>;
};

} // namespace SDL

#endif /* SDL3PP_OBJECT_REFERENCE_HPP */
