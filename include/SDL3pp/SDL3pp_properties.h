/**
 * @file properties.hpp
 *
 * # CategoryProperties
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
 */
#ifndef SDL3PP_PROPERTIES_H_
#define SDL3PP_PROPERTIES_H_

#include <SDL3/SDL_properties.h>
#include "SDL3pp_callbackWrapper.h"
#include "SDL3pp_error.h"
#include "SDL3pp_objectWrapper.h"
#include "SDL3pp_stringParam.h"

namespace SDL {

// Forward decl
template<class T>
struct PropertiesBase;

/**
 * @brief Handle to a non owned properties
 */
using PropertiesRef =
  PropertiesBase<ObjectRef<SDL_PropertiesID, FancyPointer<SDL_PropertiesID>>>;

struct PropertiesDeleter
{
  using pointer = FancyPointer<SDL_PropertiesID>;
  inline void operator()(pointer props) const;
};

/**
 * @brief Handle to an owned surface
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
 * @brief Wrap properties id
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
 */
template<class T>
struct PropertiesBase : T
{
  using T::T;

  /**
   * Create a group of properties.
   *
   * All properties are automatically destroyed when SDL_Quit() is called.
   *
   * @returns an ID for a new group of properties, or 0 on failure; call
   *          SDL_GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_DestroyProperties
   */
  PropertiesBase()
    : T(FancyPointer{SDL_CreateProperties()})
  {
  }

  /**
   * @brief Copy a group of properties.
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
   */
  bool CopyPropertiesTo(PropertiesRef dst) const
  {
    return SDL_CopyProperties(T::get(), dst.get());
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
   * @sa SetPointerWithCleanup
   */
  using CleanupCallback = SDL_CleanupPropertyCallback;

  /**
   * @sa PropertiesRef.CleanupCallback
   */
  using CleanupFunction = std::function<void(void*)>;

  // TODO SDL_LockProperties & SDL_UnlockProperties

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
  inline bool SetPointerWithCleanup(StringParam name,
                                    void* value,
                                    CleanupCallback cleanup,
                                    void* userdata)
  {
    return SDL_SetPointerPropertyWithCleanup(
      T::get(), name, value, cleanup, userdata);
  }

  /**
   * @brief Set a pointer property in a group of properties with a cleanup
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
   */
  bool SetPointerWithCleanup(StringParam name,
                             void* value,
                             CleanupFunction cleanup)
  {
    using Wrapper = CallbackWrapper<void(void* value)>;

    return SetPointerWithCleanup(std::move(name),
                                 value,
                                 &Wrapper::CallOnce,
                                 Wrapper::Wrap(std::move(cleanup)));
  }

  /**
   * @brief Set a pointer property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or NULL to delete the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool SetPointer(StringParam name, void* value)
  {
    return SDL_SetPointerProperty(T::get(), name, value);
  }

  /**
   * @brief Set a string property in a group of properties.
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
   */
  bool SetString(StringParam name, StringParam value)
  {
    return SDL_SetStringProperty(T::get(), name, value);
  }

  /**
   * @brief Set an integer property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool SetNumber(StringParam name, Sint64 value)
  {
    return SDL_SetNumberProperty(T::get(), name, value);
  }

  /**
   * @brief Set an floating point property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool SetFloat(StringParam name, float value)
  {
    return SDL_SetFloatProperty(T::get(), name, value);
  }

  /**
   * @brief Set an boolean property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool SetBoolean(StringParam name, bool value)
  {
    return SDL_SetBooleanProperty(T::get(), name, value);
  }

  /// @brief Return whether a property exists
  bool Has(StringParam name) const { return SDL_HasProperty(T::get(), name); }

  /// @brief Get the type of a property in a group of properties.
  PropertyType GetType(StringParam name) const
  {
    return SDL_GetPropertyType(T::get(), name);
  }

  /**
   * @brief Get a pointer property from a group of properties.
   *
   * You can use SDL_GetPropertyType() to query whether the property exists and
   * is a pointer property.
   *
   * By convention, the names of properties that SDL exposes on objects will
   * start with "SDL.", and properties that SDL uses internally will start with
   * "SDL.internal.". These should be considered read-only and should not be
   * modified by applications.
   *
   * @param the name of the property to query.
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
   * @brief Get a number property from a group of properties.
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
   */
  Sint64 GetNumber(StringParam name, Sint64 default_value) const
  {
    return SDL_GetNumberProperty(T::get(), name, default_value);
  }

  /**
   * @brief Get a floating point property from a group of properties.
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
   */
  float GetFloat(StringParam name, float default_value) const
  {
    return SDL_GetFloatProperty(T::get(), name, default_value);
  }

  /**
   * @brief Get a boolean property from a group of properties.
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
   */
  bool GetBoolean(StringParam name, bool default_value) const
  {
    return SDL_GetBooleanProperty(T::get(), name, default_value);
  }

  /**
   * @brief Clear a property from a group of properties.
   *
   * @param name the name of the property to clear.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool Clear(StringParam name) { return SDL_ClearProperty(T::get(), name); }

  /**
   * A callback used to enumerate all the properties in a group of properties.
   *
   * This callback is called from SDL_EnumerateProperties(), and is called once
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
   * @sa SDL_EnumerateProperties
   */
  using EnumerateCallback = SDL_EnumeratePropertiesCallback;

  /**
   * @sa PropertiesRef.EnumerateCallback
   */
  using EnumerateFunction = std::function<void(SDL_PropertiesID, const char*)>;

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
   */
  bool Enumerate(EnumerateCallback callback, void* userdata) const
  {
    return SDL_EnumerateProperties(T::get(), callback, userdata);
  }

  /**
   * @brief Enumerate the properties contained in a group of properties.
   *
   * The callback function is called for each property in the group of
   * properties. The properties are locked during enumeration.
   *
   * @param callback the function to call for each property.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool Enumerate(EnumerateFunction callback) const
  {
    using Wrapper =
      CallbackWrapper<void(SDL_PropertiesID props, const char* name)>;
    void* cbHandle = Wrapper::Wrap(std::move(callback));
    return Enumerate(&Wrapper::CallOnce, cbHandle);
  }

  /**
   * @brief Enumerate the properties contained in a group of properties.
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
 * @sa SDL_CreateProperties
 */
template<ObjectBox<FancyPointer<SDL_PropertiesID>> T>
inline void DestroyProperties(T&& props)
{
  SDL_DestroyProperties(props.release());
}

#pragma region impl

inline void PropertiesDeleter::operator()(
  PropertiesDeleter::pointer props) const
{
  DestroyProperties(PropertiesRef(props));
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_PROPERTIES_H_ */
