#ifndef SDL3PP_CALLBACK_WRAPPER_H_
#define SDL3PP_CALLBACK_WRAPPER_H_

#include <functional>
#include <memory>
#include <unordered_map>
#include <SDL3/SDL_assert.h>

namespace SDL {

/**
 * @defgroup CategoryCallbackWrapper Async callback helpers
 *
 * Async callback wrapper helper functions and types.
 *
 * @{
 */

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
};

/**
 * @brief Wrapper key to value [result callbacks](#result-callback).
 *
 * @tparam KEY the key type.
 * @tparam VALUE the value type.
 *
 */
template<class KEY, class VALUE>
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
  static void* Wrap(KeyType key, ValueType&& value)
  {
    {
      auto lockGuard = lock();
      Values().insert_or_assign(key, std::move(value));
    }
    return reinterpret_cast<void*>(key);
  }

  /// True if handle is stored.
  static bool contains(void* handle)
  {
    auto lockGuard = lock();
    return Values().contains((KeyType)(handle));
  }

  /// Return unwrapped value of handle.
  static const ValueType& at(void* handle)
  {
    auto lockGuard = lock();
    return Values().at((KeyType)(handle));
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

  /// Return unwrapped value of handle and remove association.
  static ValueType release(void* handle) { return release((KeyType)handle); }

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

  /**
   * Remove association.
   *
   * @param handle the handle.
   * @return true if the key was associated and was erased, false otherwise.
   */
  static bool erase(void* handle) { return erase((KeyType)handle); }

private:
  static std::unordered_map<KeyType, ValueType>& Values()
  {
    static std::unordered_map<KeyType, ValueType> values;
    return values;
  }

  static std::lock_guard<std::mutex> lock()
  {
    static std::mutex uniqueMutex;
    return std::lock_guard{uniqueMutex};
  }
};

/**
 * @brief Stored Wrapper unique by type [result callbacks](#result-callback).
 *
 * @tparam VALUE the function type.
 */
template<class VALUE>
struct UniqueWrapper
{
  UniqueWrapper() = delete;

  /// Wrapped type.
  using ValueType = VALUE;

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
  static const ValueType& at(void* handle)
  {
    auto lockGuard = lock();
    auto& v = Value();
    SDL_assert_paranoid(&v == handle);
    return v;
  }

  /// Return wrapped type and erase it from store.
  static ValueType release(void* handle)
  {
    auto lockGuard = lock();
    auto& v = Value();
    SDL_assert_paranoid(&v == handle);

    ValueType value{std::move(v)};
    return value;
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
