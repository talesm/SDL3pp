#ifndef SDL3PP_PROPERTIES_H_
#define SDL3PP_PROPERTIES_H_

#include <SDL3/SDL_properties.h>
#include "SDL3pp_callbackWrapper.h"
#include "SDL3pp_error.h"
#include "SDL3pp_strings.h"

namespace SDL {

/**
 * @defgroup CategoryProperties Category Properties
 *
 * A property is a variable that can be created and retrieved by name at
 * runtime.
 *
 * All properties are part of a property group (Properties). A property
 * group can be created with the Properties.Create function and destroyed
 * with the Properties.Destroy function.
 *
 * Properties can be added to and retrieved from a property group through the
 * following functions:
 *
 * - SetPointerProperty and GetPointerProperty operate on `void*`
 *   pointer types.
 * - SetStringProperty and GetStringProperty operate on string types.
 * - SetNumberProperty and GetNumberProperty operate on signed 64-bit
 *   integer types.
 * - SetFloatProperty and GetFloatProperty operate on floating point
 *   types.
 * - SetBooleanProperty and GetBooleanProperty operate on boolean
 *   types.
 *
 * Properties can be removed from a group by using ClearProperty.
 *
 * @{
 */

// Forward decl
struct Properties;

using PropertiesID = SDL_PropertiesID;

/**
 * Safely wrap Properties for non owning parameters
 */
struct PropertiesParam
{

  PropertiesID value;

  constexpr PropertiesParam(PropertiesID value)
    : value(value)
  {
  }

  constexpr PropertiesParam(std::nullptr_t _)
    : value(0)
  {
  }

  constexpr operator PropertiesID() const { return value; }
};

/**
 * SDL properties ID
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 */
class Properties
{

  PropertiesID m_resource = 0;

public:
  constexpr Properties() = default;

  constexpr explicit Properties(const PropertiesID resource)
    : m_resource(resource)
  {
  }

  Properties(const Properties& other) {}

  constexpr Properties(Properties&& other) { other.m_resource = 0; }

  /**
   * Create a group of properties.
   *
   * All properties are automatically destroyed when Quit() is called.
   *
   * @returns an ID for a new group of properties, or 0 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.Destroy
   */
  static Properties Create() { return Properties(SDL_CreateProperties()); }

  ~Properties() { SDL_DestroyProperties(m_resource); }

