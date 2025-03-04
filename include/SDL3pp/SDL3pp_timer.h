#ifndef SDL3PP_TIMER_H_
#define SDL3PP_TIMER_H_

#include <chrono>
#include <SDL3/SDL_timer.h>
#include "SDL3pp_error.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @brief  Wait a specified number of milliseconds before returning.
 *
 * This function waits a specified number of milliseconds before returning. It
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
  SDL_DelayNS(std::max(duration.count(), 1l));
}

/**
 * @brief  Wait a specified number of milliseconds before returning.
 *
 * This function waits a specified number of milliseconds before returning. It
 * waits at least the specified time, but possibly longer due to OS
 * scheduling.
 *
 * @param ms the number of milliseconds to delay.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 */
inline void Delay(Uint32 ms) { SDL_Delay(ms); }

/**
 * @brief Wait a specified number of nanoseconds before returning.
 *
 * This function waits a specified number of nanoseconds before returning. It
 * will attempt to wait as close to the requested time as possible, busy
 * waiting if necessary, but could return later due to OS scheduling.
 *
 * @param ns the number of nanoseconds to delay.
 *
 * @threadsafety It is safe to call this function from any thread.
 */
inline void DelayNS(Uint64 ns) { SDL_DelayNS(ns); }

// TODO TimerID

} // namespace SDL

#endif /* SDL3PP_TIMER_H_ */
