#ifndef SDL3PP_MAIN_H_
#define SDL3PP_MAIN_H_

#include <SDL3/SDL_main.h>
#include "SDL3pp.h"

namespace SDL {

/**
 * @defgroup CategoryMain Category Main
 *
 * Redefine main() if necessary so that it is called by SDL.
 *
 * In order to make this consistent on all platforms, the application's main()
 * should look like this:
 *
 * ```c
 * #include <SDL3/SDL.h>
 * #include <SDL3/SDL_main.h>
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
 * SDL_main.h is a "single-header library," which is to say that including
 * this header inserts code into your program, and you should only include it
 * once in most cases. SDL.h does not include this header automatically.
 *
 * For more information, see:
 *
 * https://wiki.libsdl.org/SDL3/README-main-functions
 *
 * @{
 */

/**
 * Inform SDL that the app is providing an entry point instead of SDL.
 *
 * SDL does not define this macro, but will check if it is defined when
 * including `SDL_main.h`. If defined, SDL will expect the app to provide the
 * proper entry point for the platform, and all the other magic details
 * needed, like manually calling SetMainReady.
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
 * The prototype for the application's main() function
 *
 * @param argc an ANSI-C style main function's argc.
 * @param argv an ANSI-C style main function's argv.
 * @returns an ANSI-C main return code; generally 0 is considered successful
 *          program completion, and small non-zero values are considered
 *          errors.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using main_func = SDL_main_func;

/**
 * Circumvent failure of Init() when not using SDL_main() as an entry
 * point.
 *
 * This function is defined in SDL_main.h, along with the preprocessor rule to
 * redefine main() as SDL_main(). Thus to ensure that your main() function
 * will not be changed it is necessary to define SDL3PP_MAIN_HANDLED before
 * including SDL.h.
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
 * @param argc the argc parameter from the application's main() function, or 0
 *             if the platform's main-equivalent has no argc.
 * @param argv the argv parameter from the application's main() function, or
 *             nullptr if the platform's main-equivalent has no argv.
 * @param mainFunction your SDL app's C-style main(). NOT the function you're
 *                     calling this from! Its name doesn't matter; it doesn't
 *                     literally have to be `main`.
 * @param reserved should be nullptr (reserved for future use, will probably be
 *                 platform-specific then).
 * @returns the return value from mainFunction: 0 on success, otherwise
 *          failure; GetError() might have more information on the
 *          failure.
 *
 * @threadsafety Generally this is called once, near startup, from the
 *               process's initial thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int RunApp(int argc,
                  char* argv[],
                  main_func mainFunction,
                  void* reserved)
{
  return SDL_RunApp(argc, argv[], mainFunction, reserved);
}

/**
 * An entry point for SDL's use in SDL3PP_MAIN_USE_CALLBACKS.
 *
 * Generally, you should not call this function directly. This only exists to
 * hand off work into SDL as soon as possible, where it has a lot more control
 * and functionality available, and make the inline code in SDL_main.h as
 * small as possible.
 *
 * Not all platforms use this, it's actual use is hidden in a magic
 * header-only library, and you should not call this directly unless you
 * _really_ know what you're doing.
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
    argc, argv[], appinit, appiter, appevent, appquit);
}

/**
 * Callback from the application to let the suspend continue.
 *
 * This function is only needed for Xbox GDK support; all other platforms will
 * do nothing and set an "unsupported" error message.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void GDKSuspendComplete() { SDL_GDKSuspendComplete(); }

/// @}

} // namespace SDL

#endif /* SDL3PP_MAIN_H_ */
