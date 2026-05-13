#ifndef SDL3PP_MAIN_H_
#define SDL3PP_MAIN_H_

#ifdef SDL3PP_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#endif // SDL3PP_MAIN_HANDLED

#ifdef SDL3PP_MAIN_USE_CALLBACKS
#define SDL_MAIN_USE_CALLBACKS
#endif // SDL3PP_MAIN_USE_CALLBACKS

#ifdef SDL3PP_MAIN_USE_CLASS_CALLBACKS
#define SDL_MAIN_USE_CALLBACKS
#endif // SDL3PP_MAIN_USE_CLASS_CALLBACKS

#include <SDL3/SDL_main.h>
#include "SDL3pp.h"

namespace SDL {

/**
 * @defgroup CategoryMain Application entry points
 *
 * Redefine main() if necessary so that it is called by SDL.
 *
 * In order to make this consistent on all platforms, the application's main()
 * should look like this:
 *
 * ```cpp
 * #include <SDL3pp/SDL3pp.h>
 * #include <SDL3pp/SDL3pp_main.h>
 *
 * int main(int argc, char *argv[])
 * {
 * }
 * ```
 *
 * SDL will take care of platform specific details on how it gets called.
 *
 * This is also where an app can be configured to use the main callbacks, via
 * the SDL3PP_MAIN_USE_CALLBACKS macro.
 *
 * SDL_main.h is a "single-header library," which is to say that including this
 * header inserts code into your program, and you should only include it once in
 * most cases. SDL.h does not include this header automatically.
 *
 * For more information, see:
 *
 * https://wiki.libsdl.org/SDL3/README-main-functions
 *
 * @{
 */

#ifdef SDL3PP_DOC

/**
 * Inform SDL that the app is providing an entry point instead of SDL.
 *
 * SDL does not define this macro, but will check if it is defined when
 * including `SDL_main.h`. If defined, SDL will expect the app to provide the
 * proper entry point for the platform, and all the other magic details needed,
 * like manually calling SetMainReady.
 *
 * Please see [README-main-functions](README-main-functions), (or
 * docs/README-main-functions.md in the source tree) for a more detailed
 * explanation.
 *
 * @since This macro is used by the headers since SDL 3.2.0.
 */
#define SDL3PP_MAIN_HANDLED SDL_MAIN_HANDLED

/**
 * Inform SDL to use the main callbacks instead of main.
 *
 * SDL does not define this macro, but will check if it is defined when
 * including `SDL_main.h`. If defined, SDL will expect the app to provide
 * several functions: SDL_AppInit, SDL_AppEvent, SDL_AppIterate, and
 * SDL_AppQuit. The app should not provide a `main` function in this case, and
 * doing so will likely cause the build to fail.
 *
 * Please see [README-main-functions](README-main-functions), (or
 * docs/README-main-functions.md in the source tree) for a more detailed
 * explanation.
 *
 * @since This macro is used by the headers since SDL 3.2.0.
 *
 * @sa SDL_AppInit
 * @sa SDL_AppEvent
 * @sa SDL_AppIterate
 * @sa SDL_AppQuit
 */
#define SDL3PP_MAIN_USE_CALLBACKS SDL_MAIN_USE_CALLBACKS

/**
 * Inform SDL to use the main class callbacks instead of main.
 *
 * SDL does not define this macro, but will check if it is defined when
 * including `SDL_main.h`. If defined, SDL will expect the app to provide
 * a function: SDL_AppCreate. The app should not provide a `main` function in
 * this case, and doing so will likely cause the build to fail.
 *
 * @todo
 *
 * @since This macro is used by the headers since SDLpp 0.11.0.
 *
 * @sa SDL_AppCreate
 * @sa SDL3PP_MAIN_USE_CALLBACKS
 */
#define SDL3PP_MAIN_USE_CLASS_CALLBACKS 1

#endif // SDL3PP_DOC

/**
 * The prototype for the application's main() function
 *
 * @param argc an ANSI-C style main function's argc.
 * @param argv an ANSI-C style main function's argv.
 * @returns an ANSI-C main return code; generally 0 is considered successful
 *          program completion, and small non-zero values are considered errors.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using main_func = int(SDLCALL*)(int argc, char* argv[]);

/**
 * Circumvent failure of Init() when not using SDL_main() as an entry point.
 *
 * This function is defined in SDL_main.h, along with the preprocessor rule to
 * redefine main() as SDL_main(). Thus to ensure that your main() function will
 * not be changed it is necessary to define SDL3PP_MAIN_HANDLED before including
 * SDL.h.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Init
 */
inline void SetMainReady() { SDL_SetMainReady(); }

/**
 * Initializes and launches an SDL application, by doing platform-specific
 * initialization before calling your mainFunction and cleanups after it
 * returns, if that is needed for a specific platform, otherwise it just calls
 * mainFunction.
 *
 * You can use this if you want to use your own main() implementation without
 * using SDL_main (like when using SDL3PP_MAIN_HANDLED). When using this, you do
 * *not* need SetMainReady().
 *
 * If `argv` is nullptr, SDL will provide command line arguments, either by
 * querying the OS for them if possible, or supplying a filler array if not.
 *
 * @param argc the argc parameter from the application's main() function, or 0
 *             if the platform's main-equivalent has no argc.
 * @param argv the argv parameter from the application's main() function, or
 *             nullptr if the platform's main-equivalent has no argv.
 * @param mainFunction your SDL app's C-style main(). NOT the function you're
 *                     calling this from! Its name doesn't matter; it doesn't
 *                     literally have to be `main`.
 * @param reserved should be nullptr (reserved for future use, will probably be
 *                 platform-specific then).
 * @returns the return value from mainFunction: 0 on success, otherwise failure;
 *          GetError() might have more information on the failure.
 *
 * @threadsafety Generally this is called once, near startup, from the process's
 *               initial thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int RunApp(int argc,
                  char* argv[],
                  main_func mainFunction,
                  void* reserved)
{
  return SDL_RunApp(argc, argv, mainFunction, reserved);
}

/**
 * An entry point for SDL's use in SDL3PP_MAIN_USE_CALLBACKS.
 *
 * Generally, you should not call this function directly. This only exists to
 * hand off work into SDL as soon as possible, where it has a lot more control
 * and functionality available, and make the inline code in SDL_main.h as small
 * as possible.
 *
 * Not all platforms use this, it's actual use is hidden in a magic header-only
 * library, and you should not call this directly unless you _really_ know what
 * you're doing.
 *
 * @param argc standard Unix main argc.
 * @param argv standard Unix main argv.
 * @param appinit the application's SDL_AppInit function.
 * @param appiter the application's SDL_AppIterate function.
 * @param appevent the application's SDL_AppEvent function.
 * @param appquit the application's SDL_AppQuit function.
 * @returns standard Unix main return value.
 *
 * @threadsafety It is not safe to call this anywhere except as the only
 *               function call in SDL_main.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int EnterAppMainCallbacks(int argc,
                                 char* argv[],
                                 AppInit_func appinit,
                                 AppIterate_func appiter,
                                 AppEvent_func appevent,
                                 AppQuit_func appquit)
{
  return SDL_EnterAppMainCallbacks(
    argc, argv, appinit, appiter, appevent, appquit);
}

#if defined(SDL_PLATFORM_WINDOWS)

/**
 * Register a win32 window class for SDL's use.
 *
 * This can be called to set the application window class at startup. It is safe
 * to call this multiple times, as long as every call is eventually paired with
 * a call to UnregisterApp, but a second registration attempt while a previous
 * registration is still active will be ignored, other than to increment a
 * counter.
 *
 * Most applications do not need to, and should not, call this directly; SDL
 * will call it when initializing the video subsystem.
 *
 * @param name the window class name, in UTF-8 encoding. If nullptr, SDL
 *             currently uses "SDL_app" but this isn't guaranteed.
 * @param style the value to use in WNDCLASSEX::style. If `name` is nullptr, SDL
 *              currently uses `(CS_BYTEALIGNCLIENT | CS_OWNDC)` regardless of
 *              what is specified here.
 * @param hInst the HINSTANCE to use in WNDCLASSEX::hInstance. If zero, SDL will
 *              use `GetModuleHandle(nullptr)` instead.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool RegisterApp(StringParam name, Uint32 style, void* hInst)
{
  return SDL_RegisterApp(name, style, hInst);
}

/**
 * Deregister the win32 window class from an RegisterApp call.
 *
 * This can be called to undo the effects of RegisterApp.
 *
 * Most applications do not need to, and should not, call this directly; SDL
 * will call it when deinitializing the video subsystem.
 *
 * It is safe to call this multiple times, as long as every call is eventually
 * paired with a prior call to RegisterApp. The window class will only be
 * deregistered when the registration counter in RegisterApp decrements to zero
 * through calls to this function.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void UnregisterApp() { SDL_UnregisterApp(); }

#endif /* defined(SDL_PLATFORM_WINDOWS) */

/**
 * Callback from the application to let the suspend continue.
 *
 * This should be called from an event watch in response to an
 * `EVENT_DID_ENTER_BACKGROUND` event.
 *
 * When using SDL_Render, your event watch should be added _after_ creating the
 * `Renderer`; this allows the timing of the D3D12 command queue suspension to
 * execute in the correct order.
 *
 * When using SDL_GPU, this should be called after calling
 * GPUDevice.GDKSuspendGPU.
 *
 * If you're writing your own D3D12 renderer, this should be called after
 * calling `ID3D12CommandQueue::SuspendX`.
 *
 * This function is only needed for Xbox GDK support; all other platforms will
 * do nothing and set an "unsupported" error message.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddEventWatch
 */
inline void GDKSuspendComplete() { SDL_GDKSuspendComplete(); }

/**
 * Base class for SDL3PP_MAIN_USE_CLASS_CALLBACKS main callback classes.
 *
 * This is an alternative that already includes exception handling and logging,
 * so you have sensible behavior for C++.
 *
 * Also, you can just override the functions you need, and rely on the default
 * behavior of the others, which is to continue iterating until a quit event is
 * received, at which point it will exit with success.
 */
struct AppInterface
{
  virtual ~AppInterface() = default;

