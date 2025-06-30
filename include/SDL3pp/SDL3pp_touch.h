#ifndef SDL3PP_TOUCH_H_
#define SDL3PP_TOUCH_H_

#include <SDL3/SDL_touch.h>
#include "SDL3pp_error.h"

namespace SDL {

/**
 * @defgroup CategoryTouch Touch Support
 *
 * SDL offers touch input, on platforms that support it. It can manage
 * multiple touch devices and track multiple fingers on those devices.
 *
 * Touches are mostly dealt with through the event system, in the
 * SDL_EVENT_FINGER_DOWN, SDL_EVENT_FINGER_MOTION, and SDL_EVENT_FINGER_UP
 * events, but there are also functions to query for hardware details, etc.
 *
 * The touch system, by default, will also send virtual mouse events; this can
 * be useful for making a some desktop apps work on a phone without
 * significant changes. For apps that care about mouse and touch input
 * separately, they should ignore mouse events that have a `which` field of
 * SDL_TOUCH_MOUSEID.
 */

/**
 * A unique ID for a touch device.
 *
 * This ID is valid for the time the device is connected to the system, and is
 * never reused for the lifetime of the application.
 *
 * The value 0 is an invalid ID.
 *
 * \since This datatype is available since SDL 3.2.0.
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
 * \since This datatype is available since SDL 3.2.0.
 */
using FingerID = SDL_FingerID;

/**
 * An enum that describes the type of a touch device.
 *
 * \since This enum is available since SDL 3.2.0.
 */
using TouchDeviceType = SDL_TouchDeviceType;
  
constexpr TouchDeviceType TOUCH_DEVICE_INVALID = SDL_TOUCH_DEVICE_INVALID;
constexpr TouchDeviceType TOUCH_DEVICE_DIRECT = SDL_TOUCH_DEVICE_DIRECT;
constexpr TouchDeviceType TOUCH_DEVICE_INDIRECT_ABSOLUTE = SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE;
constexpr TouchDeviceType TOUCH_DEVICE_INDIRECT_RELATIVE = SDL_TOUCH_DEVICE_INDIRECT_RELATIVE;

/**
 * Data about a single finger in a multitouch event.
 *
 * Each touch event is a collection of fingers that are simultaneously in
 * contact with the touch device (so a "touch" can be a "multitouch," in
 * reality), and this struct reports details of the specific fingers.
 *
 * \since This struct is available since SDL 3.2.0.
 *
 * \sa GetTouchFingers
 */
using Finger = SDL_Finger

/**
 * The SDL_MouseID for mouse events simulated with touch input.
 *
 * \since This macro is available since SDL 3.2.0.
 */
constexpr auto TOUCH_MOUSEID = SDL_TOUCH_MOUSEID;
  
/**
 * The SDL_TouchID for touch events simulated with mouse input.
 *
 * \since This macro is available since SDL 3.2.0.
 */
constexpr auto MOUSE_TOUCHID = SDL_MOUSE_TOUCHID;

/**
 * Get a list of registered touch devices.
 *
 * On some platforms SDL first sees the touch device if it was actually used.
 * Therefore the returned list might be empty, although devices are available.
 * After using all devices at least once the number will be correct.
 *
 * \param count a pointer filled in with the number of devices returned, may
 *              be NULL.
 * \returns a 0 terminated array of touch device IDs or NULL on failure; call
 *          SDL_GetError() for more information. This should be freed with
 *          free() when it is no longer needed.
 *
 * \since This function is available since SDL 3.2.0.
 */
inline OwnArray<TouchID> GetTouchDevices()
{
  int count = 0;
  auto data = CheckError(SDL_GetTouchDevices(&count));
  return OwnArray<TouchID>{data, size_t(count)};
}
  
/**
 * Get the touch device name as reported from the driver.
 *
 * \param touchID the touch device instance ID.
 * \returns touch device name, or NULL on failure; call SDL_GetError() for
 *          more information.
 *
 * \since This function is available since SDL 3.2.0.
 */
inline const char* GetTouchDeviceName(TouchID touchID)
{
  return CheckError(SDL_GetTouchDeviceName(touchID));
}

/**
 * Get the type of the given touch device.
 *
 * \param touchID the ID of a touch device.
 * \returns touch device type.
 *
 * \since This function is available since SDL 3.2.0.
 */
inline TouchDeviceType GetTouchDeviceType(TouchID touchID)
{
  return SDL_GetTouchDeviceType(touchID)
}

/**
 * Get a list of active fingers for a given touch device.
 *
 * \param touchID the ID of a touch device.
 * \param count a pointer filled in with the number of fingers returned, can
 *              be NULL.
 * \returns a NULL terminated array of SDL_Finger pointers or NULL on failure;
 *          call SDL_GetError() for more information. This is a single
 *          allocation that should be freed with SDL_free() when it is no
 *          longer needed.
 *
 * \since This function is available since SDL 3.2.0.
 */
inline OwnArray<Finger*> GetTouchFingers(TouchID touchID)
{
  int count = 0;
  OwnArray<Finger*> data = CheckError(SDL_GetTouchFingers(touchID, &count));
  return OwnArray<Finger*>{data, size_t(count)};
}  
} // namespace SDL

#endif /* SDL3PP_TOUCH_H_ */
