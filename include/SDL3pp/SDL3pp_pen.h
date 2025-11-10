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
 * To get started with pens, simply handle SDL_EVENT_PEN_* events. When a pen
 * starts providing input, SDL will assign it a unique PenID, which will remain
 * for the life of the process, as long as the pen stays connected.
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
 * These are the valid values for the `axis` field in PenAxisEvent. All axes are
 * either normalised to 0..1 or report a (positive or negative) angle in
 * degrees, with 0.0 representing the centre. Not all pens/backends support all
 * axes: unsupported axes are always zero.
 *
 * To convert angles for tilt and rotation into vector representation, use sin
 * on the XTILT, YTILT, or ROTATION component, for example:
 *
 * `sin(xtilt * PI_F / 180.0)`.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using PenAxis = SDL_PenAxis;

constexpr PenAxis PEN_AXIS_PRESSURE =
  SDL_PEN_AXIS_PRESSURE; ///< Pen pressure. Unidirectional: 0 to 1.0

/// Pen horizontal tilt angle. Bidirectional: -90.0 to 90.0 (left-to-right).
constexpr PenAxis PEN_AXIS_XTILT = SDL_PEN_AXIS_XTILT;

/// Pen vertical tilt angle. Bidirectional: -90.0 to 90.0 (top-to-down).
constexpr PenAxis PEN_AXIS_YTILT = SDL_PEN_AXIS_YTILT;

/// Pen distance to drawing surface. Unidirectional: 0.0 to 1.0
constexpr PenAxis PEN_AXIS_DISTANCE = SDL_PEN_AXIS_DISTANCE;

/**
 * Pen barrel rotation. Bidirectional: -180 to 179.9 (clockwise, 0 is facing up,
 * -180.0 is facing down).
 */
constexpr PenAxis PEN_AXIS_ROTATION = SDL_PEN_AXIS_ROTATION;

/// Pen finger wheel or slider (e.g., Airbrush Pen). Unidirectional: 0 to 1.0
constexpr PenAxis PEN_AXIS_SLIDER = SDL_PEN_AXIS_SLIDER;

/// Pressure from squeezing the pen ("barrel pressure").
constexpr PenAxis PEN_AXIS_TANGENTIAL_PRESSURE =
  SDL_PEN_AXIS_TANGENTIAL_PRESSURE;

/**
 * Total known pen axis types in this version of SDL. This number may grow in
 * future releases!
 */
constexpr PenAxis PEN_AXIS_COUNT = SDL_PEN_AXIS_COUNT;

/// @}

} // namespace SDL

#endif /* SDL3PP_PEN_H_ */
