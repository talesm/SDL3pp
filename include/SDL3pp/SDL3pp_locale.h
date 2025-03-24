#ifndef SDL3PP_LOCALE_H_
#define SDL3PP_LOCALE_H_

#include <SDL3/SDL_locale.h>
#include "SDL3pp_stdinc.h"

namespace SDL {
/**
 * @defgroup CategoryLocale Locale Info
 *
 * SDL locale services.
 *
 * This provides a way to get a list of preferred locales (language plus
 * country) for the user. There is exactly one function:
 * GetPreferredLocales(), which handles all the heavy lifting, and offers
 * documentation on all the strange ways humans might have configured their
 * language settings.
 *
 * @{
 */

/**
 * A struct to provide locale data.
 *
 * Locale data is split into a spoken language, like English, and an optional
 * country, like Canada. The language will be in ISO-639 format (so English
 * would be "en"), and the country, if not NULL, will be an ISO-3166 country
 * code (so Canada would be "CA").
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetPreferredLocales()
 */
using Locale = SDL_Locale;

/**
 * Report the user's preferred locale.
 *
 * Returned language strings are in the format xx, where 'xx' is an ISO-639
 * language specifier (such as "en" for English, "de" for German, etc).
 * Country strings are in the format YY, where "YY" is an ISO-3166 country
 * code (such as "US" for the United States, "CA" for Canada, etc). Country
 * might be NULL if there's no specific guidance on them (so you might get {
 * "en", "US" } for American English, but { "en", NULL } means "English
 * language, generically"). Language strings are never NULL, except to
 * terminate the array.
 *
 * Please note that not all of these strings are 2 characters; some are three
 * or more.
 *
 * The returned list of locales are in the order of the user's preference. For
 * example, a German citizen that is fluent in US English and knows enough
 * Japanese to navigate around Tokyo might have a list like: { "de", "en_US",
 * "jp", NULL }. Someone from England might prefer British English (where
 * "color" is spelled "colour", etc), but will settle for anything like it: {
 * "en_GB", "en", NULL }.
 *
 * This function returns NULL on error, including when the platform does not
 * supply this information at all.
 *
 * This might be a "slow" call that has to query the operating system. It's
 * best to ask for this once and save the results. However, this list can
 * change, usually because the user has changed a system preference outside of
 * your program; SDL will send an SDL::EVENT_LOCALE_CHANGED event in this case,
 * if possible, and you can call this function again to get an updated copy of
 * preferred locales.
 *
 * @returns a std::nullptr terminated array of locale pointers, or std::nullptr
 * on failure; call GetError() for more information. This is a single allocation
 *          that should be freed with free() when it is no longer needed.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline OwnArray<Locale*> GetPreferredLocales()
{
  int count = 0;
  auto data = SDL_GetPreferredLocales(&count);
  return OwnArray<Locale*>{data, size_t(count)};
}

/// @}

} // namespace SDL

#endif /* SDL3PP_LOCALE_H_ */
