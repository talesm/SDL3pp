#ifndef SDL3PP_TIME_H_
#define SDL3PP_TIME_H_

#include <SDL3/SDL_time.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryTime Category Time
 *
 * SDL realtime clock and date/time routines.
 *
 * There are two data types that are used in this category: Time, which
 * represents the nanoseconds since a specific moment (an "epoch"), and
 * DateTime, which breaks time down into human-understandable components:
 * years, months, days, hours, etc.
 *
 * Much of the functionality is involved in converting those two types to
 * other useful forms.
 *
 * @{
 */

/// Alias to raw representation for DateTime.
using DateTimeRaw = SDL_DateTime;

/**
 * A structure holding a calendar date and time broken down into its
 * components.
 *
 * @since This struct is available since SDL 3.2.0.
 */
struct DateTime : DateTimeRaw
{
  /**
   * Wraps DateTime.
   *
   * @param dateTime the value to be wrapped
   */
  constexpr DateTime(const DateTimeRaw& dateTime = {})
    : DateTimeRaw(dateTime)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param year the value for year.
   * @param month the value for month.
   * @param day the value for day.
   * @param hour the value for hour.
   * @param minute the value for minute.
   * @param second the value for second.
   * @param nanosecond the value for nanosecond.
   * @param day_of_week the value for day_of_week.
   * @param utc_offset the value for utc_offset.
   */
  constexpr DateTime(int year,
                     int month,
                     int day,
                     int hour,
                     int minute,
                     int second,
                     int nanosecond,
                     int day_of_week,
                     int utc_offset)
    : DateTimeRaw{year,
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
   * Converts an Time in nanoseconds since the epoch to a calendar time in
   * the DateTime format.
   *
   * @param ticks the Time to be converted.
   * @param dt the resulting DateTime.
   * @param localTime the resulting DateTime will be expressed in local time
   *                  if true, otherwise it will be in Universal Coordinated
   *                  Time (UTC).
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  DateTime(Time ticks, bool localTime = true)
    : DateTimeRaw(CheckError(SDL_TimeToDateTime(ticks, localTime)))
  {
  }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return *this != DateTimeRaw{}; }

  /**
   * Get the year.
   *
   * @returns current year value.
   */
  constexpr int GetYear() const { return year; }

  /**
   * Set the year.
   *
   * @param newYear the new year value.
   * @returns Reference to self.
   */
  constexpr DateTime& SetYear(int newYear)
  {
    year = newYear;
    return *this;
  }

  /**
   * Get the month.
   *
   * @returns current month value.
   */
  constexpr int GetMonth() const { return month; }

  /**
   * Set the month.
   *
   * @param newMonth the new month value.
   * @returns Reference to self.
   */
  constexpr DateTime& SetMonth(int newMonth)
  {
    month = newMonth;
    return *this;
  }

  /**
   * Get the day.
   *
   * @returns current day value.
   */
  constexpr int GetDay() const { return day; }

  /**
   * Set the day.
   *
   * @param newDay the new day value.
   * @returns Reference to self.
   */
  constexpr DateTime& SetDay(int newDay)
  {
    day = newDay;
    return *this;
  }

  /**
   * Get the hour.
   *
   * @returns current hour value.
   */
  constexpr int GetHour() const { return hour; }

  /**
   * Set the hour.
   *
   * @param newHour the new hour value.
   * @returns Reference to self.
   */
  constexpr DateTime& SetHour(int newHour)
  {
    hour = newHour;
    return *this;
  }

  /**
   * Get the minute.
   *
   * @returns current minute value.
   */
  constexpr int GetMinute() const { return minute; }

  /**
   * Set the minute.
   *
   * @param newMinute the new minute value.
   * @returns Reference to self.
   */
  constexpr DateTime& SetMinute(int newMinute)
  {
    minute = newMinute;
    return *this;
  }

  /**
   * Get the second.
   *
   * @returns current second value.
   */
  constexpr int GetSecond() const { return second; }

  /**
   * Set the second.
   *
   * @param newSecond the new second value.
   * @returns Reference to self.
   */
  constexpr DateTime& SetSecond(int newSecond)
  {
    second = newSecond;
    return *this;
  }

  /**
   * Get the nanosecond.
   *
   * @returns current nanosecond value.
   */
  constexpr int GetNanosecond() const { return nanosecond; }

  /**
   * Set the nanosecond.
   *
   * @param newNanosecond the new nanosecond value.
   * @returns Reference to self.
   */
  constexpr DateTime& SetNanosecond(int newNanosecond)
  {
    nanosecond = newNanosecond;
    return *this;
  }

  /**
   * Get the day_of_week.
   *
   * @returns current day_of_week value.
   */
  constexpr int GetDay_of_week() const { return day_of_week; }

  /**
   * Set the day_of_week.
   *
   * @param newDay_of_week the new day_of_week value.
   * @returns Reference to self.
   */
  constexpr DateTime& SetDay_of_week(int newDay_of_week)
  {
    day_of_week = newDay_of_week;
    return *this;
  }

  /**
   * Get the utc_offset.
   *
   * @returns current utc_offset value.
   */
  constexpr int GetUtc_offset() const { return utc_offset; }

  /**
   * Set the utc_offset.
   *
   * @param newUtc_offset the new utc_offset value.
   * @returns Reference to self.
   */
  constexpr DateTime& SetUtc_offset(int newUtc_offset)
  {
    utc_offset = newUtc_offset;
    return *this;
  }

  /**
   * Converts a calendar time to an Time in nanoseconds since the epoch.
   *
   * This function ignores the day_of_week member of the DateTime struct, so
   * it may remain unset.
   *
   * @param dt the source DateTime.
   * @param ticks the resulting Time.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  operator Time() const;
};

/**
 * The preferred date format of the current system locale.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GetDateTimeLocalePreferences
 */
using DateFormat = SDL_DateFormat;

constexpr DateFormat DATE_FORMAT_YYYYMMDD =
  SDL_DATE_FORMAT_YYYYMMDD; ///< Year/Month/Day.

constexpr DateFormat DATE_FORMAT_DDMMYYYY =
  SDL_DATE_FORMAT_DDMMYYYY; ///< Day/Month/Year.

constexpr DateFormat DATE_FORMAT_MMDDYYYY =
  SDL_DATE_FORMAT_MMDDYYYY; ///< Month/Day/Year.

/**
 * The preferred time format of the current system locale.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GetDateTimeLocalePreferences
 */
using TimeFormat = SDL_TimeFormat;

constexpr TimeFormat TIME_FORMAT_24HR = SDL_TIME_FORMAT_24HR; ///< 24 hour time

constexpr TimeFormat TIME_FORMAT_12HR = SDL_TIME_FORMAT_12HR; ///< 12 hour time

/**
 * Gets the current preferred date and time format for the system locale.
 *
 * This might be a "slow" call that has to query the operating system. It's
 * best to ask for this once and save the results. However, the preferred
 * formats can change, usually because the user has changed a system
 * preference outside of your program.
 *
 * @param dateFormat a pointer to the DateFormat to hold the returned date
 *                   format, may be nullptr.
 * @param timeFormat a pointer to the TimeFormat to hold the returned time
 *                   format, may be nullptr.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void GetDateTimeLocalePreferences(DateFormat* dateFormat,
                                         TimeFormat* timeFormat)
{
  CheckError(SDL_GetDateTimeLocalePreferences(dateFormat, timeFormat));
}

/**
 * Gets the current value of the system realtime clock in nanoseconds since
 * Jan 1, 1970 in Universal Coordinated Time (UTC).
 *
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Time Time::Current() { return CheckError(SDL_GetCurrentTime()); }

/**
 * Converts an Time in nanoseconds since the epoch to a calendar time in
 * the DateTime format.
 *
 * @param ticks the Time to be converted.
 * @param dt the resulting DateTime.
 * @param localTime the resulting DateTime will be expressed in local time
 *                  if true, otherwise it will be in Universal Coordinated
 *                  Time (UTC).
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline DateTime TimeToDateTime(Time ticks, bool localTime = true)
{
  return DateTime(ticks, localTime);
}

/**
 * Converts a calendar time to an Time in nanoseconds since the epoch.
 *
 * This function ignores the day_of_week member of the DateTime struct, so
 * it may remain unset.
 *
 * @param dt the source DateTime.
 * @param ticks the resulting Time.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Time DateTimeToTime(const DateTimeRaw& dt)
{
  return CheckError(SDL_DateTimeToTime(dt));
}

inline DateTime::operator Time() const : DateTimeRaw(SDL::DateTimeToTime()) {}

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
  SDL_TimeToWindows(dwLowDateTime, dwHighDateTime);
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
  return SDL_TimeFromWindows(dwLowDateTime, dwHighDateTime);
}

/**
 * Get the number of days in a month for a given year.
 *
 * @param year the year.
 * @param month the month [1-12].
 * @returns the number of days in the requested month or -1 on failure; call
 *          GetError() for more information.
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
 *          call GetError() for more information.
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
 *          -1 on failure; call GetError() for more information.
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
