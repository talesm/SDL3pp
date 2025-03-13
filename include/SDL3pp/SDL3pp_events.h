#ifndef SDL3PP_EVENTS_H_
#define SDL3PP_EVENTS_H_

#include <atomic>
#include <chrono>
#include <SDL3/SDL_events.h>
#include "SDL3pp_callbackWrapper.h"
#include "SDL3pp_error.h"
#include "SDL3pp_stdinc.h"
#include "SDL3pp_stringParam.h"
#include "SDL3pp_video.h"

namespace SDL {

/**
 * @defgroup CategoryEvents Category Events
 *
 * Event queue management.
 *
 * It's extremely common--often required--that an app deal with SDL's event
 * queue. Almost all useful information about interactions with the real world
 * flow through here: the user interacting with the computer and app, hardware
 * coming and going, the system changing in some way, etc.
 *
 * An app generally takes a moment, perhaps at the start of a new frame, to
 * examine any events that have occurred since the last time and process or
 * ignore them. This is generally done by calling SDL_PollEvent() in a loop
 * until it returns false (or, if using the main callbacks, events are
 * provided one at a time in calls to SDL_AppEvent() before the next call to
 * SDL_AppIterate(); in this scenario, the app does not call PollEvent()
 * at all).
 *
 * There is other forms of control, too: PeepEvents() has more
 * functionality at the cost of more complexity, and WaitEvent() can block
 * the process until something interesting happens, which might be beneficial
 * for certain types of programs on low-power hardware. One may also call
 * AddEventWatch() to set a callback when new events arrive.
 *
 * The app is free to generate their own events, too: PushEvent() allows the
 * app to put events onto the queue for later retrieval; RegisterEvents()
 * can guarantee that these events have a type that isn't in use by other
 * parts of the system.
 *
 * @{
 */

/**
 * The types of events that can be delivered.
 *
 * @since This enum is available since SDL 3.2.0.
 * @sa EventTypes
 */
using EventType = SDL_EventType;

/**
 * @defgroup EventTypes Event Types
 *
 * Event types for Event.
 * @{
 */

/**
 * Unused (do not remove)
 */
constexpr EventType EVENT_FIRST = SDL_EVENT_FIRST;

/**
 * User-requested quit
 */
constexpr EventType EVENT_QUIT = SDL_EVENT_QUIT;

constexpr EventType EVENT_TERMINATING = SDL_EVENT_TERMINATING;

constexpr EventType EVENT_LOW_MEMORY = SDL_EVENT_LOW_MEMORY;

constexpr EventType EVENT_WILL_ENTER_BACKGROUND =
  SDL_EVENT_WILL_ENTER_BACKGROUND;

constexpr EventType EVENT_DID_ENTER_BACKGROUND = SDL_EVENT_DID_ENTER_BACKGROUND;

constexpr EventType EVENT_WILL_ENTER_FOREGROUND =
  SDL_EVENT_WILL_ENTER_FOREGROUND;

constexpr EventType EVENT_DID_ENTER_FOREGROUND = SDL_EVENT_DID_ENTER_FOREGROUND;

/**
 * The user's locale preferences have changed.
 */
constexpr EventType EVENT_LOCALE_CHANGED = SDL_EVENT_LOCALE_CHANGED;

/**
 * The system theme changed
 */
constexpr EventType EVENT_SYSTEM_THEME_CHANGED = SDL_EVENT_SYSTEM_THEME_CHANGED;

/**
 * Display orientation has changed to data1
 */
constexpr EventType EVENT_DISPLAY_ORIENTATION = SDL_EVENT_DISPLAY_ORIENTATION;

/**
 * Display has been added to the system
 */
constexpr EventType EVENT_DISPLAY_ADDED = SDL_EVENT_DISPLAY_ADDED;

/**
 * Display has been removed from the system
 */
constexpr EventType EVENT_DISPLAY_REMOVED = SDL_EVENT_DISPLAY_REMOVED;

/**
 * Display has changed position
 */
constexpr EventType EVENT_DISPLAY_MOVED = SDL_EVENT_DISPLAY_MOVED;

/**
 * Display has changed desktop mode
 */
constexpr EventType EVENT_DISPLAY_DESKTOP_MODE_CHANGED =
  SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED;

/**
 * Display has changed current mode
 */
constexpr EventType EVENT_DISPLAY_CURRENT_MODE_CHANGED =
  SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED;

/**
 * Display has changed content scale
 */
constexpr EventType EVENT_DISPLAY_CONTENT_SCALE_CHANGED =
  SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED;

constexpr EventType EVENT_DISPLAY_FIRST = SDL_EVENT_DISPLAY_FIRST;

constexpr EventType EVENT_DISPLAY_LAST = SDL_EVENT_DISPLAY_LAST;

/**
 * Window has been shown
 */
constexpr EventType EVENT_WINDOW_SHOWN = SDL_EVENT_WINDOW_SHOWN;

/**
 * Window has been hidden
 */
constexpr EventType EVENT_WINDOW_HIDDEN = SDL_EVENT_WINDOW_HIDDEN;

/**
 * Window has been exposed and should be redrawn, and can be redrawn directly
 * from event watchers for this event
 */
constexpr EventType EVENT_WINDOW_EXPOSED = SDL_EVENT_WINDOW_EXPOSED;

/**
 * Window has been moved to data1, data2
 */
constexpr EventType EVENT_WINDOW_MOVED = SDL_EVENT_WINDOW_MOVED;

/**
 * Window has been resized to data1xdata2
 */
constexpr EventType EVENT_WINDOW_RESIZED = SDL_EVENT_WINDOW_RESIZED;

/**
 * The pixel size of the window has changed to data1xdata2
 */
constexpr EventType EVENT_WINDOW_PIXEL_SIZE_CHANGED =
  SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED;

/**
 * The pixel size of a Metal view associated with the window has changed
 */
constexpr EventType EVENT_WINDOW_METAL_VIEW_RESIZED =
  SDL_EVENT_WINDOW_METAL_VIEW_RESIZED;

/**
 * Window has been minimized
 */
constexpr EventType EVENT_WINDOW_MINIMIZED = SDL_EVENT_WINDOW_MINIMIZED;

/**
 * Window has been maximized
 */
constexpr EventType EVENT_WINDOW_MAXIMIZED = SDL_EVENT_WINDOW_MAXIMIZED;

/**
 * Window has been restored to normal size and position
 */
constexpr EventType EVENT_WINDOW_RESTORED = SDL_EVENT_WINDOW_RESTORED;

/**
 * Window has gained mouse focus
 */
constexpr EventType EVENT_WINDOW_MOUSE_ENTER = SDL_EVENT_WINDOW_MOUSE_ENTER;

/**
 * Window has lost mouse focus
 */
constexpr EventType EVENT_WINDOW_MOUSE_LEAVE = SDL_EVENT_WINDOW_MOUSE_LEAVE;

/**
 * Window has gained keyboard focus
 */
constexpr EventType EVENT_WINDOW_FOCUS_GAINED = SDL_EVENT_WINDOW_FOCUS_GAINED;

/**
 * Window has lost keyboard focus
 */
constexpr EventType EVENT_WINDOW_FOCUS_LOST = SDL_EVENT_WINDOW_FOCUS_LOST;

/**
 * The window manager requests that the window be closed
 */
constexpr EventType EVENT_WINDOW_CLOSE_REQUESTED =
  SDL_EVENT_WINDOW_CLOSE_REQUESTED;

/**
 * Window had a hit test that wasn't SDL_HITTEST_NORMAL
 */
constexpr EventType EVENT_WINDOW_HIT_TEST = SDL_EVENT_WINDOW_HIT_TEST;

/**
 * The ICC profile of the window's display has changed
 */
constexpr EventType EVENT_WINDOW_ICCPROF_CHANGED =
  SDL_EVENT_WINDOW_ICCPROF_CHANGED;

/**
 * Window has been moved to display data1
 */
constexpr EventType EVENT_WINDOW_DISPLAY_CHANGED =
  SDL_EVENT_WINDOW_DISPLAY_CHANGED;

/**
 * Window display scale has been changed
 */
constexpr EventType EVENT_WINDOW_DISPLAY_SCALE_CHANGED =
  SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED;

/**
 * The window safe area has been changed
 */
constexpr EventType EVENT_WINDOW_SAFE_AREA_CHANGED =
  SDL_EVENT_WINDOW_SAFE_AREA_CHANGED;

/**
 * The window has been occluded
 */
constexpr EventType EVENT_WINDOW_OCCLUDED = SDL_EVENT_WINDOW_OCCLUDED;

/**
 * The window has entered fullscreen mode
 */
constexpr EventType EVENT_WINDOW_ENTER_FULLSCREEN =
  SDL_EVENT_WINDOW_ENTER_FULLSCREEN;

/**
 * The window has left fullscreen mode
 */
constexpr EventType EVENT_WINDOW_LEAVE_FULLSCREEN =
  SDL_EVENT_WINDOW_LEAVE_FULLSCREEN;

constexpr EventType EVENT_WINDOW_DESTROYED = SDL_EVENT_WINDOW_DESTROYED;

/**
 * Window HDR properties have changed
 */
constexpr EventType EVENT_WINDOW_HDR_STATE_CHANGED =
  SDL_EVENT_WINDOW_HDR_STATE_CHANGED;

constexpr EventType EVENT_WINDOW_FIRST = SDL_EVENT_WINDOW_FIRST;

constexpr EventType EVENT_WINDOW_LAST = SDL_EVENT_WINDOW_LAST;

/**
 * Key pressed
 */
constexpr EventType EVENT_KEY_DOWN = SDL_EVENT_KEY_DOWN;

/**
 * Key released
 */
constexpr EventType EVENT_KEY_UP = SDL_EVENT_KEY_UP;

/**
 * Keyboard text editing (composition)
 */
constexpr EventType EVENT_TEXT_EDITING = SDL_EVENT_TEXT_EDITING;

/**
 * Keyboard text input
 */
constexpr EventType EVENT_TEXT_INPUT = SDL_EVENT_TEXT_INPUT;

constexpr EventType EVENT_KEYMAP_CHANGED = SDL_EVENT_KEYMAP_CHANGED;

/**
 * A new keyboard has been inserted into the system
 */
constexpr EventType EVENT_KEYBOARD_ADDED = SDL_EVENT_KEYBOARD_ADDED;

/**
 * A keyboard has been removed
 */
constexpr EventType EVENT_KEYBOARD_REMOVED = SDL_EVENT_KEYBOARD_REMOVED;

/**
 * Keyboard text editing candidates
 */
constexpr EventType EVENT_TEXT_EDITING_CANDIDATES =
  SDL_EVENT_TEXT_EDITING_CANDIDATES;

/**
 * Mouse moved
 */
constexpr EventType EVENT_MOUSE_MOTION = SDL_EVENT_MOUSE_MOTION;

/**
 * Mouse button pressed
 */
constexpr EventType EVENT_MOUSE_BUTTON_DOWN = SDL_EVENT_MOUSE_BUTTON_DOWN;

/**
 * Mouse button released
 */
constexpr EventType EVENT_MOUSE_BUTTON_UP = SDL_EVENT_MOUSE_BUTTON_UP;

/**
 * Mouse wheel motion
 */
constexpr EventType EVENT_MOUSE_WHEEL = SDL_EVENT_MOUSE_WHEEL;

/**
 * A new mouse has been inserted into the system
 */
constexpr EventType EVENT_MOUSE_ADDED = SDL_EVENT_MOUSE_ADDED;

/**
 * A mouse has been removed
 */
constexpr EventType EVENT_MOUSE_REMOVED = SDL_EVENT_MOUSE_REMOVED;

/**
 * Joystick axis motion
 */
constexpr EventType EVENT_JOYSTICK_AXIS_MOTION = SDL_EVENT_JOYSTICK_AXIS_MOTION;

/**
 * Joystick trackball motion
 */
constexpr EventType EVENT_JOYSTICK_BALL_MOTION = SDL_EVENT_JOYSTICK_BALL_MOTION;

/**
 * Joystick hat position change
 */
constexpr EventType EVENT_JOYSTICK_HAT_MOTION = SDL_EVENT_JOYSTICK_HAT_MOTION;

/**
 * Joystick button pressed
 */
constexpr EventType EVENT_JOYSTICK_BUTTON_DOWN = SDL_EVENT_JOYSTICK_BUTTON_DOWN;

/**
 * Joystick button released
 */
constexpr EventType EVENT_JOYSTICK_BUTTON_UP = SDL_EVENT_JOYSTICK_BUTTON_UP;

/**
 * A new joystick has been inserted into the system
 */
constexpr EventType EVENT_JOYSTICK_ADDED = SDL_EVENT_JOYSTICK_ADDED;

/**
 * An opened joystick has been removed
 */
constexpr EventType EVENT_JOYSTICK_REMOVED = SDL_EVENT_JOYSTICK_REMOVED;

/**
 * Joystick battery level change
 */
constexpr EventType EVENT_JOYSTICK_BATTERY_UPDATED =
  SDL_EVENT_JOYSTICK_BATTERY_UPDATED;

/**
 * Joystick update is complete
 */
constexpr EventType EVENT_JOYSTICK_UPDATE_COMPLETE =
  SDL_EVENT_JOYSTICK_UPDATE_COMPLETE;

/**
 * Gamepad axis motion
 */
constexpr EventType EVENT_GAMEPAD_AXIS_MOTION = SDL_EVENT_GAMEPAD_AXIS_MOTION;

/**
 * Gamepad button pressed
 */
constexpr EventType EVENT_GAMEPAD_BUTTON_DOWN = SDL_EVENT_GAMEPAD_BUTTON_DOWN;

/**
 * Gamepad button released
 */
constexpr EventType EVENT_GAMEPAD_BUTTON_UP = SDL_EVENT_GAMEPAD_BUTTON_UP;

/**
 * A new gamepad has been inserted into the system
 */
constexpr EventType EVENT_GAMEPAD_ADDED = SDL_EVENT_GAMEPAD_ADDED;

/**
 * A gamepad has been removed
 */
constexpr EventType EVENT_GAMEPAD_REMOVED = SDL_EVENT_GAMEPAD_REMOVED;

/**
 * The gamepad mapping was updated
 */
constexpr EventType EVENT_GAMEPAD_REMAPPED = SDL_EVENT_GAMEPAD_REMAPPED;

/**
 * Gamepad touchpad was touched
 */
constexpr EventType EVENT_GAMEPAD_TOUCHPAD_DOWN =
  SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN;

/**
 * Gamepad touchpad finger was moved
 */
constexpr EventType EVENT_GAMEPAD_TOUCHPAD_MOTION =
  SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION;

/**
 * Gamepad touchpad finger was lifted
 */
constexpr EventType EVENT_GAMEPAD_TOUCHPAD_UP = SDL_EVENT_GAMEPAD_TOUCHPAD_UP;

/**
 * Gamepad sensor was updated
 */
constexpr EventType EVENT_GAMEPAD_SENSOR_UPDATE =
  SDL_EVENT_GAMEPAD_SENSOR_UPDATE;

/**
 * Gamepad update is complete
 */
constexpr EventType EVENT_GAMEPAD_UPDATE_COMPLETE =
  SDL_EVENT_GAMEPAD_UPDATE_COMPLETE;

/**
 * Gamepad Steam handle has changed
 */
constexpr EventType EVENT_GAMEPAD_STEAM_HANDLE_UPDATED =
  SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED;

constexpr EventType EVENT_FINGER_DOWN = SDL_EVENT_FINGER_DOWN;

constexpr EventType EVENT_FINGER_UP = SDL_EVENT_FINGER_UP;

constexpr EventType EVENT_FINGER_MOTION = SDL_EVENT_FINGER_MOTION;

constexpr EventType EVENT_FINGER_CANCELED = SDL_EVENT_FINGER_CANCELED;

/**
 * The clipboard or primary selection changed
 */
constexpr EventType EVENT_CLIPBOARD_UPDATE = SDL_EVENT_CLIPBOARD_UPDATE;

/**
 * The system requests a file open
 */
constexpr EventType EVENT_DROP_FILE = SDL_EVENT_DROP_FILE;

/**
 * text/plain drag-and-drop event
 */
constexpr EventType EVENT_DROP_TEXT = SDL_EVENT_DROP_TEXT;

/**
 * A new set of drops is beginning (NULL filename)
 */
constexpr EventType EVENT_DROP_BEGIN = SDL_EVENT_DROP_BEGIN;

/**
 * Current set of drops is now complete (NULL filename)
 */
constexpr EventType EVENT_DROP_COMPLETE = SDL_EVENT_DROP_COMPLETE;

/**
 * Position while moving over the window
 */
constexpr EventType EVENT_DROP_POSITION = SDL_EVENT_DROP_POSITION;

/**
 * A new audio device is available
 */
constexpr EventType EVENT_AUDIO_DEVICE_ADDED = SDL_EVENT_AUDIO_DEVICE_ADDED;

/**
 * An audio device has been removed.
 */
constexpr EventType EVENT_AUDIO_DEVICE_REMOVED = SDL_EVENT_AUDIO_DEVICE_REMOVED;

/**
 * An audio device's format has been changed by the system.
 */
constexpr EventType EVENT_AUDIO_DEVICE_FORMAT_CHANGED =
  SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED;

/**
 * A sensor was updated
 */
constexpr EventType EVENT_SENSOR_UPDATE = SDL_EVENT_SENSOR_UPDATE;

/**
 * Pressure-sensitive pen has become available
 */
constexpr EventType EVENT_PEN_PROXIMITY_IN = SDL_EVENT_PEN_PROXIMITY_IN;

/**
 * Pressure-sensitive pen has become unavailable
 */
constexpr EventType EVENT_PEN_PROXIMITY_OUT = SDL_EVENT_PEN_PROXIMITY_OUT;

/**
 * Pressure-sensitive pen touched drawing surface
 */
constexpr EventType EVENT_PEN_DOWN = SDL_EVENT_PEN_DOWN;

/**
 * Pressure-sensitive pen stopped touching drawing surface
 */
constexpr EventType EVENT_PEN_UP = SDL_EVENT_PEN_UP;

/**
 * Pressure-sensitive pen button pressed
 */
constexpr EventType EVENT_PEN_BUTTON_DOWN = SDL_EVENT_PEN_BUTTON_DOWN;

/**
 * Pressure-sensitive pen button released
 */
constexpr EventType EVENT_PEN_BUTTON_UP = SDL_EVENT_PEN_BUTTON_UP;

/**
 * Pressure-sensitive pen is moving on the tablet
 */
constexpr EventType EVENT_PEN_MOTION = SDL_EVENT_PEN_MOTION;

/**
 * Pressure-sensitive pen angle/pressure/etc changed
 */
constexpr EventType EVENT_PEN_AXIS = SDL_EVENT_PEN_AXIS;

/**
 * A new camera device is available
 */
constexpr EventType EVENT_CAMERA_DEVICE_ADDED = SDL_EVENT_CAMERA_DEVICE_ADDED;

/**
 * A camera device has been removed.
 */
constexpr EventType EVENT_CAMERA_DEVICE_REMOVED =
  SDL_EVENT_CAMERA_DEVICE_REMOVED;

/**
 * A camera device has been approved for use by the user.
 */
constexpr EventType EVENT_CAMERA_DEVICE_APPROVED =
  SDL_EVENT_CAMERA_DEVICE_APPROVED;

/**
 * A camera device has been denied for use by the user.
 */
constexpr EventType EVENT_CAMERA_DEVICE_DENIED = SDL_EVENT_CAMERA_DEVICE_DENIED;

/**
 * The render targets have been reset and their contents need to be updated
 */
constexpr EventType EVENT_RENDER_TARGETS_RESET = SDL_EVENT_RENDER_TARGETS_RESET;

/**
 * The device has been reset and all textures need to be recreated
 */
constexpr EventType EVENT_RENDER_DEVICE_RESET = SDL_EVENT_RENDER_DEVICE_RESET;

/**
 * The device has been lost and can't be recovered.
 */
constexpr EventType EVENT_RENDER_DEVICE_LOST = SDL_EVENT_RENDER_DEVICE_LOST;

constexpr EventType EVENT_PRIVATE0 = SDL_EVENT_PRIVATE0;

constexpr EventType EVENT_PRIVATE1 = SDL_EVENT_PRIVATE1;

constexpr EventType EVENT_PRIVATE2 = SDL_EVENT_PRIVATE2;

constexpr EventType EVENT_PRIVATE3 = SDL_EVENT_PRIVATE3;

/**
 * Signals the end of an event poll cycle
 */
constexpr EventType EVENT_POLL_SENTINEL = SDL_EVENT_POLL_SENTINEL;

/**
 * Events SDL_EVENT_USER through SDL_EVENT_LAST are for your use,
 *  and should be allocated with SDL_RegisterEvents()
 */
constexpr EventType EVENT_USER = SDL_EVENT_USER;

/**
 * This last event is only for bounding internal arrays
 */
constexpr EventType EVENT_LAST = SDL_EVENT_LAST;

constexpr EventType EVENT_ENUM_PADDING = SDL_EVENT_ENUM_PADDING;

/// @}

/**
 * Fields shared by every event
 *
 * @since This struct is available since SDL 3.2.0.
 */
using CommonEvent = SDL_CommonEvent;

/**
 * Display state change event data (event.display.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using DisplayEvent = SDL_DisplayEvent;

/**
 * Window state change event data (event.window.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using WindowEvent = SDL_WindowEvent;

/**
 * Keyboard device event structure (event.kdevice.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using KeyboardDeviceEvent = SDL_KeyboardDeviceEvent;

/**
 * Keyboard button event structure (event.key.*)
 *
 * The `key` is the base SDL_Keycode generated by pressing the `scancode`
 * using the current keyboard layout, applying any options specified in
 * SDL_HINT_KEYCODE_OPTIONS. You can get the SDL_Keycode corresponding to the
 * event scancode and modifiers directly from the keyboard layout, bypassing
 * SDL_HINT_KEYCODE_OPTIONS, by calling SDL_GetKeyFromScancode().
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GetKeyFromScancode()
 * @sa HINT_KEYCODE_OPTIONS
 */
using KeyboardEvent = SDL_KeyboardEvent;

/**
 * Keyboard text editing event structure (event.edit.*)
 *
 * The start cursor is the position, in UTF-8 characters, where new typing
 * will be inserted into the editing text. The length is the number of UTF-8
 * characters that will be replaced by new typing.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using TextEditingEvent = SDL_TextEditingEvent;

/**
 * Keyboard IME candidates event structure (event.edit_candidates.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using TextEditingCandidatesEvent = SDL_TextEditingCandidatesEvent;

/**
 * Keyboard text input event structure (event.text.*)
 *
 * This event will never be delivered unless text input is enabled by calling
 * SDL_StartTextInput(). Text input is disabled by default!
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa StartTextInput()
 * @sa StopTextInput()
 */
using TextInputEvent = SDL_TextInputEvent;

/**
 * Mouse device event structure (event.mdevice.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using MouseDeviceEvent = SDL_MouseDeviceEvent;

/**
 * Mouse motion event structure (event.motion.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using MouseMotionEvent = SDL_MouseMotionEvent;

/**
 * Mouse button event structure (event.button.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using MouseButtonEvent = SDL_MouseButtonEvent;

/**
 * Mouse wheel event structure (event.wheel.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using MouseWheelEvent = SDL_MouseWheelEvent;

/**
 * Joystick axis motion event structure (event.jaxis.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using JoyAxisEvent = SDL_JoyAxisEvent;

/**
 * Joystick trackball motion event structure (event.jball.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using JoyBallEvent = SDL_JoyBallEvent;

/**
 * Joystick hat position change event structure (event.jhat.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using JoyHatEvent = SDL_JoyHatEvent;

/**
 * Joystick button event structure (event.jbutton.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using JoyButtonEvent = SDL_JoyButtonEvent;

/**
 * Joystick device event structure (event.jdevice.*)
 *
 * SDL will send JOYSTICK_ADDED events for devices that are already plugged in
 * during SDL_Init.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GamepadDeviceEvent
 */
using JoyDeviceEvent = SDL_JoyDeviceEvent;

/**
 * Joystick battery level change event structure (event.jbattery.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using JoyBatteryEvent = SDL_JoyBatteryEvent;

/**
 * Gamepad axis motion event structure (event.gaxis.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using GamepadAxisEvent = SDL_GamepadAxisEvent;

/**
 * Gamepad button event structure (event.gbutton.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using GamepadButtonEvent = SDL_GamepadButtonEvent;

/**
 * Gamepad device event structure (event.gdevice.*)
 *
 * Joysticks that are supported gamepads receive both an SDL_JoyDeviceEvent
 * and an SDL_GamepadDeviceEvent.
 *
 * SDL will send GAMEPAD_ADDED events for joysticks that are already plugged
 * in during SDL_Init() and are recognized as gamepads. It will also send
 * events for joysticks that get gamepad mappings at runtime.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa JoyDeviceEvent
 */
using GamepadDeviceEvent = SDL_GamepadDeviceEvent;

/**
 * Gamepad touchpad event structure (event.gtouchpad.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using GamepadTouchpadEvent = SDL_GamepadTouchpadEvent;

/**
 * Gamepad sensor event structure (event.gsensor.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using GamepadSensorEvent = SDL_GamepadSensorEvent;

/**
 * Audio device event structure (event.adevice.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using AudioDeviceEvent = SDL_AudioDeviceEvent;

/**
 * Camera device event structure (event.cdevice.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using CameraDeviceEvent = SDL_CameraDeviceEvent;

/**
 * Renderer event structure (event.render.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using RenderEvent = SDL_RenderEvent;

/**
 * Touch finger event structure (event.tfinger.*)
 *
 * Coordinates in this event are normalized. `x` and `y` are normalized to a
 * range between 0.0f and 1.0f, relative to the window, so (0,0) is the top
 * left and (1,1) is the bottom right. Delta coordinates `dx` and `dy` are
 * normalized in the ranges of -1.0f (traversed all the way from the bottom or
 * right to all the way up or left) to 1.0f (traversed all the way from the
 * top or left to all the way down or right).
 *
 * Note that while the coordinates are _normalized_, they are not _clamped_,
 * which means in some circumstances you can get a value outside of this
 * range. For example, a renderer using logical presentation might give a
 * negative value when the touch is in the letterboxing. Some platforms might
 * report a touch outside of the window, which will also be outside of the
 * range.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using TouchFingerEvent = SDL_TouchFingerEvent;

/**
 * Pressure-sensitive pen proximity event structure (event.pmotion.*)
 *
 * When a pen becomes visible to the system (it is close enough to a tablet,
 * etc), SDL will send an SDL_EVENT_PEN_PROXIMITY_IN event with the new pen's
 * ID. This ID is valid until the pen leaves proximity again (has been removed
 * from the tablet's area, the tablet has been unplugged, etc). If the same
 * pen reenters proximity again, it will be given a new ID.
 *
 * Note that "proximity" means "close enough for the tablet to know the tool
 * is there." The pen touching and lifting off from the tablet while not
 * leaving the area are handled by SDL_EVENT_PEN_DOWN and SDL_EVENT_PEN_UP.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenProximityEvent = SDL_PenProximityEvent;

/**
 * Pressure-sensitive pen motion event structure (event.pmotion.*)
 *
 * Depending on the hardware, you may get motion events when the pen is not
 * touching a tablet, for tracking a pen even when it isn't drawing. You
 * should listen for SDL_EVENT_PEN_DOWN and SDL_EVENT_PEN_UP events, or check
 * `pen_state & SDL_PEN_INPUT_DOWN` to decide if a pen is "drawing" when
 * dealing with pen motion.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenMotionEvent = SDL_PenMotionEvent;

/**
 * Pressure-sensitive pen touched event structure (event.ptouch.*)
 *
 * These events come when a pen touches a surface (a tablet, etc), or lifts
 * off from one.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenTouchEvent = SDL_PenTouchEvent;

/**
 * Pressure-sensitive pen button event structure (event.pbutton.*)
 *
 * This is for buttons on the pen itself that the user might click. The pen
 * itself pressing down to draw triggers a SDL_EVENT_PEN_DOWN event instead.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenButtonEvent = SDL_PenButtonEvent;

/**
 * Pressure-sensitive pen pressure / angle event structure (event.paxis.*)
 *
 * You might get some of these events even if the pen isn't touching the
 * tablet.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenAxisEvent = SDL_PenAxisEvent;

/**
 * An event used to drop text or request a file open by the system
 * (event.drop.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using DropEvent = SDL_DropEvent;

/**
 * An event triggered when the clipboard contents have changed
 * (event.clipboard.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using ClipboardEvent = SDL_ClipboardEvent;

/**
 * Sensor event structure (event.sensor.*)
 *
 * @since This struct is available since SDL 3.2.0.
 */
using SensorEvent = SDL_SensorEvent;

/**
 * The "quit requested" event
 *
 * @since This struct is available since SDL 3.2.0.
 */
using QuitEvent = SDL_QuitEvent;

/**
 * A user-defined event type (event.user.*)
 *
 * This event is unique; it is never created by SDL, but only by the
 * application. The event can be pushed onto the event queue using
 * SDL_PushEvent(). The contents of the structure members are completely up to
 * the programmer; the only requirement is that '''type''' is a value obtained
 * from SDL_RegisterEvents().
 *
 * @since This struct is available since SDL 3.2.0.
 */
using UserEvent = SDL_UserEvent;

/**
 * The structure for all events in SDL.
 *
 * The SDL_Event structure is the core of all event handling in SDL. SDL_Event
 * is a union of all event structures used in SDL.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using Event = SDL_Event;

/**
 * Pump the event loop, gathering events from the input devices.
 *
 * This function updates the event queue and internal input device state.
 *
 * SDL_PumpEvents() gathers all the pending input information from devices and
 * places it in the event queue. Without calls to SDL_PumpEvents() no events
 * would ever be placed on the queue. Often the need for calls to
 * SDL_PumpEvents() is hidden from the user since SDL_PollEvent() and
 * SDL_WaitEvent() implicitly call SDL_PumpEvents(). However, if you are not
 * polling or waiting for events (e.g. you are filtering them), then you must
 * call SDL_PumpEvents() to force an event queue update.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa WaitEvent()
 */
inline void PumpEvents() { return SDL_PumpEvents(); }

/**
 * @name EventActions
 * The type of action to request from PeepEvents().
 * @{
 */

/**
 * The type of action to request from PeepEvents().
 *
 * @since This enum is available since SDL 3.2.0.
 */
using EventAction = SDL_EventAction;

/**
 * Add events to the back of the queue.
 */
constexpr EventAction ADDEVENT = SDL_ADDEVENT;

/**
 * Check but don't remove events from the queue front.
 */
constexpr EventAction PEEKEVENT = SDL_PEEKEVENT;

/**
 * Retrieve/remove events from the front of the queue.
 */
constexpr EventAction GETEVENT = SDL_GETEVENT;

/// @}

/**
 * Check the event queue for messages and optionally return them.
 *
 * `action` may be any of the following:
 *
 * ## remarks
 *
 * - `SDL_ADDEVENT`: up to `numevents` events will be added to the back of the
 *   event queue.
 * - `SDL_PEEKEVENT`: `numevents` events at the front of the event queue,
 *   within the specified minimum and maximum type, will be returned to the
 *   caller and will _not_ be removed from the queue. If you pass NULL for
 *   `events`, then `numevents` is ignored and the total number of matching
 *   events will be returned.
 * - `SDL_GETEVENT`: up to `numevents` events at the front of the event queue,
 *   within the specified minimum and maximum type, will be returned to the
 *   caller and will be removed from the queue.
 *
 * You may have to call SDL_PumpEvents() before calling this function.
 * Otherwise, the events may not be ready to be filtered when you call
 * SDL_PeepEvents().
 *
 * @param events destination buffer for the retrieved events, may be NULL to
 *               leave the events in the queue and return the number of events
 *               that would have been stored.
 * @param numevents if action is SDL_ADDEVENT, the number of events to add
 *                  back to the event queue; if action is SDL_PEEKEVENT or
 *                  SDL_GETEVENT, the maximum number of events to retrieve.
 * @param action action to take; see [Remarks](#remarks) for details.
 * @param minType minimum value of the event type to be considered;
 *                SDL_EVENT_FIRST is a safe choice.
 * @param maxType maximum value of the event type to be considered;
 *                SDL_EVENT_LAST is a safe choice.
 * @returns the number of events actually stored or -1 on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PumpEvents()
 * @sa PushEvent()
 */
inline int PeepEvents(Event* events,
                      int numevents,
                      EventAction action,
                      Uint32 minType = EVENT_FIRST,
                      Uint32 maxType = EVENT_LAST)
{
  return SDL_PeepEvents(events, numevents, action, minType, maxType);
}

/**
 * Check for the existence of a certain event type in the event queue.
 *
 * If you need to check for a range of event types, use SDL_HasEvents()
 * instead.
 *
 * @param type the type of event to be queried; see @ref EventTypes for
 * details.
 * @returns true if events matching `type` are present, or false if events
 *          matching `type` are not present.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasEvents()
 */
inline bool HasEvent(Uint32 type) { return SDL_HasEvent(type); }

/**
 * Check for the existence of certain event types in the event queue.
 *
 * If you need to check for a single event type, use SDL_HasEvent() instead.
 *
 * @param minType the low end of event type to be queried, inclusive; see
 *                SDL_EventType for details.
 * @param maxType the high end of event type to be queried, inclusive; see
 *                SDL_EventType for details.
 * @returns true if events with type >= `minType` and <= `maxType` are
 *          present, or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasEvent()
 */
inline bool HasEvents(Uint32 minType = EVENT_FIRST, Uint32 maxType = EVENT_LAST)
{
  return SDL_HasEvents(minType, maxType);
}

/**
 * Clear events of a specific type from the event queue.
 *
 * This will unconditionally remove any events from the queue that match
 * `type`. If you need to remove a range of event types, use SDL_FlushEvents()
 * instead.
 *
 * It's also normal to just ignore events you don't care about in your event
 * loop without calling this function.
 *
 * This function only affects currently queued events. If you want to make
 * sure that all pending OS events are flushed, you can call SDL_PumpEvents()
 * on the main thread immediately before the flush call.
 *
 * If you have user events with custom data that needs to be freed, you should
 * use SDL_PeepEvents() to remove and clean up those events before calling
 * this function.
 *
 * @param type the type of event to be cleared; see SDL_EventType for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FlushEvents()
 */
inline void FlushEvent(Uint32 type) { return SDL_FlushEvent(type); }

/**
 * Clear events of a range of types from the event queue.
 *
 * This will unconditionally remove any events from the queue that are in the
 * range of `minType` to `maxType`, inclusive. If you need to remove a single
 * event type, use SDL_FlushEvent() instead.
 *
 * It's also normal to just ignore events you don't care about in your event
 * loop without calling this function.
 *
 * This function only affects currently queued events. If you want to make
 * sure that all pending OS events are flushed, you can call SDL_PumpEvents()
 * on the main thread immediately before the flush call.
 *
 * @param minType the low end of event type to be cleared, inclusive; see
 *                SDL_EventType for details.
 * @param maxType the high end of event type to be cleared, inclusive; see
 *                SDL_EventType for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FlushEvent()
 */
inline void FlushEvents(Uint32 minType = EVENT_FIRST,
                        Uint32 maxType = EVENT_LAST)
{
  return SDL_FlushEvents(minType, maxType);
}

/**
 * Poll for currently pending events.
 *
 * If `event` is not nullptr, the next event is removed from the queue and
 * stored in the Event structure pointed to by `event`. The true returned
 * refers to this event, immediately stored in the SDL Event structure -- not an
 * event to follow.
 *
 * If `event` is nullptr, it simply returns true if there is an event in the
 * queue, but will not remove it from the queue.
 *
 * As this function may implicitly call PumpEvents(), you can only call
 * this function in the thread that set the video mode.
 *
 * PollEvent() is the favored way of receiving system events since it can
 * be done from the main loop and does not suspend the main loop while waiting
 * on an event to be posted.
 *
 * The common practice is to fully process the event queue once every frame,
 * usually as a first step before updating the game's state:
 *
 * ```c
 * while (game_is_still_running) {
 *     SDL::Event event;
 *     while (SDL::PollEvent(&event)) {  // poll until all events are handled!
 *         // decide what to do with this event.
 *     }
 *
 *     // update game state, draw the current frame
 * }
 * ```
 *
 * @param event the SDL_Event structure to be filled with the next event from
 *              the queue, or nullptr.
 * @returns true if this got an event or false if there are none available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PushEvent()
 * @sa WaitEvent()
 * @sa WaitEventTimeout()
 */
inline bool PollEvent(Event* event) { return SDL_PollEvent(event); }

/**
 * Poll for currently pending events.
 *
 * The next event is removed from the queue and returned.
 *
 * As this function may implicitly call PumpEvents(), you can only call
 * this function in the thread that set the video mode.
 *
 * PollEvent() is the favored way of receiving system events since it can
 * be done from the main loop and does not suspend the main loop while waiting
 * on an event to be posted.
 *
 * The common practice is to fully process the event queue once every frame,
 * usually as a first step before updating the game's state:
 *
 * ```c
 * while (game_is_still_running) {
 *     while (auto event = SDL::PollEvent()) {  // poll until all events are
 * handled!
 *         // decide what to do with this event.
 *     }
 *
 *     // update game state, draw the current frame
 * }
 * ```
 *
 * @returns Event if this got an event or std::nullopt if there are none
 * available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PushEvent()
 * @sa WaitEvent()
 * @sa WaitEventTimeout()
 */
inline std::optional<Event> PollEvent()
{
  if (Event event; PollEvent(&event)) return event;
  return std::nullopt;
}

/**
 * Wait indefinitely for the next available event.
 *
 * If `event` is not nullptr, the next event is removed from the queue and
 * stored in the Event structure pointed to by `event`.
 *
 * As this function may implicitly call PumpEvents(), you can only call
 * this function in the thread that initialized the video subsystem.
 *
 * @param event the Event structure to be filled in with the next event
 *              from the queue, or nullptr.
 * @returns true on success or false if there was an error while waiting for
 *          events; call GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PushEvent()
 * @sa WaitEventTimeout()
 */
inline bool WaitEvent(Event* event) { return SDL_WaitEvent(event); }

/**
 * Wait indefinitely for the next available event.
 *
 * The next event is removed from the queue and returned.
 *
 * As this function may implicitly call PumpEvents(), you can only call
 * this function in the thread that initialized the video subsystem.
 *
 * @returns Event on success or std::nullopt if there was an error while waiting
 * for events; call GetError() for more information.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PushEvent()
 * @sa WaitEventTimeout()
 */
inline std::optional<Event> WaitEvent()
{
  if (Event event; WaitEvent(&event)) return event;
  return std::nullopt;
}

/**
 * Wait until the specified timeout (in milliseconds) for the next available
 * event.
 *
 * If `event` is not nullptr, the next event is removed from the queue and
 * stored in the Event structure pointed to by `event`.
 *
 * As this function may implicitly call PumpEvents(), you can only call this
 * function in the thread that initialized the video subsystem.
 *
 * The timeout is not guaranteed, the actual wait time could be longer due to
 * system scheduling.
 *
 * @param event the Event structure to be filled in with the next event from the
 * queue, or nullptr.
 * @param timeoutMS the maximum number of milliseconds to wait for the next
 *                  available event.
 * @returns true if this got an event or false if the timeout elapsed without
 *          any events available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PushEvent()
 * @sa WaitEvent()
 */
inline bool WaitEventTimeout(Event* event, Sint32 timeoutMS)
{
  return SDL_WaitEventTimeout(event, timeoutMS);
}

/**
 * Wait until the specified timeout (in milliseconds) for the next available
 * event.
 *
 * As this function may implicitly call PumpEvents(), you can only call this
 * function in the thread that initialized the video subsystem.
 *
 * The timeout is not guaranteed, the actual wait time could be longer due to
 * system scheduling.
 *
 * @param timeoutMS the maximum number of milliseconds to wait for the next
 *                  available event.
 * @returns the Event if this got an event or std::nullopt if the timeout
 * elapsed without any events available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PushEvent()
 * @sa WaitEvent()
 */
inline std::optional<Event> WaitEventTimeout(Sint32 timeoutMS)
{
  if (Event event; WaitEventTimeout(&event, timeoutMS)) return event;
  return std::nullopt;
}

/**
 * Wait until the specified timeout (with milliseconds precision) for the next
 * available event.
 *
 * If `event` is not nullptr, the next event is removed from the queue and
 * stored in the Event structure pointed to by `event`.
 *
 * As this function may implicitly call PumpEvents(), you can only call this
 * function in the thread that initialized the video subsystem.
 *
 * The timeout is not guaranteed, the actual wait time could be longer due to
 * system scheduling.
 *
 * @param event the Event structure to be filled in with the next event from the
 * queue, or nullptr.
 * @param timeoutDuration the duration to wait for the next available event,
 * with millisecond precision
 * @returns true if this got an event or false if the timeout elapsed without
 *          any events available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PushEvent()
 * @sa WaitEvent()
 */
inline bool WaitEventTimeout(Event* event,
                             std::chrono::milliseconds timeoutDuration)
{
  return WaitEventTimeout(event, std::max(timeoutDuration.count(), 1l));
}
/**
 * Wait until the specified timeout (with milliseconds precision) for the next
 * available event.
 *
 * As this function may implicitly call PumpEvents(), you can only call this
 * function in the thread that initialized the video subsystem.
 *
 * The timeout is not guaranteed, the actual wait time could be longer due to
 * system scheduling.
 *
 * @param timeoutDuration the duration to wait for the next available event,
 * with millisecond precision.
 * @returns the Event if this got an event or std::nullopt if the timeout
 * elapsed without any events available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa PushEvent()
 * @sa WaitEvent()
 */
inline std::optional<Event> WaitEventTimeout(
  std::chrono::milliseconds timeoutDuration)
{
  if (Event event; WaitEventTimeout(&event, timeoutDuration)) return event;
  return std::nullopt;
}
/**
 * Add an event to the event queue.
 *
 * The event queue can actually be used as a two way communication channel.
 * Not only can events be read from the queue, but the user can also push
 * their own events onto it. `event` is a pointer to the event structure you
 * wish to push onto the queue. The event is copied into the queue, and the
 * caller may dispose of the memory pointed to after SDL_PushEvent() returns.
 *
 * Note: Pushing device input events onto the queue doesn't modify the state
 * of the device within SDL.
 *
 * Note: Events pushed onto the queue with SDL_PushEvent() get passed through
 * the event filter but events added with SDL_PeepEvents() do not.
 *
 * For pushing application-specific events, please use SDL_RegisterEvents() to
 * get an event type that does not conflict with other code that also wants
 * its own custom event types.
 *
 * @param event the SDL_Event to be added to the queue.
 * @returns true on success, false if the event was filtered or on failure;
 *          call GetError() for more information. A common reason for
 *          error is the event queue being full.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PeepEvents()
 * @sa PollEvent()
 * @sa RegisterEvents()
 */
inline bool PushEvent(Event* event) { return SDL_PushEvent(event); }

inline bool PushEvent(const Event& event)
{
  return PushEvent(const_cast<Event*>(&event));
}

/**
 * A function pointer used for callbacks that watch the event queue.
 *
 * @param userdata what was passed as `userdata` to SetEventFilter() or
 *                 AddEventWatch(), etc.
 * @param event the event that triggered the callback.
 * @returns true to permit event to be added to the queue, and false to
 *          disallow it. When used with AddEventWatch(), the return value is
 *          ignored.
 *
 * @threadsafety SDL may call this callback at any time from any thread; the
 *               application is responsible for locking resources the callback
 *               touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SetEventFilter()
 * @sa AddEventWatch()
 */
using EventFilter = SDL_EventFilter;

/**
 * A std::function used for callbacks that watch the event queue.
 *
 * @param event the event that triggered the callback.
 * @returns true to permit event to be added to the queue, and false to
 *          disallow it. When used with AddEventWatch(), the return value is
 *          ignored.
 *
 * @threadsafety SDL may call this callback at any time from any thread; the
 *               application is responsible for locking resources the callback
 *               touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @ingroup ListenerCallback
 *
 * @sa ListenerCallback
 * @sa SetEventFilter()
 * @sa AddEventWatch()
 * @sa EventFilter
 */
using EventFilterFunction = std::function<bool(const Event&)>;

/**
 * Handle returned by AddEventWatch(EventFilterFunction)
 *
 * This can be used later to remove the event filter
 * RemoveEventWatch(EventFilterHandle).
 */
class EventWatchHandle
{
  void* id;

public:
  constexpr explicit EventWatchHandle(void* id = nullptr)
    : id(id)
  {
  }

