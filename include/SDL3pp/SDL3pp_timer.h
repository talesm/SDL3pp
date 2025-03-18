#ifndef SDL3PP_TIMER_H_
#define SDL3PP_TIMER_H_

#include <chrono>
#include <functional>
#include <SDL3/SDL_timer.h>
#include "SDL3pp_callbackWrapper.h"
#include "SDL3pp_error.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryTimer Timer Support
 *
 * SDL provides time management functionality. It is useful for dealing with
 * (usually) small durations of time.
 *
 * This is not to be confused with _calendar time_ management, which is
 * provided by [CategoryTime](#CategoryTime).
 *
 * This category covers measuring time elapsed (GetTicks(),
 * GetPerformanceCounter()), putting a thread to sleep for a certain
 * amount of time (SDL_Delay(), SDL_DelayNS(), SDL_DelayPrecise()), and firing
 * a callback function after a certain amount of time has elapsed
 * (AddTimer(), etc).
 *
 * @{
 */

/**
 * Get the time elapsed since SDL library initialization.
 *
 * @returns a std::chrono::nanoseconds value representing the number of
 * nanoseconds since the SDL library initialized.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline std::chrono::nanoseconds GetTicks()
{
  return std::chrono::nanoseconds{SDL_GetTicksNS()};
}

/**
 * Get the current value of the high resolution counter.
 *
 * This function is typically used for profiling.
 *
 * The counter values are only meaningful relative to each other. Differences
 * between values can be converted to times by using
 * SDL_GetPerformanceFrequency().
 *
 * @returns the current counter value.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPerformanceFrequency()
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
 * @sa GetPerformanceCounter()
 */
inline Uint64 GetPerformanceFrequency()
{
  return SDL_GetPerformanceFrequency();
}

/**
 * Wait a specified duration before returning.
 *
 * This function waits a specified duration before returning. It
 * waits at least the specified time, but possibly longer due to OS
 * scheduling.
 *
 * @param duration the duration to delay, with max precision in ns.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 */
inline void Delay(std::chrono::nanoseconds duration)
{
  SDL_DelayNS(duration.count());
}

/**
 * Wait a specified duration before returning.
 *
 * This function waits a specified duration before returning. It
 * will attempt to wait as close to the requested time as possible, busy
 * waiting if necessary, but could return later due to OS scheduling.
 *
 * @param duration the duration to delay.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Delay()
 * @sa DelayNS()
 */
inline void DelayPrecise(std::chrono::nanoseconds duration)
{
  SDL_DelayPrecise(duration.count());
}
/**
 * Definition of the timer ID type.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using TimerID = SDL_TimerID;

/**
 * Function prototype for the millisecond timer callback function.
 *
 * The callback function is passed the current timer interval and returns the
 * next timer interval, in milliseconds. If the returned value is the same as
 * the one passed in, the periodic alarm continues, otherwise a new alarm is
 * scheduled. If the callback returns 0, the periodic alarm is canceled and
 * will be removed.
 *
 * @param userdata an arbitrary pointer provided by the app through
 *                 SDL_AddTimer, for its own use.
 * @param timerID the current timer being processed.
 * @param interval the current callback time interval.
 * @returns the new callback time interval, or 0 to disable further runs of
 *          the callback.
 *
 * @threadsafety SDL may call this callback at any time from a background
 *               thread; the application is responsible for locking resources
 *               the callback touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AddTimer()
 */
using TimerCallback = SDL_TimerCallback;

/**
 * Function prototype for the nanosecond timer callback function.
 *
 * The callback function is passed the current timer interval and returns the
 * next timer interval, in nanoseconds. If the returned value is the same as
 * the one passed in, the periodic alarm continues, otherwise a new alarm is
 * scheduled. If the callback returns 0, the periodic alarm is canceled and
 * will be removed.
 *
 * @param userdata an arbitrary pointer provided by the app through
 *                 AddTimer(), for its own use.
 * @param timerID the current timer being processed.
 * @param interval the current callback time interval.
 * @returns the new callback time interval, or 0 to disable further runs of
 *          the callback.
 *
 * @threadsafety SDL may call this callback at any time from a background
 *               thread; the application is responsible for locking resources
 *               the callback touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AddTimer()
 */
using NSTimerCallback = SDL_NSTimerCallback;

/**
 * Function prototype for the nanosecond timer callback function.
 *
 * The callback function is passed the current timer interval and returns the
 * next timer interval, in nanoseconds. If the returned value is the same as
 * the one passed in, the periodic alarm continues, otherwise a new alarm is
 * scheduled. If the callback returns 0, the periodic alarm is canceled and
 * will be removed.
 *
 * @param timerID the current timer being processed.
 * @param interval the current callback time interval.
 * @returns the new callback time interval, or 0 to disable further runs of
 *          the callback.
 *
 * @threadsafety SDL may call this callback at any time from a background
 *               thread; the application is responsible for locking resources
 *               the callback touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @ingroup listener-callback
 *
 * @sa listener-callback
 * @sa AddTimer(TimerCB)
 */
using TimerCB =
  std::function<std::chrono::nanoseconds(TimerID, std::chrono::nanoseconds)>;

/**
 * Call a callback function at a future time.
 *
 * The callback function is passed the current timer interval and the user
 * supplied parameter from the AddTimer() call and should return the next
 * timer interval. If the value returned from the callback is 0, the timer is
 * canceled and will be removed.
 *
 * The callback is run on a separate thread, and for short timeouts can
 * potentially be called before this function returns.
 *
 * Timers take into account the amount of time it took to execute the
 * callback. For example, if the callback took 250 ms to execute and returned
 * 1000 (ms), the timer would only wait another 750 ms before its next
 * iteration.
 *
 * Timing may be inexact due to OS scheduling. Be sure to note the current
 * time with GetTicksNS() or GetPerformanceCounter() in case your
 * callback needs to adjust for variances.
 *
 * @param interval the timer delay, in milliseconds, passed to `callback`.
 * @param callback the TimerCallback function to call when the specified
 *                 `interval` elapses.
 * @param userdata a pointer that is passed to `callback`.
 * @returns a timer ID or 0 on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RemoveTimer()
 */
inline TimerID AddTimer(std::chrono::milliseconds interval,
                        TimerCallback callback,
                        void* userdata)
{
  return SDL_AddTimer(interval.count(), callback, userdata);
}

/**
 * Call a callback function at a future time.
 *
 * The callback function is passed the current timer interval and the user
 * supplied parameter from the AddTimerNS() call and should return the
 * next timer interval. If the value returned from the callback is 0, the
 * timer is canceled and will be removed.
 *
 * The callback is run on a separate thread, and for short timeouts can
 * potentially be called before this function returns.
 *
 * Timers take into account the amount of time it took to execute the
 * callback. For example, if the callback took 250 ns to execute and returned
 * 1000 (ns), the timer would only wait another 750 ns before its next
 * iteration.
 *
 * Timing may be inexact due to OS scheduling. Be sure to note the current
 * time with GetTicksNS() or GetPerformanceCounter() in case your
 * callback needs to adjust for variances.
 *
 * @param interval the timer delay, in std::chrono::nanoseconds, passed to
 * `callback`.
 * @param callback the NSTimerCallback function to call when the specified
 *                 `interval` elapses.
 * @param userdata a pointer that is passed to `callback`.
 * @returns a timer ID or 0 on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RemoveTimer()
 */
inline TimerID AddTimer(std::chrono::nanoseconds interval,
                        NSTimerCallback callback,
                        void* userdata)
{
  return SDL_AddTimerNS(interval.count(), callback, userdata);
}

/**
 * Call a callback function at a future time.
 *
 * The callback function is passed the current timer interval and the user
 * supplied parameter from the AddTimerNS() call and should return the
 * next timer interval. If the value returned from the callback is 0, the
 * timer is canceled and will be removed.
 *
 * The callback is run on a separate thread, and for short timeouts can
 * potentially be called before this function returns.
 *
 * Timers take into account the amount of time it took to execute the
 * callback. For example, if the callback took 250 ns to execute and returned
 * 1000 (ns), the timer would only wait another 750 ns before its next
 * iteration.
 *
 * Timing may be inexact due to OS scheduling. Be sure to note the current
 * time with GetTicksNS() or GetPerformanceCounter() in case your
 * callback needs to adjust for variances.
 *
 * @param interval the timer delay, in std::chrono::nanoseconds, passed to
 * `callback`.
 * @param callback the TimerCB function to call when the specified
 *                 `interval` elapses.
 * @returns a timer ID or 0 on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @ingroup listener-callback
 *
 * @sa listener-callback
 * @sa RemoveTimer()
 */
inline TimerID AddTimer(std::chrono::nanoseconds interval, TimerCB callback)
{
  using Wrapper = CallbackWrapper<TimerCB>;
  using Store = KeyValueWrapper<TimerID, TimerCB*>;

  auto cb = Wrapper::Wrap(std::move(callback));

  if (TimerID id = SDL_AddTimerNS(
        interval.count(),
        [](void* userdata, TimerID timerID, Uint64 interval) -> Uint64 {
          auto& f = *static_cast<TimerCB*>(userdata);
          auto next = f(timerID, std::chrono::nanoseconds(interval)).count();
          // If ask to removal, then remove it
          if (next == 0) delete Store::release(timerID);
          return next;
        },
        cb)) {
    Store::Wrap(id, std::move(cb));
    return id;
  }
  delete cb;
  return TimerID{0};
}

/**
 * Remove a timer created with AddTimer().
 *
 * @param id the ID of the timer to remove.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddTimer()
 */
inline bool RemoveTimer(TimerID id)
{
  delete KeyValueWrapper<TimerID, TimerCB*>::release(id);
  return SDL_RemoveTimer(id);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_TIMER_H_ */
