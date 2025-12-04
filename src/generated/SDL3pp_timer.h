#ifndef SDL3PP_TIMER_H_
#define SDL3PP_TIMER_H_

#include <SDL3/SDL_timer.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryTimer Category Timer
 *
 * SDL provides time management functionality. It is useful for dealing with
 * (usually) small durations of time.
 *
 * This is not to be confused with _calendar time_ management, which is provided
 * by [CategoryTime](CategoryTime).
 *
 * This category covers measuring time elapsed (SDL_GetTicks(),
 * GetPerformanceCounter()), putting a thread to sleep for a certain amount of
 * time (Delay(), SDL_DelayNS(), DelayPrecise()), and firing a callback function
 * after a certain amount of time has elasped (SDL_AddTimer(), etc).
 *
 * There are also useful macros to convert between time units, like
 * Time.FromPosix() and such.
 *
 * @{
 */

constexpr Time Time::FromPosix(Sint64 time) { return SDL_SECONDS_TO_NS(time); }

constexpr Sint64 Time::ToPosix() const { return SDL_NS_TO_SECONDS(m_time); }

/**
 * Get the number of nanoseconds since SDL library initialization.
 *
 * @returns an unsigned 64-bit value representing the number of nanoseconds
 *          since the SDL library initialized.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline std::chrono::nanoseconds GetTicks() { return SDL_GetTicksNS(); }

/**
 * Get the current value of the high resolution counter.
 *
 * This function is typically used for profiling.
 *
 * The counter values are only meaningful relative to each other. Differences
 * between values can be converted to times by using GetPerformanceFrequency().
 *
 * @returns the current counter value.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPerformanceFrequency
 */
inline Uint64 GetPerformanceCounter() { return SDL_GetPerformanceCounter(); }

/**
 * Get the count per second of the high resolution counter.
 *
 * @returns a platform-specific count per second.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPerformanceCounter
 */
inline Uint64 GetPerformanceFrequency()
{
  return SDL_GetPerformanceFrequency();
}

/**
 * Wait a specified number of milliseconds before returning.
 *
 * This function waits a specified number of milliseconds before returning. It
 * waits at least the specified time, but possibly longer due to OS scheduling.
 *
 * @param ms the number of milliseconds to delay.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_DelayNS
 * @sa DelayPrecise
 */
inline void Delay(std::chrono::nanoseconds duration) { SDL_Delay(duration); }

/**
 * Wait a specified number of nanoseconds before returning.
 *
 * This function waits a specified number of nanoseconds before returning. It
 * will attempt to wait as close to the requested time as possible, busy waiting
 * if necessary, but could return later due to OS scheduling.
 *
 * @param ns the number of nanoseconds to delay.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Delay
 * @sa SDL_DelayNS
 */
inline void DelayPrecise(std::chrono::nanoseconds duration)
{
  SDL_DelayPrecise(duration);
}

/**
 * Definition of the timer ID type.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using TimerID = SDL_TimerID;

/**
 * Function prototype for the nanosecond timer callback function.
 *
 * The callback function is passed the current timer interval and returns the
 * next timer interval, in nanoseconds. If the returned value is the same as the
 * one passed in, the periodic alarm continues, otherwise a new alarm is
 * scheduled. If the callback returns 0, the periodic alarm is canceled and will
 * be removed.
 *
 * @param userdata an arbitrary pointer provided by the app through
 *                 SDL_AddTimer, for its own use.
 * @param timerID the current timer being processed.
 * @param interval the current callback time interval.
 * @returns the new callback time interval, or 0 to disable further runs of the
 *          callback.
 *
 * @threadsafety SDL may call this callback at any time from a background
 *               thread; the application is responsible for locking resources
 *               the callback touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AddTimer
 */
using TimerCallback = Uint64(SDLCALL*)(void* userdata,
                                       TimerID timerID,
                                       Uint64 interval);

/**
 * Function prototype for the nanosecond timer callback function.
 *
 * The callback function is passed the current timer interval and returns the
 * next timer interval, in nanoseconds. If the returned value is the same as the
 * one passed in, the periodic alarm continues, otherwise a new alarm is
 * scheduled. If the callback returns 0, the periodic alarm is canceled and will
 * be removed.
 *
 * @param timerID the current timer being processed.
 * @param interval the current callback time interval.
 * @returns the new callback time interval, or 0 to disable further runs of the
 *          callback.
 *
 * @threadsafety SDL may call this callback at any time from a background
 *               thread; the application is responsible for locking resources
 *               the callback touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AddTimer
 *
 * @sa TimerCallback
 */
using TimerCB = MakeFrontCallback<Uint64(TimerID timerID, Uint64 interval)>;

/**
 * Call a callback function at a future time.
 *
 * The callback function is passed the current timer interval and the user
 * supplied parameter from the AddTimer() call and should return the next timer
 * interval. If the value returned from the callback is 0, the timer is canceled
 * and will be removed.
 *
 * The callback is run on a separate thread, and for short timeouts can
 * potentially be called before this function returns.
 *
 * Timers take into account the amount of time it took to execute the callback.
 * For example, if the callback took 250 ns to execute and returned 1000 (ns),
 * the timer would only wait another 750 ns before its next iteration.
 *
 * Timing may be inexact due to OS scheduling. Be sure to note the current time
 * with GetTicks() or GetPerformanceCounter() in case your callback needs to
 * adjust for variances.
 *
 * @param interval the timer delay, in nanoseconds, passed to `callback`.
 * @param callback the SDL_TimerCallback function to call when the specified
 *                 `interval` elapses.
 * @param userdata a pointer that is passed to `callback`.
 * @returns a timer ID or 0 on failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_AddTimer
 * @sa RemoveTimer
 */
inline TimerID AddTimer(std::chrono::nanoseconds interval,
                        TimerCallback callback,
                        void* userdata)
{
  return SDL_AddTimerNS(interval, callback, userdata);
}

/**
 * Call a callback function at a future time.
 *
 * The callback function is passed the current timer interval and the user
 * supplied parameter from the AddTimer() call and should return the next timer
 * interval. If the value returned from the callback is 0, the timer is canceled
 * and will be removed.
 *
 * The callback is run on a separate thread, and for short timeouts can
 * potentially be called before this function returns.
 *
 * Timers take into account the amount of time it took to execute the callback.
 * For example, if the callback took 250 ns to execute and returned 1000 (ns),
 * the timer would only wait another 750 ns before its next iteration.
 *
 * Timing may be inexact due to OS scheduling. Be sure to note the current time
 * with GetTicks() or GetPerformanceCounter() in case your callback needs to
 * adjust for variances.
 *
 * @param interval the timer delay, in nanoseconds, passed to `callback`.
 * @param callback the SDL_TimerCallback function to call when the specified
 *                 `interval` elapses.
 * @param userdata a pointer that is passed to `callback`.
 * @returns a timer ID or 0 on failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_AddTimer
 * @sa RemoveTimer
 */
inline TimerID AddTimer(std::chrono::nanoseconds interval, TimerCB callback)
{
  static_assert(false, "Not implemented");
}

/**
 * Remove a timer created with SDL_AddTimer().
 *
 * @param id the ID of the timer to remove.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_AddTimer
 */
inline void RemoveTimer(TimerID id) { CheckError(SDL_RemoveTimer(id)); }

/// @}

} // namespace SDL

#endif /* SDL3PP_TIMER_H_ */
