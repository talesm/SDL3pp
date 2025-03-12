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
struct ResultCallbackWrapper;

/**
 * @brief Wrapper [result callbacks](#ResultCallback)
 *
 * @tparam F the function type
 *
 * For the simpler case, where no transformation is done on the parameters, you
 * can just pass CallOnce() or CallOnceSuffixed(). Otherwise use release().
 *
 * In all cases, use Wrap to change the callback into a void* pointer.
 */
template<class Result, class... Args>
struct ResultCallbackWrapper<std::function<Result(Args...)>>
{
  ResultCallbackWrapper() = delete;

  using FunctionType = std::function<Result(Args...)>;

  /**
   * @brief Change the callback into a void* pointer
   *
   * @param cb
   * @return void*
   */
  static void* Wrap(FunctionType&& cb)
  {
    return new FunctionType(std::move(cb));
  }

  static Result CallOnce(void* handle, Args... args)
  {
    auto f = release(handle);
    return f(args...);
  }
  static Result CallOnceSuffixed(Args... args, void* handle)
  {
    auto f = release(handle);
    return f(args...);
  }

  /**
   * @brief Transfer ownership from the function and delete handle
   *
   * @param handle the handle to be released
   *
   * @return the callback ready to be invoked.
   */
  static FunctionType release(void* handle)
  {
    if (handle == nullptr) return {};
    auto ptr = static_cast<FunctionType*>(handle);
    FunctionType value{std::move(*ptr)};
    delete ptr;
    return value;
  }
};

template<class KEY, class VALUE>
struct KeyValueWrapper
{
  static_assert(sizeof(KEY) <= sizeof(void*));
  KeyValueWrapper() = delete;

  using KeyType = KEY;
  using ValueType = VALUE;

  static void* Wrap(KeyType key, ValueType&& value)
  {
    Values().insert_or_assign(key, std::move(value));
    return (void*)(key);
  }

  static bool contains(void* handle)
  {
    return Values().contains((KeyType)(handle));
  }

  static const ValueType& at(void* handle)
  {
    return Values().at((KeyType)(handle));
  }

  static ValueType release(void* handle)
  {
    ValueType value{std::move(Values().at((KeyType)(handle)))};
    erase(handle);
    return value;
  }

  static bool erase(void* handle) { return Values().erase((KeyType)handle); }

private:
  static std::unordered_map<KeyType, ValueType>& Values()
  {
    static std::unordered_map<KeyType, ValueType> values;
    return values;
  }
};

template<class VALUE>
struct UniqueWrapper
{
  UniqueWrapper() = delete;

  using ValueType = VALUE;

  static ValueType* Wrap(ValueType&& value)
  {
    /// @todo make this an opaque type
    auto& v = Value();
    if (value)
      v = std::make_unique<ValueType>(std::move(value));
    else
      v.reset();
    return v.get();
  }

  static bool contains(void* handle) { return Value().get() == handle; }

  static const ValueType& at(void* handle)
  {
    auto& v = Value();
    SDL_assert_paranoid(v.get() == handle);
    return *v;
  }

  static ValueType release(void* handle)
  {
    auto& v = Value();
    SDL_assert_paranoid(v.get() == handle);

    ValueType value{std::move(*v)};
    v.reset();

    return value;
  }

  static void erase() { return Value().reset(); }

private:
  static std::unique_ptr<ValueType>& Value()
  {
    static std::unique_ptr<ValueType> value;
    return value;
  }
};

/// @}

} // namespace SDL

#endif /* SDL3PP_CALLBACK_WRAPPER_H_ */
