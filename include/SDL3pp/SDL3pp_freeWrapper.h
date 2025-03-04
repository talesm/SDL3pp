#ifndef SDL3PP_FREE_WRAPPER_H_
#define SDL3PP_FREE_WRAPPER_H_

#include <memory>
#include <SDL3/SDL_stdinc.h>

namespace SDL {

/**
 * @brief Wraps around SDL alloced pointers to automatically free them
 *
 */
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

#endif /* SDL3PP_FREE_WRAPPER_H_ */
