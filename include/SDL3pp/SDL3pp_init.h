#ifndef SDL3PP_INIT_H_
#define SDL3PP_INIT_H_

#include <SDL3/SDL_init.h>
#include "SDL3pp_callbackWrapper.h"
#include "SDL3pp_error.h"
#include "SDL3pp_stdinc.h"
#include "SDL3pp_stringParam.h"

/**
 * @namespace SDL
 * @brief the main namespace where all SDL3pp public functions and types live
 */
namespace SDL {

/**
 * @defgroup CategoryInit Category Init
 *
 * All SDL programs need to initialize the library before starting to work
 * with it.
 *
 * Almost everything can simply call SDL_Init() near startup, with a handful
 * of flags to specify subsystems to touch. These are here to make sure SDL
 * does not even attempt to touch low-level pieces of the operating system
 * that you don't intend to use. For example, you might be using SDL for video
 * and input but chose an external library for audio, and in this case you
 * would just need to leave off the `SDL_INIT_AUDIO` flag to make sure that
 * external library has complete control.
 *
 * Most apps, when terminating, should call SDL_Quit(). This will clean up
 * (nearly) everything that SDL might have allocated, and crucially, it'll
 * make sure that the display's resolution is back to what the user expects if
 * you had previously changed it for your game.
 *
 * SDL3 apps are strongly encouraged to call SDL_SetAppMetadata() at startup
 * to fill in details about the program. This is completely optional, but it
 * helps in small ways (we can provide an About dialog box for the macOS menu,
 * we can name the app in the system's audio mixer, etc). Those that want to
 * provide a _lot_ of information should look at the more-detailed
 * SDL_SetAppMetadataProperty().
 *
 * @{
 */

/**
 * @brief Initialization flags for SDL
 */
using InitFlags = SDL_InitFlags;

/**
 * Return values for optional main callbacks.
 *
 * Returning SDL_APP_SUCCESS or SDL_APP_FAILURE from SDL_AppInit,
 * SDL_AppEvent, or SDL_AppIterate will terminate the program and report
 * success/failure to the operating system. What that means is
 * platform-dependent. On Unix, for example, on success, the process error
 * code will be zero, and on failure it will be 1. This interface doesn't
 * allow you to return specific exit codes, just whether there was an error
 * generally or not.
 *
 * Returning SDL_APP_CONTINUE from these functions will let the app continue
 * to run.
 *
 * See
 * [Main callbacks in
 * SDL3](https://wiki.libsdl.org/SDL3/README/main-functions#main-callbacks-in-sdl3)
 * for complete details.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using AppResult = SDL_AppResult;

/**
 * Function pointer typedef for SDL_AppInit.
 *
 * These are used by SDL_EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppInit directly.
 *
 * @param appstate a place where the app can optionally store a pointer for
 *                 future use.
 * @param argc the standard ANSI C main's argc; number of elements in `argv`.
 * @param argv the standard ANSI C main's argv; array of command line
 *             arguments.
 * @returns SDL_APP_FAILURE to terminate with an error, SDL_APP_SUCCESS to
 *          terminate with success, SDL_APP_CONTINUE to continue.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppInit_func = SDL_AppInit_func;

/**
 * Function pointer typedef for SDL_AppIterate.
 *
 * These are used by SDL_EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppIterate directly.
 *
 * @param appstate an optional pointer, provided by the app in SDL_AppInit.
 * @returns SDL_APP_FAILURE to terminate with an error, SDL_APP_SUCCESS to
 *          terminate with success, SDL_APP_CONTINUE to continue.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppIterate_func = SDL_AppIterate_func;

/**
 * Function pointer typedef for SDL_AppEvent.
 *
 * These are used by SDL_EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppEvent directly.
 *
 * @param appstate an optional pointer, provided by the app in SDL_AppInit.
 * @param event the new event for the app to examine.
 * @returns SDL_APP_FAILURE to terminate with an error, SDL_APP_SUCCESS to
 *          terminate with success, SDL_APP_CONTINUE to continue.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppEvent_func = SDL_AppEvent_func;

/**
 * Function pointer typedef for SDL_AppQuit.
 *
 * These are used by SDL_EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppEvent directly.
 *
 * @param appstate an optional pointer, provided by the app in SDL_AppInit.
 * @param result the result code that terminated the app (success or failure).
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppQuit_func = SDL_AppQuit_func;

/**
 * Initialize the SDL library.
 *
 * SDL_Init() simply forwards to calling SDL_InitSubSystem(). Therefore, the
 * two may be used interchangeably. Though for readability of your code
 * SDL_InitSubSystem() might be preferred.
 *
 * The file I/O (for example: SDL_IOFromFile) and threading (SDL_CreateThread)
 * subsystems are initialized by default. Message boxes
 * (SDL_ShowSimpleMessageBox) also attempt to work without initializing the
 * video subsystem, in hopes of being useful in showing an error dialog when
 * SDL_Init fails. You must specifically initialize other subsystems if you
 * use them in your application.
 *
 * Logging (such as SDL_Log) works without initialization, too.
 *
 * `flags` may be any of the following OR'd together:
 *
 * - `SDL_INIT_AUDIO`: audio subsystem; automatically initializes the events
 *   subsystem
 * - `SDL_INIT_VIDEO`: video subsystem; automatically initializes the events
 *   subsystem, should be initialized on the main thread.
 * - `SDL_INIT_JOYSTICK`: joystick subsystem; automatically initializes the
 *   events subsystem
 * - `SDL_INIT_HAPTIC`: haptic (force feedback) subsystem
 * - `SDL_INIT_GAMEPAD`: gamepad subsystem; automatically initializes the
 *   joystick subsystem
 * - `SDL_INIT_EVENTS`: events subsystem
 * - `SDL_INIT_SENSOR`: sensor subsystem; automatically initializes the events
 *   subsystem
 * - `SDL_INIT_CAMERA`: camera subsystem; automatically initializes the events
 *   subsystem
 *
 * Subsystem initialization is ref-counted, you must call SDL_QuitSubSystem()
 * for each SDL_InitSubSystem() to correctly shutdown a subsystem manually (or
 * call SDL_Quit() to force shutdown). If a subsystem is already loaded then
 * this call will increase the ref-count and return.
 *
 * Consider reporting some basic metadata about your application before
 * calling SDL_Init, using either SDL_SetAppMetadata() or
 * SDL_SetAppMetadataProperty().
 *
 * @param flags subsystem initialization flags.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAppMetadata()
 * @sa SetAppMetadataProperty()
 * @sa InitSubSystem()
 * @sa Quit()
 * @sa SetMainReady()
 * @sa WasInit()
 */
inline bool Init(InitFlags flags) { return SDL_Init(flags); }

/**
 * Compatibility function to initialize the SDL library.
 *
 * This function and SDL_Init() are interchangeable.
 *
 * @param flags any of the flags used by SDL_Init(); see SDL_Init for details.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init()
 * @sa Quit()
 * @sa QuitSubSystem()
 */
inline bool InitSubSystem(InitFlags flags) { return SDL_InitSubSystem(flags); }

/**
 * Shut down specific SDL subsystems.
 *
 * You still need to call SDL_Quit() even if you close all open subsystems
 * with SDL_QuitSubSystem().
 *
 * @param flags any of the flags used by SDL_Init(); see SDL_Init for details.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa InitSubSystem()
 * @sa Quit()
 */
inline void QuitSubSystem(InitFlags flags) { return SDL_QuitSubSystem(flags); }

/**
 * Get a mask of the specified subsystems which are currently initialized.
 *
 * @param flags any of the flags used by SDL_Init(); see SDL_Init for details.
 * @returns a mask of all initialized subsystems if `flags` is 0, otherwise it
 *          returns the initialization status of the specified subsystems.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init()
 * @sa InitSubSystem()
 */
inline InitFlags WasInit(InitFlags flags) { return SDL_WasInit(flags); }

/**
 * Clean up all initialized subsystems.
 *
 * You should call this function even if you have already shutdown each
 * initialized subsystem with SDL_QuitSubSystem(). It is safe to call this
 * function even in the case of errors in initialization.
 *
 * You can use this function with atexit() to ensure that it is run when your
 * application is shutdown, but it is not wise to do this from a library or
 * other dynamically loaded code.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init()
 * @sa QuitSubSystem()
 */
inline void Quit() { return SDL_Quit(); }

/**
 * @brief Initialize the SDL library.
 *
 * Also init any subsystem passed as InitFlags
 *
 * This might be called multiple times, it keeps a ref count and calls SDL_Quit
 * only on the last one.
 *
 * The SubSystems are out of the refCount, as SDL itself already keep track
 * internally.
 */
struct SDL
{