  /// Get Internal id
  constexpr void* get() const { return id; }

  /// True if has a valid id
  constexpr operator bool() const { return id != 0; }
};

/**
 * Set up a filter to process all events before they are added to the internal
 * event queue.
 *
 * If you just want to see events without modifying them or preventing them
 * from being queued, you should use SDL_AddEventWatch() instead.
 *
 * If the filter function returns true when called, then the event will be
 * added to the internal queue. If it returns false, then the event will be
 * dropped from the queue, but the internal state will still be updated. This
 * allows selective filtering of dynamically arriving events.
 *
 * **WARNING**: Be very careful of what you do in the event filter function,
 * as it may run in a different thread!
 *
 * On platforms that support it, if the quit event is generated by an
 * interrupt signal (e.g. pressing Ctrl-C), it will be delivered to the
 * application at the next event poll.
 *
 * Note: Disabled events never make it to the event filter function; see
 * SDL_SetEventEnabled().
 *
 * Note: Events pushed onto the queue with SDL_PushEvent() get passed through
 * the event filter, but events pushed onto the queue with SDL_PeepEvents() do
 * not.
 *
 * @param filter an SDL_EventFilter function to call when an event happens.
 * @param userdata a pointer that is passed to `filter`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddEventWatch()
 * @sa SetEventEnabled()
 * @sa GetEventFilter()
 * @sa PeepEvents()
 * @sa PushEvent()
 */
inline void SetEventFilter(EventFilter filter, void* userdata)
{
  UniqueWrapper<EventFilterFunction>::erase();
  return SDL_SetEventFilter(filter, userdata);
}

/**
 * Set up a filter to process all events before they are added to the internal
 * event queue.
 *
 * If you just want to see events without modifying them or preventing them
 * from being queued, you should use SDL_AddEventWatch() instead.
 *
 * If the filter function returns true when called, then the event will be
 * added to the internal queue. If it returns false, then the event will be
 * dropped from the queue, but the internal state will still be updated. This
 * allows selective filtering of dynamically arriving events.
 *
 * **WARNING**: Be very careful of what you do in the event filter function,
 * as it may run in a different thread!
 *
 * On platforms that support it, if the quit event is generated by an
 * interrupt signal (e.g. pressing Ctrl-C), it will be delivered to the
 * application at the next event poll.
 *
 * Note: Disabled events never make it to the event filter function; see
 * SDL_SetEventEnabled().
 *
 * Note: Events pushed onto the queue with SDL_PushEvent() get passed through
 * the event filter, but events pushed onto the queue with SDL_PeepEvents() do
 * not.
 *
 * @param filter an EventFilterFunction function to call when an event happens.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @ingroup ListenerCallback
 *
 * @sa ListenerCallback
 * @sa AddEventWatch()
 * @sa SetEventEnabled()
 * @sa GetEventFilter()
 * @sa PeepEvents()
 * @sa PushEvent()
 */
inline auto SetEventFilter(EventFilterFunction filter = {})
{
  using Wrapper = UniqueWrapper<EventFilterFunction>;
  return SDL_SetEventFilter(
    [](void* userdata, SDL_Event* event) {
      return Wrapper::at(userdata)(*event);
    },
    Wrapper::Wrap(std::move(filter)));
}

/**
 * Query the current event filter.
 *
 * This function can be used to "chain" filters, by saving the existing filter
 * before replacing it with a function that will call that saved filter.
 *
 * @param filter the current callback function will be stored here.
 * @param userdata the pointer that is passed to the current event filter will
 *                 be stored here.
 * @returns true on success or false if there is no event filter set.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetEventFilter()
 */
inline bool GetEventFilter(EventFilter* filter, void** userdata)
{
  return SDL_GetEventFilter(filter, userdata);
}

/**
 * Query the current event filter.
 *
 * This function can be used to "chain" filters, by saving the existing filter
 * before replacing it with a function that will call that saved filter.
 *
 * @returns EventFilterFunction on success or false if there is no event filter
 * set.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @ingroup ListenerCallback
 *
 * @sa ListenerCallback
 * @sa SetEventFilter()
 */
inline EventFilterFunction GetEventFilter()
{
  using Wrapper = UniqueWrapper<EventFilterFunction>;

  EventFilter filter;
  void* userdata;
  if (!SDL_GetEventFilter(&filter, &userdata)) return {};
  if (auto cb = Wrapper::at(userdata)) return cb;
  return [filter, userdata](const Event& event) {
    return filter(userdata, const_cast<Event*>(&event));
  };
}

/**
 * Add a callback to be triggered when an event is added to the event queue.
 *
 * `filter` will be called when an event happens, and its return value is
 * ignored.
 *
 * **WARNING**: Be very careful of what you do in the event filter function,
 * as it may run in a different thread!
 *
 * If the quit event is generated by a signal (e.g. SIGINT), it will bypass
 * the internal queue and be delivered to the watch callback immediately, and
 * arrive at the next event poll.
 *
 * Note: the callback is called for events posted by the user through
 * PushEvent(), but not for disabled events, nor for events by a filter
 * callback set with SetEventFilter(), nor for events posted by the user
 * through PeepEvents().
 *
 * @param filter an EventFilter function to call when an event happens.
 * @param userdata a pointer that is passed to `filter`.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @ingroup ListenerCallback
 *
 * @sa ListenerCallback
 * @sa RemoveEventWatch()
 * @sa SetEventFilter()
 */
inline bool AddEventWatch(EventFilter filter, void* userdata)
{
  return SDL_AddEventWatch(filter, userdata);
}

/**
 * Add a callback to be triggered when an event is added to the event queue.
 *
 * `filter` will be called when an event happens, and its return value is
 * ignored.
 *
 * **WARNING**: Be very careful of what you do in the event filter function,
 * as it may run in a different thread!
 *
 * If the quit event is generated by a signal (e.g. SIGINT), it will bypass
 * the internal queue and be delivered to the watch callback immediately, and
 * arrive at the next event poll.
 *
 * Note: the callback is called for events posted by the user through
 * PushEvent(), but not for disabled events, nor for events by a filter
 * callback set with SetEventFilter(), nor for events posted by the user
 * through PeepEvents().
 *
 * @param filter an EventFilterFunction to call when an event happens.
 * @returns a handle that can be used on RemoveEventWatch(EventFilterHandle) on
 * success or false on failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RemoveEventWatch()
 * @sa SetEventFilter()
 */
inline EventWatchHandle AddEventWatch(EventFilterFunction filter)
{
  using Wrapper = CallbackWrapper<EventFilterFunction>;
  using Store = KeyValueWrapper<size_t, EventFilterFunction*>;

  auto cb = Wrapper::Wrap(std::move(filter));

  static std::atomic_size_t lastId = 0;
  size_t id = ++lastId;
  return EventWatchHandle{Store::Wrap(id, std::move(cb))};
}

/**
 * Remove an event watch callback added with SDL_AddEventWatch().
 *
 * This function takes the same input as SDL_AddEventWatch() to identify and
 * delete the corresponding callback.
 *
 * @param filter the function originally passed to SDL_AddEventWatch().
 * @param userdata the pointer originally passed to SDL_AddEventWatch().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddEventWatch()
 */
inline void RemoveEventWatch(EventFilter filter, void* userdata)
{
  SDL_RemoveEventWatch(filter, userdata);
}

/**
 * Remove an event watch callback added with
 * SDL_AddEventWatch(EventFilterFunction).
 *
 * @param handle the handle returned by SDL_AddEventWatch(EventFilterFunction).
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @ingroup ListenerCallback
 *
 * @sa ListenerCallback
 * @sa AddEventWatch(EventFilterFunction)
 */
inline void RemoveEventWatch(EventWatchHandle handle)
{
  using Store = KeyValueWrapper<size_t, EventFilterFunction*>;
  delete Store::release(handle.get());
}

/**
 * Run a specific filter function on the current event queue, removing any
 * events for which the filter returns false.
 *
 * See SetEventFilter() for more information. Unlike SetEventFilter(), this
 * function does not change the filter permanently, it only uses the supplied
 * filter until this function returns.
 *
 * @param filter the EventFilter function to call when an event happens.
 * @param userdata a pointer that is passed to `filter`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetEventFilter()
 * @sa SetEventFilter()
 */
inline void FilterEvents(EventFilter filter, void* userdata)
{
  SDL_FilterEvents(filter, userdata);
}

/**
 * Run a specific filter function on the current event queue, removing any
 * events for which the filter returns false.
 *
 * See SetEventFilter() for more information. Unlike SetEventFilter(), this
 * function does not change the filter permanently, it only uses the supplied
 * filter until this function returns.
 *
 * @param filter the EventFilter function to call when an event happens.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @ingroup SyncCallback
 *
 * @sa SyncCallback
 * @sa GetEventFilter()
 * @sa SetEventFilter()
 */
inline void FilterEvents(EventFilterFunction filter)
{
  return FilterEvents(
    [](void* userdata, SDL_Event* event) {
      auto& f = *static_cast<EventFilterFunction*>(userdata);
      return f(*event);
    },
    &filter);
}
/**
 * Set the state of processing events by type.
 *
 * @param type the type of event; see SDL_EventType for details.
 * @param enabled whether to process the event or not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa EventEnabled()
 */
inline void SetEventEnabled(Uint32 type, bool enabled)
{
  return SDL_SetEventEnabled(type, enabled);
}

/**
 * Query the state of processing events by type.
 *
 * @param type the type of event; see SDL_EventType for details.
 * @returns true if the event is being processed, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetEventEnabled()
 */
inline bool EventEnabled(Uint32 type) { return SDL_EventEnabled(type); }

/**
 * Allocate a set of user-defined events, and return the beginning event
 * number for that set of events.
 *
 * @param numevents the number of events to be allocated.
 * @returns the beginning event number, or 0 if numevents is invalid or if
 *          there are not enough user-defined events left.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PushEvent()
 */
inline Uint32 RegisterEvents(int numevents)
{
  return SDL_RegisterEvents(numevents);
}

/**
 * Get window associated with an event.
 *
 * @param event an event containing a `windowID`.
 * @returns the associated window on success or NULL if there is none.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent()
 * @sa WaitEvent()
 * @sa WaitEventTimeout()
 */
inline WindowRef GetWindowFromEvent(const Event* event)
{
  return SDL_GetWindowFromEvent(event);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_EVENTS_H_ */
