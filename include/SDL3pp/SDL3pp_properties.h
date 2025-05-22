#ifndef SDL3PP_PROPERTIES_H_
#define SDL3PP_PROPERTIES_H_

#include <SDL3/SDL_properties.h>
#include "SDL3pp_error.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryProperties Object Properties
 *
 * A property is a variable that can be created and retrieved by name at
 * runtime.
 *
 * All properties are part of a property group (PropertiesRef). A property
 * group can be created with the CreateProperties() function or by simply
 * instantiating @ref Properties. It can be destroyed with the
 * PropertiesRef.reset(), but the Properties destructor probably will do what
 * you want to, automatically.
 *
 * Properties can be added to and retrieved from a property group through the
 * following functions:
 *
 * - PropertiesRef.SetPointer() and PropertiesRef.GetPointer() operate on
 * `void*` pointer types.
 * - PropertiesRef.SetString() and PropertiesRef.GetString() operate on string
 * types.
 * - PropertiesRef.SetNumber() and PropertiesRef.GetNumber() operate on signed
 * 64-bit integer types.
 * - PropertiesRef.SetFloat() and PropertiesRef.GetFloat() operate on floating
 * point types.
 * - PropertiesRef.SetBoolean() and PropertiesRef.GetBoolean() operate on
 * boolean types.
 *
 * Properties can be removed from a group by using PropertiesRef.Clear().
 * @{
 */

/**
 * SDL property type
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PropertyType = SDL_PropertyType;

/**
 * @name Callbacks for PropertiesRef.SetPointerWithCleanup()
 * @{
 */

/**
 * A callback used to free resources when a property is deleted.
 *
 * This should release any resources associated with `value` that are no
 * longer needed.
 *
 * This callback is set per-property. Different properties in the same group
 * can have different cleanup callbacks.
 *
 * This callback will be called _during_ PropertiesRef.SetPointerWithCleanup if
 * the function fails for any reason.
 *
 * @param userdata an app-defined pointer passed to the callback.
 * @param value the pointer assigned to the property to clean up.
 *
 * @threadsafety This callback may fire without any locks held; if this is a
 *               concern, the app should provide its own locking.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa PropertiesRef.SetPointerWithCleanup
 */
using CleanupPropertyCallback = SDL_CleanupPropertyCallback;

/**
 * A callback used to free resources when a property is deleted.
 *
 * @sa CleanupPropertyCallback
 * @sa PropertiesRef.SetPointerWithCleanup()
 * @sa result-callback
 *
 * @cat result-callback
 */
using CleanupPropertyCB = std::function<void(void*)>;

/// @}
/**
 * @name Callbacks for PropertiesRef.Enumerate()
 * @{
 */

/**
 * A callback used to enumerate all the properties in a group of properties.
 *
 * This callback is called from PropertiesRef.Enumerate(), and is called once
 * per property in the set.
 *
 * @param userdata an app-defined pointer passed to the callback.
 * @param props the PropertiesRef that is being enumerated.
 * @param name the next property name in the enumeration.
 *
 * @threadsafety PropertiesRef.Enumerate holds a lock on `props` during this
 *               callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa PropertiesRef.Enumerate
 */
using EnumeratePropertiesCallback = SDL_EnumeratePropertiesCallback;

// Forward decl
struct PropertiesRef;

/**
 * A callback used to enumerate all the properties in a group of properties.
 *
 * This callback is called from PropertiesRef.Enumerate(), and is called once
 * per property in the set.
 *
 * @param props the PropertiesRef that is being enumerated.
 * @param name the next property name in the enumeration.
 *
 * @threadsafety PropertiesRef.Enumerate holds a lock on `props` during this
 *               callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat immediate-callback
 *
 *
 * @sa PropertiesRef.Enumerate
 * @sa EnumeratePropertiesCallback
 */
using EnumeratePropertiesCB =
  std::function<void(PropertiesRef props, const char* name)>;

/// @}

// Forward decl
struct PropertiesLock;

// Forward decl
struct Properties;

constexpr PropertyType PROPERTY_TYPE_INVALID =
  SDL_PROPERTY_TYPE_INVALID; ///< INVALID

constexpr PropertyType PROPERTY_TYPE_POINTER =
  SDL_PROPERTY_TYPE_POINTER; ///< POINTER

constexpr PropertyType PROPERTY_TYPE_STRING =
  SDL_PROPERTY_TYPE_STRING; ///< STRING

constexpr PropertyType PROPERTY_TYPE_NUMBER =
  SDL_PROPERTY_TYPE_NUMBER; ///< NUMBER

constexpr PropertyType PROPERTY_TYPE_FLOAT = SDL_PROPERTY_TYPE_FLOAT; ///< FLOAT

constexpr PropertyType PROPERTY_TYPE_BOOLEAN =
  SDL_PROPERTY_TYPE_BOOLEAN; ///< BOOLEAN

/**
 * Wrap properties id
 *
 * A property is a variable that can be created and retrieved by name at
 * runtime.
 *
 * All properties are part of a property group (Properties). A property
 * group can be created with the Properties constructor and destroyed
 * with this goes out of scope.
 *
 * Properties can be added to and retrieved from a property group through the
 * following functions:
 *
 * - SetPointer() and GetPointer() operate on `void*`
 *   pointer types.
 * - SetString() and GetString() operate on string types.
 * - SetNumber() and GetNumber() operate on signed 64-bit
 *   integer types.
 * - SetFloat() and GetFloat() operate on floating point
 *   types.
 * - SetBoolean() and GetBoolean() operate on boolean
 *   types.
 *
 * Properties can be removed from a group by using SDL_ClearProperty.
 *
 * To create a new properties group use CreateProperties() or
 * Properties.Properties().
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa Properties.Properties
 * @sa Properties
 */
struct PropertiesRef : Resource<SDL_PropertiesID>
{
  using Resource::Resource;

  /**
   * Copy constructor.
   */
  constexpr PropertiesRef(const PropertiesRef& other)
    : PropertiesRef(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr PropertiesRef(PropertiesRef&& other)
    : PropertiesRef(other.release())
  {
  }

  /**
   * Assignment operator.
   */
  PropertiesRef& operator=(PropertiesRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Copy a group of properties.
   *
   * Copy all the properties from one group of properties to another, with the
   * exception of properties requiring cleanup (set using
   * PropertiesRef.SetPointerWithCleanup()), which will not be copied. Any
   * property that already exists on `dst` will be overwritten.
   *
   * @param dst the destination properties.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void CopyPropertiesTo(PropertiesRef& dst) const
  {
    CheckError(SDL_CopyProperties(get(), dst.get()));
  }

  /**
   * Lock a group of properties.
   *
   * Obtain a multi-threaded lock for these properties. Other threads will wait
   * while trying to lock these properties until they are unlocked. Properties
   * must be unlocked before they are destroyed.
   *
   * The lock is automatically taken when setting individual properties, this
   * function is only needed when you want to set several properties atomically
   * or want to guarantee that properties being queried aren't freed in another
   * thread.
   *
   * @returns PropertiesLock on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesLock.Unlock
   */
  PropertiesLock Lock() &;

  /**
   * Set a pointer property in a group of properties with a cleanup
   * function that is called when the property is deleted.
   *
   * The cleanup function is also called if setting the property fails for any
   * reason.
   *
   * For simply setting basic data types, like numbers, bools, or strings, use
   * SetNumber(), SetBoolean(), or SetString()
   * instead, as those functions will handle cleanup on your behalf. This
   * function is only for more complex, custom data.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or NULL to delete the property.
   * @param cleanup the function to call when this property is deleted.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @sa result-callback
   *
   * @cat result-callback
   *
   */
  void SetPointerWithCleanup(StringParam name,
                             void* value,
                             CleanupPropertyCB cleanup)
  {
    using Wrapper = CallbackWrapper<CleanupPropertyCB>;
    SetPointerWithCleanup(std::move(name),
                          value,
                          &Wrapper::CallOnce,
                          Wrapper::Wrap(std::move(cleanup)));
  }

  /**
   * Set a pointer property in a group of properties with a cleanup function
   * that is called when the property is deleted.
   *
   * The cleanup function is also called if setting the property fails for any
   * reason.
   *
   * For simply setting basic data types, like numbers, bools, or strings, use
   * PropertiesRef.SetNumber, PropertiesRef.SetBoolean, or
   * PropertiesRef.SetString instead, as those functions will handle cleanup on
   * your behalf. This function is only for more complex, custom data.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or nullptr to delete the
   *              property.
   * @param cleanup the function to call when this property is deleted, or
   *                nullptr if no cleanup is necessary.
   * @param userdata a pointer that is passed to the cleanup function.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetPointer
   * @sa PropertiesRef.SetPointer
   * @sa CleanupPropertyCallback
   */
  void SetPointerWithCleanup(StringParam name,
                             void* value,
                             CleanupPropertyCallback cleanup,
                             void* userdata)
  {
    CheckError(
      SDL_SetPointerPropertyWithCleanup(get(), name, value, cleanup, userdata));
  }

  /**
   * Set a pointer property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or nullptr to delete the
   *              property.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetPointer
   * @sa PropertiesRef.Has
   * @sa PropertiesRef.SetBoolean
   * @sa PropertiesRef.SetFloat
   * @sa PropertiesRef.SetNumber
   * @sa PropertiesRef.SetPointerWithCleanup
   * @sa PropertiesRef.SetString
   */
  void SetPointer(StringParam name, void* value)
  {
    CheckError(SDL_SetPointerProperty(get(), name, value));
  }

  /**
   * Set a string property in a group of properties.
   *
   * This function makes a copy of the string; the caller does not have to
   * preserve the data after this call completes.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or nullptr to delete the
   *              property.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetString
   */
  void SetString(StringParam name, StringParam value)
  {
    CheckError(SDL_SetStringProperty(get(), name, value));
  }

  /**
   * Set an integer property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetNumber
   */
  void SetNumber(StringParam name, Sint64 value)
  {
    CheckError(SDL_SetNumberProperty(get(), name, value));
  }

  /**
   * Set a floating point property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetFloat
   */
  void SetFloat(StringParam name, float value)
  {
    CheckError(SDL_SetFloatProperty(get(), name, value));
  }

  /**
   * Set a boolean property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetBoolean
   */
  void SetBoolean(StringParam name, bool value)
  {
    CheckError(SDL_SetBooleanProperty(get(), name, value));
  }

  /**
   * Return whether a property exists in a group of properties.
   *
   * @param name the name of the property to query.
   * @returns true if the property exists, or false if it doesn't.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetType
   */
  bool Has(StringParam name) const { return SDL_HasProperty(get(), name); }

  /**
   * Get the type of a property.
   *
   * @param name the name of the property to query.
   * @returns the type of the property, or PROPERTY_TYPE_INVALID if it is
   *          not set.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.Has
   */
  PropertyType GetType(StringParam name) const
  {
    return SDL_GetPropertyType(get(), name);
  }

  /**
   * Get a pointer property from a group of properties.
   *
   * You can use SDL_GetPropertyType() to query whether the property exists and
   * is a pointer property.
   *
   * By convention, the names of properties that SDL exposes on objects will
   * start with "SDL.", and properties that SDL uses internally will start with
   * "SDL.internal.". These should be considered read-only and should not be
   * modified by applications.
   *
   * @param name the name of the property to query.
   * @param default_value the default value of the property.
   * @returns the value of the property, or `default_value` if it is not set or
   *          not a pointer property.
   *
   * @threadsafety It is safe to call this function from any thread, although
   *               the data returned is not protected and could potentially be
   *               freed if you call PropertiesRef.SetPointer() or
   *               PropertiesRef.Clear() on these properties from another
   *               thread. If you need to avoid this, use PropertiesRef.Lock()
   *               and PropertiesLock.Unlock().
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetBoolean
   * @sa PropertiesRef.GetFloat
   * @sa PropertiesRef.GetNumber
   * @sa PropertiesRef.GetType
   * @sa PropertiesRef.GetString
   * @sa PropertiesRef.Has
   * @sa PropertiesRef.SetPointer
   */
  void* GetPointer(StringParam name, void* default_value) const
  {
    return SDL_GetPointerProperty(get(), name, default_value);
  }

  /**
   * Get a string property from a group of properties.
   *
   * You can use SDL_GetPropertyType() to query whether the property exists and
   * is a string property.
   *
   * By convention, the names of properties that SDL exposes on objects will
   * start with "SDL.", and properties that SDL uses internally will start with
   * "SDL.internal.". These should be considered read-only and should not be
   * modified by applications.
   *
   * @param name the name of the property to query.
   * @param default_value the default value of the property.
   * @returns the value of the property, or `default_value` if it is not set or
   *          not a string property.
   *
   * @threadsafety It is safe to call this function from any thread, although
   *               the data returned is not protected and could potentially be
   *               freed if you call PropertiesRef.SetString() or
   *               PropertiesRef.Clear() on these properties from another
   *               thread. If you need to avoid this, use PropertiesRef.Lock()
   *               and PropertiesLock.Unlock().
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetType
   * @sa PropertiesRef.Has
   * @sa PropertiesRef.SetString
   */
  const char* GetString(StringParam name, StringParam default_value) const
  {
    return SDL_GetStringProperty(get(), name, default_value);
  }

  /**
   * Get a number property from a group of properties.
   *
   * You can use PropertiesRef.GetType() to query whether the property exists
   * and is a number property.
   *
   * By convention, the names of properties that SDL exposes on objects will
   * start with "SDL.", and properties that SDL uses internally will start with
   * "SDL.internal.". These should be considered read-only and should not be
   * modified by applications.
   *
   * @param name the name of the property to query.
   * @param default_value the default value of the property.
   * @returns the value of the property, or `default_value` if it is not set or
   *          not a number property.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetType
   * @sa PropertiesRef.Has
   * @sa PropertiesRef.SetNumber
   */
  Sint64 GetNumber(StringParam name, Sint64 default_value) const
  {
    return SDL_GetNumberProperty(get(), name, default_value);
  }

  /**
   * Get a floating point property from a group of properties.
   *
   * You can use PropertiesRef.GetType() to query whether the property exists
   * and is a floating point property.
   *
   * By convention, the names of properties that SDL exposes on objects will
   * start with "SDL.", and properties that SDL uses internally will start with
   * "SDL.internal.". These should be considered read-only and should not be
   * modified by applications.
   *
   * @param name the name of the property to query.
   * @param default_value the default value of the property.
   * @returns the value of the property, or `default_value` if it is not set or
   *          not a float property.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetType
   * @sa PropertiesRef.Has
   * @sa PropertiesRef.SetFloat
   */
  float GetFloat(StringParam name, float default_value) const
  {
    return SDL_GetFloatProperty(get(), name, default_value);
  }

  /**
   * Get a boolean property from a group of properties.
   *
   * You can use PropertiesRef.GetType() to query whether the property exists
   * and is a boolean property.
   *
   * By convention, the names of properties that SDL exposes on objects will
   * start with "SDL.", and properties that SDL uses internally will start with
   * "SDL.internal.". These should be considered read-only and should not be
   * modified by applications.
   *
   * @param name the name of the property to query.
   * @param default_value the default value of the property.
   * @returns the value of the property, or `default_value` if it is not set or
   *          not a boolean property.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetType
   * @sa PropertiesRef.Has
   * @sa PropertiesRef.SetBoolean
   */
  bool GetBoolean(StringParam name, bool default_value) const
  {
    return SDL_GetBooleanProperty(get(), name, default_value);
  }

  /**
   * Clear a property from a group of properties.
   *
   * @param name the name of the property to clear.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Clear(StringParam name) { CheckError(SDL_ClearProperty(get(), name)); }

  /**
   * Enumerate the properties contained in a group of properties.
   *
   * @param outputIter an output iterator to be assigned to each property name.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  template<std::output_iterator<const char*> IT>
  void Enumerate(IT outputIter) const
  {
    Enumerate(
      [&outputIter](auto props, const char name) { *outputIter++ = name; });
  }

  /**
   * Enumerate the properties contained in a group of properties.
   *
   * The callback function is called for each property in the group of
   * properties. The properties are locked during enumeration.
   *
   * @param callback the function to call for each property.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @cat immediate-callback
   *
   * @sa immediate-callback
   */
  void Enumerate(EnumeratePropertiesCB callback) const;

  /**
   * Enumerate the properties contained in a group of properties.
   *
   * The callback function is called for each property in the group of
   * properties. The properties are locked during enumeration.
   *
   * @param callback the function to call for each property.
   * @param userdata a pointer that is passed to `callback`.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Enumerate(EnumeratePropertiesCallback callback, void* userdata) const
  {
    CheckError(SDL_EnumerateProperties(get(), callback, userdata));
  }

  /**
   * Returns the number of properties this has
   *
   * This uses Enumerate() internally, so might not be so fast
   */
  Uint64 GetCount() const;

protected:
  /**
   * Destroy a group of properties.
   *
   * All properties are deleted and their cleanup functions will be called, if
   * any.
   *
   *
   * @threadsafety This function should not be called while these properties are
   *               locked or other threads might be setting or getting values
   *               from these properties.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.Properties
   */
  void Destroy() { reset(); }

  /**
   * Destroy a group of properties.
   *
   * All properties are deleted and their cleanup functions will be called, if
   * any.
   *
   * @threadsafety This function should not be called while these properties are
   *               locked or other threads might be setting or getting values
   *               from these properties.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.Properties
   */
  void reset(SDL_PropertiesID newResource = {})
  {
    SDL_DestroyProperties(release(newResource));
  }
};

/**
 * Unsafe Handle to properties
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa PropertiesRef
 */
struct PropertiesUnsafe : PropertiesRef
{
  using PropertiesRef::Destroy;

  using PropertiesRef::PropertiesRef;

  using PropertiesRef::reset;
};

/**
 * Handle to an owned properties
 *
 * @cat resource
 *
 * @sa PropertiesRef
 */
struct Properties : PropertiesUnsafe
{
  using PropertiesUnsafe::PropertiesUnsafe;

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit Properties(SDL_PropertiesID resource)
    : PropertiesUnsafe(resource)
  {
  }

  constexpr Properties(const Properties& other) = delete;

  /**
   * Move constructor.
   */
  constexpr Properties(Properties&& other) = default;

  /**
   * Create a group of properties.
   *
   * All properties are automatically destroyed when Quit() is called.
   *
   * @post an ID for a new group of properties
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.Destroy
   */
  explicit Properties()
    : Properties(CheckError(SDL_CreateProperties()))
  {
  }

  /**
   * Frees up resource when object goes out of scope.
   */
  ~Properties() { reset(); }

  /**
   * Assignment operator.
   */
  Properties& operator=(Properties other)
  {
    reset(other.release());
    return *this;
  }
};

/**
 * Wrap the lock state for PropertiesRef
 *
 */
class PropertiesLock
{
  PropertiesRef properties;

  /**
   * @sa PropertiesRef.Lock()
   */
  explicit PropertiesLock(PropertiesRef properties)
    : properties(properties)
  {
  }

public:
  /// Default ctor
  constexpr PropertiesLock() = default;

  PropertiesLock(const PropertiesLock& other) = delete;

  /// Move ctor
  PropertiesLock(PropertiesLock&& other)
    : properties(std::move(other.properties))
  {
  }

  /**
   * @sa Unlock()
   */
  ~PropertiesLock() { Unlock(); }

  /// Assignment operator
  PropertiesLock& operator=(PropertiesLock other)
  {
    std::swap(properties, other.properties);
    return *this;
  }

  /**
   * Returns true if lock is active
   */
  constexpr operator bool() const { return bool(properties); }

  /**
   * Unlock a group of properties.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.Lock
   */
  void Unlock() { SDL_UnlockProperties(properties.release()); }

  friend class PropertiesRef;
};

/**
 * Get the global SDL properties.
 *
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesRef GetGlobalProperties()
{
  return CheckError(SDL_GetGlobalProperties());
}

inline PropertiesLock PropertiesRef::Lock() &
{
  CheckError(SDL_LockProperties(get()));
  return PropertiesLock{get()};
}

/**
 * Create a group of properties.
 *
 * All properties are automatically destroyed when Quit() is called.
 *
 * @returns a valid Properties for a new group of properties, or false on
 *          failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties
 */
inline Properties CreateProperties()
{
  return Properties{SDL_CreateProperties()};
}

#pragma region impl
/// @}

inline void PropertiesRef::Enumerate(EnumeratePropertiesCB callback) const
{
  return Enumerate(
    [](void* userdata, SDL_PropertiesID props, const char* name) {
      auto& f = *static_cast<EnumeratePropertiesCB*>(userdata);
      f(props, name);
    },
    &callback);
}

inline Uint64 PropertiesRef::GetCount() const
{
  Uint64 count = 0;
  Enumerate([&](auto, const char*) { count++; });
  return count;
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_PROPERTIES_H_ */
