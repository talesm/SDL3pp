#ifndef SDL3PP_FREE_WRAPPER_HPP_
#define SDL3PP_FREE_WRAPPER_HPP_

#include <memory>
#include <SDL3/SDL_stdinc.h>

namespace SDL {

struct FreeWrapperDeleter
{
  void operator()(void* mem) const { SDL_free(mem); }
};

template<class T>
using FreeWrapper = std::unique_ptr<T, FreeWrapperDeleter>;

template<class T>
FreeWrapper<T[]> wrapArray(T* array)
{
  return FreeWrapper<T[]>{array};
}
} // namespace SDL

#endif /* SDL3PP_FREE_WRAPPER_HPP_ */