  /**
   * Called to initialize the app. This is called once at startup.
   *
   * @return SDL::APP_CONTINUE to continue running the app, otherwise
   * SDL::APP_SUCCESS to exit with success and SDL::APP_FAILURE to exit with
   * failure.
   */
  virtual SDL::AppResult Init() { return SDL::APP_CONTINUE; }

  /**
   * Called to iterate the app.
   *
   * This is called repeatedly until it returns something other than
   * SDL::APP_CONTINUE.
   *
   * @return SDL::APP_CONTINUE to continue running the app, otherwise
   * SDL::APP_SUCCESS to exit with success and SDL::APP_FAILURE to exit with
   * failure.
   */
  virtual SDL::AppResult Iterate() { return SDL::APP_CONTINUE; }

  /**
   * Called to handle an event.
   *
   * This is called when an event is received.
   *
   * @param ev The event to handle.
   * @return SDL::APP_CONTINUE to continue running the app, otherwise
   * SDL::APP_SUCCESS to exit with success and SDL::APP_FAILURE to exit with
   * failure.
   */
  virtual SDL::AppResult Event(const SDL::Event& ev)
  {
    return ev.type == SDL_EVENT_QUIT ? SDL::APP_SUCCESS : SDL::APP_CONTINUE;
  }

  /**
   * Called to quit the app.
   *
   * This is called when the app is quitting.
   *
   * @param result The result of the app.
   */
  virtual void quit(SDL::AppResult result) {}
};

} // namespace SDL

