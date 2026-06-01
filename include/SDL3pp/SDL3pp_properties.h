#ifndef SDL3PP_PROPERTIES_H_
#define SDL3PP_PROPERTIES_H_

#include <SDL3/SDL_properties.h>
#include "SDL3pp_callbackWrapper.h"
#include "SDL3pp_error.h"
#include "SDL3pp_resource.h"
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
 * can be created with the CreateProperties function and destroyed with the
 * DestroyProperties function, but the Properties destructor probably will do
 * what you want to, automatically.
 *
 * Properties can be added to and retrieved from a property group through the
 * following functions:
 *
 * - SetPointerProperty and GetPointerProperty operate on `void*` pointer types.
 * - SetStringProperty and GetStringProperty operate on string types.
 * - SetNumberProperty and GetNumberProperty operate on signed 64-bit integer
 *   types.
 * - SetFloatProperty and GetFloatProperty operate on floating point types.
 * - SetBooleanProperty and GetBooleanProperty operate on boolean types.
 *
 * Properties can be removed from a group by using ClearProperty.
 *
 * @{
 */

// Forward decl
struct PropertiesBase;

// Forward decl
struct Properties;

/// Alias to raw representation for Properties.
using PropertiesID = SDL_PropertiesID;

/**
 * Reference for Properties.
 *
 * This does not take ownership!
 */
using PropertiesRef = ResourceRefT<PropertiesBase>;

// Forward decl
struct PropertiesLock;

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
 * This callback is called from EnumerateProperties(), and is called once per
 * property in the set.
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
using EnumeratePropertiesCallback = void(SDLCALL*)(void* userdata,
                                                   PropertiesID props,
                                                   const char* name);

/**
 * A callback used to enumerate all the properties in a group of properties.
 *
 * This callback is called from EnumerateProperties(), and is called once per
 * property in the set.
 *
 * @param props the Properties that is being enumerated.
 * @param name the next property name in the enumeration.
 *
 * @threadsafety EnumerateProperties holds a lock on `props` during this
 *               callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa EnumerateProperties
 *
 * @sa EnumeratePropertiesCallback
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
 * This callback will be called _during_ SetPointerPropertyWithCleanup if the
 * function fails for any reason.
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
 * This callback will be called _during_ SetPointerPropertyWithCleanup if the
 * function fails for any reason.
 *
 * @param value the pointer assigned to the property to clean up.
 *
 * @threadsafety This callback may fire without any locks held; if this is a
 *               concern, the app should provide its own locking.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SetPointerPropertyWithCleanup
 *
 * @sa CleanupPropertyCallback
 */
using CleanupPropertyCB = std::function<void(void* value)>;

/**
 * Base class to Properties.
 *
 * @see Properties
 */
struct PropertiesBase : ResourceBaseT<PropertiesID>
{
  using ResourceBaseT::ResourceBaseT;

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
   * @sa CreateProperties
   */
  void Destroy();

  /**
   * Copy a group of properties.
   *
   * Copy all the properties from one group of properties to another, with the
   * exception of properties requiring cleanup (set using
   * SetPointerPropertyWithCleanup()), which will not be copied. Any property
   * that already exists on `dst` will be overwritten.
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
  void Copy(PropertiesRef dst);

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
   * @sa UnlockProperties
   */
  PropertiesLock Lock();

  /**
   * Unlock a group of properties.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LockProperties
   */
  void Unlock(PropertiesLock&& lock);

  /**
   * Set a property in a group of properties.
   *
   * @param name the name of the property to modify.
   * @param value the new value of the property, or nullptr to delete the
   *              property.
   * @throws Error on failure.
   */
  template<class V>
  void Set(StringParam name, V&& value);

  /**
   * Set a pointer property in a group of properties with a cleanup function
   * that is called when the property is deleted.
   *
   * The cleanup function is also called if setting the property fails for any
   * reason.
   *
   * For simply setting basic data types, like numbers, bools, or strings, use
   * SetNumberProperty, SetBooleanProperty, or SetStringProperty instead, as
   * those functions will handle cleanup on your behalf. This function is only
   * for more complex, custom data.
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
   * @sa GetPointerProperty
   * @sa SetPointerProperty
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
   * SetNumberProperty, SetBooleanProperty, or SetStringProperty instead, as
   * those functions will handle cleanup on your behalf. This function is only
   * for more complex, custom data.
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
   * @sa GetPointerProperty
   * @sa SetPointerProperty
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
   * @sa GetPointerProperty
   * @sa HasProperty
   * @sa SetBooleanProperty
   * @sa SetFloatProperty
   * @sa SetNumberProperty
   * @sa SetPointerPropertyWithCleanup
   * @sa SetStringProperty
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
   * @sa GetStringProperty
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
   * @sa GetNumberProperty
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
   * @sa GetFloatProperty
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
   * @sa GetBooleanProperty
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
   * @sa GetPropertyType
   */
  bool HasProperty(StringParam name) const;

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
   * @sa HasProperty
   */
  PropertyType GetPropertyType(StringParam name) const;

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
   *               freed if you call SetPointerProperty() or ClearProperty() on
   *               these properties from another thread. If you need to avoid
   *               this, use LockProperties() and UnlockProperties().
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
  void* GetPointerProperty(StringParam name,
                           void* default_value = nullptr) const;

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
   *               freed if you call SetStringProperty() or ClearProperty() on
   *               these properties from another thread. If you need to avoid
   *               this, use LockProperties() and UnlockProperties().
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetPropertyType
   * @sa HasProperty
   * @sa SetStringProperty
   */
  const char* GetStringProperty(StringParam name,
                                StringParam default_value = "") const;

  /**
   * Get a number property from a group of properties.
   *
   * You can use GetPropertyType() to query whether the property exists and is a
   * number property.
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
   * @sa GetPropertyType
   * @sa HasProperty
   * @sa SetNumberProperty
   */
  Sint64 GetNumberProperty(StringParam name, Sint64 default_value = 0) const;

  /**
   * Get a floating point property from a group of properties.
   *
   * You can use GetPropertyType() to query whether the property exists and is a
   * floating point property.
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
   * @sa GetPropertyType
   * @sa HasProperty
   * @sa SetFloatProperty
   */
  float GetFloatProperty(StringParam name, float default_value = 0) const;

  /**
   * Get a boolean property from a group of properties.
   *
   * You can use GetPropertyType() to query whether the property exists and is a
   * boolean property.
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
   * @sa GetPropertyType
   * @sa HasProperty
   * @sa SetBooleanProperty
   */
  bool GetBooleanProperty(StringParam name, bool default_value = false) const;

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
  void Enumerate(EnumeratePropertiesCallback callback, void* userdata) const;

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
  void Enumerate(EnumeratePropertiesCB callback) const;

  /**
   * Returns the number of properties this has
   *
   * This uses EnumerateProperties() internally, so might not be so fast
   *
   * @return Uint64
   */
  Uint64 GetCount() const;
};

/**
 * An ID that represents a properties set.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa Properties.Create
 * @sa prop
 */
struct Properties : PropertiesBase
{
  using PropertiesBase::PropertiesBase;

  /**
   * Constructs from raw Properties.
   *
   * @param resource a PropertiesID to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Properties(PropertiesID resource) noexcept
    : PropertiesBase(resource)
  {
  }

  /// Move constructor
  constexpr Properties(Properties&& other) noexcept
    : Properties(other.release())
  {
  }

  /**
   * Create a group of properties.
   *
   * All properties are automatically destroyed when Quit() is called.
   *
   * @post an ID for a new group of properties on success.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa DestroyProperties
   */
  Properties();

  /// Destructor
  ~Properties() { SDL_DestroyProperties(get()); }

  /// Assignment operator.
  constexpr Properties& operator=(Properties&& other) noexcept
  {
    swap(*this, other);
    return *this;
  }
};

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
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa UnlockProperties
 */
class PropertiesLock
{
  PropertiesRef m_lock;

public:
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
   * @param resource the properties to lock.
   * @post true on success or false on failure; call GetError() for more
   *       information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa UnlockProperties
   */
  PropertiesLock(PropertiesRef resource);

  /// Copy constructor
  PropertiesLock(const PropertiesLock& other) = delete;

  /// Move constructor
  PropertiesLock(PropertiesLock&& other) noexcept
    : m_lock(std::move(other.m_lock))
  {
  }

  /**
   * Unlock a group of properties.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LockProperties
   */
  ~PropertiesLock() { reset(); }

  PropertiesLock& operator=(const PropertiesLock& other) = delete;

  /// Assignment operator
  PropertiesLock& operator=(PropertiesLock&& other) noexcept
  {
    std::swap(m_lock, other.m_lock);
    return *this;
  }

  /// True if not locked.
  constexpr operator bool() const { return bool(m_lock); }

  /**
   * Unlock a group of properties.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LockProperties
   */
  void reset();

  /// Get the reference to locked resource.
  PropertiesRef resource() const { return m_lock; }

  /// Releases the lock without unlocking.
  void release() { m_lock.release(); }
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
constexpr const char* PROP_NAME_STRING = SDL_PROP_NAME_STRING;

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Get the global SDL properties.
 *
 * @returns a valid property ID on success.
 *
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
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
 * @returns an ID for a new group of properties on success.
 *
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa DestroyProperties
 */
inline Properties CreateProperties() { return Properties(); }

inline Properties::Properties()
  : Properties(CheckError(SDL_CreateProperties()))
{
}

/**
 * Copy a group of properties.
 *
 * Copy all the properties from one group of properties to another, with the
 * exception of properties requiring cleanup (set using
 * SetPointerPropertyWithCleanup()), which will not be copied. Any property that
 * already exists on `dst` will be overwritten.
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
inline void CopyProperties(PropertiesRef src, PropertiesRef dst)
{
  CheckError(SDL_CopyProperties(src, dst));
}

inline void PropertiesBase::Copy(PropertiesRef dst)
{
  SDL::CopyProperties(get(), dst);
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
 * @sa UnlockProperties
 */
inline void LockProperties(PropertiesRef props)
{
  CheckError(SDL_LockProperties(props));
}

inline PropertiesLock PropertiesBase::Lock() { return {PropertiesRef(*this)}; }

inline PropertiesLock::PropertiesLock(PropertiesRef resource)
  : m_lock(std::move(resource))
{
  LockProperties(m_lock);
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
inline void UnlockProperties(PropertiesRef props)
{
  SDL_UnlockProperties(props);
}

inline void PropertiesBase::Unlock(PropertiesLock&& lock)
{
  SDL_assert_paranoid(lock.resource() == *this);
  std::move(lock).reset();
}

inline void PropertiesLock::reset()
{
  if (!m_lock) return;
  UnlockProperties(m_lock);
  m_lock = {};
}

/**
 * Set a property in a group of properties.
 *
 * @param props the properties to modify.
 * @param name the name of the property to modify.
 * @param value the new value of the property, or nullptr to delete the
 *              property.
 * @throws Error on failure.
 */
template<class V>
inline void SetProperty(PropertiesRef props, StringParam name, V&& value)
{
  if constexpr (std::is_same_v<V, bool>) {
    props.SetBooleanProperty(std::move(name), std::forward<V>(value));
  } else if constexpr (std::is_integral_v<V>) {
    props.SetNumberProperty(std::move(name), std::forward<V>(value));
  } else {
    struct Visitor
    {
      PropertiesRef props;
      StringParam name;

      void operator()(std::monostate) { props.ClearProperty(std::move(name)); }
      void operator()(void* v) { props.SetPointerProperty(std::move(name), v); }

      void operator()(StringParam&& v)
      {
        props.SetStringProperty(std::move(name), std::move(v));
      }
      void operator()(const char* v)
      {
        props.SetStringProperty(std::move(name), std::move(v));
      }

      void operator()(Sint64 v) { props.SetNumberProperty(std::move(name), v); }
      void operator()(float v) { props.SetFloatProperty(std::move(name), v); }
      void operator()(bool v) { props.SetBooleanProperty(std::move(name), v); }
    };
    Visitor{props, std::move(name)}(std::forward<V>(value));
  }
}

template<class V>
inline void PropertiesBase::Set(StringParam name, V&& value)
{
  SDL::SetProperty(get(), std::move(name), std::forward<V>(value));
}

/**
 * Set a pointer property in a group of properties with a cleanup function that
 * is called when the property is deleted.
 *
 * The cleanup function is also called if setting the property fails for any
 * reason.
 *
 * For simply setting basic data types, like numbers, bools, or strings, use
 * SetNumberProperty, SetBooleanProperty, or SetStringProperty instead, as those
 * functions will handle cleanup on your behalf. This function is only for more
 * complex, custom data.
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
 * @sa GetPointerProperty
 * @sa SetPointerProperty
 * @sa CleanupPropertyCallback
 */
inline void SetPointerPropertyWithCleanup(PropertiesRef props,
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
 * SetNumberProperty, SetBooleanProperty, or SetStringProperty instead, as those
 * functions will handle cleanup on your behalf. This function is only for more
 * complex, custom data.
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
 * @sa GetPointerProperty
 * @sa SetPointerProperty
 * @sa CleanupPropertyCallback
 */
inline void SetPointerPropertyWithCleanup(PropertiesRef props,
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

inline void PropertiesBase::SetPointerPropertyWithCleanup(
  StringParam name,
  void* value,
  CleanupPropertyCallback cleanup,
  void* userdata)
{
  SDL::SetPointerPropertyWithCleanup(
    get(), std::move(name), value, cleanup, userdata);
}

inline void PropertiesBase::SetPointerPropertyWithCleanup(
  StringParam name,
  void* value,
  CleanupPropertyCB cleanup)
{
  SDL::SetPointerPropertyWithCleanup(
    get(), std::move(name), value, std::move(cleanup));
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
 * @sa GetPointerProperty
 * @sa HasProperty
 * @sa SetBooleanProperty
 * @sa SetFloatProperty
 * @sa SetNumberProperty
 * @sa SetPointerPropertyWithCleanup
 * @sa SetStringProperty
 */
inline void SetPointerProperty(PropertiesRef props,
                               StringParam name,
                               void* value)
{
  CheckError(SDL_SetPointerProperty(props, name, value));
}

inline void PropertiesBase::SetPointerProperty(StringParam name, void* value)
{
  SDL::SetPointerProperty(get(), std::move(name), value);
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
 * @sa GetStringProperty
 */
inline void SetStringProperty(PropertiesRef props,
                              StringParam name,
                              StringParam value)
{
  CheckError(SDL_SetStringProperty(props, name, value));
}

inline void PropertiesBase::SetStringProperty(StringParam name,
                                              StringParam value)
{
  SDL::SetStringProperty(get(), std::move(name), std::move(value));
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
inline void SetNumberProperty(PropertiesRef props,
                              StringParam name,
                              Sint64 value)
{
  CheckError(SDL_SetNumberProperty(props, name, value));
}

inline void PropertiesBase::SetNumberProperty(StringParam name, Sint64 value)
{
  SDL::SetNumberProperty(get(), std::move(name), value);
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
inline void SetFloatProperty(PropertiesRef props, StringParam name, float value)
{
  CheckError(SDL_SetFloatProperty(props, name, value));
}

inline void PropertiesBase::SetFloatProperty(StringParam name, float value)
{
  SDL::SetFloatProperty(get(), std::move(name), value);
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
inline void SetBooleanProperty(PropertiesRef props,
                               StringParam name,
                               bool value)
{
  CheckError(SDL_SetBooleanProperty(props, name, value));
}

inline void PropertiesBase::SetBooleanProperty(StringParam name, bool value)
{
  SDL::SetBooleanProperty(get(), std::move(name), value);
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
inline bool HasProperty(PropertiesRef props, StringParam name)
{
  return SDL_HasProperty(props, name);
}

inline bool PropertiesBase::HasProperty(StringParam name) const
{
  return SDL::HasProperty(get(), std::move(name));
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
 * @sa HasProperty
 */
inline PropertyType GetPropertyType(PropertiesRef props, StringParam name)
{
  return SDL_GetPropertyType(props, name);
}

inline PropertyType PropertiesBase::GetPropertyType(StringParam name) const
{
  return SDL::GetPropertyType(get(), std::move(name));
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
 *               if you call SetPointerProperty() or ClearProperty() on these
 *               properties from another thread. If you need to avoid this, use
 *               LockProperties() and UnlockProperties().
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
inline void* GetPointerProperty(PropertiesRef props,
                                StringParam name,
                                void* default_value = nullptr)
{
  return SDL_GetPointerProperty(props, name, default_value);
}

inline void* PropertiesBase::GetPointerProperty(StringParam name,
                                                void* default_value) const
{
  return SDL::GetPointerProperty(get(), std::move(name), default_value);
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
 *               if you call SetStringProperty() or ClearProperty() on these
 *               properties from another thread. If you need to avoid this, use
 *               LockProperties() and UnlockProperties().
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPropertyType
 * @sa HasProperty
 * @sa SetStringProperty
 */
inline const char* GetStringProperty(PropertiesRef props,
                                     StringParam name,
                                     StringParam default_value = "")
{
  return SDL_GetStringProperty(props, name, default_value);
}

inline const char* PropertiesBase::GetStringProperty(
  StringParam name,
  StringParam default_value) const
{
  return SDL::GetStringProperty(
    get(), std::move(name), std::move(default_value));
}

/**
 * Get a number property from a group of properties.
 *
 * You can use GetPropertyType() to query whether the property exists and is a
 * number property.
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
inline Sint64 GetNumberProperty(PropertiesRef props,
                                StringParam name,
                                Sint64 default_value = 0)
{
  return SDL_GetNumberProperty(props, name, default_value);
}

inline Sint64 PropertiesBase::GetNumberProperty(StringParam name,
                                                Sint64 default_value) const
{
  return SDL::GetNumberProperty(get(), std::move(name), default_value);
}

/**
 * Get a floating point property from a group of properties.
 *
 * You can use GetPropertyType() to query whether the property exists and is a
 * floating point property.
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
inline float GetFloatProperty(PropertiesRef props,
                              StringParam name,
                              float default_value = 0)
{
  return SDL_GetFloatProperty(props, name, default_value);
}

inline float PropertiesBase::GetFloatProperty(StringParam name,
                                              float default_value) const
{
  return SDL::GetFloatProperty(get(), std::move(name), default_value);
}

/**
 * Get a boolean property from a group of properties.
 *
 * You can use GetPropertyType() to query whether the property exists and is a
 * boolean property.
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
inline bool GetBooleanProperty(PropertiesRef props,
                               StringParam name,
                               bool default_value = false)
{
  return SDL_GetBooleanProperty(props, name, default_value);
}

inline bool PropertiesBase::GetBooleanProperty(StringParam name,
                                               bool default_value) const
{
  return SDL::GetBooleanProperty(get(), std::move(name), default_value);
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
inline void ClearProperty(PropertiesRef props, StringParam name)
{
  CheckError(SDL_ClearProperty(props, name));
}

inline void PropertiesBase::ClearProperty(StringParam name)
{
  SDL::ClearProperty(get(), std::move(name));
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
inline void EnumerateProperties(PropertiesRef props,
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
inline void EnumerateProperties(PropertiesRef props,
                                EnumeratePropertiesCB callback)
{
  return EnumerateProperties(
    props,
    [](void* userdata, PropertiesID props, const char* name) {
      auto& f = *static_cast<const EnumeratePropertiesCB*>(userdata);
      f(props, name);
    },
    &callback);
}

inline void PropertiesBase::Enumerate(EnumeratePropertiesCallback callback,
                                      void* userdata) const
{
  SDL::EnumerateProperties(get(), callback, userdata);
}

inline void PropertiesBase::Enumerate(EnumeratePropertiesCB callback) const
{
  SDL::EnumerateProperties(get(), std::move(callback));
}

/**
 * Returns the number of properties this has
 *
 * This uses EnumerateProperties() internally, so might not be so fast
 *
 * @param props
 * @return Uint64
 */
inline Uint64 CountProperties(PropertiesRef props)
{
  Uint64 count = 0;
  EnumerateProperties(props, [&count](auto, const char*) { count++; });
  return count;
}

inline Uint64 PropertiesBase::GetCount() const
{
  return SDL::CountProperties(get());
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
 * @sa CreateProperties
 */
inline void DestroyProperties(PropertiesID props)
{
  SDL_DestroyProperties(props);
}

inline void PropertiesBase::Destroy() { DestroyProperties(release()); }

/// @}

/**
 * @namespace SDL::prop
 * @brief Constants for Properties keys.
 *
 * All subnamespaces in `prop` are intended to be used as namespaces for
 * properties related to a specific type of object. For example, `prop::Surface`
 * contains properties related to surfaces.
 *
 * @sa Properties
 */

} // namespace SDL

#endif /* SDL3PP_PROPERTIES_H_ */
