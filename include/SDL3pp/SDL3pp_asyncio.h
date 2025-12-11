#ifndef SDL3PP_ASYNCIO_H_
#define SDL3PP_ASYNCIO_H_

#include <SDL3/SDL_asyncio.h>
#include "SDL3pp_log.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryAsyncIO Async I/O
 *
 * SDL offers a way to perform I/O asynchronously. This allows an app to read or
 * write files without waiting for data to actually transfer; the functions that
 * request I/O never block while the request is fulfilled.
 *
 * Instead, the data moves in the background and the app can check for results
 * at their leisure.
 *
 * This is more complicated than just reading and writing files in a synchronous
 * way, but it can allow for more efficiency, and never having framerate drops
 * as the hard drive catches up, etc.
 *
 * The general usage pattern for async I/O is:
 *
 * - Create one or more AsyncIOQueue objects.
 * - Open files with AsyncIO.AsyncIO.
 * - Start I/O tasks to the files with AsyncIO.Read or AsyncIO.Write, putting
 *   those tasks into one of the queues.
 * - Later on, use AsyncIOQueue.GetResult on a queue to see if any task is
 *   finished without blocking. Tasks might finish in any order with success or
 *   failure.
 * - When all your tasks are done, close the file with AsyncIO.Close. This also
 *   generates a task, since it might flush data to disk!
 *
 * This all works, without blocking, in a single thread, but one can also wait
 * on a queue in a background thread, sleeping until new results have arrived:
 *
 * - Call AsyncIOQueue.WaitResult from one or more threads to efficiently block
 *   until new tasks complete.
 * - When shutting down, call AsyncIOQueue.Signal to unblock any sleeping
 *   threads despite there being no new tasks completed.
 *
 * And, of course, to match the synchronous LoadFile, we offer LoadFileAsync as
 * a convenience function. This will handle allocating a buffer, slurping in the
 * file data, and null-terminating it; you still check for results later.
 *
 * Behind the scenes, SDL will use newer, efficient APIs on platforms that
 * support them: Linux's io_uring and Windows 11's IoRing, for example. If those
 * technologies aren't available, SDL will offload the work to a thread pool
 * that will manage otherwise-synchronous loads without blocking the app.
 *
 * ## Best Practices
 *
 * Simple non-blocking I/O--for an app that just wants to pick up data whenever
 * it's ready without losing framerate waiting on disks to spin--can use
 * whatever pattern works well for the program. In this case, simply call
 * AsyncIO.Read, or maybe LoadFileAsync, as needed. Once a frame, call
 * AsyncIOQueue.GetResult to check for any completed tasks and deal with the
 * data as it arrives.
 *
 * If two separate pieces of the same program need their own I/O, it is legal
 * for each to create their own queue. This will prevent either piece from
 * accidentally consuming the other's completed tasks. Each queue does require
 * some amount of resources, but it is not an overwhelming cost. Do not make a
 * queue for each task, however. It is better to put many tasks into a single
 * queue. They will be reported in order of completion, not in the order they
 * were submitted, so it doesn't generally matter what order tasks are started.
 *
 * One async I/O queue can be shared by multiple threads, or one thread can have
 * more than one queue, but the most efficient way--if ruthless efficiency is
 * the goal--is to have one queue per thread, with multiple threads working in
 * parallel, and attempt to keep each queue loaded with tasks that are both
 * started by and consumed by the same thread. On modern platforms that can use
 * newer interfaces, this can keep data flowing as efficiently as possible all
 * the way from storage hardware to the app, with no contention between threads
 * for access to the same queue.
 *
 * Written data is not guaranteed to make it to physical media by the time a
 * closing task is completed, unless AsyncIO.Close is called with its `flush`
 * parameter set to true, which is to say that a successful result here can
 * still result in lost data during an unfortunately-timed power outage if not
 * flushed. However, flushing will take longer and may be unnecessary, depending
 * on the app's needs.
 *
 * @{
 */

// Forward decl
struct AsyncIO;

/// Alias to raw representation for AsyncIO.
using AsyncIORaw = SDL_AsyncIO*;

// Forward decl
struct AsyncIORef;

/// Safely wrap AsyncIO for non owning parameters
struct AsyncIOParam
{
  AsyncIORaw value; ///< parameter's AsyncIORaw

  /// Constructs from AsyncIORaw
  constexpr AsyncIOParam(AsyncIORaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr AsyncIOParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const AsyncIOParam& other) const = default;

  /// Converts to underlying AsyncIORaw
  constexpr operator AsyncIORaw() const { return value; }
};

// Forward decl
struct AsyncIOQueue;

/// Alias to raw representation for AsyncIOQueue.
using AsyncIOQueueRaw = SDL_AsyncIOQueue*;

// Forward decl
struct AsyncIOQueueRef;

/// Safely wrap AsyncIOQueue for non owning parameters
struct AsyncIOQueueParam
{
  AsyncIOQueueRaw value; ///< parameter's AsyncIOQueueRaw

  /// Constructs from AsyncIOQueueRaw
  constexpr AsyncIOQueueParam(AsyncIOQueueRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr AsyncIOQueueParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const AsyncIOQueueParam& other) const = default;

  /// Converts to underlying AsyncIOQueueRaw
  constexpr operator AsyncIOQueueRaw() const { return value; }
};

/**
 * The asynchronous I/O operation structure.
 *
 * This operates as an opaque handle. One can then request read or write
 * operations on it.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa AsyncIO.AsyncIO
 *
 * @cat resource
 */
class AsyncIO
{
  AsyncIORaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr AsyncIO() = default;

  /**
   * Constructs from AsyncIOParam.
   *
   * @param resource a AsyncIORaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit AsyncIO(const AsyncIORaw resource) noexcept
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr AsyncIO(const AsyncIO& other) = delete;

  /// Move constructor
  constexpr AsyncIO(AsyncIO&& other) noexcept
    : AsyncIO(other.release())
  {
  }

  constexpr AsyncIO(const AsyncIORef& other) = delete;

  constexpr AsyncIO(AsyncIORef&& other) = delete;

  /**
   * Use this function to create a new AsyncIO object for reading from and/or
   * writing to a named file.
   *
   * The `mode` string understands the following values:
   *
   * - "r": Open a file for reading only. It must exist.
   * - "w": Open a file for writing only. It will create missing files or
   *   truncate existing ones.
   * - "r+": Open a file for update both reading and writing. The file must
   *   exist.
   * - "w+": Create an empty file for both reading and writing. If a file with
   *   the same name already exists its content is erased and the file is
   *   treated as a new empty file.
   *
   * There is no "b" mode, as there is only "binary" style I/O, and no "a" mode
   * for appending, since you specify the position when starting a task.
   *
   * This function supports Unicode filenames, but they must be encoded in UTF-8
   * format, regardless of the underlying operating system.
   *
   * This call is _not_ asynchronous; it will open the file before returning,
   * under the assumption that doing so is generally a fast operation. Future
   * reads and writes to the opened file will be async, however.
   *
   * @param file a UTF-8 string representing the filename to open.
   * @param mode an ASCII string representing the mode to be used for opening
   *             the file.
   * @post a pointer to the AsyncIO structure that is created or nullptr on
   *       failure; call GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AsyncIO.Close
   * @sa AsyncIO.Read
   * @sa AsyncIO.Write
   */
  AsyncIO(StringParam file, StringParam mode)
    : m_resource(SDL_AsyncIOFromFile(file, mode))
  {
  }

  /// Destructor
  ~AsyncIO()
  {
    if (m_resource) {
      LOG_CATEGORY_ERROR.LogDebug("AsyncIO ID was not properly Destroyed: {}",
                                  (void*)(m_resource));
    }
  }

  /// Assignment operator.
  constexpr AsyncIO& operator=(AsyncIO&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr AsyncIO& operator=(const AsyncIO& other) noexcept = default;

public:
  /// Retrieves underlying AsyncIORaw.
  constexpr AsyncIORaw get() const noexcept { return m_resource; }

  /// Retrieves underlying AsyncIORaw and clear this.
  constexpr AsyncIORaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const AsyncIO& other) const noexcept = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to AsyncIOParam
  constexpr operator AsyncIOParam() const noexcept { return {m_resource}; }

  /**
   * Close and free any allocated resources for an async I/O object.
   *
   * Closing a file is _also_ an asynchronous task! If a write failure were to
   * happen during the closing process, for example, the task results will
   * report it as usual.
   *
   * Closing a file that has been written to does not guarantee the data has
   * made it to physical media; it may remain in the operating system's file
   * cache, for later writing to disk. This means that a successfully-closed
   * file can be lost if the system crashes or loses power in this small window.
   * To prevent this, call this function with the `flush` parameter set to true.
   * This will make the operation take longer, and perhaps increase system load
   * in general, but a successful result guarantees that the data has made it to
   * physical storage. Don't use this for temporary files, caches, and
   * unimportant data, and definitely use it for crucial irreplaceable files,
   * like game saves.
   *
   * This function guarantees that the close will happen after any other pending
   * tasks to `asyncio`, so it's safe to open a file, start several operations,
   * close the file immediately, then check for all results later. This function
   * will not block until the tasks have completed.
   *
   * Once this function returns true, `asyncio` is no longer valid, regardless
   * of any future outcomes. Any completed tasks might still contain this
   * pointer in their AsyncIOOutcome data, in case the app was using this value
   * to track information, but it should not be used again.
   *
   * If this function returns false, the close wasn't started at all, and it's
   * safe to attempt to close again later.
   *
   * An AsyncIOQueue must be specified. The newly-created task will be added to
   * it when it completes its work.
   *
   * @param flush true if data should sync to disk before the task completes.
   * @param queue a queue to add the new AsyncIO to.
   * @param userdata an app-defined pointer that will be provided with the task
   *                 results.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread, but two
   *               threads should not attempt to close the same object.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Close(bool flush, AsyncIOQueueParam queue, void* userdata);

  /**
   * Use this function to get the size of the data stream in an AsyncIO.
   *
   * This call is _not_ asynchronous; it assumes that obtaining this info is a
   * non-blocking operation in most reasonable cases.
   *
   * @returns the size of the data stream in the IOStream on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sint64 GetSize();

  /**
   * Start an async read.
   *
   * This function reads up to `size` bytes from `offset` position in the data
   * source to the area pointed at by `ptr`. This function may read less bytes
   * than requested.
   *
   * This function returns as quickly as possible; it does not wait for the read
   * to complete. On a successful return, this work will continue in the
   * background. If the work begins, even failure is asynchronous: a failing
   * return value from this function only means the work couldn't start at all.
   *
   * `ptr` must remain available until the work is done, and may be accessed by
   * the system at any time until then. Do not allocate it on the stack, as this
   * might take longer than the life of the calling function to complete!
   *
   * An AsyncIOQueue must be specified. The newly-created task will be added to
   * it when it completes its work.
   *
   * @param ptr a pointer to a buffer to read data into.
   * @param offset the position to start reading in the data source.
   * @param size the number of bytes to read from the data source.
   * @param queue a queue to add the new AsyncIO to.
   * @param userdata an app-defined pointer that will be provided with the task
   *                 results.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AsyncIO.Write
   * @sa AsyncIOQueue.AsyncIOQueue
   */
  void Read(void* ptr,
            Uint64 offset,
            Uint64 size,
            AsyncIOQueueParam queue,
            void* userdata);

  /**
   * Start an async write.
   *
   * This function writes `size` bytes from `offset` position in the data source
   * to the area pointed at by `ptr`.
   *
   * This function returns as quickly as possible; it does not wait for the
   * write to complete. On a successful return, this work will continue in the
   * background. If the work begins, even failure is asynchronous: a failing
   * return value from this function only means the work couldn't start at all.
   *
   * `ptr` must remain available until the work is done, and may be accessed by
   * the system at any time until then. Do not allocate it on the stack, as this
   * might take longer than the life of the calling function to complete!
   *
   * An AsyncIOQueue must be specified. The newly-created task will be added to
   * it when it completes its work.
   *
   * @param ptr a pointer to a buffer to write data from.
   * @param offset the position to start writing to the data source.
   * @param size the number of bytes to write to the data source.
   * @param queue a queue to add the new AsyncIO to.
   * @param userdata an app-defined pointer that will be provided with the task
   *                 results.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AsyncIO.Read
   * @sa AsyncIOQueue.AsyncIOQueue
   */
  void Write(void* ptr,
             Uint64 offset,
             Uint64 size,
             AsyncIOQueueParam queue,
             void* userdata);
};

/// Semi-safe reference for AsyncIO.
struct AsyncIORef : AsyncIO
{
  using AsyncIO::AsyncIO;

  /**
   * Constructs from AsyncIOParam.
   *
   * @param resource a AsyncIORaw or AsyncIO.
   *
   * This does not takes ownership!
   */
  AsyncIORef(AsyncIOParam resource) noexcept
    : AsyncIO(resource.value)
  {
  }

  /**
   * Constructs from AsyncIOParam.
   *
   * @param resource a AsyncIORaw or AsyncIO.
   *
   * This does not takes ownership!
   */
  AsyncIORef(AsyncIORaw resource) noexcept
    : AsyncIO(resource)
  {
  }

  /// Copy constructor.
  AsyncIORef(const AsyncIORef& other) noexcept
    : AsyncIO(other.get())
  {
  }

  /// Destructor
  ~AsyncIORef() { release(); }
};

/**
 * Types of asynchronous I/O tasks.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using AsyncIOTaskType = SDL_AsyncIOTaskType;

constexpr AsyncIOTaskType ASYNCIO_TASK_READ =
  SDL_ASYNCIO_TASK_READ; ///< A read operation.

constexpr AsyncIOTaskType ASYNCIO_TASK_WRITE =
  SDL_ASYNCIO_TASK_WRITE; ///< A write operation.

constexpr AsyncIOTaskType ASYNCIO_TASK_CLOSE =
  SDL_ASYNCIO_TASK_CLOSE; ///< A close operation.

/**
 * Possible outcomes of an asynchronous I/O task.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using AsyncIOResult = SDL_AsyncIOResult;

constexpr AsyncIOResult ASYNCIO_COMPLETE =
  SDL_ASYNCIO_COMPLETE; ///< request was completed without error

constexpr AsyncIOResult ASYNCIO_FAILURE =
  SDL_ASYNCIO_FAILURE; ///< request failed for some reason; check GetError()!

constexpr AsyncIOResult ASYNCIO_CANCELED =
  SDL_ASYNCIO_CANCELED; ///< request was canceled before completing.

/**
 * Information about a completed asynchronous I/O request.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using AsyncIOOutcome = SDL_AsyncIOOutcome;

/**
 * A queue of completed asynchronous I/O tasks.
 *
 * When starting an asynchronous operation, you specify a queue for the new
 * task. A queue can be asked later if any tasks in it have completed, allowing
 * an app to manage multiple pending tasks in one place, in whatever order they
 * complete.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa AsyncIOQueue.AsyncIOQueue
 * @sa AsyncIO.Read
 * @sa AsyncIO.Write
 * @sa AsyncIOQueue.GetResult
 * @sa AsyncIOQueue.WaitResult
 *
 * @cat resource
 */
class AsyncIOQueue
{
  AsyncIOQueueRaw m_resource = nullptr;

public:
  /**
   * Constructs from AsyncIOQueueParam.
   *
   * @param resource a AsyncIOQueueRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit AsyncIOQueue(const AsyncIOQueueRaw resource) noexcept
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr AsyncIOQueue(const AsyncIOQueue& other) = delete;

  /// Move constructor
  constexpr AsyncIOQueue(AsyncIOQueue&& other) noexcept
    : AsyncIOQueue(other.release())
  {
  }

  constexpr AsyncIOQueue(const AsyncIOQueueRef& other) = delete;

  constexpr AsyncIOQueue(AsyncIOQueueRef&& other) = delete;

  /**
   * Create a task queue for tracking multiple I/O operations.
   *
   * Async I/O operations are assigned to a queue when started. The queue can be
   * checked for completed tasks thereafter.
   *
   * @post a new task queue object or nullptr if there was an error; call
   *       GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AsyncIOQueue.Destroy
   * @sa AsyncIOQueue.GetResult
   * @sa AsyncIOQueue.WaitResult
   */
  AsyncIOQueue()
    : m_resource(SDL_CreateAsyncIOQueue())
  {
  }

  /// Destructor
  ~AsyncIOQueue() { SDL_DestroyAsyncIOQueue(m_resource); }

  /// Assignment operator.
  constexpr AsyncIOQueue& operator=(AsyncIOQueue&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr AsyncIOQueue& operator=(const AsyncIOQueue& other) noexcept =
    default;

public:
  /// Retrieves underlying AsyncIOQueueRaw.
  constexpr AsyncIOQueueRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying AsyncIOQueueRaw and clear this.
  constexpr AsyncIOQueueRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const AsyncIOQueue& other) const noexcept =
    default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to AsyncIOQueueParam
  constexpr operator AsyncIOQueueParam() const noexcept { return {m_resource}; }

  /**
   * Destroy a previously-created async I/O task queue.
   *
   * If there are still tasks pending for this queue, this call will block until
   * those tasks are finished. All those tasks will be deallocated. Their
   * results will be lost to the app.
   *
   * Any pending reads from LoadFileAsync() that are still in this queue will
   * have their buffers deallocated by this function, to prevent a memory leak.
   *
   * Once this function is called, the queue is no longer valid and should not
   * be used, including by other threads that might access it while destruction
   * is blocking on pending tasks.
   *
   * Do not destroy a queue that still has threads waiting on it through
   * AsyncIOQueue.WaitResult(). You can call AsyncIOQueue.Signal() first to
   * unblock those threads, and take measures (such as Thread.Wait()) to make
   * sure they have finished their wait and won't wait on the queue again.
   *
   * @threadsafety It is safe to call this function from any thread, so long as
   *               no other thread is waiting on the queue with
   *               AsyncIOQueue.WaitResult.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Destroy();

  /**
   * Query an async I/O task queue for completed tasks.
   *
   * If a task assigned to this queue has finished, this will return true and
   * fill in `outcome` with the details of the task. If no task in the queue has
   * finished, this function will return false. This function does not block.
   *
   * If a task has completed, this function will free its resources and the task
   * pointer will no longer be valid. The task will be removed from the queue.
   *
   * It is safe for multiple threads to call this function on the same queue at
   * once; a completed task will only go to one of the threads.
   *
   * @returns details of a finished task if a task has completed, std::nullopt
   *          otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AsyncIOQueue.WaitResult
   */
  std::optional<AsyncIOOutcome> GetResult();

  /**
   * Block until an async I/O task queue has a completed task.
   *
   * This function puts the calling thread to sleep until there a task assigned
   * to the queue that has finished.
   *
   * If a task assigned to the queue has finished, this will return true and
   * fill in `outcome` with the details of the task. If no task in the queue has
   * finished, this function will return false.
   *
   * If a task has completed, this function will free its resources and the task
   * pointer will no longer be valid. The task will be removed from the queue.
   *
   * It is safe for multiple threads to call this function on the same queue at
   * once; a completed task will only go to one of the threads.
   *
   * Note that by the nature of various platforms, more than one waiting thread
   * may wake to handle a single task, but only one will obtain it, so
   * `timeoutMS` is a _maximum_ wait time, and this function may return false
   * sooner.
   *
   * This function may return false if there was a system error, the OS
   * inadvertently awoke multiple threads, or if AsyncIOQueue.Signal() was
   * called to wake up all waiting threads without a finished task.
   *
   * A timeout can be used to specify a maximum wait time, but rather than
   * polling, it is possible to have a timeout of -1 to wait forever, and use
   * AsyncIOQueue.Signal() to wake up the waiting threads later.
   *
   * @param timeout the maximum time to wait, in milliseconds.
   * @returns details of a finished task if a task has completed, std::nullopt
   *          otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AsyncIOQueue.Signal
   */
  std::optional<AsyncIOOutcome> WaitResult(Milliseconds timeout);

  /**
   * Block until an async I/O task queue has a completed task.
   *
   * This function puts the calling thread to sleep until there a task assigned
   * to the queue that has finished.
   *
   * If a task assigned to the queue has finished, this will return true and
   * fill in `outcome` with the details of the task. If no task in the queue has
   * finished, this function will return false.
   *
   * If a task has completed, this function will free its resources and the task
   * pointer will no longer be valid. The task will be removed from the queue.
   *
   * It is safe for multiple threads to call this function on the same queue at
   * once; a completed task will only go to one of the threads.
   *
   * Note that by the nature of various platforms, more than one waiting thread
   * may wake to handle a single task, but only one will obtain it, so
   * `timeoutMS` is a _maximum_ wait time, and this function may return false
   * sooner.
   *
   * This function may return false if there was a system error, the OS
   * inadvertently awoke multiple threads, or if AsyncIOQueue.Signal() was
   * called to wake up all waiting threads without a finished task.
   *
   * A timeout can be used to specify a maximum wait time, but rather than
   * polling, it is possible to have a timeout of -1 to wait forever, and use
   * AsyncIOQueue.Signal() to wake up the waiting threads later.
   *
   * @returns details of a finished task if a task has completed, std::nullopt
   *          otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AsyncIOQueue.Signal
   */
  std::optional<AsyncIOOutcome> WaitResult();

  /**
   * Wake up any threads that are blocking in AsyncIOQueue.WaitResult().
   *
   * This will unblock any threads that are sleeping in a call to
   * AsyncIOQueue.WaitResult for the specified queue, and cause them to return
   * from that function.
   *
   * This can be useful when destroying a queue to make sure nothing is touching
   * it indefinitely. In this case, once this call completes, the caller should
   * take measures to make sure any previously-blocked threads have returned
   * from their wait and will not touch the queue again (perhaps by setting a
   * flag to tell the threads to terminate and then using Thread.Wait() to make
   * sure they've done so).
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AsyncIOQueue.WaitResult
   */
  void Signal();
};

/// Semi-safe reference for AsyncIOQueue.
struct AsyncIOQueueRef : AsyncIOQueue
{
  using AsyncIOQueue::AsyncIOQueue;

  /**
   * Constructs from AsyncIOQueueParam.
   *
   * @param resource a AsyncIOQueueRaw or AsyncIOQueue.
   *
   * This does not takes ownership!
   */
  AsyncIOQueueRef(AsyncIOQueueParam resource) noexcept
    : AsyncIOQueue(resource.value)
  {
  }

  /**
   * Constructs from AsyncIOQueueParam.
   *
   * @param resource a AsyncIOQueueRaw or AsyncIOQueue.
   *
   * This does not takes ownership!
   */
  AsyncIOQueueRef(AsyncIOQueueRaw resource) noexcept
    : AsyncIOQueue(resource)
  {
  }

  /// Copy constructor.
  AsyncIOQueueRef(const AsyncIOQueueRef& other) noexcept
    : AsyncIOQueue(other.get())
  {
  }

  /// Destructor
  ~AsyncIOQueueRef() { release(); }
};

/**
 * Use this function to create a new AsyncIO object for reading from and/or
 * writing to a named file.
 *
 * The `mode` string understands the following values:
 *
 * - "r": Open a file for reading only. It must exist.
 * - "w": Open a file for writing only. It will create missing files or truncate
 *   existing ones.
 * - "r+": Open a file for update both reading and writing. The file must exist.
 * - "w+": Create an empty file for both reading and writing. If a file with the
 *   same name already exists its content is erased and the file is treated as a
 *   new empty file.
 *
 * There is no "b" mode, as there is only "binary" style I/O, and no "a" mode
 * for appending, since you specify the position when starting a task.
 *
 * This function supports Unicode filenames, but they must be encoded in UTF-8
 * format, regardless of the underlying operating system.
 *
 * This call is _not_ asynchronous; it will open the file before returning,
 * under the assumption that doing so is generally a fast operation. Future
 * reads and writes to the opened file will be async, however.
 *
 * @param file a UTF-8 string representing the filename to open.
 * @param mode an ASCII string representing the mode to be used for opening the
 *             file.
 * @returns a pointer to the AsyncIO structure that is created or nullptr on
 *          failure; call GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AsyncIO.Close
 * @sa AsyncIO.Read
 * @sa AsyncIO.Write
 */
inline AsyncIO AsyncIOFromFile(StringParam file, StringParam mode)
{
  return AsyncIO(std::move(file), std::move(mode));
}

/**
 * Use this function to get the size of the data stream in an AsyncIO.
 *
 * This call is _not_ asynchronous; it assumes that obtaining this info is a
 * non-blocking operation in most reasonable cases.
 *
 * @param asyncio the AsyncIO to get the size of the data stream from.
 * @returns the size of the data stream in the IOStream on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Sint64 GetAsyncIOSize(AsyncIOParam asyncio)
{
  return CheckError(SDL_GetAsyncIOSize(asyncio));
}

inline Sint64 AsyncIO::GetSize() { return SDL::GetAsyncIOSize(m_resource); }

/**
 * Start an async read.
 *
 * This function reads up to `size` bytes from `offset` position in the data
 * source to the area pointed at by `ptr`. This function may read less bytes
 * than requested.
 *
 * This function returns as quickly as possible; it does not wait for the read
 * to complete. On a successful return, this work will continue in the
 * background. If the work begins, even failure is asynchronous: a failing
 * return value from this function only means the work couldn't start at all.
 *
 * `ptr` must remain available until the work is done, and may be accessed by
 * the system at any time until then. Do not allocate it on the stack, as this
 * might take longer than the life of the calling function to complete!
 *
 * An AsyncIOQueue must be specified. The newly-created task will be added to it
 * when it completes its work.
 *
 * @param asyncio a pointer to an AsyncIO structure.
 * @param ptr a pointer to a buffer to read data into.
 * @param offset the position to start reading in the data source.
 * @param size the number of bytes to read from the data source.
 * @param queue a queue to add the new AsyncIO to.
 * @param userdata an app-defined pointer that will be provided with the task
 *                 results.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AsyncIO.Write
 * @sa AsyncIOQueue.AsyncIOQueue
 */
inline void ReadAsyncIO(AsyncIOParam asyncio,
                        void* ptr,
                        Uint64 offset,
                        Uint64 size,
                        AsyncIOQueueParam queue,
                        void* userdata)
{
  CheckError(SDL_ReadAsyncIO(asyncio, ptr, offset, size, queue, userdata));
}

inline void AsyncIO::Read(void* ptr,
                          Uint64 offset,
                          Uint64 size,
                          AsyncIOQueueParam queue,
                          void* userdata)
{
  SDL::ReadAsyncIO(m_resource, ptr, offset, size, queue, userdata);
}

/**
 * Start an async write.
 *
 * This function writes `size` bytes from `offset` position in the data source
 * to the area pointed at by `ptr`.
 *
 * This function returns as quickly as possible; it does not wait for the write
 * to complete. On a successful return, this work will continue in the
 * background. If the work begins, even failure is asynchronous: a failing
 * return value from this function only means the work couldn't start at all.
 *
 * `ptr` must remain available until the work is done, and may be accessed by
 * the system at any time until then. Do not allocate it on the stack, as this
 * might take longer than the life of the calling function to complete!
 *
 * An AsyncIOQueue must be specified. The newly-created task will be added to it
 * when it completes its work.
 *
 * @param asyncio a pointer to an AsyncIO structure.
 * @param ptr a pointer to a buffer to write data from.
 * @param offset the position to start writing to the data source.
 * @param size the number of bytes to write to the data source.
 * @param queue a queue to add the new AsyncIO to.
 * @param userdata an app-defined pointer that will be provided with the task
 *                 results.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AsyncIO.Read
 * @sa AsyncIOQueue.AsyncIOQueue
 */
inline void WriteAsyncIO(AsyncIOParam asyncio,
                         void* ptr,
                         Uint64 offset,
                         Uint64 size,
                         AsyncIOQueueParam queue,
                         void* userdata)
{
  CheckError(SDL_WriteAsyncIO(asyncio, ptr, offset, size, queue, userdata));
}

inline void AsyncIO::Write(void* ptr,
                           Uint64 offset,
                           Uint64 size,
                           AsyncIOQueueParam queue,
                           void* userdata)
{
  SDL::WriteAsyncIO(m_resource, ptr, offset, size, queue, userdata);
}

/**
 * Close and free any allocated resources for an async I/O object.
 *
 * Closing a file is _also_ an asynchronous task! If a write failure were to
 * happen during the closing process, for example, the task results will report
 * it as usual.
 *
 * Closing a file that has been written to does not guarantee the data has made
 * it to physical media; it may remain in the operating system's file cache, for
 * later writing to disk. This means that a successfully-closed file can be lost
 * if the system crashes or loses power in this small window. To prevent this,
 * call this function with the `flush` parameter set to true. This will make the
 * operation take longer, and perhaps increase system load in general, but a
 * successful result guarantees that the data has made it to physical storage.
 * Don't use this for temporary files, caches, and unimportant data, and
 * definitely use it for crucial irreplaceable files, like game saves.
 *
 * This function guarantees that the close will happen after any other pending
 * tasks to `asyncio`, so it's safe to open a file, start several operations,
 * close the file immediately, then check for all results later. This function
 * will not block until the tasks have completed.
 *
 * Once this function returns true, `asyncio` is no longer valid, regardless of
 * any future outcomes. Any completed tasks might still contain this pointer in
 * their AsyncIOOutcome data, in case the app was using this value to track
 * information, but it should not be used again.
 *
 * If this function returns false, the close wasn't started at all, and it's
 * safe to attempt to close again later.
 *
 * An AsyncIOQueue must be specified. The newly-created task will be added to it
 * when it completes its work.
 *
 * @param asyncio a pointer to an AsyncIO structure to close.
 * @param flush true if data should sync to disk before the task completes.
 * @param queue a queue to add the new AsyncIO to.
 * @param userdata an app-defined pointer that will be provided with the task
 *                 results.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread, but two
 *               threads should not attempt to close the same object.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool CloseAsyncIO(AsyncIORaw asyncio,
                         bool flush,
                         AsyncIOQueueParam queue,
                         void* userdata)
{
  return SDL_CloseAsyncIO(asyncio, flush, queue, userdata);
}

inline bool AsyncIO::Close(bool flush, AsyncIOQueueParam queue, void* userdata)
{
  return CloseAsyncIO(release(), flush, queue, userdata);
}

/**
 * Create a task queue for tracking multiple I/O operations.
 *
 * Async I/O operations are assigned to a queue when started. The queue can be
 * checked for completed tasks thereafter.
 *
 * @returns a new task queue object or nullptr if there was an error; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AsyncIOQueue.Destroy
 * @sa AsyncIOQueue.GetResult
 * @sa AsyncIOQueue.WaitResult
 */
inline AsyncIOQueue CreateAsyncIOQueue() { return AsyncIOQueue(); }

/**
 * Destroy a previously-created async I/O task queue.
 *
 * If there are still tasks pending for this queue, this call will block until
 * those tasks are finished. All those tasks will be deallocated. Their results
 * will be lost to the app.
 *
 * Any pending reads from LoadFileAsync() that are still in this queue will have
 * their buffers deallocated by this function, to prevent a memory leak.
 *
 * Once this function is called, the queue is no longer valid and should not be
 * used, including by other threads that might access it while destruction is
 * blocking on pending tasks.
 *
 * Do not destroy a queue that still has threads waiting on it through
 * AsyncIOQueue.WaitResult(). You can call AsyncIOQueue.Signal() first to
 * unblock those threads, and take measures (such as Thread.Wait()) to make sure
 * they have finished their wait and won't wait on the queue again.
 *
 * @param queue the task queue to destroy.
 *
 * @threadsafety It is safe to call this function from any thread, so long as no
 *               other thread is waiting on the queue with
 *               AsyncIOQueue.WaitResult.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void DestroyAsyncIOQueue(AsyncIOQueueRaw queue)
{
  SDL_DestroyAsyncIOQueue(queue);
}

inline void AsyncIOQueue::Destroy() { DestroyAsyncIOQueue(release()); }

/**
 * Query an async I/O task queue for completed tasks.
 *
 * If a task assigned to this queue has finished, this will return true and fill
 * in `outcome` with the details of the task. If no task in the queue has
 * finished, this function will return false. This function does not block.
 *
 * If a task has completed, this function will free its resources and the task
 * pointer will no longer be valid. The task will be removed from the queue.
 *
 * It is safe for multiple threads to call this function on the same queue at
 * once; a completed task will only go to one of the threads.
 *
 * @param queue the async I/O task queue to query.
 * @returns details of a finished task if a task has completed, std::nullopt
 *          otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AsyncIOQueue.WaitResult
 */
inline std::optional<AsyncIOOutcome> GetAsyncIOResult(AsyncIOQueueParam queue)
{
  if (AsyncIOOutcome outcome; SDL_GetAsyncIOResult(queue, &outcome)) {
    return outcome;
  }
  return std::nullopt;
}

inline std::optional<AsyncIOOutcome> AsyncIOQueue::GetResult()
{
  return SDL::GetAsyncIOResult(m_resource);
}

/**
 * Block until an async I/O task queue has a completed task.
 *
 * This function puts the calling thread to sleep until there a task assigned to
 * the queue that has finished.
 *
 * If a task assigned to the queue has finished, this will return true and fill
 * in `outcome` with the details of the task. If no task in the queue has
 * finished, this function will return false.
 *
 * If a task has completed, this function will free its resources and the task
 * pointer will no longer be valid. The task will be removed from the queue.
 *
 * It is safe for multiple threads to call this function on the same queue at
 * once; a completed task will only go to one of the threads.
 *
 * Note that by the nature of various platforms, more than one waiting thread
 * may wake to handle a single task, but only one will obtain it, so `timeoutMS`
 * is a _maximum_ wait time, and this function may return false sooner.
 *
 * This function may return false if there was a system error, the OS
 * inadvertently awoke multiple threads, or if AsyncIOQueue.Signal() was called
 * to wake up all waiting threads without a finished task.
 *
 * A timeout can be used to specify a maximum wait time, but rather than
 * polling, it is possible to have a timeout of -1 to wait forever, and use
 * AsyncIOQueue.Signal() to wake up the waiting threads later.
 *
 * @param queue the async I/O task queue to wait on.
 * @param timeout the maximum time to wait, in milliseconds.
 * @returns details of a finished task if a task has completed, std::nullopt
 *          otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AsyncIOQueue.Signal
 */
inline std::optional<AsyncIOOutcome> WaitAsyncIOResult(AsyncIOQueueParam queue,
                                                       Milliseconds timeout)
{
  if (AsyncIOOutcome outcome;
      SDL_WaitAsyncIOResult(queue, &outcome, timeout.count())) {
    return outcome;
  }
  return std::nullopt;
}

/**
 * Block until an async I/O task queue has a completed task.
 *
 * This function puts the calling thread to sleep until there a task assigned to
 * the queue that has finished.
 *
 * If a task assigned to the queue has finished, this will return true and fill
 * in `outcome` with the details of the task. If no task in the queue has
 * finished, this function will return false.
 *
 * If a task has completed, this function will free its resources and the task
 * pointer will no longer be valid. The task will be removed from the queue.
 *
 * It is safe for multiple threads to call this function on the same queue at
 * once; a completed task will only go to one of the threads.
 *
 * Note that by the nature of various platforms, more than one waiting thread
 * may wake to handle a single task, but only one will obtain it, so `timeoutMS`
 * is a _maximum_ wait time, and this function may return false sooner.
 *
 * This function may return false if there was a system error, the OS
 * inadvertently awoke multiple threads, or if AsyncIOQueue.Signal() was called
 * to wake up all waiting threads without a finished task.
 *
 * A timeout can be used to specify a maximum wait time, but rather than
 * polling, it is possible to have a timeout of -1 to wait forever, and use
 * AsyncIOQueue.Signal() to wake up the waiting threads later.
 *
 * @param queue the async I/O task queue to wait on.
 * @returns details of a finished task if a task has completed, std::nullopt
 *          otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AsyncIOQueue.Signal
 */
inline std::optional<AsyncIOOutcome> WaitAsyncIOResult(AsyncIOQueueParam queue)
{
  if (AsyncIOOutcome outcome; SDL_WaitAsyncIOResult(queue, &outcome, -1)) {
    return outcome;
  }
  return std::nullopt;
}

inline std::optional<AsyncIOOutcome> AsyncIOQueue::WaitResult(
  Milliseconds timeout)
{
  return SDL::WaitAsyncIOResult(m_resource, timeout);
}

inline std::optional<AsyncIOOutcome> AsyncIOQueue::WaitResult()
{
  return SDL::WaitAsyncIOResult(m_resource);
}

/**
 * Wake up any threads that are blocking in AsyncIOQueue.WaitResult().
 *
 * This will unblock any threads that are sleeping in a call to
 * AsyncIOQueue.WaitResult for the specified queue, and cause them to return
 * from that function.
 *
 * This can be useful when destroying a queue to make sure nothing is touching
 * it indefinitely. In this case, once this call completes, the caller should
 * take measures to make sure any previously-blocked threads have returned from
 * their wait and will not touch the queue again (perhaps by setting a flag to
 * tell the threads to terminate and then using Thread.Wait() to make sure
 * they've done so).
 *
 * @param queue the async I/O task queue to signal.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AsyncIOQueue.WaitResult
 */
inline void SignalAsyncIOQueue(AsyncIOQueueParam queue)
{
  SDL_SignalAsyncIOQueue(queue);
}

inline void AsyncIOQueue::Signal() { SDL::SignalAsyncIOQueue(m_resource); }

/**
 * Load all the data from a file path, asynchronously.
 *
 * This function returns as quickly as possible; it does not wait for the read
 * to complete. On a successful return, this work will continue in the
 * background. If the work begins, even failure is asynchronous: a failing
 * return value from this function only means the work couldn't start at all.
 *
 * The data is allocated with a zero byte at the end (null terminated) for
 * convenience. This extra byte is not included in AsyncIOOutcome's
 * bytes_transferred value.
 *
 * This function will allocate the buffer to contain the file. It must be
 * deallocated by calling free() on AsyncIOOutcome's buffer field after
 * completion.
 *
 * An AsyncIOQueue must be specified. The newly-created task will be added to it
 * when it completes its work.
 *
 * @param file the path to read all available data from.
 * @param queue a queue to add the new AsyncIO to.
 * @param userdata an app-defined pointer that will be provided with the task
 *                 results.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa IOStream.LoadFile
 */
inline void LoadFileAsync(StringParam file,
                          AsyncIOQueueParam queue,
                          void* userdata)
{
  CheckError(SDL_LoadFileAsync(file, queue, userdata));
}

/// @}

} // namespace SDL

#endif /* SDL3PP_ASYNCIO_H_ */
