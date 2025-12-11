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
 * To use them, build such a library, then call SharedObject.SharedObject() on
 * it. Once loaded, you can use SharedObject.LoadFunction() on that object to
 * find the address of its exported symbols. When done with the object, call
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
 *   not defined whether or not it goes into the global symbol namespace for the
 *   application. If it does and it conflicts with symbols in your code or other
 *   shared libraries, you will not get the results you expect. :)
 * - Once a library is unloaded, all pointers into it obtained through
 *   SharedObject.LoadFunction() become invalid, even if the library is later
 *   reloaded. Don't unload a library if you plan to use these pointers in the
 *   future. Notably: beware of giving one of these pointers to atexit(), since
 *   it may call that pointer after the library unloads.
 *
 * @{
 */

// Forward decl
struct SharedObject;

/// Alias to raw representation for SharedObject.
using SharedObjectRaw = SDL_SharedObject*;

// Forward decl
struct SharedObjectRef;

/// Safely wrap SharedObject for non owning parameters
struct SharedObjectParam
{
  SharedObjectRaw value; ///< parameter's SharedObjectRaw

  /// Constructs from SharedObjectRaw
  constexpr SharedObjectParam(SharedObjectRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr SharedObjectParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const SharedObjectParam& other) const = default;

  /// Converts to underlying SharedObjectRaw
  constexpr operator SharedObjectRaw() const { return value; }
};

/**
 * An opaque datatype that represents a loaded shared object.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SharedObject.SharedObject
 * @sa SharedObject.LoadFunction
 * @sa SharedObject.Unload
 *
 * @cat resource
 */
class SharedObject
{
  SharedObjectRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr SharedObject() = default;

  /**
   * Constructs from SharedObjectParam.
   *
   * @param resource a SharedObjectRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit SharedObject(const SharedObjectRaw resource) noexcept
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr SharedObject(const SharedObject& other) = delete;

  /// Move constructor
  constexpr SharedObject(SharedObject&& other) noexcept
    : SharedObject(other.release())
  {
  }

  constexpr SharedObject(const SharedObjectRef& other) = delete;

  constexpr SharedObject(SharedObjectRef&& other) = delete;

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
   * @sa SharedObject.LoadFunction
   * @sa SharedObject.Unload
   */
  SharedObject(StringParam sofile)
    : m_resource(SDL_LoadObject(sofile))
  {
  }

  /// Destructor
  ~SharedObject() { SDL_UnloadObject(m_resource); }

  /// Assignment operator.
  constexpr SharedObject& operator=(SharedObject&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr SharedObject& operator=(const SharedObject& other) noexcept =
    default;

public:
  /// Retrieves underlying SharedObjectRaw.
  constexpr SharedObjectRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying SharedObjectRaw and clear this.
  constexpr SharedObjectRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const SharedObject& other) const noexcept =
    default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to SharedObjectParam
  constexpr operator SharedObjectParam() const noexcept { return {m_resource}; }

  /**
   * Unload a shared object from memory.
   *
   * Note that any pointers from this object looked up through
   * SharedObject.LoadFunction() will no longer be valid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SharedObject.SharedObject
   */
  void Unload();

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
   * @sa SharedObject.SharedObject
   */
  FunctionPointer LoadFunction(StringParam name);
};

/// Semi-safe reference for SharedObject.
struct SharedObjectRef : SharedObject
{
  using SharedObject::SharedObject;

  /**
   * Constructs from SharedObjectParam.
   *
   * @param resource a SharedObjectRaw or SharedObject.
   *
   * This does not takes ownership!
   */
  SharedObjectRef(SharedObjectParam resource) noexcept
    : SharedObject(resource.value)
  {
  }

  /**
   * Constructs from SharedObjectParam.
   *
   * @param resource a SharedObjectRaw or SharedObject.
   *
   * This does not takes ownership!
   */
  SharedObjectRef(SharedObjectRaw resource) noexcept
    : SharedObject(resource)
  {
  }

  /// Copy constructor.
  SharedObjectRef(const SharedObjectRef& other) noexcept
    : SharedObject(other.get())
  {
  }

  /// Destructor
  ~SharedObjectRef() { release(); }
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
 * @sa SharedObject.LoadFunction
 * @sa SharedObject.Unload
 */
inline SharedObject LoadObject(StringParam sofile)
{
  return SharedObject(std::move(sofile));
}

/**
 * Look up the address of the named function in a shared object.
 *
 * This function pointer is no longer valid after calling SharedObject.Unload().
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
 * @param handle a valid shared object handle returned by
 *               SharedObject.SharedObject().
 * @param name the name of the function to look up.
 * @returns a pointer to the function or nullptr on failure; call GetError() for
 *          more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SharedObject.SharedObject
 */
inline FunctionPointer LoadFunction(SharedObjectParam handle, StringParam name)
{
  return SDL_LoadFunction(handle, name);
}

inline FunctionPointer SharedObject::LoadFunction(StringParam name)
{
  return SDL::LoadFunction(m_resource, std::move(name));
}

/**
 * Unload a shared object from memory.
 *
 * Note that any pointers from this object looked up through
 * SharedObject.LoadFunction() will no longer be valid.
 *
 * @param handle a valid shared object handle returned by
 *               SharedObject.SharedObject().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SharedObject.SharedObject
 */
inline void UnloadObject(SharedObjectRaw handle) { SDL_UnloadObject(handle); }

inline void SharedObject::Unload() { UnloadObject(release()); }

/// @}

} // namespace SDL

#endif /* SDL3PP_LOADSO_H_ */
