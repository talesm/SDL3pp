#ifndef SDL3PP_ASSERT_H_
#define SDL3PP_ASSERT_H_

#include <SDL3/SDL_assert.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryAssert Assertions
 *
 * A helpful assertion macro!
 *
 * SDL assertions operate like your usual `assert` macro, but with some added
 * features:
 *
 * - It uses a trick with the `sizeof` operator, so disabled assertions
 *   vaporize out of the compiled code, but variables only referenced in the
 *   assertion won't trigger compiler warnings about being unused.
 * - It is safe to use with a dangling-else: `if (x) SDL_assert(y); else
 *   do_something();`
 * - It works the same everywhere, instead of counting on various platforms'
 *   compiler and C runtime to behave.
 * - It provides multiple levels of assertion (SDL_assert, SDL_assert_release,
 *   SDL_assert_paranoid) instead of a single all-or-nothing option.
 * - It offers a variety of responses when an assertion fails (retry, trigger
 *   the debugger, abort the program, ignore the failure once, ignore it for
 *   the rest of the program's run).
 * - It tries to show the user a dialog by default, if possible, but the app
 *   can provide a callback to handle assertion failures however they like.
 * - It lets failed assertions be retried. Perhaps you had a network failure
 *   and just want to retry the test after plugging your network cable back
 *   in? You can.
 * - It lets the user ignore an assertion failure, if there's a harmless
 *   problem that one can continue past.
 * - It lets the user mark an assertion as ignored for the rest of the
 *   program's run; if there's a harmless problem that keeps popping up.
 * - It provides statistics and data on all failed assertions to the app.
 * - It allows the default assertion handler to be controlled with environment
 *   variables, in case an automated script needs to control it.
 * - It can be used as an aid to Clang's static analysis; it will treat SDL
 *   assertions as universally true (under the assumption that you are serious
 *   about the asserted claims and that your debug builds will detect when
 *   these claims were wrong). This can help the analyzer avoid false
 *   positives.
 *
 * To use it: compile a debug build and just sprinkle around tests to check
 * your code!
 *
 * @{
 */

/**
 * Possible outcomes from a triggered assertion.
 *
 * When an enabled assertion triggers, it may call the assertion handler
 * (possibly one provided by the app via SDL_SetAssertionHandler), which will
 * return one of these values, possibly after asking the user.
 *
 * Then SDL will respond based on this outcome (loop around to retry the
 * condition, try to break in a debugger, kill the program, or ignore the
 * problem).
 *
 * @since This enum is available since SDL 3.2.0.
 */
using AssertState = SDL_AssertState;

/**
 * Retry the assert immediately.
 */
constexpr AssertState ASSERTION_RETRY = SDL_ASSERTION_RETRY;

/**
 * Make the debugger trigger a breakpoint.
 */
constexpr AssertState ASSERTION_BREAK = SDL_ASSERTION_BREAK;

/**
 * Terminate the program.
 */
constexpr AssertState ASSERTION_ABORT = SDL_ASSERTION_ABORT;

/**
 * Ignore the assert.
 */
constexpr AssertState ASSERTION_IGNORE = SDL_ASSERTION_IGNORE;

/**
 * Ignore the assert from now on.
 */
constexpr AssertState ASSERTION_ALWAYS_IGNORE = SDL_ASSERTION_ALWAYS_IGNORE;

/**
 * Information about an assertion failure.
 *
 * This structure is filled in with information about a triggered assertion,
 * used by the assertion handler, then added to the assertion report. This is
 * returned as a linked list from SDL_GetAssertionReport().
 *
 * @since This struct is available since SDL 3.2.0.
 */
using AssertData = SDL_AssertData;

/**
 * Never call this directly.
 *
 * Use the SDL_assert macros instead.
 *
 * @param data assert data structure.
 * @param func function name.
 * @param file file name.
 * @param line line number.
 * @returns assert state.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline AssertState ReportAssertion(AssertData* data,
                                   StringParam func,
                                   StringParam file,
                                   int line)
{
  return SDL_ReportAssertion(data, func, file, line);
}

/**
 * A @ref callback that fires when an SDL assertion fails.
 *
 * @param data a pointer to the SDL_AssertData structure corresponding to the
 *             current assertion.
 * @param userdata what was passed as `userdata` to SDL_SetAssertionHandler().
 * @returns an SDL_AssertState value indicating how to handle the failure.
 *
 * @threadsafety This callback may be called from any thread that triggers an
 *               assert at any time.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AssertionHandler = SDL_AssertionHandler;

/**
 * Set an application-defined assertion handler.
 *
 * This function allows an application to show its own assertion UI and/or
 * force the response to an assertion failure. If the application doesn't
 * provide this, SDL will try to do the right thing, popping up a
 * system-specific GUI dialog, and probably minimizing any fullscreen windows.
 *
 * This callback may fire from any thread, but it runs wrapped in a mutex, so
 * it will only fire from one thread at a time.
 *
 * This callback is NOT reset to SDL's internal handler upon SDL_Quit()!
 *
 * @param handler the SDL_AssertionHandler function to call when an assertion
 *                fails or NULL for the default handler.
 * @param userdata a pointer that is passed to `handler`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetAssertionHandler
 */
inline void SetAssertionHandler(AssertionHandler handler, void* userdata)
{
  return SDL_SetAssertionHandler(handler, userdata);
}

/**
 * Get the default assertion handler.
 *
 * This returns the function pointer that is called by default when an
 * assertion is triggered. This is an internal function provided by SDL, that
 * is used for assertions when SDL_SetAssertionHandler() hasn't been used to
 * provide a different function.
 *
 * @returns the default SDL_AssertionHandler that is called when an assert
 *          triggers.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetAssertionHandler
 */
inline AssertionHandler GetDefaultAssertionHandler()
{
  return SDL_GetDefaultAssertionHandler();
}

/**
 * Get the current assertion handler.
 *
 * This returns the function pointer that is called when an assertion is
 * triggered. This is either the value last passed to
 * SDL_SetAssertionHandler(), or if no application-specified function is set,
 * is equivalent to calling SDL_GetDefaultAssertionHandler().
 *
 * The parameter `puserdata` is a pointer to a void*, which will store the
 * "userdata" pointer that was passed to SDL_SetAssertionHandler(). This value
 * will always be NULL for the default handler. If you don't care about this
 * data, it is safe to pass a NULL pointer to this function to ignore it.
 *
 * @param puserdata pointer which is filled with the "userdata" pointer that
 *                  was passed to SDL_SetAssertionHandler().
 * @returns the SDL_AssertionHandler that is called when an assert triggers.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_SetAssertionHandler
 */
inline AssertionHandler GetAssertionHandler(void** puserdata)
{
  return SDL_GetAssertionHandler(puserdata);
}

/**
 * Get a list of all assertion failures.
 *
 * This function gets all assertions triggered since the last call to
 * SDL_ResetAssertionReport(), or the start of the program.
 *
 * The proper way to examine this data looks something like this:
 *
 * ```c
 * const SDL_AssertData *item = SDL_GetAssertionReport();
 * while (item) {
 *    printf("'%s', %s (%s:%d), triggered %u times, always ignore: %s.\@n",
 *           item->condition, item->function, item->filename,
 *           item->linenum, item->trigger_count,
 *           item->always_ignore ? "yes" : "no");
 *    item = item->next;
 * }
 * ```
 *
 * @returns a list of all failed assertions or NULL if the list is empty. This
 *          memory should not be modified or freed by the application. This
 *          pointer remains valid until the next call to SDL_Quit() or
 *          SDL_ResetAssertionReport().
 *
 * @threadsafety This function is not thread safe. Other threads calling
 *               SDL_ResetAssertionReport() simultaneously, may render the
 *               returned pointer invalid.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_ResetAssertionReport
 */
inline const AssertData* GetAssertionReport()
{
  return SDL_GetAssertionReport();
}

/**
 * Clear the list of all assertion failures.
 *
 * This function will clear the list of all assertions triggered up to that
 * point. Immediately following this call, SDL_GetAssertionReport will return
 * no items. In addition, any previously-triggered assertions will be reset to
 * a trigger_count of zero, and their always_ignore state will be false.
 *
 * @threadsafety This function is not thread safe. Other threads triggering an
 *               assertion, or simultaneously calling this function may cause
 *               memory leaks or crashes.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetAssertionReport
 */
inline void ResetAssertionReport() { return SDL_ResetAssertionReport(); }

/// @}

} // namespace SDL

#endif /* SDL3PP_ASSERT_H_ */
