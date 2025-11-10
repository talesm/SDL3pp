#ifndef SDL3PP_CPUINFO_H_
#define SDL3PP_CPUINFO_H_

#include <SDL3/SDL_cpuinfo.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryCPUInfo CPU Feature Detection
 *
 * CPU feature detection for SDL.
 *
 * These functions are largely concerned with reporting if the system has access
 * to various SIMD instruction sets, but also has other important info to share,
 * such as system RAM size and number of logical CPU cores.
 *
 * CPU instruction set checks, like HasSSE() and HasNEON(), are available on all
 * platforms, even if they don't make sense (an ARM processor will never have
 * SSE and an x86 processor will never have NEON, for example, but these
 * functions still exist and will simply return false in these cases).
 *
 * @{
 */

/**
 * A guess for the cacheline size used for padding.
 *
 * Most x86 processors have a 64 byte cache line. The 64-bit PowerPC processors
 * have a 128 byte cache line. We use the larger value to be generally safe.
 *
 * @since This constant is available since SDL 3.2.0.
 */
constexpr int CACHELINE_SIZE = SDL_CACHELINE_SIZE;

/**
 * Get the number of logical CPU cores available.
 *
 * @returns the total number of logical CPU cores. On CPUs that include
 *          technologies such as hyperthreading, the number of logical cores may
 *          be more than the number of physical cores.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int GetNumLogicalCPUCores() { return SDL_GetNumLogicalCPUCores(); }

/**
 * Determine the L1 cache line size of the CPU.
 *
 * This is useful for determining multi-threaded structure padding or SIMD
 * prefetch sizes.
 *
 * @returns the L1 cache line size of the CPU, in bytes.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int GetCPUCacheLineSize() { return SDL_GetCPUCacheLineSize(); }

/**
 * Determine whether the CPU has AltiVec features.
 *
 * This always returns false on CPUs that aren't using PowerPC instruction sets.
 *
 * @returns true if the CPU has AltiVec features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool HasAltiVec() { return SDL_HasAltiVec(); }

/**
 * Determine whether the CPU has MMX features.
 *
 * This always returns false on CPUs that aren't using Intel instruction sets.
 *
 * @returns true if the CPU has MMX features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool HasMMX() { return SDL_HasMMX(); }

/**
 * Determine whether the CPU has SSE features.
 *
 * This always returns false on CPUs that aren't using Intel instruction sets.
 *
 * @returns true if the CPU has SSE features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasSSE2
 * @sa HasSSE3
 * @sa HasSSE41
 * @sa HasSSE42
 */
inline bool HasSSE() { return SDL_HasSSE(); }

/**
 * Determine whether the CPU has SSE2 features.
 *
 * This always returns false on CPUs that aren't using Intel instruction sets.
 *
 * @returns true if the CPU has SSE2 features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasSSE
 * @sa HasSSE3
 * @sa HasSSE41
 * @sa HasSSE42
 */
inline bool HasSSE2() { return SDL_HasSSE2(); }

/**
 * Determine whether the CPU has SSE3 features.
 *
 * This always returns false on CPUs that aren't using Intel instruction sets.
 *
 * @returns true if the CPU has SSE3 features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasSSE
 * @sa HasSSE2
 * @sa HasSSE41
 * @sa HasSSE42
 */
inline bool HasSSE3() { return SDL_HasSSE3(); }

/**
 * Determine whether the CPU has SSE4.1 features.
 *
 * This always returns false on CPUs that aren't using Intel instruction sets.
 *
 * @returns true if the CPU has SSE4.1 features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasSSE
 * @sa HasSSE2
 * @sa HasSSE3
 * @sa HasSSE42
 */
inline bool HasSSE41() { return SDL_HasSSE41(); }

/**
 * Determine whether the CPU has SSE4.2 features.
 *
 * This always returns false on CPUs that aren't using Intel instruction sets.
 *
 * @returns true if the CPU has SSE4.2 features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasSSE
 * @sa HasSSE2
 * @sa HasSSE3
 * @sa HasSSE41
 */
inline bool HasSSE42() { return SDL_HasSSE42(); }

/**
 * Determine whether the CPU has AVX features.
 *
 * This always returns false on CPUs that aren't using Intel instruction sets.
 *
 * @returns true if the CPU has AVX features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasAVX2
 * @sa HasAVX512F
 */
inline bool HasAVX() { return SDL_HasAVX(); }

/**
 * Determine whether the CPU has AVX2 features.
 *
 * This always returns false on CPUs that aren't using Intel instruction sets.
 *
 * @returns true if the CPU has AVX2 features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasAVX
 * @sa HasAVX512F
 */
inline bool HasAVX2() { return SDL_HasAVX2(); }

/**
 * Determine whether the CPU has AVX-512F (foundation) features.
 *
 * This always returns false on CPUs that aren't using Intel instruction sets.
 *
 * @returns true if the CPU has AVX-512F features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasAVX
 * @sa HasAVX2
 */
inline bool HasAVX512F() { return SDL_HasAVX512F(); }

/**
 * Determine whether the CPU has ARM SIMD (ARMv6) features.
 *
 * This is different from ARM NEON, which is a different instruction set.
 *
 * This always returns false on CPUs that aren't using ARM instruction sets.
 *
 * @returns true if the CPU has ARM SIMD features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasNEON
 */
inline bool HasARMSIMD() { return SDL_HasARMSIMD(); }

/**
 * Determine whether the CPU has NEON (ARM SIMD) features.
 *
 * This always returns false on CPUs that aren't using ARM instruction sets.
 *
 * @returns true if the CPU has ARM NEON features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool HasNEON() { return SDL_HasNEON(); }

/**
 * Determine whether the CPU has LSX (LOONGARCH SIMD) features.
 *
 * This always returns false on CPUs that aren't using LOONGARCH instruction
 * sets.
 *
 * @returns true if the CPU has LOONGARCH LSX features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool HasLSX() { return SDL_HasLSX(); }

/**
 * Determine whether the CPU has LASX (LOONGARCH SIMD) features.
 *
 * This always returns false on CPUs that aren't using LOONGARCH instruction
 * sets.
 *
 * @returns true if the CPU has LOONGARCH LASX features or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool HasLASX() { return SDL_HasLASX(); }

/**
 * Get the amount of RAM configured in the system.
 *
 * @returns the amount of RAM configured in the system in MiB.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int GetSystemRAM() { return SDL_GetSystemRAM(); }

/**
 * Report the alignment this system needs for SIMD allocations.
 *
 * This will return the minimum number of bytes to which a pointer must be
 * aligned to be compatible with SIMD instructions on the current machine. For
 * example, if the machine supports SSE only, it will return 16, but if it
 * supports AVX-512F, it'll return 64 (etc). This only reports values for
 * instruction sets SDL knows about, so if your SDL build doesn't have
 * HasAVX512F(), then it might return 16 for the SSE support it sees and not 64
 * for the AVX-512 instructions that exist but SDL doesn't know about. Plan
 * accordingly.
 *
 * @returns the alignment in bytes needed for available, known SIMD
 *          instructions.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa aligned_alloc
 * @sa aligned_free
 */
inline size_t GetSIMDAlignment() { return SDL_GetSIMDAlignment(); }

/// @}

} // namespace SDL

#endif /* SDL3PP_CPUINFO_H_ */