  Properties& operator=(Properties other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  constexpr PropertiesID get() const { return m_resource; }

  constexpr PropertiesID release()
  {
    auto r = m_resource;
    m_resource = 0;
    return r;
  }

  constexpr operator PropertiesParam() const { return {m_resource}; }

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
   * @sa Properties.Create
   */
  void Destroy()
  {
    SDL_DestroyProperties(m_resource);
    m_resource = 0;
  }
};

/**
 * SDL property type
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PropertyType = SDL_PropertyType;

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
 * Get the global SDL properties.
 *
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesID GetGlobalProperties()
{
  return CheckError(SDL_GetGlobalProperties());
}

/**
 * Create a group of properties.
 *
 * All properties are automatically destroyed when Quit() is called.
 *
 * @returns an ID for a new group of properties, or 0 on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.Destroy
 */
inline Properties CreateProperties()
{
  return Properties(SDL_CreateProperties());
}

/**
 * Copy a group of properties.
 *
 * Copy all the properties from one group of properties to another, with the
 * exception of properties requiring cleanup (set using
 * SetPointerPropertyWithCleanup()), which will not be copied. Any
 * property that already exists on `dst` will be overwritten.
 *
 * @param src the properties to copy.
 * @param dst the destination properties.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void CopyProperties(PropertiesParam src, PropertiesParam dst)
{
  CheckError(SDL_CopyProperties(src, dst));
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
 * @param props the properties to lock.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa UnlockProperties
 */
inline void LockProperties(PropertiesParam props)
{
  CheckError(SDL_LockProperties(props));
}

/**
 * Unlock a group of properties.
 *
 * @param props the properties to unlock.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LockProperties
 */
inline void UnlockProperties(PropertiesParam props)
{
  SDL_UnlockProperties(props);
}

/**
 * A callback used to free resources when a property is deleted.
 *
 * This should release any resources associated with `value` that are no
 * longer needed.
 *
 * This callback is set per-property. Different properties in the same group
 * can have different cleanup callbacks.
 *
 * This callback will be called _during_ SetPointerPropertyWithCleanup if
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
 * @sa SetPointerPropertyWithCleanup
 */
using CleanupPropertyCallback = SDL_CleanupPropertyCallback;

/**
 * A callback used to free resources when a property is deleted.
 *
 * This should release any resources associated with `value` that are no
 * longer needed.
 *
 * This callback is set per-property. Different properties in the same group
 * can have different cleanup callbacks.
 *
 * This callback will be called _during_ SetPointerPropertyWithCleanup if
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
 * @sa SetPointerPropertyWithCleanup
 * @sa CleanupPropertyCallback
 */
using CleanupPropertyCB = std::function<void(void*)>;

/**
 * Set a pointer property in a group of properties with a cleanup function
 * that is called when the property is deleted.
 *
 * The cleanup function is also called if setting the property fails for any
 * reason.
 *
 * For simply setting basic data types, like numbers, bools, or strings, use
 * SetNumberProperty, SetBooleanProperty, or SetStringProperty
 * instead, as those functions will handle cleanup on your behalf. This
 * function is only for more complex, custom data.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property, or nullptr to delete the
 * property.
 * @param cleanup the function to call when this property is deleted, or nullptr
 *                if no cleanup is necessary.
 * @param userdata a pointer that is passed to the cleanup function.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPointerProperty
 * @sa SetPointerProperty
 * @sa CleanupPropertyCallback
 */
inline void SetPointerPropertyWithCleanup(PropertiesParam props,
                                          StringParam name,
                                          void* value,
                                          CleanupPropertyCallback cleanup,
                                          void* userdata)
{
  CheckError(
    SDL_SetPointerPropertyWithCleanup(props, name, value, cleanup, userdata));
}

/**
 * Set a pointer property in a group of properties with a cleanup function
 * that is called when the property is deleted.
 *
 * The cleanup function is also called if setting the property fails for any
 * reason.
 *
 * For simply setting basic data types, like numbers, bools, or strings, use
 * SetNumberProperty, SetBooleanProperty, or SetStringProperty
 * instead, as those functions will handle cleanup on your behalf. This
 * function is only for more complex, custom data.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property, or nullptr to delete the
 * property.
 * @param cleanup the function to call when this property is deleted, or nullptr
 *                if no cleanup is necessary.
 * @param userdata a pointer that is passed to the cleanup function.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPointerProperty
 * @sa SetPointerProperty
 * @sa CleanupPropertyCallback
 */
inline void SetPointerPropertyWithCleanup(PropertiesParam props,
                                          StringParam name,
                                          void* value,
                                          CleanupPropertyCB cleanup)
{
  static_assert(false, "Not implemented");
}

/**
 * Set a pointer property in a group of properties.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property, or nullptr to delete the
 * property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPointerProperty
 * @sa HasProperty
 * @sa SetBooleanProperty
 * @sa SetFloatProperty
 * @sa SetNumberProperty
 * @sa SetPointerPropertyWithCleanup
 * @sa SetStringProperty
 */
inline void SetPointerProperty(PropertiesParam props,
                               StringParam name,
                               void* value)
{
  CheckError(SDL_SetPointerProperty(props, name, value));
}

/**
 * Set a string property in a group of properties.
 *
 * This function makes a copy of the string; the caller does not have to
 * preserve the data after this call completes.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property, or nullptr to delete the
 * property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetStringProperty
 */
inline void SetStringProperty(PropertiesParam props,
                              StringParam name,
                              StringParam value)
{
  CheckError(SDL_SetStringProperty(props, name, value));
}

/**
 * Set an integer property in a group of properties.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetNumberProperty
 */
inline void SetNumberProperty(PropertiesParam props,
                              StringParam name,
                              Sint64 value)
{
  CheckError(SDL_SetNumberProperty(props, name, value));
}

/**
 * Set a floating point property in a group of properties.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetFloatProperty
 */
inline void SetFloatProperty(PropertiesParam props,
                             StringParam name,
                             float value)
{
  CheckError(SDL_SetFloatProperty(props, name, value));
}

/**
 * Set a boolean property in a group of properties.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetBooleanProperty
 */
inline void SetBooleanProperty(PropertiesParam props,
                               StringParam name,
                               bool value)
{
  CheckError(SDL_SetBooleanProperty(props, name, value));
}

/**
 * Return whether a property exists in a group of properties.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @returns true if the property exists, or false if it doesn't.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPropertyType
 */
inline bool HasProperty(PropertiesParam props, StringParam name)
{
  return SDL_HasProperty(props, name);
}

/**
 * Get the type of a property in a group of properties.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @returns the type of the property, or PROPERTY_TYPE_INVALID if it is
 *          not set.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasProperty
 */
inline PropertyType GetPropertyType(PropertiesParam props, StringParam name)
{
  return SDL_GetPropertyType(props, name);
}

/**
 * Get a pointer property from a group of properties.
 *
 * By convention, the names of properties that SDL exposes on objects will
 * start with "SDL.", and properties that SDL uses internally will start with
 * "SDL.internal.". These should be considered read-only and should not be
 * modified by applications.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @param default_value the default value of the property.
 * @returns the value of the property, or `default_value` if it is not set or
 *          not a pointer property.
 *
 * @threadsafety It is safe to call this function from any thread, although
 *               the data returned is not protected and could potentially be
 *               freed if you call SetPointerProperty() or
 *               ClearProperty() on these properties from another thread.
 *               If you need to avoid this, use LockProperties() and
 *               UnlockProperties().
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetBooleanProperty
 * @sa GetFloatProperty
 * @sa GetNumberProperty
 * @sa GetPropertyType
 * @sa GetStringProperty
 * @sa HasProperty
 * @sa SetPointerProperty
 */
inline void* GetPointerProperty(PropertiesParam props,
                                StringParam name,
                                void* default_value)
{
  return SDL_GetPointerProperty(props, name, default_value);
}

/**
 * Get a string property from a group of properties.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @param default_value the default value of the property.
 * @returns the value of the property, or `default_value` if it is not set or
 *          not a string property.
 *
 * @threadsafety It is safe to call this function from any thread, although
 *               the data returned is not protected and could potentially be
 *               freed if you call SetStringProperty() or
 *               ClearProperty() on these properties from another thread.
 *               If you need to avoid this, use LockProperties() and
 *               UnlockProperties().
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPropertyType
 * @sa HasProperty
 * @sa SetStringProperty
 */
inline const char* GetStringProperty(PropertiesParam props,
                                     StringParam name,
                                     StringParam default_value)
{
  return SDL_GetStringProperty(props, name, default_value);
}

/**
 * Get a number property from a group of properties.
 *
 * You can use GetPropertyType() to query whether the property exists and
 * is a number property.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @param default_value the default value of the property.
 * @returns the value of the property, or `default_value` if it is not set or
 *          not a number property.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPropertyType
 * @sa HasProperty
 * @sa SetNumberProperty
 */
inline Sint64 GetNumberProperty(PropertiesParam props,
                                StringParam name,
                                Sint64 default_value)
{
  return SDL_GetNumberProperty(props, name, default_value);
}

/**
 * Get a floating point property from a group of properties.
 *
 * You can use GetPropertyType() to query whether the property exists and
 * is a floating point property.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @param default_value the default value of the property.
 * @returns the value of the property, or `default_value` if it is not set or
 *          not a float property.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPropertyType
 * @sa HasProperty
 * @sa SetFloatProperty
 */
inline float GetFloatProperty(PropertiesParam props,
                              StringParam name,
                              float default_value)
{
  return SDL_GetFloatProperty(props, name, default_value);
}

/**
 * Get a boolean property from a group of properties.
 *
 * You can use GetPropertyType() to query whether the property exists and
 * is a boolean property.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @param default_value the default value of the property.
 * @returns the value of the property, or `default_value` if it is not set or
 *          not a boolean property.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPropertyType
 * @sa HasProperty
 * @sa SetBooleanProperty
 */
inline bool GetBooleanProperty(PropertiesParam props,
                               StringParam name,
                               bool default_value)
{
  return SDL_GetBooleanProperty(props, name, default_value);
}

/**
 * Clear a property from a group of properties.
 *
 * @param props the properties to modify.
 * @param name the name of the property to clear.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ClearProperty(PropertiesParam props, StringParam name)
{
  CheckError(SDL_ClearProperty(props, name));
}

/**
 * A callback used to enumerate all the properties in a group of properties.
 *
 * This callback is called from EnumerateProperties(), and is called once
 * per property in the set.
 *
 * @param userdata an app-defined pointer passed to the callback.
 * @param props the Properties that is being enumerated.
 * @param name the next property name in the enumeration.
 *
 * @threadsafety EnumerateProperties holds a lock on `props` during this
 *               callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EnumerateProperties
 */
using EnumeratePropertiesCallback = SDL_EnumeratePropertiesCallback;

/**
 * A callback used to enumerate all the properties in a group of properties.
 *
 * This callback is called from EnumerateProperties(), and is called once
 * per property in the set.
 *
 * @param userdata an app-defined pointer passed to the callback.
 * @param props the Properties that is being enumerated.
 * @param name the next property name in the enumeration.
 *
 * @threadsafety EnumerateProperties holds a lock on `props` during this
 *               callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EnumerateProperties
 * @sa EnumeratePropertiesCallback
 */
using EnumeratePropertiesCB =
  std::function<void(PropertiesID props, const char* name)>;

/**
 * Enumerate the properties contained in a group of properties.
 *
 * The callback function is called for each property in the group of
 * properties. The properties are locked during enumeration.
 *
 * @param props the properties to query.
 * @param callback the function to call for each property.
 * @param userdata a pointer that is passed to `callback`.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void EnumerateProperties(PropertiesParam props,
                                EnumeratePropertiesCallback callback,
                                void* userdata)
{
  CheckError(SDL_EnumerateProperties(props, callback, userdata));
}

/**
 * Enumerate the properties contained in a group of properties.
 *
 * The callback function is called for each property in the group of
 * properties. The properties are locked during enumeration.
 *
 * @param props the properties to query.
 * @param callback the function to call for each property.
 * @param userdata a pointer that is passed to `callback`.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void EnumerateProperties(PropertiesParam props,
                                EnumeratePropertiesCB callback)
{
  static_assert(false, "Not implemented");
}

inline Uint64 CountProperties(PropertiesParam props)
{
  static_assert(false, "Not implemented");
}

/**
 * Destroy a group of properties.
 *
 * All properties are deleted and their cleanup functions will be called, if
 * any.
 *
 * @param props the properties to destroy.
 *
 * @threadsafety This function should not be called while these properties are
 *               locked or other threads might be setting or getting values
 *               from these properties.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.Create
 */
inline void DestroyProperties(PropertiesID props)
{
  SDL_DestroyProperties(props);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_PROPERTIES_H_ */
