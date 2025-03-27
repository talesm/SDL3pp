#ifndef SDL3PP_GUID_H_
#define SDL3PP_GUID_H_

#include <SDL3/SDL_guid.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryGUID GUIDs
 *
 * A GUID is a 128-bit value that represents something that is uniquely
 * identifiable by this value: "globally unique."
 *
 * SDL provides functions to convert a GUID to/from a string.
 * @{
 */

/**
 * An SDL_GUID is a 128-bit identifier for an input device that identifies
 * that device across runs of SDL programs on the same platform.
 *
 * If the device is detached and then re-attached to a different port, or if
 * the base system is rebooted, the device should still report the same GUID.
 *
 * GUIDs are as precise as possible but are not guaranteed to distinguish
 * physically distinct but equivalent devices. For example, two game
 * controllers from the same vendor with the same product ID and revision may
 * have the same GUID.
 *
 * GUIDs may be platform-dependent (i.e., the same device may report different
 * GUIDs on different operating systems).
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat wrap-extending-struct
 *
 * @sa wrap-extending-struct
 */
struct GUID : SDL_GUID
{
  constexpr GUID()
    : SDL_GUID({0})
  {
  }

  /// Constructor from underling type
  constexpr GUID(SDL_GUID guid)
    : SDL_GUID(guid)
  {
  }

  /**
   * Convert a GUID string into a SDL_GUID structure.
   *
   * Performs no error checking. If this function is given a string containing
   * an invalid GUID, the function will silently succeed, but the GUID generated
   * will not be useful.
   *
   * @param pchGUID string containing an ASCII representation of a GUID.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_GUIDToString
   */
  GUID(StringParam pchGUID)
    : SDL_GUID(SDL_StringToGUID(pchGUID))
  {
  }

  /**
   * Get an ASCII string representation for a given SDL_GUID.
   *
   * @returns pszGUID the ASCII string representation for
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa SDL_StringToGUID
   */
  std::string ToString() const
  {
    std::string result(32, ' ');
    SDL_GUIDToString(*this, result.data(), 33);
    return result;
  }
};

/**
 * Get an ASCII string representation for a given SDL_GUID.
 *
 * @param guid the SDL_GUID you wish to convert to string.
 * @param pszGUID buffer in which to write the ASCII string.
 * @param cbGUID the size of pszGUID, should be at least 33 bytes.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_StringToGUID
 */
inline void GUIDToString(SDL_GUID guid, char* pszGUID, int cbGUID)
{
  return SDL_GUIDToString(guid, pszGUID, cbGUID);
}

/**
 * Convert a GUID string into a SDL_GUID structure.
 *
 * Performs no error checking. If this function is given a string containing
 * an invalid GUID, the function will silently succeed, but the GUID generated
 * will not be useful.
 *
 * @param pchGUID string containing an ASCII representation of a GUID.
 * @returns a SDL_GUID structure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SDL_GUIDToString
 */
inline SDL_GUID StringToGUID(StringParam pchGUID)
{
  return SDL_StringToGUID(pchGUID);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_GUID_H_ */
