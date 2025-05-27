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
struct SharedObjectRef;

// Forward decl
struct SharedObject;

/**
 * An opaque datatype that represents a loaded shared object.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa SharedObjectRef.SharedObjectRef
 * @sa SharedObjectRef.LoadFunction
 * @sa SharedObjectRef.Unload
 * @sa SharedObject
 * @sa SharedObjectRef
 */
struct SharedObjectRef : Resource<SDL_SharedObject*>
{
  using Resource::Resource;

  /**
   * Copy constructor.
   */
  constexpr SharedObjectRef(const SharedObjectRef& other)
    : SharedObjectRef(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr SharedObjectRef(SharedObjectRef&& other)
    : SharedObjectRef(other.release())
  {
  }

  SharedObjectRef(SharedObject&& other) = delete;

  /**
   * Assignment operator.
   */
  SharedObjectRef& operator=(SharedObjectRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Look up the address of the named function in a shared object.
   *
   * This function pointer is no longer valid after calling
   * SharedObjectRef.Unload().
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
   * @sa SharedObjectRef.SharedObjectRef
   */
  FunctionPointer LoadFunction(StringParam name)
  {
    return SDL_LoadFunction(get(), name);
  }

protected:
  /**
   * Unload a shared object from memory.
   *
   * Note that any pointers from this object looked up through
   * SharedObjectRef.LoadFunction() will no longer be valid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SharedObjectRef.SharedObjectRef
   */
  void Unload() { reset(); }

  /**
   * Unload a shared object from memory.
   *
   * Note that any pointers from this object looked up through
   * SharedObjectRef.LoadFunction() will no longer be valid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SharedObjectRef.SharedObjectRef
   */
  void reset(SDL_SharedObject* newResource = {})
  {
    SDL_UnloadObject(release(newResource));
  }
};

/**
 * Unsafe Handle to sharedObject
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa SharedObjectRef
 */
struct SharedObjectUnsafe : SharedObjectRef
{
  using SharedObjectRef::SharedObjectRef;

  using SharedObjectRef::Unload;

  using SharedObjectRef::reset;

  SharedObjectUnsafe(const SharedObject& other) = delete;

  /**
   * Constructs SharedObjectUnsafe from SharedObject.
   */
  explicit SharedObjectUnsafe(SharedObject&& other);
};

/**
 * Handle to an owned sharedObject
 *
 * @cat resource
 *
 * @sa SharedObjectRef
 */
struct SharedObject : SharedObjectUnsafe
{
  using SharedObjectUnsafe::SharedObjectUnsafe;

  /**
   * Constructs an empty SharedObject.
   */
  constexpr SharedObject()
    : SharedObjectUnsafe(nullptr)
  {
  }

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit SharedObject(SDL_SharedObject* resource)
    : SharedObjectUnsafe(resource)
  {
  }

  constexpr SharedObject(const SharedObject& other) = delete;

  /**
   * Move constructor.
   */
  constexpr SharedObject(SharedObject&& other) = default;

  /**
   * Dynamically load a shared object.
   *
   * @param sofile a system-dependent name of the object file.
   * @post an opaque pointer to the object handle or nullptr on failure; call
   *       GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SharedObjectRef.LoadFunction
   * @sa SharedObjectRef.Unload
   */
  SharedObject(StringParam sofile)
    : SharedObject(SDL_LoadObject(sofile))
  {
  }

  /**
   * Frees up resource when object goes out of scope.
   */
  ~SharedObject() { reset(); }

  /**
   * Assignment operator.
   */
  SharedObject& operator=(SharedObject other)
  {
    reset(other.release());
    return *this;
  }
};

inline SharedObjectUnsafe::SharedObjectUnsafe(SharedObject&& other)
  : SharedObjectUnsafe(other.release())
{
}

/// @}

} // namespace SDL

#endif /* SDL3PP_LOADSO_H_ */