/**
 * @addtogroup CategoryMain
 * @{
 */
/**
 * Prototype for the application's main class creation function.
 *
 * You need to implement this function if you want to use
 * SDL3PP_MAIN_USE_CLASS_CALLBACKS. It should return a pointer to an instance of
 * your main class, which should be a subclass of AppInterface. This function is
 * called by SDL to create the main class instance, and then SDL will call the
 * appropriate functions on that instance for initialization, iteration, event
 * handling, and quitting.
 *
 * You should not call this function directly; it is called by SDL3pp
 * internally.
 *
 * You may also use the macros SDL3PP_DEFINE_CALLBACKS or
 * SDL3PP_DEFINE_CLASS_CALLBACKS to define this function for you, if your main
 * class is compatible with the default implementations of the main callbacks.
 *
 * ```cpp
 * #define SDL3PP_MAIN_USE_CLASS_CALLBACKS
 * #include <SDL3pp/SDL3pp_main.h>
 * #include <SDL3pp/SDL3pp.h>
 *
 * class Main: public SDL::AppInterface {
 *   /// Game logic
 * }
 *
 * SDL::AppInterface *SDL_AppCreate(int argc, char *argv[]) {
 *   return new Main();
 * }
 * ```
 *
 * @param argc an ANSI-C style main function's argc.
 * @param argv an ANSI-C style main function's argv.
 * @returns a pointer to the application's main class instance, or nullptr to
 *          indicate early exit with APP_SUCCESS.
 * @throws any exception, which will be caught and logged by SDL, and cause
 *         the app to exit with APP_FAILURE.
 *
 * @since This function is available since SDL3pp 0.10.1.
 * @sa SDL3PP_DEFINE_CALLBACKS
 * @sa SDL3PP_DEFINE_CLASS_CALLBACKS
 */
extern "C" SDLMAIN_DECLSPEC SDL::AppInterface* SDLCALL
SDL_AppCreate(int argc, char* argv[]);

