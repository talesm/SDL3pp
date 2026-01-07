#ifndef SDL3PP_PROPERTIES_H_
#define SDL3PP_PROPERTIES_H_

#include <SDL3/SDL_properties.h>
#include "SDL3pp_callbackWrapper.h"
#include "SDL3pp_error.h"
#include "SDL3pp_strings.h"
#include "SDL3pp_version.h"

namespace SDL {

/**
 * @defgroup CategoryProperties Object Properties
 *
 * A property is a variable that can be created and retrieved by name at
 * runtime.
 *
 * All properties are part of a property group (Properties). A property group
 * can be created with the Properties.Create function and destroyed with the
 * Properties.Destroy function, but the Properties destructor probably will do
 * what you want to, automatically.
 *
 * Properties can be added to and retrieved from a property group through the
 * following functions:
 *
 * - Properties.SetPointerProperty and Properties.GetPointerProperty operate on
 *   `void*` pointer types.
 * - Properties.SetStringProperty and Properties.GetStringProperty operate on
 *   string types.
 * - Properties.SetNumberProperty and Properties.GetNumberProperty operate on
 *   signed 64-bit integer types.
 * - Properties.SetFloatProperty and Properties.GetFloatProperty operate on
 *   floating point types.
 * - Properties.SetBooleanProperty and Properties.GetBooleanProperty operate on
 *   boolean types.
 *
 * Properties can be removed from a group by using Properties.ClearProperty.
 *
 * @{
 */

// Forward decl
struct Properties;

/// Alias to raw representation for Properties.
using PropertiesID = SDL_PropertiesID;

// Forward decl
struct PropertiesRef;

/// Safely wrap Properties for non owning parameters
struct PropertiesParam
{
  PropertiesID value; ///< parameter's PropertiesID

  /// Constructs from PropertiesID
  constexpr PropertiesParam(PropertiesID value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr PropertiesParam(std::nullptr_t _ = nullptr)
    : value(0)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const PropertiesParam& other) const = default;

