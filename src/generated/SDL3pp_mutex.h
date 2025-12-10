#ifndef SDL3PP_MUTEX_H_
#define SDL3PP_MUTEX_H_

#include <SDL3/SDL_mutex.h>
#include "SDL3pp_stdinc.h"
#include "SDL3pp_thread.h"

namespace SDL {

/**
 * @defgroup CategoryMutex Category Mutex
 *
 * SDL offers several thread synchronization primitives. This document can't
 * cover the complicated topic of thread safety, but reading up on what each of
 * these primitives are, why they are useful, and how to correctly use them is
 * vital to writing correct and safe multithreaded programs.
 *
 * - Mutexes: Mutex.Mutex()
 * - Read/Write locks: RWLock.RWLock()
 * - Semaphores: Semaphore.Semaphore()
 * - Condition variables: Condition.Condition()
 *
 * SDL also offers a datatype, InitState, which can be used to make sure only
 * one thread initializes/deinitializes some resource that several threads might
 * try to use for the first time simultaneously.
 *
 * @{
 */

// Forward decl
struct Mutex;

/// Alias to raw representation for Mutex.
using MutexRaw = SDL_Mutex*;

// Forward decl
struct MutexRef;

/// Safely wrap Mutex for non owning parameters
struct MutexParam
{
  MutexRaw value; ///< parameter's MutexRaw

  /// Constructs from MutexRaw
  constexpr MutexParam(MutexRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr MutexParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const MutexParam& other) const = default;

  /// Converts to underlying MutexRaw
  constexpr operator MutexRaw() const { return value; }
};

// Forward decl
struct RWLock;

/// Alias to raw representation for RWLock.
using RWLockRaw = SDL_RWLock*;

// Forward decl
struct RWLockRef;

/// Safely wrap RWLock for non owning parameters
struct RWLockParam
{
  RWLockRaw value; ///< parameter's RWLockRaw

  /// Constructs from RWLockRaw
  constexpr RWLockParam(RWLockRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr RWLockParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const RWLockParam& other) const = default;

  /// Converts to underlying RWLockRaw
  constexpr operator RWLockRaw() const { return value; }
};

// Forward decl
struct Semaphore;

/// Alias to raw representation for Semaphore.
using SemaphoreRaw = SDL_Semaphore*;

// Forward decl
struct SemaphoreRef;

/// Safely wrap Semaphore for non owning parameters
struct SemaphoreParam
{
  SemaphoreRaw value; ///< parameter's SemaphoreRaw

  /// Constructs from SemaphoreRaw
  constexpr SemaphoreParam(SemaphoreRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr SemaphoreParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const SemaphoreParam& other) const = default;

  /// Converts to underlying SemaphoreRaw
  constexpr operator SemaphoreRaw() const { return value; }
};

// Forward decl
struct Condition;

/// Alias to raw representation for Condition.
using ConditionRaw = SDL_Condition*;

// Forward decl
struct ConditionRef;

/// Safely wrap Condition for non owning parameters
struct ConditionParam
{
  ConditionRaw value; ///< parameter's ConditionRaw

  /// Constructs from ConditionRaw
  constexpr ConditionParam(ConditionRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr ConditionParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const ConditionParam& other) const = default;

  /// Converts to underlying ConditionRaw
  constexpr operator ConditionRaw() const { return value; }
};

/// Alias to raw representation for InitState.
using InitStateRaw = SDL_InitState;

// Forward decl
struct InitState;

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
 */
class Mutex
{
  MutexRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Mutex() = default;

  /**
   * Constructs from MutexParam.
   *
   * @param resource a MutexRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Mutex(const MutexRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Mutex(const Mutex& other) = delete;

  /// Move constructor
  constexpr Mutex(Mutex&& other)
    : Mutex(other.release())
  {
  }

  constexpr Mutex(const MutexRef& other) = delete;

  constexpr Mutex(MutexRef&& other) = delete;

  /**
   * Create a new mutex.
   *
   * All newly-created mutexes begin in the _unlocked_ state.
   *
   * Calls to Mutex.Lock() will not return while the mutex is locked by another
   * thread. See Mutex.TryLock() to attempt to lock without blocking.
   *
   * SDL mutexes are reentrant.
   *
   * @post the initialized and unlocked mutex or nullptr on failure; call
   *       GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Mutex.Destroy
   * @sa Mutex.Lock
   * @sa Mutex.TryLock
   * @sa Mutex.Unlock
   */
  Mutex()
    : m_resource(SDL_CreateMutex())
  {
  }

  /// Destructor
  ~Mutex() { SDL_DestroyMutex(m_resource); }