/// @}
namespace SDL {

#ifdef SDL3PP_MAIN_USE_CLASS_CALLBACKS

/// @private
extern "C" SDLMAIN_DECLSPEC SDL::AppResult SDL_AppInit(void** appstate,
                                                       int argc,
                                                       char* argv[])
{
  try {
    auto app = SDL_AppCreate(argc, argv);
    if (!app) return APP_SUCCESS;
    *appstate = app;
    app->Init();
    return APP_CONTINUE;
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(
      SDL3PP_APPCLASS_LOG_PRIORITY, "Critical error during app initialization");
  }
  return APP_FAILURE;
}

/// @private
extern "C" SDLMAIN_DECLSPEC SDL::AppResult SDL_AppIterate(void* appstate)
{
  try {
    return static_cast<AppInterface*>(appstate)->Iterate();
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(
      SDL3PP_APPCLASS_LOG_PRIORITY, "Critical error during app iteration");
  }
  return APP_FAILURE;
}

/// @private
extern "C" SDLMAIN_DECLSPEC SDL::AppResult SDL_AppEvent(void* appstate,
                                                        SDL::Event* event)
{
  try {
    return static_cast<AppInterface*>(appstate)->Event(*event);
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(
      SDL3PP_APPCLASS_LOG_PRIORITY, "Critical error during app event handling");
  }
  return APP_FAILURE;
}

/// @private
extern "C" SDLMAIN_DECLSPEC void SDL_AppQuit(void* appstate,
                                             SDL::AppResult result)
{
  try {
    auto app = static_cast<AppInterface*>(appstate);
    if (!app) return;
    app->quit(result);
    delete app;
  } catch (std::exception& e) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            e.what());
  } catch (...) {
    LOG_CATEGORY_APPLICATION.LogUnformatted(SDL3PP_APPCLASS_LOG_PRIORITY,
                                            "Critical error during app exit");
  }
}

/**
 * Use this to define the callbacks for given class
 * @param CLASS The class to wrap in callbacks.
 */
#define SDL3PP_DEFINE_CALLBACKS(CLASS)                                         \
  extern "C" SDLMAIN_DECLSPEC SDL::AppInterface* SDLCALL SDL_AppCreate(        \
    int, char*[])                                                              \
  {                                                                            \
    return new CLASS();                                                        \
  }

/**
 * Use this to define the callbacks for given class, with app metadata.
 * @param CLASS The class to wrap in callbacks.
 * @param INIT_FLAGS The flags to pass to SDL::Init when initializing the app.
 * @param APPNAME The application name to set in SDL_SetAppMetadata.
 * @param APPVERSION The application version to set in SDL_SetAppMetadata.
 * @param APPIDENTIFIER The application identifier to set in SDL_SetAppMetadata.
 */
#define SDL3PP_DEFINE_CLASS_CALLBACKS(                                         \
  CLASS, INIT_FLAGS, APPNAME, APPVERSION, APPIDENTIFIER)                       \
  extern "C" SDLMAIN_DECLSPEC SDL::AppInterface* SDLCALL SDL_AppCreate(        \
    int, char*[])                                                              \
  {                                                                            \
    SDL::SetAppMetadata(APPNAME, APPVERSION, APPIDENTIFIER);                   \
    SDL::Init(INIT_FLAGS);                                                     \
    return new CLASS();                                                        \
  }

#else // SDL3PP_MAIN_USE_CLASS_CALLBACKS

/**
 * Use this to define the callbacks for given class
 * @param CLASS The class to wrap in callbacks.
 */
#define SDL3PP_DEFINE_CALLBACKS(CLASS)                                         \
  static_assert(SDL::HasIterateFunction<CLASS>, "Main class not compatible");  \
  inline SDL::AppResult SDL_AppInit(void** appstate, int argc, char* argv[])   \
  {                                                                            \
    return SDL::InitClass(reinterpret_cast<CLASS**>(appstate),                 \
                          SDL::AppArgs{argv, size_t(argc)});                   \
  }                                                                            \
  inline SDL::AppResult SDL_AppIterate(void* appstate)                         \
  {                                                                            \
    return SDL::IterateClass(static_cast<CLASS*>(appstate));                   \
  }                                                                            \
  inline SDL::AppResult SDL_AppEvent(void* appstate, SDL::Event* event)        \
  {                                                                            \
    return SDL::EventClass(static_cast<CLASS*>(appstate), *event);             \
  }                                                                            \
  inline void SDL_AppQuit(void* appstate, SDL::AppResult result)               \
  {                                                                            \
    SDL::QuitClass(static_cast<CLASS*>(appstate), result);                     \
  }

#endif // SDL3PP_MAIN_USE_CLASS_CALLBACKS

/// @}

} // namespace SDL

#endif /* SDL3PP_MAIN_H_ */
