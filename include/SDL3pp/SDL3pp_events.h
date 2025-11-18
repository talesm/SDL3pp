#ifndef SDL3PP_EVENTS_H_
#define SDL3PP_EVENTS_H_

#include <SDL3/SDL_events.h>
#include "SDL3pp_stdinc.h"
#include "SDL3pp_video.h"

namespace SDL {

/**
 * @defgroup CategoryEvents Event Handling
 *
 * Event queue management.
 *
 * It's extremely common--often required--that an app deal with SDL's event
 * queue. Almost all useful information about interactions with the real world
 * flow through here: the user interacting with the computer and app, hardware
 * coming and going, the system changing in some way, etc.
 *
 * An app generally takes a moment, perhaps at the start of a new frame, to
 * examine any events that have occured since the last time and process or
 * ignore them. This is generally done by calling PollEvent() in a loop until it
 * returns false (or, if using the main callbacks, events are provided one at a
 * time in calls to SDL_AppEvent() before the next call to SDL_AppIterate(); in
 * this scenario, the app does not call PollEvent() at all).
 *
 * There is other forms of control, too: PeepEvents() has more functionality at
 * the cost of more complexity, and WaitEvent() can block the process until
 * something interesting happens, which might be beneficial for certain types of
 * programs on low-power hardware. One may also call AddEventWatch() to set a
 * callback when new events arrive.
 *
 * The app is free to generate their own events, too: PushEvent allows the app
 * to put events onto the queue for later retrieval; RegisterEvents can
 * guarantee that these events have a type that isn't in use by other parts of
 * the system.
 *
 * @{
 */

/**
 * @defgroup EventTypes Event Types
 *
 * Event types for Event.
 * @{
 */

/**
 * The types of events that can be delivered.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using EventType = SDL_EventType;

constexpr EventType EVENT_FIRST = SDL_EVENT_FIRST; ///< Unused (do not remove)

constexpr EventType EVENT_QUIT = SDL_EVENT_QUIT; ///< User-requested quit

/**
 * The application is being terminated by the OS. This event must be handled in
 * a callback set with AddEventWatch(). Called on iOS in
 * applicationWillTerminate() Called on Android in onDestroy()
 */
constexpr EventType EVENT_TERMINATING = SDL_EVENT_TERMINATING;

/**
 * The application is low on memory, free memory if possible. This event must be
 * handled in a callback set with AddEventWatch(). Called on iOS in
 * applicationDidReceiveMemoryWarning() Called on Android in onTrimMemory()
 */
constexpr EventType EVENT_LOW_MEMORY = SDL_EVENT_LOW_MEMORY;

/**
 * The application is about to enter the background. This event must be handled
 * in a callback set with AddEventWatch(). Called on iOS in
 * applicationWillResignActive() Called on Android in onPause()
 */
constexpr EventType EVENT_WILL_ENTER_BACKGROUND =
  SDL_EVENT_WILL_ENTER_BACKGROUND;

/**
 * The application did enter the background and may not get CPU for some time.
 * This event must be handled in a callback set with AddEventWatch(). Called on
 * iOS in applicationDidEnterBackground() Called on Android in onPause()
 */
constexpr EventType EVENT_DID_ENTER_BACKGROUND = SDL_EVENT_DID_ENTER_BACKGROUND;

/**
 * The application is about to enter the foreground. This event must be handled
 * in a callback set with AddEventWatch(). Called on iOS in
 * applicationWillEnterForeground() Called on Android in onResume()
 */
constexpr EventType EVENT_WILL_ENTER_FOREGROUND =
  SDL_EVENT_WILL_ENTER_FOREGROUND;

/**
 * The application is now interactive. This event must be handled in a callback
 * set with AddEventWatch(). Called on iOS in applicationDidBecomeActive()
 * Called on Android in onResume()
 */
constexpr EventType EVENT_DID_ENTER_FOREGROUND = SDL_EVENT_DID_ENTER_FOREGROUND;

constexpr EventType EVENT_LOCALE_CHANGED =
  SDL_EVENT_LOCALE_CHANGED; ///< The user's locale preferences have changed.

constexpr EventType EVENT_SYSTEM_THEME_CHANGED =
  SDL_EVENT_SYSTEM_THEME_CHANGED; ///< The system theme changed

constexpr EventType EVENT_DISPLAY_ORIENTATION =
  SDL_EVENT_DISPLAY_ORIENTATION; ///< Display orientation has changed to data1

constexpr EventType EVENT_DISPLAY_ADDED =
  SDL_EVENT_DISPLAY_ADDED; ///< Display has been added to the system

constexpr EventType EVENT_DISPLAY_REMOVED =
  SDL_EVENT_DISPLAY_REMOVED; ///< Display has been removed from the system

constexpr EventType EVENT_DISPLAY_MOVED =
  SDL_EVENT_DISPLAY_MOVED; ///< Display has changed position

constexpr EventType EVENT_DISPLAY_DESKTOP_MODE_CHANGED =
  SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED; ///< Display has changed desktop mode

constexpr EventType EVENT_DISPLAY_CURRENT_MODE_CHANGED =
  SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED; ///< Display has changed current mode

constexpr EventType EVENT_DISPLAY_CONTENT_SCALE_CHANGED =
  SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED; ///< Display has changed content
                                           ///< scale

constexpr EventType EVENT_DISPLAY_FIRST =
  SDL_EVENT_DISPLAY_FIRST; ///< DISPLAY_FIRST

constexpr EventType EVENT_DISPLAY_LAST =
  SDL_EVENT_DISPLAY_LAST; ///< DISPLAY_LAST

constexpr EventType EVENT_WINDOW_SHOWN =
  SDL_EVENT_WINDOW_SHOWN; ///< Window has been shown

constexpr EventType EVENT_WINDOW_HIDDEN =
  SDL_EVENT_WINDOW_HIDDEN; ///< Window has been hidden

/**
 * Window has been exposed and should be redrawn, and can be redrawn directly
 * from event watchers for this event
 */
constexpr EventType EVENT_WINDOW_EXPOSED = SDL_EVENT_WINDOW_EXPOSED;

constexpr EventType EVENT_WINDOW_MOVED =
  SDL_EVENT_WINDOW_MOVED; ///< Window has been moved to data1, data2

constexpr EventType EVENT_WINDOW_RESIZED =
  SDL_EVENT_WINDOW_RESIZED; ///< Window has been resized to data1xdata2

/// The pixel size of the window has changed to data1xdata2
constexpr EventType EVENT_WINDOW_PIXEL_SIZE_CHANGED =
  SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED;

/// The pixel size of a Metal view associated with the window has changed
constexpr EventType EVENT_WINDOW_METAL_VIEW_RESIZED =
  SDL_EVENT_WINDOW_METAL_VIEW_RESIZED;

constexpr EventType EVENT_WINDOW_MINIMIZED =
  SDL_EVENT_WINDOW_MINIMIZED; ///< Window has been minimized

constexpr EventType EVENT_WINDOW_MAXIMIZED =
  SDL_EVENT_WINDOW_MAXIMIZED; ///< Window has been maximized

/// Window has been restored to normal size and position
constexpr EventType EVENT_WINDOW_RESTORED = SDL_EVENT_WINDOW_RESTORED;

constexpr EventType EVENT_WINDOW_MOUSE_ENTER =
  SDL_EVENT_WINDOW_MOUSE_ENTER; ///< Window has gained mouse focus

constexpr EventType EVENT_WINDOW_MOUSE_LEAVE =
  SDL_EVENT_WINDOW_MOUSE_LEAVE; ///< Window has lost mouse focus

constexpr EventType EVENT_WINDOW_FOCUS_GAINED =
  SDL_EVENT_WINDOW_FOCUS_GAINED; ///< Window has gained keyboard focus

constexpr EventType EVENT_WINDOW_FOCUS_LOST =
  SDL_EVENT_WINDOW_FOCUS_LOST; ///< Window has lost keyboard focus

/// The window manager requests that the window be closed
constexpr EventType EVENT_WINDOW_CLOSE_REQUESTED =
  SDL_EVENT_WINDOW_CLOSE_REQUESTED;

constexpr EventType EVENT_WINDOW_HIT_TEST =
  SDL_EVENT_WINDOW_HIT_TEST; ///< Window had a hit test that wasn't
                             ///< HITTEST_NORMAL

/// The ICC profile of the window's display has changed
constexpr EventType EVENT_WINDOW_ICCPROF_CHANGED =
  SDL_EVENT_WINDOW_ICCPROF_CHANGED;

constexpr EventType EVENT_WINDOW_DISPLAY_CHANGED =
  SDL_EVENT_WINDOW_DISPLAY_CHANGED; ///< Window has been moved to display data1

constexpr EventType EVENT_WINDOW_DISPLAY_SCALE_CHANGED =
  SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED; ///< Window display scale has been
                                          ///< changed

constexpr EventType EVENT_WINDOW_SAFE_AREA_CHANGED =
  SDL_EVENT_WINDOW_SAFE_AREA_CHANGED; ///< The window safe area has been changed

constexpr EventType EVENT_WINDOW_OCCLUDED =
  SDL_EVENT_WINDOW_OCCLUDED; ///< The window has been occluded

constexpr EventType EVENT_WINDOW_ENTER_FULLSCREEN =
  SDL_EVENT_WINDOW_ENTER_FULLSCREEN; ///< The window has entered fullscreen mode

constexpr EventType EVENT_WINDOW_LEAVE_FULLSCREEN =
  SDL_EVENT_WINDOW_LEAVE_FULLSCREEN; ///< The window has left fullscreen mode

/**
 * The window with the associated ID is being or has been destroyed. If this
 * message is being handled in an event watcher, the window handle is still
 * valid and can still be used to retrieve any properties associated with the
 * window. Otherwise, the handle has already been destroyed and all resources
 * associated with it are invalid
 */
constexpr EventType EVENT_WINDOW_DESTROYED = SDL_EVENT_WINDOW_DESTROYED;

constexpr EventType EVENT_WINDOW_HDR_STATE_CHANGED =
  SDL_EVENT_WINDOW_HDR_STATE_CHANGED; ///< Window HDR properties have changed

constexpr EventType EVENT_WINDOW_FIRST =
  SDL_EVENT_WINDOW_FIRST; ///< WINDOW_FIRST

constexpr EventType EVENT_WINDOW_LAST = SDL_EVENT_WINDOW_LAST; ///< WINDOW_LAST

constexpr EventType EVENT_KEY_DOWN = SDL_EVENT_KEY_DOWN; ///< Key pressed

constexpr EventType EVENT_KEY_UP = SDL_EVENT_KEY_UP; ///< Key released

constexpr EventType EVENT_TEXT_EDITING =
  SDL_EVENT_TEXT_EDITING; ///< Keyboard text editing (composition)

constexpr EventType EVENT_TEXT_INPUT =
  SDL_EVENT_TEXT_INPUT; ///< Keyboard text input

/**
 * Keymap changed due to a system event such as an input language or keyboard
 * layout change.
 */
constexpr EventType EVENT_KEYMAP_CHANGED = SDL_EVENT_KEYMAP_CHANGED;

constexpr EventType EVENT_KEYBOARD_ADDED =
  SDL_EVENT_KEYBOARD_ADDED; ///< A new keyboard has been inserted into the
                            ///< system

constexpr EventType EVENT_KEYBOARD_REMOVED =
  SDL_EVENT_KEYBOARD_REMOVED; ///< A keyboard has been removed

constexpr EventType EVENT_TEXT_EDITING_CANDIDATES =
  SDL_EVENT_TEXT_EDITING_CANDIDATES; ///< Keyboard text editing candidates

constexpr EventType EVENT_MOUSE_MOTION =
  SDL_EVENT_MOUSE_MOTION; ///< Mouse moved

constexpr EventType EVENT_MOUSE_BUTTON_DOWN =
  SDL_EVENT_MOUSE_BUTTON_DOWN; ///< Mouse button pressed

constexpr EventType EVENT_MOUSE_BUTTON_UP =
  SDL_EVENT_MOUSE_BUTTON_UP; ///< Mouse button released

constexpr EventType EVENT_MOUSE_WHEEL =
  SDL_EVENT_MOUSE_WHEEL; ///< Mouse wheel motion

constexpr EventType EVENT_MOUSE_ADDED =
  SDL_EVENT_MOUSE_ADDED; ///< A new mouse has been inserted into the system

constexpr EventType EVENT_MOUSE_REMOVED =
  SDL_EVENT_MOUSE_REMOVED; ///< A mouse has been removed

constexpr EventType EVENT_JOYSTICK_AXIS_MOTION =
  SDL_EVENT_JOYSTICK_AXIS_MOTION; ///< Joystick axis motion

constexpr EventType EVENT_JOYSTICK_BALL_MOTION =
  SDL_EVENT_JOYSTICK_BALL_MOTION; ///< Joystick trackball motion

constexpr EventType EVENT_JOYSTICK_HAT_MOTION =
  SDL_EVENT_JOYSTICK_HAT_MOTION; ///< Joystick hat position change

constexpr EventType EVENT_JOYSTICK_BUTTON_DOWN =
  SDL_EVENT_JOYSTICK_BUTTON_DOWN; ///< Joystick button pressed

constexpr EventType EVENT_JOYSTICK_BUTTON_UP =
  SDL_EVENT_JOYSTICK_BUTTON_UP; ///< Joystick button released

constexpr EventType EVENT_JOYSTICK_ADDED =
  SDL_EVENT_JOYSTICK_ADDED; ///< A new joystick has been inserted into the
                            ///< system

constexpr EventType EVENT_JOYSTICK_REMOVED =
  SDL_EVENT_JOYSTICK_REMOVED; ///< An opened joystick has been removed

constexpr EventType EVENT_JOYSTICK_BATTERY_UPDATED =
  SDL_EVENT_JOYSTICK_BATTERY_UPDATED; ///< Joystick battery level change

constexpr EventType EVENT_JOYSTICK_UPDATE_COMPLETE =
  SDL_EVENT_JOYSTICK_UPDATE_COMPLETE; ///< Joystick update is complete

constexpr EventType EVENT_GAMEPAD_AXIS_MOTION =
  SDL_EVENT_GAMEPAD_AXIS_MOTION; ///< Gamepad axis motion

constexpr EventType EVENT_GAMEPAD_BUTTON_DOWN =
  SDL_EVENT_GAMEPAD_BUTTON_DOWN; ///< Gamepad button pressed

constexpr EventType EVENT_GAMEPAD_BUTTON_UP =
  SDL_EVENT_GAMEPAD_BUTTON_UP; ///< Gamepad button released

constexpr EventType EVENT_GAMEPAD_ADDED =
  SDL_EVENT_GAMEPAD_ADDED; ///< A new gamepad has been inserted into the system

constexpr EventType EVENT_GAMEPAD_REMOVED =
  SDL_EVENT_GAMEPAD_REMOVED; ///< A gamepad has been removed

constexpr EventType EVENT_GAMEPAD_REMAPPED =
  SDL_EVENT_GAMEPAD_REMAPPED; ///< The gamepad mapping was updated

constexpr EventType EVENT_GAMEPAD_TOUCHPAD_DOWN =
  SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN; ///< Gamepad touchpad was touched

constexpr EventType EVENT_GAMEPAD_TOUCHPAD_MOTION =
  SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION; ///< Gamepad touchpad finger was moved

constexpr EventType EVENT_GAMEPAD_TOUCHPAD_UP =
  SDL_EVENT_GAMEPAD_TOUCHPAD_UP; ///< Gamepad touchpad finger was lifted

constexpr EventType EVENT_GAMEPAD_SENSOR_UPDATE =
  SDL_EVENT_GAMEPAD_SENSOR_UPDATE; ///< Gamepad sensor was updated

constexpr EventType EVENT_GAMEPAD_UPDATE_COMPLETE =
  SDL_EVENT_GAMEPAD_UPDATE_COMPLETE; ///< Gamepad update is complete

constexpr EventType EVENT_GAMEPAD_STEAM_HANDLE_UPDATED =
  SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED; ///< Gamepad Steam handle has changed

constexpr EventType EVENT_FINGER_DOWN = SDL_EVENT_FINGER_DOWN; ///< FINGER_DOWN

constexpr EventType EVENT_FINGER_UP = SDL_EVENT_FINGER_UP; ///< FINGER_UP

constexpr EventType EVENT_FINGER_MOTION =
  SDL_EVENT_FINGER_MOTION; ///< FINGER_MOTION

constexpr EventType EVENT_FINGER_CANCELED =
  SDL_EVENT_FINGER_CANCELED; ///< FINGER_CANCELED

constexpr EventType EVENT_CLIPBOARD_UPDATE =
  SDL_EVENT_CLIPBOARD_UPDATE; ///< The clipboard or primary selection changed

constexpr EventType EVENT_DROP_FILE =
  SDL_EVENT_DROP_FILE; ///< The system requests a file open

constexpr EventType EVENT_DROP_TEXT =
  SDL_EVENT_DROP_TEXT; ///< text/plain drag-and-drop event

constexpr EventType EVENT_DROP_BEGIN =
  SDL_EVENT_DROP_BEGIN; ///< A new set of drops is beginning (NULL filename)

/// Current set of drops is now complete (NULL filename)
constexpr EventType EVENT_DROP_COMPLETE = SDL_EVENT_DROP_COMPLETE;

constexpr EventType EVENT_DROP_POSITION =
  SDL_EVENT_DROP_POSITION; ///< Position while moving over the window

constexpr EventType EVENT_AUDIO_DEVICE_ADDED =
  SDL_EVENT_AUDIO_DEVICE_ADDED; ///< A new audio device is available

constexpr EventType EVENT_AUDIO_DEVICE_REMOVED =
  SDL_EVENT_AUDIO_DEVICE_REMOVED; ///< An audio device has been removed.

/// An audio device's format has been changed by the system.
constexpr EventType EVENT_AUDIO_DEVICE_FORMAT_CHANGED =
  SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED;

constexpr EventType EVENT_SENSOR_UPDATE =
  SDL_EVENT_SENSOR_UPDATE; ///< A sensor was updated

constexpr EventType EVENT_PEN_PROXIMITY_IN =
  SDL_EVENT_PEN_PROXIMITY_IN; ///< Pressure-sensitive pen has become available

constexpr EventType EVENT_PEN_PROXIMITY_OUT =
  SDL_EVENT_PEN_PROXIMITY_OUT; ///< Pressure-sensitive pen has become
                               ///< unavailable

constexpr EventType EVENT_PEN_DOWN =
  SDL_EVENT_PEN_DOWN; ///< Pressure-sensitive pen touched drawing surface

/// Pressure-sensitive pen stopped touching drawing surface
constexpr EventType EVENT_PEN_UP = SDL_EVENT_PEN_UP;

constexpr EventType EVENT_PEN_BUTTON_DOWN =
  SDL_EVENT_PEN_BUTTON_DOWN; ///< Pressure-sensitive pen button pressed

constexpr EventType EVENT_PEN_BUTTON_UP =
  SDL_EVENT_PEN_BUTTON_UP; ///< Pressure-sensitive pen button released

constexpr EventType EVENT_PEN_MOTION =
  SDL_EVENT_PEN_MOTION; ///< Pressure-sensitive pen is moving on the tablet

constexpr EventType EVENT_PEN_AXIS =
  SDL_EVENT_PEN_AXIS; ///< Pressure-sensitive pen angle/pressure/etc changed

constexpr EventType EVENT_CAMERA_DEVICE_ADDED =
  SDL_EVENT_CAMERA_DEVICE_ADDED; ///< A new camera device is available

constexpr EventType EVENT_CAMERA_DEVICE_REMOVED =
  SDL_EVENT_CAMERA_DEVICE_REMOVED; ///< A camera device has been removed.

/// A camera device has been approved for use by the user.
constexpr EventType EVENT_CAMERA_DEVICE_APPROVED =
  SDL_EVENT_CAMERA_DEVICE_APPROVED;

/// A camera device has been denied for use by the user.
constexpr EventType EVENT_CAMERA_DEVICE_DENIED = SDL_EVENT_CAMERA_DEVICE_DENIED;

/// The render targets have been reset and their contents need to be updated
constexpr EventType EVENT_RENDER_TARGETS_RESET = SDL_EVENT_RENDER_TARGETS_RESET;

/// The device has been reset and all textures need to be recreated
constexpr EventType EVENT_RENDER_DEVICE_RESET = SDL_EVENT_RENDER_DEVICE_RESET;

constexpr EventType EVENT_RENDER_DEVICE_LOST =
  SDL_EVENT_RENDER_DEVICE_LOST; ///< The device has been lost and can't be
                                ///< recovered.

constexpr EventType EVENT_PRIVATE0 = SDL_EVENT_PRIVATE0; ///< PRIVATE0

constexpr EventType EVENT_PRIVATE1 = SDL_EVENT_PRIVATE1; ///< PRIVATE1

constexpr EventType EVENT_PRIVATE2 = SDL_EVENT_PRIVATE2; ///< PRIVATE2

constexpr EventType EVENT_PRIVATE3 = SDL_EVENT_PRIVATE3; ///< PRIVATE3

constexpr EventType EVENT_POLL_SENTINEL =
  SDL_EVENT_POLL_SENTINEL; ///< Signals the end of an event poll cycle

/**
 * Events EVENT_USER through EVENT_LAST are for your use, and should be
 * allocated with RegisterEvents()
 */
constexpr EventType EVENT_USER = SDL_EVENT_USER;

/// This last event is only for bounding internal arrays
constexpr EventType EVENT_LAST = SDL_EVENT_LAST;

constexpr EventType EVENT_ENUM_PADDING =
  SDL_EVENT_ENUM_PADDING; ///< ENUM_PADDING

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
 * The `key` is the base Keycode generated by pressing the `scancode` using the
 * current keyboard layout, applying any options specified in
 * SDL_HINT_KEYCODE_OPTIONS. You can get the Keycode corresponding to the event
 * scancode and modifiers directly from the keyboard layout, bypassing
 * SDL_HINT_KEYCODE_OPTIONS, by calling Keycode.Keycode().
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa Keycode.Keycode
 * @sa SDL_HINT_KEYCODE_OPTIONS
 */
using KeyboardEvent = SDL_KeyboardEvent;

/**
 * Keyboard text editing event structure (event.edit.*)
 *
 * The start cursor is the position, in UTF-8 characters, where new typing will
 * be inserted into the editing text. The length is the number of UTF-8
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
 * Window.StartTextInput(). Text input is disabled by default!
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa Window.StartTextInput
 * @sa Window.StopTextInput
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
 * during Init.
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
 * Joysticks that are supported gamepads receive both an JoyDeviceEvent and an
 * GamepadDeviceEvent.
 *
 * SDL will send GAMEPAD_ADDED events for joysticks that are already plugged in
 * during Init() and are recognized as gamepads. It will also send events for
 * joysticks that get gamepad mappings at runtime.
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
 * range between 0.0f and 1.0f, relative to the window, so (0,0) is the top left
 * and (1,1) is the bottom right. Delta coordinates `dx` and `dy` are normalized
 * in the ranges of -1.0f (traversed all the way from the bottom or right to all
 * the way up or left) to 1.0f (traversed all the way from the top or left to
 * all the way down or right).
 *
 * Note that while the coordinates are _normalized_, they are not _clamped_,
 * which means in some circumstances you can get a value outside of this range.
 * For example, a renderer using logical presentation might give a negative
 * value when the touch is in the letterboxing. Some platforms might report a
 * touch outside of the window, which will also be outside of the range.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using TouchFingerEvent = SDL_TouchFingerEvent;

/**
 * Pressure-sensitive pen proximity event structure (event.pmotion.*)
 *
 * When a pen becomes visible to the system (it is close enough to a tablet,
 * etc), SDL will send an EVENT_PEN_PROXIMITY_IN event with the new pen's ID.
 * This ID is valid until the pen leaves proximity again (has been removed from
 * the tablet's area, the tablet has been unplugged, etc). If the same pen
 * reenters proximity again, it will be given a new ID.
 *
 * Note that "proximity" means "close enough for the tablet to know the tool is
 * there." The pen touching and lifting off from the tablet while not leaving
 * the area are handled by EVENT_PEN_DOWN and EVENT_PEN_UP.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenProximityEvent = SDL_PenProximityEvent;

/**
 * Pressure-sensitive pen motion event structure (event.pmotion.*)
 *
 * Depending on the hardware, you may get motion events when the pen is not
 * touching a tablet, for tracking a pen even when it isn't drawing. You should
 * listen for EVENT_PEN_DOWN and EVENT_PEN_UP events, or check `pen_state &
 * PEN_INPUT_DOWN` to decide if a pen is "drawing" when dealing with pen motion.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenMotionEvent = SDL_PenMotionEvent;

/**
 * Pressure-sensitive pen touched event structure (event.ptouch.*)
 *
 * These events come when a pen touches a surface (a tablet, etc), or lifts off
 * from one.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenTouchEvent = SDL_PenTouchEvent;

/**
 * Pressure-sensitive pen button event structure (event.pbutton.*)
 *
 * This is for buttons on the pen itself that the user might click. The pen
 * itself pressing down to draw triggers a EVENT_PEN_DOWN event instead.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using PenButtonEvent = SDL_PenButtonEvent;

/**
 * Pressure-sensitive pen pressure / angle event structure (event.paxis.*)
 *
 * You might get some of these events even if the pen isn't touching the tablet.
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
 * application. The event can be pushed onto the event queue using PushEvent().
 * The contents of the structure members are completely up to the programmer;
 * the only requirement is that '''type''' is a value obtained from
 * RegisterEvents().
 *
 * @since This struct is available since SDL 3.2.0.
 */
using UserEvent = SDL_UserEvent;

/**
 * The structure for all events in SDL.
 *
 * The Event structure is the core of all event handling in SDL. Event is a
 * union of all event structures used in SDL.
 *
 * @since This struct is available since SDL 3.2.0.
 */
using Event = SDL_Event;

/**
 * Pump the event loop, gathering events from the input devices.
 *
 * This function updates the event queue and internal input device state.
 *
 * PumpEvents() gathers all the pending input information from devices and
 * places it in the event queue. Without calls to PumpEvents() no events would
 * ever be placed on the queue. Often the need for calls to PumpEvents() is
 * hidden from the user since PollEvent() and WaitEvent() implicitly call
 * PumpEvents(). However, if you are not polling or waiting for events (e.g. you
 * are filtering them), then you must call PumpEvents() to force an event queue
 * update.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent
 * @sa WaitEvent
 */
inline void PumpEvents() { SDL_PumpEvents(); }

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

constexpr EventAction ADDEVENT =
  SDL_ADDEVENT; ///< Add events to the back of the queue.

/// Check but don't remove events from the queue front.
constexpr EventAction PEEKEVENT = SDL_PEEKEVENT;

/// Retrieve/remove events from the front of the queue.
constexpr EventAction GETEVENT = SDL_GETEVENT;

/// @}

/**
 * Check the event queue for messages and optionally return them.
 *
 * `action` may be any of the following:
 *
 * ## remarks
 *
 * - `ADDEVENT`: up to `numevents` events will be added to the back of the event
 *   queue.
 * - `PEEKEVENT`: `numevents` events at the front of the event queue, within the
 *   specified minimum and maximum type, will be returned to the caller and will
 *   _not_ be removed from the queue. If you pass nullptr for `events`, then
 *   `numevents` is ignored and the total number of matching events will be
 *   returned.
 * - `GETEVENT`: up to `numevents` events at the front of the event queue,
 *   within the specified minimum and maximum type, will be returned to the
 *   caller and will be removed from the queue.
 *
 * You may have to call PumpEvents() before calling this function. Otherwise,
 * the events may not be ready to be filtered when you call PeepEvents().
 *
 * @param events destination buffer for the retrieved events, may be nullptr to
 *               leave the events in the queue and return the number of events
 *               that would have been stored.
 * @param numevents if action is ADDEVENT, the number of events to add back to
 *                  the event queue; if action is PEEKEVENT or GETEVENT, the
 *                  maximum number of events to retrieve.
 * @param action action to take; see [Remarks](#remarks) for details.
 * @param minType minimum value of the event type to be considered; EVENT_FIRST
 *                is a safe choice.
 * @param maxType maximum value of the event type to be considered; EVENT_LAST
 *                is a safe choice.
 * @returns the number of events actually stored or -1 on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent
 * @sa PumpEvents
 * @sa PushEvent
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
 * If you need to check for a range of event types, use HasEvents() instead.
 *
 * @param type the type of event to be queried; see EventType for details.
 * @returns true if events matching `type` are present, or false if events
 *          matching `type` are not present.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasEvents
 */
inline bool HasEvent(Uint32 type) { return SDL_HasEvent(type); }

/**
 * Check for the existence of certain event types in the event queue.
 *
 * If you need to check for a single event type, use HasEvent() instead.
 *
 * @param minType the low end of event type to be queried, inclusive; see
 *                EventType for details.
 * @param maxType the high end of event type to be queried, inclusive; see
 *                EventType for details.
 * @returns true if events with type >= `minType` and <= `maxType` are present,
 *          or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasEvents
 */
inline bool HasEvents(Uint32 minType = EVENT_FIRST, Uint32 maxType = EVENT_LAST)
{
  return SDL_HasEvents(minType, maxType);
}

/**
 * Clear events of a specific type from the event queue.
 *
 * This will unconditionally remove any events from the queue that match `type`.
 * If you need to remove a range of event types, use FlushEvents() instead.
 *
 * It's also normal to just ignore events you don't care about in your event
 * loop without calling this function.
 *
 * This function only affects currently queued events. If you want to make sure
 * that all pending OS events are flushed, you can call PumpEvents() on the main
 * thread immediately before the flush call.
 *
 * If you have user events with custom data that needs to be freed, you should
 * use PeepEvents() to remove and clean up those events before calling this
 * function.
 *
 * @param type the type of event to be cleared; see EventType for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FlushEvents
 */
inline void FlushEvent(Uint32 type) { SDL_FlushEvent(type); }

/**
 * Clear events of a range of types from the event queue.
 *
 * This will unconditionally remove any events from the queue that are in the
 * range of `minType` to `maxType`, inclusive. If you need to remove a single
 * event type, use FlushEvent() instead.
 *
 * It's also normal to just ignore events you don't care about in your event
 * loop without calling this function.
 *
 * This function only affects currently queued events. If you want to make sure
 * that all pending OS events are flushed, you can call PumpEvents() on the main
 * thread immediately before the flush call.
 *
 * @param minType the low end of event type to be cleared, inclusive; see
 *                EventType for details.
 * @param maxType the high end of event type to be cleared, inclusive; see
 *                EventType for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa FlushEvent
 */
inline void FlushEvents(Uint32 minType = EVENT_FIRST,
                        Uint32 maxType = EVENT_LAST)
{
  SDL_FlushEvents(minType, maxType);
}

/**
 * Poll for currently pending events.
 *
 * If `event` is not nullptr, the next event is removed from the queue and
 * stored in the Event structure pointed to by `event`. The 1 returned refers to
 * this event, immediately stored in the SDL Event structure -- not an event to
 * follow.
 *
 * If `event` is nullptr, it simply returns 1 if there is an event in the queue,
 * but will not remove it from the queue.
 *
 * As this function may implicitly call PumpEvents(), you can only call this
 * function in the thread that set the video mode.
 *
 * PollEvent() is the favored way of receiving system events since it can be
 * done from the main loop and does not suspend the main loop while waiting on
 * an event to be posted.
 *
 * The common practice is to fully process the event queue once every frame,
 * usually as a first step before updating the game's state:
 *
 * ```cpp
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
 * @param event the Event structure to be filled with the next event from the
 *              queue, or nullptr.
 * @returns true if this got an event or false if there are none available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PushEvent
 * @sa WaitEvent
 * @sa WaitEventTimeout
 */
inline bool PollEvent(Event* event) { return SDL_PollEvent(event); }

/**
 * Poll for currently pending events.
 *
 * The next event is removed from the queue and returned.
 *
 * As this function may implicitly call PumpEvents(), you can only call this
 * function in the thread that set the video mode.
 *
 * PollEvent() is the favored way of receiving system events since it can be
 * done from the main loop and does not suspend the main loop while waiting on
 * an event to be posted.
 *
 * The common practice is to fully process the event queue once every frame,
 * usually as a first step before updating the game's state:
 *
 * ```c
 * while (game_is_still_running) {
 *     while (auto event = SDL::PollEvent()) {
 *         // decide what to do with this event.
 *     }
 *
 *     // update game state, draw the current frame
 * }
 * ```
 *
 * @returns Event if this got an event or std::nullopt if there are none
 *          available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PushEvent
 * @sa WaitEvent
 * @sa WaitEventTimeout
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
 * As this function may implicitly call PumpEvents(), you can only call this
 * function in the thread that initialized the video subsystem.
 *
 * @param event the Event structure to be filled in with the next event from the
 *              queue, or nullptr.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent
 * @sa PushEvent
 * @sa WaitEventTimeout
 */
inline void WaitEvent(Event* event) { CheckError(SDL_WaitEvent(event)); }

/**
 * Wait indefinitely for the next available event.
 *
 * The next event is removed from the queue and returned.
 *
 * As this function may implicitly call PumpEvents(), you can only call this
 * function in the thread that initialized the video subsystem.
 *
 * @returns Event on success.
 *
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent
 * @sa PushEvent
 * @sa WaitEventTimeout
 */
inline Event WaitEvent()
{
  Event event;
  WaitEvent(&event);
  return event;
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
 *              queue, or nullptr.
 * @param timeoutMS the maximum number of milliseconds to wait for the next
 *                  available event.
 * @returns true if this got an event or false if the timeout elapsed without
 *          any events available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent
 * @sa PushEvent
 * @sa WaitEvent
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
 * @sa PollEvent
 * @sa PushEvent
 * @sa WaitEvent
 */
inline std::optional<Event> WaitEventTimeout(Sint32 timeoutMS)
{
  if (Event event; WaitEventTimeout(&event, timeoutMS)) return event;
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
 *              queue, or nullptr.
 * @param timeoutDuration the duration to wait for the next available event,
 *                        with millisecond precision
 * @returns true if this got an event or false if the timeout elapsed without
 *          any events available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent
 * @sa PushEvent
 * @sa WaitEvent
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
 *                        with millisecond precision.
 * @returns the Event if this got an event or std::nullopt if the timeout
 *          elapsed without any events available.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent
 * @sa PushEvent
 * @sa WaitEvent
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
 * The event queue can actually be used as a two way communication channel. Not
 * only can events be read from the queue, but the user can also push their own
 * events onto it. `event` is a pointer to the event structure you wish to push
 * onto the queue. The event is copied into the queue, and the caller may
 * dispose of the memory pointed to after PushEvent() returns.
 *
 * Note: Pushing device input events onto the queue doesn't modify the state of
 * the device within SDL.
 *
 * Note: Events pushed onto the queue with PushEvent() get passed through the
 * event filter but events added with PeepEvents() do not.
 *
 * For pushing application-specific events, please use RegisterEvents() to get
 * an event type that does not conflict with other code that also wants its own
 * custom event types.
 *
 * @param event the Event to be added to the queue.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PeepEvents
 * @sa PollEvent
 * @sa RegisterEvents
 */
inline void PushEvent(Event* event) { CheckError(SDL_PushEvent(event)); }

/**
 * Add an event to the event queue.
 *
 * The event queue can actually be used as a two way communication channel. Not
 * only can events be read from the queue, but the user can also push their own
 * events onto it. `event` is a pointer to the event structure you wish to push
 * onto the queue. The event is copied into the queue, and the caller may
 * dispose of the memory pointed to after PushEvent() returns.
 *
 * Note: Pushing device input events onto the queue doesn't modify the state of
 * the device within SDL.
 *
 * Note: Events pushed onto the queue with PushEvent() get passed through the
 * event filter but events added with PeepEvents() do not.
 *
 * For pushing application-specific events, please use RegisterEvents() to get
 * an event type that does not conflict with other code that also wants its own
 * custom event types.
 *
 * @param event the Event to be added to the queue.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PeepEvents
 * @sa PollEvent
 * @sa RegisterEvents
 */
inline void PushEvent(const Event& event)
{
  PushEvent(const_cast<Event*>(&event));
}

/**
 * A function pointer used for callbacks that watch the event queue.
 *
 * @param userdata what was passed as `userdata` to SetEventFilter() or
 *                 AddEventWatch, etc.
 * @param event the event that triggered the callback.
 * @returns true to permit event to be added to the queue, and false to disallow
 *          it. When used with AddEventWatch, the return value is ignored.
 *
 * @threadsafety SDL may call this callback at any time from any thread; the
 *               application is responsible for locking resources the callback
 *               touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa SetEventFilter
 * @sa AddEventWatch
 */
using EventFilter = SDL_EventFilter;

/**
 * A std::function used for callbacks that watch the event queue.
 *
 * @param event the event that triggered the callback.
 * @returns true to permit event to be added to the queue, and false to disallow
 *          it. When used with AddEventWatch, the return value is ignored.
 *
 * @threadsafety SDL may call this callback at any time from any thread; the
 *               application is responsible for locking resources the callback
 *               touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat listener-callback
 *
 * @sa listener-callback
 * @sa SetEventFilter()
 * @sa AddEventWatch()
 * @sa EventFilter
 */
using EventFilterCB = std::function<bool(const Event&)>;

/**
 * Handle returned by AddEventWatch(EventFilterCB)
 *
 * This can be used later to remove the event filter
 * RemoveEventWatch(EventFilterHandle).
 */
struct EventWatchHandle : CallbackHandle
{
  using CallbackHandle::CallbackHandle;
};

/**
 * Set up a filter to process all events before they are added to the internal
 * event queue.
 *
 * If you just want to see events without modifying them or preventing them from
 * being queued, you should use AddEventWatch() instead.
 *
 * If the filter function returns true when called, then the event will be added
 * to the internal queue. If it returns false, then the event will be dropped
 * from the queue, but the internal state will still be updated. This allows
 * selective filtering of dynamically arriving events.
 *
 * **WARNING**: Be very careful of what you do in the event filter function, as
 * it may run in a different thread!
 *
 * On platforms that support it, if the quit event is generated by an interrupt
 * signal (e.g. pressing Ctrl-C), it will be delivered to the application at the
 * next event poll.
 *
 * Note: Disabled events never make it to the event filter function; see
 * SetEventEnabled().
 *
 * Note: Events pushed onto the queue with PushEvent() get passed through the
 * event filter, but events pushed onto the queue with PeepEvents() do not.
 *
 * @param filter an EventFilter function to call when an event happens.
 * @param userdata a pointer that is passed to `filter`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddEventWatch
 * @sa SetEventEnabled
 * @sa GetEventFilter
 * @sa PeepEvents
 * @sa PushEvent
 */
inline void SetEventFilter(EventFilter filter, void* userdata)
{
  UniqueCallbackWrapper<EventFilterCB>::erase();
  return SDL_SetEventFilter(filter, userdata);
}

/**
 * Set up a filter to process all events before they are added to the internal
 * event queue.
 *
 * If you just want to see events without modifying them or preventing them from
 * being queued, you should use AddEventWatch() instead.
 *
 * If the filter function returns true when called, then the event will be added
 * to the internal queue. If it returns false, then the event will be dropped
 * from the queue, but the internal state will still be updated. This allows
 * selective filtering of dynamically arriving events.
 *
 * **WARNING**: Be very careful of what you do in the event filter function, as
 * it may run in a different thread!
 *
 * On platforms that support it, if the quit event is generated by an interrupt
 * signal (e.g. pressing Ctrl-C), it will be delivered to the application at the
 * next event poll.
 *
 * Note: Disabled events never make it to the event filter function; see
 * SetEventEnabled().
 *
 * Note: Events pushed onto the queue with PushEvent() get passed through the
 * event filter, but events pushed onto the queue with PeepEvents() do not.
 *
 * @param filter an EventFilterCB function to call when an event happens.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @cat listener-callback
 *
 * @sa listener-callback
 * @sa AddEventWatch
 * @sa SetEventEnabled
 * @sa GetEventFilter
 * @sa PeepEvents
 * @sa PushEvent
 */
inline void SetEventFilter(EventFilterCB filter = {})
{
  using Wrapper = UniqueCallbackWrapper<EventFilterCB>;
  SDL_SetEventFilter(
    [](void* userdata, SDL_Event* event) {
      return Wrapper::Call(userdata, *event);
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
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetEventFilter
 */
inline void GetEventFilter(EventFilter* filter, void** userdata)
{
  CheckError(SDL_GetEventFilter(filter, userdata));
}

/**
 * Query the current event filter.
 *
 * This function can be used to "chain" filters, by saving the existing filter
 * before replacing it with a function that will call that saved filter.
 *
 * @returns EventFilterCB on success or false if there is no event filter
 * set.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @cat listener-callback
 *
 * @sa listener-callback
 * @sa SetEventFilter
 */
inline EventFilterCB GetEventFilter()
{
  using Wrapper = UniqueCallbackWrapper<EventFilterCB>;

  EventFilter filter;
  void* userdata;
  GetEventFilter(&filter, &userdata);
  if (!userdata)
    return [filter](const Event& event) {
      return filter(nullptr, const_cast<Event*>(&event));
    };
  if (auto cb = Wrapper::at(userdata)) return cb;
  return [filter, userdata](const Event& event) {
    return filter(userdata, const_cast<Event*>(&event));
  };
}

/// @private
inline bool EventWatchAuxCallback(void* userdata, Event* event)
{
  auto& f = *static_cast<EventFilterCB*>(userdata);
  return f(*event);
}

/**
 * Add a callback to be triggered when an event is added to the event queue.
 *
 * `filter` will be called when an event happens, and its return value is
 * ignored.
 *
 * **WARNING**: Be very careful of what you do in the event filter function, as
 * it may run in a different thread!
 *
 * If the quit event is generated by a signal (e.g. SIGINT), it will bypass the
 * internal queue and be delivered to the watch callback immediately, and arrive
 * at the next event poll.
 *
 * Note: the callback is called for events posted by the user through
 * PushEvent(), but not for disabled events, nor for events by a filter callback
 * set with SetEventFilter(), nor for events posted by the user through
 * PeepEvents().
 *
 * @param filter an EventFilter function to call when an event happens.
 * @param userdata a pointer that is passed to `filter`.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa RemoveEventWatch
 * @sa SetEventFilter
 */
inline void AddEventWatch(EventFilter filter, void* userdata)
{
  CheckError(SDL_AddEventWatch(filter, userdata));
}

/**
 * Add a callback to be triggered when an event is added to the event queue.
 *
 * `filter` will be called when an event happens, and its return value is
 * ignored.
 *
 * **WARNING**: Be very careful of what you do in the event filter function, as
 * it may run in a different thread!
 *
 * If the quit event is generated by a signal (e.g. SIGINT), it will bypass the
 * internal queue and be delivered to the watch callback immediately, and arrive
 * at the next event poll.
 *
 * Note: the callback is called for events posted by the user through
 * PushEvent(), but not for disabled events, nor for events by a filter callback
 * set with SetEventFilter(), nor for events posted by the user through
 * PeepEvents().
 *
 * @param filter an EventFilterCB to call when an event happens.
 * @returns a handle that can be used on RemoveEventWatch(EventFilterHandle) on
 *          success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @cat listener-callback
 *
 * @sa listener-callback
 * @sa RemoveEventWatch
 * @sa SetEventFilter
 */
inline EventWatchHandle AddEventWatch(EventFilterCB filter)
{
  using Wrapper = CallbackWrapper<EventFilterCB>;
  auto cb = Wrapper::Wrap(std::move(filter));
  if (!SDL_AddEventWatch(&EventWatchAuxCallback, &cb)) {
    Wrapper::release(cb);
    throw Error{};
  }
  return EventWatchHandle{cb};
}

/**
 * Remove an event watch callback added with AddEventWatch().
 *
 * This function takes the same input as AddEventWatch() to identify and delete
 * the corresponding callback.
 *
 * @param filter the function originally passed to AddEventWatch().
 * @param userdata the pointer originally passed to AddEventWatch().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddEventWatch
 */
inline void RemoveEventWatch(EventFilter filter, void* userdata)
{
  SDL_RemoveEventWatch(filter, userdata);
}

/**
 * Remove an event watch callback added with AddEventWatch().
 *
 * @param handle the handle returned by SDL_AddEventWatch(EventFilterCB).
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @cat listener-callback
 *
 * @sa listener-callback
 * @sa AddEventWatch(EventFilterCB)
 */
inline void RemoveEventWatch(EventWatchHandle handle)
{
  using Wrapper = CallbackWrapper<EventFilterCB>;
  Wrapper::release(handle);
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
 * @sa GetEventFilter
 * @sa SetEventFilter
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
 * @cat immediate-callback
 *
 * @sa immediate-callback
 * @sa GetEventFilter
 * @sa SetEventFilter
 */
inline void FilterEvents(EventFilterCB filter)
{
  return FilterEvents(
    [](void* userdata, SDL_Event* event) {
      auto& f = *static_cast<EventFilterCB*>(userdata);
      return f(*event);
    },
    &filter);
}

/**
 * Set the state of processing events by type.
 *
 * @param type the type of event; see EventType for details.
 * @param enabled whether to process the event or not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa EventEnabled
 */
inline void SetEventEnabled(Uint32 type, bool enabled)
{
  SDL_SetEventEnabled(type, enabled);
}

/**
 * Query the state of processing events by type.
 *
 * @param type the type of event; see EventType for details.
 * @returns true if the event is being processed, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetEventEnabled
 */
inline bool EventEnabled(Uint32 type) { return SDL_EventEnabled(type); }

/**
 * Allocate a set of user-defined events, and return the beginning event number
 * for that set of events.
 *
 * @param numevents the number of events to be allocated.
 * @returns the beginning event number, or 0 if numevents is invalid or if there
 *          are not enough user-defined events left.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PushEvent
 */
inline Uint32 RegisterEvents(int numevents)
{
  return SDL_RegisterEvents(numevents);
}

/**
 * Get window associated with an event.
 *
 * @param event an event containing a `windowID`.
 * @returns the associated window on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa PollEvent
 * @sa WaitEvent
 * @sa WaitEventTimeout
 */
inline WindowRef GetWindowFromEvent(const Event& event)
{
  return {CheckError(SDL_GetWindowFromEvent(&event))};
}

/// @}

} // namespace SDL

#endif /* SDL3PP_EVENTS_H_ */
