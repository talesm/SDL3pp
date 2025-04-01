#ifndef SDL3PP_STDINC_H_
#define SDL3PP_STDINC_H_

#include <chrono>
#include <SDL3/SDL_stdinc.h>
#include "SDL3pp_callbackWrapper.h"
#include "SDL3pp_objectWrapper.h"
#include "SDL3pp_optionalRef.h"
#include "SDL3pp_ownPtr.h"
#include "SDL3pp_spanRef.h"
#include "SDL3pp_stringParam.h"

namespace SDL {

/**
 * @defgroup CategoryStdinc Standard Library Functionality
 *
 * SDL provides its own implementation of some of the most important C runtime
 * functions.
 *
 * Using these functions allows an app to have access to common C
 * functionality without depending on a specific C runtime (or a C runtime at
 * all). More importantly, the SDL implementations work identically across
 * platforms, so apps can avoid surprises like snprintf() behaving differently
 * between Windows and Linux builds, or itoa() only existing on some
 * platforms.
 *
 * For many of the most common functions, like SDL_memcpy, SDL might just call
 * through to the usual C runtime behind the scenes, if it makes sense to do
 * so (if it's faster and always available/reliable on a given platform),
 * reducing library size and offering the most optimized option.
 *
 * SDL also offers other C-runtime-adjacent functionality in this header that
 * either isn't, strictly speaking, part of any C runtime standards, like
 * SDL_crc32() and SDL_reinterpret_cast, etc. It also offers a few better
 * options, like SDL_strlcpy(), which functions as a safer form of strcpy().
 *
 * @{
 */

// Forward decl
template<ObjectBox<SDL_Environment*> T>
struct EnvironmentBase;

/**
 * Handle to a non owning environment
 *
 * @cat resource
 *
 * @sa resource
 * @sa EnvironmentBase
 * @sa Environment
 */
using EnvironmentRef = EnvironmentBase<ObjectRef<SDL_Environment>>;

/**
 * Handle to an owning environment
 *
 * @cat resource
 *
 * @sa resource
 * @sa EnvironmentBase
 * @sa EnvironmentRef
 */
using Environment = EnvironmentBase<ObjectUnique<SDL_Environment>>;

// Forward decl
template<ObjectBox<SDL_iconv_data_t*> T>
struct IConvBase;

/**
 * Handle to a non owning iconv
 *
 * @cat resource
 *
 * @sa resource
 * @sa IConvBase
 * @sa IConv
 */
using IConvRef = IConvBase<ObjectRef<SDL_iconv_data_t>>;

/**
 * Handle to an owning iconv
 *
 * @cat resource
 *
 * @sa resource
 * @sa IConvBase
 * @sa IConvRef
 */
using IConv = IConvBase<ObjectUnique<SDL_iconv_data_t>>;

#ifdef SDL3PP_DOC

/**
 * Don't let SDL use "long long" C types.
 *
 * SDL will define this if it believes the compiler doesn't understand the
 * "long long" syntax for C datatypes. This can happen on older compilers.
 *
 * If _your_ compiler doesn't support "long long" but SDL doesn't know it, it
 * is safe to define this yourself to build against the SDL headers.
 *
 * If this is defined, it will remove access to some C runtime support
 * functions, like SDL_ulltoa and SDL_strtoll that refer to this datatype
 * explicitly. The rest of SDL will still be available.
 *
 * SDL's own source code cannot be built with a compiler that has this
 * defined, for various technical reasons.
 */
#define SDL_NOLONGLONG 1

/**
 * The largest value that a `size_t` can hold for the target platform.
 *
 * `size_t` is generally the same size as a pointer in modern times, but this
 * can get weird on very old and very esoteric machines. For example, on a
 * 16-bit Intel 286, you might have a 32-bit "far" pointer (16-bit segment
 * plus 16-bit offset), but `size_t` is 16 bits, because it can only deal with
 * the offset into an individual segment.
 *
 * In modern times, it's generally expected to cover an entire linear address
 * space. But be careful!
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_SIZE_MAX SIZE_MAX

#endif // SDL3PP_DOC

/**
 * The number of elements in a static array.
 *
 * This will compile but return incorrect results for a pointer to an array;
 * it has to be an array the compiler knows the size of.
 *
 * @since This function is available since SDL 3.2.0.
 */
template<class T, std::size_t N>
constexpr std::size_t arraysize(const T (&array)[N])
{
  return SDL_arraysize(array);
}

#ifdef SDL3PP_DOC

/**
 * Macro useful for building other macros with strings in them.
 *
 * For example:
 *
 * ```c
 * #define LOG_ERROR(X) OutputDebugString(SDL_STRINGIFY_ARG(__FUNCTION__) ": " X
 * "@n")`
 * ```
 *
 * @param arg the text to turn into a string literal.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_STRINGIFY_ARG(arg) #arg

#endif // SDL3PP_DOC

/**
 * Define a four character code as a Uint32.
 *
 * @param a the first ASCII character.
 * @param b the second ASCII character.
 * @param c the third ASCII character.
 * @param d the fourth ASCII character.
 * @returns the four characters converted into a Uint32, one character
 *          per-byte.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 **/
constexpr Uint32 FourCC(Uint8 a, Uint8 b, Uint8 c, Uint8 d)
{
  return SDL_FOURCC(a, b, c, d);
}

#ifdef SDL3PP_DOC

/**
 * Append the 64 bit integer suffix to a signed integer literal.
 *
 * This helps compilers that might believe a integer literal larger than
 * 0xFFFFFFFF is overflowing a 32-bit value. Use `SDL_SINT64_C(0xFFFFFFFF1)`
 * instead of `0xFFFFFFFF1` by itself.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_UINT64_C
 */
#define SDL_SINT64_C(c) c##LL /* or whatever the current compiler uses. */

/**
 * Append the 64 bit integer suffix to an unsigned integer literal.
 *
 * This helps compilers that might believe a integer literal larger than
 * 0xFFFFFFFF is overflowing a 32-bit value. Use `SDL_UINT64_C(0xFFFFFFFF1)`
 * instead of `0xFFFFFFFF1` by itself.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_SINT64_C
 */
#define SDL_UINT64_C(c) c##ULL /* or whatever the current compiler uses. */

#endif // SDL3PP_DOC

constexpr Sint8 MAX_SINT8 = SDL_MAX_SINT8;

constexpr Sint8 MIN_SINT8 = SDL_MIN_SINT8;

constexpr Uint8 MAX_UINT8 = SDL_MAX_UINT8;

constexpr Uint8 MIN_UINT8 = SDL_MIN_UINT8;

constexpr Sint16 MAX_SINT16 = SDL_MAX_SINT16;

constexpr Sint16 MIN_SINT16 = SDL_MIN_SINT16;

constexpr Uint16 MAX_UINT16 = SDL_MAX_UINT16;

constexpr Uint16 MIN_UINT16 = SDL_MIN_UINT16;

constexpr Sint32 MAX_SINT32 = SDL_MAX_SINT32;

constexpr Sint32 MIN_SINT32 = SDL_MIN_SINT32;

constexpr Uint32 MAX_UINT32 = SDL_MAX_UINT32;

constexpr Uint8 MIN_UINT32 = SDL_MIN_UINT32;

constexpr Sint64 MAX_SINT64 = SDL_MAX_SINT64;

constexpr Sint64 MIN_SINT64 = SDL_MIN_SINT64;

constexpr Uint64 MAX_UINT64 = SDL_MAX_UINT64;

constexpr Uint8 MIN_UINT64 = SDL_MIN_UINT64;

/**
 * SDL times are signed, 64-bit integers representing nanoseconds since the
 * Unix epoch (Jan 1, 1970).
 *
 * They can be converted between POSIX time_t values with ToPosix()
 * and FromPosix(), and between Windows FILETIME values with
 * ToWindows() and FromWindows().
 *
 * @since This type is available since SDL 3.2.0.
 *
 * @sa SDL_MAX_SINT64
 * @sa SDL_MIN_SINT64
 */
class Time
{
  std::chrono::nanoseconds m_value;

public:
  /// Constructs from a nanoseconds period.
  constexpr Time() = default;

  constexpr explicit Time(std::chrono::nanoseconds time)
    : m_value(time)
  {
  }

  /// True if not zero
  constexpr operator bool() const
  {
    return m_value == std::chrono::nanoseconds{};
  }

  /// Converts to nanoseconds period
  constexpr operator std::chrono::nanoseconds() const { return m_value; }

  static Time Current();

  /// Create from a nanoseconds Sint64.
  static constexpr Time FromNS(Sint64 time)
  {
    return Time{std::chrono::nanoseconds{time}};
  }

  /// Converts to nanoseconds Sint64
  constexpr Sint64 ToNS() const { return m_value.count(); }

  static constexpr Time FromPosix(Sint64 time);

  constexpr Sint64 ToPosix() const;

  static Time FromWindows(Uint32 dwLowDateTime, Uint32 dwHighDateTime);

  void ToWindows(Uint32* dwLowDateTime, Uint32* dwHighDateTime) const;

  /// Increment time
  constexpr Time& operator+=(std::chrono::nanoseconds interval)
  {
    m_value += interval;
    return *this;
  }

  /// Decrement
  constexpr Time& operator-=(std::chrono::nanoseconds interval)
  {
    m_value -= interval;
    return *this;
  }
};

constexpr Time MAX_TIME = Time::FromNS(SDL_MAX_TIME);

constexpr Time MIN_TIME = Time::FromNS(SDL_MIN_TIME);

#ifdef SDL3PP_DOC

/**
 * Epsilon constant, used for comparing floating-point numbers.
 *
 * Equals by default to platform-defined `FLT_EPSILON`, or
 * `1.1920928955078125e-07F` if that's not available.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_FLT_EPSILON 1.1920928955078125e-07F /* 0x0.000002p0 */

/**
 * A macro to initialize an SDL interface.
 *
 * This macro will initialize an SDL interface structure and should be called
 * before you fill out the fields with your implementation.
 *
 * You can use it like this:
 *
 * ```c
 * SDL_IOStreamInterface iface;
 *
 * SDL_INIT_INTERFACE(&iface);
 *
 * // Fill in the interface function pointers with your implementation
 * iface.seek = ...
 *
 * stream = SDL_OpenIO(&iface, NULL);
 * ```
 *
 * If you are using designated initializers, you can use the size of the
 * interface as the version, e.g.
 *
 * ```c
 * SDL_IOStreamInterface iface = {
 *     .version = sizeof(iface),
 *     .seek = ...
 * };
 * stream = SDL_OpenIO(&iface, NULL);
 * ```
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_IOStreamInterface
 * @sa SDL_StorageInterface
 * @sa SDL_VirtualJoystickDesc
 */
#define SDL_INIT_INTERFACE(iface)                                              \
  do {                                                                         \
    SDL_zerop(iface);                                                          \
    (iface)->version = sizeof(*(iface));                                       \
  } while (0)

#endif // SDL3PP_DOC

/**
 * Allocate uninitialized memory.
 *
 * The allocated memory returned by this function must be freed with
 * SDL_free().
 *
 * If `size` is 0, it will be set to 1.
 *
 * If you want to allocate memory aligned to a specific alignment, consider
 * using SDL_aligned_alloc().
 *
 * @param size the size to allocate.
 * @returns a pointer to the allocated memory, or NULL if allocation failed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_free
 * @sa SDL_calloc
 * @sa SDL_realloc
 * @sa SDL_aligned_alloc
 **/
inline void* malloc(size_t size) { return SDL_malloc(size); }

/**
 * Allocate a zero-initialized array.
 *
 * The memory returned by this function must be freed with SDL_free().
 *
 * If either of `nmemb` or `size` is 0, they will both be set to 1.
 *
 * @param nmemb the number of elements in the array.
 * @param size the size of each element of the array.
 * @returns a pointer to the allocated array, or NULL if allocation failed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_free
 * @sa SDL_malloc
 * @sa SDL_realloc
 **/
inline void* calloc(size_t nmemb, size_t size)
{
  return SDL_calloc(nmemb, size);
}

/**
 * Change the size of allocated memory.
 *
 * The memory returned by this function must be freed with SDL_free().
 *
 * If `size` is 0, it will be set to 1. Note that this is unlike some other C
 * runtime `realloc` implementations, which may treat `realloc(mem, 0)` the
 * same way as `free(mem)`.
 *
 * If `mem` is NULL, the behavior of this function is equivalent to
 * SDL_malloc(). Otherwise, the function can have one of three possible
 * outcomes:
 *
 * - If it returns the same pointer as `mem`, it means that `mem` was resized
 *   in place without freeing.
 * - If it returns a different non-NULL pointer, it means that `mem` was freed
 *   and cannot be dereferenced anymore.
 * - If it returns NULL (indicating failure), then `mem` will remain valid and
 *   must still be freed with SDL_free().
 *
 * @param mem a pointer to allocated memory to reallocate, or NULL.
 * @param size the new size of the memory.
 * @returns a pointer to the newly allocated memory, or NULL if allocation
 *          failed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_free
 * @sa SDL_malloc
 * @sa SDL_calloc
 **/
inline void* realloc(void* mem, size_t size) { return SDL_realloc(mem, size); }

/**
 * Free allocated memory.
 *
 * The pointer is no longer valid after this call and cannot be dereferenced
 * anymore.
 *
 * If `mem` is NULL, this function does nothing.
 *
 * @param mem a pointer to allocated memory, or NULL.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_malloc
 * @sa SDL_calloc
 * @sa SDL_realloc
 **/
inline void free(void* mem) { SDL_free(mem); }

/**
 * A callback used to implement SDL_malloc().
 *
 * SDL will always ensure that the passed `size` is greater than 0.
 *
 * @param size the size to allocate.
 * @returns a pointer to the allocated memory, or NULL if allocation failed.
 *
 * @threadsafety It should be safe to call this callback from any thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SDL_malloc
 * @sa SDL_GetOriginalMemoryFunctions
 * @sa SDL_GetMemoryFunctions
 * @sa SDL_SetMemoryFunctions
 **/
using malloc_func = SDL_malloc_func;

/**
 * A callback used to implement SDL_calloc().
 *
 * SDL will always ensure that the passed `nmemb` and `size` are both greater
 * than 0.
 *
 * @param nmemb the number of elements in the array.
 * @param size the size of each element of the array.
 * @returns a pointer to the allocated array, or NULL if allocation failed.
 *
 * @threadsafety It should be safe to call this callback from any thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SDL_calloc
 * @sa SDL_GetOriginalMemoryFunctions
 * @sa SDL_GetMemoryFunctions
 * @sa SDL_SetMemoryFunctions
 **/
using calloc_func = SDL_calloc_func;

/**
 * A callback used to implement SDL_realloc().
 *
 * SDL will always ensure that the passed `size` is greater than 0.
 *
 * @param mem a pointer to allocated memory to reallocate, or NULL.
 * @param size the new size of the memory.
 * @returns a pointer to the newly allocated memory, or NULL if allocation
 *          failed.
 *
 * @threadsafety It should be safe to call this callback from any thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SDL_realloc
 * @sa SDL_GetOriginalMemoryFunctions
 * @sa SDL_GetMemoryFunctions
 * @sa SDL_SetMemoryFunctions
 **/
using realloc_func = SDL_realloc_func;

/**
 * A callback used to implement SDL_free().
 *
 * SDL will always ensure that the passed `mem` is a non-NULL pointer.
 *
 * @param mem a pointer to allocated memory.
 *
 * @threadsafety It should be safe to call this callback from any thread.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SDL_free
 * @sa SDL_GetOriginalMemoryFunctions
 * @sa SDL_GetMemoryFunctions
 * @sa SDL_SetMemoryFunctions
 **/
using free_func = SDL_free_func;

/**
 * Get the original set of SDL memory functions.
 *
 * This is what SDL_malloc and friends will use by default, if there has been
 * no call to SDL_SetMemoryFunctions. This is not necessarily using the C
 * runtime's `malloc` functions behind the scenes! Different platforms and
 * build configurations might do any number of unexpected things.
 *
 * @param malloc_func filled with malloc function.
 * @param calloc_func filled with calloc function.
 * @param realloc_func filled with realloc function.
 * @param free_func filled with free function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline void GetOriginalMemoryFunctions(malloc_func* malloc_func,
                                       calloc_func* calloc_func,
                                       realloc_func* realloc_func,
                                       free_func* free_func)
{
  SDL_GetOriginalMemoryFunctions(
    malloc_func, calloc_func, realloc_func, free_func);
}

/**
 * Get the current set of SDL memory functions.
 *
 * @param malloc_func filled with malloc function.
 * @param calloc_func filled with calloc function.
 * @param realloc_func filled with realloc function.
 * @param free_func filled with free function.
 *
 * @threadsafety This does not hold a lock, so do not call this in the
 *               unlikely event of a background thread calling
 *               SDL_SetMemoryFunctions simultaneously.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_SetMemoryFunctions
 * @sa SDL_GetOriginalMemoryFunctions
 **/
inline void GetMemoryFunctions(malloc_func* malloc_func,
                               calloc_func* calloc_func,
                               realloc_func* realloc_func,
                               free_func* free_func)
{
  SDL_GetMemoryFunctions(malloc_func, calloc_func, realloc_func, free_func);
}

/**
 * Replace SDL's memory allocation functions with a custom set.
 *
 * It is not safe to call this function once any allocations have been made,
 * as future calls to SDL_free will use the new allocator, even if they came
 * from an SDL_malloc made with the old one!
 *
 * If used, usually this needs to be the first call made into the SDL library,
 * if not the very first thing done at program startup time.
 *
 * @param malloc_func custom malloc function.
 * @param calloc_func custom calloc function.
 * @param realloc_func custom realloc function.
 * @param free_func custom free function.
 * @returns true on success or false on failure; call SDL_GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread, but one
 *               should not replace the memory functions once any allocations
 *               are made!
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetMemoryFunctions
 * @sa SDL_GetOriginalMemoryFunctions
 **/
inline bool SetMemoryFunctions(malloc_func malloc_func,
                               calloc_func calloc_func,
                               realloc_func realloc_func,
                               free_func free_func)
{
  return SDL_SetMemoryFunctions(
    malloc_func, calloc_func, realloc_func, free_func);
}

/**
 * Allocate memory aligned to a specific alignment.
 *
 * The memory returned by this function must be freed with SDL_aligned_free(),
 * _not_ SDL_free().
 *
 * If `alignment` is less than the size of `void *`, it will be increased to
 * match that.
 *
 * The returned memory address will be a multiple of the alignment value, and
 * the size of the memory allocated will be a multiple of the alignment value.
 *
 * @param alignment the alignment of the memory.
 * @param size the size to allocate.
 * @returns a pointer to the aligned memory, or NULL if allocation failed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_aligned_free
 **/
inline void* aligned_alloc(size_t alignment, size_t size)
{
  return SDL_aligned_alloc(alignment, size);
}

/**
 * Free memory allocated by SDL_aligned_alloc().
 *
 * The pointer is no longer valid after this call and cannot be dereferenced
 * anymore.
 *
 * If `mem` is NULL, this function does nothing.
 *
 * @param mem a pointer previously returned by SDL_aligned_alloc(), or NULL.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_aligned_alloc
 **/
inline void aligned_free(void* mem) { SDL_aligned_free(mem); }

/**
 * Get the number of outstanding (unfreed) allocations.
 *
 * @returns the number of allocations or -1 if allocation counting is
 *          disabled.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int GetNumAllocations() { return SDL_GetNumAllocations(); }

/**
 * A thread-safe set of environment variables
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa resource
 * @sa Environment
 * @sa EnvironmentRef
 **/
template<ObjectBox<SDL_Environment*> T>
struct EnvironmentBase : T
{
  using T::T;

