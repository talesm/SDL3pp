#ifndef SDL3PP_ASSERT_H_
#define SDL3PP_ASSERT_H_

#include <SDL3/SDL_assert.h>
#include "SDL3pp_callbackWrapper.h"
#include "SDL3pp_strings.h"

namespace SDL {

/**
 * @defgroup CategoryAssert Category Assert
 *
 * A helpful assertion macro!
 *
 * SDL assertions operate like your usual `assert` macro, but with some added
 * features:
 *
 * - It uses a trick with the `sizeof` operator, so disabled assertions vaporize
 *   out of the compiled code, but variables only referenced in the assertion
 *   won't trigger compiler warnings about being unused.
 * - It is safe to use with a dangling-else: `if (x) SDL_assert(y); else
 *   do_something();`
 * - It works the same everywhere, instead of counting on various platforms'
 *   compiler and C runtime to behave.
 * - It provides multiple levels of assertion (SDL_assert, SDL_assert_release,
 *   SDL_assert_paranoid) instead of a single all-or-nothing option.
 * - It offers a variety of responses when an assertion fails (retry, trigger
 *   the debugger, abort the program, ignore the failure once, ignore it for the
 *   rest of the program's run).
 * - It tries to show the user a dialog by default, if possible, but the app can
 *   provide a callback to handle assertion failures however they like.
 * - It lets failed assertions be retried. Perhaps you had a network failure and
 *   just want to retry the test after plugging your network cable back in? You
 *   can.
 * - It lets the user ignore an assertion failure, if there's a harmless problem
 *   that one can continue past.
 * - It lets the user mark an assertion as ignored for the rest of the program's
 *   run; if there's a harmless problem that keeps popping up.
 * - It provides statistics and data on all failed assertions to the app.
 * - It allows the default assertion handler to be controlled with environment
 *   variables, in case an automated script needs to control it.
 * - It can be used as an aid to Clang's static analysis; it will treat SDL
 *   assertions as universally true (under the assumption that you are serious
 *   about the asserted claims and that your debug builds will detect when these
 *   claims were wrong). This can help the analyzer avoid false positives.
 *
 * To use it: compile a debug build and just sprinkle around tests to check your
 * code!
 *
 * @{
 */

/**
 * The level of assertion aggressiveness.
 *
 * This value changes depending on compiler options and other preprocessor
 * defines.
 *
 * It is currently one of the following values, but future SDL releases might
 * add more:
 *
 * - 0: All SDL assertion macros are disabled.
 * - 1: Release settings: SDL_assert disabled, SDL_assert_release enabled.
 * - 2: Debug settings: SDL_assert and SDL_assert_release enabled.
 * - 3: Paranoid settings: All SDL assertion macros enabled, including
 *   SDL_assert_paranoid.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_ASSERT_LEVEL SomeNumberBasedOnVariousFactors

/**
 * Attempt to tell an attached debugger to pause.
 *
 * This allows an app to programmatically halt ("break") the debugger as if it
 * had hit a breakpoint, allowing the developer to examine program state, etc.
 *
 * This is a macro--not a function--so that the debugger breaks on the source
 * code line that used SDL_TriggerBreakpoint and not in some random guts of SDL.
 * SDL_assert uses this macro for the same reason.
 *
 * If the program is not running under a debugger, SDL_TriggerBreakpoint will
 * likely terminate the app, possibly without warning. If the current platform
 * isn't supported, this macro is left undefined.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_TriggerBreakpoint() TriggerABreakpointInAPlatformSpecificManner

/**
 * A macro that reports the current function being compiled.
 *
 * If SDL can't figure how the compiler reports this, it will use "???".
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_FUNCTION __FUNCTION__

/**
 * A macro that reports the current file being compiled.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_FILE __FILE__

/**
 * A macro that reports the current line number of the file being compiled.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_LINE __LINE__

/**
 * A macro for wrapping code in `do {} while (0);` without compiler warnings.
 *
 * Visual Studio with really aggressive warnings enabled needs this to avoid
 * compiler complaints.
 *
 * the `do {} while (0);` trick is useful for wrapping code in a macro that may
 * or may not be a single statement, to avoid various C language accidents.
 *
 * To use:
 *
 * ```c
 * do { SomethingOnce(); } while (SDL_nullptr_WHILE_LOOP_CONDITION (0));
 * ```
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_NULL_WHILE_LOOP_CONDITION (0)

/**
 * The macro used when an assertion is disabled.
 *
 * This isn't for direct use by apps, but this is the code that is inserted when
 * an SDL_assert is disabled (perhaps in a release build).
 *
 * The code does nothing, but wraps `condition` in a sizeof operator, which
 * generates no code and has no side effects, but avoid compiler warnings about
 * unused variables.
 *
 * @param condition the condition to assert (but not actually run here).
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_disabled_assert(condition)                                         \
  do {                                                                         \
    (void)sizeof((condition));                                                 \
  } while (SDL_NULL_WHILE_LOOP_CONDITION)

/**
 * Possible outcomes from a triggered assertion.
 *
 * When an enabled assertion triggers, it may call the assertion handler
 * (possibly one provided by the app via SetAssertionHandler), which will return
 * one of these values, possibly after asking the user.
 *
 * Then SDL will respond based on this outcome (loop around to retry the
 * condition, try to break in a debugger, kill the program, or ignore the
 * problem).
 *
 * @since This enum is available since SDL 3.2.0.
 */
using AssertState = SDL_AssertState;

constexpr AssertState ASSERTION_RETRY =
  SDL_ASSERTION_RETRY; ///< Retry the assert immediately.

constexpr AssertState ASSERTION_BREAK =
  SDL_ASSERTION_BREAK; ///< Make the debugger trigger a breakpoint.

constexpr AssertState ASSERTION_ABORT =
  SDL_ASSERTION_ABORT; ///< Terminate the program.

constexpr AssertState ASSERTION_IGNORE =
  SDL_ASSERTION_IGNORE; ///< Ignore the assert.

constexpr AssertState ASSERTION_ALWAYS_IGNORE =
  SDL_ASSERTION_ALWAYS_IGNORE; ///< Ignore the assert from now on.

/**
 * Information about an assertion failure.
 *
 * This structure is filled in with information about a triggered assertion,
 * used by the assertion handler, then added to the assertion report. This is
 * returned as a linked list from GetAssertionReport().
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
 * The macro used when an assertion triggers a breakpoint.
 *
 * This isn't for direct use by apps; use SDL_assert or SDL_TriggerBreakpoint
 * instead.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_AssertBreakpoint() SDL_TriggerBreakpoint()

/**
 * The macro used when an assertion is enabled.
 *
 * This isn't for direct use by apps, but this is the code that is inserted when
 * an SDL_assert is enabled.
 *
 * The `do {} while(0)` avoids dangling else problems:
 *
 * ```c
 * if (x) SDL_assert(y); else blah();
 * ```
 *
 * ... without the do/while, the "else" could attach to this macro's "if". We
 * try to handle just the minimum we need here in a macro...the loop, the static
 * vars, and break points. The heavy lifting is handled in ReportAssertion().
 *
 * @param condition the condition to assert.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_enabled_assert(condition)                                          \
  do {                                                                         \
    while (!(condition)) {                                                     \
      static struct SDL_AssertData sdl_assert_data = {                         \
        false, 0, #condition, NULL, 0, NULL, NULL};                            \
      const SDL_AssertState sdl_assert_state = SDL_ReportAssertion(            \
        &sdl_assert_data, SDL_FUNCTION, SDL_FILE, SDL_LINE);                   \
      if (sdl_assert_state == SDL_ASSERTION_RETRY) {                           \
        continue; /* go again. */                                              \
      } else if (sdl_assert_state == SDL_ASSERTION_BREAK) {                    \
        SDL_AssertBreakpoint();                                                \
      }                                                                        \
      break; /* not retrying. */                                               \
    }                                                                          \
  } while (SDL_NULL_WHILE_LOOP_CONDITION)

