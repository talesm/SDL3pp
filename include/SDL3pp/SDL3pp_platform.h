#ifndef SDL3PP_PLATFORM_H_
#define SDL3PP_PLATFORM_H_

#include <SDL3/SDL_platform.h>

namespace SDL {

/**
 * @defgroup CategoryPlatform Platform Detection
 *
 * SDL provides a means to identify the app's platform, both at compile time and
 * runtime.
 *
 * For sanity, we don't wrap the macros, so use the regular SDL_* macros as
 * documented.
 *
 * @{
 */

/**
 * Get the name of the platform.
 *
 * Here are the names returned for some (but not all) supported platforms:
 *
 * - "Windows"
 * - "macOS"
 * - "Linux"
 * - "iOS"
 * - "Android"
 *
 * @returns the name of the platform. If the correct platform name is not
 *          available, returns a string beginning with the text "Unknown".
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetPlatform() { return SDL_GetPlatform(); }

#ifdef SDL3PP_DOC

/**
 * A preprocessor macro that is only defined if compiling for AIX.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_AIX 1

/**
 * A preprocessor macro that is only defined if compiling for BSDi
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_BSDI 1

/**
 * A preprocessor macro that is only defined if compiling for FreeBSD.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_FREEBSD 1

/**
 * A preprocessor macro that is only defined if compiling for HP-UX.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_HPUX 1

/**
 * A preprocessor macro that is only defined if compiling for IRIX.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_IRIX 1

/**
 * A preprocessor macro that is only defined if compiling for Linux.
 *
 * Note that Android, although ostensibly a Linux-based system, will not
 * define this. It defines SDL_PLATFORM_ANDROID instead.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_LINUX 1

/**
 * A preprocessor macro that is only defined if compiling for Android.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_ANDROID 1

/**
 * A preprocessor macro that is only defined if compiling for a Unix-like
 * system.
 *
 * Other platforms, like Linux, might define this in addition to their primary
 * define.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_UNIX 1

/**
 * A preprocessor macro that is only defined if compiling for Apple platforms.
 *
 * iOS, macOS, etc will additionally define a more specific platform macro.
 *
 * \since This macro is available since SDL 3.2.0.
 *
 * \sa SDL_PLATFORM_MACOS
 * \sa SDL_PLATFORM_IOS
 * \sa SDL_PLATFORM_TVOS
 * \sa SDL_PLATFORM_VISIONOS
 */
#define SDL_PLATFORM_APPLE 1

/**
 * A preprocessor macro that is only defined if compiling for tvOS.
 *
 * \since This macro is available since SDL 3.2.0.
 *
 * \sa SDL_PLATFORM_APPLE
 */
#define SDL_PLATFORM_TVOS 1

/**
 * A preprocessor macro that is only defined if compiling for VisionOS.
 *
 * \since This macro is available since SDL 3.2.0.
 *
 * \sa SDL_PLATFORM_APPLE
 */
#define SDL_PLATFORM_VISIONOS 1

/**
 * A preprocessor macro that is only defined if compiling for iOS.
 *
 * \since This macro is available since SDL 3.2.0.
 *
 * \sa SDL_PLATFORM_APPLE
 */
#define SDL_PLATFORM_IOS 1
/**
 * A preprocessor macro that is only defined if compiling for macOS.
 *
 * \since This macro is available since SDL 3.2.0.
 *
 * \sa SDL_PLATFORM_APPLE
 */
#define SDL_PLATFORM_MACOS 1

/**
 * A preprocessor macro that is only defined if compiling for Emscripten.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_EMSCRIPTEN 1

/**
 * A preprocessor macro that is only defined if compiling for NetBSD.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_NETBSD 1

/**
 * A preprocessor macro that is only defined if compiling for OpenBSD.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_OPENBSD 1

/**
 * A preprocessor macro that is only defined if compiling for OS/2.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_OS2 1

/**
 * A preprocessor macro that is only defined if compiling for Tru64 (OSF/1).
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_OSF 1

/**
 * A preprocessor macro that is only defined if compiling for QNX Neutrino.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_QNXNTO 1

/**
 * A preprocessor macro that is only defined if compiling for RISC OS.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_RISCOS 1

/**
 * A preprocessor macro that is only defined if compiling for SunOS/Solaris.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_SOLARIS 1

/**
 * A preprocessor macro that is only defined if compiling for Cygwin.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_CYGWIN 1

/**
 * A preprocessor macro that is only defined if compiling for Windows.
 *
 * This also covers several other platforms, like Microsoft GDK, Xbox, WinRT,
 * etc. Each will have their own more-specific platform macros, too.
 *
 * \since This macro is available since SDL 3.2.0.
 *
 * \sa SDL_PLATFORM_WIN32
 * \sa SDL_PLATFORM_XBOXONE
 * \sa SDL_PLATFORM_XBOXSERIES
 * \sa SDL_PLATFORM_WINGDK
 * \sa SDL_PLATFORM_GDK
 */
#define SDL_PLATFORM_WINDOWS 1

/**
 * A preprocessor macro that defined to 1 if compiling for Windows Phone.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_WINAPI_FAMILY_PHONE 1

/**
 * A preprocessor macro that is only defined if compiling for Microsoft GDK
 * for Windows.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_WINGDK 1

/**
 * A preprocessor macro that is only defined if compiling for Xbox Series.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_XBOXSERIES 1

/**
 * A preprocessor macro that is only defined if compiling for desktop Windows.
 *
 * Despite the "32", this also covers 64-bit Windows; as an informal
 * convention, its system layer tends to still be referred to as "the Win32
 * API."
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_WIN32 1

/**
 * A preprocessor macro that is only defined if compiling for Microsoft GDK on
 * any platform.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_GDK 1

/**
 * A preprocessor macro that is only defined if compiling for Sony PSP.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_PSP 1

/**
 * A preprocessor macro that is only defined if compiling for Sony PlayStation
 * 2.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_PS2 1

/**
 * A preprocessor macro that is only defined if compiling for Sony Vita.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_VITA 1

/**
 * A preprocessor macro that is only defined if compiling for Nintendo 3DS.
 *
 * \since This macro is available since SDL 3.2.0.
 */
#define SDL_PLATFORM_3DS 1

#endif // SDL3PP_DOC

/// @}

} // namespace SDL

#endif /* SDL3PP_PLATFORM_H_ */
