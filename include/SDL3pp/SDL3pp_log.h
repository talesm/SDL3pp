#ifndef SDL3PP_LOG_H_
#define SDL3PP_LOG_H_

#include <format>
#include <SDL3/SDL_log.h>
#include "SDL3pp_stdinc.h"

namespace SDL {
/**
 * @defgroup CategoryLog Log Handling
 *
 * Simple log messages with priorities and categories. A message's
 * LogPriority signifies how important the message is. A message's
 * LogCategory signifies from what domain it belongs to. Every category
 * has a minimum priority specified: when a message belongs to that category,
 * it will only be sent out if it has that minimum priority or higher.
 *
 * SDL's own logs are sent below the default priority threshold, so they are
 * quiet by default.
 *
 * You can change the log verbosity programmatically using
 * LogCategory.SetLogPriority() or with SDL_SetHint(SDL_HINT_LOGGING, ...), or
 * with the "SDL_LOGGING" environment variable. This variable is a comma
 * separated set of category=level tokens that define the default logging levels
 * for SDL applications.
 *
 * The category can be a numeric category, one of "app", "error", "assert",
 * "system", "audio", "video", "render", "input", "test", or `*` for any
 * unspecified category.
 *
 * The level can be a numeric level, one of "trace", "verbose", "debug",
 * "info", "warn", "error", "critical", or "quiet" to disable that category.
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
 * You don't need to have a newline (`@n`) on the end of messages, the
 * functions will do that for you. For consistent behavior cross-platform, you
 * shouldn't have any newlines in messages, such as to log multiple lines in
 * one call; unusual platform-specific behavior can be observed in such usage.
 * Do one log call per line instead, with no newlines in messages.
 *
 * Each log call is atomic, so you won't see log messages cut off one another
 * when logging from multiple threads.
 *
 * @{
 */

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
 * By default the application and gpu categories are enabled at the INFO
 * level, the assert category is enabled at the WARN level, test is enabled at
 * the VERBOSE level and all other categories are enabled at the ERROR level.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @cat wrap-state
 *
 * @sa wrap-state
 */
class LogCategory
{
  int m_category;

public:
  constexpr explicit LogCategory(int category)
    : m_category(category)
  {
  }

  constexpr LogCategory(SDL_LogCategory category = SDL_LOG_CATEGORY_APPLICATION)
    : m_category(category)
  {
  }

  constexpr operator int() { return m_category; }

  constexpr operator SDL_LogCategory() { return SDL_LogCategory(m_category); }

  constexpr auto operator<=>(const LogCategory& other) const = default;

  /**
   * Set the priority of all log categories.
   *
   * @param priority the LogPriority to assign.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ResetLogPriorities()
   * @sa SetLogPriority()
   */
  static void SetLogPriorities(LogPriority priority)
  {
    return SDL_SetLogPriorities(priority);
  }

  /**
   * Set the priority of a particular log category.
   *
   * @param priority the SDL_LogPriority to assign.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetLogPriority()
   * @sa ResetLogPriorities()
   * @sa SetLogPriorities()
   */
  void SetLogPriority(LogPriority priority)
  {
    return SDL_SetLogPriority(m_category, priority);
  }

  /**
   * Get the priority of a particular log category.
   *
   * @returns the LogPriority for the requested category.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetLogPriority()
   */
  LogPriority GetLogPriority() const { return SDL_GetLogPriority(m_category); }

  /**
   * Reset all priorities to default.
   *
   * This is called by SDL_Quit().
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SetLogPriorities
   * @sa SetLogPriority
   */
  static void ResetLogPriorities() { return SDL_ResetLogPriorities(); }

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
    return SDL_LogMessage(m_category, priority, "%s", (const char*)(message));
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
   * @sa formatted-string
   * @sa SDL::Log()
   * @sa LogUnformatted()
   * @sa LogCritical()
   * @sa LogDebug()
   * @sa LogError()
   * @sa LogInfo()
   * @sa LogTrace()
   * @sa LogVerbose()
   * @sa LogWarn()
   */
  template<class... ARGS>
  void Log(LogPriority priority, std::string_view fmt, ARGS... args) const
  {
    return LogUnformatted(priority,
                          std::vformat(fmt, std::make_format_args(args...)));
  }

