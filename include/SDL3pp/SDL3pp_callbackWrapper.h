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
{
  R (*wrapper)(void*, PARAMS...);
  void* data;

  /// ctor
  explicit(false) MakeFrontCallback(R (*func)(PARAMS...))
    : wrapper([](void* userdata, PARAMS... params) {
      auto f = static_cast<R (*)(PARAMS...)>(userdata);
      return f(params...);
    })
    , data(static_cast<void*>(func))
  {
  }

  /// ctor
  template<std::invocable<PARAMS...> F>
  explicit(false) MakeFrontCallback(const F& func)
  {
    static_assert(sizeof(func) <= sizeof(data), "Function must fit data");
  }
};

/**
 * @brief Wrapper key to value [result callbacks](#result-callback).
 *
 * @tparam KEY the key type.
 * @tparam VALUE the value type.
 * @tparam VARIANT the variant, if more than one listener type is associated.
 *
 */
template<class KEY, class VALUE, size_t VARIANT = 0>
struct KeyValueWrapper
{
  static_assert(sizeof(KEY) <= sizeof(void*));
  KeyValueWrapper() = delete;

  /// Key type
  using KeyType = KEY;

  /// Value type.
  using ValueType = VALUE;

  /**
   * @brief Change the value into a void* pointer held by key.
   *
   * @param key
   * @param value
   * @return void*
   */
  static ValueType* Wrap(KeyType key, ValueType&& value)
  {
    auto lockGuard = lock();
    return &Values().insert_or_assign(key, std::move(value)).first->second;
  }

  /// True if handle is stored.
  static bool contains(KeyType handle)
  {
    auto lockGuard = lock();
    return Values().contains(handle);
  }

  /// Return unwrapped value of handle.
  static const ValueType& at(KeyType handle)
  {
    auto lockGuard = lock();
    return Values().at(handle);
  }

  /// Return unwrapped value associated by key and remove association.
  static ValueType release(KeyType key)
  {
    auto lockGuard = lock();
    auto& values = Values();
    auto it = values.find(key);
    if (it == values.end()) return {};
    ValueType value{std::move(it->second)};
    values.erase(it);
    return value;
  }

  /**
   * Remove association.
   *
   * @param key the key associated.
   * @return true if the key was associated and was erased, false otherwise.
   */
  static bool erase(KeyType key)
  {
    auto lockGuard = lock();
    return Values().erase(key);
  }

private:
  static std::map<KeyType, ValueType>& Values()
  {
    static std::map<KeyType, ValueType> values;
    return values;
  }

  static std::lock_guard<std::mutex> lock()
  {
    static std::mutex uniqueMutex;
    return std::lock_guard{uniqueMutex};
  }
};

/// Store callbacks by key
template<class KEY, class VALUE, size_t VARIANT = 0>
struct KeyValueCallbackWrapper;

/// Store callbacks by key
template<class KEY, class Result, class... Args, size_t VARIANT>
struct KeyValueCallbackWrapper<KEY, std::function<Result(Args...)>, VARIANT>
  : CallbackWrapperBase<Result, Args...>
  , KeyValueWrapper<KEY, std::function<Result(Args...)>, VARIANT>
{
  KeyValueCallbackWrapper() = delete;

  /// Wrapped type.
  using ValueType = std::function<Result(Args...)>;
};

/**
 * @brief Stored Wrapper unique by type [result callbacks](#result-callback).
 *
 * @tparam VALUE the function type.
 */
template<class VALUE>
struct UniqueCallbackWrapper;

/**
 * @brief Stored Wrapper unique by type [result callbacks](#result-callback).
 */
template<class Result, class... Args>
struct UniqueCallbackWrapper<std::function<Result(Args...)>>
  : CallbackWrapperBase<Result, Args...>
{
  UniqueCallbackWrapper() = delete;

  /// Wrapped type.
  using ValueType = std::function<Result(Args...)>;

  /**
   * @brief Change the value into a void* pointer held uniquely by this type.
   *
   * @param value
   * @return void*
   */
  static ValueType* Wrap(ValueType&& value)
  {
    auto lockGuard = lock();
    auto& v = Value();
    v = std::move(value);
    return &v;
  }

  /// True if handle equals to wrapped value.
  static bool contains(void* handle)
  {
    auto lockGuard = lock();
    auto& v = Value();
    return bool(v) && &v == handle;
  }

  /// Return wrapped type, if handle is contained.
  static ValueType at(void* handle)
  {
    if (&get() == handle) {
      return CallbackWrapperBase<Result, Args...>::Unwrap(handle);
    }
    return {};
  }

  /// Return wrapped type, if handle is contained.
  static const ValueType& get()
  {
    auto lockGuard = lock();
    return Value();
  }

  /// Return wrapped type and erase it from store.
  static ValueType release()
  {
    auto lockGuard = lock();
    ValueType value{std::move(Value())};
    return value;
  }

  /// Return wrapped type and erase it from store.
  static ValueType release(void* handle)
  {
    SDL_assert_paranoid(&get() == handle);
    return release();
  }

  /// Erase value from store.
  static void erase()
  {
    auto lockGuard = lock();
    Value() = {};
  }

private:
  static ValueType& Value()
  {
    static ValueType value;
    return value;
  }

  static std::lock_guard<std::mutex> lock()
  {
    static std::mutex uniqueMutex;
    return std::lock_guard{uniqueMutex};
  }
};

/// @}

} // namespace SDL

#endif /* SDL3PP_CALLBACK_WRAPPER_H_ */