  /// Assignment operator.
  constexpr Mutex& operator=(Mutex&& other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Mutex& operator=(const Mutex& other)
  {
    m_resource = other.m_resource;
    return *this;
  }

public:
  /// Retrieves underlying MutexRaw.
  constexpr MutexRaw get() const { return m_resource; }

  /// Retrieves underlying MutexRaw and clear this.
  constexpr MutexRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Mutex& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to MutexParam
  constexpr operator MutexParam() const { return {m_resource}; }

  /**
   * Destroy a mutex created with Mutex.Mutex().
   *
   * This function must be called on any mutex that is no longer needed. Failure
   * to destroy a mutex will result in a system memory or resource leak. While
   * it is safe to destroy a mutex that is _unlocked_, it is not safe to attempt
   * to destroy a locked mutex, and may result in undefined behavior depending
   * on the platform.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Mutex.Mutex
   */
  void Destroy();

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
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Mutex.TryLock
   * @sa Mutex.Unlock
   */
  void Lock();

  /**
   * Try to lock a mutex without blocking.
   *
   * This works just like Mutex.Lock(), but if the mutex is not available, this
   * function returns false immediately.
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
   * @sa Mutex.Lock
   * @sa Mutex.Unlock
   */
  void TryLock();

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
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Mutex.Lock
   * @sa Mutex.TryLock
   */
  void Unlock();
};

/// Semi-safe reference for Mutex.
struct MutexRef : Mutex
{
  using Mutex::Mutex;

  /**
   * Constructs from MutexParam.
   *
   * @param resource a MutexRaw or Mutex.
   *
   * This does not takes ownership!
   */
  MutexRef(MutexParam resource)
    : Mutex(resource.value)
  {
  }

  /**
   * Constructs from MutexParam.
   *
   * @param resource a MutexRaw or Mutex.
   *
   * This does not takes ownership!
   */
  MutexRef(MutexRaw resource)
    : Mutex(resource)
  {
  }

  /// Copy constructor.
  MutexRef(const MutexRef& other)
    : Mutex(other.get())
  {
  }

  /// Destructor
  ~MutexRef() { release(); }
};

/**
 * Create a new mutex.
 *
 * All newly-created mutexes begin in the _unlocked_ state.
 *
 * Calls to Mutex.Lock() will not return while the mutex is locked by another
 * thread. See Mutex.TryLock() to attempt to lock without blocking.
 *
 * SDL mutexes are reentrant.
 *
 * @returns the initialized and unlocked mutex or nullptr on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Mutex.Destroy
 * @sa Mutex.Lock
 * @sa Mutex.TryLock
 * @sa Mutex.Unlock
 */
inline Mutex CreateMutex() { return Mutex(); }

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
 * This function does not fail; if mutex is nullptr, it will return immediately
 * having locked nothing. If the mutex is valid, this function will always block
 * until it can lock the mutex, and return with it locked.
 *
 * @param mutex the mutex to lock.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Mutex.TryLock
 * @sa Mutex.Unlock
 */
inline void LockMutex(MutexParam mutex) { SDL_LockMutex(mutex); }

inline void Mutex::Lock() { SDL::LockMutex(m_resource); }

/**
 * Try to lock a mutex without blocking.
 *
 * This works just like Mutex.Lock(), but if the mutex is not available, this
 * function returns false immediately.
 *
 * This technique is useful if you need exclusive access to a resource but don't
 * want to wait for it, and will return to it to try again later.
 *
 * This function returns true if passed a nullptr mutex.
 *
 * @param mutex the mutex to try to lock.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Mutex.Lock
 * @sa Mutex.Unlock
 */
inline void TryLockMutex(MutexParam mutex)
{
  CheckError(SDL_TryLockMutex(mutex));
}

inline void Mutex::TryLock() { SDL::TryLockMutex(m_resource); }

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
 * @param mutex the mutex to unlock.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Mutex.Lock
 * @sa Mutex.TryLock
 */
inline void UnlockMutex(MutexParam mutex) { SDL_UnlockMutex(mutex); }

inline void Mutex::Unlock() { SDL::UnlockMutex(m_resource); }

/**
 * Destroy a mutex created with Mutex.Mutex().
 *
 * This function must be called on any mutex that is no longer needed. Failure
 * to destroy a mutex will result in a system memory or resource leak. While it
 * is safe to destroy a mutex that is _unlocked_, it is not safe to attempt to
 * destroy a locked mutex, and may result in undefined behavior depending on the
 * platform.
 *
 * @param mutex the mutex to destroy.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Mutex.Mutex
 */
inline void DestroyMutex(MutexRaw mutex) { SDL_DestroyMutex(mutex); }

inline void Mutex::Destroy() { DestroyMutex(release()); }

/**
 * A mutex that allows read-only threads to run in parallel.
 *
 * A rwlock is roughly the same concept as Mutex, but allows threads that
 * request read-only access to all hold the lock at the same time. If a thread
 * requests write access, it will block until all read-only threads have
 * released the lock, and no one else can hold the thread (for reading or
 * writing) at the same time as the writing thread.
 *
 * This can be more efficient in cases where several threads need to access data
 * frequently, but changes to that data are rare.
 *
 * There are other rules that apply to rwlocks that don't apply to mutexes,
 * about how threads are scheduled and when they can be recursively locked.
 * These are documented in the other rwlock functions.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class RWLock
{
  RWLockRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr RWLock() = default;

  /**
   * Constructs from RWLockParam.
   *
   * @param resource a RWLockRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit RWLock(const RWLockRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr RWLock(const RWLock& other) = delete;

  /// Move constructor
  constexpr RWLock(RWLock&& other)
    : RWLock(other.release())
  {
  }

  constexpr RWLock(const RWLockRef& other) = delete;

  constexpr RWLock(RWLockRef&& other) = delete;

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
   * Calls to RWLock.LockForReading() and RWLock.LockForWriting will not return
   * while the rwlock is locked _for writing_ by another thread. See
   * RWLock.TryLockForReading() and RWLock.TryLockForWriting() to attempt to
   * lock without blocking.
   *
   * SDL read/write locks are only recursive for read-only locks! They are not
   * guaranteed to be fair, or provide access in a FIFO manner! They are not
   * guaranteed to favor writers. You may not lock a rwlock for both read-only
   * and write access at the same time from the same thread (so you can't
   * promote your read-only lock to a write lock without unlocking first).
   *
   * @post the initialized and unlocked read/write lock or nullptr on failure;
   *       call GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RWLock.Destroy
   * @sa RWLock.LockForReading
   * @sa RWLock.LockForWriting
   * @sa RWLock.TryLockForReading
   * @sa RWLock.TryLockForWriting
   * @sa RWLock.Unlock
   */
  RWLock()
    : m_resource(SDL_CreateRWLock())
  {
  }

