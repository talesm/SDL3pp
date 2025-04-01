#ifndef SDL3PP_VERSION_H_
#define SDL3PP_VERSION_H_

#include <SDL3/SDL_version.h>
#include "SDL3pp_error.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryVersion Querying SDL Version
 *
 * Functionality to query the current SDL version, both as headers the app was
 * compiled against, and a library the app is linked to.
 *
 * @{
 */

#ifdef SDL3PP_DOC

/**
 * The current major version of SDL headers.
 *
 * If this were SDL version 3.2.1, this value would be 3.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_MAJOR_VERSION 3

/**
 * The current minor version of the SDL headers.
 *
 * If this were SDL version 3.2.1, this value would be 2.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_MINOR_VERSION 2

/**
 * The current micro (or patchlevel) version of the SDL headers.
 *
 * If this were SDL version 3.2.1, this value would be 1.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_MICRO_VERSION 11

/**
 * This macro turns the version numbers into a numeric value.
 *
 * (1,2,3) becomes 1002003.
 *
 * @param major the major version number.
 * @param minor the minorversion number.
 * @param patch the patch version number.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_VERSIONNUM(major, minor, patch)                                    \
  ((major) * 1000000 + (minor) * 1000 + (patch))

/**
 * This macro extracts the major version from a version number
 *
 * 1002003 becomes 1.
 *
 * @param version the version number.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_VERSIONNUM_MAJOR(version) ((version) / 1000000)

/**
 * This macro extracts the minor version from a version number
 *
 * 1002003 becomes 2.
 *
 * @param version the version number.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_VERSIONNUM_MINOR(version) (((version) / 1000) % 1000)

/**
 * This macro extracts the micro version from a version number
 *
 * 1002003 becomes 3.
 *
 * @param version the version number.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_VERSIONNUM_MICRO(version) ((version) % 1000)

/**
 * This is the version number macro for the current SDL version.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_GetVersion
 */
#define SDL_VERSION                                                            \
  SDL_VERSIONNUM(SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION)

/**
 * This macro will evaluate to true if compiled with SDL at least X.Y.Z.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_VERSION_ATLEAST(X, Y, Z) (SDL_VERSION >= SDL_VERSIONNUM(X, Y, Z))

#endif // SDL3PP_DOC

/**
 * Get the version of SDL that is linked against your program.
 *
 * If you are linking to SDL dynamically, then it is possible that the current
 * version will be different than the version you compiled against. This
 * function returns the current version, while SDL_VERSION is the version you
 * compiled with.
 *
 * This function may be called safely at any time, even before SDL_Init().
 *
 * @returns the version of the linked library.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetRevision
 */
inline int GetVersion() { return SDL_GetVersion(); }

/**
 * Get the code revision of SDL that is linked against your program.
 *
 * This value is the revision of the code you are linked with and may be
 * different from the code you are compiling with, which is found in the
 * constant SDL_REVISION.
 *
 * The revision is arbitrary string (a hash value) uniquely identifying the
 * exact revision of the SDL library in use, and is only useful in comparing
 * against other revisions. It is NOT an incrementing number.
 *
 * If SDL wasn't built from a git repository with the appropriate tools, this
 * will return an empty string.
 *
 * You shouldn't use this function for anything but logging it for debugging
 * purposes. The string is not intended to be reliable in any way.
 *
 * @returns an arbitrary string, uniquely identifying the exact revision of
 *          the SDL library in use.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetVersion
 */
inline const char* GetRevision() { return SDL_GetRevision(); }

/// @}

} // namespace SDL

#endif /* SDL3PP_VERSION_H_ */
