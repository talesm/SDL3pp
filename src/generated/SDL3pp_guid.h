#ifndef SDL3PP_GUID_H_
#define SDL3PP_GUID_H_

#include <SDL3/SDL_guid.h>
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryGUID Category GUID
 *
 * A GUID is a 128-bit value that represents something that is uniquely
 * identifiable by this value: "globally unique."
 *
 * SDL provides functions to convert a GUID to/from a string.
 *
 * @{
 */

/// Alias to raw representation for GUID.
using GUIDRaw = SDL_GUID;

/**
 * An GUID is a 128-bit identifier for an input device that identifies
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
 */
struct GUID : GUIDRaw
{
  /**
   * Wraps GUID.
   *
   * @param gUID the value to be wrapped
   */
  constexpr GUID(const GUIDRaw& gUID = {})
    : GUIDRaw(gUID)
  {
  }

  /**
   * Convert a GUID string into a GUID structure.
   *
   * Performs no error checking. If this function is given a string containing
   * an invalid GUID, the function will silently succeed, but the GUID generated
   * will not be useful.
   *
   * @param pchGUID string containing an ASCII representation of a GUID.
   * @post a GUID structure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GUID.ToString
   */
  GUID(StringParam pchGUID)
    : GUIDRaw(SDL_StringToGUID(pchGUID))
  {
  }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return *this != GUIDRaw{}; }

  /**
   * Get an ASCII string representation for a given GUID.
   *
   * @param guid the GUID you wish to convert to string.
   * @param pszGUID buffer in which to write the ASCII string.
   * @param cbGUID the size of pszGUID, should be at least 33 bytes.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GUID.GUID
   */
  std::string ToString() const;
};

/**
 * Get an ASCII string representation for a given GUID.
 *
 * @param guid the GUID you wish to convert to string.
 * @param pszGUID buffer in which to write the ASCII string.
 * @param cbGUID the size of pszGUID, should be at least 33 bytes.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GUID.GUID
 */
inline std::string GUIDToString(const GUIDRaw& guid)
{
  return SDL_GUIDToString(guid);
}

inline std::string GUID::ToString() const { return SDL::GUIDToString(this); }

/**
 * Convert a GUID string into a GUID structure.
 *
 * Performs no error checking. If this function is given a string containing
 * an invalid GUID, the function will silently succeed, but the GUID generated
 * will not be useful.
 *
 * @param pchGUID string containing an ASCII representation of a GUID.
 * @returns a GUID structure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GUID.ToString
 */
inline GUID StringToGUID(StringParam pchGUID)
{
  return GUID(std::move(pchGUID));
}

/// @}

} // namespace SDL

#endif /* SDL3PP_GUID_H_ */
