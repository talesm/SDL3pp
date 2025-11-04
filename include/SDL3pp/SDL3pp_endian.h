#ifndef SDL3PP_ENDIAN_H_
#define SDL3PP_ENDIAN_H_

#include <SDL3/SDL_endian.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryEndian Byte Order and Byte Swapping
 *
 * Functions converting endian-specific values to different byte orders.
 *
 * These functions either unconditionally swap byte order (Swap16,
 * Swap32, Swap64, SwapFloat), or they swap to/from the system's
 * native byte order (Swap16LE, Swap16BE, Swap32LE, Swap32BE,
 * Swap32LE, Swap32BE, SwapFloatLE, SwapFloatBE). In the
 * latter case, the functionality is provided by macros that become no-ops if
 * a swap isn't necessary: on an x86 (littleendian) processor, Swap32LE
 * does nothing, but Swap32BE reverses the bytes of the data. On a PowerPC
 * processor (bigendian), the macros behavior is reversed.
 *
 * The swap routines are inline functions, and attempt to use compiler
 * intrinsics, inline assembly, and other magic to make byteswapping
 * efficient.
 *
 * @{
 */

#ifdef SDL3PP_DOC

/**
 * A value to represent littleendian byteorder.
 *
 * This is used with the preprocessor macro SDL_BYTEORDER, to determine a
 * platform's byte ordering:
 *
 * ```cpp
 * #if SDL_BYTEORDER == SDL_LIL_ENDIAN
 * Log("This system is littleendian.");
 * #endif
 * ```
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_BYTEORDER
 * @sa SDL_BIG_ENDIAN
 */
#define SDL_LIL_ENDIAN 1234

/**
 * A value to represent bigendian byteorder.
 *
 * This is used with the preprocessor macro SDL_BYTEORDER, to determine a
 * platform's byte ordering:
 *
 * ```cpp
 * #if SDL_BYTEORDER == SDL_BIG_ENDIAN
 * Log("This system is bigendian.");
 * #endif
 * ```
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_BYTEORDER
 * @sa SDL_LIL_ENDIAN
 */
#define SDL_BIG_ENDIAN 4321

/**
 * A macro that reports the target system's byte order.
 *
 * This is set to either SDL_LIL_ENDIAN or SDL_BIG_ENDIAN (and maybe other
 * values in the future, if something else becomes popular). This can be
 * tested with the preprocessor, so decisions can be made at compile time.
 *
 * ```c
 * #if SDL_BYTEORDER == SDL_BIG_ENDIAN
 * Log("This system is bigendian.");
 * #endif
 * ```
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_LIL_ENDIAN
 * @sa SDL_BIG_ENDIAN
 */
#define SDL_BYTEORDER SDL_LIL_ENDIAN___or_maybe___SDL_BIG_ENDIAN

/**
 * A macro that reports the target system's floating point word order.
 *
 * This is set to either SDL_LIL_ENDIAN or SDL_BIG_ENDIAN (and maybe other
 * values in the future, if something else becomes popular). This can be
 * tested with the preprocessor, so decisions can be made at compile time.
 *
 * ```c
 * #if SDL_FLOATWORDORDER == SDL_BIG_ENDIAN
 * Log("This system's floats are bigendian.");
 * #endif
 * ```
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_LIL_ENDIAN
 * @sa SDL_BIG_ENDIAN
 */
#define SDL_FLOATWORDORDER SDL_LIL_ENDIAN___or_maybe___SDL_BIG_ENDIAN

#endif // SDL3PP_DOC

/**
 * Byte-swap an unsigned 16-bit number.
 *
 * This will always byte-swap the value, whether it's currently in the native
 * byteorder of the system or not. You should use Swap16LE or Swap16BE
 * instead, in most cases.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param x the value to byte-swap.
 * @returns `x`, with its bytes in the opposite endian order.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint16 Swap16(Uint16 x) { return SDL_Swap16(x); }

/**
 * Byte-swap an unsigned 32-bit number.
 *
 * This will always byte-swap the value, whether it's currently in the native
 * byteorder of the system or not. You should use Swap32LE or Swap32BE
 * instead, in most cases.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param x the value to byte-swap.
 * @returns `x`, with its bytes in the opposite endian order.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint32 Swap32(Uint32 x) { return SDL_Swap32(x); }

/**
 * Byte-swap an unsigned 64-bit number.
 *
 * This will always byte-swap the value, whether it's currently in the native
 * byteorder of the system or not. You should use Swap64LE or Swap64BE
 * instead, in most cases.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param x the value to byte-swap.
 * @returns `x`, with its bytes in the opposite endian order.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint64 Swap64(Uint64 x) { return SDL_Swap64(x); }

/**
 * Byte-swap a floating point number.
 *
 * This will always byte-swap the value, whether it's currently in the native
 * byteorder of the system or not. You should use SwapFloatLE or
 * SwapFloatBE instead, in most cases.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param x the value to byte-swap.
 * @returns x, with its bytes in the opposite endian order.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr float SwapFloat(float x) { return SDL_SwapFloat(x); }

/**
 * Swap a 16-bit value from littleendian to native byte order.
 *
 * If this is running on a littleendian system, `x` is returned unchanged.
<<<<<<<
=======
 * byteorder of the system or not. You should use Swap16LE or Swap16BE instead,
 * in most cases.
>>>>>>>
 *
 * This macro never references `x` more than once, avoiding side effects.
 *
 * @param x the value to swap, in littleendian byte order.
 * @returns `x` in native byte order.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr Uint16 Swap16LE(Uint16 x) { return SDL_Swap16LE(x); }

/**
 * Swap a 32-bit value from littleendian to native byte order.
 *
<<<<<<<
=======
constexpr Uint16 Swap16(Uint16 x) { return SDL_Swap16(x); }
>>>>>>>
 * If this is running on a littleendian system, `x` is returned unchanged.
 *
 * This macro never references `x` more than once, avoiding side effects.
 *
 * @param x the value to swap, in littleendian byte order.
 * @returns `x` in native byte order.
<<<<<<<
=======
 * byteorder of the system or not. You should use Swap32LE or Swap32BE instead,
 * in most cases.
>>>>>>>
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr Uint32 Swap32LE(Uint32 x) { return SDL_Swap32LE(x); }

/**
 * Swap a 64-bit value from littleendian to native byte order.
 *
 * If this is running on a littleendian system, `x` is returned unchanged.
 *
 * This macro never references `x` more than once, avoiding side effects.
 *
 * @param x the value to swap, in littleendian byte order.
<<<<<<<
=======
constexpr Uint32 Swap32(Uint32 x) { return SDL_Swap32(x); }
>>>>>>>
 * @returns `x` in native byte order.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
<<<<<<<
=======
 * byteorder of the system or not. You should use Swap64LE or Swap64BE instead,
 * in most cases.
>>>>>>>
constexpr Uint64 Swap64LE(Uint64 x) { return SDL_Swap64LE(x); }

/**
 * Swap a floating point value from littleendian to native byte order.
 *
 * If this is running on a littleendian system, `x` is returned unchanged.
 *
 * This macro never references `x` more than once, avoiding side effects.
 *
 * @param x the value to swap, in littleendian byte order.
 * @returns `x` in native byte order.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
<<<<<<<
=======
constexpr Uint64 Swap64(Uint64 x) { return SDL_Swap64(x); }
>>>>>>>
 */
constexpr float SwapFloatLE(float x) { return SDL_SwapFloatLE(x); }

/**
 * Swap a 16-bit value from bigendian to native byte order.
 *
 * If this is running on a bigendian system, `x` is returned unchanged.
 *
 * This macro never references `x` more than once, avoiding side effects.
 *
 * @param x the value to swap, in bigendian byte order.
 * @returns `x` in native byte order.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr Uint16 Swap16BE(Uint16 x) { return SDL_Swap16BE(x); }

/**
 * Swap a 32-bit value from bigendian to native byte order.
 *
 * If this is running on a bigendian system, `x` is returned unchanged.
 *
 * This macro never references `x` more than once, avoiding side effects.
 *
 * @param x the value to swap, in bigendian byte order.
 * @returns `x` in native byte order.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr Uint32 Swap32BE(Uint32 x) { return SDL_Swap32BE(x); }

/**
 * Swap a 64-bit value from bigendian to native byte order.
 *
 * If this is running on a bigendian system, `x` is returned unchanged.
 *
 * This macro never references `x` more than once, avoiding side effects.
 *
 * @param x the value to swap, in bigendian byte order.
 * @returns `x` in native byte order.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr Uint64 Swap64BE(Uint64 x) { return SDL_Swap64BE(x); }

/**
 * Swap a floating point value from bigendian to native byte order.
 *
 * If this is running on a bigendian system, `x` is returned unchanged.
 *
 * This macro never references `x` more than once, avoiding side effects.
 *
 * @param x the value to swap, in bigendian byte order.
 * @returns `x` in native byte order.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr float SwapFloatBE(float x) { return SDL_SwapFloatBE(x); }

/// @}

} // namespace SDL

#endif /* SDL3PP_ENDIAN_H_ */
