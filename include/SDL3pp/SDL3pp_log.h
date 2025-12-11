#ifndef SDL3PP_LOG_H_
#define SDL3PP_LOG_H_

#include <format>
#include <SDL3/SDL_log.h>
#include "SDL3pp_callbackWrapper.h"
#include "SDL3pp_error.h"
#include "SDL3pp_strings.h"

namespace SDL {

/**
 * @defgroup CategoryLog Log Handling
 *
 * Simple log messages with priorities and categories. A message's LogPriority
 * signifies how important the message is. A message's LogCategory signifies
 * from what domain it belongs to. Every category has a minimum priority
 * specified: when a message belongs to that category, it will only be sent out
 * if it has that minimum priority or higher.
 *
 * SDL's own logs are sent below the default priority threshold, so they are
 * quiet by default.
 *
 * You can change the log verbosity programmatically using
 * LogCategory.SetLogPriority() or with SetHint(SDL_HINT_LOGGING, ...), or with
 * the "SDL_LOGGING" environment variable. This variable is a comma separated
 * set of category=level tokens that define the default logging levels for SDL
 * applications.
 *
 * The category can be a numeric category, one of "app", "error", "assert",
 * "system", "audio", "video", "render", "input", "test", or `*` for any
 * unspecified category.
 *
 * The level can be a numeric level, one of "trace", "verbose", "debug", "info",
 * "warn", "error", "critical", or "quiet" to disable that category.
 *
 * You can omit the category if you want to set the logging level for all
 * categories.
 *
 * If this hint isn't set, the default log levels are equivalent to:
 *
 * `app=info,assert=warn,test=verbose,*=error`
 *
 * Here's where the messages go on different platforms:
 *
 * - Windows: debug output stream
 * - Android: log output
 * - Others: standard error output (stderr)
 *
 * You don't need to have a newline (`@n`) on the end of messages, the functions
 * will do that for you. For consistent behavior cross-platform, you shouldn't
 * have any newlines in messages, such as to log multiple lines in one call;
 * unusual platform-specific behavior can be observed in such usage. Do one log
 * call per line instead, with no newlines in messages.
 *
 * Each log call is atomic, so you won't see log messages cut off one another
 * when logging from multiple threads.
 *
 * @{
 */

/// Alias to raw representation for LogCategory.
using LogCategoryRaw = SDL_LogCategory;

// Forward decl
struct LogCategory;

/**
 * @name LogPriorities
 *
 * The priorities assignable for a LogCategory.
 *
 * @{
 */

/**
 * The predefined log priorities
 *
 * @since This enum is available since SDL 3.2.0.
 */
using LogPriority = SDL_LogPriority;

constexpr LogPriority LOG_PRIORITY_INVALID =
  SDL_LOG_PRIORITY_INVALID; ///< INVALID

constexpr LogPriority LOG_PRIORITY_TRACE = SDL_LOG_PRIORITY_TRACE; ///< TRACE

constexpr LogPriority LOG_PRIORITY_VERBOSE =
  SDL_LOG_PRIORITY_VERBOSE; ///< VERBOSE

constexpr LogPriority LOG_PRIORITY_DEBUG = SDL_LOG_PRIORITY_DEBUG; ///< DEBUG

constexpr LogPriority LOG_PRIORITY_INFO = SDL_LOG_PRIORITY_INFO; ///< INFO

constexpr LogPriority LOG_PRIORITY_WARN = SDL_LOG_PRIORITY_WARN; ///< WARN

constexpr LogPriority LOG_PRIORITY_ERROR = SDL_LOG_PRIORITY_ERROR; ///< ERROR

constexpr LogPriority LOG_PRIORITY_CRITICAL =
  SDL_LOG_PRIORITY_CRITICAL; ///< CRITICAL

constexpr LogPriority LOG_PRIORITY_COUNT = SDL_LOG_PRIORITY_COUNT; ///< COUNT

/// @}

/**
 * @name LogCategories
 *
 * The logging categories. see LogCategory for more info
 *
 * @{
 */

/**
 * The predefined log categories
 *
 * By default the application and gpu categories are enabled at the INFO level,
 * the assert category is enabled at the WARN level, test is enabled at the
 * VERBOSE level and all other categories are enabled at the ERROR level.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @cat wrap-state
 *
 * @sa wrap-state
 */
class LogCategory
{
  LogCategoryRaw m_category;

public:
  /**
   * Wraps LogCategory.
   *
   * @param category the value to be wrapped
   */
  constexpr LogCategory(
    LogCategoryRaw category = SDL_LOG_CATEGORY_APPLICATION) noexcept
    : m_category(category)
  {
  }

