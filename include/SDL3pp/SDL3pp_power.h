#ifndef SDL3PP_POWER_H_
#define SDL3PP_POWER_H_

#include <SDL3/SDL_power.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryPower Power Management Status
 *
 * SDL power management routines.
 *
 * There is a single function in this category: GetPowerInfo().
 *
 * This function is useful for games on the go. This allows an app to know if
 * it's running on a draining battery, which can be useful if the app wants to
 * reduce processing, or perhaps framerate, to extend the duration of the
 * battery's charge. Perhaps the app just wants to show a battery meter when
 * fullscreen, or alert the user when the power is getting extremely low, so
 * they can save their game.
 *
 * @{
 */

/**
 * @name Power States
 * @{
 */

/**
 * The basic state for the system's power supply.
 *
 * These are results returned by GetPowerInfo().
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PowerState = SDL_PowerState;

constexpr PowerState POWERSTATE_ERROR =
  SDL_POWERSTATE_ERROR; ///< error determining power status

constexpr PowerState POWERSTATE_UNKNOWN =
  SDL_POWERSTATE_UNKNOWN; ///< cannot determine power status

constexpr PowerState POWERSTATE_ON_BATTERY =
  SDL_POWERSTATE_ON_BATTERY; ///< Not plugged in, running on the battery

constexpr PowerState POWERSTATE_NO_BATTERY =
  SDL_POWERSTATE_NO_BATTERY; ///< Plugged in, no battery available

constexpr PowerState POWERSTATE_CHARGING =
  SDL_POWERSTATE_CHARGING; ///< Plugged in, charging battery

constexpr PowerState POWERSTATE_CHARGED =
  SDL_POWERSTATE_CHARGED; ///< Plugged in, battery charged

/// @}

/**
 * Get the current power supply details.
 *
 * You should never take a battery status as absolute truth. Batteries
 * (especially failing batteries) are delicate hardware, and the values reported
 * here are best estimates based on what that hardware reports. It's not
 * uncommon for older batteries to lose stored power much faster than it
 * reports, or completely drain when reporting it has 20 percent left, etc.
 *
 * Battery status can change at any time; if you are concerned with power state,
 * you should call this function frequently, and perhaps ignore changes until
 * they seem to be stable for a few seconds.
 *
 * It's possible a platform can only report battery percentage or time left but
 * not both.
 *
 * On some platforms, retrieving power supply details might be expensive. If you
 * want to display continuous status you could call this function every minute
 * or so.
 *
 * @param seconds a pointer filled in with the seconds of battery life left, or
 *                nullptr to ignore. This will be filled in with -1 if we can't
 *                determine a value or there is no battery.
 * @param percent a pointer filled in with the percentage of battery life left,
 *                between 0 and 100, or nullptr to ignore. This will be filled
 *                in with -1 we can't determine a value or there is no battery.
 * @returns the current battery state or `POWERSTATE_ERROR` on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PowerState GetPowerInfo(int* seconds, int* percent)
{
  return SDL_GetPowerInfo(seconds, percent);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_POWER_H_ */
