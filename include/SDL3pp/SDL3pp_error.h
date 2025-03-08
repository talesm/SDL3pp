#ifndef SDL3PP_ERROR_H_
#define SDL3PP_ERROR_H_

#include <SDL3/SDL_error.h>

namespace SDL {

/**
 * @defgroup CategoryError Category Error
 *
 * Simple error message routines for SDL.
 *
 * Most apps will interface with these APIs in exactly one function: when
 * almost any SDL function call reports failure, you can get a human-readable
 * string of the problem from GetError().
 *
 * These strings are maintained per-thread, and apps are welcome to set their
 * own errors, which is popular when building libraries on top of SDL for
 * other apps to consume. These strings are set by calling SDL_SetError().
 *
 * A common usage pattern is to have a function that returns true for success
 * and false for failure, and do this when something fails:
 *
 * ```c
 * if (something_went_wrong) {
 *    return SetError("The thing broke in this specific way: %d", errcode);
 * }
 * ```
 *
 * It's also common to just return `false` in this case if the failing thing
 * is known to call SetError(), so errors simply propagate through.
 *
 * @{
 */

/**
 * @brief Retrieve a message about the last error that occurred on the current
 * thread.
 *
 * It is possible for multiple errors to occur before calling GetError().
 * Only the last error is returned.
 *
 * The message is only applicable when an SDL function has signaled an error.
 * You must check the return values of SDL function calls to determine when to
 * appropriately call GetError(). You should *not* use the results of
 * GetError() to decide if an error has occurred! Sometimes SDL will set
 * an error string even when reporting success.
 *
 * SDL will *not* clear the error string for successful API calls. You *must*
 * check return values for failure cases before you can assume the error
 * string applies.
 *
 * Error strings are set per-thread, so an error set in a different thread
 * will not interfere with the current thread's operation.
 *
 * The returned value is a thread-local string which will remain valid until
 * the current thread's error string is changed. The caller should make a copy
 * if the value is needed after the next SDL API call.
 *
 * @return a message with information about the specific error that occurred,
 *          or an empty string if there hasn't been an error message set since
 *          the last call to ClearError().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @sa ClearError()
 */
inline const char* GetError() { return SDL_GetError(); }

/**
 * @brief Clear any previous error message for this thread.
 *
 * @returns true.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @sa GetError()
 */
inline bool ClearError() { return SDL_ClearError(); }

/** @} */

} // namespace SDL

#endif /* SDL3PP_ERROR_H_ */
