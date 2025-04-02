#ifndef SDL3PP_LOADSO_H_
#define SDL3PP_LOADSO_H_

#include <SDL3/SDL_loadso.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 *
 * @defgroup CategorySharedObject Shared Object/DLL Management
 *
 * System-dependent library loading routines.
 *
 * Shared objects are code that is programmatically loadable at runtime.
 * Windows calls these "DLLs", Linux calls them "shared libraries", etc.
 *
 * To use them, build such a library, then call SDL_LoadObject() on it. Once
 * loaded, you can use SDL_LoadFunction() on that object to find the address
 * of its exported symbols. When done with the object, call SDL_UnloadObject()
 * to dispose of it.
 *
 * Some things to keep in mind:
 *
 * - These functions only work on C function names. Other languages may have
 *   name mangling and intrinsic language support that varies from compiler to
 *   compiler.
 * - Make sure you declare your function pointers with the same calling
 *   convention as the actual library function. Your code will crash
 *   mysteriously if you do not do this.
 * - Avoid namespace collisions. If you load a symbol from the library, it is
 *   not defined whether or not it goes into the global symbol namespace for
 *   the application. If it does and it conflicts with symbols in your code or
 *   other shared libraries, you will not get the results you expect. :)
 * - Once a library is unloaded, all pointers into it obtained through
 *   SDL_LoadFunction() become invalid, even if the library is later reloaded.
 *   Don't unload a library if you plan to use these pointers in the future.
 *   Notably: beware of giving one of these pointers to atexit(), since it may
 *   call that pointer after the library unloads.
 *
 * @{
 */

// Forward decl
template<ObjectBox<SDL_SharedObject*> T>
struct SharedObjectBase;

/**
 * Handle to a non owned sharedObject
 *
 * @cat resource
 *
 * @sa SharedObjectBase
 * @sa SharedObject
 */
using SharedObjectRef = SharedObjectBase<ObjectRef<SDL_SharedObject>>;

/**
 * Handle to an owned sharedObject
 *
 * @cat resource
 *
 * @sa SharedObjectBase
 * @sa SharedObjectRef
 */
using SharedObject = SharedObjectBase<ObjectUnique<SDL_SharedObject>>;

/**
 * An opaque datatype that represents a loaded shared object.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SharedObjectBase()
 * @sa LoadFunction()
 */
template<ObjectBox<SDL_SharedObject*> T>
struct SharedObjectBase : T
{
  using T::T;

  /**
   * Dynamically load a shared object.
   *
   * @param sofile a system-dependent name of the object file.
   * @post an opaque pointer to the object handle or NULL on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LoadFunction()
   * @sa Unload()
   */
  SharedObjectBase(StringParam sofile)
    : T(SDL_LoadObject(sofile))
  {
  }

  /**
   * Look up the address of the named function in a shared object.
   *
   * This function pointer is no longer valid after calling SDL_UnloadObject().
   *
   * This function can only look up C function names. Other languages may have
   * name mangling and intrinsic language support that varies from compiler to
   * compiler.
   *
   * Make sure you declare your function pointers with the same calling
   * convention as the actual library function. Your code will crash
   * mysteriously if you do not do this.
   *
   * If the requested function doesn't exist, NULL is returned.
   *
   * @param name the name of the function to look up.
   * @returns a pointer to the function or NULL on failure; call SDL_GetError()
   *          for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  FunctionPointer LoadFunction(StringParam name)
  {
    return SDL_LoadFunction(T::get(), name);
  }

  /**
   * Unload a shared object from memory.
   *
   * Note that any pointers from this object looked up through
   * SDL_LoadFunction() will no longer be valid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_LoadObject
   */
  void Unload() { T::free(); }
};

/**
 * Callback for sharedObject resource cleanup
 *
 * @private
 */
template<>
inline void ObjectRef<SDL_SharedObject>::doFree(SDL_SharedObject* resource)
{
  SDL_UnloadObject(resource);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_LOADSO_H_ */