/**
 * An assertion test that is normally performed only in debug builds.
 *
 * This macro is enabled when the SDL_ASSERT_LEVEL is >= 2, otherwise it is
 * disabled. This is meant to only do these tests in debug builds, so they can
 * tend to be more expensive, and they are meant to bring everything to a halt
 * when they fail, with the programmer there to assess the problem.
 *
 * In short: you can sprinkle these around liberally and assume they will
 * evaporate out of the build when building for end-users.
 *
 * When assertions are disabled, this wraps `condition` in a `sizeof` operator,
 * which means any function calls and side effects will not run, but the
 * compiler will not complain about any otherwise-unused variables that are only
 * referenced in the assertion.
 *
 * One can set the environment variable "SDL_ASSERT" to one of several strings
 * ("abort", "break", "retry", "ignore", "always_ignore") to force a default
 * behavior, which may be desirable for automation purposes. If your platform
 * requires GUI interfaces to happen on the main thread but you're debugging an
 * assertion in a background thread, it might be desirable to set this to
 * "break" so that your debugger takes control as soon as assert is triggered,
 * instead of risking a bad UI interaction (deadlock, etc) in the application.
 *
 * @param condition boolean value to test.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_assert(condition)                                                  \
  if (assertion_enabled && (condition)) { trigger_assertion; }

/**
 * An assertion test that is performed even in release builds.
 *
 * This macro is enabled when the SDL_ASSERT_LEVEL is >= 1, otherwise it is
 * disabled. This is meant to be for tests that are cheap to make and extremely
 * unlikely to fail; generally it is frowned upon to have an assertion failure
 * in a release build, so these assertions generally need to be of more than
 * life-and-death importance if there's a chance they might trigger. You should
 * almost always consider handling these cases more gracefully than an assert
 * allows.
 *
 * When assertions are disabled, this wraps `condition` in a `sizeof` operator,
 * which means any function calls and side effects will not run, but the
 * compiler will not complain about any otherwise-unused variables that are only
 * referenced in the assertion.
 *
 * One can set the environment variable "SDL_ASSERT" to one of several strings
 * ("abort", "break", "retry", "ignore", "always_ignore") to force a default
 * behavior, which may be desirable for automation purposes. If your platform
 * requires GUI interfaces to happen on the main thread but you're debugging an
 * assertion in a background thread, it might be desirable to set this to
 * "break" so that your debugger takes control as soon as assert is triggered,
 * instead of risking a bad UI interaction (deadlock, etc) in the application. *
 *
 * @param condition boolean value to test.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_assert_release(condition) SDL_disabled_assert(condition)

/**
 * An assertion test that is performed only when built with paranoid settings.
 *
 * This macro is enabled when the SDL_ASSERT_LEVEL is >= 3, otherwise it is
 * disabled. This is a higher level than both release and debug, so these tests
 * are meant to be expensive and only run when specifically looking for
 * extremely unexpected failure cases in a special build.
 *
 * When assertions are disabled, this wraps `condition` in a `sizeof` operator,
 * which means any function calls and side effects will not run, but the
 * compiler will not complain about any otherwise-unused variables that are only
 * referenced in the assertion.
 *
 * One can set the environment variable "SDL_ASSERT" to one of several strings
 * ("abort", "break", "retry", "ignore", "always_ignore") to force a default
 * behavior, which may be desirable for automation purposes. If your platform
 * requires GUI interfaces to happen on the main thread but you're debugging an
 * assertion in a background thread, it might be desirable to set this to
 * "break" so that your debugger takes control as soon as assert is triggered,
 * instead of risking a bad UI interaction (deadlock, etc) in the application.
 *
 * @param condition boolean value to test.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_assert_paranoid(condition) SDL_disabled_assert(condition)

/**
 * An assertion test that is always performed.
 *
 * This macro is always enabled no matter what SDL_ASSERT_LEVEL is set to. You
 * almost never want to use this, as it could trigger on an end-user's system,
 * crashing your program.
 *
 * One can set the environment variable "SDL_ASSERT" to one of several strings
 * ("abort", "break", "retry", "ignore", "always_ignore") to force a default
 * behavior, which may be desirable for automation purposes. If your platform
 * requires GUI interfaces to happen on the main thread but you're debugging an
 * assertion in a background thread, it might be desirable to set this to
 * "break" so that your debugger takes control as soon as assert is triggered,
 * instead of risking a bad UI interaction (deadlock, etc) in the application.
 *
 * @param condition boolean value to test.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_assert_always(condition) SDL_enabled_assert(condition)

/**
 * A callback that fires when an SDL assertion fails.
 *
 * @param data a pointer to the AssertData structure corresponding to the
 *             current assertion.
 * @param userdata what was passed as `userdata` to SetAssertionHandler().
 * @returns an AssertState value indicating how to handle the failure.
 *
 * @threadsafety This callback may be called from any thread that triggers an
 *               assert at any time.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using AssertionHandler = AssertState(SDLCALL*)(const AssertData* data,
                                               void* userdata);

/**
 * A callback that fires when an SDL assertion fails.
 *
 * @param data a pointer to the AssertData structure corresponding to the
 *             current assertion.
 * @returns an AssertState value indicating how to handle the failure.
 *
 * @threadsafety This callback may be called from any thread that triggers an
 *               assert at any time.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AssertionHandler
 */
