#ifndef SDL3PP_PROPERTIES_H_
#define SDL3PP_PROPERTIES_H_

#include <SDL3/SDL_properties.h>
#include "SDL3pp_callbackWrapper.h"
#include "SDL3pp_error.h"
#include "SDL3pp_objectWrapper.h"
#include "SDL3pp_stringParam.h"

namespace SDL {

/**
 * @defgroup CategoryProperties Object Properties
 *
 * A property is a variable that can be created and retrieved by name at
 * runtime.
 *
 * All properties are part of a property group (SDL_PropertiesID). A property
 * group can be created with the SDL_CreateProperties function and destroyed
 * with the SDL_DestroyProperties function.
 *
 * Properties can be added to and retrieved from a property group through the
 * following functions:
 *
 * - PropertiesBase.SetPointer() and PropertiesBase.GetPointer() operate on
 * `void*` pointer types.
 * - PropertiesBase.SetString() and PropertiesBase.GetString() operate on string
 * types.
 * - PropertiesBase.SetNumber() and PropertiesBase.GetNumber() operate on signed
 * 64-bit integer types.
 * - PropertiesBase.SetFloat() and PropertiesBase.GetFloat() operate on floating
 * point types.
 * - PropertiesBase.SetBoolean() and PropertiesBase.GetBoolean() operate on
 * boolean types.
 *
 * Properties can be removed from a group by using PropertiesBase.Clear().
 * @{
 */

// Forward decl
template<ObjectBox<FancyPointer<SDL_PropertiesID>> T>
struct PropertiesBase;

/**
 * Handle to a non owned properties
 *
 * To create a new property group use CreateProperties()
 *
 * @ingroup resource
 *
 * @sa resource
 * @sa PropertiesBase
 * @sa Properties
 * @sa CreateProperties()
 */
using PropertiesRef =
  PropertiesBase<ObjectRef<SDL_PropertiesID, FancyPointer<SDL_PropertiesID>>>;

struct PropertiesDeleter
{
  using pointer = FancyPointer<SDL_PropertiesID>;
  inline void operator()(PropertiesRef props) const;
};

/**
 * Handle to an owned properties
 *
 * @ingroup resource
 *
 * @sa resource
 * @sa PropertiesBase
 * @sa PropertiesRef
 */
using Properties =
  PropertiesBase<ObjectUnique<SDL_PropertiesID, PropertiesDeleter>>;

/**
 * SDL property type
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PropertyType = SDL_PropertyType;

/**
 * @name Callbacks for PropertiesBase.SetPointerWithCleanup()
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
 * This callback will be called _during_ SetPointerWithCleanup() if
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
 * @sa PropertiesBase.SetPointerWithCleanup()
 */
using CleanupPropertyCallback = SDL_CleanupPropertyCallback;

/**
 * A callback used to free resources when a property is deleted.
 *
 * @sa PropertiesRef.CleanupPropertyCallback
 * @sa PropertiesBase.SetPointerWithCleanup()
 * @sa ResultCallback
 *
 * @ingroup ResultCallback
 */
using CleanupPropertyCB = std::function<void(void*)>;

/// @}
/**
 * @name Callbacks for PropertiesBase.Enumerate()
 * @{
 */

/**
 * A callback used to enumerate all the properties in a group of properties.
 *
 * This callback is called from PropertiesBase::Enumerate(), and is called once
 * per property in the set.
 *
 * @param userdata an app-defined pointer passed to the callback.
 * @param props the SDL_PropertiesID that is being enumerated.
 * @param name the next property name in the enumeration.
 *
 * @threadsafety SDL_EnumerateProperties holds a lock on `props` during this
 *               callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa PropertiesBase::Enumerate()
 */
using EnumeratePropertiesCallback = SDL_EnumeratePropertiesCallback;

/**
 * A callback used to enumerate all the properties in a group of properties.
 *
 * This callback is called from PropertiesBase::Enumerate(), and is called once
 * per property in the set.
 *
 * @sa EnumeratePropertyCallback
 * @sa PropertiesBase::Enumerate()
 * @sa SyncCallback
 *
 * @ingroup SyncCallback
 */
using EnumeratePropertiesCB =
  std::function<void(PropertiesRef props, const char* name)>;

/// @}

// Forward decl
struct PropertiesLock;

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
 * To create a new properties group use CreateProperties().
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @ingroup resource
 *
 * @sa resource
 * @sa CreateProperties()
 * @sa Properties
 * @sa PropertiesRef
 */
template<ObjectBox<FancyPointer<SDL_PropertiesID>> T>
struct PropertiesBase : T
{
  using T::T;