  /**
   * Wraps LogCategory.
   *
   * @param category the value to be wrapped
   */
  constexpr explicit LogCategory(int category)
    : m_category(SDL_LogCategory(category))
  {
  }

  /**
   * Unwraps to the underlying LogCategory.
   *
   * @returns the underlying LogCategoryRaw.
   */
  constexpr operator LogCategoryRaw() const noexcept { return m_category; }

  /**
   * Set the priority of a particular log category.
   *
   * @param priority the LogPriority to assign.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LogCategory.GetLogPriority
   * @sa ResetLogPriorities
   * @sa SetLogPriorities
   */
  void SetLogPriority(LogPriority priority) const;

  /**
   * Get the priority of a particular log category.
   *
   * @returns the LogPriority for the requested category.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa LogCategory.SetLogPriority
   */
  LogPriority GetLogPriority() const;

  /**
   * Log an unformatted message with the specified priority.
   *
   * @param priority the priority of the message.
   * @param message string to output.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL::Log()
   * @sa Log()
   * @sa LogCritical()
   * @sa LogDebug()
   * @sa LogError()
   * @sa LogInfo()
   * @sa LogTrace()
   * @sa LogVerbose()
   * @sa LogWarn()
   */
  void LogUnformatted(LogPriority priority, StringParam message) const
  {
    SDL_LogMessage(m_category, priority, "%s", (const char*)(message));
  }

  /**
   * Log a message with the specified priority.
   *
   * @param priority the priority of the message.
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogDebug
   * @sa LogCategory.LogError
   * @sa LogCategory.LogInfo
   * @sa LogUnformatted
   * @sa LogCategory.LogTrace
   * @sa LogCategory.LogVerbose
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogMessage(LogPriority priority,
                  std::string_view fmt,
                  ARGS... args) const;

  /**
   * Log a message with LOG_PRIORITY_TRACE.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogDebug
   * @sa LogCategory.LogError
   * @sa LogCategory.LogInfo
   * @sa LogCategory.LogMessage
   * @sa LogUnformatted
   * @sa LogCategory.LogTrace
   * @sa LogCategory.LogVerbose
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogTrace(std::string_view fmt, ARGS&&... args) const;

  /**
   * Log a message with LOG_PRIORITY_VERBOSE.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogDebug
   * @sa LogCategory.LogError
   * @sa LogCategory.LogInfo
   * @sa LogCategory.LogMessage
   * @sa LogUnformatted
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogVerbose(std::string_view fmt, ARGS&&... args) const;

  /**
   * Log a message with LOG_PRIORITY_DEBUG.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogError
   * @sa LogCategory.LogInfo
   * @sa LogCategory.LogMessage
   * @sa LogUnformatted
   * @sa LogCategory.LogTrace
   * @sa LogCategory.LogVerbose
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogDebug(std::string_view fmt, ARGS&&... args) const;

  /**
   * Log a message with LOG_PRIORITY_INFO.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogDebug
   * @sa LogCategory.LogError
   * @sa LogCategory.LogMessage
   * @sa LogUnformatted
   * @sa LogCategory.LogTrace
   * @sa LogCategory.LogVerbose
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogInfo(std::string_view fmt, ARGS&&... args) const;

  /**
   * Log a message with LOG_PRIORITY_WARN.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogDebug
   * @sa LogCategory.LogError
   * @sa LogCategory.LogInfo
   * @sa LogCategory.LogMessage
   * @sa LogUnformatted
   * @sa LogCategory.LogTrace
   * @sa LogCategory.LogVerbose
   */
  template<class... ARGS>
  void LogWarn(std::string_view fmt, ARGS&&... args) const;

