#ifndef SDL3PP_CALLBACK_WRAPPER_H_
#define SDL3PP_CALLBACK_WRAPPER_H_

#include <functional>
#include <map>
#include <memory>
#include <SDL3/SDL_assert.h>

namespace SDL {

/**
 * @defgroup CategoryCallbackWrapper Async callback helpers
 *
 * Async callback wrapper helper functions and types.
 *
 * @{
 */

/** A typesafe handle for callback */
class CallbackHandle
{
  void* id;

public:
  /// @private
  constexpr explicit CallbackHandle(void* id = nullptr)
    : id(id)
  {
  }
  /// Get Internal id
  constexpr void* get() const { return id; }

  /// True if has a valid id
  constexpr operator bool() const { return id != 0; }
};

/// Base class for callback wrappers
template<class Result, class... Args>
struct CallbackWrapperBase
{
  /// The wrapped std::function type
  using ValueType = std::function<Result(Args...)>;

  /// Return unwrapped value of handle.
  static const ValueType& Unwrap(void* handle)
  {
    return *static_cast<ValueType*>(handle);
  }

  /// Call
  static Result Call(void* handle, Args... args)
  {
    auto& f = Unwrap(handle);
    return f(args...);
  }

  /// Call with suffix handle.
  static Result CallSuffixed(Args... args, void* handle)
  {
    auto& f = Unwrap(handle);
    return f(args...);
  }
};

template<class F>
struct CallbackWrapper;

/**
 * @brief Wrapper [result callbacks](#result-callback).
 *
 * @tparam F the function type.
 *
 * For the simpler case, where no transformation is done on the parameters, you
 * can just pass CallOnce() or CallOnceSuffixed(). Otherwise use release().
 *
 * In all cases, use Wrap to change the callback into a void* pointer.
 */
template<class Result, class... Args>
struct CallbackWrapper<std::function<Result(Args...)>>
  : CallbackWrapperBase<Result, Args...>
{
  CallbackWrapper() = delete;

  /// The wrapped std::function type
  using ValueType = std::function<Result(Args...)>;

  /**
   * @brief Change the callback into a void* pointer.
   *
   * @param cb
   * @return void*
   */
  static ValueType* Wrap(ValueType&& cb)
  {
    return new ValueType(std::move(cb));
  }

  /// Call once and release.
  static Result CallOnce(void* handle, Args... args)
  {
    auto f = release(handle);
    return f(args...);
  }

  /// Call once and release with suffix handle.
  static Result CallOnceSuffixed(Args... args, void* handle)
  {
    auto f = release(handle);
    return f(args...);
  }

  /**
   * @brief Transfer ownership from the function and delete handle.
   *
   * @param handle the handle to be released.
   *
   * @return the callback ready to be invoked.
   */
  static ValueType release(void* handle)
  {
    if (handle == nullptr) return {};
    auto ptr = static_cast<ValueType*>(handle);
    ValueType value{std::move(*ptr)};
    delete ptr;
    return value;
  }

  /// Return unwrapped value of handle.
  static const ValueType release(CallbackHandle handle)
  {
    return release(handle.get());
  }
};

/**
 * Lightweight wrapper.
 *
 * @tparam SELF a CRTP class
 */
template<class SELF, class R, class... PARAMS>
struct LightweightCallbackT
{
  /// The wrapper function
  R (*wrapper)(void*, PARAMS...);

  /// The wrapped data
  void* data;

  /// ctor
  template<class F>
  LightweightCallbackT(const F& func)
  {
    static_assert(sizeof(func) <= sizeof(data), "Function must fit size_t");
    union PunAux
    {
      void* ptr;
      F func;
    };
    wrapper = [](void* userdata, PARAMS... params) {
      PunAux aux{.ptr = userdata};
      return SELF::doCall(aux.func, params...);
    };
    PunAux aux{.func = func};
    data = aux.ptr;
  }
};

/**
 * Lightweight wrapper.
 *
 * @tparam SELF a CRTP class
 */
template<class SELF, class R, class... PARAMS>
struct LightweightTrailingCallbackT
{
  /// The wrapper function
  R (*wrapper)(PARAMS..., void*);

  /// The wrapped data
  void* data;

  /// ctor
  template<class F>
  LightweightTrailingCallbackT(const F& func)
  {
    static_assert(sizeof(func) <= sizeof(data), "Function must fit size_t");
    union PunAux
    {
      void* ptr;
      F func;
    };
    wrapper = [](PARAMS... params, void* userdata) {
      PunAux aux{.ptr = userdata};
      return SELF::doCall(aux.func, params...);
    };
    PunAux aux{.func = func};
    data = aux.ptr;
  }
};

template<class F>
struct MakeFrontCallback;

/**
 * Make Front Callback
 *
 * @tparam R
 * @tparam PARAMS
 */
template<class R, class... PARAMS>
struct MakeFrontCallback<R(PARAMS...)>
  : LightweightCallbackT<MakeFrontCallback<R(PARAMS...)>, R, PARAMS...>
{
  /// ctor
  template<std::invocable<PARAMS...> F>
  MakeFrontCallback(const F& func)
    : LightweightCallbackT<MakeFrontCallback<R(PARAMS...)>, R, PARAMS...>(func)
  {
  }

  /// @private
  template<std::invocable<PARAMS...> F>
  static R doCall(F& func, PARAMS... params)
  {
    return func(params...);
  }
};

template<class F>
struct MakeBackCallback;

/**
 * Make Back Callback
 *
 * @tparam R
 * @tparam PARAMS
 */
template<class R, class... PARAMS>
struct MakeBackCallback<R(PARAMS...)>
  : LightweightTrailingCallbackT<MakeBackCallback<R(PARAMS...)>, R, PARAMS...>
{
  /// ctor
  template<std::invocable<PARAMS...> F>
  MakeBackCallback(const F& func)
    : LightweightTrailingCallbackT<MakeBackCallback<R(PARAMS...)>,
                                   R,
                                   PARAMS...>(func)
  {
  }

  /// @private
  template<std::invocable<PARAMS...> F>
  static R doCall(const F& func, PARAMS... params)
  {
    return func(params...);
  }
};

/// @}

} // namespace SDL

#endif /* SDL3PP_CALLBACK_WRAPPER_H_ */