  /**
   * Log a message with SDL_LOG_PRIORITY_TRACE.
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
   * @sa formatted-string
   * @sa SDL::Log()
   * @sa LogCritical()
   * @sa LogDebug()
   * @sa LogError()
   * @sa LogInfo()
   * @sa Log()
   * @sa LogUnformatted()
   * @sa LogVerbose()
   * @sa LogWarn()
   */
  template<class... ARGS>
  void LogTrace(std::string_view fmt, ARGS&&... args) const
  {
    return Log(SDL_LOG_PRIORITY_TRACE, fmt, std::forward<ARGS>(args)...);
  }

  /**
   * Log a message with SDL_LOG_PRIORITY_VERBOSE.
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
   * @sa SDL::Log()
   * @sa LogCritical()
   * @sa LogDebug()
   * @sa LogError()
   * @sa LogInfo()
   * @sa Log()
   * @sa LogUnformatted()
   * @sa LogTrace()
   * @sa LogWarn()
   */
  template<class... ARGS>
  void LogVerbose(std::string_view fmt, ARGS&&... args) const
  {
    return Log(SDL_LOG_PRIORITY_VERBOSE, fmt, std::forward<ARGS>(args)...);
  }

  /**
   * Log a message with SDL_LOG_PRIORITY_DEBUG.
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
   * @sa SDL::Log()
   * @sa LogCritical()
   * @sa LogError()
   * @sa LogInfo()
   * @sa Log()
   * @sa LogUnformatted()
   * @sa LogTrace()
   * @sa LogVerbose()
   * @sa LogWarn()
   */
  template<class... ARGS>
  void LogDebug(std::string_view fmt, ARGS&&... args) const
  {
    return Log(SDL_LOG_PRIORITY_DEBUG, fmt, std::forward<ARGS>(args)...);
  }

  /**
   * Log a message with SDL_LOG_PRIORITY_INFO.
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
   * @sa SDL::Log()
   * @sa LogCritical()
   * @sa LogDebug()
   * @sa LogError()
   * @sa Log()
   * @sa LogUnformatted()
   * @sa LogTrace()
   * @sa LogVerbose()
   * @sa LogWarn()
   */
  template<class... ARGS>
  void LogInfo(std::string_view fmt, ARGS&&... args) const
  {
    return Log(SDL_LOG_PRIORITY_INFO, fmt, std::forward<ARGS>(args)...);
  }

  /**
   * Log a message with SDL_LOG_PRIORITY_WARN.
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
   * @sa SDL::Log()
   * @sa LogCritical()
   * @sa LogDebug()
   * @sa LogError()
   * @sa LogInfo()
   * @sa Log()
   * @sa LogUnformatted()
   * @sa LogTrace()
   * @sa LogVerbose()
   */
  template<class... ARGS>
  void LogWarn(std::string_view fmt, ARGS&&... args) const
  {
    return Log(SDL_LOG_PRIORITY_WARN, fmt, std::forward<ARGS>(args)...);
  }

  /**
   * Log a message with SDL_LOG_PRIORITY_ERROR.
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
   * @sa SDL::Log()
   * @sa LogCritical()
   * @sa LogDebug()
   * @sa LogInfo()
   * @sa Log()
   * @sa LogUnformatted()
   * @sa LogTrace()
   * @sa LogVerbose()
   * @sa LogWarn()
   */
  template<class... ARGS>
  void LogError(std::string_view fmt, ARGS&&... args) const
  {
    return Log(SDL_LOG_PRIORITY_ERROR, fmt, std::forward<ARGS>(args)...);
  }

