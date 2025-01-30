#ifndef SDL3PP_ERROR_HPP_
#define SDL3PP_ERROR_HPP_

#include <SDL3/SDL_error.h>

namespace SDL {

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

} // namespace SDL

#endif /* SDL3PP_ERROR_HPP_ */
