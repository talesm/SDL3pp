/**
 * @file stdinc.hpp
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
 **/
#ifndef SDL3PP_STDINC_HPP_
#define SDL3PP_STDINC_HPP_

#include <SDL3/SDL_stdinc.h>
#include "stringParam.hpp"

namespace SDL {
// Content here
} // namespace SDL

#endif /* SDL3PP_STDINC_HPP_ */