  /**
   * Create a set of environment variables
   *
   * @param populated true to initialize it from the C runtime environment,
   *                  false to create an empty environment.
   * @post the new environment (convertible to true) on success or convertible
   * to false on failure; call GetError() for more information.
   *
   * @threadsafety If `populated` is false, it is safe to call this function
   *               from any thread, otherwise it is safe if no other threads are
   *               calling setenv() or unsetenv()
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetEnvironmentVariable
   * @sa SDL_GetEnvironmentVariables
   * @sa SDL_SetEnvironmentVariable
   * @sa SDL_UnsetEnvironmentVariable
   * @sa SDL_DestroyEnvironment
   **/
  inline EnvironmentBase(bool populated)
    : T(SDL_CreateEnvironment(populated))
  {
  }

  /**
   * Get the value of a variable in the environment.
   *
   * @param name the name of the variable to get.
   * @returns a pointer to the value of the variable or NULL if it can't be
   *          found.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetEnvironment
   * @sa SDL_CreateEnvironment
   * @sa SDL_GetEnvironmentVariables
   * @sa SDL_SetEnvironmentVariable
   * @sa SDL_UnsetEnvironmentVariable
   **/
  inline const char* GetVariable(StringParam name)
  {
    return SDL_GetEnvironmentVariable(T::get(), name);
  }

  /**
   * Get all variables in the environment.
   *
   * @returns a NULL terminated array of pointers to environment variables in
   *          the form "variable=value" or NULL on failure; call SDL_GetError()
   *          for more information. This is wrapped to be auto-deleted, use
   *          FreeWrapper.release() if you want to manage manually.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetEnvironment
   * @sa SDL_CreateEnvironment
   * @sa SDL_GetEnvironmentVariables
   * @sa SDL_SetEnvironmentVariable
   * @sa SDL_UnsetEnvironmentVariable
   **/
  inline OwnArray<char*> GetVariables()
  {
    return OwnArray<char*>{SDL_GetEnvironmentVariables(T::get())};
  }

  /**
   * Get the Variables count.
   *
   * @return the number of existing environment variables
   *
   * This might be slow.
   */
  inline Uint64 GetVariableCount()
  {
    Uint64 count = 0;
    for (auto vars = GetVariables(); vars != nullptr; ++vars) count += 1;
    return count;
  }

  /**
   * Set the value of a variable in the environment.
   *
   * @param name the name of the variable to set.
   * @param value the value of the variable to set.
   * @param overwrite true to overwrite the variable if it exists, false to
   *                  return success without setting the variable if it already
   *                  exists.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GetEnvironment
   * @sa SDL_CreateEnvironment
   * @sa SDL_GetEnvironmentVariable
   * @sa SDL_GetEnvironmentVariables
   * @sa SDL_UnsetEnvironmentVariable
   **/
  inline bool SetVariable(StringParam name, StringParam value, bool overwrite)
  {
    return SDL_SetEnvironmentVariable(T::get(), name, value, overwrite);
  }

  /**
   * Clear a variable from the environment.
   *
   * @param name the name of the variable to unset.
   * @returns true on success or false on failure; call SDL_GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetEnvironment()
   * @sa EnvironmentBase()
   * @sa GetVariable()
   * @sa GetVariables()
   * @sa SetVariable()
   * @sa UnsetVariable()
   **/
  inline bool UnsetVariable(StringParam name)
  {
    return SDL_UnsetEnvironmentVariable(T::get(), name);
  }

