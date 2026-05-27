#ifndef SDL3PP_LOADSO_H_
#define SDL3PP_LOADSO_H_

#include <SDL3/SDL_loadso.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategorySharedObject Shared Object/DLL Management
 *
 * System-dependent library loading routines.
 *
 * Shared objects are code that is programmatically loadable at runtime. Windows
 * calls these "DLLs", Linux calls them "shared libraries", etc.
 *
 * To use them, build such a library, then call LoadObject() on it. Once loaded,
 * you can use LoadFunction() on that object to find the address of its exported
 * symbols. When done with the object, call UnloadObject() to dispose of it.
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
 *   not defined whether or not it goes into the global symbol namespace for the
 *   application. If it does and it conflicts with symbols in your code or other
 *   shared libraries, you will not get the results you expect. :)
 * - Once a library is unloaded, all pointers into it obtained through
 *   LoadFunction() become invalid, even if the library is later reloaded. Don't
 *   unload a library if you plan to use these pointers in the future. Notably:
 *   beware of giving one of these pointers to atexit(), since it may call that
 *   pointer after the library unloads.
 *
 * @{
 */

// Forward decl
struct SharedObjectBase;

// Forward decl
struct SharedObject;

/// Alias to raw representation for SharedObject.
using SharedObjectRaw = SDL_SharedObject*;

/**
 * Reference for SharedObject.
 *
 * This does not take ownership!
 */
using SharedObjectRef = ResourceRefT<SharedObjectBase>;

/**
 * Base class to SharedObject.
 *
 * @see SharedObject
 */
struct SharedObjectBase : ResourceBaseT<SharedObjectRaw>
{
  using ResourceBaseT::ResourceBaseT;

  /**
   * Unload a shared object from memory.
   *
   * Note that any pointers from this object looked up through LoadFunction()
   * will no longer be valid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LoadObject
   */
  void Unload();

  /**
   * Look up the address of the named function in a shared object.
   *
   * This function pointer is no longer valid after calling UnloadObject().
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
   * @sa LoadObject
   */
  FunctionPointer LoadFunction(StringParam name);
};

/**
 * An opaque datatype that represents a loaded shared object.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa LoadObject
 * @sa LoadFunction
 * @sa UnloadObject
 *
 * @cat resource
 */
struct SharedObject : SharedObjectBase
{
  using SharedObjectBase::SharedObjectBase;

  /**
   * Constructs from raw SharedObject.
   *
   * @param resource a SharedObjectRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit SharedObject(SharedObjectRaw resource) noexcept
    : SharedObjectBase(resource)
  {
  }

  /// Move constructor
  constexpr SharedObject(SharedObject&& other) noexcept
    : SharedObject(other.release())
  {
  }

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
   * @sa LoadFunction
   * @sa UnloadObject
   */
  SharedObject(StringParam sofile);

  /// Destructor
  ~SharedObject() { SDL_UnloadObject(get()); }

  /// Assignment operator.
  constexpr SharedObject& operator=(SharedObject&& other) noexcept
  {
    swap(*this, other);
    return *this;
  }
};

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
 * @sa LoadFunction
 * @sa UnloadObject
 */
inline SharedObject LoadObject(StringParam sofile)
{
  return SharedObject(std::move(sofile));
}

inline SharedObject::SharedObject(StringParam sofile)
  : SharedObject(SDL_LoadObject(sofile))
{
}

/**
 * Look up the address of the named function in a shared object.
 *
 * This function pointer is no longer valid after calling UnloadObject().
 *
 * This function can only look up C function names. Other languages may have
 * name mangling and intrinsic language support that varies from compiler to
 * compiler.
 *
 * Make sure you declare your function pointers with the same calling convention
 * as the actual library function. Your code will crash mysteriously if you do
 * not do this.
 *
 * If the requested function doesn't exist, nullptr is returned.
 *
 * @param handle a valid shared object handle returned by LoadObject().
 * @param name the name of the function to look up.
 * @returns a pointer to the function or nullptr on failure; call GetError() for
 *          more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LoadObject
 */
inline FunctionPointer LoadFunction(SharedObjectRef handle, StringParam name)
{
  return SDL_LoadFunction(handle, name);
}

inline FunctionPointer SharedObjectBase::LoadFunction(StringParam name)
{
  return SDL::LoadFunction(get(), std::move(name));
}

/**
 * Unload a shared object from memory.
 *
 * Note that any pointers from this object looked up through LoadFunction() will
 * no longer be valid.
 *
 * @param handle a valid shared object handle returned by LoadObject().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LoadObject
 */
inline void UnloadObject(SharedObjectRaw handle) { SDL_UnloadObject(handle); }

inline void SharedObjectBase::Unload() { UnloadObject(release()); }

/// @}

} // namespace SDL

#endif /* SDL3PP_LOADSO_H_ */
