#ifndef SDL3PP_INIT_H_
#define SDL3PP_INIT_H_

#include <atomic>
#include <SDL3/SDL_init.h>
#include "SDL3pp_callbackWrapper.h"
#include "SDL3pp_error.h"
#include "SDL3pp_log.h"
#include "SDL3pp_strings.h"

namespace SDL {

/**
 * @defgroup CategoryInit Initialization and Shutdown
 *
 * All SDL programs need to initialize the library before starting to work
 * with it.
 *
 * Almost everything can simply call Init() near startup, with a handful
 * of flags to specify subsystems to touch. These are here to make sure SDL
 * does not even attempt to touch low-level pieces of the operating system
 * that you don't intend to use. For example, you might be using SDL for video
 * and input but chose an external library for audio, and in this case you
 * would just need to leave off the `INIT_AUDIO` flag to make sure that
 * external library has complete control.
 *
 * Most apps, when terminating, should call Quit(). This will clean up
 * (nearly) everything that SDL might have allocated, and crucially, it'll
 * make sure that the display's resolution is back to what the user expects if
 * you had previously changed it for your game.
 *
 * SDL3 apps are strongly encouraged to call SetAppMetadata() at startup
 * to fill in details about the program. This is completely optional, but it
 * helps in small ways (we can provide an About dialog box for the macOS menu,
 * we can name the app in the system's audio mixer, etc). Those that want to
 * provide a _lot_ of information should look at the more-detailed
 * SetAppMetadataProperty().
 *
 * @{
 */

/**
 * @defgroup InitFlags Initialization flags
 *
 * @{
 */

/**
 * Initialization flags for Init and/or InitSubSystem
 *
 * These are the flags which may be passed to Init(). You should specify
 * the subsystems which you will be using in your application.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa Init
 * @sa Quit
 * @sa InitSubSystem
 * @sa QuitSubSystem
 * @sa WasInit
 */
using InitFlags = Uint32;

constexpr InitFlags INIT_AUDIO =
  SDL_INIT_AUDIO; ///< `INIT_AUDIO` implies `INIT_EVENTS`

/**
 * `INIT_VIDEO` implies `INIT_EVENTS`, should be initialized on the main thread
 */
constexpr InitFlags INIT_VIDEO = SDL_INIT_VIDEO;

constexpr InitFlags INIT_JOYSTICK =
  SDL_INIT_JOYSTICK; ///< `INIT_JOYSTICK` implies `INIT_EVENTS`

constexpr InitFlags INIT_HAPTIC = SDL_INIT_HAPTIC; ///< HAPTIC

constexpr InitFlags INIT_GAMEPAD =
  SDL_INIT_GAMEPAD; ///< `INIT_GAMEPAD` implies `INIT_JOYSTICK`

constexpr InitFlags INIT_EVENTS = SDL_INIT_EVENTS; ///< EVENTS

constexpr InitFlags INIT_SENSOR =
  SDL_INIT_SENSOR; ///< `INIT_SENSOR` implies `INIT_EVENTS`

constexpr InitFlags INIT_CAMERA =
  SDL_INIT_CAMERA; ///< `INIT_CAMERA` implies `INIT_EVENTS`

/// @}

/**
 * @name AppResult
 * App result for Main callback
 * @{
 */

/**
 * Return values for optional main callbacks.
 *
 * Returning APP_SUCCESS or APP_FAILURE from SDL_AppInit,
 * SDL_AppEvent, or SDL_AppIterate will terminate the program and report
 * success/failure to the operating system. What that means is
 * platform-dependent. On Unix, for example, on success, the process error
 * code will be zero, and on failure it will be 1. This interface doesn't
 * allow you to return specific exit codes, just whether there was an error
 * generally or not.
 *
 * Returning APP_CONTINUE from these functions will let the app continue
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
 * Value that requests that the app continue from the main callbacks.
 */
constexpr AppResult APP_CONTINUE = SDL_APP_CONTINUE;

/**
 * Value that requests termination with success from the main callbacks.
 */
constexpr AppResult APP_SUCCESS = SDL_APP_SUCCESS;

/**
 * Value that requests termination with error from the main callbacks.
 */
constexpr AppResult APP_FAILURE = SDL_APP_FAILURE;

/// @}

/**
 * @name Callbacks for EnterAppMainCallbacks()
 *
 * @{
 */

/**
 * Function pointer typedef for SDL_AppInit.
 *
 * These are used by EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppInit directly.
 *
 * @param appstate a place where the app can optionally store a pointer for
 *                 future use.
 * @param argc the standard ANSI C main's argc; number of elements in `argv`.
 * @param argv the standard ANSI C main's argv; array of command line
 *             arguments.
 * @returns APP_FAILURE to terminate with an error, APP_SUCCESS to
 *          terminate with success, APP_CONTINUE to continue.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppInit_func = SDL_AppInit_func;

/**
 * Function pointer typedef for SDL_AppIterate.
 *
 * These are used by EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppIterate directly.
 *
 * @param appstate an optional pointer, provided by the app in SDL_AppInit.
 * @returns APP_FAILURE to terminate with an error, APP_SUCCESS to
 *          terminate with success, APP_CONTINUE to continue.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppIterate_func = SDL_AppIterate_func;

/**
 * Function pointer typedef for SDL_AppEvent.
 *
 * These are used by EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppEvent directly.
 *
 * @param appstate an optional pointer, provided by the app in SDL_AppInit.
 * @param event the new event for the app to examine.
 * @returns APP_FAILURE to terminate with an error, APP_SUCCESS to
 *          terminate with success, APP_CONTINUE to continue.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppEvent_func = SDL_AppEvent_func;

/**
 * Function pointer typedef for SDL_AppQuit.
 *
 * These are used by EnterAppMainCallbacks. This mechanism operates behind
 * the scenes for apps using the optional main callbacks. Apps that want to
 * use this should just implement SDL_AppEvent directly.
 *
 * @param appstate an optional pointer, provided by the app in SDL_AppInit.
 * @param result the result code that terminated the app (success or failure).
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AppQuit_func = SDL_AppQuit_func;

/// @}

/**
 * Initialize the SDL library.
 *
 * Init() simply forwards to calling InitSubSystem(). Therefore, the
 * two may be used interchangeably. Though for readability of your code
 * InitSubSystem() might be preferred.
 *
 * The file I/O (for example: SDL_IOFromFile) and threading (SDL_CreateThread)
 * subsystems are initialized by default. Message boxes
 * (SDL_ShowSimpleMessageBox) also attempt to work without initializing the
 * video subsystem, in hopes of being useful in showing an error dialog when
 * Init fails. You must specifically initialize other subsystems if you
 * use them in your application.
 *
 * Logging (such as Log) works without initialization, too.
 *
 * `flags` may be any of the following OR'd together:
 *
 * - `INIT_AUDIO`: audio subsystem; automatically initializes the events
 *   subsystem
 * - `INIT_VIDEO`: video subsystem; automatically initializes the events
 *   subsystem, should be initialized on the main thread.
 * - `INIT_JOYSTICK`: joystick subsystem; automatically initializes the
 *   events subsystem
 * - `INIT_HAPTIC`: haptic (force feedback) subsystem
 * - `INIT_GAMEPAD`: gamepad subsystem; automatically initializes the
 *   joystick subsystem
 * - `INIT_EVENTS`: events subsystem
 * - `INIT_SENSOR`: sensor subsystem; automatically initializes the events
 *   subsystem
 * - `INIT_CAMERA`: camera subsystem; automatically initializes the events
 *   subsystem
 *
 * Subsystem initialization is ref-counted, you must call QuitSubSystem()
 * for each InitSubSystem() to correctly shutdown a subsystem manually (or
 * call Quit() to force shutdown). If a subsystem is already loaded then
 * this call will increase the ref-count and return.
 *
 * Consider reporting some basic metadata about your application before
 * calling Init, using either SetAppMetadata() or
 * SetAppMetadataProperty().
 *
 * @param flags subsystem initialization flags.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAppMetadata
 * @sa SetAppMetadataProperty
 * @sa InitSubSystem
 * @sa Quit
 * @sa SetMainReady
 * @sa WasInit
 */
inline void Init(InitFlags flags) { CheckError(SDL_Init(flags)); }

/**
 * Compatibility function to initialize the SDL library.
 *
 * This function and Init() are interchangeable.
 *
 * @param flags any of the flags used by Init(); see Init for details.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init
 * @sa Quit
 * @sa QuitSubSystem
 */
inline void InitSubSystem(InitFlags flags)
{
  CheckError(SDL_InitSubSystem(flags));
}

/**
 * Shut down specific SDL subsystems.
 *
 * You still need to call Quit() even if you close all open subsystems
 * with QuitSubSystem().
 *
 * @param flags any of the flags used by Init(); see Init for details.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa InitSubSystem
 * @sa Quit
 */
inline void QuitSubSystem(InitFlags flags) { SDL_QuitSubSystem(flags); }

/**
 * Get a mask of the specified subsystems which are currently initialized.
 *
 * @param flags any of the flags used by Init(); see Init for details.
 * @returns a mask of all initialized subsystems if `flags` is 0, otherwise it
 *          returns the initialization status of the specified subsystems.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init
 * @sa InitSubSystem
 */
inline InitFlags WasInit(InitFlags flags) { return SDL_WasInit(flags); }

/**
 * Clean up all initialized subsystems.
 *
 * You should call this function even if you have already shutdown each
 * initialized subsystem with QuitSubSystem(). It is safe to call this
 * function even in the case of errors in initialization.
 *
 * You can use this function with atexit() to ensure that it is run when your
 * application is shutdown, but it is not wise to do this from a library or
 * other dynamically loaded code.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init
 * @sa QuitSubSystem
 */
inline void Quit() { SDL_Quit(); }

/**
 * Return whether this is the main thread.
 *
 * On Apple platforms, the main thread is the thread that runs your program's
 * main() entry point. On other platforms, the main thread is the one that
 * calls Init(INIT_VIDEO), which should usually be the one that runs
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
 * @sa RunOnMainThread
 */
inline bool IsMainThread() { return SDL_IsMainThread(); }

/**
 * @name Callbacks for RunOnMainThread()
 * @{
 */

/**
 * Callback run on the main thread.
 *
 * @param userdata an app-controlled pointer that is passed to the callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa RunOnMainThread
 */
using MainThreadCallback = SDL_MainThreadCallback;

/**
 * Callback run on the main thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa RunOnMainThread
 * @sa MainThreadCallback
 *
 * @cat result-callback
 *
 */
using MainThreadCB = std::function<void()>;

/// @}

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
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IsMainThread
 */
inline void RunOnMainThread(MainThreadCallback callback,
                            void* userdata,
                            bool wait_complete)
{
  CheckError(SDL_RunOnMainThread(callback, userdata, wait_complete));
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
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IsMainThread
 * @sa result-callback
 *
 * @cat result-callback
 */
inline void RunOnMainThread(MainThreadCB callback, bool wait_complete)
{
  using Wrapper = CallbackWrapper<MainThreadCB>;
  void* wrapped = Wrapper::Wrap(std::move(callback));
  RunOnMainThread(&Wrapper::CallOnce, wrapped, wait_complete);
}

/**
 * Specify basic metadata about your app.
 *
 * You can optionally provide metadata about your app to SDL. This is not
 * required, but strongly encouraged.
 *
 * There are several locations where SDL can make use of metadata (an "About"
 * box in the macOS menu bar, the name of the app can be shown on some audio
 * mixers, etc). Any piece of metadata can be left as nullptr, if a specific
 * detail doesn't make sense for the app.
 *
 * This function should be called as early as possible, before Init.
 * Multiple calls to this function are allowed, but various state might not
 * change once it has been set up with a previous call to this function.
 *
 * Passing a nullptr removes any previous metadata.
 *
 * This is a simplified interface for the most important information. You can
 * supply significantly more detailed metadata with
 * SetAppMetadataProperty().
 *
 * @param appname The name of the application ("My Game 2: Bad Guy's
 *                Revenge!").
 * @param appversion The version of the application ("1.0.0beta5" or a git
 *                   hash, or whatever makes sense).
 * @param appidentifier A unique string in reverse-domain format that
 *                      identifies this app ("com.example.mygame2").
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAppMetadataProperty
 */
inline void SetAppMetadata(StringParam appname,
                           StringParam appversion,
                           StringParam appidentifier)
{
  CheckError(SDL_SetAppMetadata(appname, appversion, appidentifier));
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
 * This function should be called as early as possible, before Init.
 * Multiple calls to this function are allowed, but various state might not
 * change once it has been set up with a previous call to this function.
 *
 * Once set, this metadata can be read using GetAppMetadataProperty().
 *
 * These are the supported properties:
 *
 * - `prop::appMetaData.NAME_STRING`: The human-readable name of the
 *   application, like "My Game 2: Bad Guy's Revenge!". This will show up
 *   anywhere the OS shows the name of the application separately from window
 *   titles, such as volume control applets, etc. This defaults to "SDL
 *   Application".
 * - `prop::appMetaData.VERSION_STRING`: The version of the app that is
 *   running; there are no rules on format, so "1.0.3beta2" and "April 22nd,
 *   2024" and a git hash are all valid options. This has no default.
 * - `prop::appMetaData.IDENTIFIER_STRING`: A unique string that
 *   identifies this app. This must be in reverse-domain format, like
 *   "com.example.mygame2". This string is used by desktop compositors to
 *   identify and group windows together, as well as match applications with
 *   associated desktop settings and icons. If you plan to package your
 *   application in a container such as Flatpak, the app ID should match the
 *   name of your Flatpak container as well. This has no default.
 * - `prop::appMetaData.CREATOR_STRING`: The human-readable name of the
 *   creator/developer/maker of this app, like "MojoWorkshop, LLC"
 * - `prop::appMetaData.COPYRIGHT_STRING`: The human-readable copyright
 *   notice, like "Copyright (c) 2024 MojoWorkshop, LLC" or whatnot. Keep this
 *   to one line, don't paste a copy of a whole software license in here. This
 *   has no default.
 * - `prop::appMetaData.URL_STRING`: A URL to the app on the web. Maybe a
 *   product page, or a storefront, or even a GitHub repository, for user's
 *   further information This has no default.
 * - `prop::appMetaData.TYPE_STRING`: The type of application this is.
 *   Currently this string can be "game" for a video game, "mediaplayer" for a
 *   media player, or generically "application" if nothing else applies.
 *   Future versions of SDL might add new types. This defaults to
 *   "application".
 *
 * @param name the name of the metadata property to set.
 * @param value the value of the property, or nullptr to remove that property.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAppMetadataProperty
 * @sa SetAppMetadata
 */
inline void SetAppMetadataProperty(StringParam name, StringParam value)
{
  CheckError(SDL_SetAppMetadataProperty(name, value));
}

namespace prop::appMetaData {

constexpr auto NAME_STRING = SDL_PROP_APP_METADATA_NAME_STRING;

constexpr auto VERSION_STRING = SDL_PROP_APP_METADATA_VERSION_STRING;

constexpr auto IDENTIFIER_STRING = SDL_PROP_APP_METADATA_IDENTIFIER_STRING;

constexpr auto CREATOR_STRING = SDL_PROP_APP_METADATA_CREATOR_STRING;

constexpr auto COPYRIGHT_STRING = SDL_PROP_APP_METADATA_COPYRIGHT_STRING;

constexpr auto URL_STRING = SDL_PROP_APP_METADATA_URL_STRING;

constexpr auto TYPE_STRING = SDL_PROP_APP_METADATA_TYPE_STRING;

} // namespace prop::appMetaData

/**
 * Get metadata about your app.
 *
 * This returns metadata previously set using SetAppMetadata() or
 * SetAppMetadataProperty(). See SetAppMetadataProperty() for the list
 * of available properties and their meanings.
 *
 * @param name the name of the metadata property to get.
 * @returns the current value of the metadata property, or the default if it
 *          is not set, nullptr for properties with no default.
 *
 * @threadsafety It is safe to call this function from any thread, although
 *               the string returned is not protected and could potentially be
 *               freed if you call SetAppMetadataProperty() to set that
 *               property from another thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAppMetadata
 * @sa SetAppMetadataProperty
 */
inline const char* GetAppMetadataProperty(StringParam name)
{
  return SDL_GetAppMetadataProperty(name);
}

#ifndef SDL3PP_APPCLASS_LOG_PRIORITY
/**
 * The default log priority for app class.
 */
#define SDL3PP_APPCLASS_LOG_PRIORITY LOG_PRIORITY_CRITICAL
#endif // SDL3PP_APPCLASS_LOG_PRIORITY

/**
 * Represents application parameters
 */
using AppArgs = std::span<char const* const>;

/**
 * @{
 *
 * Allocate and initialize state with new.
 *
 * If possible, pass the args to constructor, otherwise expects a default ctor;
 *
 * @tparam T the state class
 * @param state the state to initialize
 * @param args the program arguments
 * @return the app status
 */
template<class T>
inline AppResult DefaultCreateClass(T** state, AppArgs args)
{
  static_assert(std::is_default_constructible_v<T>);
  *state = new T{};
  return APP_CONTINUE;
}

template<class T>
  requires std::convertible_to<AppArgs, T>
inline AppResult DefaultCreateClass(T** state, AppArgs args)
{
  *state = new T{args};
  return APP_CONTINUE;
}
/// @}

/// @private
template<class T>
concept HasInitFunction = requires(T** state) {
  { T::Init(state, AppArgs{}) } -> std::convertible_to<AppResult>;
};

/**
 * @{
 *
 * Init state with arguments.
 *
 * This will call T::Init() if available, otherwise it delegates to
 * DefaultCreateClass().
 *
 * @tparam T the state class
 * @param state the state to initialize
 * @param args the program arguments
 * @return the app status
 */
template<class T>
inline AppResult InitClass(T** state, AppArgs args)
{
  try {
    return DefaultCreateClass(state, args);
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
  }
  return APP_FAILURE;
}

template<HasInitFunction T>
inline AppResult InitClass(T** state, AppArgs args)
{
  *state = nullptr;
  try {
    AppResult result = T::Init(state, args);
    if (*state == nullptr && result != APP_FAILURE) return APP_SUCCESS;
    return result;
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
  }
  return APP_FAILURE;
}
/// @}

/// @private
template<class T>
concept HasIterateFunction = requires(T* state) { state->Iterate(); };

/**
 * Iterate the state
 *
 * @tparam T the state class
 * @param state the state
 * @return the app status
 */
template<HasIterateFunction T>
inline AppResult IterateClass(T* state)
{
  try {
    return state->Iterate();
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
  }
  return APP_FAILURE;
}

/// @private
template<class T>
concept HasEventFunction =
  requires(T* state, const SDL_Event& event) { state->Event(event); };

/**
 * Default handle by finishing if QUIT is requested
 *
 * @tparam T the state class
 * @param state the state
 * @param event the event
 * @return APP_SUCCESS if event is QUIT_EVENT, APP_CONTINUE otherwise,
 */
template<class T>
inline AppResult DefaultEventClass(T* state, const SDL_Event& event)
{
  if (event.type == SDL_EVENT_QUIT) return APP_SUCCESS;
  return APP_CONTINUE;
}

/**
 * @{
 * Iterate the state
 *
 * @tparam T the state class
 * @param state the state
 * @param event the event to handle
 * @return the app status
 */
template<class T>
inline AppResult EventClass(T* state, const SDL_Event& event)
{
  try {
    return DefaultEventClass(state, event);
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
  }
  return APP_FAILURE;
}

template<HasEventFunction T>
inline AppResult EventClass(T* state, const SDL_Event& event)
{
  try {
    return state->Event(event);
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
  }
  return APP_FAILURE;
}

/// @}

/**
 * Destroy state with delete;
 *
 * @tparam T
 * @param state
 */
template<class T>
inline void DefaultClassDestroy(T* state)
{
  delete state;
}

/// @private
template<class T>
concept HasQuitFunction =
  requires(T* state, AppResult result) { T::Quit(state, result); };

/**
 * @{
 * Destroy state with given result
 *
 * This is responsible to destroy and deallocate the state. It tries to call
 * T::Quit() if available and delegates to it the duty of deleting. Otherwise it
 * calls delete directly.
 *
 * @tparam T the state class.
 * @param state the state to destroy.
 * @param result the app result.
 */
template<class T>
inline void QuitClass(T* state, AppResult result)
{
  DefaultClassDestroy(state);
}

template<HasQuitFunction T>
inline void QuitClass(T* state, AppResult result)
{
  T::Quit(state, result);
}
/// @}

/// @}

} // namespace SDL

#endif /* SDL3PP_INIT_H_ */