  /**
   * Destroy a set of environment variables.
   *
   * This object becomes empty after the call.
   *
   * @threadsafety It is safe to call this function from any thread, as long as
   *               the environment is no longer in use.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_CreateEnvironment
   */
  void Destroy() { return SDL_DestroyEnvironment(T::release()); }
};

/**
 * Destroy a set of environment variables.
 *
 * @param resource the environment to destroy.
 *
 * @threadsafety It is safe to call this function from any thread, as long as
 *               the environment is no longer in use.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Environment
 * @sa EnvironmentBase
 */
template<>
inline void ObjectRef<SDL_Environment>::doFree(SDL_Environment* resource)
{
  return SDL_DestroyEnvironment(resource);
}

/**
 * Get the process environment.
 *
 * This is initialized at application start and is not affected by setenv()
 * and unsetenv() calls after that point. Use SDL_SetEnvironmentVariable() and
 * SDL_UnsetEnvironmentVariable() if you want to modify this environment, or
 * SDL_setenv_unsafe() or SDL_unsetenv_unsafe() if you want changes to persist
 * in the C runtime environment after SDL_Quit().
 *
 * @returns a pointer to the environment for the process or NULL on failure;
 *          call SDL_GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GetEnvironmentVariable
 * @sa SDL_GetEnvironmentVariables
 * @sa SDL_SetEnvironmentVariable
 * @sa SDL_UnsetEnvironmentVariable
 */
inline EnvironmentRef GetEnvironment() { return SDL_GetEnvironment(); }

/**
 * Get the value of a variable in the environment.
 *
 * This function uses SDL's cached copy of the environment and is thread-safe.
 *
 * @param name the name of the variable to get.
 * @returns a pointer to the value of the variable or NULL if it can't be
 *          found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline const char* getenv(StringParam name) { return SDL_getenv(name); }

/**
 * Get the value of a variable in the environment.
 *
 * This function bypasses SDL's cached copy of the environment and is not
 * thread-safe.
 *
 * @param name the name of the variable to get.
 * @returns a pointer to the value of the variable or NULL if it can't be
 *          found.
 *
 * @threadsafety This function is not thread safe, consider using SDL_getenv()
 *               instead.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_getenv
 **/
inline const char* getenv_unsafe(StringParam name)
{
  return SDL_getenv_unsafe(name);
}

/**
 * Set the value of a variable in the environment.
 *
 * @param name the name of the variable to set.
 * @param value the value of the variable to set.
 * @param overwrite 1 to overwrite the variable if it exists, 0 to return
 *                  success without setting the variable if it already exists.
 * @returns 0 on success, -1 on error.
 *
 * @threadsafety This function is not thread safe, consider using
 *               SDL_SetEnvironmentVariable() instead.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_SetEnvironmentVariable
 **/
inline int setenv_unsafe(StringParam name, StringParam value, int overwrite)
{
  return SDL_setenv_unsafe(name, value, overwrite);
}

/**
 * Clear a variable from the environment.
 *
 * @param name the name of the variable to unset.
 * @returns 0 on success, -1 on error.
 *
 * @threadsafety This function is not thread safe, consider using
 *               SDL_UnsetEnvironmentVariable() instead.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_UnsetEnvironmentVariable
 **/
inline int unsetenv_unsafe(StringParam name)
{
  return SDL_unsetenv_unsafe(name);
}

/**
 * A callback used with SDL sorting and binary search functions.
 *
 * @param a a pointer to the first element being compared.
 * @param b a pointer to the second element being compared.
 * @returns -1 if `a` should be sorted before `b`, 1 if `b` should be sorted
 *          before `a`, 0 if they are equal. If two elements are equal, their
 *          order in the sorted array is undefined.
 *
 * @since This callback is available since SDL 3.2.0.
 *
 * @sa SDL_bsearch
 * @sa SDL_qsort
 **/
using CompareCallback = SDL_CompareCallback;

/**
 * Sort an array.
 *
 * For example:
 *
 * ```c
 * typedef struct {
 *     int key;
 *     const char *string;
 * } data;
 *
 * int SDLCALL compare(const void *a, const void *b)
 * {
 *     const data *A = (const data *)a;
 *     const data *B = (const data *)b;
 *
 *     if (A->n < B->n) {
 *         return -1;
 *     } else if (B->n < A->n) {
 *         return 1;
 *     } else {
 *         return 0;
 *     }
 * }
 *
 * data values[] = {
 *     { 3, "third" }, { 1, "first" }, { 2, "second" }
 * };
 *
 * SDL_qsort(values, SDL_arraysize(values), sizeof(values[0]), compare);
 * ```
 *
 * @param base a pointer to the start of the array.
 * @param nmemb the number of elements in the array.
 * @param size the size of the elements in the array.
 * @param compare a function used to compare elements in the array.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_bsearch
 * @sa SDL_qsort_r
 **/
inline void qsort(void* base,
                  size_t nmemb,
                  size_t size,
                  CompareCallback compare)
{
  SDL_qsort(base, nmemb, size, compare);
}

/**
 * Perform a binary search on a previously sorted array.
 *
 * For example:
 *
 * ```c
 * typedef struct {
 *     int key;
 *     const char *string;
 * } data;
 *
 * int SDLCALL compare(const void *a, const void *b)
 * {
 *     const data *A = (const data *)a;
 *     const data *B = (const data *)b;
 *
 *     if (A->n < B->n) {
 *         return -1;
 *     } else if (B->n < A->n) {
 *         return 1;
 *     } else {
 *         return 0;
 *     }
 * }
 *
 * data values[] = {
 *     { 1, "first" }, { 2, "second" }, { 3, "third" }
 * };
 * data key = { 2, NULL };
 *
 * data *result = SDL_bsearch(&key, values, SDL_arraysize(values),
 *sizeof(values[0]), compare);
 * ```
 *
 * @param key a pointer to a key equal to the element being searched for.
 * @param base a pointer to the start of the array.
 * @param nmemb the number of elements in the array.
 * @param size the size of the elements in the array.
 * @param compare a function used to compare elements in the array.
 * @returns a pointer to the matching element in the array, or NULL if not
 *          found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_bsearch_r
 * @sa SDL_qsort
 **/
inline void* bsearch(const void* key,
                     const void* base,
                     size_t nmemb,
                     size_t size,
                     CompareCallback compare)
{
  return SDL_bsearch(key, base, nmemb, size, compare);
}

/**
 * A callback used with SDL sorting and binary search functions.
 *
 * @param userdata the `userdata` pointer passed to the sort function.
 * @param a a pointer to the first element being compared.
 * @param b a pointer to the second element being compared.
 * @returns -1 if `a` should be sorted before `b`, 1 if `b` should be sorted
 *          before `a`, 0 if they are equal. If two elements are equal, their
 *          order in the sorted array is undefined.
 *
 * @since This callback is available since SDL 3.2.0.
 *
 * @sa SDL_qsort_r
 * @sa SDL_bsearch_r
 **/
using CompareCallback_r = SDL_CompareCallback_r;

/**
 * Sort an array, passing a userdata pointer to the compare function.
 *
 * For example:
 *
 * ```c
 * typedef enum {
 *     sort_increasing,
 *     sort_decreasing,
 * } sort_method;
 *
 * typedef struct {
 *     int key;
 *     const char *string;
 * } data;
 *
 * int SDLCALL compare(const void *userdata, const void *a, const void *b)
 * {
 *     sort_method method = (sort_method)(uintptr_t)userdata;
 *     const data *A = (const data *)a;
 *     const data *B = (const data *)b;
 *
 *     if (A->key < B->key) {
 *         return (method == sort_increasing) ? -1 : 1;
 *     } else if (B->key < A->key) {
 *         return (method == sort_increasing) ? 1 : -1;
 *     } else {
 *         return 0;
 *     }
 * }
 *
 * data values[] = {
 *     { 3, "third" }, { 1, "first" }, { 2, "second" }
 * };
 *
 * SDL_qsort_r(values, SDL_arraysize(values), sizeof(values[0]), compare, (const
 *void *)(uintptr_t)sort_increasing);
 * ```
 *
 * @param base a pointer to the start of the array.
 * @param nmemb the number of elements in the array.
 * @param size the size of the elements in the array.
 * @param compare a function used to compare elements in the array.
 * @param userdata a pointer to pass to the compare function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_bsearch_r
 * @sa SDL_qsort
 **/
inline void qsort_r(void* base,
                    size_t nmemb,
                    size_t size,
                    CompareCallback_r compare,
                    void* userdata)
{
  SDL_qsort_r(base, nmemb, size, compare, userdata);
}

/**
 * Perform a binary search on a previously sorted array, passing a userdata
 * pointer to the compare function.
 *
 * For example:
 *
 * ```c
 * typedef enum {
 *     sort_increasing,
 *     sort_decreasing,
 * } sort_method;
 *
 * typedef struct {
 *     int key;
 *     const char *string;
 * } data;
 *
 * int SDLCALL compare(const void *userdata, const void *a, const void *b)
 * {
 *     sort_method method = (sort_method)(uintptr_t)userdata;
 *     const data *A = (const data *)a;
 *     const data *B = (const data *)b;
 *
 *     if (A->key < B->key) {
 *         return (method == sort_increasing) ? -1 : 1;
 *     } else if (B->key < A->key) {
 *         return (method == sort_increasing) ? 1 : -1;
 *     } else {
 *         return 0;
 *     }
 * }
 *
 * data values[] = {
 *     { 1, "first" }, { 2, "second" }, { 3, "third" }
 * };
 * data key = { 2, NULL };
 *
 * data *result = SDL_bsearch_r(&key, values, SDL_arraysize(values),
 *sizeof(values[0]), compare, (const void *)(uintptr_t)sort_increasing);
 * ```
 *
 * @param key a pointer to a key equal to the element being searched for.
 * @param base a pointer to the start of the array.
 * @param nmemb the number of elements in the array.
 * @param size the size of the elements in the array.
 * @param compare a function used to compare elements in the array.
 * @param userdata a pointer to pass to the compare function.
 * @returns a pointer to the matching element in the array, or NULL if not
 *          found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_bsearch
 * @sa SDL_qsort_r
 **/
inline void* bsearch_r(const void* key,
                       const void* base,
                       size_t nmemb,
                       size_t size,
                       CompareCallback_r compare,
                       void* userdata)
{
  return SDL_bsearch_r(key, base, nmemb, size, compare, userdata);
}

/**
 * Compute the absolute value of `x`.
 *
 * @param x an integer value.
 * @returns the absolute value of x.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int abs(int x) { return SDL_abs(x); }

/**
 * Return the lesser of two values.
 *
 * This is a helper macro that might be more clear than writing out the
 * comparisons directly, and works with any type that can be compared with the
 * `<` operator. However, it double-evaluates both its parameters, so do not
 * use expressions with side-effects here.
 *
 * @param x the first value to compare.
 * @param y the second value to compare.
 * @returns the lesser of `x` and `y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
template<class T, class U>
constexpr T min(T x, U y)
{
  return SDL_min(x, y);
}

/**
 * Return the greater of two values.
 *
 * This is a helper function that might be more clear than writing out the
 * comparisons directly, and works with any type that can be compared with the
 * `>` operator. However, it double-evaluates both its parameters, so do not
 * use expressions with side-effects here.
 *
 * @param x the first value to compare.
 * @param y the second value to compare.
 * @returns the lesser of `x` and `y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
template<class T, class U>
constexpr T max(T x, U y)
{
  return SDL_max(x, y);
}

/**
 * Return a value clamped to a range.
 *
 * If `x` is outside the range a values between `a` and `b`, the returned
 * value will be `a` or `b` as appropriate. Otherwise, `x` is returned.
 *
 * This function will produce incorrect results if `b` is less than `a`.
 *
 * This is a helper function that might be more clear than writing out the
 * comparisons directly, and works with any type that can be compared with the
 * `<` and `>` operators. However, it double-evaluates all its parameters, so
 * do not use expressions with side-effects here.
 *
 * @param x the value to compare.
 * @param a the low end value.
 * @param b the high end value.
 * @returns x, clamped between a and b.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
template<class T, class U, class V>
constexpr T clamp(T x, U a, V b)
{
  return SDL_clamp(x, a, b);
}

/**
 * Query if a character is alphabetic (a letter).
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * for English 'a-z' and 'A-Z' as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int isalpha(int x) { return SDL_isalpha(x); }

/**
 * Query if a character is alphabetic (a letter) or a number.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * for English 'a-z', 'A-Z', and '0-9' as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int isalnum(int x) { return SDL_isalnum(x); }

/**
 * Report if a character is blank (a space or tab).
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * 0x20 (space) or 0x9 (tab) as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int isblank(int x) { return SDL_isblank(x); }

/**
 * Report if a character is a control character.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * 0 through 0x1F, and 0x7F, as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int iscntrl(int x) { return SDL_iscntrl(x); }

/**
 * Report if a character is a numeric digit.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * '0' (0x30) through '9' (0x39), as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int isdigit(int x) { return SDL_isdigit(x); }

/**
 * Report if a character is a hexadecimal digit.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * 'A' through 'F', 'a' through 'f', and '0' through '9', as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int isxdigit(int x) { return SDL_isxdigit(x); }

/**
 * Report if a character is a punctuation mark.
 *
 * **WARNING**: Regardless of system locale, this is equivalent to
 * `((SDL_isgraph(x)) && (!SDL_isalnum(x)))`.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_isgraph
 * @sa SDL_isalnum
 **/
inline int ispunct(int x) { return SDL_ispunct(x); }

/**
 * Report if a character is whitespace.
 *
 * **WARNING**: Regardless of system locale, this will only treat the
 * following ASCII values as true:
 *
 * - space (0x20)
 * - tab (0x09)
 * - newline (0x0A)
 * - vertical tab (0x0B)
 * - form feed (0x0C)
 * - return (0x0D)
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int isspace(int x) { return SDL_isspace(x); }

/**
 * Report if a character is upper case.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * 'A' through 'Z' as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int isupper(int x) { return SDL_isupper(x); }

/**
 * Report if a character is lower case.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * 'a' through 'z' as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int islower(int x) { return SDL_islower(x); }

/**
 * Report if a character is "printable".
 *
 * Be advised that "printable" has a definition that goes back to text
 * terminals from the dawn of computing, making this a sort of special case
 * function that is not suitable for Unicode (or most any) text management.
 *
 * **WARNING**: Regardless of system locale, this will only treat ASCII values
 * ' ' (0x20) through '~' (0x7E) as true.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int isprint(int x) { return SDL_isprint(x); }

/**
 * Report if a character is any "printable" except space.
 *
 * Be advised that "printable" has a definition that goes back to text
 * terminals from the dawn of computing, making this a sort of special case
 * function that is not suitable for Unicode (or most any) text management.
 *
 * **WARNING**: Regardless of system locale, this is equivalent to
 * `(SDL_isprint(x)) && ((x) != ' ')`.
 *
 * @param x character value to check.
 * @returns non-zero if x falls within the character class, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_isprint
 **/
inline int isgraph(int x) { return SDL_isgraph(x); }

/**
 * Convert low-ASCII English letters to uppercase.
 *
 * **WARNING**: Regardless of system locale, this will only convert ASCII
 * values 'a' through 'z' to uppercase.
 *
 * This function returns the uppercase equivalent of `x`. If a character
 * cannot be converted, or is already uppercase, this function returns `x`.
 *
 * @param x character value to check.
 * @returns capitalized version of x, or x if no conversion available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int toupper(int x) { return SDL_toupper(x); }

/**
 * Convert low-ASCII English letters to lowercase.
 *
 * **WARNING**: Regardless of system locale, this will only convert ASCII
 * values 'A' through 'Z' to lowercase.
 *
 * This function returns the lowercase equivalent of `x`. If a character
 * cannot be converted, or is already lowercase, this function returns `x`.
 *
 * @param x character value to check.
 * @returns lowercase version of x, or x if no conversion available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int tolower(int x) { return SDL_tolower(x); }

/**
 * Calculate a CRC-16 value.
 *
 * https://en.wikipedia.org/wiki/Cyclic_redundancy_check
 *
 * This function can be called multiple times, to stream data to be
 * checksummed in blocks. Each call must provide the previous CRC-16 return
 * value to be updated with the next block. The first call to this function
 * for a set of blocks should pass in a zero CRC value.
 *
 * @param crc the current checksum for this data set, or 0 for a new data set.
 * @param data a new block of data to add to the checksum.
 * @param len the size, in bytes, of the new block of data.
 * @returns a CRC-16 checksum value of all blocks in the data set.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline Uint16 crc16(Uint16 crc, const void* data, size_t len)
{
  return SDL_crc16(crc, data, len);
}

/**
 * Calculate a CRC-32 value.
 *
 * https://en.wikipedia.org/wiki/Cyclic_redundancy_check
 *
 * This function can be called multiple times, to stream data to be
 * checksummed in blocks. Each call must provide the previous CRC-32 return
 * value to be updated with the next block. The first call to this function
 * for a set of blocks should pass in a zero CRC value.
 *
 * @param crc the current checksum for this data set, or 0 for a new data set.
 * @param data a new block of data to add to the checksum.
 * @param len the size, in bytes, of the new block of data.
 * @returns a CRC-32 checksum value of all blocks in the data set.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline Uint32 crc32(Uint32 crc, const void* data, size_t len)
{
  return SDL_crc32(crc, data, len);
}

/**
 * Calculate a 32-bit MurmurHash3 value for a block of data.
 *
 * https://en.wikipedia.org/wiki/MurmurHash
 *
 * A seed may be specified, which changes the final results consistently, but
 * this does not work like SDL_crc16 and SDL_crc32: you can't feed a previous
 * result from this function back into itself as the next seed value to
 * calculate a hash in chunks; it won't produce the same hash as it would if
 * the same data was provided in a single call.
 *
 * If you aren't sure what to provide for a seed, zero is fine. Murmur3 is not
 * cryptographically secure, so it shouldn't be used for hashing top-secret
 * data.
 *
 * @param data the data to be hashed.
 * @param len the size of data, in bytes.
 * @param seed a value that alters the final hash value.
 * @returns a Murmur3 32-bit hash value.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline Uint32 murmur3_32(const void* data, size_t len, Uint32 seed)
{
  return SDL_murmur3_32(data, len, seed);
}

/**
 * Copy non-overlapping memory.
 *
 * The memory regions must not overlap. If they do, use SDL_memmove() instead.
 *
 * @param dst The destination memory region. Must not be NULL, and must not
 *            overlap with `src`.
 * @param src The source memory region. Must not be NULL, and must not overlap
 *            with `dst`.
 * @param len The length in bytes of both `dst` and `src`.
 * @returns `dst`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_memmove
 **/
inline void* memcpy(void* dst, const void* src, size_t len)
{
  return SDL_memcpy(dst, src, len);
}

#ifdef SDL3PP_DOC

/**
 * A macro to copy memory between objects, with basic type checking.
 *
 * SDL_memcpy and SDL_memmove do not care where you copy memory to and from,
 * which can lead to bugs. This macro aims to avoid most of those bugs by
 * making sure that the source and destination are both pointers to objects
 * that are the same size. It does not check that the objects are the same
 * _type_, just that the copy will not overflow either object.
 *
 * The size check happens at compile time, and the compiler will throw an
 * error if the objects are different sizes.
 *
 * Generally this is intended to copy a single object, not an array.
 *
 * This macro looks like it double-evaluates its parameters, but the extras
 * them are in `sizeof` sections, which generate no code nor side-effects.
 *
 * @param dst a pointer to the destination object. Must not be NULL.
 * @param src a pointer to the source object. Must not be NULL.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
#define SDL_copyp(dst, src)                                                    \
  {                                                                            \
    SDL_COMPILE_TIME_ASSERT(SDL_copyp, sizeof(*(dst)) == sizeof(*(src)));      \
  }                                                                            \
  SDL_memcpy((dst), (src), sizeof(*(src)))

#endif // SDL3PP_DOC

/**
 * Copy memory ranges that might overlap.
 *
 * It is okay for the memory regions to overlap. If you are confident that the
 * regions never overlap, using SDL_memcpy() may improve performance.
 *
 * @param dst The destination memory region. Must not be NULL.
 * @param src The source memory region. Must not be NULL.
 * @param len The length in bytes of both `dst` and `src`.
 * @returns `dst`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_memcpy
 **/
inline void* memmove(void* dst, const void* src, size_t len)
{
  return SDL_memmove(dst, src, len);
}

/**
 * Initialize all bytes of buffer of memory to a specific value.
 *
 * This function will set `len` bytes, pointed to by `dst`, to the value
 * specified in `c`.
 *
 * Despite `c` being an `int` instead of a `char`, this only operates on
 * bytes; `c` must be a value between 0 and 255, inclusive.
 *
 * @param dst the destination memory region. Must not be NULL.
 * @param c the byte value to set.
 * @param len the length, in bytes, to set in `dst`.
 * @returns `dst`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline void* memset(void* dst, int c, size_t len)
{
  return SDL_memset(dst, c, len);
}

/**
 * Initialize all 32-bit words of buffer of memory to a specific value.
 *
 * This function will set a buffer of `dwords` Uint32 values, pointed to by
 * `dst`, to the value specified in `val`.
 *
 * Unlike SDL_memset, this sets 32-bit values, not bytes, so it's not limited
 * to a range of 0-255.
 *
 * @param dst the destination memory region. Must not be NULL.
 * @param val the Uint32 value to set.
 * @param dwords the number of Uint32 values to set in `dst`.
 * @returns `dst`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline void* memset4(void* dst, Uint32 val, size_t dwords)
{
  return SDL_memset4(dst, val, dwords);
}

/**
 * Clear an object's memory to zero.
 *
 * This is wrapper over SDL_memset that handles calculating the object size,
 * so there's no chance of copy/paste errors, and the code is cleaner.
 *
 * This requires an object, not a pointer to an object, nor an array.
 *
 * @param x the object to clear.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_zerop
 * @sa SDL_zeroa
 */
template<class T>
inline void zero(T& x)
{
  SDL_zero(x);
}

/**
 * Clear an object's memory to zero, using a pointer.
 *
 * This is wrapper over SDL_memset that handles calculating the object size,
 * so there's no chance of copy/paste errors, and the code is cleaner.
 *
 * This requires a pointer to an object, not an object itself, nor an array.
 *
 * @param x a pointer to the object to clear.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_zero
 * @sa SDL_zeroa
 */
template<class T>
inline void zerop(T* x)
{
  SDL_zerop(x);
}

/**
 * Clear an array's memory to zero.
 *
 * This is wrapper over SDL_memset that handles calculating the array size, so
 * there's no chance of copy/paste errors, and the code is cleaner.
 *
 * This requires an array, not an object, nor a pointer to an object.
 *
 * @param x an array to clear.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_zero
 * @sa SDL_zeroa
 */
template<class T, std::size_t N>
inline void zeroa(T (&x)[N])
{
  SDL_zeroa(x);
}

/**
 * Compare two buffers of memory.
 *
 * @param s1 the first buffer to compare. NULL is not permitted!
 * @param s2 the second buffer to compare. NULL is not permitted!
 * @param len the number of bytes to compare between the buffers.
 * @returns less than zero if s1 is "less than" s2, greater than zero if s1 is
 *          "greater than" s2, and zero if the buffers match exactly for `len`
 *          bytes.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int memcmp(const void* s1, const void* s2, size_t len)
{
  return SDL_memcmp(s1, s2, len);
}

/**
 * This works exactly like wcslen() but doesn't require access to a C runtime.
 *
 * Counts the number of wchar_t values in `wstr`, excluding the null
 * terminator.
 *
 * Like SDL_strlen only counts bytes and not codepoints in a UTF-8 string,
 * this counts wchar_t values in a string, even if the string's encoding is of
 * variable width, like UTF-16.
 *
 * Also be aware that wchar_t is different sizes on different platforms (4
 * bytes on Linux, 2 on Windows, etc).
 *
 * @param wstr The null-terminated wide string to read. Must not be NULL.
 * @returns the length (in wchar_t values, excluding the null terminator) of
 *          `wstr`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_wcsnlen
 * @sa SDL_utf8strlen
 * @sa SDL_utf8strnlen
 **/
inline size_t wcslen(const wchar_t* wstr) { return SDL_wcslen(wstr); }

/**
 * This works exactly like wcsnlen() but doesn't require access to a C
 * runtime.
 *
 * Counts up to a maximum of `maxlen` wchar_t values in `wstr`, excluding the
 * null terminator.
 *
 * Like SDL_strnlen only counts bytes and not codepoints in a UTF-8 string,
 * this counts wchar_t values in a string, even if the string's encoding is of
 * variable width, like UTF-16.
 *
 * Also be aware that wchar_t is different sizes on different platforms (4
 * bytes on Linux, 2 on Windows, etc).
 *
 * Also, `maxlen` is a count of wide characters, not bytes!
 *
 * @param wstr The null-terminated wide string to read. Must not be NULL.
 * @param maxlen The maximum amount of wide characters to count.
 * @returns the length (in wide characters, excluding the null terminator) of
 *          `wstr` but never more than `maxlen`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_wcslen
 * @sa SDL_utf8strlen
 * @sa SDL_utf8strnlen
 **/
inline size_t wcsnlen(const wchar_t* wstr, size_t maxlen)
{
  return SDL_wcsnlen(wstr, maxlen);
}

/**
 * Copy a wide string.
 *
 * This function copies `maxlen` - 1 wide characters from `src` to `dst`, then
 * appends a null terminator.
 *
 * `src` and `dst` must not overlap.
 *
 * If `maxlen` is 0, no wide characters are copied and no null terminator is
 * written.
 *
 * @param dst The destination buffer. Must not be NULL, and must not overlap
 *            with `src`.
 * @param src The null-terminated wide string to copy. Must not be NULL, and
 *            must not overlap with `dst`.
 * @param maxlen The length (in wide characters) of the destination buffer.
 * @returns the length (in wide characters, excluding the null terminator) of
 *          `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_wcslcat
 **/
inline size_t wcslcpy(wchar_t* dst, const wchar_t* src, size_t maxlen)
{
  return SDL_wcslcpy(dst, src, maxlen);
}

/**
 * Concatenate wide strings.
 *
 * This function appends up to `maxlen` - SDL_wcslen(dst) - 1 wide characters
 * from `src` to the end of the wide string in `dst`, then appends a null
 * terminator.
 *
 * `src` and `dst` must not overlap.
 *
 * If `maxlen` - SDL_wcslen(dst) - 1 is less than or equal to 0, then `dst` is
 * unmodified.
 *
 * @param dst The destination buffer already containing the first
 *            null-terminated wide string. Must not be NULL and must not
 *            overlap with `src`.
 * @param src The second null-terminated wide string. Must not be NULL, and
 *            must not overlap with `dst`.
 * @param maxlen The length (in wide characters) of the destination buffer.
 * @returns the length (in wide characters, excluding the null terminator) of
 *          the string in `dst` plus the length of `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_wcslcpy
 **/
inline size_t wcslcat(wchar_t* dst, const wchar_t* src, size_t maxlen)
{
  return SDL_wcslcat(dst, src, maxlen);
}

/**
 * Allocate a copy of a wide string.
 *
 * This allocates enough space for a null-terminated copy of `wstr`, using
 * SDL_malloc, and then makes a copy of the string into this space.
 *
 * The returned string is owned by the caller, and should be passed to
 * SDL_free when no longer needed.
 *
 * @param wstr the string to copy.
 * @returns a pointer to the newly-allocated wide string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline wchar_t* wcsdup(const wchar_t* wstr) { return SDL_wcsdup(wstr); }

/**
 * Search a wide string for the first instance of a specific substring.
 *
 * The search ends once it finds the requested substring, or a null terminator
 * byte to end the string.
 *
 * Note that this looks for strings of _wide characters_, not _codepoints_, so
 * it's legal to search for malformed and incomplete UTF-16 sequences.
 *
 * @param haystack the wide string to search. Must not be NULL.
 * @param needle the wide string to search for. Must not be NULL.
 * @returns a pointer to the first instance of `needle` in the string, or NULL
 *          if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline wchar_t* wcsstr(const wchar_t* haystack, const wchar_t* needle)
{
  return SDL_wcsstr(haystack, needle);
}

/**
 * Search a wide string, up to n wide chars, for the first instance of a
 * specific substring.
 *
 * The search ends once it finds the requested substring, or a null terminator
 * value to end the string, or `maxlen` wide character have been examined. It
 * is possible to use this function on a wide string without a null
 * terminator.
 *
 * Note that this looks for strings of _wide characters_, not _codepoints_, so
 * it's legal to search for malformed and incomplete UTF-16 sequences.
 *
 * @param haystack the wide string to search. Must not be NULL.
 * @param needle the wide string to search for. Must not be NULL.
 * @param maxlen the maximum number of wide characters to search in
 *               `haystack`.
 * @returns a pointer to the first instance of `needle` in the string, or NULL
 *          if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline wchar_t* wcsnstr(const wchar_t* haystack,
                        const wchar_t* needle,
                        size_t maxlen)
{
  return SDL_wcsnstr(haystack, needle, maxlen);
}

/**
 * Compare two null-terminated wide strings.
 *
 * This only compares wchar_t values until it hits a null-terminating
 * character; it does not care if the string is well-formed UTF-16 (or UTF-32,
 * depending on your platform's wchar_t size), or uses valid Unicode values.
 *
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int wcscmp(const wchar_t* str1, const wchar_t* str2)
{
  return SDL_wcscmp(str1, str2);
}

/**
 * Compare two wide strings up to a number of wchar_t values.
 *
 * This only compares wchar_t values; it does not care if the string is
 * well-formed UTF-16 (or UTF-32, depending on your platform's wchar_t size),
 * or uses valid Unicode values.
 *
 * Note that while this function is intended to be used with UTF-16 (or
 * UTF-32, depending on your platform's definition of wchar_t), it is
 * comparing raw wchar_t values and not Unicode codepoints: `maxlen` specifies
 * a wchar_t limit! If the limit lands in the middle of a multi-wchar UTF-16
 * sequence, it will only compare a portion of the final character.
 *
 * `maxlen` specifies a maximum number of wchar_t to compare; if the strings
 * match to this number of wide chars (or both have matched to a
 * null-terminator character before this count), they will be considered
 * equal.
 *
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @param maxlen the maximum number of wchar_t to compare.
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int wcsncmp(const wchar_t* str1, const wchar_t* str2, size_t maxlen)
{
  return SDL_wcsncmp(str1, str2, maxlen);
}

/**
 * Compare two null-terminated wide strings, case-insensitively.
 *
 * This will work with Unicode strings, using a technique called
 * "case-folding" to handle the vast majority of case-sensitive human
 * languages regardless of system locale. It can deal with expanding values: a
 * German Eszett character can compare against two ASCII 's' chars and be
 * considered a match, for example. A notable exception: it does not handle
 * the Turkish 'i' character; human language is complicated!
 *
 * Depending on your platform, "wchar_t" might be 2 bytes, and expected to be
 * UTF-16 encoded (like Windows), or 4 bytes in UTF-32 format. Since this
 * handles Unicode, it expects the string to be well-formed and not a
 * null-terminated string of arbitrary bytes. Characters that are not valid
 * UTF-16 (or UTF-32) are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), which is to say two strings of random bits may turn out to
 * match if they convert to the same amount of replacement characters.
 *
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int wcscasecmp(const wchar_t* str1, const wchar_t* str2)
{
  return SDL_wcscasecmp(str1, str2);
}

/**
 * Compare two wide strings, case-insensitively, up to a number of wchar_t.
 *
 * This will work with Unicode strings, using a technique called
 * "case-folding" to handle the vast majority of case-sensitive human
 * languages regardless of system locale. It can deal with expanding values: a
 * German Eszett character can compare against two ASCII 's' chars and be
 * considered a match, for example. A notable exception: it does not handle
 * the Turkish 'i' character; human language is complicated!
 *
 * Depending on your platform, "wchar_t" might be 2 bytes, and expected to be
 * UTF-16 encoded (like Windows), or 4 bytes in UTF-32 format. Since this
 * handles Unicode, it expects the string to be well-formed and not a
 * null-terminated string of arbitrary bytes. Characters that are not valid
 * UTF-16 (or UTF-32) are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), which is to say two strings of random bits may turn out to
 * match if they convert to the same amount of replacement characters.
 *
 * Note that while this function might deal with variable-sized characters,
 * `maxlen` specifies a _wchar_ limit! If the limit lands in the middle of a
 * multi-byte UTF-16 sequence, it may convert a portion of the final character
 * to one or more Unicode character U+FFFD (REPLACEMENT CHARACTER) so as not
 * to overflow a buffer.
 *
 * `maxlen` specifies a maximum number of wchar_t values to compare; if the
 * strings match to this number of wchar_t (or both have matched to a
 * null-terminator character before this number of bytes), they will be
 * considered equal.
 *
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @param maxlen the maximum number of wchar_t values to compare.
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int wcsncasecmp(const wchar_t* str1, const wchar_t* str2, size_t maxlen)
{
  return SDL_wcsncasecmp(str1, str2, maxlen);
}

/**
 * Parse a `long` from a wide string.
 *
 * If `str` starts with whitespace, then those whitespace characters are
 * skipped before attempting to parse the number.
 *
 * If the parsed number does not fit inside a `long`, the result is clamped to
 * the minimum and maximum representable `long` values.
 *
 * @param str The null-terminated wide string to read. Must not be NULL.
 * @param endp If not NULL, the address of the first invalid wide character
 *             (i.e. the next character after the parsed number) will be
 *             written to this pointer.
 * @param base The base of the integer to read. Supported values are 0 and 2
 *             to 36 inclusive. If 0, the base will be inferred from the
 *             number's prefix (0x for hexadecimal, 0 for octal, decimal
 *             otherwise).
 * @returns the parsed `long`, or 0 if no number could be parsed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strtol
 **/
inline long wcstol(const wchar_t* str, wchar_t** endp, int base)
{
  return SDL_wcstol(str, endp, base);
}

/**
 * This works exactly like strlen() but doesn't require access to a C runtime.
 *
 * Counts the bytes in `str`, excluding the null terminator.
 *
 * If you need the length of a UTF-8 string, consider using SDL_utf8strlen().
 *
 * @param str The null-terminated string to read. Must not be NULL.
 * @returns the length (in bytes, excluding the null terminator) of `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strnlen
 * @sa SDL_utf8strlen
 * @sa SDL_utf8strnlen
 **/
inline size_t strlen(StringParam str) { return SDL_strlen(str); }

/**
 * This works exactly like strnlen() but doesn't require access to a C
 * runtime.
 *
 * Counts up to a maximum of `maxlen` bytes in `str`, excluding the null
 * terminator.
 *
 * If you need the length of a UTF-8 string, consider using SDL_utf8strnlen().
 *
 * @param str The null-terminated string to read. Must not be NULL.
 * @param maxlen The maximum amount of bytes to count.
 * @returns the length (in bytes, excluding the null terminator) of `src` but
 *          never more than `maxlen`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strlen
 * @sa SDL_utf8strlen
 * @sa SDL_utf8strnlen
 **/
inline size_t strnlen(StringParam str, size_t maxlen)
{
  return SDL_strnlen(str, maxlen);
}

/**
 * Copy a string.
 *
 * This function copies up to `maxlen` - 1 characters from `src` to `dst`,
 * then appends a null terminator.
 *
 * If `maxlen` is 0, no characters are copied and no null terminator is
 * written.
 *
 * If you want to copy an UTF-8 string but need to ensure that multi-byte
 * sequences are not truncated, consider using SDL_utf8strlcpy().
 *
 * @param dst The destination buffer. Must not be NULL, and must not overlap
 *            with `src`.
 * @param src The null-terminated string to copy. Must not be NULL, and must
 *            not overlap with `dst`.
 * @param maxlen The length (in characters) of the destination buffer.
 * @returns the length (in characters, excluding the null terminator) of
 *          `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strlcat
 * @sa SDL_utf8strlcpy
 **/
inline size_t strlcpy(char* dst, StringParam src, size_t maxlen)
{
  return SDL_strlcpy(dst, src, maxlen);
}

/**
 * Copy an UTF-8 string.
 *
 * This function copies up to `dst_bytes` - 1 bytes from `src` to `dst` while
 * also ensuring that the string written to `dst` does not end in a truncated
 * multi-byte sequence. Finally, it appends a null terminator.
 *
 * `src` and `dst` must not overlap.
 *
 * Note that unlike SDL_strlcpy(), this function returns the number of bytes
 * written, not the length of `src`.
 *
 * @param dst The destination buffer. Must not be NULL, and must not overlap
 *            with `src`.
 * @param src The null-terminated UTF-8 string to copy. Must not be NULL, and
 *            must not overlap with `dst`.
 * @param dst_bytes The length (in bytes) of the destination buffer. Must not
 *                  be 0.
 * @returns the number of bytes written, excluding the null terminator.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strlcpy
 **/
inline size_t utf8strlcpy(char* dst, StringParam src, size_t dst_bytes)
{
  return SDL_utf8strlcpy(dst, src, dst_bytes);
}

/**
 * Concatenate strings.
 *
 * This function appends up to `maxlen` - SDL_strlen(dst) - 1 characters from
 * `src` to the end of the string in `dst`, then appends a null terminator.
 *
 * `src` and `dst` must not overlap.
 *
 * If `maxlen` - SDL_strlen(dst) - 1 is less than or equal to 0, then `dst` is
 * unmodified.
 *
 * @param dst The destination buffer already containing the first
 *            null-terminated string. Must not be NULL and must not overlap
 *            with `src`.
 * @param src The second null-terminated string. Must not be NULL, and must
 *            not overlap with `dst`.
 * @param maxlen The length (in characters) of the destination buffer.
 * @returns the length (in characters, excluding the null terminator) of the
 *          string in `dst` plus the length of `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strlcpy
 **/
inline size_t strlcat(char* dst, StringParam src, size_t maxlen)
{
  return SDL_strlcat(dst, src, maxlen);
}

/**
 * Allocate a copy of a string.
 *
 * This allocates enough space for a null-terminated copy of `str`, using
 * SDL_malloc, and then makes a copy of the string into this space.
 *
 * The returned string is owned by the caller, and should be passed to
 * SDL_free when no longer needed.
 *
 * @param str the string to copy.
 * @returns a pointer to the newly-allocated string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strdup(StringParam str) { return SDL_strdup(str); }

/**
 * Allocate a copy of a string, up to n characters.
 *
 * This allocates enough space for a null-terminated copy of `str`, up to
 * `maxlen` bytes, using SDL_malloc, and then makes a copy of the string into
 * this space.
 *
 * If the string is longer than `maxlen` bytes, the returned string will be
 * `maxlen` bytes long, plus a null-terminator character that isn't included
 * in the count.
 *
 * The returned string is owned by the caller, and should be passed to
 * SDL_free when no longer needed.
 *
 * @param str the string to copy.
 * @param maxlen the maximum length of the copied string, not counting the
 *               null-terminator character.
 * @returns a pointer to the newly-allocated string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strndup(StringParam str, size_t maxlen)
{
  return SDL_strndup(str, maxlen);
}

/**
 * Reverse a string's contents.
 *
 * This reverses a null-terminated string in-place. Only the content of the
 * string is reversed; the null-terminator character remains at the end of the
 * reversed string.
 *
 * **WARNING**: This function reverses the _bytes_ of the string, not the
 * codepoints. If `str` is a UTF-8 string with Unicode codepoints > 127, this
 * will ruin the string data. You should only use this function on strings
 * that are completely comprised of low ASCII characters.
 *
 * @param str the string to reverse.
 * @returns `str`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strrev(char* str) { return SDL_strrev(str); }

/**
 * Convert a string to uppercase.
 *
 * **WARNING**: Regardless of system locale, this will only convert ASCII
 * values 'A' through 'Z' to uppercase.
 *
 * This function operates on a null-terminated string of bytes--even if it is
 * malformed UTF-8!--and converts ASCII characters 'a' through 'z' to their
 * uppercase equivalents in-place, returning the original `str` pointer.
 *
 * @param str the string to convert in-place. Can not be NULL.
 * @returns the `str` pointer passed into this function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strlwr
 **/
inline char* strupr(char* str) { return SDL_strupr(str); }

/**
 * Convert a string to lowercase.
 *
 * **WARNING**: Regardless of system locale, this will only convert ASCII
 * values 'A' through 'Z' to lowercase.
 *
 * This function operates on a null-terminated string of bytes--even if it is
 * malformed UTF-8!--and converts ASCII characters 'A' through 'Z' to their
 * lowercase equivalents in-place, returning the original `str` pointer.
 *
 * @param str the string to convert in-place. Can not be NULL.
 * @returns the `str` pointer passed into this function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_strupr
 **/
inline char* strlwr(char* str) { return SDL_strlwr(str); }

/**
 * Search a string for the first instance of a specific byte.
 *
 * The search ends once it finds the requested byte value, or a null
 * terminator byte to end the string.
 *
 * Note that this looks for _bytes_, not _characters_, so you cannot match
 * against a Unicode codepoint > 255, regardless of character encoding.
 *
 * @param str the string to search. Must not be NULL.
 * @param c the byte value to search for.
 * @returns a pointer to the first instance of `c` in the string, or NULL if
 *          not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strchr(StringParam str, int c) { return SDL_strchr(str, c); }

/**
 * Search a string for the last instance of a specific byte.
 *
 * The search must go until it finds a null terminator byte to end the string.
 *
 * Note that this looks for _bytes_, not _characters_, so you cannot match
 * against a Unicode codepoint > 255, regardless of character encoding.
 *
 * @param str the string to search. Must not be NULL.
 * @param c the byte value to search for.
 * @returns a pointer to the last instance of `c` in the string, or NULL if
 *          not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strrchr(StringParam str, int c) { return SDL_strrchr(str, c); }

/**
 * Search a string for the first instance of a specific substring.
 *
 * The search ends once it finds the requested substring, or a null terminator
 * byte to end the string.
 *
 * Note that this looks for strings of _bytes_, not _characters_, so it's
 * legal to search for malformed and incomplete UTF-8 sequences.
 *
 * @param haystack the string to search. Must not be NULL.
 * @param needle the string to search for. Must not be NULL.
 * @returns a pointer to the first instance of `needle` in the string, or NULL
 *          if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strstr(StringParam haystack, StringParam needle)
{
  return SDL_strstr(haystack, needle);
}

/**
 * Search a string, up to n bytes, for the first instance of a specific
 * substring.
 *
 * The search ends once it finds the requested substring, or a null terminator
 * byte to end the string, or `maxlen` bytes have been examined. It is
 * possible to use this function on a string without a null terminator.
 *
 * Note that this looks for strings of _bytes_, not _characters_, so it's
 * legal to search for malformed and incomplete UTF-8 sequences.
 *
 * @param haystack the string to search. Must not be NULL.
 * @param needle the string to search for. Must not be NULL.
 * @param maxlen the maximum number of bytes to search in `haystack`.
 * @returns a pointer to the first instance of `needle` in the string, or NULL
 *          if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strnstr(StringParam haystack, StringParam needle, size_t maxlen)
{
  return SDL_strnstr(haystack, needle, maxlen);
}

/**
 * Search a UTF-8 string for the first instance of a specific substring,
 * case-insensitively.
 *
 * This will work with Unicode strings, using a technique called
 * "case-folding" to handle the vast majority of case-sensitive human
 * languages regardless of system locale. It can deal with expanding values: a
 * German Eszett character can compare against two ASCII 's' chars and be
 * considered a match, for example. A notable exception: it does not handle
 * the Turkish 'i' character; human language is complicated!
 *
 * Since this handles Unicode, it expects the strings to be well-formed UTF-8
 * and not a null-terminated string of arbitrary bytes. Bytes that are not
 * valid UTF-8 are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), which is to say two strings of random bits may turn out to
 * match if they convert to the same amount of replacement characters.
 *
 * @param haystack the string to search. Must not be NULL.
 * @param needle the string to search for. Must not be NULL.
 * @returns a pointer to the first instance of `needle` in the string, or NULL
 *          if not found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strcasestr(StringParam haystack, StringParam needle)
{
  return SDL_strcasestr(haystack, needle);
}

/**
 * This works exactly like strtok_r() but doesn't require access to a C
 * runtime.
 *
 * Break a string up into a series of tokens.
 *
 * To start tokenizing a new string, `str` should be the non-NULL address of
 * the string to start tokenizing. Future calls to get the next token from the
 * same string should specify a NULL.
 *
 * Note that this function will overwrite pieces of `str` with null chars to
 * split it into tokens. This function cannot be used with const/read-only
 * strings!
 *
 * `saveptr` just needs to point to a `char *` that can be overwritten; SDL
 * will use this to save tokenizing state between calls. It is initialized if
 * `str` is non-NULL, and used to resume tokenizing when `str` is NULL.
 *
 * @param str the string to tokenize, or NULL to continue tokenizing.
 * @param delim the delimiter string that separates tokens.
 * @param saveptr pointer to a char *, used for ongoing state.
 * @returns A pointer to the next token, or NULL if no tokens remain.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strtok_r(char* str, StringParam delim, char** saveptr)
{
  return SDL_strtok_r(str, delim, saveptr);
}

/**
 * Count the number of codepoints in a UTF-8 string.
 *
 * Counts the _codepoints_, not _bytes_, in `str`, excluding the null
 * terminator.
 *
 * If you need to count the bytes in a string instead, consider using
 * SDL_strlen().
 *
 * Since this handles Unicode, it expects the strings to be well-formed UTF-8
 * and not a null-terminated string of arbitrary bytes. Bytes that are not
 * valid UTF-8 are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), so a malformed or incomplete UTF-8 sequence might increase the
 * count by several replacement characters.
 *
 * @param str The null-terminated UTF-8 string to read. Must not be NULL.
 * @returns The length (in codepoints, excluding the null terminator) of
 *          `src`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_utf8strnlen
 * @sa SDL_strlen
 **/
inline size_t utf8strlen(StringParam str) { return SDL_utf8strlen(str); }

/**
 * Count the number of codepoints in a UTF-8 string, up to n bytes.
 *
 * Counts the _codepoints_, not _bytes_, in `str`, excluding the null
 * terminator.
 *
 * If you need to count the bytes in a string instead, consider using
 * SDL_strnlen().
 *
 * The counting stops at `bytes` bytes (not codepoints!). This seems
 * counterintuitive, but makes it easy to express the total size of the
 * string's buffer.
 *
 * Since this handles Unicode, it expects the strings to be well-formed UTF-8
 * and not a null-terminated string of arbitrary bytes. Bytes that are not
 * valid UTF-8 are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), so a malformed or incomplete UTF-8 sequence might increase the
 * count by several replacement characters.
 *
 * @param str The null-terminated UTF-8 string to read. Must not be NULL.
 * @param bytes The maximum amount of bytes to count.
 * @returns The length (in codepoints, excluding the null terminator) of `src`
 *          but never more than `maxlen`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_utf8strlen
 * @sa SDL_strnlen
 **/
inline size_t utf8strnlen(StringParam str, size_t bytes)
{
  return SDL_utf8strnlen(str, bytes);
}

/**
 * Convert an integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexidecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use SDL_snprintf to clamp output, or
 * SDL_asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
 * much more space than you expect to use (and don't forget possible negative
 * signs, null terminator bytes, etc).
 *
 * @param value the integer to convert.
 * @param str the buffer to write the string into.
 * @param radix the radix to use for string generation.
 * @returns `str`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_uitoa
 * @sa SDL_ltoa
 * @sa SDL_lltoa
 **/
inline char* itoa(int value, char* str, int radix)
{
  return SDL_itoa(value, str, radix);
}

/**
 * Convert an unsigned integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexidecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use SDL_snprintf to clamp output, or
 * SDL_asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
 * much more space than you expect to use (and don't forget null terminator
 * bytes, etc).
 *
 * @param value the unsigned integer to convert.
 * @param str the buffer to write the string into.
 * @param radix the radix to use for string generation.
 * @returns `str`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_itoa
 * @sa SDL_ultoa
 * @sa SDL_ulltoa
 **/
inline char* uitoa(unsigned int value, char* str, int radix)
{
  return SDL_uitoa(value, str, radix);
}

/**
 * Convert a long integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexidecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use SDL_snprintf to clamp output, or
 * SDL_asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
 * much more space than you expect to use (and don't forget possible negative
 * signs, null terminator bytes, etc).
 *
 * @param value the long integer to convert.
 * @param str the buffer to write the string into.
 * @param radix the radix to use for string generation.
 * @returns `str`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_ultoa
 * @sa SDL_itoa
 * @sa SDL_lltoa
 **/
inline char* ltoa(long value, char* str, int radix)
{
  return SDL_ltoa(value, str, radix);
}

/**
 * Convert an unsigned long integer into a string.
 *
 * This requires a radix to specified for string format. Specifying 10
 * produces a decimal number, 16 hexidecimal, etc. Must be in the range of 2
 * to 36.
 *
 * Note that this function will overflow a buffer if `str` is not large enough
 * to hold the output! It may be safer to use SDL_snprintf to clamp output, or
 * SDL_asprintf to allocate a buffer. Otherwise, it doesn't hurt to allocate
 * much more space than you expect to use (and don't forget null terminator
 * bytes, etc).
 *
 * @param value the unsigned long integer to convert.
 * @param str the buffer to write the string into.
 * @param radix the radix to use for string generation.
 * @returns `str`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_ltoa
 * @sa SDL_uitoa
 * @sa SDL_ulltoa
 **/
inline char* ultoa(unsigned long value, char* str, int radix)
{
  return SDL_ultoa(value, str, radix);
}

/**
 * Parse an `int` from a string.
 *
 * The result of calling `SDL_atoi(str)` is equivalent to
 * `(int)SDL_strtol(str, NULL, 10)`.
 *
 * @param str The null-terminated string to read. Must not be NULL.
 * @returns the parsed `int`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atof
 * @sa SDL_strtol
 * @sa SDL_strtoul
 * @sa SDL_strtoll
 * @sa SDL_strtoull
 * @sa SDL_strtod
 * @sa SDL_itoa
 **/
inline int atoi(StringParam str) { return SDL_atoi(str); }

/**
 * Parse a `double` from a string.
 *
 * The result of calling `SDL_atof(str)` is equivalent to `SDL_strtod(str,
 * NULL)`.
 *
 * @param str The null-terminated string to read. Must not be NULL.
 * @returns the parsed `double`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atoi
 * @sa SDL_strtol
 * @sa SDL_strtoul
 * @sa SDL_strtoll
 * @sa SDL_strtoull
 * @sa SDL_strtod
 **/
inline double atof(StringParam str) { return SDL_atof(str); }

/**
 * Parse a `long` from a string.
 *
 * If `str` starts with whitespace, then those whitespace characters are
 * skipped before attempting to parse the number.
 *
 * If the parsed number does not fit inside a `long`, the result is clamped to
 * the minimum and maximum representable `long` values.
 *
 * @param str The null-terminated string to read. Must not be NULL.
 * @param endp If not NULL, the address of the first invalid character (i.e.
 *             the next character after the parsed number) will be written to
 *             this pointer.
 * @param base The base of the integer to read. Supported values are 0 and 2
 *             to 36 inclusive. If 0, the base will be inferred from the
 *             number's prefix (0x for hexadecimal, 0 for octal, decimal
 *             otherwise).
 * @returns the parsed `long`, or 0 if no number could be parsed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atoi
 * @sa SDL_atof
 * @sa SDL_strtoul
 * @sa SDL_strtoll
 * @sa SDL_strtoull
 * @sa SDL_strtod
 * @sa SDL_ltoa
 * @sa SDL_wcstol
 **/
inline long strtol(StringParam str, char** endp, int base)
{
  return SDL_strtol(str, endp, base);
}

/**
 * Parse an `unsigned long` from a string.
 *
 * If `str` starts with whitespace, then those whitespace characters are
 * skipped before attempting to parse the number.
 *
 * If the parsed number does not fit inside an `unsigned long`, the result is
 * clamped to the maximum representable `unsigned long` value.
 *
 * @param str The null-terminated string to read. Must not be NULL.
 * @param endp If not NULL, the address of the first invalid character (i.e.
 *             the next character after the parsed number) will be written to
 *             this pointer.
 * @param base The base of the integer to read. Supported values are 0 and 2
 *             to 36 inclusive. If 0, the base will be inferred from the
 *             number's prefix (0x for hexadecimal, 0 for octal, decimal
 *             otherwise).
 * @returns the parsed `unsigned long`, or 0 if no number could be parsed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atoi
 * @sa SDL_atof
 * @sa SDL_strtol
 * @sa SDL_strtoll
 * @sa SDL_strtoull
 * @sa SDL_strtod
 * @sa SDL_ultoa
 **/
inline unsigned long strtoul(StringParam str, char** endp, int base)
{
  return SDL_strtoul(str, endp, base);
}

/**
 * Parse a `double` from a string.
 *
 * This function makes fewer guarantees than the C runtime `strtod`:
 *
 * - Only decimal notation is guaranteed to be supported. The handling of
 *   scientific and hexadecimal notation is unspecified.
 * - Whether or not INF and NAN can be parsed is unspecified.
 * - The precision of the result is unspecified.
 *
 * @param str the null-terminated string to read. Must not be NULL.
 * @param endp if not NULL, the address of the first invalid character (i.e.
 *             the next character after the parsed number) will be written to
 *             this pointer.
 * @returns the parsed `double`, or 0 if no number could be parsed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atoi
 * @sa SDL_atof
 * @sa SDL_strtol
 * @sa SDL_strtoll
 * @sa SDL_strtoul
 * @sa SDL_strtoull
 **/
inline double strtod(StringParam str, char** endp)
{
  return SDL_strtod(str, endp);
}

/**
 * Compare two null-terminated UTF-8 strings.
 *
 * Due to the nature of UTF-8 encoding, this will work with Unicode strings,
 * since effectively this function just compares bytes until it hits a
 * null-terminating character. Also due to the nature of UTF-8, this can be
 * used with SDL_qsort() to put strings in (roughly) alphabetical order.
 *
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int strcmp(StringParam str1, StringParam str2)
{
  return SDL_strcmp(str1, str2);
}

/**
 * Compare two UTF-8 strings up to a number of bytes.
 *
 * Due to the nature of UTF-8 encoding, this will work with Unicode strings,
 * since effectively this function just compares bytes until it hits a
 * null-terminating character. Also due to the nature of UTF-8, this can be
 * used with SDL_qsort() to put strings in (roughly) alphabetical order.
 *
 * Note that while this function is intended to be used with UTF-8, it is
 * doing a bytewise comparison, and `maxlen` specifies a _byte_ limit! If the
 * limit lands in the middle of a multi-byte UTF-8 sequence, it will only
 * compare a portion of the final character.
 *
 * `maxlen` specifies a maximum number of bytes to compare; if the strings
 * match to this number of bytes (or both have matched to a null-terminator
 * character before this number of bytes), they will be considered equal.
 *
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @param maxlen the maximum number of _bytes_ to compare.
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int strncmp(StringParam str1, StringParam str2, size_t maxlen)
{
  return SDL_strncmp(str1, str2, maxlen);
}

/**
 * Compare two null-terminated UTF-8 strings, case-insensitively.
 *
 * This will work with Unicode strings, using a technique called
 * "case-folding" to handle the vast majority of case-sensitive human
 * languages regardless of system locale. It can deal with expanding values: a
 * German Eszett character can compare against two ASCII 's' chars and be
 * considered a match, for example. A notable exception: it does not handle
 * the Turkish 'i' character; human language is complicated!
 *
 * Since this handles Unicode, it expects the string to be well-formed UTF-8
 * and not a null-terminated string of arbitrary bytes. Bytes that are not
 * valid UTF-8 are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), which is to say two strings of random bits may turn out to
 * match if they convert to the same amount of replacement characters.
 *
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int strcasecmp(StringParam str1, StringParam str2)
{
  return SDL_strcasecmp(str1, str2);
}

/**
 * Compare two UTF-8 strings, case-insensitively, up to a number of bytes.
 *
 * This will work with Unicode strings, using a technique called
 * "case-folding" to handle the vast majority of case-sensitive human
 * languages regardless of system locale. It can deal with expanding values: a
 * German Eszett character can compare against two ASCII 's' chars and be
 * considered a match, for example. A notable exception: it does not handle
 * the Turkish 'i' character; human language is complicated!
 *
 * Since this handles Unicode, it expects the string to be well-formed UTF-8
 * and not a null-terminated string of arbitrary bytes. Bytes that are not
 * valid UTF-8 are treated as Unicode character U+FFFD (REPLACEMENT
 * CHARACTER), which is to say two strings of random bits may turn out to
 * match if they convert to the same amount of replacement characters.
 *
 * Note that while this function is intended to be used with UTF-8, `maxlen`
 * specifies a _byte_ limit! If the limit lands in the middle of a multi-byte
 * UTF-8 sequence, it may convert a portion of the final character to one or
 * more Unicode character U+FFFD (REPLACEMENT CHARACTER) so as not to overflow
 * a buffer.
 *
 * `maxlen` specifies a maximum number of bytes to compare; if the strings
 * match to this number of bytes (or both have matched to a null-terminator
 * character before this number of bytes), they will be considered equal.
 *
 * @param str1 the first string to compare. NULL is not permitted!
 * @param str2 the second string to compare. NULL is not permitted!
 * @param maxlen the maximum number of bytes to compare.
 * @returns less than zero if str1 is "less than" str2, greater than zero if
 *          str1 is "greater than" str2, and zero if the strings match
 *          exactly.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline int strncasecmp(StringParam str1, StringParam str2, size_t maxlen)
{
  return SDL_strncasecmp(str1, str2, maxlen);
}

/**
 * Searches a string for the first occurence of any character contained in a
 * breakset, and returns a pointer from the string to that character.
 *
 * @param str The null-terminated string to be searched. Must not be NULL, and
 *            must not overlap with `breakset`.
 * @param breakset A null-terminated string containing the list of characters
 *                 to look for. Must not be NULL, and must not overlap with
 *                 `str`.
 * @returns A pointer to the location, in str, of the first occurence of a
 *          character present in the breakset, or NULL if none is found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* strpbrk(StringParam str, StringParam breakset)
{
  return SDL_strpbrk(str, breakset);
}

/**
 * The Unicode REPLACEMENT CHARACTER codepoint.
 *
 * SDL_StepUTF8() and SDL_StepBackUTF8() report this codepoint when they
 * encounter a UTF-8 string with encoding errors.
 *
 * This tends to render as something like a question mark in most places.
 *
 * @since This constant is available since SDL 3.2.0.
 *
 * @sa SDL_StepBackUTF8
 * @sa SDL_StepUTF8
 */
constexpr Uint32 INVALID_UNICODE_CODEPOINT = SDL_INVALID_UNICODE_CODEPOINT;

/**
 * Decode a UTF-8 string, one Unicode codepoint at a time.
 *
 * This will return the first Unicode codepoint in the UTF-8 encoded string in
 * `*pstr`, and then advance `*pstr` past any consumed bytes before returning.
 *
 * It will not access more than `*pslen` bytes from the string. `*pslen` will
 * be adjusted, as well, subtracting the number of bytes consumed.
 *
 * `pslen` is allowed to be NULL, in which case the string _must_ be
 * NULL-terminated, as the function will blindly read until it sees the NULL
 * char.
 *
 * if `*pslen` is zero, it assumes the end of string is reached and returns a
 * zero codepoint regardless of the contents of the string buffer.
 *
 * If the resulting codepoint is zero (a NULL terminator), or `*pslen` is
 * zero, it will not advance `*pstr` or `*pslen` at all.
 *
 * Generally this function is called in a loop until it returns zero,
 * adjusting its parameters each iteration.
 *
 * If an invalid UTF-8 sequence is encountered, this function returns
 * SDL_INVALID_UNICODE_CODEPOINT and advances the string/length by one byte
 * (which is to say, a multibyte sequence might produce several
 * SDL_INVALID_UNICODE_CODEPOINT returns before it syncs to the next valid
 * UTF-8 sequence).
 *
 * Several things can generate invalid UTF-8 sequences, including overlong
 * encodings, the use of UTF-16 surrogate values, and truncated data. Please
 * refer to
 * [RFC3629](https://www.ietf.org/rfc/rfc3629.txt)
 * for details.
 *
 * @param pstr a pointer to a UTF-8 string pointer to be read and adjusted.
 * @param pslen a pointer to the number of bytes in the string, to be read and
 *              adjusted. NULL is allowed.
 * @returns the first Unicode codepoint in the string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline Uint32 StepUTF8(const char** pstr, size_t* pslen)
{
  return SDL_StepUTF8(pstr, pslen);
}

/**
 * Decode a UTF-8 string in reverse, one Unicode codepoint at a time.
 *
 * This will go to the start of the previous Unicode codepoint in the string,
 * move `*pstr` to that location and return that codepoint.
 *
 * If `*pstr` is already at the start of the string), it will not advance
 * `*pstr` at all.
 *
 * Generally this function is called in a loop until it returns zero,
 * adjusting its parameter each iteration.
 *
 * If an invalid UTF-8 sequence is encountered, this function returns
 * SDL_INVALID_UNICODE_CODEPOINT.
 *
 * Several things can generate invalid UTF-8 sequences, including overlong
 * encodings, the use of UTF-16 surrogate values, and truncated data. Please
 * refer to
 * [RFC3629](https://www.ietf.org/rfc/rfc3629.txt)
 * for details.
 *
 * @param start a pointer to the beginning of the UTF-8 string.
 * @param pstr a pointer to a UTF-8 string pointer to be read and adjusted.
 * @returns the previous Unicode codepoint in the string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline Uint32 StepBackUTF8(StringParam start, const char** pstr)
{
  return SDL_StepBackUTF8(start, pstr);
}

/**
 * Convert a single Unicode codepoint to UTF-8.
 *
 * The buffer pointed to by `dst` must be at least 4 bytes long, as this
 * function may generate between 1 and 4 bytes of output.
 *
 * This function returns the first byte _after_ the newly-written UTF-8
 * sequence, which is useful for encoding multiple codepoints in a loop, or
 * knowing where to write a NULL-terminator character to end the string (in
 * either case, plan to have a buffer of _more_ than 4 bytes!).
 *
 * If `codepoint` is an invalid value (outside the Unicode range, or a UTF-16
 * surrogate value, etc), this will use U+FFFD (REPLACEMENT CHARACTER) for the
 * codepoint instead, and not set an error.
 *
 * If `dst` is NULL, this returns NULL immediately without writing to the
 * pointer and without setting an error.
 *
 * @param codepoint a Unicode codepoint to convert to UTF-8.
 * @param dst the location to write the encoded UTF-8. Must point to at least
 *            4 bytes!
 * @returns the first byte past the newly-written UTF-8 sequence.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline char* UCS4ToUTF8(Uint32 codepoint, char* dst)
{
  return SDL_UCS4ToUTF8(codepoint, dst);
}

/**
 * This works exactly like sscanf() but doesn't require access to a C runtime.
 *
 * Scan a string, matching a format string, converting each '%' item and
 * storing it to pointers provided through variable arguments.
 *
 * @param text the string to scan. Must not be NULL.
 * @param fmt a printf-style format string. Must not be NULL.
 * @param ... a list of pointers to values to be filled in with scanned items.
 * @returns the number of items that matched the format string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int sscanf(StringParam text,
                  SDL_SCANF_FORMAT_STRING const char* fmt,
                  ...)
{
  int rc;
  va_list ap;
  va_start(ap, fmt);
  rc = SDL_vsscanf(text, fmt, ap);
  va_end(ap);
  return rc;
}

/**
 * This works exactly like vsscanf() but doesn't require access to a C
 * runtime.
 *
 * Functions identically to SDL_sscanf(), except it takes a `va_list` instead
 * of using `...` variable arguments.
 *
 * @param text the string to scan. Must not be NULL.
 * @param fmt a printf-style format string. Must not be NULL.
 * @param ap a `va_list` of pointers to values to be filled in with scanned
 *           items.
 * @returns the number of items that matched the format string.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int vsscanf(StringParam text,
                   SDL_SCANF_FORMAT_STRING const char* fmt,
                   va_list ap)
{
  return SDL_vsscanf(text, fmt, ap);
}

/**
 * This works exactly like snprintf() but doesn't require access to a C
 * runtime.
 *
 * Format a string of up to `maxlen`-1 bytes, converting each '%' item with
 * values provided through variable arguments.
 *
 * While some C runtimes differ on how to deal with too-large strings, this
 * function null-terminates the output, by treating the null-terminator as
 * part of the `maxlen` count. Note that if `maxlen` is zero, however, no
 * bytes will be written at all.
 *
 * This function returns the number of _bytes_ (not _characters_) that should
 * be written, excluding the null-terminator character. If this returns a
 * number >= `maxlen`, it means the output string was truncated. A negative
 * return value means an error occurred.
 *
 * Referencing the output string's pointer with a format item is undefined
 * behavior.
 *
 * @param text the buffer to write the string into. Must not be NULL.
 * @param maxlen the maximum bytes to write, including the null-terminator.
 * @param fmt a printf-style format string. Must not be NULL.
 * @param ... a list of values to be used with the format string.
 * @returns the number of bytes that should be written, not counting the
 *          null-terminator char, or a negative value on error.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int snprintf(char* text,
                    size_t maxlen,
                    SDL_PRINTF_FORMAT_STRING const char* fmt,
                    ...)
{
  va_list ap;
  int result;

  va_start(ap, fmt);
  result = SDL_vsnprintf(text, maxlen, fmt, ap);
  va_end(ap);

  return result;
}

/**
 * This works exactly like swprintf() but doesn't require access to a C
 * runtime.
 *
 * Format a wide string of up to `maxlen`-1 wchar_t values, converting each
 * '%' item with values provided through variable arguments.
 *
 * While some C runtimes differ on how to deal with too-large strings, this
 * function null-terminates the output, by treating the null-terminator as
 * part of the `maxlen` count. Note that if `maxlen` is zero, however, no wide
 * characters will be written at all.
 *
 * This function returns the number of _wide characters_ (not _codepoints_)
 * that should be written, excluding the null-terminator character. If this
 * returns a number >= `maxlen`, it means the output string was truncated. A
 * negative return value means an error occurred.
 *
 * Referencing the output string's pointer with a format item is undefined
 * behavior.
 *
 * @param text the buffer to write the wide string into. Must not be NULL.
 * @param maxlen the maximum wchar_t values to write, including the
 *               null-terminator.
 * @param fmt a printf-style format string. Must not be NULL.
 * @param ... a list of values to be used with the format string.
 * @returns the number of wide characters that should be written, not counting
 *          the null-terminator char, or a negative value on error.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int swprintf(wchar_t* text,
                    size_t maxlen,
                    SDL_PRINTF_FORMAT_STRING const wchar_t* fmt,
                    ...)
{
  va_list ap;
  int result;

  va_start(ap, fmt);
  result = SDL_vswprintf(text, maxlen, fmt, ap);
  va_end(ap);

  return result;
}

/**
 * This works exactly like vsnprintf() but doesn't require access to a C
 * runtime.
 *
 * Functions identically to SDL_snprintf(), except it takes a `va_list`
 * instead of using `...` variable arguments.
 *
 * @param text the buffer to write the string into. Must not be NULL.
 * @param maxlen the maximum bytes to write, including the null-terminator.
 * @param fmt a printf-style format string. Must not be NULL.
 * @param ap a `va_list` values to be used with the format string.
 * @returns the number of bytes that should be written, not counting the
 *          null-terminator char, or a negative value on error.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int vsnprintf(char* text,
                     size_t maxlen,
                     SDL_PRINTF_FORMAT_STRING const char* fmt,
                     va_list ap)
{
  return SDL_vsnprintf(text, maxlen, fmt, ap);
}

/**
 * This works exactly like vswprintf() but doesn't require access to a C
 * runtime.
 *
 * Functions identically to SDL_swprintf(), except it takes a `va_list`
 * instead of using `...` variable arguments.
 *
 * @param text the buffer to write the string into. Must not be NULL.
 * @param maxlen the maximum wide characters to write, including the
 *               null-terminator.
 * @param fmt a printf-style format wide string. Must not be NULL.
 * @param ap a `va_list` values to be used with the format string.
 * @returns the number of wide characters that should be written, not counting
 *          the null-terminator char, or a negative value on error.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int vswprintf(wchar_t* text,
                     size_t maxlen,
                     SDL_PRINTF_FORMAT_STRING const wchar_t* fmt,
                     va_list ap)
{
  return SDL_vswprintf(text, maxlen, fmt, ap);
}

/**
 * This works exactly like asprintf() but doesn't require access to a C
 * runtime.
 *
 * Functions identically to SDL_snprintf(), except it allocates a buffer large
 * enough to hold the output string on behalf of the caller.
 *
 * On success, this function returns the number of bytes (not characters)
 * comprising the output string, not counting the null-terminator character,
 * and sets `*strp` to the newly-allocated string.
 *
 * On error, this function returns a negative number, and the value of `*strp`
 * is undefined.
 *
 * The returned string is owned by the caller, and should be passed to
 * SDL_free when no longer needed.
 *
 * @param strp on output, is set to the new string. Must not be NULL.
 * @param fmt a printf-style format string. Must not be NULL.
 * @param ... a list of values to be used with the format string.
 * @returns the number of bytes in the newly-allocated string, not counting
 *          the null-terminator char, or a negative value on error.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int asprintf(char** strp, SDL_PRINTF_FORMAT_STRING const char* fmt, ...)
{
  va_list ap;
  int result;

  va_start(ap, fmt);
  result = SDL_vasprintf(strp, fmt, ap);
  va_end(ap);

  return result;
}

/**
 * This works exactly like vasprintf() but doesn't require access to a C
 * runtime.
 *
 * Functions identically to SDL_asprintf(), except it takes a `va_list`
 * instead of using `...` variable arguments.
 *
 * @param strp on output, is set to the new string. Must not be NULL.
 * @param fmt a printf-style format string. Must not be NULL.
 * @param ap a `va_list` values to be used with the format string.
 * @returns the number of bytes in the newly-allocated string, not counting
 *          the null-terminator char, or a negative value on error.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int vasprintf(char** strp,
                     SDL_PRINTF_FORMAT_STRING const char* fmt,
                     va_list ap)
{
  return SDL_vasprintf(strp, fmt, ap);
}

/**
 * Seeds the pseudo-random number generator.
 *
 * Reusing the seed number will cause rand_*() to repeat the same stream
 * of 'random' numbers.
 *
 * @param seed the value to use as a random number seed, or 0 to use
 *             SDL_GetPerformanceCounter().
 *
 * @threadsafety This should be called on the same thread that calls
 *               rand*()
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa rand()
 * @sa rand_bits()
 * @sa randf()
 **/
inline void srand(Uint64 seed) { SDL_srand(seed); }

/**
 * Generate a pseudo-random number less than n for positive n
 *
 * The method used is faster and of better quality than `rand() % n`. Odds are
 * roughly 99.9% even for n = 1 million. Evenness is better for smaller n, and
 * much worse as n gets bigger.
 *
 * Example: to simulate a d6 use `rand(6) + 1` The +1 converts 0..5 to
 * 1..6
 *
 * If you want to generate a pseudo-random number in the full range of Sint32,
 * you should use: (Sint32)SDL_rand_bits()
 *
 * If you want reproducible output, be sure to initialize with srand() first.
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @param n the number of possible outcomes. n must be positive.
 * @returns a random value in the range of [0 .. n-1].
 *
 * @threadsafety All calls should be made from a single thread
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa srand()
 * @sa randf()
 **/
inline Sint32 rand(Sint32 n) { return SDL_rand(n); }

/**
 * Generate a uniform pseudo-random floating point number less than 1.0
 *
 * If you want reproducible output, be sure to initialize with srand() first.
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @returns a random value in the range of [0.0, 1.0).
 *
 * @threadsafety All calls should be made from a single thread
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa srand()
 * @sa rand()
 **/
inline float randf() { return SDL_randf(); }

/**
 * Generate 32 pseudo-random bits.
 *
 * You likely want to use rand() to get a pseudo-random number instead.
 *
 * There are no guarantees as to the quality of the random sequence produced,
 * and this should not be used for security (cryptography, passwords) or where
 * money is on the line (loot-boxes, casinos). There are many random number
 * libraries available with different characteristics and you should pick one
 * of those to meet any serious needs.
 *
 * @returns a random value in the range of [0-SDL_MAX_UINT32].
 *
 * @threadsafety All calls should be made from a single thread
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa rand()
 * @sa randf()
 * @sa srand()
 **/
inline Uint32 rand_bits() { return SDL_rand_bits(); }

/**
 * A independent pseudo random state
 *
 * This can be instantiated in any thread and as long as it is not shared with
 * another thread all members are safe to call.
 *
 * @cat wrap-state
 *
 * @sa wrap-state
 */
class Random
{
  Uint64 m_state;

public:
  /**
   * Init state with the given value
   */
  constexpr Random(Uint64 state)
    : m_state(state)
  {
  }

