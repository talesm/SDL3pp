#ifndef SDL3PP_HINTS_H_
#define SDL3PP_HINTS_H_

#include <SDL3/SDL_hints.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryHints Configuration Variables
 *
 * This file contains functions to set and get configuration hints, as well as
 * listing each of them alphabetically.
 *
 * The convention for naming hints is SDL_HINT_X, where "SDL_X" is the
 * environment variable that can be used to override the default.
 *
 * In general these hints are just that - they may or may not be supported or
 * applicable on any given platform, but they provide a way for an application
 * or user to give the library a hint as to how they would like the library to
 * work.
 *
 * @{
 */

/**
 * An enumeration of hint priorities.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using HintPriority = SDL_HintPriority;

constexpr HintPriority HINT_DEFAULT = SDL_HINT_DEFAULT;

constexpr HintPriority HINT_NORMAL = SDL_HINT_NORMAL;

constexpr HintPriority HINT_OVERRIDE = SDL_HINT_OVERRIDE;

/**
 * Set a hint with a specific priority.
 *
 * The priority controls the behavior when setting a hint that already has a
 * value. Hints will replace existing hints of their priority and lower.
 * Environment variables are considered to have override priority.
 *
 * @param name the hint to set.
 * @param value the value of the hint variable.
 * @param priority the SDL_HintPriority level for the hint.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetHint
 * @sa SDL_ResetHint
 * @sa SDL_SetHint
 */
inline bool SetHintWithPriority(StringParam name,
                                StringParam value,
                                HintPriority priority)
{
  return SDL_SetHintWithPriority(name, value, priority);
}

/**
 * Set a hint with normal priority.
 *
 * Hints will not be set if there is an existing override hint or environment
 * variable that takes precedence. You can use SDL_SetHintWithPriority() to
 * set the hint with override priority instead.
 *
 * @param name the hint to set.
 * @param value the value of the hint variable.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetHint
 * @sa SDL_ResetHint
 * @sa SDL_SetHintWithPriority
 */
inline bool SetHint(StringParam name, StringParam value)
{
  return SDL_SetHint(name, value);
}

/**
 * Reset a hint to the default value.
 *
 * This will reset a hint to the value of the environment variable, or NULL if
 * the environment isn't set. Callbacks will be called normally with this
 * change.
 *
 * @param name the hint to set.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_SetHint
 * @sa SDL_ResetHints
 */
inline bool ResetHint(StringParam name) { return SDL_ResetHint(name); }

/**
 * Reset all hints to the default values.
 *
 * This will reset all hints to the value of the associated environment
 * variable, or NULL if the environment isn't set. Callbacks will be called
 * normally with this change.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_ResetHint
 */
inline void ResetHints() { return SDL_ResetHints(); }

/**
 * Get the value of a hint.
 *
 * @param name the hint to query.
 * @returns the string value of a hint or NULL if the hint isn't set.
 *
 * @threadsafety It is safe to call this function from any thread, however the
 *               return value only remains valid until the hint is changed; if
 *               another thread might do so, the app should supply locks
 *               and/or make a copy of the string. Note that using a hint
 *               callback instead is always thread-safe, as SDL holds a lock
 *               on the thread subsystem during the callback.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_SetHint
 * @sa SDL_SetHintWithPriority
 */
inline const char* GetHint(StringParam name) { return SDL_GetHint(name); }

/**
 * Get the boolean value of a hint variable.
 *
 * @param name the name of the hint to get the boolean value from.
 * @param default_value the value to return if the hint does not exist.
 * @returns the boolean value of a hint or the provided default value if the
 *          hint does not exist.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetHint
 * @sa SDL_SetHint
 */
inline bool GetHintBoolean(StringParam name, bool default_value)
{
  return SDL_GetHintBoolean(name, default_value);
}

/**
 * A callback used to send notifications of hint value changes.
 *
 * This is called an initial time during SDL_AddHintCallback with the hint's
 * current value, and then again each time the hint's value changes.
 *
 * @param userdata what was passed as `userdata` to SDL_AddHintCallback().
 * @param name what was passed as `name` to SDL_AddHintCallback().
 * @param oldValue the previous hint value.
 * @param newValue the new value hint is to be set to.
 *
 * @threadsafety This callback is fired from whatever thread is setting a new
 *               hint value. SDL holds a lock on the hint subsystem when
 *               calling this callback.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SDL_AddHintCallback
 */
using HintCallback = SDL_HintCallback;

/**
 * Add a function to watch a particular hint.
 *
 * The callback function is called _during_ this function, to provide it an
 * initial value, and again each time the hint's value changes.
 *
 * @param name the hint to watch.
 * @param callback An SDL_HintCallback function that will be called when the
 *                 hint value changes.
 * @param userdata a pointer to pass to the callback function.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_RemoveHintCallback
 */
inline bool AddHintCallback(StringParam name,
                            HintCallback callback,
                            void* userdata)
{
  return SDL_AddHintCallback(name, callback, userdata);
}

/**
 * Remove a function watching a particular hint.
 *
 * @param name the hint being watched.
 * @param callback an SDL_HintCallback function that will be called when the
 *                 hint value changes.
 * @param userdata a pointer being passed to the callback function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_AddHintCallback
 */
inline void RemoveHintCallback(StringParam name,
                               HintCallback callback,
                               void* userdata)
{
  return SDL_RemoveHintCallback(name, callback, userdata);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_HINTS_H_ */
