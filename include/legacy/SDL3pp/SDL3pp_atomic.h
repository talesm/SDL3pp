#ifndef SDL3PP_ATOMIC_H_
#define SDL3PP_ATOMIC_H_

#include <SDL3/SDL_atomic.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 *
 * @defgroup CategoryAtomic Atomic Operations
 *
 * Atomic operations.
 *
 * IMPORTANT: If you are not an expert in concurrent lockless programming, you
 * should not be using any functions in this file. You should be protecting
 * your data structures with full mutexes instead.
 *
 * ***Seriously, here be dragons!***
 *
 * You can find out a little more about lockless programming and the subtle
 * issues that can arise here:
 * https://learn.microsoft.com/en-us/windows/win32/dxtecharts/lockless-programming
 *
 * There's also lots of good information here:
 *
 * - https://www.1024cores.net/home/lock-free-algorithms
 * - https://preshing.com/
 *
 * These operations may or may not actually be implemented using processor
 * specific atomic operations. When possible they are implemented as true
 * processor specific atomic operations. When that is not possible the are
 * implemented using locks that *do* use the available atomic operations.
 *
 * All of the atomic operations that modify memory are full memory barriers.
 *
 * @{
 */

/**
 * Insert a memory release barrier (function version).
 *
 * Please refer to SDL_MemoryBarrierRelease for details. This is a function
 * version, which might be useful if you need to use this functionality from a
 * scripting language, etc. Also, some of the macro versions call this
 * function behind the scenes, where more heavy lifting can happen inside of
 * SDL. Generally, though, an app written in C/C++/etc should use the macro
 * version, as it will be more efficient.
 *
 * @threadsafety Obviously this function is safe to use from any thread at any
 *               time, but if you find yourself needing this, you are probably
 *               dealing with some very sensitive code; be careful!
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_MemoryBarrierRelease
 */
inline void MemoryBarrierRelease() { SDL_MemoryBarrierReleaseFunction(); }

/**
 * Insert a memory acquire barrier (function version).
 *
 * Please refer to SDL_MemoryBarrierRelease for details. This is a function
 * version, which might be useful if you need to use this functionality from a
 * scripting language, etc. Also, some of the macro versions call this
 * function behind the scenes, where more heavy lifting can happen inside of
 * SDL. Generally, though, an app written in C/C++/etc should use the macro
 * version, as it will be more efficient.
 *
 * @threadsafety Obviously this function is safe to use from any thread at any
 *               time, but if you find yourself needing this, you are probably
 *               dealing with some very sensitive code; be careful!
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_MemoryBarrierAcquire
 */
inline void MemoryBarrierAcquire() { SDL_MemoryBarrierAcquireFunction(); }

#ifdef SDL3PP_DOC

/**
 * Mark a compiler barrier.
 *
 * A compiler barrier prevents the compiler from reordering reads and writes
 * to globally visible variables across the call.
 *
 * This macro only prevents the compiler from reordering reads and writes, it
 * does not prevent the CPU from reordering reads and writes. However, all of
 * the atomic operations that modify memory are full memory barriers.
 *
 * @threadsafety Obviously this macro is safe to use from any thread at any
 *               time, but if you find yourself needing this, you are probably
 *               dealing with some very sensitive code; be careful!
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_CompilerBarrier() DoCompilerSpecificReadWriteBarrier()

/**
 * Insert a memory release barrier (macro version).
 *
 * Memory barriers are designed to prevent reads and writes from being
 * reordered by the compiler and being seen out of order on multi-core CPUs.
 *
 * A typical pattern would be for thread A to write some data and a flag, and
 * for thread B to read the flag and get the data. In this case you would
 * insert a release barrier between writing the data and the flag,
 * guaranteeing that the data write completes no later than the flag is
 * written, and you would insert an acquire barrier between reading the flag
 * and reading the data, to ensure that all the reads associated with the flag
 * have completed.
 *
 * In this pattern you should always see a release barrier paired with an
 * acquire barrier and you should gate the data reads/writes with a single
 * flag variable.
 *
 * For more information on these semantics, take a look at the blog post:
 * http://preshing.com/20120913/acquire-and-release-semantics
 *
 * This is the macro version of this functionality; if possible, SDL will use
 * compiler intrinsics or inline assembly, but some platforms might need to
 * call the function version of this, MemoryBarrierRelease to do
 * the heavy lifting. Apps that can use the macro should favor it over the
 * function.
 *
 * @threadsafety Obviously this macro is safe to use from any thread at any
 *               time, but if you find yourself needing this, you are probably
 *               dealing with some very sensitive code; be careful!
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_MemoryBarrierAcquire
 * @sa MemoryBarrierRelease
 */
#define SDL_MemoryBarrierRelease() SDL_MemoryBarrierReleaseFunction()

/**
 * Insert a memory acquire barrier (macro version).
 *
 * Please see SDL_MemoryBarrierRelease for the details on what memory barriers
 * are and when to use them.
 *
 * This is the macro version of this functionality; if possible, SDL will use
 * compiler intrinsics or inline assembly, but some platforms might need to
 * call the function version of this, MemoryBarrierAcquire, to do
 * the heavy lifting. Apps that can use the macro should favor it over the
 * function.
 *
 * @threadsafety Obviously this macro is safe to use from any thread at any
 *               time, but if you find yourself needing this, you are probably
 *               dealing with some very sensitive code; be careful!
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_MemoryBarrierRelease
 * @sa MemoryBarrierAcquire
 */
#define SDL_MemoryBarrierAcquire() SDL_MemoryBarrierAcquireFunction()

/**
 * A macro to insert a CPU-specific "pause" instruction into the program.
 *
 * This can be useful in busy-wait loops, as it serves as a hint to the CPU as
 * to the program's intent; some CPUs can use this to do more efficient
 * processing. On some platforms, this doesn't do anything, so using this
 * macro might just be a harmless no-op.
 *
 * Note that if you are busy-waiting, there are often more-efficient
 * approaches with other synchronization primitives: mutexes, semaphores,
 * condition variables, etc.
 *
 * @threadsafety This macro is safe to use from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_CPUPauseInstruction()                                              \
  DoACPUPauseInACompilerAndArchitectureSpecificWay

#endif

/**
 * A type representing an atomic integer value.
 *
 * This can be used to manage a value that is synchronized across multiple
 * CPUs without a race condition; when an app sets a value with
 * AtomicInt.Set all other threads, regardless of the CPU it is running on,
 * will see that value when retrieved with AtomicInt.Get, regardless of CPU
 * caches, etc.
 *
 * This is also useful for atomic compare-and-swap operations: a thread can
 * change the value as long as its current value matches expectations. When
 * done in a loop, one can guarantee data consistency across threads without a
 * lock (but the usual warnings apply: if you don't know what you're doing, or
 * you don't do it carefully, you can confidently cause any number of
 * disasters with this, so in most cases, you _should_ use a mutex instead of
 * this!).
 *
 * This is a struct so people don't accidentally use numeric operations on it
 * directly. You have to use SDL atomic functions.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa AtomicInt.CompareAndSwap
 * @sa AtomicInt.Get
 * @sa AtomicInt.Set
 * @sa AtomicInt.Add
 */
class AtomicInt
{
  SDL_AtomicInt m_value;

public:
  /// Constructor
  constexpr AtomicInt(int value = 0)
    : m_value(value)
  {
  }

  AtomicInt(const AtomicInt& value) = delete;

  AtomicInt& operator=(const AtomicInt& value) = delete;

  /// Convert to underlying type
  constexpr operator SDL_AtomicInt*() { return &m_value; }

  /**
   * Set an atomic variable to a new value if it is currently an old value.
   *
   * ***Note: If you don't know what this function is for, you shouldn't use
   * it!***
   *
   * @param oldval the old value.
   * @param newval the new value.
   * @returns true if the atomic variable was set, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AtomicInt.Get
   * @sa AtomicInt.Set
   */
  bool CompareAndSwap(int oldval, int newval)
  {
    return SDL_CompareAndSwapAtomicInt(&m_value, oldval, newval);
  }

  /**
   * Set an atomic variable to a value.
   *
   * This function also acts as a full memory barrier.
   *
   * ***Note: If you don't know what this function is for, you shouldn't use
   * it!***
   *
   * @param v the desired value.
   * @returns the previous value of the atomic variable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AtomicInt.Get
   */
  int Set(int v) { return SDL_SetAtomicInt(&m_value, v); }

  /**
   * Get the value of an atomic variable.
   *
   * ***Note: If you don't know what this function is for, you shouldn't use
   * it!***
   *
   * @returns the current value of an atomic variable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AtomicInt.Set
   */
  int Get() { return SDL_GetAtomicInt(&m_value); }

  /**
   * Add to an atomic variable.
   *
   * This function also acts as a full memory barrier.
   *
   * ***Note: If you don't know what this function is for, you shouldn't use
   * it!***
   *
   * @param v the desired value to add.
   * @returns the previous value of the atomic variable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AtomicInt.AtomicDecRef
   * @sa AtomicInt.AtomicIncRef
   */
  int Add(int v) { return SDL_AddAtomicInt(&m_value, v); }

  /**
   * Increment an atomic variable used as a reference count.
   *
   * ***Note: If you don't know what this macro is for, you shouldn't use it!***
   *
   * @returns the previous value of the atomic variable.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   *
   * @sa AtomicInt.AtomicDecRef
   */
  bool AtomicIncRef() { return SDL_AtomicIncRef(&m_value); }

  /**
   * Decrement an atomic variable used as a reference count.
   *
   * ***Note: If you don't know what this macro is for, you shouldn't use it!***
   *
   * @returns true if the variable reached zero after decrementing, false
   *          otherwise.
   *
   * @threadsafety It is safe to call this macro from any thread.
   *
   * @since This macro is available since SDL 3.2.0.
   *
   * @sa AtomicInt.AtomicIncRef
   */
  bool AtomicDecRef() { return SDL_AtomicDecRef(&m_value); }
};

/**
 * A type representing an atomic unsigned 32-bit value.
 *
 * This can be used to manage a value that is synchronized across multiple
 * CPUs without a race condition; when an app sets a value with
 * AtomicU32.Set all other threads, regardless of the CPU it is running on,
 * will see that value when retrieved with AtomicU32.Get, regardless of CPU
 * caches, etc.
 *
 * This is also useful for atomic compare-and-swap operations: a thread can
 * change the value as long as its current value matches expectations. When
 * done in a loop, one can guarantee data consistency across threads without a
 * lock (but the usual warnings apply: if you don't know what you're doing, or
 * you don't do it carefully, you can confidently cause any number of
 * disasters with this, so in most cases, you _should_ use a mutex instead of
 * this!).
 *
 * This is a struct so people don't accidentally use numeric operations on it
 * directly. You have to use SDL atomic functions.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa AtomicU32.CompareAndSwap
 * @sa AtomicU32.Get
 * @sa AtomicU32.Set
 */
class AtomicU32
{
  SDL_AtomicU32 m_value;

public:
  /// Constructor
  constexpr AtomicU32(Uint32 value = 0)
    : m_value(value)
  {
  }

  AtomicU32(const AtomicU32& value) = delete;

  AtomicU32& operator=(const AtomicU32& value) = delete;

  /// Convert to underlying type
  constexpr operator SDL_AtomicU32*() { return &m_value; }

  /**
   * Set an atomic variable to a new value if it is currently an old value.
   *
   * ***Note: If you don't know what this function is for, you shouldn't use
   * it!***
   *
   * @param oldval the old value.
   * @param newval the new value.
   * @returns true if the atomic variable was set, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AtomicU32.Get
   * @sa AtomicU32.Set
   */
  bool CompareAndSwap(Uint32 oldval, Uint32 newval)
  {
    return SDL_CompareAndSwapAtomicU32(&m_value, oldval, newval);
  }

  /**
   * Set an atomic variable to a value.
   *
   * This function also acts as a full memory barrier.
   *
   * ***Note: If you don't know what this function is for, you shouldn't use
   * it!***
   *
   * @param v the desired value.
   * @returns the previous value of the atomic variable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AtomicU32.Get
   */
  Uint32 Set(Uint32 v) { return SDL_SetAtomicU32(&m_value, v); }

  /**
   * Get the value of an atomic variable.
   *
   * ***Note: If you don't know what this function is for, you shouldn't use
   * it!***
   *
   * @returns the current value of an atomic variable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AtomicU32.Set
   */
  Uint32 Get() { return SDL_GetAtomicU32(&m_value); }
};

/// Type representing an atomic pointer
template<class T>
class AtomicPointer
{
  T* m_value;

public:
  /// Constructor
  constexpr AtomicPointer(T* value = nullptr)
    : m_value(value)
  {
  }

  AtomicPointer(const AtomicPointer& value) = delete;

  AtomicPointer& operator=(const AtomicPointer& value) = delete;

  /**
   * Set a pointer to a new value if it is currently an old value.
   *
   * ***Note: If you don't know what this function is for, you shouldn't use
   * it!***
   *
   * @param oldval the old pointer value.
   * @param newval the new pointer value.
   * @returns true if the pointer was set, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AtomicInt.CompareAndSwap
   * @sa AtomicPointer.Get
   * @sa AtomicPointer.Set
   */
  bool CompareAndSwap(T* oldval, T* newval)
  {
    return SDL_CompareAndSwapAtomicPointer(&m_value, oldval, newval);
  }

  /**
   * Set a pointer to a value atomically.
   *
   * ***Note: If you don't know what this function is for, you shouldn't use
   * it!***
   *
   * @param v the desired pointer value.
   * @returns the previous value of the pointer.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AtomicPointer.CompareAndSwap
   * @sa AtomicPointer.Get
   */
  T* Set(T* v) { return SDL_SetAtomicPointer(&m_value, v); }

  /**
   * Get the value of a pointer atomically.
   *
   * ***Note: If you don't know what this function is for, you shouldn't use
   * it!***
   *
   * @returns the current value of a pointer.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AtomicPointer.CompareAndSwap
   * @sa AtomicPointer.Set
   */
  T* Get() { return SDL_GetAtomicPointer(&m_value); }
};

/// @}
} // namespace SDL

#endif /* SDL3PP_ATOMIC_H_ */