  /// Convert to the underlying type
  constexpr operator Uint64() { return m_state; }

  /**
   * Generate a pseudo-random number less than n for positive n
   *
   * The method used is faster and of better quality than `rand() % n`. Odds are
   * roughly 99.9% even for n = 1 million. Evenness is better for smaller n, and
   * much worse as n gets bigger.
   *
   * Example: to simulate a d6 use `state.rand(6) + 1` The +1 converts
   * 0..5 to 1..6
   *
   * If you want to generate a pseudo-random number in the full range of Sint32,
   * you should use: (Sint32)state.rand_bits()
   *
   * There are no guarantees as to the quality of the random sequence produced,
   * and this should not be used for security (cryptography, passwords) or where
   * money is on the line (loot-boxes, casinos). There are many random number
   * libraries available with different characteristics and you should pick one
   * of those to meet any serious needs.
   *
   * @param n the number of possible outcomes. n must be positive.
   * @returns a random value in the range of [0 .. n-1].
   *
   * @threadsafety This function is thread-safe, as long as this object
   *               isn't shared between threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL::rand()
   * @sa rand_bits()
   * @sa randf()
   */
  Sint32 rand(Sint32 n) { return SDL_rand_r(&m_state, n); }

  /**
   * Generate a uniform pseudo-random floating point number less than 1.0
   *
   * There are no guarantees as to the quality of the random sequence produced,
   * and this should not be used for security (cryptography, passwords) or where
   * money is on the line (loot-boxes, casinos). There are many random number
   * libraries available with different characteristics and you should pick one
   * of those to meet any serious needs.
   *
   * @returns a random value in the range of [0.0, 1.0).
   *
   * @threadsafety This function is thread-safe, as long as this object
   *               isn't shared between threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa rand_bits()
   * @sa rand()
   * @sa SDL::randf()
   */
  float randf() { return SDL_randf_r(&m_state); }