  /// Destructor
  ~RWLock() { SDL_DestroyRWLock(m_resource); }

  /// Assignment operator.
  constexpr RWLock& operator=(RWLock&& other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr RWLock& operator=(const RWLock& other)
  {
    m_resource = other.m_resource;
    return *this;
  }

public:
  /// Retrieves underlying RWLockRaw.
  constexpr RWLockRaw get() const { return m_resource; }

  /// Retrieves underlying RWLockRaw and clear this.
  constexpr RWLockRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const RWLock& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to RWLockParam
  constexpr operator RWLockParam() const { return {m_resource}; }

  /**
   * Destroy a read/write lock created with RWLock.RWLock().
   *
   * This function must be called on any read/write lock that is no longer
   * needed. Failure to destroy a rwlock will result in a system memory or
   * resource leak. While it is safe to destroy a rwlock that is _unlocked_, it
   * is not safe to attempt to destroy a locked rwlock, and may result in
   * undefined behavior depending on the platform.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RWLock.RWLock
   */
  void Destroy();

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
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RWLock.LockForWriting
   * @sa RWLock.TryLockForReading
   * @sa RWLock.Unlock
   */
  void LockForReading();

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
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RWLock.LockForReading
   * @sa RWLock.TryLockForWriting
   * @sa RWLock.Unlock
   */
  void LockForWriting();

  /**
   * Try to lock a read/write lock _for reading_ without blocking.
   *
   * This works just like RWLock.LockForReading(), but if the rwlock is not
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
   * @sa RWLock.LockForReading
   * @sa RWLock.TryLockForWriting
   * @sa RWLock.Unlock
   */
  void TryLockForReading();

  /**
   * Try to lock a read/write lock _for writing_ without blocking.
   *
   * This works just like RWLock.LockForWriting(), but if the rwlock is not
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
   * @sa RWLock.LockForWriting
   * @sa RWLock.TryLockForReading
   * @sa RWLock.Unlock
   */
  void TryLockForWriting();

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
   * @since This function is available since SDL 3.2.0.
   *
   * @sa RWLock.LockForReading
   * @sa RWLock.LockForWriting
   * @sa RWLock.TryLockForReading
   * @sa RWLock.TryLockForWriting
   */
  void Unlock();
};

/// Semi-safe reference for RWLock.
struct RWLockRef : RWLock
{
  using RWLock::RWLock;

  /**
   * Constructs from RWLockParam.
   *
   * @param resource a RWLockRaw or RWLock.
   *
   * This does not takes ownership!
   */
  RWLockRef(RWLockParam resource)
    : RWLock(resource.value)
  {
  }

  /**
   * Constructs from RWLockParam.
   *
   * @param resource a RWLockRaw or RWLock.
   *
   * This does not takes ownership!
   */
  RWLockRef(RWLockRaw resource)
    : RWLock(resource)
  {
  }

  /// Copy constructor.
  RWLockRef(const RWLockRef& other)
    : RWLock(other.get())
  {
  }

