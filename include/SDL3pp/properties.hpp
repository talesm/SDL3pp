#ifndef SDL3PP_PROPERTIES_HPP_
#define SDL3PP_PROPERTIES_HPP_

#include <SDL3/SDL_properties.h>
#include "callbackWrapper.hpp"
#include "stringParam.hpp"

namespace SDL {

/**
 * @brief SDL properties ID
 *
 */
using PropertiesID = SDL_PropertiesID;

/**
 * @brief SDL property type
 *
 */
using PropertyType = SDL_PropertyType;

/**
 * @brief Wrap properties id
 *
 * A property is a variable that can be created and retrieved by name at
 * runtime.
 *
 * All properties are part of a property group (PropertiesID). A property
 * group can be created with the Properties constructor and destroyed
 * with this goes out of scope.
 *
 * Properties can be added to and retrieved from a property group through the
 * following functions:
 *
 * - SDL_SetPointerProperty and SDL_GetPointerProperty operate on `void*`
 *   pointer types.
 * - SDL_SetStringProperty and SDL_GetStringProperty operate on string types.
 * - SDL_SetNumberProperty and SDL_GetNumberProperty operate on signed 64-bit
 *   integer types.
 * - SDL_SetFloatProperty and SDL_GetFloatProperty operate on floating point
 *   types.
 * - SDL_SetBooleanProperty and SDL_GetBooleanProperty operate on boolean
 *   types.
 *
 * Properties can be removed from a group by using SDL_ClearProperty.
 *
 */
class PropertiesRef
{
  PropertiesID id;

public:
  constexpr PropertiesRef(PropertiesID id = 0)
    : id(id)
  {
  }
  /// @brief Returns if valid (id != 0)
  constexpr bool Valid() const { return id != 0; }

  /// @brief Returns the Id
  constexpr PropertiesID Get() const { return id; }

  /// @brief Returns if valid (id != 0)
  constexpr operator bool() const { return Valid(); }

  /**
   * @brief Resets the status
   *
   * This **does not** Destroy the property group
   *
   * @param id Optional: The new value for id.
   */
  void Reset(PropertiesID id = 0) { id = 0; }

  /**
   * @brief Copy a group of properties.
   *
   * Copy all the properties from one group of properties to another, with the
   * exception of properties requiring cleanup (set using
   * SDL_SetPointerPropertyWithCleanup()), which will not be copied. Any
   * property that already exists on `dst` will be overwritten.
   *
   * @param dst the destination properties.
   * @return true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool CopyPropertiesTo(PropertiesRef dst) const
  {
    return SDL_CopyProperties(Get(), dst.Get());
  }

  // TODO SDL_LockProperties & SDL_UnlockProperties

  /**
   * @brief Set a pointer property in a group of properties with a cleanup
   * function that is called when the property is deleted.
   *
   * The cleanup function is also called if setting the property fails for any
   * reason.
   *
   * For simply setting basic data types, like numbers, bools, or strings, use
   * SDL_SetNumberProperty, SDL_SetBooleanProperty, or SDL_SetStringProperty
   * instead, as those functions will handle cleanup on your behalf. This
   * function is only for more complex, custom data.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or NULL to delete the property.
   * @param cleanup the function to call when this property is deleted, or NULL
   *                if no cleanup is necessary.
   * @param userData a pointer that is passed to the cleanup function.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool SetPointerWithCleanup(StringParam name,
                             void* value,
                             SDL_CleanupPropertyCallback cleanup,
                             void* userData)
  {
    return SDL_SetPointerPropertyWithCleanup(
      Get(), name, value, cleanup, userData);
  }

  /**
   * @brief A callback used to free resources when a property is deleted.
   *
   * This should release any resources associated with `value` that are no
   * longer needed.
   *
   * This callback is set per-property. Different properties in the same group
   * can have different cleanup callbacks.
   *
   * This callback will be called _during_ SDL_SetPointerPropertyWithCleanup if
   * the function fails for any reason.
   *
   * @param value the pointer assigned to the property to clean up.
   *
   * @threadsafety This callback may fire without any locks held; if this is a
   *               concern, the app should provide its own locking.
   */
  using CleanupPropertyCallback = void(void* value);