  /**
   * Generate 32 pseudo-random bits.
   *
   * You likely want to use Random.rand() to get a pseudo-random number instead.
   *
   * There are no guarantees as to the quality of the random sequence produced,
   * and this should not be used for security (cryptography, passwords) or where
   * money is on the line (loot-boxes, casinos). There are many random number
   * libraries available with different characteristics and you should pick one
   * of those to meet any serious needs.
   *
   * @returns a random value in the range of [0-SDL_MAX_UINT32].
   *
   * @threadsafety This function is thread-safe, as long as this object
   *               isn't shared between threads.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa rand()
   * @sa randf()
   * @sa SDL::rand_bits()
   */
  Uint32 rand_bits() { return SDL_rand_bits_r(&m_state); }
};

#ifdef SDL3PP_DOC

/**
 * The value of Pi, as a double-precision floating point literal.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_PI_F
 */
#define SDL_PI_D 3.141592653589793238462643383279502884

/**
 * The value of Pi, as a single-precision floating point literal.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_PI_D
 */
#define SDL_PI_F 3.141592653589793238462643383279502884F

#endif // SDL3PP_DOC

/**
 * Compute the arc cosine of `x`.
 *
 * The definition of `y = acos(x)` is `x = cos(y)`.
 *
 * Domain: `-1 <= x <= 1`
 *
 * Range: `0 <= y <= Pi`
 *
 * This function operates on double-precision floating point values, use
 * SDL_acosf for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns arc cosine of `x`, in radians.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_acosf
 * @sa SDL_asin
 * @sa SDL_cos
 **/
inline double acos(double x) { return SDL_acos(x); }

/**
 * Compute the arc cosine of `x`.
 *
 * The definition of `y = acos(x)` is `x = cos(y)`.
 *
 * Domain: `-1 <= x <= 1`
 *
 * Range: `0 <= y <= Pi`
 *
 * This function operates on single-precision floating point values, use
 * SDL_acos for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns arc cosine of `x`, in radians.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_acos
 * @sa SDL_asinf
 * @sa SDL_cosf
 **/
inline float acosf(float x) { return SDL_acosf(x); }

/**
 * Compute the arc sine of `x`.
 *
 * The definition of `y = asin(x)` is `x = sin(y)`.
 *
 * Domain: `-1 <= x <= 1`
 *
 * Range: `-Pi/2 <= y <= Pi/2`
 *
 * This function operates on double-precision floating point values, use
 * SDL_asinf for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns arc sine of `x`, in radians.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_asinf
 * @sa SDL_acos
 * @sa SDL_sin
 **/
inline double asin(double x) { return SDL_asin(x); }

/**
 * Compute the arc sine of `x`.
 *
 * The definition of `y = asin(x)` is `x = sin(y)`.
 *
 * Domain: `-1 <= x <= 1`
 *
 * Range: `-Pi/2 <= y <= Pi/2`
 *
 * This function operates on single-precision floating point values, use
 * SDL_asin for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns arc sine of `x`, in radians.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_asin
 * @sa SDL_acosf
 * @sa SDL_sinf
 **/
inline float asinf(float x) { return SDL_asinf(x); }

/**
 * Compute the arc tangent of `x`.
 *
 * The definition of `y = atan(x)` is `x = tan(y)`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-Pi/2 <= y <= Pi/2`
 *
 * This function operates on double-precision floating point values, use
 * SDL_atanf for single-precision floats.
 *
 * To calculate the arc tangent of y / x, use SDL_atan2.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns arc tangent of of `x` in radians, or 0 if `x = 0`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atanf
 * @sa SDL_atan2
 * @sa SDL_tan
 **/
inline double atan(double x) { return SDL_atan(x); }

/**
 * Compute the arc tangent of `x`.
 *
 * The definition of `y = atan(x)` is `x = tan(y)`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-Pi/2 <= y <= Pi/2`
 *
 * This function operates on single-precision floating point values, use
 * SDL_atan for dboule-precision floats.
 *
 * To calculate the arc tangent of y / x, use SDL_atan2f.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns arc tangent of of `x` in radians, or 0 if `x = 0`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atan
 * @sa SDL_atan2f
 * @sa SDL_tanf
 **/
inline float atanf(float x) { return SDL_atanf(x); }

/**
 * Compute the arc tangent of `y / x`, using the signs of x and y to adjust
 * the result's quadrant.
 *
 * The definition of `z = atan2(x, y)` is `y = x tan(z)`, where the quadrant
 * of z is determined based on the signs of x and y.
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`
 *
 * Range: `-Pi/2 <= y <= Pi/2`
 *
 * This function operates on double-precision floating point values, use
 * SDL_atan2f for single-precision floats.
 *
 * To calculate the arc tangent of a single value, use SDL_atan.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param y floating point value of the numerator (y coordinate).
 * @param x floating point value of the denominator (x coordinate).
 * @returns arc tangent of of `y / x` in radians, or, if `x = 0`, either
 *          `-Pi/2`, `0`, or `Pi/2`, depending on the value of `y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atan2f
 * @sa SDL_atan
 * @sa SDL_tan
 **/
inline double atan2(double y, double x) { return SDL_atan2(y, x); }

/**
 * Compute the arc tangent of `y / x`, using the signs of x and y to adjust
 * the result's quadrant.
 *
 * The definition of `z = atan2(x, y)` is `y = x tan(z)`, where the quadrant
 * of z is determined based on the signs of x and y.
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`
 *
 * Range: `-Pi/2 <= y <= Pi/2`
 *
 * This function operates on single-precision floating point values, use
 * SDL_atan2 for double-precision floats.
 *
 * To calculate the arc tangent of a single value, use SDL_atanf.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param y floating point value of the numerator (y coordinate).
 * @param x floating point value of the denominator (x coordinate).
 * @returns arc tangent of of `y / x` in radians, or, if `x = 0`, either
 *          `-Pi/2`, `0`, or `Pi/2`, depending on the value of `y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_atan2f
 * @sa SDL_atan
 * @sa SDL_tan
 **/
inline float atan2f(float y, float x) { return SDL_atan2f(y, x); }

/**
 * Compute the ceiling of `x`.
 *
 * The ceiling of `x` is the smallest integer `y` such that `y > x`, i.e `x`
 * rounded up to the nearest integer.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on double-precision floating point values, use
 * SDL_ceilf for single-precision floats.
 *
 * @param x floating point value.
 * @returns the ceiling of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_ceilf
 * @sa SDL_floor
 * @sa SDL_trunc
 * @sa SDL_round
 * @sa SDL_lround
 **/
inline double ceil(double x) { return SDL_ceil(x); }

/**
 * Compute the ceiling of `x`.
 *
 * The ceiling of `x` is the smallest integer `y` such that `y > x`, i.e `x`
 * rounded up to the nearest integer.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on single-precision floating point values, use
 * SDL_ceil for double-precision floats.
 *
 * @param x floating point value.
 * @returns the ceiling of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_ceil
 * @sa SDL_floorf
 * @sa SDL_truncf
 * @sa SDL_roundf
 * @sa SDL_lroundf
 **/
inline float ceilf(float x) { return SDL_ceilf(x); }

/**
 * Copy the sign of one floating-point value to another.
 *
 * The definition of copysign is that ``copysign(x, y) = abs(x) * sign(y)``.
 *
 * Domain: `-INF <= x <= INF`, ``-INF <= y <= f``
 *
 * Range: `-INF <= z <= INF`
 *
 * This function operates on double-precision floating point values, use
 * SDL_copysignf for single-precision floats.
 *
 * @param x floating point value to use as the magnitude.
 * @param y floating point value to use as the sign.
 * @returns the floating point value with the sign of y and the magnitude of
 *          x.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_copysignf
 * @sa SDL_fabs
 **/
inline double copysign(double x, double y) { return SDL_copysign(x, y); }

/**
 * Copy the sign of one floating-point value to another.
 *
 * The definition of copysign is that ``copysign(x, y) = abs(x) * sign(y)``.
 *
 * Domain: `-INF <= x <= INF`, ``-INF <= y <= f``
 *
 * Range: `-INF <= z <= INF`
 *
 * This function operates on single-precision floating point values, use
 * SDL_copysign for double-precision floats.
 *
 * @param x floating point value to use as the magnitude.
 * @param y floating point value to use as the sign.
 * @returns the floating point value with the sign of y and the magnitude of
 *          x.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_copysignf
 * @sa SDL_fabsf
 **/
inline float copysignf(float x, float y) { return SDL_copysignf(x, y); }

/**
 * Compute the cosine of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-1 <= y <= 1`
 *
 * This function operates on double-precision floating point values, use
 * SDL_cosf for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value, in radians.
 * @returns cosine of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_cosf
 * @sa SDL_acos
 * @sa SDL_sin
 **/
inline double cos(double x) { return SDL_cos(x); }

/**
 * Compute the cosine of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-1 <= y <= 1`
 *
 * This function operates on single-precision floating point values, use
 * SDL_cos for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value, in radians.
 * @returns cosine of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_cos
 * @sa SDL_acosf
 * @sa SDL_sinf
 **/
inline float cosf(float x) { return SDL_cosf(x); }

/**
 * Compute the exponential of `x`.
 *
 * The definition of `y = exp(x)` is `y = e^x`, where `e` is the base of the
 * natural logarithm. The inverse is the natural logarithm, SDL_log.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * The output will overflow if `exp(x)` is too large to be represented.
 *
 * This function operates on double-precision floating point values, use
 * SDL_expf for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns value of `e^x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_expf
 * @sa SDL_log
 **/
inline double exp(double x) { return SDL_exp(x); }

/**
 * Compute the exponential of `x`.
 *
 * The definition of `y = exp(x)` is `y = e^x`, where `e` is the base of the
 * natural logarithm. The inverse is the natural logarithm, SDL_logf.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * The output will overflow if `exp(x)` is too large to be represented.
 *
 * This function operates on single-precision floating point values, use
 * SDL_exp for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value.
 * @returns value of `e^x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_exp
 * @sa SDL_logf
 **/
inline float expf(float x) { return SDL_expf(x); }

/**
 * Compute the absolute value of `x`
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * This function operates on double-precision floating point values, use
 * SDL_copysignf for single-precision floats.
 *
 * @param x floating point value to use as the magnitude.
 * @returns the absolute value of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_fabsf
 **/
inline double fabs(double x) { return SDL_fabs(x); }

/**
 * Compute the absolute value of `x`
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * This function operates on single-precision floating point values, use
 * SDL_copysignf for double-precision floats.
 *
 * @param x floating point value to use as the magnitude.
 * @returns the absolute value of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_fabs
 **/
inline float fabsf(float x) { return SDL_fabsf(x); }

/**
 * Compute the floor of `x`.
 *
 * The floor of `x` is the largest integer `y` such that `y > x`, i.e `x`
 * rounded down to the nearest integer.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on double-precision floating point values, use
 * SDL_floorf for single-precision floats.
 *
 * @param x floating point value.
 * @returns the floor of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_floorf
 * @sa SDL_ceil
 * @sa SDL_trunc
 * @sa SDL_round
 * @sa SDL_lround
 **/
inline double floor(double x) { return SDL_floor(x); }

/**
 * Compute the floor of `x`.
 *
 * The floor of `x` is the largest integer `y` such that `y > x`, i.e `x`
 * rounded down to the nearest integer.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on single-precision floating point values, use
 * SDL_floorf for double-precision floats.
 *
 * @param x floating point value.
 * @returns the floor of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_floor
 * @sa SDL_ceilf
 * @sa SDL_truncf
 * @sa SDL_roundf
 * @sa SDL_lroundf
 **/
inline float floorf(float x) { return SDL_floorf(x); }

/**
 * Truncate `x` to an integer.
 *
 * Rounds `x` to the next closest integer to 0. This is equivalent to removing
 * the fractional part of `x`, leaving only the integer part.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on double-precision floating point values, use
 * SDL_truncf for single-precision floats.
 *
 * @param x floating point value.
 * @returns `x` truncated to an integer.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_truncf
 * @sa SDL_fmod
 * @sa SDL_ceil
 * @sa SDL_floor
 * @sa SDL_round
 * @sa SDL_lround
 **/
inline double trunc(double x) { return SDL_trunc(x); }

/**
 * Truncate `x` to an integer.
 *
 * Rounds `x` to the next closest integer to 0. This is equivalent to removing
 * the fractional part of `x`, leaving only the integer part.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on single-precision floating point values, use
 * SDL_truncf for double-precision floats.
 *
 * @param x floating point value.
 * @returns `x` truncated to an integer.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_trunc
 * @sa SDL_fmodf
 * @sa SDL_ceilf
 * @sa SDL_floorf
 * @sa SDL_roundf
 * @sa SDL_lroundf
 **/
inline float truncf(float x) { return SDL_truncf(x); }

/**
 * Return the floating-point remainder of `x / y`
 *
 * Divides `x` by `y`, and returns the remainder.
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`, `y != 0`
 *
 * Range: `-y <= z <= y`
 *
 * This function operates on double-precision floating point values, use
 * SDL_fmodf for single-precision floats.
 *
 * @param x the numerator.
 * @param y the denominator. Must not be 0.
 * @returns the remainder of `x / y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_fmodf
 * @sa SDL_modf
 * @sa SDL_trunc
 * @sa SDL_ceil
 * @sa SDL_floor
 * @sa SDL_round
 * @sa SDL_lround
 **/
inline double fmod(double x, double y) { return SDL_fmod(x, y); }

/**
 * Return the floating-point remainder of `x / y`
 *
 * Divides `x` by `y`, and returns the remainder.
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`, `y != 0`
 *
 * Range: `-y <= z <= y`
 *
 * This function operates on single-precision floating point values, use
 * SDL_fmod for single-precision floats.
 *
 * @param x the numerator.
 * @param y the denominator. Must not be 0.
 * @returns the remainder of `x / y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_fmod
 * @sa SDL_truncf
 * @sa SDL_modff
 * @sa SDL_ceilf
 * @sa SDL_floorf
 * @sa SDL_roundf
 * @sa SDL_lroundf
 **/
inline float fmodf(float x, float y) { return SDL_fmodf(x, y); }

/**
 * Return whether the value is infinity.
 *
 * @param x double-precision floating point value.
 * @returns non-zero if the value is infinity, 0 otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_isinff
 **/
inline int isinf(double x) { return SDL_isinf(x); }

/**
 * Return whether the value is infinity.
 *
 * @param x floating point value.
 * @returns non-zero if the value is infinity, 0 otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_isinf
 **/
inline int isinff(float x) { return SDL_isinff(x); }

/**
 * Return whether the value is NaN.
 *
 * @param x double-precision floating point value.
 * @returns non-zero if the value is NaN, 0 otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_isnanf
 **/
inline int isnan(double x) { return SDL_isnan(x); }

/**
 * Return whether the value is NaN.
 *
 * @param x floating point value.
 * @returns non-zero if the value is NaN, 0 otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_isnan
 **/
inline int isnanf(float x) { return SDL_isnanf(x); }

/**
 * Compute the natural logarithm of `x`.
 *
 * Domain: `0 < x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * It is an error for `x` to be less than or equal to 0.
 *
 * This function operates on double-precision floating point values, use
 * SDL_logf for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value. Must be greater than 0.
 * @returns the natural logarithm of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_logf
 * @sa SDL_log10
 * @sa SDL_exp
 **/
inline double log(double x) { return SDL_log(x); }

/**
 * Compute the natural logarithm of `x`.
 *
 * Domain: `0 < x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * It is an error for `x` to be less than or equal to 0.
 *
 * This function operates on single-precision floating point values, use
 * SDL_log for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value. Must be greater than 0.
 * @returns the natural logarithm of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_log
 * @sa SDL_expf
 **/
inline float logf(float x) { return SDL_logf(x); }

/**
 * Compute the base-10 logarithm of `x`.
 *
 * Domain: `0 < x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * It is an error for `x` to be less than or equal to 0.
 *
 * This function operates on double-precision floating point values, use
 * SDL_log10f for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value. Must be greater than 0.
 * @returns the logarithm of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_log10f
 * @sa SDL_log
 * @sa SDL_pow
 **/
inline double log10(double x) { return SDL_log10(x); }

/**
 * Compute the base-10 logarithm of `x`.
 *
 * Domain: `0 < x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * It is an error for `x` to be less than or equal to 0.
 *
 * This function operates on single-precision floating point values, use
 * SDL_log10 for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value. Must be greater than 0.
 * @returns the logarithm of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_log10
 * @sa SDL_logf
 * @sa SDL_powf
 **/
inline float log10f(float x) { return SDL_log10f(x); }

/**
 * Split `x` into integer and fractional parts
 *
 * This function operates on double-precision floating point values, use
 * SDL_modff for single-precision floats.
 *
 * @param x floating point value.
 * @param y output pointer to store the integer part of `x`.
 * @returns the fractional part of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_modff
 * @sa SDL_trunc
 * @sa SDL_fmod
 **/
inline double modf(double x, double* y) { return SDL_modf(x, y); }

/**
 * Split `x` into integer and fractional parts
 *
 * This function operates on single-precision floating point values, use
 * SDL_modf for double-precision floats.
 *
 * @param x floating point value.
 * @param y output pointer to store the integer part of `x`.
 * @returns the fractional part of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_modf
 * @sa SDL_truncf
 * @sa SDL_fmodf
 **/
inline float modff(float x, float* y) { return SDL_modff(x, y); }

/**
 * Raise `x` to the power `y`
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`
 *
 * Range: `-INF <= z <= INF`
 *
 * If `y` is the base of the natural logarithm (e), consider using SDL_exp
 * instead.
 *
 * This function operates on double-precision floating point values, use
 * SDL_powf for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x the base.
 * @param y the exponent.
 * @returns `x` raised to the power `y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_powf
 * @sa SDL_exp
 * @sa SDL_log
 **/
inline double pow(double x, double y) { return SDL_pow(x, y); }

/**
 * Raise `x` to the power `y`
 *
 * Domain: `-INF <= x <= INF`, `-INF <= y <= INF`
 *
 * Range: `-INF <= z <= INF`
 *
 * If `y` is the base of the natural logarithm (e), consider using SDL_exp
 * instead.
 *
 * This function operates on single-precision floating point values, use
 * SDL_powf for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x the base.
 * @param y the exponent.
 * @returns `x` raised to the power `y`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_pow
 * @sa SDL_expf
 * @sa SDL_logf
 **/
inline float powf(float x, float y) { return SDL_powf(x, y); }

/**
 * Round `x` to the nearest integer.
 *
 * Rounds `x` to the nearest integer. Values halfway between integers will be
 * rounded away from zero.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on double-precision floating point values, use
 * SDL_roundf for single-precision floats. To get the result as an integer
 * type, use SDL_lround.
 *
 * @param x floating point value.
 * @returns the nearest integer to `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_roundf
 * @sa SDL_lround
 * @sa SDL_floor
 * @sa SDL_ceil
 * @sa SDL_trunc
 **/
inline double round(double x) { return SDL_round(x); }

/**
 * Round `x` to the nearest integer.
 *
 * Rounds `x` to the nearest integer. Values halfway between integers will be
 * rounded away from zero.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`, y integer
 *
 * This function operates on double-precision floating point values, use
 * SDL_roundf for single-precision floats. To get the result as an integer
 * type, use SDL_lroundf.
 *
 * @param x floating point value.
 * @returns the nearest integer to `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_round
 * @sa SDL_lroundf
 * @sa SDL_floorf
 * @sa SDL_ceilf
 * @sa SDL_truncf
 **/
inline float roundf(float x) { return SDL_roundf(x); }

/**
 * Round `x` to the nearest integer representable as a long
 *
 * Rounds `x` to the nearest integer. Values halfway between integers will be
 * rounded away from zero.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `MIN_LONG <= y <= MAX_LONG`
 *
 * This function operates on double-precision floating point values, use
 * SDL_lround for single-precision floats. To get the result as a
 * floating-point type, use SDL_round.
 *
 * @param x floating point value.
 * @returns the nearest integer to `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_lroundf
 * @sa SDL_round
 * @sa SDL_floor
 * @sa SDL_ceil
 * @sa SDL_trunc
 **/
inline long lround(double x) { return SDL_lround(x); }

/**
 * Round `x` to the nearest integer representable as a long
 *
 * Rounds `x` to the nearest integer. Values halfway between integers will be
 * rounded away from zero.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `MIN_LONG <= y <= MAX_LONG`
 *
 * This function operates on single-precision floating point values, use
 * SDL_lroundf for double-precision floats. To get the result as a
 * floating-point type, use SDL_roundf,
 *
 * @param x floating point value.
 * @returns the nearest integer to `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_lround
 * @sa SDL_roundf
 * @sa SDL_floorf
 * @sa SDL_ceilf
 * @sa SDL_truncf
 **/
inline long lroundf(float x) { return SDL_lroundf(x); }

/**
 * Scale `x` by an integer power of two.
 *
 * Multiplies `x` by the `n`th power of the floating point radix (always 2).
 *
 * Domain: `-INF <= x <= INF`, `n` integer
 *
 * Range: `-INF <= y <= INF`
 *
 * This function operates on double-precision floating point values, use
 * SDL_scalbnf for single-precision floats.
 *
 * @param x floating point value to be scaled.
 * @param n integer exponent.
 * @returns `x * 2^n`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_scalbnf
 * @sa SDL_pow
 **/
inline double scalbn(double x, int n) { return SDL_scalbn(x, n); }

/**
 * Scale `x` by an integer power of two.
 *
 * Multiplies `x` by the `n`th power of the floating point radix (always 2).
 *
 * Domain: `-INF <= x <= INF`, `n` integer
 *
 * Range: `-INF <= y <= INF`
 *
 * This function operates on single-precision floating point values, use
 * SDL_scalbn for double-precision floats.
 *
 * @param x floating point value to be scaled.
 * @param n integer exponent.
 * @returns `x * 2^n`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_scalbn
 * @sa SDL_powf
 **/
inline float scalbnf(float x, int n) { return SDL_scalbnf(x, n); }

/**
 * Compute the sine of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-1 <= y <= 1`
 *
 * This function operates on double-precision floating point values, use
 * SDL_sinf for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value, in radians.
 * @returns sine of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_sinf
 * @sa SDL_asin
 * @sa SDL_cos
 **/
inline double sin(double x) { return SDL_sin(x); }

/**
 * Compute the sine of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-1 <= y <= 1`
 *
 * This function operates on single-precision floating point values, use
 * SDL_sin for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value, in radians.
 * @returns sine of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_sin
 * @sa SDL_asinf
 * @sa SDL_cosf
 **/
inline float sinf(float x) { return SDL_sinf(x); }

/**
 * Compute the square root of `x`.
 *
 * Domain: `0 <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * This function operates on double-precision floating point values, use
 * SDL_sqrtf for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value. Must be greater than or equal to 0.
 * @returns square root of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_sqrtf
 **/
inline double sqrt(double x) { return SDL_sqrt(x); }

/**
 * Compute the square root of `x`.
 *
 * Domain: `0 <= x <= INF`
 *
 * Range: `0 <= y <= INF`
 *
 * This function operates on single-precision floating point values, use
 * SDL_sqrt for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value. Must be greater than or equal to 0.
 * @returns square root of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_sqrt
 **/
inline float sqrtf(float x) { return SDL_sqrtf(x); }

/**
 * Compute the tangent of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * This function operates on double-precision floating point values, use
 * SDL_tanf for single-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value, in radians.
 * @returns tangent of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_tanf
 * @sa SDL_sin
 * @sa SDL_cos
 * @sa SDL_atan
 * @sa SDL_atan2
 **/
inline double tan(double x) { return SDL_tan(x); }

/**
 * Compute the tangent of `x`.
 *
 * Domain: `-INF <= x <= INF`
 *
 * Range: `-INF <= y <= INF`
 *
 * This function operates on single-precision floating point values, use
 * SDL_tanf for double-precision floats.
 *
 * This function may use a different approximation across different versions,
 * platforms and configurations. i.e, it can return a different value given
 * the same input on different machines or operating systems, or if SDL is
 * updated.
 *
 * @param x floating point value, in radians.
 * @returns tangent of `x`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_tan
 * @sa SDL_sinf
 * @sa SDL_cosf
 * @sa SDL_atanf
 * @sa SDL_atan2f
 **/
inline float tanf(float x) { return SDL_tanf(x); }

/**
 * An opaque handle representing string encoding conversion state.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa IConv
 * @sa IConvRef
 */
template<ObjectBox<SDL_iconv_data_t*> T>
struct IConvBase : T
{
  using T::T;