  /**
   * Log a message with LOG_PRIORITY_ERROR.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa LogCategory.LogCritical
   * @sa LogCategory.LogDebug
   * @sa LogCategory.LogInfo
   * @sa LogCategory.LogMessage
   * @sa LogUnformatted
   * @sa LogCategory.LogTrace
   * @sa LogCategory.LogVerbose
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogError(std::string_view fmt, ARGS&&... args) const;

  /**
   * Log a message with LOG_PRIORITY_CRITICAL.
   *
   * @param fmt a
   * [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
   * style message format string.
   * @param args additional parameters matching the `{}` tokens in the format
   * string, if any.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @cat formatted-string
   *
   * @sa formatted-string
   * @sa LogCategory.LogDebug()
   * @sa LogCategory.LogError()
   * @sa LogCategory.LogInfo()
   * @sa LogUnformatted()
   * @sa LogCategory.LogUnformatted()
   * @sa LogCategory.LogTrace()
   * @sa LogCategory.LogVerbose()
   * @sa LogCategory.LogWarn
   */
  template<class... ARGS>
  void LogCritical(std::string_view fmt, ARGS&&... args) const;
};

constexpr LogCategory LOG_CATEGORY_APPLICATION =
  SDL_LOG_CATEGORY_APPLICATION; ///< APPLICATION

constexpr LogCategory LOG_CATEGORY_ERROR = SDL_LOG_CATEGORY_ERROR; ///< ERROR

constexpr LogCategory LOG_CATEGORY_ASSERT = SDL_LOG_CATEGORY_ASSERT; ///< ASSERT

constexpr LogCategory LOG_CATEGORY_SYSTEM = SDL_LOG_CATEGORY_SYSTEM; ///< SYSTEM

constexpr LogCategory LOG_CATEGORY_AUDIO = SDL_LOG_CATEGORY_AUDIO; ///< AUDIO

constexpr LogCategory LOG_CATEGORY_VIDEO = SDL_LOG_CATEGORY_VIDEO; ///< VIDEO

constexpr LogCategory LOG_CATEGORY_RENDER = SDL_LOG_CATEGORY_RENDER; ///< RENDER

constexpr LogCategory LOG_CATEGORY_INPUT = SDL_LOG_CATEGORY_INPUT; ///< INPUT

constexpr LogCategory LOG_CATEGORY_TEST = SDL_LOG_CATEGORY_TEST; ///< TEST

constexpr LogCategory LOG_CATEGORY_GPU = SDL_LOG_CATEGORY_GPU; ///< GPU

constexpr LogCategory LOG_CATEGORY_RESERVED2 =
  SDL_LOG_CATEGORY_RESERVED2; ///< RESERVED2

constexpr LogCategory LOG_CATEGORY_RESERVED3 =
  SDL_LOG_CATEGORY_RESERVED3; ///< RESERVED3

constexpr LogCategory LOG_CATEGORY_RESERVED4 =
  SDL_LOG_CATEGORY_RESERVED4; ///< RESERVED4

constexpr LogCategory LOG_CATEGORY_RESERVED5 =
  SDL_LOG_CATEGORY_RESERVED5; ///< RESERVED5

constexpr LogCategory LOG_CATEGORY_RESERVED6 =
  SDL_LOG_CATEGORY_RESERVED6; ///< RESERVED6

constexpr LogCategory LOG_CATEGORY_RESERVED7 =
  SDL_LOG_CATEGORY_RESERVED7; ///< RESERVED7

constexpr LogCategory LOG_CATEGORY_RESERVED8 =
  SDL_LOG_CATEGORY_RESERVED8; ///< RESERVED8

constexpr LogCategory LOG_CATEGORY_RESERVED9 =
  SDL_LOG_CATEGORY_RESERVED9; ///< RESERVED9

constexpr LogCategory LOG_CATEGORY_RESERVED10 =
  SDL_LOG_CATEGORY_RESERVED10; ///< RESERVED10

constexpr LogCategory LOG_CATEGORY_CUSTOM = SDL_LOG_CATEGORY_CUSTOM; ///< CUSTOM

/**
 * Set the priority of all log categories.
 *
 * @param priority the LogPriority to assign.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ResetLogPriorities
 * @sa LogCategory.SetLogPriority
 */
inline void SetLogPriorities(LogPriority priority)
{
  SDL_SetLogPriorities(priority);
}

/**
 * Set the priority of a particular log category.
 *
 * @param category the category to assign a priority to.
 * @param priority the LogPriority to assign.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LogCategory.GetLogPriority
 * @sa ResetLogPriorities
 * @sa SetLogPriorities
 */
inline void SetLogPriority(int category, LogPriority priority)
{
  SDL_SetLogPriority(category, priority);
}

inline void LogCategory::SetLogPriority(LogPriority priority) const
{
  SDL::SetLogPriority(m_category, priority);
}

/**
 * Get the priority of a particular log category.
 *
 * @param category the category to query.
 * @returns the LogPriority for the requested category.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LogCategory.SetLogPriority
 */
inline LogPriority GetLogPriority(int category)
{
  return SDL_GetLogPriority(category);
}

inline LogPriority LogCategory::GetLogPriority() const
{
  return SDL::GetLogPriority(m_category);
}

/**
 * Reset all priorities to default.
 *
 * This is called by Quit().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetLogPriorities
 * @sa LogCategory.SetLogPriority
 */
inline void ResetLogPriorities() { SDL_ResetLogPriorities(); }

/**
 * Set the text prepended to log messages of a given priority.
 *
 * By default LOG_PRIORITY_INFO and below have no prefix, and LOG_PRIORITY_WARN
 * and higher have a prefix showing their priority, e.g. "WARNING: ".
 *
 * @param priority the LogPriority to modify.
 * @param prefix the prefix to use for that log priority, or nullptr to use no
 *               prefix.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetLogPriorities
 * @sa LogCategory.SetLogPriority
 */
inline void SetLogPriorityPrefix(LogPriority priority, StringParam prefix)
{
  CheckError(SDL_SetLogPriorityPrefix(priority, prefix));
}

/**
 * Log an unformatted message with LOG_CATEGORY_APPLICATION and
 * LOG_PRIORITY_INFO.
 *
 * @param category the category of the message.
 * @param priority the priority of the message.
 * @param message string to output.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
inline void LogUnformatted(LogCategory category,
                           LogPriority priority,
                           StringParam message)
{
  SDL_LogMessage(category, priority, "%s", static_cast<const char*>(message));
}

/**
 * Log an unformatted message with LOG_CATEGORY_APPLICATION and
 * LOG_PRIORITY_INFO.
 *
 * @param message string to output.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
inline void LogUnformatted(StringParam message)
{
  SDL_Log("%s", static_cast<const char*>(message));
}

/**
 * Log a message with LOG_CATEGORY_APPLICATION and LOG_PRIORITY_INFO.
 *
 * @param fmt a
 *            [std::format/fmt](https://en.cppreference.com/w/cpp/utility/format/spec)
 *            style message format string.
 * @param args additional parameters matching the `{}` tokens in the format
 *             string, if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @cat formatted-string
 *
 * @sa formatted-string
 * @sa LogUnformatted()
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void Log(std::string_view fmt, ARGS&&... args)
{
  LOG_CATEGORY_APPLICATION.LogInfo(fmt, std::forward<ARGS>(args)...);
}

/**
 * Log a message with the specified category and priority.
 *
 * @param category the category of the message.
 * @param priority the priority of the message.
 * @param fmt a printf() style message format string.
 * @param args additional parameters matching % tokens in the **fmt** string,
 *             if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogMessage(LogCategory category,
                       LogPriority priority,
                       std::string_view fmt,
                       ARGS... args)
{
  LogUnformatted(
    category, priority, std::vformat(fmt, std::make_format_args(args...)));
}

template<class... ARGS>
inline void LogCategory::LogMessage(LogPriority priority,
                                    std::string_view fmt,
                                    ARGS... args) const
{
  SDL::LogMessage(m_category, priority, fmt, args...);
}

/**
 * Log a message with LOG_PRIORITY_TRACE.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param args additional parameters matching % tokens in the **fmt** string,
 *             if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogTrace(LogCategory category, std::string_view fmt, ARGS&&... args)
{
  LogMessage(category, LOG_PRIORITY_TRACE, fmt, args...);
}

template<class... ARGS>
inline void LogCategory::LogTrace(std::string_view fmt, ARGS&&... args) const
{
  SDL::LogTrace(m_category, fmt, args...);
}

/**
 * Log a message with LOG_PRIORITY_VERBOSE.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param args additional parameters matching % tokens in the **fmt** string,
 *             if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogVerbose(LogCategory category,
                       std::string_view fmt,
                       ARGS&&... args)
{
  LogMessage(category, LOG_PRIORITY_VERBOSE, fmt, args...);
}

template<class... ARGS>
inline void LogCategory::LogVerbose(std::string_view fmt, ARGS&&... args) const
{
  SDL::LogVerbose(m_category, fmt, args...);
}

/**
 * Log a message with LOG_PRIORITY_DEBUG.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param args additional parameters matching % tokens in the **fmt** string,
 *             if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogDebug(LogCategory category, std::string_view fmt, ARGS&&... args)
{
  LogMessage(category, LOG_PRIORITY_DEBUG, fmt, args...);
}

template<class... ARGS>
inline void LogCategory::LogDebug(std::string_view fmt, ARGS&&... args) const
{
  SDL::LogDebug(m_category, fmt, args...);
}

/**
 * Log a message with LOG_PRIORITY_INFO.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param args additional parameters matching % tokens in the **fmt** string,
 *             if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogInfo(LogCategory category, std::string_view fmt, ARGS&&... args)
{
  LogMessage(category, LOG_PRIORITY_INFO, fmt, args...);
}

template<class... ARGS>
inline void LogCategory::LogInfo(std::string_view fmt, ARGS&&... args) const
{
  SDL::LogInfo(m_category, fmt, args...);
}

/**
 * Log a message with LOG_PRIORITY_WARN.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param args additional parameters matching % tokens in the **fmt** string,
 *             if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 */
template<class... ARGS>
inline void LogWarn(LogCategory category, std::string_view fmt, ARGS&&... args)
{
  LogMessage(category, SDL_LOG_PRIORITY_WARN, fmt, args...);
}

template<class... ARGS>
inline void LogCategory::LogWarn(std::string_view fmt, ARGS&&... args) const
{
  SDL::LogWarn(m_category, fmt, args...);
}

/**
 * Log a message with LOG_PRIORITY_ERROR.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param args additional parameters matching % tokens in the **fmt** string,
 *             if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogCritical
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogError(LogCategory category, std::string_view fmt, ARGS&&... args)
{
  LogMessage(category, SDL_LOG_PRIORITY_ERROR, fmt, args...);
}

template<class... ARGS>
inline void LogCategory::LogError(std::string_view fmt, ARGS&&... args) const
{
  SDL::LogError(m_category, fmt, args...);
}

/**
 * Log a message with LOG_PRIORITY_CRITICAL.
 *
 * @param category the category of the message.
 * @param fmt a printf() style message format string.
 * @param args additional parameters matching % tokens in the **fmt** string,
 *            if any.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Log
 * @sa LogCategory.LogDebug
 * @sa LogCategory.LogError
 * @sa LogCategory.LogInfo
 * @sa LogCategory.LogMessage
 * @sa LogUnformatted
 * @sa LogCategory.LogTrace
 * @sa LogCategory.LogVerbose
 * @sa LogCategory.LogWarn
 */
template<class... ARGS>
inline void LogCritical(LogCategory category,
                        std::string_view fmt,
                        ARGS&&... args)
{
  LogMessage(category, SDL_LOG_PRIORITY_CRITICAL, fmt, args...);
}

template<class... ARGS>
inline void LogCategory::LogCritical(std::string_view fmt, ARGS&&... args) const
{
  SDL::LogCritical(m_category, fmt, args...);
}

/**
 * The prototype for the log output callback function.
 *
 * This function is called by SDL when there is new text to be logged. A mutex
 * is held so that this function is never called by more than one thread at
 * once.
 *
 * @param userdata what was passed as `userdata` to SetLogOutputFunction().
 * @param category the category of the message.
 * @param priority the priority of the message.
 * @param message the message being output.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using LogOutputFunction = void(SDLCALL*)(void* userdata,
                                         int category,
                                         LogPriority priority,
                                         const char* message);

/**
 * The prototype for the log output callback function.
 *
 * This function is called by SDL when there is new text to be logged. A mutex
 * is held so that this function is never called by more than one thread at
 * once.
 *
 * @param category the category of the message.
 * @param priority the priority of the message.
 * @param message the message being output.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat listener-callback
 *
 * @sa LogOutputFunction
 */
using LogOutputCB = MakeFrontCallback<
  void(int category, LogPriority priority, const char* message)>;

/**
 * Get the default log output function.
 *
 * @returns the default log output callback.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetLogOutputFunction
 * @sa GetLogOutputFunction
 */
inline LogOutputFunction GetDefaultLogOutputFunction()
{
  return SDL_GetDefaultLogOutputFunction();
}

/**
 * Get the current log output function.
 *
 * @param callback an LogOutputFunction filled in with the current log callback.
 * @param userdata a pointer filled in with the pointer that is passed to
 *                 `callback`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetDefaultLogOutputFunction
 * @sa SetLogOutputFunction
 */
inline void GetLogOutputFunction(LogOutputFunction* callback, void** userdata)
{
  SDL_GetLogOutputFunction(callback, userdata);
}

/**
 * Replace the default log output function with one of your own.
 *
 * @param callback an LogOutputFunction to call instead of the default.
 * @param userdata a pointer that is passed to `callback`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetDefaultLogOutputFunction
 * @sa GetLogOutputFunction
 * @sa ResetLogOutputFunction
 */
inline void SetLogOutputFunction(LogOutputFunction callback, void* userdata)
{
  SDL_SetLogOutputFunction(callback, userdata);
}

/**
 * Replace the default log output function with one of your own.
 *
 * @param callback an LogOutputFunction to call instead of the default.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetDefaultLogOutputFunction
 * @sa GetLogOutputFunction
 * @sa ResetLogOutputFunction
 */
inline void SetLogOutputFunction(LogOutputCB callback)
{
  SetLogOutputFunction(callback.wrapper, callback.data);
}

/**
 * Replace the current log output function with the default one
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetDefaultLogOutputFunction
 * @sa GetLogOutputFunction
 */
inline void ResetLogOutputFunction()
{
  return SetLogOutputFunction(GetDefaultLogOutputFunction(), nullptr);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_LOG_H_ */
