#ifndef SDL3PP_TOUCH_H_
#define SDL3PP_TOUCH_H_

#include <SDL3/SDL_touch.h>
#include "SDL3pp_error.h"
#include "SDL3pp_mouse.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryTouch Touch Support
 *
 * SDL offers touch input, on platforms that support it. It can manage
 * multiple touch devices and track multiple fingers on those devices.
 *
 * Touches are mostly dealt with through the event system, in the
 * EVENT_FINGER_DOWN, EVENT_FINGER_MOTION, and EVENT_FINGER_UP
 * events, but there are also functions to query for hardware details, etc.
 *
 * The touch system, by default, will also send virtual mouse events; this can
 * be useful for making a some desktop apps work on a phone without
 * significant changes. For apps that care about mouse and touch input
 * separately, they should ignore mouse events that have a `which` field of
 * TOUCH_MOUSEID.
 *
 * @{
 */

/// Alias to raw representation for Finger.
using FingerRaw = SDL_Finger;

/**
 * A unique ID for a touch device.
 *
 * This ID is valid for the time the device is connected to the system, and is
 * never reused for the lifetime of the application.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using TouchID = SDL_TouchID;

/**
 * A unique ID for a single finger on a touch device.
 *
 * This ID is valid for the time the finger (stylus, etc) is touching and will
 * be unique for all fingers currently in contact, so this ID tracks the
 * lifetime of a single continuous touch. This value may represent an index, a
 * pointer, or some other unique ID, depending on the platform.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using FingerID = SDL_FingerID;

/**
 * An enum that describes the type of a touch device.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using TouchDeviceType = SDL_TouchDeviceType;

constexpr TouchDeviceType TOUCH_DEVICE_INVALID =
  SDL_TOUCH_DEVICE_INVALID; ///< TOUCH_DEVICE_INVALID

constexpr TouchDeviceType TOUCH_DEVICE_DIRECT =
  SDL_TOUCH_DEVICE_DIRECT; ///< touch screen with window-relative coordinates

constexpr TouchDeviceType TOUCH_DEVICE_INDIRECT_ABSOLUTE =
  SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE; ///< trackpad with absolute device
                                      ///< coordinates

constexpr TouchDeviceType TOUCH_DEVICE_INDIRECT_RELATIVE =
  SDL_TOUCH_DEVICE_INDIRECT_RELATIVE; ///< trackpad with screen cursor-relative
                                      ///< coordinates

/**
 * Data about a single finger in a multitouch event.
 *
 * Each touch event is a collection of fingers that are simultaneously in
 * contact with the touch device (so a "touch" can be a "multitouch," in
 * reality), and this struct reports details of the specific fingers.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GetTouchFingers
 */
struct Finger : FingerRaw
{
  /**
   * Wraps Finger.
   *
   * @param finger the value to be wrapped
   */
  constexpr Finger(const FingerRaw& finger = {})
    : FingerRaw(finger)
  {
  }

  /**
   * Constructs from its fields.
   *
   * @param id the value for id.
   * @param x the value for x.
   * @param y the value for y.
   * @param pressure the value for pressure.
   */
  constexpr Finger(SDL_FingerID id, float x, float y, float pressure)
    : FingerRaw{id, x, y, pressure}
  {
  }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return id != 0; }

  /**
   * Get the id.
   *
   * @returns current id value.
   */
  constexpr SDL_FingerID GetId() const { return id; }

  /**
   * Set the id.
   *
   * @param newId the new id value.
   * @returns Reference to self.
   */
  constexpr Finger& SetId(SDL_FingerID newId)
  {
    id = newId;
    return *this;
  }

  /**
   * Get the x.
   *
   * @returns current x value.
   */
  constexpr float GetX() const { return x; }

  /**
   * Set the x.
   *
   * @param newX the new x value.
   * @returns Reference to self.
   */
  constexpr Finger& SetX(float newX)
  {
    x = newX;
    return *this;
  }

  /**
   * Get the y.
   *
   * @returns current y value.
   */
  constexpr float GetY() const { return y; }

  /**
   * Set the y.
   *
   * @param newY the new y value.
   * @returns Reference to self.
   */
  constexpr Finger& SetY(float newY)
  {
    y = newY;
    return *this;
  }

  /**
   * Get the pressure.
   *
   * @returns current pressure value.
   */
  constexpr float GetPressure() const { return pressure; }

  /**
   * Set the pressure.
   *
   * @param newPressure the new pressure value.
   * @returns Reference to self.
   */
  constexpr Finger& SetPressure(float newPressure)
  {
    pressure = newPressure;
    return *this;
  }
};

/**
 * The MouseID for mouse events simulated with touch input.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr MouseID TOUCH_MOUSEID = SDL_TOUCH_MOUSEID;

/**
 * The TouchID for touch events simulated with mouse input.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr TouchID MOUSE_TOUCHID = SDL_MOUSE_TOUCHID;

/**
 * Get a list of registered touch devices.
 *
 * On some platforms SDL first sees the touch device if it was actually used.
 * Therefore the returned list might be empty, although devices are available.
 * After using all devices at least once the number will be correct.
 *
 * @returns a 0 terminated array of touch device IDs or nullptr on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline OwnArray<TouchID> GetTouchDevices()
{
  int count = 0;
  auto data = SDL_GetTouchDevices(&count);
  return OwnArray<TouchID>(data, size_t(count));
}

/**
 * Get the touch device name as reported from the driver.
 *
 * @param touchID the touch device instance ID.
 * @returns touch device name, or nullptr on failure; call GetError() for
 *          more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetTouchDeviceName(TouchID touchID)
{
  return SDL_GetTouchDeviceName(touchID);
}

/**
 * Get the type of the given touch device.
 *
 * @param touchID the ID of a touch device.
 * @returns touch device type.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline TouchDeviceType GetTouchDeviceType(TouchID touchID)
{
  return SDL_GetTouchDeviceType(touchID);
}

/**
 * Get a list of active fingers for a given touch device.
 *
 * @param touchID the ID of a touch device.
 * @returns a nullptr terminated array of Finger pointers or nullptr on failure;
 *          call GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline OwnArray<Finger*> GetTouchFingers(TouchID touchID)
{
  int count = 0;
  auto data = reinterpret_cast<Finger**>(SDL_GetTouchFingers(touchID, &count));
  return OwnArray<Finger*>(data, size_t(count));
}

/// @}

} // namespace SDL

#endif /* SDL3PP_TOUCH_H_ */
