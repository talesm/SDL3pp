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
struct SharedObjectBase;

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
 * @sa SharedObjectBase.SharedObjectBase
 * @sa SharedObjectBase.LoadFunction
 * @sa SharedObjectRef.Unload
 * @sa SharedObject
 * @sa SharedObjectRef
 */
struct SharedObjectBase : Resource<SDL_SharedObject*>
{
  using Resource::Resource;

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
   * @sa SharedObjectBase.LoadFunction
   * @sa SharedObjectRef.Unload
   */
  SharedObjectBase(StringParam sofile)
    : Resource(SDL_LoadObject(sofile))
  {
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
   * @sa SharedObjectBase.SharedObjectBase
   */
  FunctionPointer LoadFunction(StringParam name)
  {
    return SDL_LoadFunction(get(), name);
  }
};

/**
 * Handle to a non owned sharedObject
 *
 * @cat resource
 *
 * @sa SharedObjectBase
 * @sa SharedObject
 */
struct SharedObjectRef : SharedObjectBase
{
  using SharedObjectBase::SharedObjectBase;

  /**
   * Copy constructor.
   */
  constexpr SharedObjectRef(const SharedObjectRef& other)
    : SharedObjectBase(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr SharedObjectRef(SharedObjectRef&& other)
    : SharedObjectBase(other.release())
  {
  }

  /**
   * Default constructor
   */
  constexpr ~SharedObjectRef() = default;

  /**
   * Assignment operator.
   */
  SharedObjectRef& operator=(SharedObjectRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Unload a shared object from memory.
   *
   * Note that any pointers from this object looked up through
   * SharedObjectBase.LoadFunction() will no longer be valid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SharedObjectBase.SharedObjectBase
   */
  void reset(SDL_SharedObject* newResource = {})
  {
    SDL_UnloadObject(release(newResource));
  }

  /**
   * Unload a shared object from memory.
   *
   * Note that any pointers from this object looked up through
   * SharedObjectBase.LoadFunction() will no longer be valid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SharedObjectBase.SharedObjectBase
   */
  void Unload() { reset(); }
};

/**
 * Handle to an owned sharedObject
 *
 * @cat resource
 *
 * @sa SharedObjectBase
 * @sa SharedObjectRef
 */
struct SharedObject : SharedObjectRef
{
  using SharedObjectRef::SharedObjectRef;

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit SharedObject(SDL_SharedObject* resource = {})
    : SharedObjectRef(resource)
  {
  }

  constexpr SharedObject(const SharedObject& other) = delete;

  /**
   * Move constructor.
   */
  constexpr SharedObject(SharedObject&& other) = default;

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

/// @}

} // namespace SDL

#endif /* SDL3PP_LOADSO_H_ */
