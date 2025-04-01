#ifndef SDL3PP_TIME_H_
#define SDL3PP_TIME_H_

#include <SDL3/SDL_time.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryTime Date and Time
 *
 * SDL realtime clock and date/time routines.
 *
 * There are two data types that are used in this category: SDL_Time, which
 * represents the nanoseconds since a specific moment (an "epoch"), and
 * SDL_DateTime, which breaks time down into human-understandable components:
 * years, months, days, hours, etc.
 *
 * Much of the functionality is involved in converting those two types to
 * other useful forms.
 * @{
 */

/**
 * A structure holding a calendar date and time broken down into its
 * components.
 *
 * @since This struct is available since SDL 3.2.0.
 */
struct DateTime : SDL_DateTime
{
  /// Constructor
  constexpr DateTime(const SDL_DateTime& dt = {0})
    : SDL_DateTime(dt)
  {
  }

  /// Constructor
  constexpr DateTime(int year,
                     int month,
                     int day,
                     int hour,
                     int minute,
                     int second,
                     int nanosecond,
                     int day_of_week,
                     int utc_offset)
    : SDL_DateTime{year,
                   month,
                   day,
                   hour,
                   minute,
                   second,
                   nanosecond,
                   day_of_week,
                   utc_offset}
  {
  }

  /**
   * Converts an SDL_Time in nanoseconds since the epoch to a calendar time in
   * the SDL_DateTime format.
   *
   * @param ticks the SDL_Time to be converted.
   * @param localTime the resulting SDL_DateTime will be expressed in local time
   *                  if true, otherwise it will be in Universal Coordinated
   *                  Time (UTC).
   * @post true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  DateTime(Time ticks, bool localTime = true)
    : SDL_DateTime(0)
  {
    SDL_TimeToDateTime(ticks.ToNS(), this, localTime);
  }

  /// Returns If valid
  constexpr operator bool() const
  {
    return year != 0 || month != 0 || day != 0 || hour != 0 || minute != 0 ||
           second != 0 || nanosecond != 0;
  }

  /**
   * Converts a calendar time to an SDL_Time in nanoseconds since the epoch.
   *
   * This function ignores the day_of_week member of the SDL_DateTime struct, so
   * it may remain unset.
   *
   * @returns time on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  operator Time() const
  {
    if (SDL_Time t; SDL_DateTimeToTime(this, &t)) return Time::FromNS(t);
    return {};
  }
};

/**
 * The preferred date format of the current system locale.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa SDL_GetDateTimeLocalePreferences
 */
using DateFormat = SDL_DateFormat;

/**
 * Year/Month/Day.
 */
constexpr DateFormat DATE_FORMAT_YYYYMMDD = SDL_DATE_FORMAT_YYYYMMDD;

/**
 * Day/Month/Year.
 */
constexpr DateFormat DATE_FORMAT_DDMMYYYY = SDL_DATE_FORMAT_DDMMYYYY;

/**
 * Month/Day/Year.
 */
constexpr DateFormat DATE_FORMAT_MMDDYYYY = SDL_DATE_FORMAT_MMDDYYYY;

/**
 * The preferred time format of the current system locale.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa SDL_GetDateTimeLocalePreferences
 */
using TimeFormat = SDL_TimeFormat;

/**
 * 24 hour time
 */
constexpr TimeFormat TIME_FORMAT_24HR = SDL_TIME_FORMAT_24HR;

/**
 * 12 hour time
 */
constexpr TimeFormat TIME_FORMAT_12HR = SDL_TIME_FORMAT_12HR;

/**
 * Gets the current preferred date and time format for the system locale.
 *
 * This might be a "slow" call that has to query the operating system. It's
 * best to ask for this once and save the results. However, the preferred
 * formats can change, usually because the user has changed a system
 * preference outside of your program.
 *
 * @param dateFormat a pointer to the SDL_DateFormat to hold the returned date
 *                   format, may be NULL.
 * @param timeFormat a pointer to the SDL_TimeFormat to hold the returned time
 *                   format, may be NULL.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GetDateTimeLocalePreferences(DateFormat* dateFormat,
                                         TimeFormat* timeFormat)
{
  return SDL_GetDateTimeLocalePreferences(dateFormat, timeFormat);
}

/**
 * Gets the current value of the system realtime clock in nanoseconds since
 * Jan 1, 1970 in Universal Coordinated Time (UTC).
 *
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Time Time::Current()
{
  if (SDL_Time t; SDL_GetCurrentTime(&t)) return Time::FromNS(t);
  return Time{};
}

/**
 * Converts an SDL time into a Windows FILETIME (100-nanosecond intervals
 * since January 1, 1601).
 *
 * This function fills in the two 32-bit values of the FILETIME structure.
 *
 * @param dwLowDateTime a pointer filled in with the low portion of the
 *                      Windows FILETIME value.
 * @param dwHighDateTime a pointer filled in with the high portion of the
 *                       Windows FILETIME value.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void Time::ToWindows(Uint32* dwLowDateTime, Uint32* dwHighDateTime) const
{
  SDL_TimeToWindows(ToNS(), dwLowDateTime, dwHighDateTime);
}

/**
 * Converts a Windows FILETIME (100-nanosecond intervals since January 1,
 * 1601) to an SDL time.
 *
 * This function takes the two 32-bit values of the FILETIME structure as
 * parameters.
 *
 * @param dwLowDateTime the low portion of the Windows FILETIME value.
 * @param dwHighDateTime the high portion of the Windows FILETIME value.
 * @returns the converted SDL time.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Time Time::FromWindows(Uint32 dwLowDateTime, Uint32 dwHighDateTime)
{
  return Time::FromNS(SDL_TimeFromWindows(dwLowDateTime, dwHighDateTime));
}

/**
 * Get the number of days in a month for a given year.
 *
 * @param year the year.
 * @param month the month [1-12].
 * @returns the number of days in the requested month or -1 on failure; call
 *          SDL_GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int GetDaysInMonth(int year, int month)
{
  return SDL_GetDaysInMonth(year, month);
}

/**
 * Get the day of year for a calendar date.
 *
 * @param year the year component of the date.
 * @param month the month component of the date.
 * @param day the day component of the date.
 * @returns the day of year [0-365] if the date is valid or -1 on failure;
 *          call SDL_GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int GetDayOfYear(int year, int month, int day)
{
  return SDL_GetDayOfYear(year, month, day);
}

/**
 * Get the day of week for a calendar date.
 *
 * @param year the year component of the date.
 * @param month the month component of the date.
 * @param day the day component of the date.
 * @returns a value between 0 and 6 (0 being Sunday) if the date is valid or
 *          -1 on failure; call SDL_GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int GetDayOfWeek(int year, int month, int day)
{
  return SDL_GetDayOfWeek(year, month, day);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_TIME_H_ */