  /**
   * This function allocates a context for the specified character set
   * conversion.
   *
   * @param tocode The target character encoding, must not be NULL.
   * @param fromcode The source character encoding, must not be NULL.
   * @post this becomes a valid handle convertible to true on success, or
   *          convertible to false on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa iconv()
   * @sa iconv_close()
   * @sa iconv_string()
   **/
  IConvBase(StringParam tocode, StringParam fromcode)
    : T(SDL_iconv_open(tocode, fromcode))
  {
  }

  /**
   * This function converts text between encodings, reading from and writing to
   * a buffer.
   *
   * It returns the number of successful conversions on success. On error,
   * SDL_ICONV_E2BIG is returned when the output buffer is too small, or
   * SDL_ICONV_EILSEQ is returned when an invalid input sequence is encountered,
   * or SDL_ICONV_EINVAL is returned when an incomplete input sequence is
   * encountered.
   *
   * On exit:
   *
   * - inbuf will point to the beginning of the next multibyte sequence. On
   *   error, this is the location of the problematic input sequence. On
   *   success, this is the end of the input sequence.
   * - inbytesleft will be set to the number of bytes left to convert, which
   *   will be 0 on success.
   * - outbuf will point to the location where to store the next output byte.
   * - outbytesleft will be set to the number of bytes left in the output
   *   buffer.
   *
   * @param inbuf Address of variable that points to the first character of the
   *              input sequence.
   * @param inbytesleft The number of bytes in the input buffer.
   * @param outbuf Address of variable that points to the output buffer.
   * @param outbytesleft The number of bytes in the output buffer.
   * @returns the number of conversions on success, or a negative error code.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa iconv_string()
   */
  size_t iconv(const char** inbuf,
               size_t* inbytesleft,
               char** outbuf,
               size_t* outbytesleft)
  {
    return SDL_iconv(T::get(), inbuf, inbytesleft, outbuf, outbytesleft);
  }