  /**
   * @brief Init given subsystems
   * @param flags
   *
   * This uses a refCount internally, so it is safe to call
   * this multiple times, the quit will be called only on the last call.
   */
  SDL(InitFlags flags)
    : flags(flags)
  {
    refCount(+1, true);
  }

  // Copy ctor
  SDL(const SDL& other)
    : flags(other.flags)
    , active(other.active)
  {
    if (active) refCount(+1);
  }

  // Move ctor
  SDL(SDL&& other)
    : flags(other.flags)
    , active(other.active)
  {
    other.active = false;
  }

  // Dtor
  ~SDL()
  {
    if (active) refCount(-1);
  }

  SDL& operator=(SDL rhs)
  {
    std::swap(active, rhs.active);
    std::swap(flags, rhs.flags);
    return *this;
  }

  /**
   * @brief Check if given subSystems are initialized
   * @param flags the flags to test or 0 to test all
   * @return Which subsystem are active
   */
  static InitFlags WasInit(InitFlags flags = 0) { return SDL_WasInit(flags); }

  /**
   * @brief release locking such as reset() does, but never calls SDL_Quit() or
   * SDL_QuitSubSystem()
   * @return false if there are still other locks, true if this was last one
   *
   * When this returns true it is safe to call SDL_Quit()
   */
  bool release()
  {
    flags = 0;
    return refCount(-1, false) == 0;
  }

