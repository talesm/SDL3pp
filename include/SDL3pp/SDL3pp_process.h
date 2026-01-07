#ifndef SDL3PP_PROCESS_H_
#define SDL3PP_PROCESS_H_

#include <SDL3/SDL_process.h>
#include "SDL3pp_iostream.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryProcess Process Control
 *
 * Process control support.
 *
 * These functions provide a cross-platform way to spawn and manage OS-level
 * processes.
 *
 * You can create a new subprocess with Process.Process() and optionally read
 * and write to it using Process.Read() or Process.GetInput() and
 * Process.GetOutput(). If more advanced functionality like chaining input
 * between processes is necessary, you can use Process.Process().
 *
 * You can get the status of a created process with Process.Wait(), or terminate
 * the process with Process.Kill().
 *
 * Don't forget to call Process.Destroy() to clean up, whether the process
 * process was killed, terminated on its own, or is still running!
 *
 * @{
 */

// Forward decl
struct Process;

/// Alias to raw representation for Process.
using ProcessRaw = SDL_Process*;

// Forward decl
struct ProcessRef;

/// Safely wrap Process for non owning parameters
struct ProcessParam
{
  ProcessRaw value; ///< parameter's ProcessRaw

  /// Constructs from ProcessRaw
  constexpr ProcessParam(ProcessRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr ProcessParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const ProcessParam& other) const = default;

  /// Converts to underlying ProcessRaw
  constexpr operator ProcessRaw() const { return value; }
};

/**
 * Description of where standard I/O should be directed when creating a process.
 *
 * If a standard I/O stream is set to PROCESS_STDIO_INHERITED, it will go to the
 * same place as the application's I/O stream. This is the default for standard
 * output and standard error.
 *
 * If a standard I/O stream is set to SDL_PROCESS_STDIO_nullptr, it is connected
 * to `NUL:` on Windows and `/dev/null` on POSIX systems. This is the default
 * for standard input.
 *
 * If a standard I/O stream is set to PROCESS_STDIO_APP, it is connected to a
 * new IOStream that is available to the application. Standard input will be
 * available as `prop::process.STDIN_POINTER` and allows Process.GetInput(),
 * standard output will be available as `prop::process.STDOUT_POINTER` and
 * allows Process.Read() and Process.GetOutput(), and standard error will be
 * available as `prop::process.STDERR_POINTER` in the properties for the created
 * process.
 *
 * If a standard I/O stream is set to PROCESS_STDIO_REDIRECT, it is connected to
 * an existing IOStream provided by the application. Standard input is provided
 * using `prop::process.CREATE_STDIN_POINTER`, standard output is provided using
 * `prop::process.CREATE_STDOUT_POINTER`, and standard error is provided using
 * `prop::process.CREATE_STDERR_POINTER` in the creation properties. These
 * existing streams should be closed by the application once the new process is
 * created.
 *
 * In order to use an IOStream with PROCESS_STDIO_REDIRECT, it must have
 * `prop::IOStream.WINDOWS_HANDLE_POINTER` or
 * `prop::IOStream.FILE_DESCRIPTOR_NUMBER` set. This is true for streams
 * representing files and process I/O.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa Process.Process
 * @sa Process.GetProperties
 * @sa Process.Read
 * @sa Process.GetInput
 * @sa Process.GetOutput
 */
using ProcessIO = SDL_ProcessIO;

constexpr ProcessIO PROCESS_STDIO_INHERITED =
  SDL_PROCESS_STDIO_INHERITED; ///< The I/O stream is inherited from the
                               ///< application.

constexpr ProcessIO PROCESS_STDIO_NULL =
  SDL_PROCESS_STDIO_NULL; ///< The I/O stream is ignored.

/**
 * The I/O stream is connected to a new IOStream that the application can read
 * or write
 */
constexpr ProcessIO PROCESS_STDIO_APP = SDL_PROCESS_STDIO_APP;

/// The I/O stream is redirected to an existing IOStream.
constexpr ProcessIO PROCESS_STDIO_REDIRECT = SDL_PROCESS_STDIO_REDIRECT;

/**
 * An opaque handle representing a system process.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa Process.Process
 *
 * @cat resource
 */
class Process
{
  ProcessRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Process(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from ProcessParam.
   *
   * @param resource a ProcessRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Process(const ProcessRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr Process(const Process& other) noexcept = default;

public:
  /// Move constructor
  constexpr Process(Process&& other) noexcept
    : Process(other.release())
  {
  }

  constexpr Process(const ProcessRef& other) = delete;

  constexpr Process(ProcessRef&& other) = delete;

  /**
   * Create a new process.
   *
   * The path to the executable is supplied in args[0]. args[1..N] are
   * additional arguments passed on the command line of the new process, and the
   * argument list should be terminated with a nullptr, e.g.:
   *
   * ```c
   * const char *args[] = { "myprogram", "argument", nullptr };
   * ```
   *
   * Setting pipe_stdio to true is equivalent to setting
   * `prop::process.CREATE_STDIN_NUMBER` and
   * `prop::process.CREATE_STDOUT_NUMBER` to `PROCESS_STDIO_APP`, and will allow
   * the use of Process.Read() or Process.GetInput() and Process.GetOutput().
   *
   * See Process.Process() for more details.
   *
   * @param args the path and arguments for the new process.
   * @param pipe_stdio true to create pipes to the process's standard input and
   *                   from the process's standard output, false for the process
   *                   to have no input and inherit the application's standard
   *                   output.
   * @post the newly created and running process, or nullptr if the process
   *       couldn't be created.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa Process.GetProperties
   * @sa Process.Read
   * @sa Process.GetInput
   * @sa Process.GetOutput
   * @sa Process.Kill
   * @sa Process.Wait
   * @sa Process.Destroy
   */
  Process(const char* const* args, bool pipe_stdio)
    : m_resource(SDL_CreateProcess(args, pipe_stdio))
  {
  }

  /**
   * Create a new process with the specified properties.
   *
   * These are the supported properties:
   *
   * - `prop::process.CREATE_ARGS_POINTER`: an array of strings containing the
   *   program to run, any arguments, and a nullptr pointer, e.g. const char
   *   *args[] = { "myprogram", "argument", nullptr }. This is a required
   *   property.
   * - `prop::process.CREATE_ENVIRONMENT_POINTER`: an Environment pointer. If
   *   this property is set, it will be the entire environment for the process,
   *   otherwise the current environment is used.
   * - `prop::process.CREATE_WORKING_DIRECTORY_STRING`: a UTF-8 encoded string
   *   representing the working directory for the process, defaults to the
   *   current working directory.
   * - `prop::process.CREATE_STDIN_NUMBER`: an ProcessIO value describing where
   *   standard input for the process comes from, defaults to
   *   `SDL_PROCESS_STDIO_nullptr`.
   * - `prop::process.CREATE_STDIN_POINTER`: an IOStream pointer used for
   *   standard input when `prop::process.CREATE_STDIN_NUMBER` is set to
   *   `PROCESS_STDIO_REDIRECT`.
   * - `prop::process.CREATE_STDOUT_NUMBER`: an ProcessIO value describing where
   *   standard output for the process goes to, defaults to
   *   `PROCESS_STDIO_INHERITED`.
   * - `prop::process.CREATE_STDOUT_POINTER`: an IOStream pointer used for
   *   standard output when `prop::process.CREATE_STDOUT_NUMBER` is set to
   *   `PROCESS_STDIO_REDIRECT`.
   * - `prop::process.CREATE_STDERR_NUMBER`: an ProcessIO value describing where
   *   standard error for the process goes to, defaults to
   *   `PROCESS_STDIO_INHERITED`.
   * - `prop::process.CREATE_STDERR_POINTER`: an IOStream pointer used for
   *   standard error when `prop::process.CREATE_STDERR_NUMBER` is set to
   *   `PROCESS_STDIO_REDIRECT`.
   * - `prop::process.CREATE_STDERR_TO_STDOUT_BOOLEAN`: true if the error output
   *   of the process should be redirected into the standard output of the
   *   process. This property has no effect if
   *   `prop::process.CREATE_STDERR_NUMBER` is set.
   * - `prop::process.CREATE_BACKGROUND_BOOLEAN`: true if the process should run
   *   in the background. In this case the default input and output is
   *   `SDL_PROCESS_STDIO_nullptr` and the exitcode of the process is not
   *   available, and will always be 0.
   * - `prop::process.CREATE_CMDLINE_STRING`: a string containing the program to
   *   run and any parameters. This string is passed directly to `CreateProcess`
   *   on Windows, and does nothing on other platforms. This property is only
   *   important if you want to start programs that does non-standard
   *   command-line processing, and in most cases using
   *   `prop::process.CREATE_ARGS_POINTER` is sufficient.
   *
   * On POSIX platforms, wait() and waitpid(-1, ...) should not be called, and
   * SIGCHLD should not be ignored or handled because those would prevent SDL
   * from properly tracking the lifetime of the underlying process. You should
   * use Process.Wait() instead.
   *
   * @param props the properties to use.
   * @post the newly created and running process, or nullptr if the process
   *       couldn't be created.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa Process.GetProperties
   * @sa Process.Read
   * @sa Process.GetInput
   * @sa Process.GetOutput
   * @sa Process.Kill
   * @sa Process.Wait
   * @sa Process.Destroy
   */
  Process(PropertiesParam props)
    : m_resource(SDL_CreateProcessWithProperties(props))
  {
  }

  /// Destructor
  ~Process() { SDL_DestroyProcess(m_resource); }

  /// Assignment operator.
  constexpr Process& operator=(Process&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Process& operator=(const Process& other) noexcept = default;

public:
  /// Retrieves underlying ProcessRaw.
  constexpr ProcessRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying ProcessRaw and clear this.
  constexpr ProcessRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Process& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to ProcessParam
  constexpr operator ProcessParam() const noexcept { return {m_resource}; }

  /**
   * Destroy a previously created process object.
   *
   * Note that this does not stop the process, just destroys the SDL object used
   * to track it. If you want to stop the process you should use Process.Kill().
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa Process.Process
   * @sa Process.Kill
   */
  void Destroy();

  /**
   * Get the properties associated with a process.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `prop::process.PID_NUMBER`: the process ID of the process.
   * - `prop::process.STDIN_POINTER`: an IOStream that can be used to write
   *   input to the process, if it was created with
   *   `prop::process.CREATE_STDIN_NUMBER` set to `PROCESS_STDIO_APP`.
   * - `prop::process.STDOUT_POINTER`: a non-blocking IOStream that can be used
   *   to read output from the process, if it was created with
   *   `prop::process.CREATE_STDOUT_NUMBER` set to `PROCESS_STDIO_APP`.
   * - `prop::process.STDERR_POINTER`: a non-blocking IOStream that can be used
   *   to read error output from the process, if it was created with
   *   `prop::process.CREATE_STDERR_NUMBER` set to `PROCESS_STDIO_APP`.
   * - `prop::process.BACKGROUND_BOOLEAN`: true if the process is running in the
   *   background.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa Process.Process
   */
  PropertiesRef GetProperties() const;

  /**
   * Read all the output from a process.
   *
   * If a process was created with I/O enabled, you can use this function to
   * read the output. This function blocks until the process is complete,
   * capturing all output, and providing the process exit code.
   *
   * The data is allocated with a zero byte at the end (null terminated) for
   * convenience. This extra byte is not included in the value reported via
   * `datasize`.
   *
   * The data should be freed with free().
   *
   * @param exitcode a pointer filled in with the process exit code if the
   *                 process has exited, may be nullptr.
   * @returns the data or nullptr on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa Process.Process
   * @sa Process.Destroy
   */
  StringResult Read(int* exitcode = nullptr);

  /**
   * Read all the output from a process.
   *
   * If a process was created with I/O enabled, you can use this function to
   * read the output. This function blocks until the process is complete,
   * capturing all output, and providing the process exit code.
   *
   * The data is allocated with a zero byte at the end (null terminated) for
   * convenience. This extra byte is not included in the value reported via
   * `datasize`.
   *
   * The data should be freed with free().
   *
   * @param exitcode a pointer filled in with the process exit code if the
   *                 process has exited, may be nullptr.
   * @returns the data or nullptr on failure; call GetError() for more
   *          information.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ProcessRef.Create
   */
  template<class T>
  OwnArray<T> ReadAs(int* exitcode = nullptr)
  {
    StringResult data = Read(exitcode);
    if (data.empty()) return {};
    size_t sz = data.size() / sizeof(T);
    return OwnArray{static_cast<T*>(data.release()), sz};
  }

  /**
   * Get the IOStream associated with process standard input.
   *
   * The process must have been created with Process.Process() and pipe_stdio
   * set to true, or with Process.Process() and
   * `prop::process.CREATE_STDIN_NUMBER` set to `PROCESS_STDIO_APP`.
   *
   * Writing to this stream can return less data than expected if the process
   * hasn't read its input. It may be blocked waiting for its output to be read,
   * if so you may need to call Process.GetOutput() and read the output in
   * parallel with writing input.
   *
   * @returns the input stream or nullptr on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa Process.Process
   * @sa Process.GetOutput
   */
  IOStreamRef GetInput();

  /**
   * Get the IOStream associated with process standard output.
   *
   * The process must have been created with Process.Process() and pipe_stdio
   * set to true, or with Process.Process() and
   * `prop::process.CREATE_STDOUT_NUMBER` set to `PROCESS_STDIO_APP`.
   *
   * Reading from this stream can return 0 with IOStream.GetStatus() returning
   * IO_STATUS_NOT_READY if no output is available yet.
   *
   * @returns the output stream or nullptr on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa Process.Process
   * @sa Process.GetInput
   */
  IOStreamRef GetOutput();

  /**
   * Stop a process.
   *
   * @param force true to terminate the process immediately, false to try to
   *              stop the process gracefully. In general you should try to stop
   *              the process gracefully first as terminating a process may
   *              leave it with half-written data or in some other unstable
   *              state.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa Process.Process
   * @sa Process.Wait
   * @sa Process.Destroy
   */
  void Kill(bool force);

  /**
   * Wait for a process to finish.
   *
   * This can be called multiple times to get the status of a process.
   *
   * The exit code will be the exit code of the process if it terminates
   * normally, a negative signal if it terminated due to a signal, or -255
   * otherwise. It will not be changed if the process is still running.
   *
   * If you create a process with standard output piped to the application
   * (`pipe_stdio` being true) then you should read all of the process output
   * before calling Process.Wait(). If you don't do this the process might be
   * blocked indefinitely waiting for output to be read and Process.Wait() will
   * never return true;
   *
   * @param block If true, block until the process finishes; otherwise, report
   *              on the process' status.
   * @param exitcode a pointer filled in with the process exit code if the
   *                 process has exited, may be nullptr.
   * @returns true if the process exited, false otherwise.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa Process.Process
   * @sa Process.Kill
   * @sa Process.Destroy
   */
  bool Wait(bool block, int* exitcode);
};

/// Semi-safe reference for Process.
struct ProcessRef : Process
{
  using Process::Process;

  /**
   * Constructs from ProcessParam.
   *
   * @param resource a ProcessRaw or Process.
   *
   * This does not takes ownership!
   */
  ProcessRef(ProcessParam resource) noexcept
    : Process(resource.value)
  {
  }

  /**
   * Constructs from ProcessParam.
   *
   * @param resource a ProcessRaw or Process.
   *
   * This does not takes ownership!
   */
  ProcessRef(ProcessRaw resource) noexcept
    : Process(resource)
  {
  }

  /// Copy constructor.
  ProcessRef(const ProcessRef& other) noexcept
    : Process(other.get())
  {
  }

  /// Destructor
  ~ProcessRef() { release(); }
};

/**
 * Create a new process.
 *
 * The path to the executable is supplied in args[0]. args[1..N] are additional
 * arguments passed on the command line of the new process, and the argument
 * list should be terminated with a nullptr, e.g.:
 *
 * ```c
 * const char *args[] = { "myprogram", "argument", nullptr };
 * ```
 *
 * Setting pipe_stdio to true is equivalent to setting
 * `prop::process.CREATE_STDIN_NUMBER` and `prop::process.CREATE_STDOUT_NUMBER`
 * to `PROCESS_STDIO_APP`, and will allow the use of Process.Read() or
 * Process.GetInput() and Process.GetOutput().
 *
 * See Process.Process() for more details.
 *
 * @param args the path and arguments for the new process.
 * @param pipe_stdio true to create pipes to the process's standard input and
 *                   from the process's standard output, false for the process
 *                   to have no input and inherit the application's standard
 *                   output.
 * @returns the newly created and running process, or nullptr if the process
 *          couldn't be created.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Process.Process
 * @sa Process.GetProperties
 * @sa Process.Read
 * @sa Process.GetInput
 * @sa Process.GetOutput
 * @sa Process.Kill
 * @sa Process.Wait
 * @sa Process.Destroy
 */
inline Process CreateProcess(const char* const* args, bool pipe_stdio)
{
  return Process(args, pipe_stdio);
}

/**
 * Create a new process with the specified properties.
 *
 * These are the supported properties:
 *
 * - `prop::process.CREATE_ARGS_POINTER`: an array of strings containing the
 *   program to run, any arguments, and a nullptr pointer, e.g. const char
 *   *args[] = { "myprogram", "argument", nullptr }. This is a required
 *   property.
 * - `prop::process.CREATE_ENVIRONMENT_POINTER`: an Environment pointer. If this
 *   property is set, it will be the entire environment for the process,
 *   otherwise the current environment is used.
 * - `prop::process.CREATE_WORKING_DIRECTORY_STRING`: a UTF-8 encoded string
 *   representing the working directory for the process, defaults to the current
 *   working directory.
 * - `prop::process.CREATE_STDIN_NUMBER`: an ProcessIO value describing where
 *   standard input for the process comes from, defaults to
 *   `SDL_PROCESS_STDIO_nullptr`.
 * - `prop::process.CREATE_STDIN_POINTER`: an IOStream pointer used for standard
 *   input when `prop::process.CREATE_STDIN_NUMBER` is set to
 *   `PROCESS_STDIO_REDIRECT`.
 * - `prop::process.CREATE_STDOUT_NUMBER`: an ProcessIO value describing where
 *   standard output for the process goes to, defaults to
 *   `PROCESS_STDIO_INHERITED`.
 * - `prop::process.CREATE_STDOUT_POINTER`: an IOStream pointer used for
 *   standard output when `prop::process.CREATE_STDOUT_NUMBER` is set to
 *   `PROCESS_STDIO_REDIRECT`.
 * - `prop::process.CREATE_STDERR_NUMBER`: an ProcessIO value describing where
 *   standard error for the process goes to, defaults to
 *   `PROCESS_STDIO_INHERITED`.
 * - `prop::process.CREATE_STDERR_POINTER`: an IOStream pointer used for
 *   standard error when `prop::process.CREATE_STDERR_NUMBER` is set to
 *   `PROCESS_STDIO_REDIRECT`.
 * - `prop::process.CREATE_STDERR_TO_STDOUT_BOOLEAN`: true if the error output
 *   of the process should be redirected into the standard output of the
 *   process. This property has no effect if
 *   `prop::process.CREATE_STDERR_NUMBER` is set.
 * - `prop::process.CREATE_BACKGROUND_BOOLEAN`: true if the process should run
 *   in the background. In this case the default input and output is
 *   `SDL_PROCESS_STDIO_nullptr` and the exitcode of the process is not
 *   available, and will always be 0.
 * - `prop::process.CREATE_CMDLINE_STRING`: a string containing the program to
 *   run and any parameters. This string is passed directly to `CreateProcess`
 *   on Windows, and does nothing on other platforms. This property is only
 *   important if you want to start programs that does non-standard command-line
 *   processing, and in most cases using `prop::process.CREATE_ARGS_POINTER` is
 *   sufficient.
 *
 * On POSIX platforms, wait() and waitpid(-1, ...) should not be called, and
 * SIGCHLD should not be ignored or handled because those would prevent SDL from
 * properly tracking the lifetime of the underlying process. You should use
 * Process.Wait() instead.
 *
 * @param props the properties to use.
 * @returns the newly created and running process, or nullptr if the process
 *          couldn't be created.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Process.Process
 * @sa Process.GetProperties
 * @sa Process.Read
 * @sa Process.GetInput
 * @sa Process.GetOutput
 * @sa Process.Kill
 * @sa Process.Wait
 * @sa Process.Destroy
 */
inline Process CreateProcessWithProperties(PropertiesParam props)
{
  return Process(props);
}

namespace prop::process {

constexpr auto CREATE_ARGS_POINTER = SDL_PROP_PROCESS_CREATE_ARGS_POINTER;

constexpr auto CREATE_ENVIRONMENT_POINTER =
  SDL_PROP_PROCESS_CREATE_ENVIRONMENT_POINTER;

#if SDL_VERSION_ATLEAST(3, 4, 0)

constexpr auto CREATE_WORKING_DIRECTORY_STRING =
  SDL_PROP_PROCESS_CREATE_WORKING_DIRECTORY_STRING;

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

constexpr auto CREATE_STDIN_NUMBER = SDL_PROP_PROCESS_CREATE_STDIN_NUMBER;

constexpr auto CREATE_STDIN_POINTER = SDL_PROP_PROCESS_CREATE_STDIN_POINTER;

constexpr auto CREATE_STDOUT_NUMBER = SDL_PROP_PROCESS_CREATE_STDOUT_NUMBER;

constexpr auto CREATE_STDOUT_POINTER = SDL_PROP_PROCESS_CREATE_STDOUT_POINTER;

constexpr auto CREATE_STDERR_NUMBER = SDL_PROP_PROCESS_CREATE_STDERR_NUMBER;

constexpr auto CREATE_STDERR_POINTER = SDL_PROP_PROCESS_CREATE_STDERR_POINTER;

constexpr auto CREATE_STDERR_TO_STDOUT_BOOLEAN =
  SDL_PROP_PROCESS_CREATE_STDERR_TO_STDOUT_BOOLEAN;

constexpr auto CREATE_BACKGROUND_BOOLEAN =
  SDL_PROP_PROCESS_CREATE_BACKGROUND_BOOLEAN;

#if SDL_VERSION_ATLEAST(3, 4, 0)

constexpr auto CREATE_CMDLINE_STRING = SDL_PROP_PROCESS_CREATE_CMDLINE_STRING;

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

constexpr auto PID_NUMBER = SDL_PROP_PROCESS_PID_NUMBER;

constexpr auto STDIN_POINTER = SDL_PROP_PROCESS_STDIN_POINTER;

constexpr auto STDOUT_POINTER = SDL_PROP_PROCESS_STDOUT_POINTER;

constexpr auto STDERR_POINTER = SDL_PROP_PROCESS_STDERR_POINTER;

constexpr auto BACKGROUND_BOOLEAN = SDL_PROP_PROCESS_BACKGROUND_BOOLEAN;

} // namespace prop::process

/**
 * Get the properties associated with a process.
 *
 * The following read-only properties are provided by SDL:
 *
 * - `prop::process.PID_NUMBER`: the process ID of the process.
 * - `prop::process.STDIN_POINTER`: an IOStream that can be used to write input
 *   to the process, if it was created with `prop::process.CREATE_STDIN_NUMBER`
 *   set to `PROCESS_STDIO_APP`.
 * - `prop::process.STDOUT_POINTER`: a non-blocking IOStream that can be used to
 *   read output from the process, if it was created with
 *   `prop::process.CREATE_STDOUT_NUMBER` set to `PROCESS_STDIO_APP`.
 * - `prop::process.STDERR_POINTER`: a non-blocking IOStream that can be used to
 *   read error output from the process, if it was created with
 *   `prop::process.CREATE_STDERR_NUMBER` set to `PROCESS_STDIO_APP`.
 * - `prop::process.BACKGROUND_BOOLEAN`: true if the process is running in the
 *   background.
 *
 * @param process the process to query.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Process.Process
 * @sa Process.Process
 */
inline PropertiesRef GetProcessProperties(ProcessParam process)
{
  return {CheckError(SDL_GetProcessProperties(process))};
}

inline PropertiesRef Process::GetProperties() const
{
  return SDL::GetProcessProperties(m_resource);
}

/**
 * Read all the output from a process.
 *
 * If a process was created with I/O enabled, you can use this function to read
 * the output. This function blocks until the process is complete, capturing all
 * output, and providing the process exit code.
 *
 * The data is allocated with a zero byte at the end (null terminated) for
 * convenience. This extra byte is not included in the value reported via
 * `datasize`.
 *
 * The data should be freed with free().
 *
 * @param process The process to read.
 * @param exitcode a pointer filled in with the process exit code if the process
 *                 has exited, may be nullptr.
 * @returns the data or nullptr on failure; call GetError() for more
 *          information.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Process.Process
 * @sa Process.Process
 * @sa Process.Destroy
 */
inline StringResult ReadProcess(ProcessParam process, int* exitcode = nullptr)
{
  size_t size = 0;
  auto data = static_cast<char*>(SDL_ReadProcess(process, &size, exitcode));
  return StringResult(CheckError(data), size);
}

inline StringResult Process::Read(int* exitcode)
{
  return SDL::ReadProcess(m_resource, exitcode);
}

/**
 * Get the IOStream associated with process standard input.
 *
 * The process must have been created with Process.Process() and pipe_stdio set
 * to true, or with Process.Process() and `prop::process.CREATE_STDIN_NUMBER`
 * set to `PROCESS_STDIO_APP`.
 *
 * Writing to this stream can return less data than expected if the process
 * hasn't read its input. It may be blocked waiting for its output to be read,
 * if so you may need to call Process.GetOutput() and read the output in
 * parallel with writing input.
 *
 * @param process The process to get the input stream for.
 * @returns the input stream or nullptr on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Process.Process
 * @sa Process.Process
 * @sa Process.GetOutput
 */
inline IOStreamRef GetProcessInput(ProcessParam process)
{
  return {SDL_GetProcessInput(process)};
}

inline IOStreamRef Process::GetInput()
{
  return SDL::GetProcessInput(m_resource);
}

/**
 * Get the IOStream associated with process standard output.
 *
 * The process must have been created with Process.Process() and pipe_stdio set
 * to true, or with Process.Process() and `prop::process.CREATE_STDOUT_NUMBER`
 * set to `PROCESS_STDIO_APP`.
 *
 * Reading from this stream can return 0 with IOStream.GetStatus() returning
 * IO_STATUS_NOT_READY if no output is available yet.
 *
 * @param process The process to get the output stream for.
 * @returns the output stream or nullptr on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Process.Process
 * @sa Process.Process
 * @sa Process.GetInput
 */
inline IOStreamRef GetProcessOutput(ProcessParam process)
{
  return {SDL_GetProcessOutput(process)};
}

inline IOStreamRef Process::GetOutput()
{
  return SDL::GetProcessOutput(m_resource);
}

/**
 * Stop a process.
 *
 * @param process The process to stop.
 * @param force true to terminate the process immediately, false to try to stop
 *              the process gracefully. In general you should try to stop the
 *              process gracefully first as terminating a process may leave it
 *              with half-written data or in some other unstable state.
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Process.Process
 * @sa Process.Process
 * @sa Process.Wait
 * @sa Process.Destroy
 */
inline void KillProcess(ProcessParam process, bool force)
{
  CheckError(SDL_KillProcess(process, force));
}

inline void Process::Kill(bool force) { SDL::KillProcess(m_resource, force); }

/**
 * Wait for a process to finish.
 *
 * This can be called multiple times to get the status of a process.
 *
 * The exit code will be the exit code of the process if it terminates normally,
 * a negative signal if it terminated due to a signal, or -255 otherwise. It
 * will not be changed if the process is still running.
 *
 * If you create a process with standard output piped to the application
 * (`pipe_stdio` being true) then you should read all of the process output
 * before calling Process.Wait(). If you don't do this the process might be
 * blocked indefinitely waiting for output to be read and Process.Wait() will
 * never return true;
 *
 * @param process The process to wait for.
 * @param block If true, block until the process finishes; otherwise, report on
 *              the process' status.
 * @param exitcode a pointer filled in with the process exit code if the process
 *                 has exited, may be nullptr.
 * @returns true if the process exited, false otherwise.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Process.Process
 * @sa Process.Process
 * @sa Process.Kill
 * @sa Process.Destroy
 */
inline bool WaitProcess(ProcessParam process, bool block, int* exitcode)
{
  return SDL_WaitProcess(process, block, exitcode);
}

inline bool Process::Wait(bool block, int* exitcode)
{
  return SDL::WaitProcess(m_resource, block, exitcode);
}

/**
 * Destroy a previously created process object.
 *
 * Note that this does not stop the process, just destroys the SDL object used
 * to track it. If you want to stop the process you should use Process.Kill().
 *
 * @param process The process object to destroy.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Process.Process
 * @sa Process.Process
 * @sa Process.Kill
 */
inline void DestroyProcess(ProcessRaw process) { SDL_DestroyProcess(process); }

inline void Process::Destroy() { DestroyProcess(release()); }

/// @}

} // namespace SDL

#endif /* SDL3PP_PROCESS_H_ */
