#ifndef SDL3PP_MUTEX_H_
#define SDL3PP_MUTEX_H_

#include <SDL3/SDL_mutex.h>
#include "SDL3pp_stdinc.h"
#include "SDL3pp_thread.h"

namespace SDL {

/**
 *
 * @defgroup CategoryMutex Thread Synchronization Primitives
 *
 * SDL offers several thread synchronization primitives. This document can't
 * cover the complicated topic of thread safety, but reading up on what each
 * of these primitives are, why they are useful, and how to correctly use them
 * is vital to writing correct and safe multithreaded programs.
 *
 * - Mutexes: MutexBase.MutexBase()
 * - Read/Write locks: RWLockBase.RWLockBase()
 * - Semaphores: SemaphoreBase.SemaphoreBase()
 * - Condition variables: ConditionBase.ConditionBase()
 *
 * SDL also offers a datatype, InitState, which can be used to make sure
 * only one thread initializes/deinitializes some resource that several
 * threads might try to use for the first time simultaneously.
 *
 * @{
 */

// Forward decl
struct MutexBase;

// Forward decl
struct MutexRef;

// Forward decl
struct Mutex;

// Forward decl
struct RWLockBase;

// Forward decl
struct RWLockRef;

// Forward decl
struct RWLock;

// Forward decl
struct SemaphoreBase;

// Forward decl
struct SemaphoreRef;

// Forward decl
struct Semaphore;

// Forward decl
struct ConditionBase;

// Forward decl
struct ConditionRef;

// Forward decl
struct Condition;

/**
 * A means to serialize access to a resource between threads.
 *
 * Mutexes (short for "mutual exclusion") are a synchronization primitive that
 * allows exactly one thread to proceed at a time.
 *
 * Wikipedia has a thorough explanation of the concept:
 *
 * https://en.wikipedia.org/wiki/Mutex
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa Mutex
 * @sa MutexRef
 */
struct MutexBase : Resource<SDL_Mutex*>
{
  using Resource::Resource;

  /**
   * Create a new mutex.
   *
   * All newly-created mutexes begin in the _unlocked_ state.
   *
   * Calls to MutexBase.Lock() will not return while the mutex is locked by
   * another thread. See MutexBase.TryLock() to attempt to lock without
   * blocking.
   *
   * SDL mutexes are reentrant.
   *
   * @post the initialized and unlocked mutex or nullptr on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa MutexBase.Lock
   * @sa MutexBase.TryLock
   * @sa MutexBase.Unlock
   */
  MutexBase()
    : Resource(SDL_CreateMutex())
  {
  }

  /**
   * Lock the mutex.
   *
   * This will block until the mutex is available, which is to say it is in the
   * unlocked state and the OS has chosen the caller as the next thread to lock
   * it. Of all threads waiting to lock the mutex, only one may do so at a time.
   *
   * It is legal for the owning thread to lock an already-locked mutex. It must
   * unlock it the same number of times before it is actually made available for
   * other threads in the system (this is known as a "recursive mutex").
   *
   * This function does not fail; if mutex is nullptr, it will return
   * immediately having locked nothing. If the mutex is valid, this function
   * will always block until it can lock the mutex, and return with it locked.
   *
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa MutexBase.TryLock
   * @sa MutexBase.Unlock
   */
  void Lock() { SDL_LockMutex(get()); }

  /**
   * Try to lock a mutex without blocking.
   *
   * This works just like MutexBase.Lock(), but if the mutex is not available,
   * this function returns false immediately.
   *
   * This technique is useful if you need exclusive access to a resource but
   * don't want to wait for it, and will return to it to try again later.
   *
   * This function returns true if passed a nullptr mutex.
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa MutexBase.Lock
   * @sa MutexBase.Unlock
   */
  void TryLock() { CheckError(SDL_TryLockMutex(get())); }

  /**
   * Unlock the mutex.
   *
   * It is legal for the owning thread to lock an already-locked mutex. It must
   * unlock it the same number of times before it is actually made available for
   * other threads in the system (this is known as a "recursive mutex").
   *
   * It is illegal to unlock a mutex that has not been locked by the current
   * thread, and doing so results in undefined behavior.
   *
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa MutexBase.Lock
   * @sa MutexBase.TryLock
   */
  void Unlock() { SDL_UnlockMutex(get()); }
};

/**
 * Handle to a non owned mutex
 *
 * @cat resource
 *
 * @sa MutexBase
 * @sa Mutex
 */
struct MutexRef : MutexBase
{
  using MutexBase::MutexBase;