  /**
   * @brief reset the value of this instance, acts like it was destroyed and
   * then newly instantiated
   * @return false if there are still other locks, true if this was last one
   */
  bool reset() { return refCount(-1) == 0; }

  /// @brief returns true if active and has no errors
  operator bool() const { return active; }

  InitFlags GetCurrentFlags() const { return flags; }

private:
  InitFlags flags = 0;
  bool active = true;

  int refCount(int delta, bool autoQuit = true);
};

/**
 * Return whether this is the main thread.
 *
 * On Apple platforms, the main thread is the thread that runs your program's
 * main() entry point. On other platforms, the main thread is the one that
 * calls SDL_Init(SDL_INIT_VIDEO), which should usually be the one that runs
 * your program's main() entry point. If you are using the main callbacks,
 * SDL_AppInit(), SDL_AppIterate(), and SDL_AppQuit() are all called on the
 * main thread.
 *
 * @returns true if this thread is the main thread, or false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RunOnMainThread()
 */
inline bool IsMainThread() { return SDL_IsMainThread(); }

/**
 * Callback run on the main thread.
 *
 * @param userdata an app-controlled pointer that is passed to the callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa RunOnMainThread()
 */
using MainThreadCallback = SDL_MainThreadCallback;

/**
 * @sa PropertiesRef.MainThreadCallback
 */
using MainThreadFunction = std::function<void()>;

/**
 * Call a function on the main thread during event processing.
 *
 * If this is called on the main thread, the callback is executed immediately.
 * If this is called on another thread, this callback is queued for execution
 * on the main thread during event processing.
 *
 * Be careful of deadlocks when using this functionality. You should not have
 * the main thread wait for the current thread while this function is being
 * called with `wait_complete` true.
 *
 * @param callback the callback to call on the main thread.
 * @param userdata a pointer that is passed to `callback`.
 * @param wait_complete true to wait for the callback to complete, false to
 *                      return immediately.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IsMainThread()
 */
inline bool RunOnMainThread(MainThreadCallback callback,
                            void* userdata,
                            bool wait_complete)
{
  return SDL_RunOnMainThread(callback, userdata, wait_complete);
}

/**
 * Call a function on the main thread during event processing.
 *
 * If this is called on the main thread, the callback is executed immediately.
 * If this is called on another thread, this callback is queued for execution
 * on the main thread during event processing.
 *
 * Be careful of deadlocks when using this functionality. You should not have
 * the main thread wait for the current thread while this function is being
 * called with `wait_complete` true.
 *
 * @param callback the callback to call on the main thread.
 * @param wait_complete true to wait for the callback to complete, false to
 *                      return immediately.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IsMainThread()
 */
inline bool RunOnMainThread(MainThreadFunction callback, bool wait_complete)
{
  using Wrapper = CallbackWrapper<void()>;
  return RunOnMainThread(
    &Wrapper::CallOnce, Wrapper::Wrap(std::move(callback)), wait_complete);
}

/**
 * Specify basic metadata about your app.
 *
 * You can optionally provide metadata about your app to SDL. This is not
 * required, but strongly encouraged.
 *
 * There are several locations where SDL can make use of metadata (an "About"
 * box in the macOS menu bar, the name of the app can be shown on some audio
 * mixers, etc). Any piece of metadata can be left as NULL, if a specific
 * detail doesn't make sense for the app.
 *
 * This function should be called as early as possible, before SDL_Init.
 * Multiple calls to this function are allowed, but various state might not
 * change once it has been set up with a previous call to this function.
 *
 * Passing a NULL removes any previous metadata.
 *
 * This is a simplified interface for the most important information. You can
 * supply significantly more detailed metadata with
 * SDL_SetAppMetadataProperty().
 *
 * @param appname The name of the application ("My Game 2: Bad Guy's
 *                Revenge!").
 * @param appversion The version of the application ("1.0.0beta5" or a git
 *                   hash, or whatever makes sense).
 * @param appidentifier A unique string in reverse-domain format that
 *                      identifies this app ("com.example.mygame2").
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAppMetadataProperty()
 */
inline bool SetAppMetadata(StringParam appname,
                           StringParam appversion,
                           StringParam appidentifier)
{
  return SDL_SetAppMetadata(appname, appversion, appidentifier);
}

/**
 * Specify metadata about your app through a set of properties.
 *
 * You can optionally provide metadata about your app to SDL. This is not
 * required, but strongly encouraged.
 *
 * There are several locations where SDL can make use of metadata (an "About"
 * box in the macOS menu bar, the name of the app can be shown on some audio
 * mixers, etc). Any piece of metadata can be left out, if a specific detail
 * doesn't make sense for the app.
 *
 * This function should be called as early as possible, before SDL_Init.
 * Multiple calls to this function are allowed, but various state might not
 * change once it has been set up with a previous call to this function.
 *
 * Once set, this metadata can be read using SDL_GetAppMetadataProperty().
 *
 * These are the supported properties:
 *
 * - `SDL_PROP_APP_METADATA_NAME_STRING`: The human-readable name of the
 *   application, like "My Game 2: Bad Guy's Revenge!". This will show up
 *   anywhere the OS shows the name of the application separately from window
 *   titles, such as volume control applets, etc. This defaults to "SDL
 *   Application".
 * - `SDL_PROP_APP_METADATA_VERSION_STRING`: The version of the app that is
 *   running; there are no rules on format, so "1.0.3beta2" and "April 22nd,
 *   2024" and a git hash are all valid options. This has no default.
 * - `SDL_PROP_APP_METADATA_IDENTIFIER_STRING`: A unique string that
 *   identifies this app. This must be in reverse-domain format, like
 *   "com.example.mygame2". This string is used by desktop compositors to
 *   identify and group windows together, as well as match applications with
 *   associated desktop settings and icons. If you plan to package your
 *   application in a container such as Flatpak, the app ID should match the
 *   name of your Flatpak container as well. This has no default.
 * - `SDL_PROP_APP_METADATA_CREATOR_STRING`: The human-readable name of the
 *   creator/developer/maker of this app, like "MojoWorkshop, LLC"
 * - `SDL_PROP_APP_METADATA_COPYRIGHT_STRING`: The human-readable copyright
 *   notice, like "Copyright (c) 2024 MojoWorkshop, LLC" or whatnot. Keep this
 *   to one line, don't paste a copy of a whole software license in here. This
 *   has no default.
 * - `SDL_PROP_APP_METADATA_URL_STRING`: A URL to the app on the web. Maybe a
 *   product page, or a storefront, or even a GitHub repository, for user's
 *   further information This has no default.
 * - `SDL_PROP_APP_METADATA_TYPE_STRING`: The type of application this is.
 *   Currently this string can be "game" for a video game, "mediaplayer" for a
 *   media player, or generically "application" if nothing else applies.
 *   Future versions of SDL might add new types. This defaults to
 *   "application".
 *
 * @param name the name of the metadata property to set.
 * @param value the value of the property, or NULL to remove that property.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAppMetadataProperty()
 * @sa SetAppMetadata()
 */
inline bool SetAppMetadataProperty(StringParam name, StringParam value)
{
  return SDL_SetAppMetadataProperty(name, value);
}

/**
 * Get metadata about your app.
 *
 * This returns metadata previously set using SDL_SetAppMetadata() or
 * SDL_SetAppMetadataProperty(). See SDL_SetAppMetadataProperty() for the list
 * of available properties and their meanings.
 *
 * @param name the name of the metadata property to get.
 * @returns the current value of the metadata property, or the default if it
 *          is not set, NULL for properties with no default.
 *
 * @threadsafety It is safe to call this function from any thread, although
 *               the string returned is not protected and could potentially be
 *               freed if you call SDL_SetAppMetadataProperty() to set that
 *               property from another thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAppMetadata()
 * @sa SetAppMetadataProperty()
 */
inline const char* GetAppMetadataProperty(StringParam name)
{
  return SDL_GetAppMetadataProperty(name);
}

/** @} */

#pragma region impl

inline int SDL::refCount(int delta, bool autoQuit)
{
  // TODO Locking these?
  static int refCount = 0;
  if (delta && active) {
    if (delta > 0) {
      refCount += 1;
      if (flags) active = SDL_InitSubSystem(flags);
    } else {
      SDL_assert_always(refCount > 0);
      active = false;
      refCount -= 1;

      if (autoQuit) {
        if (refCount <= 0) {
          // TODO Make this under FLAG
          SDL_Quit();
        } else if (flags) {
          SDL_QuitSubSystem(flags);
        }
      }
    }
  }
  return refCount;
}

#pragma endregion

} // namespace SDL

#endif /* SDL3PP_INIT_H_ */