  /**
   * This function frees a context used for character set conversion.
   *
   * This object becomes empty after the call.
   * @returns 0 on success, or -1 on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_iconv
   * @sa SDL_iconv_open
   * @sa SDL_iconv_string
   */
  int close() { return SDL_iconv_close(T::release()); }
};

/**
 * Callback for iConv resource cleanup
 *
 * @private
 */
template<>
inline void ObjectRef<SDL_iconv_data_t>::doFree(SDL_iconv_data_t* resource)
{
  SDL_iconv_close(resource);
}

#ifdef SDL3PP_DOC

/**
 * Generic error. Check SDL_GetError()?
 */
#define SDL_ICONV_ERROR (size_t)-1

/**
 * Output buffer was too small.
 */
#define SDL_ICONV_E2BIG (size_t)-2

/**
 * Invalid input sequence was encountered.
 */
#define SDL_ICONV_EILSEQ (size_t)-3

/**
 * Incomplete input sequence was encountered.
 */
#define SDL_ICONV_EINVAL (size_t)-4

#endif // SDL3PP_DOC

/**
 * Helper function to convert a string's encoding in one call.
 *
 * This function converts a buffer or string between encodings in one pass.
 *
 * The string does not need to be NULL-terminated; this function operates on
 * the number of bytes specified in `inbytesleft` whether there is a NULL
 * character anywhere in the buffer.
 *
 * The returned string is owned by the caller, and should be passed to
 * SDL_free when no longer needed.
 *
 * @param tocode the character encoding of the output string. Examples are
 *               "UTF-8", "UCS-4", etc.
 * @param fromcode the character encoding of data in `inbuf`.
 * @param inbuf the string to convert to a different encoding.
 * @param inbytesleft the size of the input string _in bytes_.
 * @returns a new string, converted to the new encoding, or NULL on error.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_iconv_open
 * @sa SDL_iconv_close
 * @sa SDL_iconv
 **/
inline OwnPtr<char> iconv_string(StringParam tocode,
                                 StringParam fromcode,
                                 StringParam inbuf,
                                 size_t inbytesleft)
{
  return OwnPtr<char>{SDL_iconv_string(tocode, fromcode, inbuf, inbytesleft)};
}

#ifdef SDL3PP_DOC

/**
 * Convert a UTF-8 string to the current locale's character encoding.
 *
 * This is a helper macro that might be more clear than calling
 * SDL_iconv_string directly. However, it double-evaluates its parameter, so
 * do not use an expression with side-effects here.
 *
 * @param S the string to convert.
 * @returns a new string, converted to the new encoding, or NULL on error.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_iconv_utf8_locale(S)                                               \
  SDL_iconv_string("", "UTF-8", S, SDL_strlen(S) + 1)

/**
 * Convert a UTF-8 string to UCS-2.
 *
 * This is a helper macro that might be more clear than calling
 * SDL_iconv_string directly. However, it double-evaluates its parameter, so
 * do not use an expression with side-effects here.
 *
 * @param S the string to convert.
 * @returns a new string, converted to the new encoding, or NULL on error.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_iconv_utf8_ucs2(S)                                                 \
  (Uint16*)SDL_iconv_string("UCS-2", "UTF-8", S, SDL_strlen(S) + 1)

/**
 * Convert a UTF-8 string to UCS-4.
 *
 * This is a helper macro that might be more clear than calling
 * SDL_iconv_string directly. However, it double-evaluates its parameter, so
 * do not use an expression with side-effects here.
 *
 * @param S the string to convert.
 * @returns a new string, converted to the new encoding, or NULL on error.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_iconv_utf8_ucs4(S)                                                 \
  (Uint32*)SDL_iconv_string("UCS-4", "UTF-8", S, SDL_strlen(S) + 1)

/**
 * Convert a wchar_t string to UTF-8.
 *
 * This is a helper macro that might be more clear than calling
 * SDL_iconv_string directly. However, it double-evaluates its parameter, so
 * do not use an expression with side-effects here.
 *
 * @param S the string to convert.
 * @returns a new string, converted to the new encoding, or NULL on error.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_iconv_wchar_utf8(S)                                                \
  SDL_iconv_string(                                                            \
    "UTF-8", "WCHAR_T", (char*)S, (SDL_wcslen(S) + 1) * sizeof(wchar_t))

#endif // SDL3PP_DOC

/**
 * Multiply two integers, checking for overflow.
 *
 * If `a * b` would overflow, return false.
 *
 * Otherwise store `a * b` via ret and return true.
 *
 * @param a the multiplicand.
 * @param b the multiplier.
 * @param ret on non-overflow output, stores the multiplication result, may
 *            not be NULL.
 * @returns false on overflow, true if result is multiplied without overflow.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline bool size_mul_check_overflow(size_t a, size_t b, size_t* ret)
{
  return SDL_size_mul_check_overflow(a, b, ret);
}

/**
 * Add two integers, checking for overflow.
 *
 * If `a + b` would overflow, return false.
 *
 * Otherwise store `a + b` via ret and return true.
 *
 * @param a the first addend.
 * @param b the second addend.
 * @param ret on non-overflow output, stores the addition result, may not be
 *            NULL.
 * @returns false on overflow, true if result is added without overflow.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 **/
inline bool size_add_check_overflow(size_t a, size_t b, size_t* ret)
{
  return SDL_size_add_check_overflow(a, b, ret);
}

/**
 * A generic function pointer.
 *
 * In theory, generic function pointers should use this, instead of `void *`,
 * since some platforms could treat code addresses differently than data
 * addresses. Although in current times no popular platforms make this
 * distinction, it is more correct and portable to use the correct type for a
 * generic pointer.
 *
 * If for some reason you need to force this typedef to be an actual `void *`,
 * perhaps to work around a compiler or existing code, you can define
 * `SDL_FUNCTION_POINTER_IS_VOID_POINTER` before including any SDL headers.
 *
 * @since This datatype is available since SDL 3.2.0.
 **/
using FunctionPointer = SDL_FunctionPointer;

#pragma region impl
/// @}

template<class T>
void PtrBase<T>::free()
{
  SDL::free(T::release());
}

#pragma endregion impl

} // namespace SDL

#endif /* SDL3PP_STDINC_H_ */
