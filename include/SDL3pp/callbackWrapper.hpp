#ifndef SDL3PP_CALLBACK_WRAPPER_HPP_
#define SDL3PP_CALLBACK_WRAPPER_HPP_

#include <functional>
#include <map>
#include <SDL3/SDL_assert.h>

namespace SDL {

template<class F>
struct CallbackWrapper;

template<typename Result, typename... Args>
struct CallbackWrapper<Result(Args...)>
{
  CallbackWrapper() = delete;
  using FunctionType = std::function<Result(Args...)>;

  static void* Wrap(FunctionType cb)
  {
    // TODO Protect against concurrency
    auto id = NextId();
    Values().insert_or_assign(id, std::move(cb));
    return (void*)id;
  }

  static Result Call(void* handle, Args... args)
  {
    auto& f = Get(handle);
    return f(args...);
  }
  static Result CallOnce(void* handle, Args... args)
  {
    auto f = release(handle);
    return f(args...);
  }

  static bool Has(void* handle) { return Values().count((size_t)handle); }

  static const FunctionType& Get(void* handle)
  {
    return Values().at((size_t)(handle));
  }

  static FunctionType release(void* handle)
  {
    auto& values = Values();
    auto value = std::move(values.at((size_t)(handle)));
    Erase(handle);
    return value;
  }

  static bool Erase(void* handle) { return Values().erase((size_t)handle); }

  static size_t NextId()
  {
    static size_t lastId = 0;
    SDL_assert_paranoid(lastId < SDL_SIZE_MAX);
    // TODO Some strategy on the odd case we get to SIZE_MAX
    ++lastId;
    return lastId;
  }

  static std::map<size_t, FunctionType>& Values()
  {
    static std::map<size_t, FunctionType> values;
    return values;
  }
};

} // namespace SDL

#endif /* SDL3PP_CALLBACK_WRAPPER_HPP_ */
