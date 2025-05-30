#ifndef SDL3PP_PROCESS_H_
#define SDL3PP_PROCESS_H_

#include <SDL3/SDL_process.h>
#include "SDL3pp_iostream.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 *
 * @defgroup CategoryProcess Process Control
 *
 * Process control support.
 *
 * These functions provide a cross-platform way to spawn and manage OS-level
 * processes.
 *
 * You can create a new subprocess with Process.Process() and optionally
 * read and write to it using Process.Read() or Process.GetInput() and
 * Process.GetOutput(). If more advanced functionality like chaining input
 * between processes is necessary, you can use
 * Process.Process().
 *
 * You can get the status of a created process with Process.Wait(), or
 * terminate the process with Process.Kill().
 *
 * Don't forget to call Process.reset() to clean up, whether the process
 * process was killed, terminated on its own, or is still running!
 *
 * @{
 */

// Forward decl
struct ProcessRef;

// Forward decl
struct Process;

/**
 * Description of where standard I/O should be directed when creating a
 * process.
 *
 * If a standard I/O stream is set to PROCESS_STDIO_INHERITED, it will go
 * to the same place as the application's I/O stream. This is the default for
 * standard output and standard error.
 *
 * If a standard I/O stream is set to SDL_PROCESS_STDIO_NULL, it is connected
 * to `NUL:` on Windows and `/dev/null` on POSIX systems. This is the default
 * for standard input.
 *
 * If a standard I/O stream is set to PROCESS_STDIO_APP, it is connected
 * to a new IOStreamRef that is available to the application. Standard input
 * will be available as `prop::process.STDIN_POINTER` and allows
 * ProcessRef.GetInput(), standard output will be available as
 * `prop::process.STDOUT_POINTER` and allows ProcessRef.Read() and
 * ProcessRef.GetOutput(), and standard error will be available as
 * `prop::process.STDERR_POINTER` in the properties for the created
 * process.
 *
 * If a standard I/O stream is set to PROCESS_STDIO_REDIRECT, it is
 * connected to an existing IOStreamRef provided by the application. Standard
 * input is provided using `prop::process.CREATE_STDIN_POINTER`, standard
 * output is provided using `prop::process.CREATE_STDOUT_POINTER`, and
 * standard error is provided using `prop::process.CREATE_STDERR_POINTER`
 * in the creation properties. These existing streams should be closed by the
 * application once the new process is created.
 *
 * In order to use an IOStreamRef with PROCESS_STDIO_REDIRECT, it must
 * have `prop::IOStream.WINDOWS_HANDLE_POINTER` or
 * `prop::IOStream.FILE_DESCRIPTOR_NUMBER` set. This is true for streams
 * representing files and process I/O.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa ProcessRef.ProcessRef
 * @sa ProcessRef.GetProperties
 * @sa ProcessRef.Read
 * @sa ProcessRef.GetInput
 * @sa ProcessRef.GetOutput
 */
using ProcessIO = SDL_ProcessIO;

constexpr ProcessIO PROCESS_STDIO_INHERITED =
  SDL_PROCESS_STDIO_INHERITED; ///< The I/O stream is inherited from the
                               ///< application.

constexpr ProcessIO PROCESS_STDIO_NULL =
  SDL_PROCESS_STDIO_NULL; ///< The I/O stream is ignored.

/**
 * The I/O stream is connected to a new IOStreamRef that the application can
 * read or write.
 */
constexpr ProcessIO PROCESS_STDIO_APP = SDL_PROCESS_STDIO_APP;

/**
 * The I/O stream is redirected to an existing IOStreamRef.
 */
constexpr ProcessIO PROCESS_STDIO_REDIRECT = SDL_PROCESS_STDIO_REDIRECT;

/**
 * An opaque handle representing a system process.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa ProcessRef.ProcessRef
 *
 * @cat resource
 *
 * @sa Process
 * @sa ProcessRef
 */
struct ProcessRef : Resource<SDL_Process*>
{
  using Resource::Resource;

  /**
   * Get the properties associated with a process.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `prop::process.PID_NUMBER`: the process ID of the process.
   * - `prop::process.STDIN_POINTER`: an IOStreamRef that can be used to
   *   write input to the process, if it was created with
   *   `prop::process.CREATE_STDIN_NUMBER` set to `PROCESS_STDIO_APP`.
   * - `prop::process.STDOUT_POINTER`: a non-blocking IOStreamRef that can
   *   be used to read output from the process, if it was created with
   *   `prop::process.CREATE_STDOUT_NUMBER` set to `PROCESS_STDIO_APP`.
   * - `prop::process.STDERR_POINTER`: a non-blocking IOStreamRef that can
   *   be used to read error output from the process, if it was created with
   *   `prop::process.CREATE_STDERR_NUMBER` set to `PROCESS_STDIO_APP`.
   * - `prop::process.BACKGROUND_BOOLEAN`: true if the process is running in
   *   the background.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   */
  PropertiesRef GetProperties() const
  {
    return CheckError(SDL_GetProcessProperties(get()));
  }

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
   * @returns the data on success.
   * @throws Error on failure.
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa ProcessRef.Destroy
   */
  StringResult Read(int* exitcode = nullptr)
  {
    size_t size = 0;
    auto data = static_cast<char*>(SDL_ReadProcess(get(), &size, exitcode));
    return StringResult(CheckError(data), size);
  }

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
   * @sa ProcessRef.ProcessRef
   */
  template<class T>
  OwnArray<T> ReadAs(int* exitcode = nullptr)
  {
    OwnArray<std::byte> data = Read(exitcode);
    if (data.empty()) return {};
    size_t sz = data.size() / sizeof(T);
    return OwnArray{static_cast<T*>(data.release()), sz};
  }

  /**
   * Get the IOStreamRef associated with process standard input.
   *
   * The process must have been created with Process.Process() and pipe_stdio
   * set to true, or with Process.Process() and
   * `prop::process.CREATE_STDIN_NUMBER` set to `PROCESS_STDIO_APP`.
   *
   * Writing to this stream can return less data than expected if the process
   * hasn't read its input. It may be blocked waiting for its output to be read,
   * if so you may need to call ProcessRef.GetOutput() and read the output in
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
   * @sa ProcessRef.GetOutput
   */
  IOStreamRef GetInput() { return SDL_GetProcessInput(get()); }

  /**
   * Get the IOStreamRef associated with process standard output.
   *
   * The process must have been created with Process.Process() and pipe_stdio
   * set to true, or with Process.Process() and
   * `prop::process.CREATE_STDOUT_NUMBER` set to `PROCESS_STDIO_APP`.
   *
   * Reading from this stream can return 0 with IOStreamRef.GetStatus()
   * returning IO_STATUS_NOT_READY if no output is available yet.
   *
   * @returns the output stream or nullptr on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa ProcessRef.GetInput
   */
  IOStreamRef GetOutput() { return SDL_GetProcessOutput(get()); }

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
   * @sa ProcessRef.Wait
   * @sa ProcessRef.Destroy
   */
  void Kill(bool force) { CheckError(SDL_KillProcess(get(), force)); }

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
   * before calling ProcessRef.Wait(). If you don't do this the process might be
   * blocked indefinitely waiting for output to be read and ProcessRef.Wait()
   * will never return true;
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
   * @sa ProcessRef.Kill
   * @sa ProcessRef.Destroy
   */
  bool Wait(bool block, int* exitcode)
  {
    return SDL_WaitProcess(get(), block, exitcode);
  }

protected:
  /**
   * Destroy a previously created process object.
   *
   * Note that this does not stop the process, just destroys the SDL object used
   * to track it. If you want to stop the process you should use
   * ProcessRef.Kill().
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa ProcessRef.Kill
   */
  void Destroy() { reset(); }

  /**
   * Destroy a previously created process object.
   *
   * Note that this does not stop the process, just destroys the SDL object used
   * to track it. If you want to stop the process you should use
   * ProcessRef.Kill().
   *
   * @threadsafety This function is not thread safe.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Process.Process
   * @sa ProcessRef.Kill
   */
  void reset(SDL_Process* newResource = {})
  {
    SDL_DestroyProcess(release(newResource));
  }
};

/**
 * Unsafe Handle to process
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa ProcessRef
 */
struct ProcessUnsafe : ProcessRef
{
  using ProcessRef::Destroy;

  using ProcessRef::ProcessRef;

  using ProcessRef::reset;

  /**
   * Constructs ProcessUnsafe from ProcessRef.
   */
  constexpr ProcessUnsafe(const ProcessRef& other)
    : ProcessRef(other.get())
  {
  }

  ProcessUnsafe(const Process& other) = delete;

  /**
   * Constructs ProcessUnsafe from Process.
   */
  constexpr explicit ProcessUnsafe(Process&& other);
  /**
   * Assignment operator.
   */

  constexpr ProcessUnsafe& operator=(ProcessUnsafe other)
  {
    release(other.release());
    return *this;
  }
};

/**
 * Handle to an owned process
 *
 * @cat resource
 *
 * @sa ProcessRef
 */
struct Process : ProcessUnsafe
{
  using ProcessUnsafe::ProcessUnsafe;

  /**
   * Constructs an empty Process.
   */
  constexpr Process()
    : ProcessUnsafe(nullptr)
  {
  }

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit Process(SDL_Process* resource)
    : ProcessUnsafe(resource)
  {
  }

  constexpr Process(const Process& other) = delete;

  /**
   * Move constructor.
   */
  constexpr Process(Process&& other)
    : Process(other.release())
  {
  }

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
   * `prop::process.CREATE_STDOUT_NUMBER` to `PROCESS_STDIO_APP`, and
   * will allow the use of ProcessRef.Read() or ProcessRef.GetInput() and
   * ProcessRef.GetOutput().
   *
   * See Process.Process() for more details.
   *
   * @param args the path and arguments for the new process.
   * @param pipe_stdio true to create pipes to the process's standard input and
   *                   from the process's standard output, false for the process
   *                   to have no input and inherit the application's standard
   *                   output.
   * @post the newly created and running process.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ProcessRef.GetProperties
   * @sa ProcessRef.Read
   * @sa ProcessRef.GetInput
   * @sa ProcessRef.GetOutput
   * @sa ProcessRef.Kill
   * @sa ProcessRef.Wait
   * @sa ProcessRef.Destroy
   */
  Process(const char* const* args, bool pipe_stdio)
    : Process(CheckError(SDL_CreateProcess(args, pipe_stdio)))
  {
  }

  /**
   * Create a new process with the specified properties.
   *
   * These are the supported properties:
   *
   * - `prop::process.CREATE_ARGS_POINTER`: an array of strings containing
   *   the program to run, any arguments, and a nullptr pointer, e.g. const char
   *   *args[] = { "myprogram", "argument", nullptr }. This is a required
   * property.
   * - `prop::process.CREATE_ENVIRONMENT_POINTER`: an EnvironmentRef
   *   pointer. If this property is set, it will be the entire environment for
   *   the process, otherwise the current environment is used.
   * - `prop::process.CREATE_STDIN_NUMBER`: an ProcessIO value describing
   *   where standard input for the process comes from, defaults to
   *   `SDL_PROCESS_STDIO_NULL`.
   * - `prop::process.CREATE_STDIN_POINTER`: an IOStreamRef pointer used for
   *   standard input when `prop::process.CREATE_STDIN_NUMBER` is set to
   *   `PROCESS_STDIO_REDIRECT`.
   * - `prop::process.CREATE_STDOUT_NUMBER`: an ProcessIO value
   *   describing where standard output for the process goes to, defaults to
   *   `PROCESS_STDIO_INHERITED`.
   * - `prop::process.CREATE_STDOUT_POINTER`: an IOStreamRef pointer used
   *   for standard output when `prop::process.CREATE_STDOUT_NUMBER` is set
   *   to `PROCESS_STDIO_REDIRECT`.
   * - `prop::process.CREATE_STDERR_NUMBER`: an ProcessIO value
   *   describing where standard error for the process goes to, defaults to
   *   `PROCESS_STDIO_INHERITED`.
   * - `prop::process.CREATE_STDERR_POINTER`: an IOStreamRef pointer used
   *   for standard error when `prop::process.CREATE_STDERR_NUMBER` is set to
   *   `PROCESS_STDIO_REDIRECT`.
   * - `prop::process.CREATE_STDERR_TO_STDOUT_BOOLEAN`: true if the error
   *   output of the process should be redirected into the standard output of
   *   the process. This property has no effect if
   *   `prop::process.CREATE_STDERR_NUMBER` is set.
   * - `prop::process.CREATE_BACKGROUND_BOOLEAN`: true if the process should
   *   run in the background. In this case the default input and output is
   *   `SDL_PROCESS_STDIO_NULL` and the exitcode of the process is not
   *   available, and will always be 0.
   *
   * On POSIX platforms, wait() and waitpid(-1, ...) should not be called, and
   * SIGCHLD should not be ignored or handled because those would prevent SDL
   * from properly tracking the lifetime of the underlying process. You should
   * use ProcessRef.Wait() instead.
   *
   * @param props the properties to use.
   * @post the newly created and running process.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ProcessRef.GetProperties
   * @sa ProcessRef.Read
   * @sa ProcessRef.GetInput
   * @sa ProcessRef.GetOutput
   * @sa ProcessRef.Kill
   * @sa ProcessRef.Wait
   * @sa ProcessRef.Destroy
   */
  Process(PropertiesRef props)
    : Process(CheckError(SDL_CreateProcessWithProperties(props.get())))
  {
  }

  /**
   * Frees up resource when object goes out of scope.
   */
  ~Process() { reset(); }

  /**
   * Assignment operator.
   */
  Process& operator=(Process other)
  {
    reset(other.release());
    return *this;
  }
};

constexpr ProcessUnsafe::ProcessUnsafe(Process&& other)
  : ProcessUnsafe(other.release())
{
}

namespace prop::process {

constexpr auto CREATE_ARGS_POINTER = SDL_PROP_PROCESS_CREATE_ARGS_POINTER;

constexpr auto CREATE_ENVIRONMENT_POINTER =
  SDL_PROP_PROCESS_CREATE_ENVIRONMENT_POINTER;

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

constexpr auto PID_NUMBER = SDL_PROP_PROCESS_PID_NUMBER;

constexpr auto STDIN_POINTER = SDL_PROP_PROCESS_STDIN_POINTER;

constexpr auto STDOUT_POINTER = SDL_PROP_PROCESS_STDOUT_POINTER;

constexpr auto STDERR_POINTER = SDL_PROP_PROCESS_STDERR_POINTER;

constexpr auto BACKGROUND_BOOLEAN = SDL_PROP_PROCESS_BACKGROUND_BOOLEAN;

} // namespace prop::process

/// @}
} // namespace SDL

#endif /* SDL3PP_PROCESS_H_ */
