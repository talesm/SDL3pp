#ifndef SDL3PP_OBJ_PARAM_H_
#define SDL3PP_OBJ_PARAM_H_

#include <cstdlib>

namespace SDL {

/// Const reference wrapper for a given resource,
template<typename RAW_POINTER, typename RAW_CONST_POINTER>
class ObjConstParam
{
public:
  using RawPointer = RAW_POINTER;
  using RawConstPointer = RAW_CONST_POINTER;

  /// Constructs from const pointer.
  constexpr ObjConstParam(RawConstPointer value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr ObjConstParam(std::nullptr_t = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison.
  constexpr auto operator<=>(const ObjConstParam& other) const = default;

  /// Converts to underlying type.
  constexpr operator RawConstPointer() const { return value; }

  /// Converts to underlying type.
  constexpr operator RawPointer() const
  {
    return const_cast<RawPointer>(value);
  }

  /// member access to underlying type.
  constexpr auto operator->() const { return value; }

  RawConstPointer value; ///< parameter's Surface
};

} // namespace SDL

#endif /* SDL3PP_OBJ_PARAM_H_ */
