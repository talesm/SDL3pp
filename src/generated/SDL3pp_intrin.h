#ifndef SDL3PP_INTRIN_H_
#define SDL3PP_INTRIN_H_

#include <SDL3/SDL_intrin.h>

namespace SDL {

/**
 * @defgroup CategoryIntrinsics Category Intrinsics
 *
 * SDL does some preprocessor gymnastics to determine if any CPU-specific
 * compiler intrinsics are available, as this is not necessarily an easy thing
 * to calculate, and sometimes depends on quirks of a system, versions of build
 * tools, and other external forces.
 *
 * Apps including SDL's headers will be able to check consistent preprocessor
 * definitions to decide if it's safe to use compiler intrinsics for a specific
 * CPU architecture. This check only tells you that the compiler is capable of
 * using those intrinsics; at runtime, you should still check if they are
 * available on the current system with the [CPU info
 * functions](https://wiki.libsdl.org/SDL3/CategoryCPUInfo) , such as HasSSE()
 * or HasNEON(). Otherwise, the process might crash for using an unsupported CPU
 * instruction.
 *
 * SDL only sets preprocessor defines for CPU intrinsics if they are supported,
 * so apps should check with `#ifdef` and not `#if`.
 *
 * SDL will also include the appropriate instruction-set-specific support
 * headers, so if SDL decides to define SDL_SSE2_INTRINSICS, it will also
 * `#include <emmintrin.h>` as well.
 *
 * @{
 */

/**
 * Defined if (and only if) the compiler supports Loongarch LSX intrinsics.
 *
 * If this macro is defined, SDL will have already included `<lsxintrin.h>`
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_LASX_INTRINSICS
 */
#define SDL_LSX_INTRINSICS 1

/**
 * Defined if (and only if) the compiler supports Loongarch LSX intrinsics.
 *
 * If this macro is defined, SDL will have already included `<lasxintrin.h>`
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_LASX_INTRINSICS
 */
#define SDL_LASX_INTRINSICS 1

/**
 * Defined if (and only if) the compiler supports ARM NEON intrinsics.
 *
 * If this macro is defined, SDL will have already included `<armintr.h>`
 * `<arm_neon.h>`, `<arm64intr.h>`, and `<arm64_neon.h>`, as appropriate.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_NEON_INTRINSICS 1

/**
 * Defined if (and only if) the compiler supports PowerPC Altivec intrinsics.
 *
 * If this macro is defined, SDL will have already included `<altivec.h>`
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_ALTIVEC_INTRINSICS 1

/**
 * Defined if (and only if) the compiler supports Intel MMX intrinsics.
 *
 * If this macro is defined, SDL will have already included `<mmintrin.h>`
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_SSE_INTRINSICS
 */
#define SDL_MMX_INTRINSICS 1

/**
 * Defined if (and only if) the compiler supports Intel SSE intrinsics.
 *
 * If this macro is defined, SDL will have already included `<xmmintrin.h>`
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_SSE2_INTRINSICS
 * @sa SDL_SSE3_INTRINSICS
 * @sa SDL_SSE4_1_INTRINSICS
 * @sa SDL_SSE4_2_INTRINSICS
 */
#define SDL_SSE_INTRINSICS 1

/**
 * Defined if (and only if) the compiler supports Intel SSE2 intrinsics.
 *
 * If this macro is defined, SDL will have already included `<emmintrin.h>`
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_SSE_INTRINSICS
 * @sa SDL_SSE3_INTRINSICS
 * @sa SDL_SSE4_1_INTRINSICS
 * @sa SDL_SSE4_2_INTRINSICS
 */
#define SDL_SSE2_INTRINSICS 1

/**
 * Defined if (and only if) the compiler supports Intel SSE3 intrinsics.
 *
 * If this macro is defined, SDL will have already included `<pmmintrin.h>`
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_SSE_INTRINSICS
 * @sa SDL_SSE2_INTRINSICS
 * @sa SDL_SSE4_1_INTRINSICS
 * @sa SDL_SSE4_2_INTRINSICS
 */
#define SDL_SSE3_INTRINSICS 1

/**
 * Defined if (and only if) the compiler supports Intel SSE4.1 intrinsics.
 *
 * If this macro is defined, SDL will have already included `<smmintrin.h>`
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_SSE_INTRINSICS
 * @sa SDL_SSE2_INTRINSICS
 * @sa SDL_SSE3_INTRINSICS
 * @sa SDL_SSE4_2_INTRINSICS
 */
#define SDL_SSE4_1_INTRINSICS 1

/**
 * Defined if (and only if) the compiler supports Intel SSE4.2 intrinsics.
 *
 * If this macro is defined, SDL will have already included `<nmmintrin.h>`
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_SSE_INTRINSICS
 * @sa SDL_SSE2_INTRINSICS
 * @sa SDL_SSE3_INTRINSICS
 * @sa SDL_SSE4_1_INTRINSICS
 */
#define SDL_SSE4_2_INTRINSICS 1

/**
 * Defined if (and only if) the compiler supports Intel AVX intrinsics.
 *
 * If this macro is defined, SDL will have already included `<immintrin.h>`
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_AVX2_INTRINSICS
 * @sa SDL_AVX512F_INTRINSICS
 */
#define SDL_AVX_INTRINSICS 1

/**
 * Defined if (and only if) the compiler supports Intel AVX2 intrinsics.
 *
 * If this macro is defined, SDL will have already included `<immintrin.h>`
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_AVX_INTRINSICS
 * @sa SDL_AVX512F_INTRINSICS
 */
#define SDL_AVX2_INTRINSICS 1

/**
 * Defined if (and only if) the compiler supports Intel AVX-512F intrinsics.
 *
 * AVX-512F is also sometimes referred to as "AVX-512 Foundation."
 *
 * If this macro is defined, SDL will have already included `<immintrin.h>`
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_AVX_INTRINSICS
 * @sa SDL_AVX2_INTRINSICS
 */
#define SDL_AVX512F_INTRINSICS 1

/**
 * A macro to decide if the compiler supports `__attribute__((target))`.
 *
 * Even though this is defined in SDL's public headers, it is generally not used
 * directly by apps. Apps should probably just use SDL_TARGETING directly,
 * instead.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa SDL_TARGETING
 */
#define SDL_HAS_TARGET_ATTRIBS

/**
 * A macro to tag a function as targeting a specific CPU architecture.
 *
 * This is a hint to the compiler that a function should be built with support
 * for a CPU instruction set that might be different than the rest of the
 * program.
 *
 * The particulars of this are explained in the GCC documentation:
 *
 * https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#index-target-function-attribute
 *
 * An example of using this feature is to turn on SSE2 support for a specific
 * function, even if the rest of the source code is not compiled to use SSE2
 * code:
 *
 * ```c
 * #ifdef SDL_SSE2_INTRINSICS
 * static void SDL_TARGETING("sse2") DoSomethingWithSSE2(char *x) {
 *    ...use SSE2 intrinsic functions, etc...
 * }
 * #endif
 *
 * // later...
 * #ifdef SDL_SSE2_INTRINSICS
 * if (HasSSE2()) {
 *     DoSomethingWithSSE2(str);
 * }
 * #endif
 * ```
 *
 * The application is, on a whole, built without SSE2 instructions, so it will
 * run on Intel machines that don't support SSE2. But then at runtime, it checks
 * if the system supports the instructions, and then calls into a function that
 * uses SSE2 opcodes. The ifdefs make sure that this code isn't used on
 * platforms that don't have SSE2 at all.
 *
 * On compilers without target support, this is defined to nothing.
 *
 * This symbol is used by SDL internally, but apps and other libraries are
 * welcome to use it for their own interfaces as well.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_TARGETING(x) __attribute__((target(x)))

/// @}

} // namespace SDL

#endif /* SDL3PP_INTRIN_H_ */