  /**
   * Copy a group of properties.
   *
   * Copy all the properties from one group of properties to another, with the
   * exception of properties requiring cleanup (set using
   * SetPointerWithCleanup()), which will not be copied. Any
   * property that already exists on `dst` will be overwritten.
   *
   * @param dst the destination properties.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool CopyPropertiesTo(PropertiesRef dst) const
  {
    return SDL_CopyProperties(T::get(), dst.get());
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
   * @returns PropertiesLock on success or false on failure; call GetError() for
   * more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesLock.UnlockProperties()
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
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @sa ResultCallback
   *
   * @ingroup ResultCallback
   *
   */
  bool SetPointerWithCleanup(StringParam name,
                             void* value,
                             CleanupPropertyCB cleanup)
  {
    using Wrapper = CallbackWrapper<CleanupPropertyCB>;

    return SetPointerWithCleanup(std::move(name),
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
   * SetNumber(), SetBoolean(), or SetString()
   * instead, as those functions will handle cleanup on your behalf. This
   * function is only for more complex, custom data.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or NULL to delete the property.
   * @param cleanup the function to call when this property is deleted, or NULL
   *                if no cleanup is necessary.
   * @param userdata a pointer that is passed to the cleanup function.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetPointer
   * @sa PropertiesRef.SetPointer
   * @sa PropertiesRef.CleanupCallback
   */
  bool SetPointerWithCleanup(StringParam name,
                             void* value,
                             CleanupPropertyCallback cleanup,
                             void* userdata)
  {
    return SDL_SetPointerPropertyWithCleanup(
      T::get(), name, value, cleanup, userdata);
  }

  /**
   * Set a pointer property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or NULL to delete the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetPointer()
   * @sa Has()
   * @sa SetBoolean()
   * @sa SetFloat()
   * @sa SetNumber()
   * @sa SetPointerWithCleanup()
   * @sa SetString()
   */
  bool SetPointer(StringParam name, void* value)
  {
    return SDL_SetPointerProperty(T::get(), name, value);
  }

  /**
   * Set a string property in a group of properties.
   *
   * This function makes a copy of the string; the caller does not have to
   * preserve the data after this call completes.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or NULL to delete the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetString()
   */
  bool SetString(StringParam name, StringParam value)
  {
    return SDL_SetStringProperty(T::get(), name, value);
  }

  /**
   * Set an integer property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetNumber()
   */
  bool SetNumber(StringParam name, Sint64 value)
  {
    return SDL_SetNumberProperty(T::get(), name, value);
  }

  /**
   * Set a floating point property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetFloat()
   */
  bool SetFloat(StringParam name, float value)
  {
    return SDL_SetFloatProperty(T::get(), name, value);
  }

  /**
   * Set a boolean property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetBoolean()
   */
  bool SetBoolean(StringParam name, bool value)
  {
    return SDL_SetBooleanProperty(T::get(), name, value);
  }

  /**
   * Return whether a property exists.
   *
   * @param name the name of the property to query.
   * @returns true if the property exists, or false if it doesn't.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetType()
   */
  bool Has(StringParam name) const { return SDL_HasProperty(T::get(), name); }

  /**
   * Get the type of a property.
   *
   * @param name the name of the property to query.
   * @returns the type of the property, or SDL_PROPERTY_TYPE_INVALID if it is
   *          not set.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Has()
   */
  PropertyType GetType(StringParam name) const
  {
    return SDL_GetPropertyType(T::get(), name);
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
   *               freed if you call SetProperty() or
   *               ClearProperty() on these properties from another thread.
   *               If you need to avoid this, use SDL_LockProperties() and
   *               SDL_UnlockProperties().
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetBoolean()
   * @sa GetFloat()
   * @sa GetNumber()
   * @sa GetType()
   * @sa GetString()
   * @sa Has()
   * @sa SetPointer()
   */
  void* GetPointer(StringParam name, void* default_value) const
  {
    return SDL_GetPointerProperty(T::get(), name, default_value);
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
   *               freed if you call SetString() or
   *               ClearProperty() on these properties from another thread.
   *               If you need to avoid this, use SDL_LockProperties() and
   *               SDL_UnlockProperties().
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa PropertiesRef.GetType()
   * @sa PropertiesRef.Has()
   * @sa PropertiesRef.SetString()
   */
  const char* GetString(StringParam name, StringParam default_value) const
  {
    return SDL_GetStringProperty(T::get(), name, default_value);
  }

  /**
   * Get a number property from a group of properties.
   *
   * You can use SDL_GetPropertyType() to query whether the property exists and
   * is a number property.
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
   * @sa GetType()
   * @sa Has()
   * @sa SetNumber()
   */
  Sint64 GetNumber(StringParam name, Sint64 default_value) const
  {
    return SDL_GetNumberProperty(T::get(), name, default_value);
  }

  /**
   * Get a floating point property from a group of properties.
   *
   * You can use SDL_GetPropertyType() to query whether the property exists and
   * is a floating point property.
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
   * @sa GetType()
   * @sa Has()
   * @sa SetFloat()
   */
  float GetFloat(StringParam name, float default_value) const
  {
    return SDL_GetFloatProperty(T::get(), name, default_value);
  }

  /**
   * Get a boolean property from a group of properties.
   *
   * You can use SDL_GetPropertyType() to query whether the property exists and
   * is a boolean property.
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
   * @sa GetType()
   * @sa Has()
   * @sa SetBoolean()
   */
  bool GetBoolean(StringParam name, bool default_value) const
  {
    return SDL_GetBooleanProperty(T::get(), name, default_value);
  }

  /**
   * Clear a property from a group of properties.
   *
   * @param name the name of the property to clear.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Clear(StringParam name) { return SDL_ClearProperty(T::get(), name); }

  /**
   * Enumerate the properties contained in a group of properties.
   *
   * @param outputIter an output iterator to be assigned to each property name
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  template<std::output_iterator<const char*> IT>
  bool Enumerate(IT outputIter) const
  {
    return Enumerate(
      [&outputIter](auto props, const char name) { *outputIter++ = name; });
  }

  /**
   * Enumerate the properties contained in a group of properties.
   *
   * The callback function is called for each property in the group of
   * properties. The properties are locked during enumeration.
   *
   * @param callback the function to call for each property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @ingroup SyncCallback
   *
   * @sa SyncCallback
   */
  bool Enumerate(EnumeratePropertiesCB callback) const
  {
    return Enumerate(
      [](void* userdata, SDL_PropertiesID props, const char* name) {
        auto& f = *static_cast<EnumeratePropertiesCB*>(userdata);
        f({props}, name);
      },
      &callback);
  }

  /**
   * Enumerate the properties contained in a group of properties.
   *
   * The callback function is called for each property in the group of
   * properties. The properties are locked during enumeration.
   *
   * @param callback the function to call for each property.
   * @param userdata a pointer that is passed to `callback`.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Enumerate(EnumeratePropertiesCallback callback, void* userdata) const
  {
    return SDL_EnumerateProperties(T::get(), callback, userdata);
  }

  /**
   * Destroy a group of properties.
   *
   * All properties are deleted and their cleanup functions will be called, if
   * any.
   *
   * @post This object becomes empty after the call.
   *
   * @threadsafety This function should not be called while these properties are
   *               locked or other threads might be setting or getting values
   *               from these properties.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Destroy() { return SDL_DestroyProperties(T::release()); }

  /**
   * Returns the number of properties this has
   *
   * This uses Enumerate() internally, so might not be so fast
   */
  Uint64 GetCount() const
  {
    Uint64 count = 0;
    if (Enumerate([&](SDL_PropertiesID, const char*) { count++; })) {
      return count;
    }
    return 0;
  }
};

/**
 * Wrap the lock state for PropertiesBase
 *
 */
class PropertiesLock
{
  PropertiesRef properties;

  /**
   * @sa PropertiesBase.Lock()
   */
  explicit PropertiesLock(PropertiesRef properties)
    : properties(properties)
  {
  }

public:
  /// Default ctor
  PropertiesLock()
    : properties(nullptr)
  {
  }

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
   * @sa PropertiesBase.Lock()
   */
  void Unlock() { return SDL_UnlockProperties(properties.release()); }

  template<ObjectBox<FancyPointer<SDL_PropertiesID>> T>
  friend class PropertiesBase;
};

constexpr PropertyType PROPERTY_TYPE_INVALID = SDL_PROPERTY_TYPE_INVALID;

constexpr PropertyType PROPERTY_TYPE_POINTER = SDL_PROPERTY_TYPE_POINTER;

constexpr PropertyType PROPERTY_TYPE_STRING = SDL_PROPERTY_TYPE_STRING;

constexpr PropertyType PROPERTY_TYPE_NUMBER = SDL_PROPERTY_TYPE_NUMBER;

constexpr PropertyType PROPERTY_TYPE_FLOAT = SDL_PROPERTY_TYPE_FLOAT;

constexpr PropertyType PROPERTY_TYPE_BOOLEAN = SDL_PROPERTY_TYPE_BOOLEAN;

/**
 * Get the global SDL properties.
 *
 * @returns a valid property ID on success or 0 on failure; call
 *          GetError() for more information.
 */
inline PropertiesRef GetGlobalProperties()
{
  return FancyPointer{SDL_GetGlobalProperties()};
}

/**
 * Create a group of properties.
 *
 * All properties are automatically destroyed when Quit() is called.
 *
 * @returns a valid Properties for a new group of properties, or false on
 * failure; call GetError() for more information.
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

inline void PropertiesDeleter::operator()(PropertiesRef props) const
{
  props.Destroy();
}

template<ObjectBox<FancyPointer<SDL_PropertiesID>> T>
PropertiesLock PropertiesBase<T>::Lock() &
{
  if (SDL_LockProperties(T::get())) return {*this};
  return {nullptr};
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_PROPERTIES_H_ */