using AssertionHandlerCB =
  MakeBackCallback<AssertState(const AssertData* data)>;

/**
 * Set an application-defined assertion handler.
 *
 * This function allows an application to show its own assertion UI and/or force
 * the response to an assertion failure. If the application doesn't provide
 * this, SDL will try to do the right thing, popping up a system-specific GUI
 * dialog, and probably minimizing any fullscreen windows.
 *
 * This callback may fire from any thread, but it runs wrapped in a mutex, so it
 * will only fire from one thread at a time.
 *
 * This callback is NOT reset to SDL's internal handler upon Quit()!
 *
 * @param handler the AssertionHandler function to call when an assertion fails
 *                or nullptr for the default handler.
 * @param userdata a pointer that is passed to `handler`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAssertionHandler
 */
inline void SetAssertionHandler(AssertionHandler handler, void* userdata)
{
  SDL_SetAssertionHandler(handler, userdata);
}

/**
 * Set an application-defined assertion handler.
 *
 * This function allows an application to show its own assertion UI and/or force
 * the response to an assertion failure. If the application doesn't provide
 * this, SDL will try to do the right thing, popping up a system-specific GUI
 * dialog, and probably minimizing any fullscreen windows.
 *
 * This callback may fire from any thread, but it runs wrapped in a mutex, so it
 * will only fire from one thread at a time.
 *
 * This callback is NOT reset to SDL's internal handler upon Quit()!
 *
 * @param handler the AssertionHandler function to call when an assertion fails
 *                or nullptr for the default handler.
 * @param userdata a pointer that is passed to `handler`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAssertionHandler
 */
