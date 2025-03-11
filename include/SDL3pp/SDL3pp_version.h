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
