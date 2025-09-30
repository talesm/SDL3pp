#ifndef SDL3PP_THREAD_H_
#define SDL3PP_THREAD_H_

#include <SDL3/SDL_thread.h>
#include "SDL3pp_atomic.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryThread Thread Management
 *
 * SDL offers cross-platform thread management functions. These are mostly
 * concerned with starting threads, setting their priority, and dealing with
 * their termination.
 *
 * In addition, there is support for Thread Local Storage (data that is unique
 * to each thread, but accessed from a single key).
 *
 * On platforms without thread support (such as Emscripten when built without
 * pthreads), these functions still exist, but things like Thread.Thread()
 * will report failure without doing anything.
 *
 * If you're going to work with threads, you almost certainly need to have a
 * good understanding of [CategoryMutex](CategoryMutex) as well.
 *
 * @{
 */

// Forward decl
struct Thread;

/// Alias to raw representation for Thread.
using ThreadRaw = SDL_Thread*;

// Forward decl
struct ThreadRef;

/// Safely wrap Thread for non owning parameters
struct ThreadParam
{
  ThreadRaw value; ///< parameter's ThreadRaw

  /// Constructs from ThreadRaw
  constexpr ThreadParam(ThreadRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr ThreadParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const ThreadParam& other) const = default;

  /// Converts to underlying ThreadRaw
  constexpr operator ThreadRaw() const { return value; }
};

/**
 * The SDL thread priority.
 *
 * SDL will make system changes as necessary in order to apply the thread
 * priority. Code which attempts to control thread state related to priority
 * should be aware that calling Thread.SetCurrentPriority may alter such
 * state. SDL_HINT_THREAD_PRIORITY_POLICY can be used to control aspects of
 * this behavior.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using ThreadPriority = SDL_ThreadPriority;

constexpr ThreadPriority THREAD_PRIORITY_LOW = SDL_THREAD_PRIORITY_LOW; ///< LOW

constexpr ThreadPriority THREAD_PRIORITY_NORMAL =
  SDL_THREAD_PRIORITY_NORMAL; ///< NORMAL

constexpr ThreadPriority THREAD_PRIORITY_HIGH =
  SDL_THREAD_PRIORITY_HIGH; ///< HIGH

constexpr ThreadPriority THREAD_PRIORITY_TIME_CRITICAL =
  SDL_THREAD_PRIORITY_TIME_CRITICAL; ///< TIME_CRITICAL

/**
 * The SDL thread state.
 *
 * The current state of a thread can be checked by calling Thread.GetState.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa Thread.GetState
 */
using ThreadState = SDL_ThreadState;

constexpr ThreadState THREAD_UNKNOWN =
  SDL_THREAD_UNKNOWN; ///< The thread is not valid.

constexpr ThreadState THREAD_ALIVE =
  SDL_THREAD_ALIVE; ///< The thread is currently running.

constexpr ThreadState THREAD_DETACHED =
  SDL_THREAD_DETACHED; ///< The thread is detached and can't be waited on.

/// The thread has finished and should be cleaned up with Thread.Wait()
constexpr ThreadState THREAD_COMPLETE = SDL_THREAD_COMPLETE;

/**
 * A unique numeric ID that identifies a thread.
 *
 * These are different from Thread objects, which are generally what an
 * application will operate on, but having a way to uniquely identify a thread
 * can be useful at times.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa Thread.GetID
 * @sa GetCurrentThreadID
 */
using ThreadID = SDL_ThreadID;

/**
 * The function passed to Thread.Thread() as the new thread's entry point.
 *
 * @param data what was passed as `data` to Thread.Thread().
 * @returns a value that can be reported through Thread.Wait().
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using ThreadFunction = SDL_ThreadFunction;

/**
 * The function passed to Thread.Thread() as the new thread's entry point.
 *
 * @returns a value that can be reported through Thread.Wait().
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using ThreadCB = std::function<int()>;

/**
 * The callback used to cleanup data passed to SetTLS.
 *
 * This is called when a thread exits, to allow an app to free any resources.
 *
 * @param value a pointer previously handed to SetTLS.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SetTLS
 */
using TLSDestructorCallback = SDL_TLSDestructorCallback;

/**
 * The SDL thread object.
 *
 * These are opaque data.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa Thread.Thread
 * @sa Thread.Wait
 *
 * @cat resource
 */
class Thread
{
  ThreadRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Thread() = default;

  /**
   * Constructs from ThreadParam.
   *
   * @param resource a ThreadRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Thread(const ThreadRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Thread(const Thread& other) = delete;

  /// Move constructor
  constexpr Thread(Thread&& other)
    : Thread(other.release())
  {
  }

  constexpr Thread(const ThreadRef& other) = delete;

  constexpr Thread(ThreadRef&& other) = delete;

  /// Default ctor
  Thread(ThreadCB fn, StringParam name) {}

  /**
   * Create a new thread with a default stack size.
   *
   * This is a convenience function, equivalent to calling
   * Thread.Thread with the following properties set:
   *
   * - `prop::thread.CREATE_ENTRY_FUNCTION_POINTER`: `fn`
   * - `prop::thread.CREATE_NAME_STRING`: `name`
   * - `prop::thread.CREATE_USERDATA_POINTER`: `data`
   *
   * Note that this "function" is actually a macro that calls an internal
   * function with two extra parameters not listed here; they are hidden through
   * preprocessor macros and are needed to support various C runtimes at the
   * point of the function call. Language bindings that aren't using the C
   * headers will need to deal with this.
   *
   * Usually, apps should just call this function the same way on every platform
   * and let the macros hide the details.
   *
   * @param fn the ThreadFunction function to call in the new thread.
   * @param name the name of the thread.
   * @param data a pointer that is passed to `fn`.
   * @post an opaque pointer to the new thread object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Thread.Thread
   * @sa Thread.Wait
   */
  Thread(ThreadFunction fn, StringParam name, void* data)
    : m_resource(CheckError(SDL_CreateThread(fn, name, data)))
  {
  }

  /**
   * Create a new thread with with the specified properties.
   *
   * These are the supported properties:
   *
   * - `prop::thread.CREATE_ENTRY_FUNCTION_POINTER`: an ThreadFunction
   *   value that will be called at the start of the new thread's life.
   *   Required.
   * - `prop::thread.CREATE_NAME_STRING`: the name of the new thread, which
   *   might be available to debuggers. Optional, defaults to nullptr.
   * - `prop::thread.CREATE_USERDATA_POINTER`: an arbitrary app-defined
   *   pointer, which is passed to the entry function on the new thread, as its
   *   only parameter. Optional, defaults to nullptr.
   * - `prop::thread.CREATE_STACKSIZE_NUMBER`: the size, in bytes, of the new
   *   thread's stack. Optional, defaults to 0 (system-defined default).
   *
   * SDL makes an attempt to report `prop::thread.CREATE_NAME_STRING` to the
   * system, so that debuggers can display it. Not all platforms support this.
   *
   * Thread naming is a little complicated: Most systems have very small limits
   * for the string length (Haiku has 32 bytes, Linux currently has 16, Visual
   * C++ 6.0 has _nine_!), and possibly other arbitrary rules. You'll have to
   * see what happens with your system's debugger. The name should be UTF-8 (but
   * using the naming limits of C identifiers is a better bet). There are no
   * requirements for thread naming conventions, so long as the string is
   * null-terminated UTF-8, but these guidelines are helpful in choosing a name:
   *
   * https://stackoverflow.com/questions/149932/naming-conventions-for-threads
   *
   * If a system imposes requirements, SDL will try to munge the string for it
   * (truncate, etc), but the original string contents will be available from
   * Thread.GetName().
   *
   * The size (in bytes) of the new stack can be specified with
   * `prop::thread.CREATE_STACKSIZE_NUMBER`. Zero means "use the system
   * default" which might be wildly different between platforms. x86 Linux
   * generally defaults to eight megabytes, an embedded device might be a few
   * kilobytes instead. You generally need to specify a stack that is a multiple
   * of the system's page size (in many cases, this is 4 kilobytes, but check
   * your system documentation).
   *
   * Note that this "function" is actually a macro that calls an internal
   * function with two extra parameters not listed here; they are hidden through
   * preprocessor macros and are needed to support various C runtimes at the
   * point of the function call. Language bindings that aren't using the C
   * headers will need to deal with this.
   *
   * The actual symbol in SDL is `SDL_CreateThreadWithPropertiesRuntime`, so
   * there is no symbol clash, but trying to load an SDL shared library and look
   * for "Thread.Thread" will fail.
   *
   * Usually, apps should just call this function the same way on every platform
   * and let the macros hide the details.
   *
   * @param props the properties to use.
   * @post an opaque pointer to the new thread object on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Thread.Thread
   * @sa Thread.Wait
   */
  Thread(PropertiesParam props)
    : m_resource(CheckError(SDL_CreateThreadWithProperties(props)))
  {
  }

  /// Destructor
  ~Thread() { SDL_DetachThread(m_resource); }

  /// Assignment operator.
  Thread& operator=(Thread other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying ThreadRaw.
  constexpr ThreadRaw get() const { return m_resource; }

  /// Retrieves underlying ThreadRaw and clear this.
  constexpr ThreadRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Thread& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to ThreadParam
  constexpr operator ThreadParam() const { return {m_resource}; }

  /**
   * Let a thread clean up on exit without intervention.
   *
   * A thread may be "detached" to signify that it should not remain until
   * another thread has called Thread.Wait() on it. Detaching a thread is
   * useful for long-running threads that nothing needs to synchronize with or
   * further manage. When a detached thread is done, it simply goes away.
   *
   * There is no way to recover the return code of a detached thread. If you
   * need this, don't detach the thread and instead use Thread.Wait().
   *
   * Once a thread is detached, you should usually assume the Thread isn't
   * safe to reference again, as it will become invalid immediately upon the
   * detached thread's exit, instead of remaining until someone has called
   * Thread.Wait() to finally clean it up. As such, don't detach the same
   * thread more than once.
   *
   * If a thread has already exited when passed to Thread.Detach(), it will
   * stop waiting for a call to Thread.Wait() and clean up immediately. It is
   * not safe to detach a thread that might be used with Thread.Wait().
   *
   * You may not call Thread.Wait() on a thread that has been detached. Use
   * either that function or this one, but not both, or behavior is undefined.
   *
   * It is safe to pass nullptr to this function; it is a no-op.
   *
   *               Thread.Thread() call that started this thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Thread.Thread
   * @sa Thread.Wait
   */
  void Detach();

  /**
   * Get the thread name as it was specified in Thread.Thread().
   *
   * @returns a pointer to a UTF-8 string that names the specified thread, or
   *          nullptr if it doesn't have a name.
   *
   * @since This function is available since SDL 3.2.0.
   */
  const char* GetName() const;

  /**
   * Get the thread identifier for the specified thread.
   *
   * This thread identifier is as reported by the underlying operating system.
   * If SDL is running on a platform that does not support threads the return
   * value will always be zero.
   *
   * @returns the ID of the specified thread, or the ID of the current thread if
   *          `thread` is nullptr.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetCurrentThreadID
   */
  ThreadID GetID() const;

  /**
   * Set the priority for the current thread.
   *
   * Note that some platforms will not let you alter the priority (or at least,
   * promote the thread to a higher priority) at all, and some require you to be
   * an administrator account. Be prepared for this to fail.
   *
   * @param priority the ThreadPriority to set.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  static void SetCurrentPriority(ThreadPriority priority);

  /**
   * Wait for a thread to finish.
   *
   * Threads that haven't been detached will remain until this function cleans
   * them up. Not doing so is a resource leak.
   *
   * Once a thread has been cleaned up through this function, the Thread
   * that references it becomes invalid and should not be referenced again. As
   * such, only one thread may call Thread.Wait() on another.
   *
   * The return code from the thread function is placed in the area pointed to
   * by `status`, if `status` is not nullptr.
   *
   * You may not wait on a thread that has been used in a call to
   * Thread.Detach(). Use either that function or this one, but not both, or
   * behavior is undefined.
   *
   * It is safe to pass a nullptr thread to this function; it is a no-op.
   *
   * Note that the thread pointer is freed by this function and is not valid
   * afterward.
   *
   *               Thread.Thread() call that started this thread.
   * @param status a pointer filled in with the value returned from the thread
   *               function by its 'return', or -1 if the thread has been
   *               detached or isn't valid, may be nullptr.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Thread.Thread
   * @sa Thread.Detach
   */
  void Wait(int* status);

  /**
   * Get the current state of a thread.
   *
   * @returns the current state of a thread, or THREAD_UNKNOWN if the thread
   *          isn't valid.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ThreadState
   */
  ThreadState GetState() const;
};

/// Semi-safe reference for Thread.
struct ThreadRef : Thread
{
  /**
   * Constructs from ThreadParam.
   *
   * @param resource a ThreadRaw or Thread.
   *
   * This does not takes ownership!
   */
  ThreadRef(ThreadParam resource)
    : Thread(resource.value)
  {
  }

  /// Copy constructor.
  ThreadRef(const ThreadRef& other)
    : Thread(other.get())
  {
  }

  /// Destructor
  ~ThreadRef() { release(); }
};

/**
 * Thread local storage ID.
 *
 * 0 is the invalid ID. An app can create these and then set data for these
 * IDs that is unique to each thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa GetTLS
 * @sa SetTLS
 */
using TLSID = AtomicInt;

/**
 * Create a new thread with a default stack size.
 *
 * This is a convenience function, equivalent to calling
 * Thread.Thread with the following properties set:
 *
 * - `prop::thread.CREATE_ENTRY_FUNCTION_POINTER`: `fn`
 * - `prop::thread.CREATE_NAME_STRING`: `name`
 * - `prop::thread.CREATE_USERDATA_POINTER`: `data`
 *
 * Note that this "function" is actually a macro that calls an internal
 * function with two extra parameters not listed here; they are hidden through
 * preprocessor macros and are needed to support various C runtimes at the
 * point of the function call. Language bindings that aren't using the C
 * headers will need to deal with this.
 *
 * Usually, apps should just call this function the same way on every platform
 * and let the macros hide the details.
 *
 * @param fn the ThreadFunction function to call in the new thread.
 * @param name the name of the thread.
 * @param data a pointer that is passed to `fn`.
 * @returns an opaque pointer to the new thread object on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Thread.Thread
 * @sa Thread.Wait
 */
inline Thread CreateThread(ThreadFunction fn, StringParam name, void* data)
{
  return Thread(fn, std::move(name), data);
}

/**
 * Create a new thread with with the specified properties.
 *
 * These are the supported properties:
 *
 * - `prop::thread.CREATE_ENTRY_FUNCTION_POINTER`: an ThreadFunction
 *   value that will be called at the start of the new thread's life.
 *   Required.
 * - `prop::thread.CREATE_NAME_STRING`: the name of the new thread, which
 *   might be available to debuggers. Optional, defaults to nullptr.
 * - `prop::thread.CREATE_USERDATA_POINTER`: an arbitrary app-defined
 *   pointer, which is passed to the entry function on the new thread, as its
 *   only parameter. Optional, defaults to nullptr.
 * - `prop::thread.CREATE_STACKSIZE_NUMBER`: the size, in bytes, of the new
 *   thread's stack. Optional, defaults to 0 (system-defined default).
 *
 * SDL makes an attempt to report `prop::thread.CREATE_NAME_STRING` to the
 * system, so that debuggers can display it. Not all platforms support this.
 *
 * Thread naming is a little complicated: Most systems have very small limits
 * for the string length (Haiku has 32 bytes, Linux currently has 16, Visual
 * C++ 6.0 has _nine_!), and possibly other arbitrary rules. You'll have to
 * see what happens with your system's debugger. The name should be UTF-8 (but
 * using the naming limits of C identifiers is a better bet). There are no
 * requirements for thread naming conventions, so long as the string is
 * null-terminated UTF-8, but these guidelines are helpful in choosing a name:
 *
 * https://stackoverflow.com/questions/149932/naming-conventions-for-threads
 *
 * If a system imposes requirements, SDL will try to munge the string for it
 * (truncate, etc), but the original string contents will be available from
 * Thread.GetName().
 *
 * The size (in bytes) of the new stack can be specified with
 * `prop::thread.CREATE_STACKSIZE_NUMBER`. Zero means "use the system
 * default" which might be wildly different between platforms. x86 Linux
 * generally defaults to eight megabytes, an embedded device might be a few
 * kilobytes instead. You generally need to specify a stack that is a multiple
 * of the system's page size (in many cases, this is 4 kilobytes, but check
 * your system documentation).
 *
 * Note that this "function" is actually a macro that calls an internal
 * function with two extra parameters not listed here; they are hidden through
 * preprocessor macros and are needed to support various C runtimes at the
 * point of the function call. Language bindings that aren't using the C
 * headers will need to deal with this.
 *
 * The actual symbol in SDL is `SDL_CreateThreadWithPropertiesRuntime`, so
 * there is no symbol clash, but trying to load an SDL shared library and look
 * for "Thread.Thread" will fail.
 *
 * Usually, apps should just call this function the same way on every platform
 * and let the macros hide the details.
 *
 * @param props the properties to use.
 * @returns an opaque pointer to the new thread object on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Thread.Thread
 * @sa Thread.Wait
 */
inline Thread CreateThreadWithProperties(PropertiesParam props)
{
  return Thread(props);
}

namespace prop::thread {

constexpr auto CREATE_ENTRY_FUNCTION_POINTER =
  SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER;

constexpr auto CREATE_NAME_STRING = SDL_PROP_THREAD_CREATE_NAME_STRING;

constexpr auto CREATE_USERDATA_POINTER =
  SDL_PROP_THREAD_CREATE_USERDATA_POINTER;

constexpr auto CREATE_STACKSIZE_NUMBER =
  SDL_PROP_THREAD_CREATE_STACKSIZE_NUMBER;

} // namespace prop::thread

/**
 * Get the thread name as it was specified in Thread.Thread().
 *
 * @param thread the thread to query.
 * @returns a pointer to a UTF-8 string that names the specified thread, or
 *          nullptr if it doesn't have a name.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetThreadName(ThreadParam thread)
{
  return SDL_GetThreadName(thread);
}

inline const char* Thread::GetName() const
{
  return SDL::GetThreadName(m_resource);
}

/**
 * Get the thread identifier for the current thread.
 *
 * This thread identifier is as reported by the underlying operating system.
 * If SDL is running on a platform that does not support threads the return
 * value will always be zero.
 *
 * This function also returns a valid thread ID when called from the main
 * thread.
 *
 * @returns the ID of the current thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Thread.GetID
 */
inline ThreadID GetCurrentThreadID() { return SDL_GetCurrentThreadID(); }

/**
 * Get the thread identifier for the specified thread.
 *
 * This thread identifier is as reported by the underlying operating system.
 * If SDL is running on a platform that does not support threads the return
 * value will always be zero.
 *
 * @param thread the thread to query.
 * @returns the ID of the specified thread, or the ID of the current thread if
 *          `thread` is nullptr.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetCurrentThreadID
 */
inline ThreadID GetThreadID(ThreadParam thread)
{
  return SDL_GetThreadID(thread);
}

inline ThreadID Thread::GetID() const { return SDL::GetThreadID(m_resource); }

/**
 * Set the priority for the current thread.
 *
 * Note that some platforms will not let you alter the priority (or at least,
 * promote the thread to a higher priority) at all, and some require you to be
 * an administrator account. Be prepared for this to fail.
 *
 * @param priority the ThreadPriority to set.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void SetCurrentThreadPriority(ThreadPriority priority)
{
  CheckError(SDL_SetCurrentThreadPriority(priority));
}

inline void Thread::SetCurrentPriority(ThreadPriority priority)
{
  SDL::SetCurrentThreadPriority(priority);
}

/**
 * Wait for a thread to finish.
 *
 * Threads that haven't been detached will remain until this function cleans
 * them up. Not doing so is a resource leak.
 *
 * Once a thread has been cleaned up through this function, the Thread
 * that references it becomes invalid and should not be referenced again. As
 * such, only one thread may call Thread.Wait() on another.
 *
 * The return code from the thread function is placed in the area pointed to
 * by `status`, if `status` is not nullptr.
 *
 * You may not wait on a thread that has been used in a call to
 * Thread.Detach(). Use either that function or this one, but not both, or
 * behavior is undefined.
 *
 * It is safe to pass a nullptr thread to this function; it is a no-op.
 *
 * Note that the thread pointer is freed by this function and is not valid
 * afterward.
 *
 * @param thread the Thread pointer that was returned from the
 *               Thread.Thread() call that started this thread.
 * @param status a pointer filled in with the value returned from the thread
 *               function by its 'return', or -1 if the thread has been
 *               detached or isn't valid, may be nullptr.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Thread.Thread
 * @sa Thread.Detach
 */
inline void WaitThread(ThreadParam thread, int* status)
{
  SDL_WaitThread(thread, status);
}

inline void Thread::Wait(int* status) { SDL::WaitThread(m_resource, status); }

/**
 * Get the current state of a thread.
 *
 * @param thread the thread to query.
 * @returns the current state of a thread, or THREAD_UNKNOWN if the thread
 *          isn't valid.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa ThreadState
 */
inline ThreadState GetThreadState(ThreadParam thread)
{
  return SDL_GetThreadState(thread);
}

inline ThreadState Thread::GetState() const
{
  return SDL::GetThreadState(m_resource);
}

/**
 * Let a thread clean up on exit without intervention.
 *
 * A thread may be "detached" to signify that it should not remain until
 * another thread has called Thread.Wait() on it. Detaching a thread is
 * useful for long-running threads that nothing needs to synchronize with or
 * further manage. When a detached thread is done, it simply goes away.
 *
 * There is no way to recover the return code of a detached thread. If you
 * need this, don't detach the thread and instead use Thread.Wait().
 *
 * Once a thread is detached, you should usually assume the Thread isn't
 * safe to reference again, as it will become invalid immediately upon the
 * detached thread's exit, instead of remaining until someone has called
 * Thread.Wait() to finally clean it up. As such, don't detach the same
 * thread more than once.
 *
 * If a thread has already exited when passed to Thread.Detach(), it will
 * stop waiting for a call to Thread.Wait() and clean up immediately. It is
 * not safe to detach a thread that might be used with Thread.Wait().
 *
 * You may not call Thread.Wait() on a thread that has been detached. Use
 * either that function or this one, but not both, or behavior is undefined.
 *
 * It is safe to pass nullptr to this function; it is a no-op.
 *
 * @param thread the Thread pointer that was returned from the
 *               Thread.Thread() call that started this thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Thread.Thread
 * @sa Thread.Wait
 */
inline void DetachThread(ThreadRaw thread) { SDL_DetachThread(thread); }

inline void Thread::Detach()
{
  SDL_DetachThread(m_resource);
  m_resource = nullptr;
}

/**
 * Get the current thread's value associated with a thread local storage ID.
 *
 * @param id a pointer to the thread local storage ID, may not be nullptr.
 * @returns the value associated with the ID for the current thread or nullptr
 * if no value has been set; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetTLS
 */
inline void* GetTLS(TLSID* id) { return SDL_GetTLS(id); }

/**
 * Set the current thread's value associated with a thread local storage ID.
 *
 * If the thread local storage ID is not initialized (the value is 0), a new
 * ID will be created in a thread-safe way, so all calls using a pointer to
 * the same ID will refer to the same local storage.
 *
 * Note that replacing a value from a previous call to this function on the
 * same thread does _not_ call the previous value's destructor!
 *
 * `destructor` can be nullptr; it is assumed that `value` does not need to be
 * cleaned up if so.
 *
 * @param id a pointer to the thread local storage ID, may not be nullptr.
 * @param value the value to associate with the ID for the current thread.
 * @param destructor a function called when the thread exits, to free the
 *                   value, may be nullptr.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetTLS
 */
inline void SetTLS(TLSID* id,
                   const void* value,
                   TLSDestructorCallback destructor)
{
  CheckError(SDL_SetTLS(id, value, destructor));
}

/**
 * Cleanup all TLS data for this thread.
 *
 * If you are creating your threads outside of SDL and then calling SDL
 * functions, you should call this function before your thread exits, to
 * properly clean up SDL memory.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void CleanupTLS() { SDL_CleanupTLS(); }

/// @}

} // namespace SDL

#endif /* SDL3PP_THREAD_H_ */