  /**
   * @brief Set a pointer property in a group of properties with a cleanup
   * function that is called when the property is deleted.
   *
   * The cleanup function is also called if setting the property fails for any
   * reason.
   *
   * For simply setting basic data types, like numbers, bools, or strings, use
   * SDL_SetNumberProperty, SDL_SetBooleanProperty, or SDL_SetStringProperty
   * instead, as those functions will handle cleanup on your behalf. This
   * function is only for more complex, custom data.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or NULL to delete the property.
   * @param cleanup the function to call when this property is deleted.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool SetPointerWithCleanup(StringParam name,
                             void* value,
                             std::function<CleanupPropertyCallback> cleanup)
  {
    using Wrapper = CallbackWrapper<CleanupPropertyCallback>;

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
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool SetPointer(StringParam name, void* value)
  {
    return SDL_SetPointerProperty(Get(), name, value);
  }

  /**
   * @brief Set a string property in a group of properties.
   *
   * This function makes a copy of the string; the caller does not have to
   * preserve the data after this call completes.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or NULL to delete the property.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool SetString(StringParam name, StringParam value)
  {
    return SDL_SetStringProperty(Get(), name, value);
  }

  /**
   * @brief Set an integer property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool SetNumber(StringParam name, Sint64 value)
  {
    return SDL_SetNumberProperty(Get(), name, value);
  }

  /**
   * @brief Set an floating point property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool SetFloat(StringParam name, float value)
  {
    return SDL_SetFloatProperty(Get(), name, value);
  }

  /**
   * @brief Set an boolean property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool SetBoolean(StringParam name, bool value)
  {
    return SDL_SetBooleanProperty(Get(), name, value);
  }

  /// @brief Return whether a property exists
  bool Has(StringParam name) const { return SDL_HasProperty(Get(), name); }

  /// @brief Get the type of a property in a group of properties.
  PropertyType GetType(StringParam name) const
  {
    return SDL_GetPropertyType(Get(), name);
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
   * @param defaultValue the default value of the property.
   * @returns the value of the property, or `defaultValue` if it is not set or
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
  void* GetPointer(StringParam name, void* defaultValue = nullptr) const
  {
    return SDL_GetPointerProperty(Get(), name, defaultValue);
  }

  /**
   * @brief Get a string property from a group of properties.
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
   * @param defaultValue the default value of the property.
   * @returns the value of the property, or `defaultValue` if it is not set or
   *          not a string property.
   *
   * @threadsafety It is safe to call this function from any thread, although
   *               the data returned is not protected and could potentially be
   *               freed if you call SetString() or
   *               ClearProperty() on these properties from another thread.
   *               If you need to avoid this, use SDL_LockProperties() and
   *               SDL_UnlockProperties().
   *
   * @sa GetPointer()
   * @sa SetString()
   */
  const char* GetString(StringParam name,
                        const char* defaultValue = nullptr) const
  {
    return SDL_GetStringProperty(Get(), name, defaultValue);
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
   * @param defaultValue the default value of the property.
   * @returns the value of the property, or `defaultValue` if it is not set or
   *          not a number property.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  Sint64 GetNumber(StringParam name, Sint64 defaultValue) const
  {
    return SDL_GetNumberProperty(Get(), name, defaultValue);
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
   * @param defaultValue the default value of the property.
   * @returns the value of the property, or `defaultValue` if it is not set or
   *          not a float property.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  float GetFloat(StringParam name, float defaultValue) const
  {
    return SDL_GetFloatProperty(Get(), name, defaultValue);
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
   * @param defaultValue the default value of the property.
   * @returns the value of the property, or `defaultValue` if it is not set or
   *          not a boolean property.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool GetBoolean(StringParam name, bool defaultValue) const
  {
    return SDL_GetBooleanProperty(Get(), name, defaultValue);
  }

  /**
   * @brief Clear a property from a group of properties.
   *
   * @param name the name of the property to clear.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool Clear(StringParam name) { return SDL_ClearProperty(Get(), name); }

  /**
   * @brief Enumerate the properties contained in a group of properties.
   *
   * The callback function is called for each property in the group of
   * properties. The properties are locked during enumeration.
   *
   * @param callback the function to call for each property.
   * @param userdata a pointer that is passed to `callback`.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool EnumerateProperties(SDL_EnumeratePropertiesCallback callback,
                           void* userdata) const
  {
    return SDL_EnumerateProperties(Get(), callback, userdata);
  }

  /**
   * @brief A callback used to enumerate all the properties in a group of
   * properties.
   *
   * This callback is called from EnumerateProperties(), and is called once
   * per property in the set.
   *
   * @param props the SDL_PropertiesID that is being enumerated.
   * @param name the next property name in the enumeration.
   *
   * @threadsafety SDL_EnumerateProperties holds a lock on `props` during this
   *               callback.
   *
   * @sa SDL_EnumerateProperties(EnumeratePropertiesCallback)
   */
  using EnumeratePropertiesCallback = void(PropertiesID props,
                                           const char* name);

  /**
   * @brief Enumerate the properties contained in a group of properties.
   *
   * The callback function is called for each property in the group of
   * properties. The properties are locked during enumeration.
   *
   * @param callback the function to call for each property.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  bool EnumerateProperties(EnumeratePropertiesCallback callback) const
  {
    using Wrapper = CallbackWrapper<EnumeratePropertiesCallback>;
    void* cbHandle = Wrapper::Wrap(std::move(callback));
    bool r = false;
    try {
      r = EnumerateProperties(&Wrapper::Call, cbHandle);
    } catch (...) {
      Wrapper::Erase(cbHandle);
      throw;
    }
    Wrapper::Erase(cbHandle);
    return r;
  }

  /**
   * @brief Enumerate the properties contained in a group of properties.
   *
   * @param outputIter an output iterator to be assigned to each property name
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   */
  template<class IT>
  bool EnumerateProperties(IT outputIter) const
  {
    return EnumerateProperties(
      [&outputIter](auto props, const char name) { *outputIter++ = name; });
  }
};

/**
 * @brief Get the global SDL properties.
 *
 * @returns a valid property ID on success or 0 on failure; call
 *          SDL_GetError() for more information.
 */
inline PropertiesRef SDL_GetGlobalProperties()
{
  return {SDL_GetGlobalProperties()};
}

/**
 * @brief A owning Property group
 *
 * Besides its automatic ownership semantics, it behaves identically to
 * PropertiesRef.
 *
 */
struct Properties : PropertiesRef
{
  /**
   * @brief Create a group of properties.
   *
   * This is automatically destroyed when it goes out of scope
   * or when Reset() is called, unless Release() is called first.
   * If Release() is called, then the user might manage it with the C API.
   *
   * If an error occurs the new object Valid() == false and the error
   * can be retrieved with SDL_GetError().
   */
  Properties()
    : Properties(SDL_CreateProperties())
  {
  }

  constexpr explicit Properties(SDL_PropertiesID id)
    : PropertiesRef(id)
  {
  }

  Properties(const Properties&) = delete;
  Properties(Properties&& other)
    : Properties(other.Release())
  {
  }
  Properties& operator=(const Properties&) = delete;
  Properties& operator=(Properties&& other)
  {
    Reset(other.Release());
    return *this;
  }

  /**
   * @brief Destroy a group of properties.
   *
   * All properties are deleted and their cleanup functions will be called, if
   * any.
   *
   * @threadsafety This function should not be called while these properties are
   *               locked or other threads might be setting or getting values
   *               from these properties.
   */
  ~Properties() { Destroy(); }

  /**
   * @brief Releases the property id ownership.
   *
   * After this call the id becomes 0.
   *
   * @return PropertiesID the previous id.
   */
  PropertiesID Release()
  {
    auto id = Get();
    PropertiesRef::Reset();
    return id;
  }

  /**
   * @brief Resets the status, destroying the property group if needed.
   *
   * @param id Optional: The new value for id.
   */
  void Reset(PropertiesID id = 0)
  {
    Destroy();
    PropertiesRef::Reset(id);
  }

private:
  void Destroy() { SDL_DestroyProperties(Get()); }
};

} // namespace SDL

#endif /* SDL3PP_PROPERTIES_HPP_ */
