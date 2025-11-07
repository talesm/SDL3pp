#ifndef SDL3PP_ERROR_H_
#define SDL3PP_ERROR_H_

#include <exception>
#include <format>
#include <SDL3/SDL_error.h>
#include "SDL3pp_strings.h"

namespace SDL {

/**
 * @defgroup CategoryError CategoryError
 *
 * Simple error message routines for SDL.
 *
 * Most apps will interface with these APIs in exactly one function: when almost
 * any SDL function call reports failure, you can get a human-readable string of
 * the problem from GetError().
 *
 * These strings are maintained per-thread, and apps are welcome to set their
 * own errors, which is popular when building libraries on top of SDL for other
 * apps to consume. These strings are set by calling SetErrorUnformatted().
 *
 * A common usage pattern is to have a function that returns true for success
 * and false for failure, and do this when something fails:
 *
 * ```c
 * if (something_went_wrong) {
 *    return SetErrorUnformatted("The thing broke in this specific way: %d",
 * errcode);
 * }
 * ```
 *
 * It's also common to just return `false` in this case if the failing thing is
 * known to call SetErrorUnformatted(), so errors simply propagate through.
 *
 * @{
 */

/**
 * Set the SDL error message for the current thread.
 *
 * Calling this function will replace any previous error message that was set.
 *
 * This function always returns false, since SDL frequently uses false to
 * signify a failing result, leading to this idiom:
 *
 * ```c
 * if (error_code) {
 *     return SetErrorUnformatted("This operation has failed: %d", error_code);
 * }
 * ```
 *
 * @param fmt a printf()-style message format string.
 * @param ... additional parameters matching % tokens in the `fmt` string, if
 *            any.
 * @returns false.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ClearError
 * @sa GetError
 * @sa SDL_SetErrorV
 */
inline bool SetErrorUnformatted(StringParam message)
{
  return SDL_SetError(message);
}

template<class... ARGS>
inline bool SetError(std::string_view fmt, ARGS... args)
{
  static_assert(false, "Not implemented");
}

/**
 * Set an error indicating that memory allocation failed.
 *
 * This function does not do any memory allocation.
 *
 * @returns false.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool OutOfMemory() { return SDL_OutOfMemory(); }

/**
 * Retrieve a message about the last error that occurred on the current thread.
 *
 * It is possible for multiple errors to occur before calling GetError(). Only
 * the last error is returned.
 *
 * The message is only applicable when an SDL function has signaled an error.
 * You must check the return values of SDL function calls to determine when to
 * appropriately call GetError(). You should *not* use the results of GetError()
 * to decide if an error has occurred! Sometimes SDL will set an error string
 * even when reporting success.
 *
 * SDL will *not* clear the error string for successful API calls. You *must*
 * check return values for failure cases before you can assume the error string
 * applies.
 *
 * Error strings are set per-thread, so an error set in a different thread will
 * not interfere with the current thread's operation.
 *
 * The returned value is a thread-local string which will remain valid until the
 * current thread's error string is changed. The caller should make a copy if
 * the value is needed after the next SDL API call.
 *
 * @returns a message with information about the specific error that occurred,
 *          or an empty string if there hasn't been an error message set since
 *          the last call to ClearError().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ClearError
 * @sa SetErrorUnformatted
 */
inline const char* GetError() { return SDL_GetError(); }

struct Error : std::exception
{
  std::string m_message;

  /// Default ctor.
  Error()
    : m_message(SDL_GetError())
  {
  }

  /// Default ctor.
  Error(std::string message)
    : m_message(std::move(message))
  {
  }

  /// Returns the explanatory string.
  const char* what() const { return GetError(); }

  constexpr const std::string& str() const
  {
    static_assert(false, "Not implemented");
  }
};

constexpr void CheckError(bool result)
{
  static_assert(false, "Not implemented");
}

template<class T>
constexpr T CheckError(T result)
{
  static_assert(false, "Not implemented");
}

template<class T>
constexpr T CheckError(T result, T invalidValue)
{
  static_assert(false, "Not implemented");
}

/**
 * Clear any previous error message for this thread.
 *
 * @returns true.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetError
 * @sa SetErrorUnformatted
 */
inline bool ClearError() { return SDL_ClearError(); }

/**
 * A macro to standardize error reporting on unsupported operations.
 *
 * This simply calls SetErrorUnformatted() with a standardized error string, for
 * convenience, consistency, and clarity.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_Unsupported() SDL_SetError("That operation is not supported")

/**
 * A macro to standardize error reporting on unsupported operations.
 *
 * This simply calls SetErrorUnformatted() with a standardized error string, for
 * convenience, consistency, and clarity.
 *
 * A common usage pattern inside SDL is this:
 *
 * ```c
 * bool MyFunction(const char *str) {
 *     if (!str) {
 *         return SDL_InvalidParamError("str");  // returns false.
 *     }
 *     DoSomething(str);
 *     return true;
 * }
 * ```
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_InvalidParamError(param)                                           \
  SDL_SetError("Parameter '%s' is invalid", (param))

/// @}

} // namespace SDL

#endif /* SDL3PP_ERROR_H_ */
