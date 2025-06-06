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
 * Shared objects are code that is programmatically loadable at runtime. Windows
 * calls these "DLLs", Linux calls them "shared libraries", etc.
 *
 * To use them, build such a library, then call SharedObject.Load() on it. Once
 * loaded, you can use SharedObjectRef.LoadFunction() on that object to find the
 * address of its exported symbols. When done with the object, call
 * SharedObject.Unload() to dispose of it.
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
 *   SharedObjectRef.LoadFunction() become invalid, even if the library is later
 *   reloaded. Don't unload a library if you plan to use these pointers in the
 *   future. Notably: beware of giving one of these pointers to atexit(), since
 *   it may call that pointer after the library unloads.
 *
 * @{
 */

// Forward decl
struct SharedObjectRef;

// Forward decl
struct SharedObject;

/**
 * Handle to a shared sharedObject.
 *
 * @cat resource
 *
 * @sa SharedObjectRef
 * @sa SharedObject
 */
using SharedObjectShared = ResourceShared<SharedObject>;

/**
 * Weak handle to a shared sharedObject.
 *
 * @cat resource
 *
 * @sa SharedObjectShared
 * @sa SharedObjectRef
 */
using SharedObjectWeak = ResourceWeak<SharedObject>;

/**
 * An opaque datatype that represents a loaded shared object.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa SharedObject.Load
 * @sa SharedObjectRef.LoadFunction
 * @sa SharedObject.Unload
 * @sa SharedObject
 */
struct SharedObjectRef : Resource<SDL_SharedObject*>
{
  using Resource::Resource;

  /**
   * Look up the address of the named function in a shared object.
   *
   * This function pointer is no longer valid after calling
   * SharedObject.Unload().
   *
   * This function can only look up C function names. Other languages may have
   * name mangling and intrinsic language support that varies from compiler to
   * compiler.
   *
   * Make sure you declare your function pointers with the same calling
   * convention as the actual library function. Your code will crash
   * mysteriously if you do not do this.
   *
   * If the requested function doesn't exist, nullptr is returned.
   *
   * @param name the name of the function to look up.
   * @returns a pointer to the function or nullptr on failure; call GetError()
   *          for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SharedObject.Load
   */
  FunctionPointer LoadFunction(StringParam name)
  {
    return SDL_LoadFunction(get(), name);
  }

  /**
   * Unload a shared object from memory.
   *
   * Note that any pointers from this object looked up through
   * SharedObjectRef.LoadFunction() will no longer be valid.
   *
   * @param resource a valid shared object handle returned.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SharedObject.Load
   */
  static void reset(SDL_SharedObject* resource) { SDL_UnloadObject(resource); }
};

/**
 * Handle to an owned sharedObject
 *
 * @cat resource
 *
 * @sa SharedObjectRef
 */
struct SharedObject : ResourceUnique<SharedObjectRef>
{
  using ResourceUnique::ResourceUnique;

  /**
   * Dynamically load a shared object.
   *
   * @param sofile a system-dependent name of the object file.
   * @returns an opaque pointer to the object handle or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SharedObjectRef.LoadFunction
   * @sa SharedObject.Unload
   */
  static SharedObject Load(StringParam sofile)
  {
    return SharedObject(SDL_LoadObject(sofile));
  }

  /**
   * Unload a shared object from memory.
   *
   * Note that any pointers from this object looked up through
   * SharedObjectRef.LoadFunction() will no longer be valid.
   *
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SharedObject.Load
   */
  void Unload() { reset(); }
  /**
   * Move this sharedObject into a SharedObjectShared.
   */
  SharedObjectShared share();

};


inline SharedObjectShared SharedObject::share()
{
  return SharedObjectShared(std::move(*this));
}

/**
 * Unsafe Handle to sharedObject
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa SharedObjectRef
 */
struct SharedObjectUnsafe : ResourceUnsafe<SharedObjectRef>
{
  using ResourceUnsafe::ResourceUnsafe;

  /**
   * Constructs SharedObjectUnsafe from SharedObject.
   */
  constexpr explicit SharedObjectUnsafe(SharedObject&& other)
    : SharedObjectUnsafe(other.release())
  {
  }
};

/// @}

} // namespace SDL

#endif /* SDL3PP_LOADSO_H_ */