  /// Destructor
  ~RWLockRef() { release(); }
};

/**
 * Create a new read/write lock.
 *
 * A read/write lock is useful for situations where you have multiple threads
 * trying to access a resource that is rarely updated. All threads requesting a
 * read-only lock will be allowed to run in parallel; if a thread requests a
 * write lock, it will be provided exclusive access. This makes it safe for
 * multiple threads to use a resource at the same time if they promise not to
 * change it, and when it has to be changed, the rwlock will serve as a gateway
 * to make sure those changes can be made safely.
 *
 * In the right situation, a rwlock can be more efficient than a mutex, which
 * only lets a single thread proceed at a time, even if it won't be modifying
 * the data.
 *
 * All newly-created read/write locks begin in the _unlocked_ state.
 *
 * Calls to RWLock.LockForReading() and RWLock.LockForWriting will not return
 * while the rwlock is locked _for writing_ by another thread. See
 * RWLock.TryLockForReading() and RWLock.TryLockForWriting() to attempt to lock
 * without blocking.
 *
 * SDL read/write locks are only recursive for read-only locks! They are not
 * guaranteed to be fair, or provide access in a FIFO manner! They are not
 * guaranteed to favor writers. You may not lock a rwlock for both read-only and
 * write access at the same time from the same thread (so you can't promote your
 * read-only lock to a write lock without unlocking first).
 *
 * @returns the initialized and unlocked read/write lock or nullptr on failure;
 *          call GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RWLock.Destroy
 * @sa RWLock.LockForReading
 * @sa RWLock.LockForWriting
 * @sa RWLock.TryLockForReading
 * @sa RWLock.TryLockForWriting
 * @sa RWLock.Unlock
 */
inline RWLock CreateRWLock() { return RWLock(); }

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
 * made available for other threads in the system (this is known as a "recursive
 * rwlock").
 *
 * Note that locking for writing is not recursive (this is only available to
 * read-only locks).
 *
 * It is illegal to request a read-only lock from a thread that already holds
 * the write lock. Doing so results in undefined behavior. Unlock the write lock
 * before requesting a read-only lock. (But, of course, if you have the write
 * lock, you don't need further locks to read in any case.)
 *
 * This function does not fail; if rwlock is nullptr, it will return immediately
 * having locked nothing. If the rwlock is valid, this function will always
 * block until it can lock the mutex, and return with it locked.
 *
 * @param rwlock the read/write lock to lock.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RWLock.LockForWriting
 * @sa RWLock.TryLockForReading
 * @sa RWLock.Unlock
 */
inline void LockRWLockForReading(RWLockParam rwlock)
{
  SDL_LockRWLockForReading(rwlock);
}

inline void RWLock::LockForReading() { SDL::LockRWLockForReading(m_resource); }

/**
 * Lock the read/write lock for _write_ operations.
 *
 * This will block until the rwlock is available, which is to say it is not
 * locked for reading or writing by any other thread. Only one thread may hold
 * the lock when it requests write access; all other threads, whether they also
 * want to write or only want read-only access, must wait until the writer
 * thread has released the lock.
 *
 * It is illegal for the owning thread to lock an already-locked rwlock for
 * writing (read-only may be locked recursively, writing can not). Doing so
 * results in undefined behavior.
 *
 * It is illegal to request a write lock from a thread that already holds a
 * read-only lock. Doing so results in undefined behavior. Unlock the read-only
 * lock before requesting a write lock.
 *
 * This function does not fail; if rwlock is nullptr, it will return immediately
 * having locked nothing. If the rwlock is valid, this function will always
 * block until it can lock the mutex, and return with it locked.
 *
 * @param rwlock the read/write lock to lock.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RWLock.LockForReading
 * @sa RWLock.TryLockForWriting
 * @sa RWLock.Unlock
 */
inline void LockRWLockForWriting(RWLockParam rwlock)
{
  SDL_LockRWLockForWriting(rwlock);
}

inline void RWLock::LockForWriting() { SDL::LockRWLockForWriting(m_resource); }

/**
 * Try to lock a read/write lock _for reading_ without blocking.
 *
 * This works just like RWLock.LockForReading(), but if the rwlock is not
 * available, then this function returns false immediately.
 *
 * This technique is useful if you need access to a resource but don't want to
 * wait for it, and will return to it to try again later.
 *
 * Trying to lock for read-only access can succeed if other threads are holding
 * read-only locks, as this won't prevent access.
 *
 * This function returns true if passed a nullptr rwlock.
 *
 * @param rwlock the rwlock to try to lock.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RWLock.LockForReading
 * @sa RWLock.TryLockForWriting
 * @sa RWLock.Unlock
 */
inline void TryLockRWLockForReading(RWLockParam rwlock)
{
  CheckError(SDL_TryLockRWLockForReading(rwlock));
}

inline void RWLock::TryLockForReading()
{
  SDL::TryLockRWLockForReading(m_resource);
}

/**
 * Try to lock a read/write lock _for writing_ without blocking.
 *
 * This works just like RWLock.LockForWriting(), but if the rwlock is not
 * available, then this function returns false immediately.
 *
 * This technique is useful if you need exclusive access to a resource but don't
 * want to wait for it, and will return to it to try again later.
 *
 * It is illegal for the owning thread to lock an already-locked rwlock for
 * writing (read-only may be locked recursively, writing can not). Doing so
 * results in undefined behavior.
 *
 * It is illegal to request a write lock from a thread that already holds a
 * read-only lock. Doing so results in undefined behavior. Unlock the read-only
 * lock before requesting a write lock.
 *
 * This function returns true if passed a nullptr rwlock.
 *
 * @param rwlock the rwlock to try to lock.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RWLock.LockForWriting
 * @sa RWLock.TryLockForReading
 * @sa RWLock.Unlock
 */
inline void TryLockRWLockForWriting(RWLockParam rwlock)
{
  CheckError(SDL_TryLockRWLockForWriting(rwlock));
}

inline void RWLock::TryLockForWriting()
{
  SDL::TryLockRWLockForWriting(m_resource);
}

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
 * @param rwlock the rwlock to unlock.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RWLock.LockForReading
 * @sa RWLock.LockForWriting
 * @sa RWLock.TryLockForReading
 * @sa RWLock.TryLockForWriting
 */
inline void UnlockRWLock(RWLockParam rwlock) { SDL_UnlockRWLock(rwlock); }

inline void RWLock::Unlock() { SDL::UnlockRWLock(m_resource); }

/**
 * Destroy a read/write lock created with RWLock.RWLock().
 *
 * This function must be called on any read/write lock that is no longer needed.
 * Failure to destroy a rwlock will result in a system memory or resource leak.
 * While it is safe to destroy a rwlock that is _unlocked_, it is not safe to
 * attempt to destroy a locked rwlock, and may result in undefined behavior
 * depending on the platform.
 *
 * @param rwlock the rwlock to destroy.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RWLock.RWLock
 */
inline void DestroyRWLock(RWLockRaw rwlock) { SDL_DestroyRWLock(rwlock); }

inline void RWLock::Destroy() { DestroyRWLock(release()); }

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
 */
class Semaphore
{
  SemaphoreRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Semaphore() = default;

  /**
   * Constructs from SemaphoreParam.
   *
   * @param resource a SemaphoreRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Semaphore(const SemaphoreRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Semaphore(const Semaphore& other) = delete;

  /// Move constructor
  constexpr Semaphore(Semaphore&& other)
    : Semaphore(other.release())
  {
  }

  constexpr Semaphore(const SemaphoreRef& other) = delete;

  constexpr Semaphore(SemaphoreRef&& other) = delete;

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
   *       information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Semaphore.Destroy
   * @sa Semaphore.Signal
   * @sa Semaphore.TryWait
   * @sa Semaphore.GetValue
   * @sa Semaphore.Wait
   * @sa Semaphore.WaitTimeout
   */
  Semaphore(Uint32 initial_value)
    : m_resource(SDL_CreateSemaphore(initial_value))
  {
  }

  /// Destructor
  ~Semaphore() { SDL_DestroySemaphore(m_resource); }

  /// Assignment operator.
  constexpr Semaphore& operator=(Semaphore&& other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Semaphore& operator=(const Semaphore& other)
  {
    m_resource = other.m_resource;
    return *this;
  }

public:
  /// Retrieves underlying SemaphoreRaw.
  constexpr SemaphoreRaw get() const { return m_resource; }

  /// Retrieves underlying SemaphoreRaw and clear this.
  constexpr SemaphoreRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Semaphore& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to SemaphoreParam
  constexpr operator SemaphoreParam() const { return {m_resource}; }

  /**
   * Destroy a semaphore.
   *
   * It is not safe to destroy a semaphore if there are threads currently
   * waiting on it.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Semaphore.Semaphore
   */
  void Destroy();

  /**
   * Wait until a semaphore has a positive value and then decrements it.
   *
   * This function suspends the calling thread until the semaphore pointed to by
   * `sem` has a positive value, and then atomically decrement the semaphore
   * value.
   *
   * This function is the equivalent of calling Semaphore.WaitTimeout() with a
   * time length of -1.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Semaphore.Signal
   * @sa Semaphore.TryWait
   * @sa Semaphore.WaitTimeout
   */
  void Wait();

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
   * @sa Semaphore.Signal
   * @sa Semaphore.Wait
   * @sa Semaphore.WaitTimeout
   */
  bool TryWait();

  /**
   * Wait until a semaphore has a positive value and then decrements it.
   *
   * This function suspends the calling thread until either the semaphore
   * pointed to by `sem` has a positive value or the specified time has elapsed.
   * If the call is successful it will atomically decrement the semaphore value.
   *
   * @param timeoutMS the length of the timeout, in milliseconds, or -1 to wait
   *                  indefinitely.
   * @returns true if the wait succeeds or false if the wait times out.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Semaphore.Signal
   * @sa Semaphore.TryWait
   * @sa Semaphore.Wait
   */
  bool WaitTimeout(std::chrono::milliseconds timeout);

  /**
   * Atomically increment a semaphore's value and wake waiting threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Semaphore.TryWait
   * @sa Semaphore.Wait
   * @sa Semaphore.WaitTimeout
   */
  void Signal();

  /**
   * Get the current value of a semaphore.
   *
   * @returns the current value of the semaphore.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint32 GetValue() const;
};

/// Semi-safe reference for Semaphore.
struct SemaphoreRef : Semaphore
{
  using Semaphore::Semaphore;

  /**
   * Constructs from SemaphoreParam.
   *
   * @param resource a SemaphoreRaw or Semaphore.
   *
   * This does not takes ownership!
   */
  SemaphoreRef(SemaphoreParam resource)
    : Semaphore(resource.value)
  {
  }

  /**
   * Constructs from SemaphoreParam.
   *
   * @param resource a SemaphoreRaw or Semaphore.
   *
   * This does not takes ownership!
   */
  SemaphoreRef(SemaphoreRaw resource)
    : Semaphore(resource)
  {
  }

  /// Copy constructor.
  SemaphoreRef(const SemaphoreRef& other)
    : Semaphore(other.get())
  {
  }

  /// Destructor
  ~SemaphoreRef() { release(); }
};

/**
 * Create a semaphore.
 *
 * This function creates a new semaphore and initializes it with the value
 * `initial_value`. Each wait operation on the semaphore will atomically
 * decrement the semaphore value and potentially block if the semaphore value is
 * 0. Each post operation will atomically increment the semaphore value and wake
 * waiting threads and allow them to retry the wait operation.
 *
 * @param initial_value the starting value of the semaphore.
 * @returns a new semaphore or nullptr on failure; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Semaphore.Destroy
 * @sa Semaphore.Signal
 * @sa Semaphore.TryWait
 * @sa Semaphore.GetValue
 * @sa Semaphore.Wait
 * @sa Semaphore.WaitTimeout
 */
inline Semaphore CreateSemaphore(Uint32 initial_value)
{
  return Semaphore(initial_value);
}

/**
 * Destroy a semaphore.
 *
 * It is not safe to destroy a semaphore if there are threads currently waiting
 * on it.
 *
 * @param sem the semaphore to destroy.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Semaphore.Semaphore
 */
inline void DestroySemaphore(SemaphoreRaw sem) { SDL_DestroySemaphore(sem); }

inline void Semaphore::Destroy() { DestroySemaphore(release()); }

/**
 * Wait until a semaphore has a positive value and then decrements it.
 *
 * This function suspends the calling thread until the semaphore pointed to by
 * `sem` has a positive value, and then atomically decrement the semaphore
 * value.
 *
 * This function is the equivalent of calling Semaphore.WaitTimeout() with a
 * time length of -1.
 *
 * @param sem the semaphore wait on.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Semaphore.Signal
 * @sa Semaphore.TryWait
 * @sa Semaphore.WaitTimeout
 */
inline void WaitSemaphore(SemaphoreParam sem) { SDL_WaitSemaphore(sem); }

inline void Semaphore::Wait() { SDL::WaitSemaphore(m_resource); }

/**
 * See if a semaphore has a positive value and decrement it if it does.
 *
 * This function checks to see if the semaphore pointed to by `sem` has a
 * positive value and atomically decrements the semaphore value if it does. If
 * the semaphore doesn't have a positive value, the function immediately returns
 * false.
 *
 * @param sem the semaphore to wait on.
 * @returns true if the wait succeeds, false if the wait would block.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Semaphore.Signal
 * @sa Semaphore.Wait
 * @sa Semaphore.WaitTimeout
 */
inline bool TryWaitSemaphore(SemaphoreParam sem)
{
  return SDL_TryWaitSemaphore(sem);
}

inline bool Semaphore::TryWait() { return SDL::TryWaitSemaphore(m_resource); }

/**
 * Wait until a semaphore has a positive value and then decrements it.
 *
 * This function suspends the calling thread until either the semaphore pointed
 * to by `sem` has a positive value or the specified time has elapsed. If the
 * call is successful it will atomically decrement the semaphore value.
 *
 * @param sem the semaphore to wait on.
 * @param timeoutMS the length of the timeout, in milliseconds, or -1 to wait
 *                  indefinitely.
 * @returns true if the wait succeeds or false if the wait times out.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Semaphore.Signal
 * @sa Semaphore.TryWait
 * @sa Semaphore.Wait
 */
inline bool WaitSemaphoreTimeout(SemaphoreParam sem,
                                 std::chrono::milliseconds timeout)
{
  return SDL_WaitSemaphoreTimeout(sem, timeout);
}

inline bool Semaphore::WaitTimeout(std::chrono::milliseconds timeout)
{
  return SDL::WaitSemaphoreTimeout(m_resource, timeout);
}

/**
 * Atomically increment a semaphore's value and wake waiting threads.
 *
 * @param sem the semaphore to increment.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Semaphore.TryWait
 * @sa Semaphore.Wait
 * @sa Semaphore.WaitTimeout
 */
inline void SignalSemaphore(SemaphoreParam sem) { SDL_SignalSemaphore(sem); }

inline void Semaphore::Signal() { SDL::SignalSemaphore(m_resource); }

/**
 * Get the current value of a semaphore.
 *
 * @param sem the semaphore to query.
 * @returns the current value of the semaphore.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint32 GetSemaphoreValue(SemaphoreParam sem)
{
  return SDL_GetSemaphoreValue(sem);
}

inline Uint32 Semaphore::GetValue() const
{
  return SDL::GetSemaphoreValue(m_resource);
}

/**
 * A means to block multiple threads until a condition is satisfied.
 *
 * Condition variables, paired with an Mutex, let an app halt multiple threads
 * until a condition has occurred, at which time the app can release one or all
 * waiting threads.
 *
 * Wikipedia has a thorough explanation of the concept:
 *
 * https://en.wikipedia.org/wiki/Condition_variable
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class Condition
{
  ConditionRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Condition() = default;

  /**
   * Constructs from ConditionParam.
   *
   * @param resource a ConditionRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Condition(const ConditionRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Condition(const Condition& other) = delete;

  /// Move constructor
  constexpr Condition(Condition&& other)
    : Condition(other.release())
  {
  }

  constexpr Condition(const ConditionRef& other) = delete;

  constexpr Condition(ConditionRef&& other) = delete;

  /**
   * Create a condition variable.
   *
   * @post a new condition variable or nullptr on failure; call GetError() for
   *       more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Condition.Broadcast
   * @sa Condition.Signal
   * @sa Condition.Wait
   * @sa Condition.WaitTimeout
   * @sa Condition.Destroy
   */
  Condition()
    : m_resource(SDL_CreateCondition())
  {
  }

  /// Destructor
  ~Condition() { SDL_DestroyCondition(m_resource); }

  /// Assignment operator.
  constexpr Condition& operator=(Condition&& other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Condition& operator=(const Condition& other)
  {
    m_resource = other.m_resource;
    return *this;
  }

public:
  /// Retrieves underlying ConditionRaw.
  constexpr ConditionRaw get() const { return m_resource; }

  /// Retrieves underlying ConditionRaw and clear this.
  constexpr ConditionRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Condition& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to ConditionParam
  constexpr operator ConditionParam() const { return {m_resource}; }

  /**
   * Destroy a condition variable.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Condition.Condition
   */
  void Destroy();

  /**
   * Restart one of the threads that are waiting on the condition variable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Condition.Broadcast
   * @sa Condition.Wait
   * @sa Condition.WaitTimeout
   */
  void Signal();

  /**
   * Restart all threads that are waiting on the condition variable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Condition.Signal
   * @sa Condition.Wait
   * @sa Condition.WaitTimeout
   */
  void Broadcast();

  /**
   * Wait until a condition variable is signaled.
   *
   * This function unlocks the specified `mutex` and waits for another thread to
   * call Condition.Signal() or Condition.Broadcast() on the condition variable
   * `cond`. Once the condition variable is signaled, the mutex is re-locked and
   * the function returns.
   *
   * The mutex must be locked before calling this function. Locking the mutex
   * recursively (more than once) is not supported and leads to undefined
   * behavior.
   *
   * This function is the equivalent of calling Condition.WaitTimeout() with a
   * time length of -1.
   *
   * @param mutex the mutex used to coordinate thread access.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Condition.Broadcast
   * @sa Condition.Signal
   * @sa Condition.WaitTimeout
   */
  void Wait(MutexParam mutex);

  /**
   * Wait until a condition variable is signaled or a certain time has passed.
   *
   * This function unlocks the specified `mutex` and waits for another thread to
   * call Condition.Signal() or Condition.Broadcast() on the condition variable
   * `cond`, or for the specified time to elapse. Once the condition variable is
   * signaled or the time elapsed, the mutex is re-locked and the function
   * returns.
   *
   * The mutex must be locked before calling this function. Locking the mutex
   * recursively (more than once) is not supported and leads to undefined
   * behavior.
   *
   * @param mutex the mutex used to coordinate thread access.
   * @param timeoutMS the maximum time to wait, in milliseconds, or -1 to wait
   *                  indefinitely.
   * @returns true if the condition variable is signaled, false if the condition
   *          is not signaled in the allotted time.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Condition.Broadcast
   * @sa Condition.Signal
   * @sa Condition.Wait
   */
  bool WaitTimeout(MutexParam mutex, std::chrono::milliseconds timeout);
};

/// Semi-safe reference for Condition.
struct ConditionRef : Condition
{
  using Condition::Condition;

  /**
   * Constructs from ConditionParam.
   *
   * @param resource a ConditionRaw or Condition.
   *
   * This does not takes ownership!
   */
  ConditionRef(ConditionParam resource)
    : Condition(resource.value)
  {
  }

  /**
   * Constructs from ConditionParam.
   *
   * @param resource a ConditionRaw or Condition.
   *
   * This does not takes ownership!
   */
  ConditionRef(ConditionRaw resource)
    : Condition(resource)
  {
  }

  /// Copy constructor.
  ConditionRef(const ConditionRef& other)
    : Condition(other.get())
  {
  }

  /// Destructor
  ~ConditionRef() { release(); }
};

/**
 * Create a condition variable.
 *
 * @returns a new condition variable or nullptr on failure; call GetError() for
 *          more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Condition.Broadcast
 * @sa Condition.Signal
 * @sa Condition.Wait
 * @sa Condition.WaitTimeout
 * @sa Condition.Destroy
 */
inline Condition CreateCondition() { return Condition(); }

/**
 * Destroy a condition variable.
 *
 * @param cond the condition variable to destroy.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Condition.Condition
 */
inline void DestroyCondition(ConditionRaw cond) { SDL_DestroyCondition(cond); }

inline void Condition::Destroy() { DestroyCondition(release()); }

/**
 * Restart one of the threads that are waiting on the condition variable.
 *
 * @param cond the condition variable to signal.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Condition.Broadcast
 * @sa Condition.Wait
 * @sa Condition.WaitTimeout
 */
inline void SignalCondition(ConditionParam cond) { SDL_SignalCondition(cond); }

inline void Condition::Signal() { SDL::SignalCondition(m_resource); }

/**
 * Restart all threads that are waiting on the condition variable.
 *
 * @param cond the condition variable to signal.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Condition.Signal
 * @sa Condition.Wait
 * @sa Condition.WaitTimeout
 */
inline void BroadcastCondition(ConditionParam cond)
{
  SDL_BroadcastCondition(cond);
}

inline void Condition::Broadcast() { SDL::BroadcastCondition(m_resource); }

/**
 * Wait until a condition variable is signaled.
 *
 * This function unlocks the specified `mutex` and waits for another thread to
 * call Condition.Signal() or Condition.Broadcast() on the condition variable
 * `cond`. Once the condition variable is signaled, the mutex is re-locked and
 * the function returns.
 *
 * The mutex must be locked before calling this function. Locking the mutex
 * recursively (more than once) is not supported and leads to undefined
 * behavior.
 *
 * This function is the equivalent of calling Condition.WaitTimeout() with a
 * time length of -1.
 *
 * @param cond the condition variable to wait on.
 * @param mutex the mutex used to coordinate thread access.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Condition.Broadcast
 * @sa Condition.Signal
 * @sa Condition.WaitTimeout
 */
inline void WaitCondition(ConditionParam cond, MutexParam mutex)
{
  SDL_WaitCondition(cond, mutex);
}

inline void Condition::Wait(MutexParam mutex)
{
  SDL::WaitCondition(m_resource, mutex);
}

/**
 * Wait until a condition variable is signaled or a certain time has passed.
 *
 * This function unlocks the specified `mutex` and waits for another thread to
 * call Condition.Signal() or Condition.Broadcast() on the condition variable
 * `cond`, or for the specified time to elapse. Once the condition variable is
 * signaled or the time elapsed, the mutex is re-locked and the function
 * returns.
 *
 * The mutex must be locked before calling this function. Locking the mutex
 * recursively (more than once) is not supported and leads to undefined
 * behavior.
 *
 * @param cond the condition variable to wait on.
 * @param mutex the mutex used to coordinate thread access.
 * @param timeoutMS the maximum time to wait, in milliseconds, or -1 to wait
 *                  indefinitely.
 * @returns true if the condition variable is signaled, false if the condition
 *          is not signaled in the allotted time.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Condition.Broadcast
 * @sa Condition.Signal
 * @sa Condition.Wait
 */
inline bool WaitConditionTimeout(ConditionParam cond,
                                 MutexParam mutex,
                                 std::chrono::milliseconds timeout)
{
  return SDL_WaitConditionTimeout(cond, mutex, timeout);
}

inline bool Condition::WaitTimeout(MutexParam mutex,
                                   std::chrono::milliseconds timeout)
{
  return SDL::WaitConditionTimeout(m_resource, mutex, timeout);
}

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
 * ```c
 *    static SDL_AtomicInitState init;
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
 * or guarantee that nobody is using those resources during cleanup. You should
 * use other mechanisms to protect those, if that's a concern for your code.
 *
 * @since This struct is available since SDL 3.2.0.
 */
struct InitState : InitStateRaw
{
  constexpr InitState()
    : SDL_InitState{0}
  {
  }

  /**
   * Return whether initialization should be done.
   *
   * This function checks the passed in state and if initialization should be
   * done, sets the status to `INIT_STATUS_INITIALIZING` and returns true. If
   * another thread is already modifying this state, it will wait until that's
   * done before returning.
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
  bool ShouldInit();

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
  bool ShouldQuit();

  /**
   * Finish an initialization state transition.
   *
   * This function sets the status of the passed in state to
   * `INIT_STATUS_INITIALIZED` or `INIT_STATUS_UNINITIALIZED` and allows any
   * threads waiting for the status to proceed.
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
  void SetInitialized(bool initialized);
};

/**
 * Return whether initialization should be done.
 *
 * This function checks the passed in state and if initialization should be
 * done, sets the status to `INIT_STATUS_INITIALIZING` and returns true. If
 * another thread is already modifying this state, it will wait until that's
 * done before returning.
 *
 * If this function returns true, the calling code must call
 * InitState.SetInitialized() to complete the initialization.
 *
 * @param state the initialization state to check.
 * @returns true if initialization needs to be done, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa InitState.SetInitialized
 * @sa InitState.ShouldQuit
 */
inline bool ShouldInit(InitStateRaw* state) { return SDL_ShouldInit(state); }

inline bool InitState::ShouldInit() { return SDL::ShouldInit(this); }

/**
 * Return whether cleanup should be done.
 *
 * This function checks the passed in state and if cleanup should be done, sets
 * the status to `INIT_STATUS_UNINITIALIZING` and returns true.
 *
 * If this function returns true, the calling code must call
 * InitState.SetInitialized() to complete the cleanup.
 *
 * @param state the initialization state to check.
 * @returns true if cleanup needs to be done, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa InitState.SetInitialized
 * @sa InitState.ShouldInit
 */
inline bool ShouldQuit(InitStateRaw* state) { return SDL_ShouldQuit(state); }

inline bool InitState::ShouldQuit() { return SDL::ShouldQuit(this); }

/**
 * Finish an initialization state transition.
 *
 * This function sets the status of the passed in state to
 * `INIT_STATUS_INITIALIZED` or `INIT_STATUS_UNINITIALIZED` and allows any
 * threads waiting for the status to proceed.
 *
 * @param state the initialization state to check.
 * @param initialized the new initialization state.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa InitState.ShouldInit
 * @sa InitState.ShouldQuit
 */
inline void SetInitialized(InitStateRaw* state, bool initialized)
{
  SDL_SetInitialized(state, initialized);
}

inline void InitState::SetInitialized(bool initialized)
{
  SDL::SetInitialized(this, initialized);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_MUTEX_H_ */