  /// Converts to underlying PropertiesID
  constexpr operator PropertiesID() const { return value; }
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
 * A callback used to enumerate all the properties in a group of properties.
 *
 * This callback is called from Properties.Enumerate(), and is called once per
 * property in the set.
 *
 * @param userdata an app-defined pointer passed to the callback.
 * @param props the Properties that is being enumerated.
 * @param name the next property name in the enumeration.
 *
 * @threadsafety Properties.Enumerate holds a lock on `props` during this
 *               callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa Properties.Enumerate
 */
using EnumeratePropertiesCallback = void(SDLCALL*)(void* userdata,
                                                   PropertiesID props,
                                                   const char* name);

/**
 * A callback used to enumerate all the properties in a group of properties.
 *
 * This callback is called from Properties.Enumerate(), and is called once per
 * property in the set.
 *
 * @param props the Properties that is being enumerated.
 * @param name the next property name in the enumeration.
 *
 * @threadsafety Properties.Enumerate holds a lock on `props` during this
 *               callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa Properties.Enumerate
 */
using EnumeratePropertiesCB =
  std::function<void(PropertiesID props, const char* name)>;

/**
 * A callback used to free resources when a property is deleted.
 *
 * This should release any resources associated with `value` that are no longer
 * needed.
 *
 * This callback is set per-property. Different properties in the same group can
 * have different cleanup callbacks.
 *
 * This callback will be called _during_
 * Properties.SetPointerPropertyWithCleanup if the function fails for any
 * reason.
 *
 * @param userdata an app-defined pointer passed to the callback.
 * @param value the pointer assigned to the property to clean up.
 *
 * @threadsafety This callback may fire without any locks held; if this is a
 *               concern, the app should provide its own locking.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa Properties.SetPointerPropertyWithCleanup
 */
using CleanupPropertyCallback = void(SDLCALL*)(void* userdata, void* value);

/**
 * A callback used to free resources when a property is deleted.
 *
 * This should release any resources associated with `value` that are no longer
 * needed.
 *
 * This callback is set per-property. Different properties in the same group can
 * have different cleanup callbacks.
 *
 * This callback will be called _during_
 * Properties.SetPointerPropertyWithCleanup if the function fails for any
 * reason.
 *
 * @param value the pointer assigned to the property to clean up.
 *
 * @threadsafety This callback may fire without any locks held; if this is a
 *               concern, the app should provide its own locking.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa Properties.SetPointerPropertyWithCleanup
 *
 * @sa CleanupPropertyCallback
 */
using CleanupPropertyCB = std::function<void(void* value)>;

/**
 * An ID that represents a properties set.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa Properties.Create
 */
class Properties
{
  PropertiesID m_resource = 0;

public:
  /// Default ctor
  constexpr Properties(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from PropertiesParam.
   *
   * @param resource a PropertiesID to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Properties(const PropertiesID resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr Properties(const Properties& other) noexcept = default;

public:
  /// Move constructor
  constexpr Properties(Properties&& other) noexcept
    : Properties(other.release())
  {
  }

  constexpr Properties(const PropertiesRef& other) = delete;

  constexpr Properties(PropertiesRef&& other) = delete;

  /**
   * Create a group of properties.
   *
   * All properties are automatically destroyed when Quit() is called.
   *
   * @returns a valid ID for a new group of properties on success;
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.Destroy
   */
  static Properties Create();

  /// Destructor
  ~Properties() { SDL_DestroyProperties(m_resource); }

  /// Assignment operator.
  constexpr Properties& operator=(Properties&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Properties& operator=(const Properties& other) noexcept = default;

public:
  /// Retrieves underlying PropertiesID.
  constexpr PropertiesID get() const noexcept { return m_resource; }

  /// Retrieves underlying PropertiesID and clear this.
  constexpr PropertiesID release() noexcept
  {
    auto r = m_resource;
    m_resource = 0;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Properties& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to PropertiesParam
  constexpr operator PropertiesParam() const noexcept { return {m_resource}; }

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
   * @sa Properties.Create
   */
  void Destroy();

  /**
   * Copy a group of properties.
   *
   * Copy all the properties from one group of properties to another, with the
   * exception of properties requiring cleanup (set using
   * Properties.SetPointerPropertyWithCleanup()), which will not be copied. Any
   * property that already exists on `dst` will be overwritten.
   *
   * @param dst the destination properties.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread. This
   *               function acquires simultaneous mutex locks on both the source
   *               and destination property sets.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Copy(PropertiesParam dst);

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
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.Unlock
   */
  void Lock();

  /**
   * Unlock a group of properties.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.Lock
   */
  void Unlock();

  /**
   * Set a pointer property in a group of properties with a cleanup function
   * that is called when the property is deleted.
   *
   * The cleanup function is also called if setting the property fails for any
   * reason.
   *
   * For simply setting basic data types, like numbers, bools, or strings, use
   * Properties.SetNumberProperty, Properties.SetBooleanProperty, or
   * Properties.SetStringProperty instead, as those functions will handle
   * cleanup on your behalf. This function is only for more complex, custom
   * data.
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
   * @sa Properties.GetPointerProperty
   * @sa Properties.SetPointerProperty
   * @sa CleanupPropertyCallback
   */
  void SetPointerPropertyWithCleanup(StringParam name,
                                     void* value,
                                     CleanupPropertyCallback cleanup,
                                     void* userdata);

  /**
   * Set a pointer property in a group of properties with a cleanup function
   * that is called when the property is deleted.
   *
   * The cleanup function is also called if setting the property fails for any
   * reason.
   *
   * For simply setting basic data types, like numbers, bools, or strings, use
   * Properties.SetNumberProperty, Properties.SetBooleanProperty, or
   * Properties.SetStringProperty instead, as those functions will handle
   * cleanup on your behalf. This function is only for more complex, custom
   * data.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or nullptr to delete the
   *              property.
   * @param cleanup the function to call when this property is deleted, or
   *                nullptr if no cleanup is necessary.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.GetPointerProperty
   * @sa Properties.SetPointerProperty
   * @sa CleanupPropertyCallback
   */
  void SetPointerPropertyWithCleanup(StringParam name,
                                     void* value,
                                     CleanupPropertyCB cleanup);

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
   * @sa Properties.GetPointerProperty
   * @sa Properties.HasProperty
   * @sa Properties.SetBooleanProperty
   * @sa Properties.SetFloatProperty
   * @sa Properties.SetNumberProperty
   * @sa Properties.SetPointerPropertyWithCleanup
   * @sa Properties.SetStringProperty
   */
  void SetPointerProperty(StringParam name, void* value);

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
   * @sa Properties.GetStringProperty
   */
  void SetStringProperty(StringParam name, StringParam value);

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
   * @sa Properties.GetNumberProperty
   */
  void SetNumberProperty(StringParam name, Sint64 value);

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
   * @sa Properties.GetFloatProperty
   */
  void SetFloatProperty(StringParam name, float value);

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
   * @sa Properties.GetBooleanProperty
   */
  void SetBooleanProperty(StringParam name, bool value);

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
   * @sa Properties.GetPropertyType
   */
  bool HasProperty(StringParam name);

  /**
   * Get the type of a property in a group of properties.
   *
   * @param name the name of the property to query.
   * @returns the type of the property, or PROPERTY_TYPE_INVALID if it is not
   *          set.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.HasProperty
   */
  PropertyType GetPropertyType(StringParam name);

  /**
   * Get a pointer property from a group of properties.
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
   *               freed if you call Properties.SetPointerProperty() or
   *               Properties.ClearProperty() on these properties from another
   *               thread. If you need to avoid this, use Properties.Lock() and
   *               Properties.Unlock().
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.GetBooleanProperty
   * @sa Properties.GetFloatProperty
   * @sa Properties.GetNumberProperty
   * @sa Properties.GetPropertyType
   * @sa Properties.GetStringProperty
   * @sa Properties.HasProperty
   * @sa Properties.SetPointerProperty
   */
  void* GetPointerProperty(StringParam name, void* default_value);

  /**
   * Get a string property from a group of properties.
   *
   * @param name the name of the property to query.
   * @param default_value the default value of the property.
   * @returns the value of the property, or `default_value` if it is not set or
   *          not a string property.
   *
   * @threadsafety It is safe to call this function from any thread, although
   *               the data returned is not protected and could potentially be
   *               freed if you call Properties.SetStringProperty() or
   *               Properties.ClearProperty() on these properties from another
   *               thread. If you need to avoid this, use Properties.Lock() and
   *               Properties.Unlock().
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Properties.GetPropertyType
   * @sa Properties.HasProperty
   * @sa Properties.SetStringProperty
   */
  const char* GetStringProperty(StringParam name, StringParam default_value);

  /**
   * Get a number property from a group of properties.
   *
   * You can use Properties.GetPropertyType() to query whether the property
   * exists and is a number property.
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
   * @sa Properties.GetPropertyType
   * @sa Properties.HasProperty
   * @sa Properties.SetNumberProperty
   */
  Sint64 GetNumberProperty(StringParam name, Sint64 default_value);

  /**
   * Get a floating point property from a group of properties.
   *
   * You can use Properties.GetPropertyType() to query whether the property
   * exists and is a floating point property.
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
   * @sa Properties.GetPropertyType
   * @sa Properties.HasProperty
   * @sa Properties.SetFloatProperty
   */
  float GetFloatProperty(StringParam name, float default_value);

  /**
   * Get a boolean property from a group of properties.
   *
   * You can use Properties.GetPropertyType() to query whether the property
   * exists and is a boolean property.
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
   * @sa Properties.GetPropertyType
   * @sa Properties.HasProperty
   * @sa Properties.SetBooleanProperty
   */
  bool GetBooleanProperty(StringParam name, bool default_value);

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
  void ClearProperty(StringParam name);

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
  void Enumerate(EnumeratePropertiesCallback callback, void* userdata);

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
   * @since This function is available since SDL 3.2.0.
   */
  void Enumerate(EnumeratePropertiesCB callback);

  /**
   * Returns the number of properties this has
   *
   * This uses EnumerateProperties() internally, so might not be so fast
   *
   * @return Uint64
   */
  Uint64 GetCount();
};

/// Semi-safe reference for Properties.
struct PropertiesRef : Properties
{
  using Properties::Properties;

  /**
   * Constructs from PropertiesParam.
   *
   * @param resource a PropertiesID or Properties.
   *
   * This does not takes ownership!
   */
  PropertiesRef(PropertiesParam resource) noexcept
    : Properties(resource.value)
  {
  }

  /**
   * Constructs from PropertiesParam.
   *
   * @param resource a PropertiesID or Properties.
   *
   * This does not takes ownership!
   */
  PropertiesRef(PropertiesID resource) noexcept
    : Properties(resource)
  {
  }

  /// Copy constructor.
  PropertiesRef(const PropertiesRef& other) noexcept
    : Properties(other.get())
  {
  }

  /// Destructor
  ~PropertiesRef() { release(); }
};

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * A generic property for naming things.
 *
 * This property is intended to be added to any Properties that needs a generic
 * name associated with the property set. It is not guaranteed that any property
 * set will include this key, but it is convenient to have a standard key that
 * any piece of code could reasonably agree to use.
 *
 * For example, the properties associated with an Texture might have a name
 * string of "player sprites", or an AudioStream might have "background music",
 * etc. This might also be useful for an IOStream to list the path to its asset.
 *
 * There is no format for the value set with this key; it is expected to be
 * human-readable and informational in nature, possibly for logging or debugging
 * purposes.
 *
 * SDL does not currently set this property on any objects it creates, but this
 * may change in later versions; it is currently expected that apps and external
 * libraries will take advantage of it, when appropriate.
 *
 * @since This constant is available since SDL 3.4.0.
 */
inline auto PROP_NAME_STRING = SDL_PROP_NAME_STRING;

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Get the global SDL properties.
 *
 * @returns a valid property ID on success.
 *
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesRef GetGlobalProperties()
{
  return {CheckError(SDL_GetGlobalProperties())};
}

/**
 * Create a group of properties.
 *
 * All properties are automatically destroyed when Quit() is called.
 *
 * @returns a valid ID for a new group of properties on success;
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.Destroy
 */
inline Properties CreateProperties()
{
  return Properties(CheckError(SDL_CreateProperties()));
}

inline Properties Properties::Create() { return SDL::CreateProperties(); }

/**
 * Copy a group of properties.
 *
 * Copy all the properties from one group of properties to another, with the
 * exception of properties requiring cleanup (set using
 * Properties.SetPointerPropertyWithCleanup()), which will not be copied. Any
 * property that already exists on `dst` will be overwritten.
 *
 * @param src the properties to copy.
 * @param dst the destination properties.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread. This function
 *               acquires simultaneous mutex locks on both the source and
 *               destination property sets.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void CopyProperties(PropertiesParam src, PropertiesParam dst)
{
  CheckError(SDL_CopyProperties(src, dst));
}

inline void Properties::Copy(PropertiesParam dst)
{
  SDL::CopyProperties(m_resource, dst);
}

/**
 * Lock a group of properties.
 *
 * Obtain a multi-threaded lock for these properties. Other threads will wait
 * while trying to lock these properties until they are unlocked. Properties
 * must be unlocked before they are destroyed.
 *
 * The lock is automatically taken when setting individual properties, this
 * function is only needed when you want to set several properties atomically or
 * want to guarantee that properties being queried aren't freed in another
 * thread.
 *
 * @param props the properties to lock.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.Unlock
 */
inline void LockProperties(PropertiesParam props)
{
  CheckError(SDL_LockProperties(props));
}

inline void Properties::Lock() { SDL::LockProperties(m_resource); }

/**
 * Unlock a group of properties.
 *
 * @param props the properties to unlock.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.Lock
 */
inline void UnlockProperties(PropertiesParam props)
{
  SDL_UnlockProperties(props);
}

inline void Properties::Unlock() { SDL::UnlockProperties(m_resource); }

/**
 * Set a pointer property in a group of properties with a cleanup function that
 * is called when the property is deleted.
 *
 * The cleanup function is also called if setting the property fails for any
 * reason.
 *
 * For simply setting basic data types, like numbers, bools, or strings, use
 * Properties.SetNumberProperty, Properties.SetBooleanProperty, or
 * Properties.SetStringProperty instead, as those functions will handle cleanup
 * on your behalf. This function is only for more complex, custom data.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property, or nullptr to delete the
 *              property.
 * @param cleanup the function to call when this property is deleted, or nullptr
 *                if no cleanup is necessary.
 * @param userdata a pointer that is passed to the cleanup function.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.GetPointerProperty
 * @sa Properties.SetPointerProperty
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
 * Set a pointer property in a group of properties with a cleanup function that
 * is called when the property is deleted.
 *
 * The cleanup function is also called if setting the property fails for any
 * reason.
 *
 * For simply setting basic data types, like numbers, bools, or strings, use
 * Properties.SetNumberProperty, Properties.SetBooleanProperty, or
 * Properties.SetStringProperty instead, as those functions will handle cleanup
 * on your behalf. This function is only for more complex, custom data.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property, or nullptr to delete the
 *              property.
 * @param cleanup the function to call when this property is deleted, or nullptr
 *                if no cleanup is necessary.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.GetPointerProperty
 * @sa Properties.SetPointerProperty
 * @sa CleanupPropertyCallback
 */
inline void SetPointerPropertyWithCleanup(PropertiesParam props,
                                          StringParam name,
                                          void* value,
                                          CleanupPropertyCB cleanup)
{
  using Wrapper = CallbackWrapper<CleanupPropertyCB>;
  SDL_SetPointerPropertyWithCleanup(props,
                                    std::move(name),
                                    value,
                                    &Wrapper::CallOnce,
                                    Wrapper::Wrap(std::move(cleanup)));
}

inline void Properties::SetPointerPropertyWithCleanup(
  StringParam name,
  void* value,
  CleanupPropertyCallback cleanup,
  void* userdata)
{
  SDL::SetPointerPropertyWithCleanup(
    m_resource, std::move(name), value, cleanup, userdata);
}

inline void Properties::SetPointerPropertyWithCleanup(StringParam name,
                                                      void* value,
                                                      CleanupPropertyCB cleanup)
{
  SDL::SetPointerPropertyWithCleanup(
    m_resource, std::move(name), value, cleanup);
}

/**
 * Set a pointer property in a group of properties.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property, or nullptr to delete the
 *              property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.GetPointerProperty
 * @sa Properties.HasProperty
 * @sa Properties.SetBooleanProperty
 * @sa Properties.SetFloatProperty
 * @sa Properties.SetNumberProperty
 * @sa Properties.SetPointerPropertyWithCleanup
 * @sa Properties.SetStringProperty
 */
inline void SetPointerProperty(PropertiesParam props,
                               StringParam name,
                               void* value)
{
  CheckError(SDL_SetPointerProperty(props, name, value));
}

inline void Properties::SetPointerProperty(StringParam name, void* value)
{
  SDL::SetPointerProperty(m_resource, std::move(name), value);
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
 *              property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.GetStringProperty
 */
inline void SetStringProperty(PropertiesParam props,
                              StringParam name,
                              StringParam value)
{
  CheckError(SDL_SetStringProperty(props, name, value));
}

inline void Properties::SetStringProperty(StringParam name, StringParam value)
{
  SDL::SetStringProperty(m_resource, std::move(name), std::move(value));
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
 * @sa Properties.GetNumberProperty
 */
inline void SetNumberProperty(PropertiesParam props,
                              StringParam name,
                              Sint64 value)
{
  CheckError(SDL_SetNumberProperty(props, name, value));
}

inline void Properties::SetNumberProperty(StringParam name, Sint64 value)
{
  SDL::SetNumberProperty(m_resource, std::move(name), value);
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
 * @sa Properties.GetFloatProperty
 */
inline void SetFloatProperty(PropertiesParam props,
                             StringParam name,
                             float value)
{
  CheckError(SDL_SetFloatProperty(props, name, value));
}

inline void Properties::SetFloatProperty(StringParam name, float value)
{
  SDL::SetFloatProperty(m_resource, std::move(name), value);
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
 * @sa Properties.GetBooleanProperty
 */
inline void SetBooleanProperty(PropertiesParam props,
                               StringParam name,
                               bool value)
{
  CheckError(SDL_SetBooleanProperty(props, name, value));
}

inline void Properties::SetBooleanProperty(StringParam name, bool value)
{
  SDL::SetBooleanProperty(m_resource, std::move(name), value);
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
 * @sa Properties.GetPropertyType
 */
inline bool HasProperty(PropertiesParam props, StringParam name)
{
  return SDL_HasProperty(props, name);
}

inline bool Properties::HasProperty(StringParam name)
{
  return SDL::HasProperty(m_resource, std::move(name));
}

/**
 * Get the type of a property in a group of properties.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @returns the type of the property, or PROPERTY_TYPE_INVALID if it is not set.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.HasProperty
 */
inline PropertyType GetPropertyType(PropertiesParam props, StringParam name)
{
  return SDL_GetPropertyType(props, name);
}

inline PropertyType Properties::GetPropertyType(StringParam name)
{
  return SDL::GetPropertyType(m_resource, std::move(name));
}

/**
 * Get a pointer property from a group of properties.
 *
 * By convention, the names of properties that SDL exposes on objects will start
 * with "SDL.", and properties that SDL uses internally will start with
 * "SDL.internal.". These should be considered read-only and should not be
 * modified by applications.
 *
 * @param props the properties to query.
 * @param name the name of the property to query.
 * @param default_value the default value of the property.
 * @returns the value of the property, or `default_value` if it is not set or
 *          not a pointer property.
 *
 * @threadsafety It is safe to call this function from any thread, although the
 *               data returned is not protected and could potentially be freed
 *               if you call Properties.SetPointerProperty() or
 *               Properties.ClearProperty() on these properties from another
 *               thread. If you need to avoid this, use Properties.Lock() and
 *               Properties.Unlock().
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.GetBooleanProperty
 * @sa Properties.GetFloatProperty
 * @sa Properties.GetNumberProperty
 * @sa Properties.GetPropertyType
 * @sa Properties.GetStringProperty
 * @sa Properties.HasProperty
 * @sa Properties.SetPointerProperty
 */
inline void* GetPointerProperty(PropertiesParam props,
                                StringParam name,
                                void* default_value)
{
  return SDL_GetPointerProperty(props, name, default_value);
}

inline void* Properties::GetPointerProperty(StringParam name,
                                            void* default_value)
{
  return SDL::GetPointerProperty(m_resource, std::move(name), default_value);
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
 * @threadsafety It is safe to call this function from any thread, although the
 *               data returned is not protected and could potentially be freed
 *               if you call Properties.SetStringProperty() or
 *               Properties.ClearProperty() on these properties from another
 *               thread. If you need to avoid this, use Properties.Lock() and
 *               Properties.Unlock().
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.GetPropertyType
 * @sa Properties.HasProperty
 * @sa Properties.SetStringProperty
 */
inline const char* GetStringProperty(PropertiesParam props,
                                     StringParam name,
                                     StringParam default_value)
{
  return SDL_GetStringProperty(props, name, default_value);
}

inline const char* Properties::GetStringProperty(StringParam name,
                                                 StringParam default_value)
{
  return SDL::GetStringProperty(
    m_resource, std::move(name), std::move(default_value));
}

/**
 * Get a number property from a group of properties.
 *
 * You can use Properties.GetPropertyType() to query whether the property exists
 * and is a number property.
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
 * @sa Properties.GetPropertyType
 * @sa Properties.HasProperty
 * @sa Properties.SetNumberProperty
 */
inline Sint64 GetNumberProperty(PropertiesParam props,
                                StringParam name,
                                Sint64 default_value)
{
  return SDL_GetNumberProperty(props, name, default_value);
}

inline Sint64 Properties::GetNumberProperty(StringParam name,
                                            Sint64 default_value)
{
  return SDL::GetNumberProperty(m_resource, std::move(name), default_value);
}

/**
 * Get a floating point property from a group of properties.
 *
 * You can use Properties.GetPropertyType() to query whether the property exists
 * and is a floating point property.
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
 * @sa Properties.GetPropertyType
 * @sa Properties.HasProperty
 * @sa Properties.SetFloatProperty
 */
inline float GetFloatProperty(PropertiesParam props,
                              StringParam name,
                              float default_value)
{
  return SDL_GetFloatProperty(props, name, default_value);
}

inline float Properties::GetFloatProperty(StringParam name, float default_value)
{
  return SDL::GetFloatProperty(m_resource, std::move(name), default_value);
}

/**
 * Get a boolean property from a group of properties.
 *
 * You can use Properties.GetPropertyType() to query whether the property exists
 * and is a boolean property.
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
 * @sa Properties.GetPropertyType
 * @sa Properties.HasProperty
 * @sa Properties.SetBooleanProperty
 */
inline bool GetBooleanProperty(PropertiesParam props,
                               StringParam name,
                               bool default_value)
{
  return SDL_GetBooleanProperty(props, name, default_value);
}

inline bool Properties::GetBooleanProperty(StringParam name, bool default_value)
{
  return SDL::GetBooleanProperty(m_resource, std::move(name), default_value);
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

inline void Properties::ClearProperty(StringParam name)
{
  SDL::ClearProperty(m_resource, std::move(name));
}

/**
 * Enumerate the properties contained in a group of properties.
 *
 * The callback function is called for each property in the group of properties.
 * The properties are locked during enumeration.
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
 * The callback function is called for each property in the group of properties.
 * The properties are locked during enumeration.
 *
 * @param props the properties to query.
 * @param callback the function to call for each property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void EnumerateProperties(PropertiesParam props,
                                EnumeratePropertiesCB callback)
{
  return EnumerateProperties(
    props,
    [](void* userdata, PropertiesID props, const char* name) {
      auto& f = *static_cast<EnumeratePropertiesCB*>(userdata);
      f(props, name);
    },
    &callback);
}

inline void Properties::Enumerate(EnumeratePropertiesCallback callback,
                                  void* userdata)
{
  SDL::EnumerateProperties(m_resource, callback, userdata);
}

inline void Properties::Enumerate(EnumeratePropertiesCB callback)
{
  SDL::EnumerateProperties(m_resource, callback);
}

/**
 * Returns the number of properties this has
 *
 * This uses EnumerateProperties() internally, so might not be so fast
 *
 * @param props
 * @return Uint64
 */
inline Uint64 CountProperties(PropertiesParam props)
{
  Uint64 count = 0;
  EnumerateProperties(props, [&](auto, const char*) { count++; });
  return count;
}

inline Uint64 Properties::GetCount()
{
  return SDL::CountProperties(m_resource);
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
 *               locked or other threads might be setting or getting values from
 *               these properties.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Properties.Create
 */
inline void DestroyProperties(PropertiesID props)
{
  SDL_DestroyProperties(props);
}

inline void Properties::Destroy() { DestroyProperties(release()); }

/// @}

} // namespace SDL

#endif /* SDL3PP_PROPERTIES_H_ */
