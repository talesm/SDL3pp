#ifndef SDL3PP_PEN_H_
#define SDL3PP_PEN_H_

#include <SDL3/SDL_pen.h>
#include "SDL3pp_mouse.h"
#include "SDL3pp_touch.h"

namespace SDL {

/**
 * @defgroup CategoryPen Pen Support
 *
 * SDL pen event handling.
 *
 * SDL provides an API for pressure-sensitive pen (stylus and/or eraser)
 * handling, e.g., for input and drawing tablets or suitably equipped mobile /
 * tablet devices.
 *
 * To get started with pens, simply handle pen events:
 *
 * - EVENT_PEN_PROXIMITY_IN, EVENT_PEN_PROXIMITY_OUT
 *   (PenProximityEvent)
 * - EVENT_PEN_DOWN, EVENT_PEN_UP (PenTouchEvent)
 * - EVENT_PEN_MOTION (PenMotionEvent)
 * - EVENT_PEN_BUTTON_DOWN, EVENT_PEN_BUTTON_UP (PenButtonEvent)
 * - EVENT_PEN_AXIS (PenAxisEvent)
 *
 * When a pen starts providing input, SDL will assign it a unique PenID,
 * which will remain for the life of the process, as long as the pen stays
 * connected.
 *
 * Pens may provide more than simple touch input; they might have other axes,
 * such as pressure, tilt, rotation, etc.
 *
 * @{
 */

/**
 * SDL pen instance IDs.
 *
 * Zero is used to signify an invalid/null device.
 *
 * These show up in pen events when SDL sees input from them. They remain
 * consistent as long as SDL can recognize a tool to be the same pen; but if a
 * pen physically leaves the area and returns, it might get a new ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using PenID = SDL_PenID;

/**
 * The MouseID for mouse events simulated with pen input.
 *
 * @since This constant is available since SDL 3.2.0.
 */
constexpr MouseID PEN_MOUSEID = SDL_PEN_MOUSEID;

/**
 * The TouchID for touch events simulated with pen input.
 *
 * @since This constant is available since SDL 3.2.0.
 */
constexpr TouchID PEN_TOUCHID = SDL_PEN_TOUCHID;

/**
 * Pen input flags, as reported by various pen events' `pen_state` field.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using PenInputFlags = Uint32;

constexpr PenInputFlags PEN_INPUT_DOWN =
  SDL_PEN_INPUT_DOWN; ///< pen is pressed down

constexpr PenInputFlags PEN_INPUT_BUTTON_1 =
  SDL_PEN_INPUT_BUTTON_1; ///< button 1 is pressed

constexpr PenInputFlags PEN_INPUT_BUTTON_2 =
  SDL_PEN_INPUT_BUTTON_2; ///< button 2 is pressed

constexpr PenInputFlags PEN_INPUT_BUTTON_3 =
  SDL_PEN_INPUT_BUTTON_3; ///< button 3 is pressed

constexpr PenInputFlags PEN_INPUT_BUTTON_4 =
  SDL_PEN_INPUT_BUTTON_4; ///< button 4 is pressed

constexpr PenInputFlags PEN_INPUT_BUTTON_5 =
  SDL_PEN_INPUT_BUTTON_5; ///< button 5 is pressed

constexpr PenInputFlags PEN_INPUT_ERASER_TIP =
  SDL_PEN_INPUT_ERASER_TIP; ///< eraser tip is used

/**
 * Pen axis indices.
 *
 * These are the valid values for the `axis` field in PenAxisEvent. All
 * axes are either normalised to 0..1 or report a (positive or negative) angle
 * in degrees, with 0.0 representing the centre. Not all pens/backends support
 * all axes: unsupported axes are always zero.
 *
 * To convert angles for tilt and rotation into vector representation, use
 * sin on the XTILT, YTILT, or ROTATION component, for example:
 *
 * `sin(xtilt * PI_F / 180.0)`.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PenAxis = SDL_PenAxis;

constexpr PenAxis PEN_AXIS_PRESSURE =
  SDL_PEN_AXIS_PRESSURE; ///< Pen pressure.  Unidirectional: 0 to 1.0

/// Pen horizontal tilt angle.  Bidirectional: -90.0 to 90.0 (left-to-right).
constexpr PenAxis PEN_AXIS_XTILT = SDL_PEN_AXIS_XTILT;

/// Pen vertical tilt angle.  Bidirectional: -90.0 to 90.0 (top-to-down).
constexpr PenAxis PEN_AXIS_YTILT = SDL_PEN_AXIS_YTILT;

/// Pen distance to drawing surface.  Unidirectional: 0.0 to 1.0
constexpr PenAxis PEN_AXIS_DISTANCE = SDL_PEN_AXIS_DISTANCE;

/**
 * Pen barrel rotation.  Bidirectional: -180 to 179.9 (clockwise, 0 is facing
 * up, -180.0 is facing down).
 */
constexpr PenAxis PEN_AXIS_ROTATION = SDL_PEN_AXIS_ROTATION;

/// Pen finger wheel or slider (e.g., Airbrush Pen).  Unidirectional: 0 to 1.0
constexpr PenAxis PEN_AXIS_SLIDER = SDL_PEN_AXIS_SLIDER;

/// Pressure from squeezing the pen ("barrel pressure").
constexpr PenAxis PEN_AXIS_TANGENTIAL_PRESSURE =
  SDL_PEN_AXIS_TANGENTIAL_PRESSURE;

/**
 * Total known pen axis types in this version of SDL.  This number may grow in
 * future releases!
 */
constexpr PenAxis PEN_AXIS_COUNT = SDL_PEN_AXIS_COUNT;

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * An enum that describes the type of a pen device.
 *
 * A "direct" device is a pen that touches a graphic display (like an Apple
 * Pencil on an iPad's screen). "Indirect" devices touch an external tablet
 * surface that is connected to the machine but is not a display (like a
 * lower-end Wacom tablet connected over USB).
 *
 * Apps may use this information to decide if they should draw a cursor; if
 * the pen is touching the screen directly, a cursor doesn't make sense and
 * can be in the way, but becomes necessary for indirect devices to know where
 * on the display they are interacting.
 *
 * @since This enum is available since SDL 3.4.0.
 */
using PenDeviceType = SDL_PenDeviceType;

constexpr PenDeviceType PEN_DEVICE_TYPE_INVALID =
  SDL_PEN_DEVICE_TYPE_INVALID; ///< Not a valid pen device.

constexpr PenDeviceType PEN_DEVICE_TYPE_UNKNOWN =
  SDL_PEN_DEVICE_TYPE_UNKNOWN; ///< Don't know specifics of this pen.

constexpr PenDeviceType PEN_DEVICE_TYPE_DIRECT =
  SDL_PEN_DEVICE_TYPE_DIRECT; ///< Pen touches display.

constexpr PenDeviceType PEN_DEVICE_TYPE_INDIRECT =
  SDL_PEN_DEVICE_TYPE_INDIRECT; ///< Pen touches something that isn't the
                                ///< display.

/**
 * Get the device type of the given pen.
 *
 * Many platforms do not supply this information, so an app must always be
 * prepared to get an PEN_DEVICE_TYPE_UNKNOWN result.
 *
 * @param instance_id the pen instance ID.
 * @returns the device type of the given pen, or PEN_DEVICE_TYPE_INVALID
 *          on failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.4.0.
 */
inline PenDeviceType GetPenDeviceType(PenID instance_id)
{
  return SDL_GetPenDeviceType(instance_id);
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

/// @}

} // namespace SDL

#endif /* SDL3PP_PEN_H_ */
