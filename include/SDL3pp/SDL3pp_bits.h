#ifndef SDL3PP_BITS_H_
#define SDL3PP_BITS_H_

#include <SDL3/SDL_bits.h>

namespace SDL {

/**
 * @defgroup CategoryBits Bit Manipulation
 *
 * Functions for fiddling with bits and bitmasks.
 *
 * @{
 */

/**
 * Get the index of the most significant (set) bit in a 32-bit number.
 *
 * Result is undefined when called with 0. This operation can also be stated
 * as "count leading zeroes" and "log base 2".
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param x the 32-bit value to examine.
 * @returns the index of the most significant bit, or -1 if the value is 0.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr int MostSignificantBitIndex32(Uint32 x)
{
  return SDL_MostSignificantBitIndex32(x);
}

/**
 * Determine if a unsigned 32-bit value has exactly one bit set.
 *
 * If there are no bits set (`x` is zero), or more than one bit set, this
 * returns false. If any one bit is exclusively set, this returns true.
 *
 * Note that this is a forced-inline function in a header, and not a public
 * API function available in the SDL library (which is to say, the code is
 * embedded in the calling program and the linker and dynamic loader will not
 * be able to find this function inside SDL itself).
 *
 * @param x the 32-bit value to examine.
 * @returns true if exactly one bit is set in `x`, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool HasExactlyOneBitSet32(Uint32 x)
{
  return SDL_HasExactlyOneBitSet32(x);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_BITS_H_ */