  /**
   * Log a message with SDL_LOG_PRIORITY_CRITICAL.
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
   * @sa SDL::Log()
   * @sa LogDebug()
   * @sa LogError()
   * @sa LogInfo()
   * @sa Log()
   * @sa LogUnformatted()
   * @sa LogTrace()
   * @sa LogVerbose()
   * @sa LogWarn()
   */
  template<class... ARGS>
  void LogCritical(std::string_view fmt, ARGS&&... args) const
  {
    return Log(SDL_LOG_PRIORITY_CRITICAL, fmt, std::forward<ARGS>(args)...);
  }
};

constexpr LogCategory LOG_CATEGORY_APPLICATION = SDL_LOG_CATEGORY_APPLICATION;

constexpr LogCategory LOG_CATEGORY_ERROR = SDL_LOG_CATEGORY_ERROR;

constexpr LogCategory LOG_CATEGORY_ASSERT = SDL_LOG_CATEGORY_ASSERT;

constexpr LogCategory LOG_CATEGORY_SYSTEM = SDL_LOG_CATEGORY_SYSTEM;

constexpr LogCategory LOG_CATEGORY_AUDIO = SDL_LOG_CATEGORY_AUDIO;

constexpr LogCategory LOG_CATEGORY_VIDEO = SDL_LOG_CATEGORY_VIDEO;

constexpr LogCategory LOG_CATEGORY_RENDER = SDL_LOG_CATEGORY_RENDER;

constexpr LogCategory LOG_CATEGORY_INPUT = SDL_LOG_CATEGORY_INPUT;

constexpr LogCategory LOG_CATEGORY_TEST = SDL_LOG_CATEGORY_TEST;

constexpr LogCategory LOG_CATEGORY_GPU = SDL_LOG_CATEGORY_GPU;

constexpr LogCategory LOG_CATEGORY_RESERVED2 = SDL_LOG_CATEGORY_RESERVED2;

constexpr LogCategory LOG_CATEGORY_RESERVED3 = SDL_LOG_CATEGORY_RESERVED3;

constexpr LogCategory LOG_CATEGORY_RESERVED4 = SDL_LOG_CATEGORY_RESERVED4;

constexpr LogCategory LOG_CATEGORY_RESERVED5 = SDL_LOG_CATEGORY_RESERVED5;

constexpr LogCategory LOG_CATEGORY_RESERVED6 = SDL_LOG_CATEGORY_RESERVED6;

constexpr LogCategory LOG_CATEGORY_RESERVED7 = SDL_LOG_CATEGORY_RESERVED7;

constexpr LogCategory LOG_CATEGORY_RESERVED8 = SDL_LOG_CATEGORY_RESERVED8;

constexpr LogCategory LOG_CATEGORY_RESERVED9 = SDL_LOG_CATEGORY_RESERVED9;

constexpr LogCategory LOG_CATEGORY_RESERVED10 = SDL_LOG_CATEGORY_RESERVED10;

constexpr LogCategory LOG_CATEGORY_CUSTOM = SDL_LOG_CATEGORY_CUSTOM;

/// @}

/**
 * @name LogPriorities
 * @{
 */

constexpr LogPriority LOG_PRIORITY_INVALID = SDL_LOG_PRIORITY_INVALID;

constexpr LogPriority LOG_PRIORITY_TRACE = SDL_LOG_PRIORITY_TRACE;

constexpr LogPriority LOG_PRIORITY_VERBOSE = SDL_LOG_PRIORITY_VERBOSE;

constexpr LogPriority LOG_PRIORITY_DEBUG = SDL_LOG_PRIORITY_DEBUG;

constexpr LogPriority LOG_PRIORITY_INFO = SDL_LOG_PRIORITY_INFO;

constexpr LogPriority LOG_PRIORITY_WARN = SDL_LOG_PRIORITY_WARN;

constexpr LogPriority LOG_PRIORITY_ERROR = SDL_LOG_PRIORITY_ERROR;

constexpr LogPriority LOG_PRIORITY_CRITICAL = SDL_LOG_PRIORITY_CRITICAL;

constexpr LogPriority LOG_PRIORITY_COUNT = SDL_LOG_PRIORITY_COUNT;

/// @}

/**
 * Set the text prepended to log messages of a given priority.
 *
 * By default LOG_PRIORITY_INFO and below have no prefix, and
 * LOG_PRIORITY_WARN and higher have a prefix showing their priority, e.g.
 * "WARNING: ".
 *
 * @param priority the LogPriority to modify.
 * @param prefix the prefix to use for that log priority, or NULL to use no
 *               prefix.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LogCategory::SetLogPriorities()
 * @sa LogCategory::SetLogPriority()
 */
inline bool SetLogPriorityPrefix(LogPriority priority, StringParam prefix)
{
  return SDL_SetLogPriorityPrefix(priority, prefix);
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
 * @sa LogCategory.LogCritical()
 * @sa LogCategory.LogDebug()
 * @sa LogCategory.LogError()
 * @sa LogCategory.LogInfo()
 * @sa LogCategory.Log()
 * @sa LogCategory.LogUnformatted()
 * @sa LogCategory.LogTrace()
 * @sa LogCategory.LogVerbose()
 * @sa LogCategory.LogWarn()
 */
template<class... ARGS>
inline void Log(std::string_view fmt, ARGS&&... args)
{
  LOG_CATEGORY_APPLICATION.Log(
    LOG_PRIORITY_INFO, fmt, std::forward<ARGS>(args)...);
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
 * @sa Log()
 * @sa LogCategory.LogCritical()
 * @sa LogCategory.LogDebug()
 * @sa LogCategory.LogError()
 * @sa LogCategory.LogInfo()
 * @sa LogCategory.Log()
 * @sa LogCategory.LogUnformatted()
 * @sa LogCategory.LogTrace()
 * @sa LogCategory.LogVerbose()
 * @sa LogCategory.LogWarn()
 */
inline void LogUnformatted(StringParam message)
{
  SDL_Log("%s", static_cast<const char*>(message));
}

/**
 * The prototype for the log output callback function.
 *
 * This function is called by SDL when there is new text to be logged. A mutex
 * is held so that this function is never called by more than one thread at
 * once.
 *
 * @param userdata what was passed as `userdata` to
 *                 SetLogOutputFunction().
 * @param category the category of the message.
 * @param priority the priority of the message.
 * @param message the message being output.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using LogOutputFunction = SDL_LogOutputFunction;

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
 * @sa listener-callback
 * @sa LogOutputFunction
 */
using LogOutputFunctionCB =
  std::function<void(LogCategory, LogPriority, StringParam)>;

/**
 * Get the default log output function.
 *
 * @returns the default log output callback.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetLogOutputFunction()
 * @sa GetLogOutputFunction()
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
 * @sa GetDefaultLogOutputFunction()
 * @sa SetLogOutputFunction()
 */
inline void GetLogOutputFunction(LogOutputFunction* callback, void** userdata)
{
  return SDL_GetLogOutputFunction(callback, userdata);
}

/**
 * Get the current log output function.
 *
 * @returns the LogOutputFunctionCB currently set
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @cat listener-callback
 *
 * @sa listener-callback
 * @sa GetDefaultLogOutputFunction()
 * @sa SetLogOutputFunction()
 */
inline LogOutputFunctionCB GetLogOutputFunction()
{
  using Wrapper = UniqueWrapper<LogOutputFunctionCB>;
  LogOutputFunction cb;
  void* userdata;
  if (userdata == nullptr) {
    return [cb](LogCategory c, LogPriority p, StringParam m) {
      cb(nullptr, c, p, m);
    };
  }
  GetLogOutputFunction(&cb, &userdata);
  if (!Wrapper::contains(userdata)) {
    return [cb, userdata](LogCategory c, LogPriority p, StringParam m) {
      cb(userdata, c, p, m);
    };
  }
  return Wrapper::at(userdata);
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
 * @sa GetDefaultLogOutputFunction()
 * @sa GetLogOutputFunction()
 * @sa ResetLogOutputFunction()
 */
inline void SetLogOutputFunction(LogOutputFunction callback, void* userdata)
{
  UniqueWrapper<LogOutputFunctionCB>::erase();
  return SDL_SetLogOutputFunction(callback, userdata);
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
 * @cat listener-callback
 *
 * @sa listener-callback
 * @sa GetDefaultLogOutputFunction()
 * @sa GetLogOutputFunction()
 * @sa ResetLogOutputFunction()
 */
inline void SetLogOutputFunction(LogOutputFunctionCB callback)
{
  using Wrapper = UniqueWrapper<LogOutputFunctionCB>;
  SDL_SetLogOutputFunction(
    [](
      void* userdata, int category, LogPriority priority, const char* message) {
      return Wrapper::at(userdata)(LogCategory{category}, priority, message);
    },
    Wrapper::Wrap(std::move(callback)));
}

/**
 * Replace the current log output function with the default one
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetDefaultLogOutputFunction()
 * @sa GetLogOutputFunction()
 */
inline void ResetLogOutputFunction()
{
  return SetLogOutputFunction(GetDefaultLogOutputFunction(), nullptr);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_LOG_H_ */