inline void SetAssertionHandler(AssertionHandlerCB handler)
{
  static_assert(false, "Not implemented");
}

/**
 * Get the default assertion handler.
 *
 * This returns the function pointer that is called by default when an assertion
 * is triggered. This is an internal function provided by SDL, that is used for
 * assertions when SetAssertionHandler() hasn't been used to provide a different
 * function.
 *
 * @returns the default AssertionHandler that is called when an assert triggers.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAssertionHandler
 */
inline AssertionHandler GetDefaultAssertionHandler()
{
  return SDL_GetDefaultAssertionHandler();
}

/**
 * Get the current assertion handler.
 *
 * This returns the function pointer that is called when an assertion is
 * triggered. This is either the value last passed to SetAssertionHandler(), or
 * if no application-specified function is set, is equivalent to calling
 * GetDefaultAssertionHandler().
 *
 * The parameter `puserdata` is a pointer to a void*, which will store the
 * "userdata" pointer that was passed to SetAssertionHandler(). This value will
 * always be nullptr for the default handler. If you don't care about this data,
 * it is safe to pass a nullptr pointer to this function to ignore it.
 *
 * @param puserdata pointer which is filled with the "userdata" pointer that was
 *                  passed to SetAssertionHandler().
 * @returns the AssertionHandler that is called when an assert triggers.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetAssertionHandler
 */
inline AssertionHandler GetAssertionHandler(void** puserdata)
{
  return SDL_GetAssertionHandler(puserdata);
}

/**
 * Get a list of all assertion failures.
 *
 * This function gets all assertions triggered since the last call to
 * ResetAssertionReport(), or the start of the program.
 *
 * The proper way to examine this data looks something like this:
 *
 * ```c
 * const AssertData *item = GetAssertionReport();
 * while (item) {
 *    printf("'%s', %s (%s:%d), triggered %u times, always ignore: %s.\@n",
 *           item->condition, item->function, item->filename,
 *           item->linenum, item->trigger_count,
 *           item->always_ignore ? "yes" : "no");
 *    item = item->next;
 * }
 * ```
 *
 * @returns a list of all failed assertions or nullptr if the list is empty.
 *          This memory should not be modified or freed by the application. This
 *          pointer remains valid until the next call to Quit() or
 *          ResetAssertionReport().
 *
 * @threadsafety This function is not thread safe. Other threads calling
 *               ResetAssertionReport() simultaneously, may render the returned
 *               pointer invalid.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ResetAssertionReport
 */
inline const AssertData& GetAssertionReport()
{
  return SDL_GetAssertionReport();
}

/**
 * Clear the list of all assertion failures.
 *
 * This function will clear the list of all assertions triggered up to that
 * point. Immediately following this call, GetAssertionReport will return no
 * items. In addition, any previously-triggered assertions will be reset to a
 * trigger_count of zero, and their always_ignore state will be false.
 *
 * @threadsafety This function is not thread safe. Other threads triggering an
 *               assertion, or simultaneously calling this function may cause
 *               memory leaks or crashes.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAssertionReport
 */
inline void ResetAssertionReport() { SDL_ResetAssertionReport(); }

/// @}

} // namespace SDL

#endif /* SDL3PP_ASSERT_H_ */