  /**
   * Copy constructor.
   */
  constexpr MutexRef(const MutexRef& other)
    : MutexBase(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr MutexRef(MutexRef&& other)
    : MutexBase(other.release())
  {
  }

  /**
   * Default constructor
   */
  constexpr ~MutexRef() = default;

  /**
   * Assignment operator.
   */
  MutexRef& operator=(MutexRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Destroy a mutex created with MutexBase.MutexBase().
   *
   * This function must be called on any mutex that is no longer needed. Failure
   * to destroy a mutex will result in a system memory or resource leak. While
   * it is safe to destroy a mutex that is _unlocked_, it is not safe to attempt
   * to destroy a locked mutex, and may result in undefined behavior depending
   * on the platform.
   *
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa MutexBase.MutexBase
   */
  void reset(SDL_Mutex* newResource = {})
  {
    SDL_DestroyMutex(release(newResource));
  }
};

/**
 * Handle to an owned mutex
 *
 * @cat resource
 *
 * @sa MutexBase
 * @sa MutexRef
 */
struct Mutex : MutexRef
{
  using MutexRef::MutexRef;

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit Mutex(SDL_Mutex* resource = {})
    : MutexRef(resource)
  {
  }

  constexpr Mutex(const Mutex& other) = delete;

  /**
   * Move constructor.
   */
  constexpr Mutex(Mutex&& other) = default;

  /**
   * Frees up resource when object goes out of scope.
   */
  ~Mutex() { reset(); }

  /**
   * Assignment operator.
   */
  Mutex& operator=(Mutex other)
  {
    reset(other.release());
    return *this;
  }
};

/**
 * A mutex that allows read-only threads to run in parallel.
 *
 * A rwlock is roughly the same concept as MutexBase, but allows threads that
 * request read-only access to all hold the lock at the same time. If a thread
 * requests write access, it will block until all read-only threads have
 * released the lock, and no one else can hold the thread (for reading or
 * writing) at the same time as the writing thread.
 *
 * This can be more efficient in cases where several threads need to access
 * data frequently, but changes to that data are rare.
 *
 * There are other rules that apply to rwlocks that don't apply to mutexes,
 * about how threads are scheduled and when they can be recursively locked.
 * These are documented in the other rwlock functions.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa RWLock
 * @sa RWLockRef
 */
struct RWLockBase : Resource<SDL_RWLock*>
{
  using Resource::Resource;

  /**
   * Create a new read/write lock.
   *
   * A read/write lock is useful for situations where you have multiple threads
   * trying to access a resource that is rarely updated. All threads requesting
   * a read-only lock will be allowed to run in parallel; if a thread requests a
   * write lock, it will be provided exclusive access. This makes it safe for
   * multiple threads to use a resource at the same time if they promise not to
   * change it, and when it has to be changed, the rwlock will serve as a
   * gateway to make sure those changes can be made safely.
   *
   * In the right situation, a rwlock can be more efficient than a mutex, which
   * only lets a single thread proceed at a time, even if it won't be modifying
   * the data.
   *
   * All newly-created read/write locks begin in the _unlocked_ state.
   *
   * Calls to RWLockBase.LockForReading() and RWLockBase.LockForWriting will not
   * return while the rwlock is locked _for writing_ by another thread. See
   * RWLockBase.TryLockForReading() and RWLockBase.TryLockForWriting() to
   * attempt to lock without blocking.
   *
   * SDL read/write locks are only recursive for read-only locks! They are not
   * guaranteed to be fair, or provide access in a FIFO manner! They are not
   * guaranteed to favor writers. You may not lock a rwlock for both read-only
   * and write access at the same time from the same thread (so you can't
   * promote your read-only lock to a write lock without unlocking first).
   *
   * @post the initialized and unlocked read/write lock or nullptr on failure;
   *          call GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RWLockBase.LockForReading
   * @sa RWLockBase.LockForWriting
   * @sa RWLockBase.TryLockForReading
   * @sa RWLockBase.TryLockForWriting
   * @sa RWLockBase.Unlock
   */
  RWLockBase()
    : Resource(SDL_CreateRWLock())
  {
  }

  /**
   * Lock the read/write lock for _read only_ operations.
   *
   * This will block until the rwlock is available, which is to say it is not
   * locked for writing by any other thread. Of all threads waiting to lock the
   * rwlock, all may do so at the same time as long as they are requesting
   * read-only access; if a thread wants to lock for writing, only one may do so
   * at a time, and no other threads, read-only or not, may hold the lock at the
   * same time.
   *
   * It is legal for the owning thread to lock an already-locked rwlock for
   * reading. It must unlock it the same number of times before it is actually
   * made available for other threads in the system (this is known as a
   * "recursive rwlock").
   *
   * Note that locking for writing is not recursive (this is only available to
   * read-only locks).
   *
   * It is illegal to request a read-only lock from a thread that already holds
   * the write lock. Doing so results in undefined behavior. Unlock the write
   * lock before requesting a read-only lock. (But, of course, if you have the
   * write lock, you don't need further locks to read in any case.)
   *
   * This function does not fail; if rwlock is nullptr, it will return
   * immediately having locked nothing. If the rwlock is valid, this function
   * will always block until it can lock the mutex, and return with it locked.
   *
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RWLockBase.LockForWriting
   * @sa RWLockBase.TryLockForReading
   * @sa RWLockBase.Unlock
   */
  void LockForReading() { SDL_LockRWLockForReading(get()); }

  /**
   * Lock the read/write lock for _write_ operations.
   *
   * This will block until the rwlock is available, which is to say it is not
   * locked for reading or writing by any other thread. Only one thread may hold
   * the lock when it requests write access; all other threads, whether they
   * also want to write or only want read-only access, must wait until the
   * writer thread has released the lock.
   *
   * It is illegal for the owning thread to lock an already-locked rwlock for
   * writing (read-only may be locked recursively, writing can not). Doing so
   * results in undefined behavior.
   *
   * It is illegal to request a write lock from a thread that already holds a
   * read-only lock. Doing so results in undefined behavior. Unlock the
   * read-only lock before requesting a write lock.
   *
   * This function does not fail; if rwlock is nullptr, it will return
   * immediately having locked nothing. If the rwlock is valid, this function
   * will always block until it can lock the mutex, and return with it locked.
   *
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RWLockBase.LockForReading
   * @sa RWLockBase.TryLockForWriting
   * @sa RWLockBase.Unlock
   */
  void LockForWriting() { SDL_LockRWLockForWriting(get()); }

  /**
   * Try to lock a read/write lock _for reading_ without blocking.
   *
   * This works just like RWLockBase.LockForReading(), but if the rwlock is not
   * available, then this function returns false immediately.
   *
   * This technique is useful if you need access to a resource but don't want to
   * wait for it, and will return to it to try again later.
   *
   * Trying to lock for read-only access can succeed if other threads are
   * holding read-only locks, as this won't prevent access.
   *
   * This function returns true if passed a nullptr rwlock.
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RWLockBase.LockForReading
   * @sa RWLockBase.TryLockForWriting
   * @sa RWLockBase.Unlock
   */
  void TryLockForReading() { CheckError(SDL_TryLockRWLockForReading(get())); }

  /**
   * Try to lock a read/write lock _for writing_ without blocking.
   *
   * This works just like RWLockBase.LockForWriting(), but if the rwlock is not
   * available, then this function returns false immediately.
   *
   * This technique is useful if you need exclusive access to a resource but
   * don't want to wait for it, and will return to it to try again later.
   *
   * It is illegal for the owning thread to lock an already-locked rwlock for
   * writing (read-only may be locked recursively, writing can not). Doing so
   * results in undefined behavior.
   *
   * It is illegal to request a write lock from a thread that already holds a
   * read-only lock. Doing so results in undefined behavior. Unlock the
   * read-only lock before requesting a write lock.
   *
   * This function returns true if passed a nullptr rwlock.
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RWLockBase.LockForWriting
   * @sa RWLockBase.TryLockForReading
   * @sa RWLockBase.Unlock
   */
  void TryLockForWriting() { CheckError(SDL_TryLockRWLockForWriting(get())); }

  /**
   * Unlock the read/write lock.
   *
   * Use this function to unlock the rwlock, whether it was locked for read-only
   * or write operations.
   *
   * It is legal for the owning thread to lock an already-locked read-only lock.
   * It must unlock it the same number of times before it is actually made
   * available for other threads in the system (this is known as a "recursive
   * rwlock").
   *
   * It is illegal to unlock a rwlock that has not been locked by the current
   * thread, and doing so results in undefined behavior.
   *
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RWLockBase.LockForReading
   * @sa RWLockBase.LockForWriting
   * @sa RWLockBase.TryLockForReading
   * @sa RWLockBase.TryLockForWriting
   */
  void Unlock() { SDL_UnlockRWLock(get()); }
};

/**
 * Handle to a non owned rWLock
 *
 * @cat resource
 *
 * @sa RWLockBase
 * @sa RWLock
 */
struct RWLockRef : RWLockBase
{
  using RWLockBase::RWLockBase;

  /**
   * Copy constructor.
   */
  constexpr RWLockRef(const RWLockRef& other)
    : RWLockBase(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr RWLockRef(RWLockRef&& other)
    : RWLockBase(other.release())
  {
  }

  /**
   * Default constructor
   */
  constexpr ~RWLockRef() = default;

  /**
   * Assignment operator.
   */
  RWLockRef& operator=(RWLockRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Destroy a read/write lock created with RWLockBase.RWLockBase().
   *
   * This function must be called on any read/write lock that is no longer
   * needed. Failure to destroy a rwlock will result in a system memory or
   * resource leak. While it is safe to destroy a rwlock that is _unlocked_, it
   * is not safe to attempt to destroy a locked rwlock, and may result in
   * undefined behavior depending on the platform.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RWLockBase.RWLockBase
   */
  void reset(SDL_RWLock* newResource = {})
  {
    SDL_DestroyRWLock(release(newResource));
  }
};

/**
 * Handle to an owned rWLock
 *
 * @cat resource
 *
 * @sa RWLockBase
 * @sa RWLockRef
 */
struct RWLock : RWLockRef
{
  using RWLockRef::RWLockRef;

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit RWLock(SDL_RWLock* resource = {})
    : RWLockRef(resource)
  {
  }

  constexpr RWLock(const RWLock& other) = delete;

  /**
   * Move constructor.
   */
  constexpr RWLock(RWLock&& other) = default;

  /**
   * Frees up resource when object goes out of scope.
   */
  ~RWLock() { reset(); }

  /**
   * Assignment operator.
   */
  RWLock& operator=(RWLock other)
  {
    reset(other.release());
    return *this;
  }
};

/**
 * A means to manage access to a resource, by count, between threads.
 *
 * Semaphores (specifically, "counting semaphores"), let X number of threads
 * request access at the same time, each thread granted access decrementing a
 * counter. When the counter reaches zero, future requests block until a prior
 * thread releases their request, incrementing the counter again.
 *
 * Wikipedia has a thorough explanation of the concept:
 *
 * https://en.wikipedia.org/wiki/Semaphore_(programming)
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa Semaphore
 * @sa SemaphoreRef
 */
struct SemaphoreBase : Resource<SDL_Semaphore*>
{
  using Resource::Resource;

  /**
   * Create a semaphore.
   *
   * This function creates a new semaphore and initializes it with the value
   * `initial_value`. Each wait operation on the semaphore will atomically
   * decrement the semaphore value and potentially block if the semaphore value
   * is 0. Each post operation will atomically increment the semaphore value and
   * wake waiting threads and allow them to retry the wait operation.
   *
   * @param initial_value the starting value of the semaphore.
   * @post a new semaphore or nullptr on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SemaphoreBase.Signal
   * @sa SemaphoreBase.TryWait
   * @sa SemaphoreBase.GetValue
   * @sa SemaphoreBase.Wait
   * @sa SemaphoreBase.WaitTimeout
   */
  SemaphoreBase(Uint32 initial_value)
    : Resource(SDL_CreateSemaphore(initial_value))
  {
  }

  /**
   * Wait until a semaphore has a positive value and then decrements it.
   *
   * This function suspends the calling thread until the semaphore pointed to by
   * `sem` has a positive value, and then atomically decrement the semaphore
   * value.
   *
   * This function is the equivalent of calling SemaphoreBase.WaitTimeout() with
   * a time length of -1.
   *
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SemaphoreBase.Signal
   * @sa SemaphoreBase.TryWait
   * @sa SemaphoreBase.WaitTimeout
   */
  void Wait() { SDL_WaitSemaphore(get()); }

  /**
   * See if a semaphore has a positive value and decrement it if it does.
   *
   * This function checks to see if the semaphore pointed to by `sem` has a
   * positive value and atomically decrements the semaphore value if it does. If
   * the semaphore doesn't have a positive value, the function immediately
   * returns false.
   *
   * @returns true if the wait succeeds, false if the wait would block.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SemaphoreBase.Signal
   * @sa SemaphoreBase.Wait
   * @sa SemaphoreBase.WaitTimeout
   */
  bool TryWait() { return SDL_TryWaitSemaphore(get()); }

  /**
   * Wait until a semaphore has a positive value and then decrements it.
   *
   * This function suspends the calling thread until either the semaphore
   * pointed to by `sem` has a positive value or the specified time has elapsed.
   * If the call is successful it will atomically decrement the semaphore value.
   *
   * @param timeout the length of the timeout, in milliseconds, or -1 to wait
   *                  indefinitely.
   * @returns true if the wait succeeds or false if the wait times out.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SemaphoreBase.Signal
   * @sa SemaphoreBase.TryWait
   * @sa SemaphoreBase.Wait
   */
  bool WaitTimeout(std::chrono::milliseconds timeout)
  {
    return SDL_WaitSemaphoreTimeout(get(), timeout.count());
  }

  /**
   * Atomically increment a semaphore's value and wake waiting threads.
   *
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SemaphoreBase.TryWait
   * @sa SemaphoreBase.Wait
   * @sa SemaphoreBase.WaitTimeout
   */
  void Signal() { SDL_SignalSemaphore(get()); }

  /**
   * Get the current value of a semaphore.
   *
   * @returns the current value of the semaphore.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint32 GetValue() const { return SDL_GetSemaphoreValue(get()); }
};

/**
 * Handle to a non owned semaphore
 *
 * @cat resource
 *
 * @sa SemaphoreBase
 * @sa Semaphore
 */
struct SemaphoreRef : SemaphoreBase
{
  using SemaphoreBase::SemaphoreBase;

  /**
   * Copy constructor.
   */
  constexpr SemaphoreRef(const SemaphoreRef& other)
    : SemaphoreBase(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr SemaphoreRef(SemaphoreRef&& other)
    : SemaphoreBase(other.release())
  {
  }

  /**
   * Default constructor
   */
  constexpr ~SemaphoreRef() = default;

  /**
   * Assignment operator.
   */
  SemaphoreRef& operator=(SemaphoreRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Destroy a semaphore.
   *
   * It is not safe to destroy a semaphore if there are threads currently
   * waiting on it.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SemaphoreBase.SemaphoreBase
   */
  void reset(SDL_Semaphore* newResource = {})
  {
    SDL_DestroySemaphore(release(newResource));
  }
};

/**
 * Handle to an owned semaphore
 *
 * @cat resource
 *
 * @sa SemaphoreBase
 * @sa SemaphoreRef
 */
struct Semaphore : SemaphoreRef
{
  using SemaphoreRef::SemaphoreRef;

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit Semaphore(SDL_Semaphore* resource = {})
    : SemaphoreRef(resource)
  {
  }

  constexpr Semaphore(const Semaphore& other) = delete;

  /**
   * Move constructor.
   */
  constexpr Semaphore(Semaphore&& other) = default;

  /**
   * Frees up resource when object goes out of scope.
   */
  ~Semaphore() { reset(); }

  /**
   * Assignment operator.
   */
  Semaphore& operator=(Semaphore other)
  {
    reset(other.release());
    return *this;
  }
};

/**
 * A means to block multiple threads until a condition is satisfied.
 *
 * Condition variables, paired with an MutexBase, let an app halt multiple
 * threads until a condition has occurred, at which time the app can release
 * one or all waiting threads.
 *
 * Wikipedia has a thorough explanation of the concept:
 *
 * https://en.wikipedia.org/wiki/Condition_variable
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa Condition
 * @sa ConditionRef
 */
struct ConditionBase : Resource<SDL_Condition*>
{
  using Resource::Resource;

  /**
   * Create a condition variable.
   *
   * @post a new condition variable or nullptr on failure; call GetError()
   *          for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ConditionBase.Broadcast
   * @sa ConditionBase.Signal
   * @sa ConditionBase.Wait
   * @sa ConditionBase.WaitTimeout
   */
  ConditionBase()
    : Resource(SDL_CreateCondition())
  {
  }

  /**
   * Restart one of the threads that are waiting on the condition variable.
   *
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ConditionBase.Broadcast
   * @sa ConditionBase.Wait
   * @sa ConditionBase.WaitTimeout
   */
  void Signal() { SDL_SignalCondition(get()); }

  /**
   * Restart all threads that are waiting on the condition variable.
   *
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ConditionBase.Signal
   * @sa ConditionBase.Wait
   * @sa ConditionBase.WaitTimeout
   */
  void Broadcast() { SDL_BroadcastCondition(get()); }

  /**
   * Wait until a condition variable is signaled.
   *
   * This function unlocks the specified `mutex` and waits for another thread to
   * call ConditionBase.Signal() or ConditionBase.Broadcast() on the condition
   * variable `cond`. Once the condition variable is signaled, the mutex is
   * re-locked and the function returns.
   *
   * The mutex must be locked before calling this function. Locking the mutex
   * recursively (more than once) is not supported and leads to undefined
   * behavior.
   *
   * This function is the equivalent of calling ConditionBase.WaitTimeout() with
   * a time length of -1.
   *
   * @param mutex the mutex used to coordinate thread access.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ConditionBase.Broadcast
   * @sa ConditionBase.Signal
   * @sa ConditionBase.WaitTimeout
   */
  void Wait(MutexBase& mutex) { SDL_WaitCondition(get(), mutex.get()); }

  /**
   * Wait until a condition variable is signaled or a certain time has passed.
   *
   * This function unlocks the specified `mutex` and waits for another thread to
   * call ConditionBase.Signal() or ConditionBase.Broadcast() on the condition
   * variable `cond`, or for the specified time to elapse. Once the condition
   * variable is signaled or the time elapsed, the mutex is re-locked and the
   * function returns.
   *
   * The mutex must be locked before calling this function. Locking the mutex
   * recursively (more than once) is not supported and leads to undefined
   * behavior.
   *
   * @param mutex the mutex used to coordinate thread access.
   * @param timeout the maximum time to wait, in milliseconds, or -1 to wait
   *                  indefinitely.
   * @returns true if the condition variable is signaled, false if the condition
   *          is not signaled in the allotted time.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ConditionBase.Broadcast
   * @sa ConditionBase.Signal
   * @sa ConditionBase.Wait
   */
  bool WaitTimeout(MutexBase& mutex, std::chrono::milliseconds timeout)
  {
    return SDL_WaitConditionTimeout(get(), mutex.get(), timeout.count());
  }
};

/**
 * Handle to a non owned condition
 *
 * @cat resource
 *
 * @sa ConditionBase
 * @sa Condition
 */
struct ConditionRef : ConditionBase
{
  using ConditionBase::ConditionBase;

  /**
   * Copy constructor.
   */
  constexpr ConditionRef(const ConditionRef& other)
    : ConditionBase(other.get())
  {
  }

  /**
   * Move constructor.
   */
  constexpr ConditionRef(ConditionRef&& other)
    : ConditionBase(other.release())
  {
  }

  /**
   * Default constructor
   */
  constexpr ~ConditionRef() = default;

  /**
   * Assignment operator.
   */
  ConditionRef& operator=(ConditionRef other)
  {
    release(other.release());
    return *this;
  }

  /**
   * Destroy a condition variable.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa ConditionBase.ConditionBase
   */
  void reset(SDL_Condition* newResource = {})
  {
    SDL_DestroyCondition(release(newResource));
  }
};

/**
 * Handle to an owned condition
 *
 * @cat resource
 *
 * @sa ConditionBase
 * @sa ConditionRef
 */
struct Condition : ConditionRef
{
  using ConditionRef::ConditionRef;

  /**
   * Constructs from the underlying resource.
   */
  constexpr explicit Condition(SDL_Condition* resource = {})
    : ConditionRef(resource)
  {
  }

  constexpr Condition(const Condition& other) = delete;

  /**
   * Move constructor.
   */
  constexpr Condition(Condition&& other) = default;

  /**
   * Frees up resource when object goes out of scope.
   */
  ~Condition() { reset(); }

  /**
   * Assignment operator.
   */
  Condition& operator=(Condition other)
  {
    reset(other.release());
    return *this;
  }
};

/**
 * The current status of an InitState structure.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using InitStatus = SDL_InitStatus;

constexpr InitStatus INIT_STATUS_UNINITIALIZED =
  SDL_INIT_STATUS_UNINITIALIZED; ///< INIT_STATUS_UNINITIALIZED

constexpr InitStatus INIT_STATUS_INITIALIZING =
  SDL_INIT_STATUS_INITIALIZING; ///< INIT_STATUS_INITIALIZING

constexpr InitStatus INIT_STATUS_INITIALIZED =
  SDL_INIT_STATUS_INITIALIZED; ///< INIT_STATUS_INITIALIZED

constexpr InitStatus INIT_STATUS_UNINITIALIZING =
  SDL_INIT_STATUS_UNINITIALIZING; ///< INIT_STATUS_UNINITIALIZING

/**
 * A structure used for thread-safe initialization and shutdown.
 *
 * Here is an example of using this:
 *
 * ```cpp
 *    static SDL::InitState init;
 *
 *    bool InitSystem(void)
 *    {
 *        if (!InitState.ShouldInit(&init)) {
 *            // The system is initialized
 *            return true;
 *        }
 *
 *        // At this point, you should not leave this function without calling
 * InitState.SetInitialized()
 *
 *        bool initialized = DoInitTasks();
 *        InitState.SetInitialized(&init, initialized);
 *        return initialized;
 *    }
 *
 *    bool UseSubsystem(void)
 *    {
 *        if (InitState.ShouldInit(&init)) {
 *            // Error, the subsystem isn't initialized
 *            InitState.SetInitialized(&init, false);
 *            return false;
 *        }
 *
 *        // Do work using the initialized subsystem
 *
 *        return true;
 *    }
 *
 *    void QuitSystem(void)
 *    {
 *        if (!InitState.ShouldQuit(&init)) {
 *            // The system is not initialized
 *            return;
 *        }
 *
 *        // At this point, you should not leave this function without calling
 * InitState.SetInitialized()
 *
 *        DoQuitTasks();
 *        InitState.SetInitialized(&init, false);
 *    }
 * ```
 *
 * Note that this doesn't protect any resources created during initialization,
 * or guarantee that nobody is using those resources during cleanup. You
 * should use other mechanisms to protect those, if that's a concern for your
 * code.
 *
 * @since This struct is available since SDL 3.2.0.
 */
struct InitState : SDL_InitState
{
  /**
   * Default comparison operator
   */
  constexpr bool operator==(const InitState& other) const = default;

  /**
   * Constructor
   */
  constexpr InitState()
    : SDL_InitState{0}
  {
  }

  /**
   * Return whether initialization should be done.
   *
   * This function checks the passed in state and if initialization should be
   * done, sets the status to `INIT_STATUS_INITIALIZING` and returns true.
   * If another thread is already modifying this state, it will wait until
   * that's done before returning.
   *
   * If this function returns true, the calling code must call
   * InitState.SetInitialized() to complete the initialization.
   *
   * @returns true if initialization needs to be done, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa InitState.SetInitialized
   * @sa InitState.ShouldQuit
   */
  bool ShouldInit() { return SDL_ShouldInit(this); }

  /**
   * Return whether cleanup should be done.
   *
   * This function checks the passed in state and if cleanup should be done,
   * sets the status to `INIT_STATUS_UNINITIALIZING` and returns true.
   *
   * If this function returns true, the calling code must call
   * InitState.SetInitialized() to complete the cleanup.
   *
   * @returns true if cleanup needs to be done, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa InitState.SetInitialized
   * @sa InitState.ShouldInit
   */
  bool ShouldQuit() { return SDL_ShouldQuit(this); }

  /**
   * Finish an initialization state transition.
   *
   * This function sets the status of the passed in state to
   * `INIT_STATUS_INITIALIZED` or `INIT_STATUS_UNINITIALIZED` and allows
   * any threads waiting for the status to proceed.
   *
   * @param initialized the new initialization state.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa InitState.ShouldInit
   * @sa InitState.ShouldQuit
   */
  void SetInitialized(bool initialized)
  {
    SDL_SetInitialized(this, initialized);
  }
};

/// @}

} // namespace SDL

#endif /* SDL3PP_MUTEX_H_ */
