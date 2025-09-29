#ifndef SDL3PP_PLATFORM_H_
#define SDL3PP_PLATFORM_H_

#include <SDL3/SDL_platform.h>

namespace SDL {

/**
 * @defgroup CategoryPlatform Category Platform
 *
 * SDL provides a means to identify the app's platform, both at compile time
 * and runtime.
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

/// @}

} // namespace SDL

#endif /* SDL3PP_PLATFORM_H_ */
