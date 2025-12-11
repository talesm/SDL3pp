#ifndef SDL3PP_GAMEPAD_H_
#define SDL3PP_GAMEPAD_H_

#include <SDL3/SDL_gamepad.h>
#include "SDL3pp_error.h"
#include "SDL3pp_guid.h"
#include "SDL3pp_iostream.h"
#include "SDL3pp_joystick.h"
#include "SDL3pp_power.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_sensor.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryGamepad Gamepad Support
 *
 * SDL provides a low-level joystick API, which just treats joysticks as an
 * arbitrary pile of buttons, axes, and hat switches. If you're planning to
 * write your own control configuration screen, this can give you a lot of
 * flexibility, but that's a lot of work, and most things that we consider
 * "joysticks" now are actually console-style gamepads. So SDL provides the
 * gamepad API on top of the lower-level joystick functionality.
 *
 * The difference between a joystick and a gamepad is that a gamepad tells you
 * _where_ a button or axis is on the device. You don't speak to gamepads in
 * terms of arbitrary numbers like "button 3" or "axis 2" but in standard
 * locations: the d-pad, the shoulder buttons, triggers, A/B/X/Y (or
 * X/O/Square/Triangle, if you will).
 *
 * One turns a joystick into a gamepad by providing a magic configuration
 * string, which tells SDL the details of a specific device: when you see this
 * specific hardware, if button 2 gets pressed, this is actually D-Pad Up, etc.
 *
 * SDL has many popular controllers configured out of the box, and users can add
 * their own controller details through an environment variable if it's
 * otherwise unknown to SDL.
 *
 * In order to use these functions, Init() must have been called with the
 * INIT_GAMEPAD flag. This causes SDL to scan the system for gamepads, and load
 * appropriate drivers.
 *
 * If you would like to receive gamepad updates while the application is in the
 * background, you should set the following hint before calling Init():
 * SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS
 *
 * Gamepads support various optional features such as rumble, color LEDs,
 * touchpad, gyro, etc. The support for these features varies depending on the
 * controller and OS support available. You can check for LED and rumble
 * capabilities at runtime by calling Gamepad.GetProperties() and checking the
 * various capability properties. You can check for touchpad by calling
 * Gamepad.GetNumTouchpads() and check for gyro and accelerometer by calling
 * Gamepad.HasSensor().
 *
 * By default SDL will try to use the most capable driver available, but you can
 * tune which OS drivers to use with the various joystick hints in SDL_hints.h.
 *
 * Your application should always support gamepad hotplugging. On some platforms
 * like Xbox, Steam Deck, etc., this is a requirement for certification. On
 * other platforms, like macOS and Windows when using Windows.Gaming.Input,
 * controllers may not be available at startup and will come in at some point
 * after you've started processing events.
 *
 * @{
 */

// Forward decl
struct Gamepad;

/// Alias to raw representation for Gamepad.
using GamepadRaw = SDL_Gamepad*;

// Forward decl
struct GamepadRef;

/// Safely wrap Gamepad for non owning parameters
struct GamepadParam
{
  GamepadRaw value; ///< parameter's GamepadRaw

  /// Constructs from GamepadRaw
  constexpr GamepadParam(GamepadRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr GamepadParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const GamepadParam& other) const = default;

  /// Converts to underlying GamepadRaw
  constexpr operator GamepadRaw() const { return value; }
};

/**
 * Standard gamepad types.
 *
 * This type does not necessarily map to first-party controllers from
 * Microsoft/Sony/Nintendo; in many cases, third-party controllers can report as
 * these, either because they were designed for a specific console, or they
 * simply most closely match that console's controllers (does it have A/B/X/Y
 * buttons or X/O/Square/Triangle? Does it have a touchpad? etc).
 */
using GamepadType = SDL_GamepadType;

constexpr GamepadType GAMEPAD_TYPE_UNKNOWN =
  SDL_GAMEPAD_TYPE_UNKNOWN; ///< GAMEPAD_TYPE_UNKNOWN

constexpr GamepadType GAMEPAD_TYPE_STANDARD =
  SDL_GAMEPAD_TYPE_STANDARD; ///< GAMEPAD_TYPE_STANDARD

constexpr GamepadType GAMEPAD_TYPE_XBOX360 =
  SDL_GAMEPAD_TYPE_XBOX360; ///< GAMEPAD_TYPE_XBOX360

constexpr GamepadType GAMEPAD_TYPE_XBOXONE =
  SDL_GAMEPAD_TYPE_XBOXONE; ///< GAMEPAD_TYPE_XBOXONE

constexpr GamepadType GAMEPAD_TYPE_PS3 =
  SDL_GAMEPAD_TYPE_PS3; ///< GAMEPAD_TYPE_PS3

constexpr GamepadType GAMEPAD_TYPE_PS4 =
  SDL_GAMEPAD_TYPE_PS4; ///< GAMEPAD_TYPE_PS4

constexpr GamepadType GAMEPAD_TYPE_PS5 =
  SDL_GAMEPAD_TYPE_PS5; ///< GAMEPAD_TYPE_PS5

constexpr GamepadType GAMEPAD_TYPE_NINTENDO_SWITCH_PRO =
  SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO; ///< GAMEPAD_TYPE_NINTENDO_SWITCH_PRO

constexpr GamepadType GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT =
  SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT; ///< GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT

constexpr GamepadType GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT =
  SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT; ///< GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT

constexpr GamepadType GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR =
  SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR; ///< GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR

#if SDL_VERSION_ATLEAST(3, 3, 2)

constexpr GamepadType GAMEPAD_TYPE_GAMECUBE =
  SDL_GAMEPAD_TYPE_GAMECUBE; ///< GAMEPAD_TYPE_GAMECUBE

#endif // SDL_VERSION_ATLEAST(3, 3, 2)

constexpr GamepadType GAMEPAD_TYPE_COUNT =
  SDL_GAMEPAD_TYPE_COUNT; ///< GAMEPAD_TYPE_COUNT

/**
 * The list of buttons available on a gamepad
 *
 * For controllers that use a diamond pattern for the face buttons, the
 * south/east/west/north buttons below correspond to the locations in the
 * diamond pattern. For Xbox controllers, this would be A/B/X/Y, for Nintendo
 * Switch controllers, this would be B/A/Y/X, for GameCube controllers this
 * would be A/X/B/Y, for PlayStation controllers this would be
 * Cross/Circle/Square/Triangle.
 *
 * For controllers that don't use a diamond pattern for the face buttons, the
 * south/east/west/north buttons indicate the buttons labeled A, B, C, D, or 1,
 * 2, 3, 4, or for controllers that aren't labeled, they are the primary,
 * secondary, etc. buttons.
 *
 * The activate action is often the south button and the cancel action is often
 * the east button, but in some regions this is reversed, so your game should
 * allow remapping actions based on user preferences.
 *
 * You can query the labels for the face buttons using Gamepad.GetButtonLabel()
 *
 * @since This enum is available since SDL 3.2.0.
 */
using GamepadButton = SDL_GamepadButton;

constexpr GamepadButton GAMEPAD_BUTTON_INVALID =
  SDL_GAMEPAD_BUTTON_INVALID; ///< GAMEPAD_BUTTON_INVALID

constexpr GamepadButton GAMEPAD_BUTTON_SOUTH =
  SDL_GAMEPAD_BUTTON_SOUTH; ///< Bottom face button (e.g. Xbox A button)

constexpr GamepadButton GAMEPAD_BUTTON_EAST =
  SDL_GAMEPAD_BUTTON_EAST; ///< Right face button (e.g. Xbox B button)

constexpr GamepadButton GAMEPAD_BUTTON_WEST =
  SDL_GAMEPAD_BUTTON_WEST; ///< Left face button (e.g. Xbox X button)

constexpr GamepadButton GAMEPAD_BUTTON_NORTH =
  SDL_GAMEPAD_BUTTON_NORTH; ///< Top face button (e.g. Xbox Y button)

constexpr GamepadButton GAMEPAD_BUTTON_BACK =
  SDL_GAMEPAD_BUTTON_BACK; ///< GAMEPAD_BUTTON_BACK

constexpr GamepadButton GAMEPAD_BUTTON_GUIDE =
  SDL_GAMEPAD_BUTTON_GUIDE; ///< GAMEPAD_BUTTON_GUIDE

constexpr GamepadButton GAMEPAD_BUTTON_START =
  SDL_GAMEPAD_BUTTON_START; ///< GAMEPAD_BUTTON_START

constexpr GamepadButton GAMEPAD_BUTTON_LEFT_STICK =
  SDL_GAMEPAD_BUTTON_LEFT_STICK; ///< GAMEPAD_BUTTON_LEFT_STICK

constexpr GamepadButton GAMEPAD_BUTTON_RIGHT_STICK =
  SDL_GAMEPAD_BUTTON_RIGHT_STICK; ///< GAMEPAD_BUTTON_RIGHT_STICK

constexpr GamepadButton GAMEPAD_BUTTON_LEFT_SHOULDER =
  SDL_GAMEPAD_BUTTON_LEFT_SHOULDER; ///< GAMEPAD_BUTTON_LEFT_SHOULDER

constexpr GamepadButton GAMEPAD_BUTTON_RIGHT_SHOULDER =
  SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER; ///< GAMEPAD_BUTTON_RIGHT_SHOULDER

constexpr GamepadButton GAMEPAD_BUTTON_DPAD_UP =
  SDL_GAMEPAD_BUTTON_DPAD_UP; ///< GAMEPAD_BUTTON_DPAD_UP

constexpr GamepadButton GAMEPAD_BUTTON_DPAD_DOWN =
  SDL_GAMEPAD_BUTTON_DPAD_DOWN; ///< GAMEPAD_BUTTON_DPAD_DOWN

constexpr GamepadButton GAMEPAD_BUTTON_DPAD_LEFT =
  SDL_GAMEPAD_BUTTON_DPAD_LEFT; ///< GAMEPAD_BUTTON_DPAD_LEFT

constexpr GamepadButton GAMEPAD_BUTTON_DPAD_RIGHT =
  SDL_GAMEPAD_BUTTON_DPAD_RIGHT; ///< GAMEPAD_BUTTON_DPAD_RIGHT

/**
 * Additional button (e.g. Xbox Series X share button, PS5 microphone button,
 * Nintendo Switch Pro capture button, Amazon Luna microphone button, Google
 * Stadia capture button)
 */
constexpr GamepadButton GAMEPAD_BUTTON_MISC1 = SDL_GAMEPAD_BUTTON_MISC1;

/**
 * Upper or primary paddle, under your right hand (e.g. Xbox Elite paddle P1,
 * DualSense Edge RB button, Right Joy-Con SR button)
 */
constexpr GamepadButton GAMEPAD_BUTTON_RIGHT_PADDLE1 =
  SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1;

/**
 * Upper or primary paddle, under your left hand (e.g. Xbox Elite paddle P3,
 * DualSense Edge LB button, Left Joy-Con SL button)
 */
constexpr GamepadButton GAMEPAD_BUTTON_LEFT_PADDLE1 =
  SDL_GAMEPAD_BUTTON_LEFT_PADDLE1;

/**
 * Lower or secondary paddle, under your right hand (e.g. Xbox Elite paddle P2,
 * DualSense Edge right Fn button, Right Joy-Con SL button)
 */
constexpr GamepadButton GAMEPAD_BUTTON_RIGHT_PADDLE2 =
  SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2;

/**
 * Lower or secondary paddle, under your left hand (e.g. Xbox Elite paddle P4,
 * DualSense Edge left Fn button, Left Joy-Con SR button)
 */
constexpr GamepadButton GAMEPAD_BUTTON_LEFT_PADDLE2 =
  SDL_GAMEPAD_BUTTON_LEFT_PADDLE2;

constexpr GamepadButton GAMEPAD_BUTTON_TOUCHPAD =
  SDL_GAMEPAD_BUTTON_TOUCHPAD; ///< PS4/PS5 touchpad button

constexpr GamepadButton GAMEPAD_BUTTON_MISC2 =
  SDL_GAMEPAD_BUTTON_MISC2; ///< Additional button

/// Additional button (e.g. Nintendo GameCube left trigger click)
constexpr GamepadButton GAMEPAD_BUTTON_MISC3 = SDL_GAMEPAD_BUTTON_MISC3;

/// Additional button (e.g. Nintendo GameCube right trigger click)
constexpr GamepadButton GAMEPAD_BUTTON_MISC4 = SDL_GAMEPAD_BUTTON_MISC4;

constexpr GamepadButton GAMEPAD_BUTTON_MISC5 =
  SDL_GAMEPAD_BUTTON_MISC5; ///< Additional button

constexpr GamepadButton GAMEPAD_BUTTON_MISC6 =
  SDL_GAMEPAD_BUTTON_MISC6; ///< Additional button

constexpr GamepadButton GAMEPAD_BUTTON_COUNT =
  SDL_GAMEPAD_BUTTON_COUNT; ///< GAMEPAD_BUTTON_COUNT

/**
 * The set of gamepad button labels
 *
 * This isn't a complete set, just the face buttons to make it easy to show
 * button prompts.
 *
 * For a complete set, you should look at the button and gamepad type and have a
 * set of symbols that work well with your art style.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using GamepadButtonLabel = SDL_GamepadButtonLabel;

constexpr GamepadButtonLabel GAMEPAD_BUTTON_LABEL_UNKNOWN =
  SDL_GAMEPAD_BUTTON_LABEL_UNKNOWN; ///< GAMEPAD_BUTTON_LABEL_UNKNOWN

constexpr GamepadButtonLabel GAMEPAD_BUTTON_LABEL_A =
  SDL_GAMEPAD_BUTTON_LABEL_A; ///< GAMEPAD_BUTTON_LABEL_A

constexpr GamepadButtonLabel GAMEPAD_BUTTON_LABEL_B =
  SDL_GAMEPAD_BUTTON_LABEL_B; ///< GAMEPAD_BUTTON_LABEL_B

constexpr GamepadButtonLabel GAMEPAD_BUTTON_LABEL_X =
  SDL_GAMEPAD_BUTTON_LABEL_X; ///< GAMEPAD_BUTTON_LABEL_X

constexpr GamepadButtonLabel GAMEPAD_BUTTON_LABEL_Y =
  SDL_GAMEPAD_BUTTON_LABEL_Y; ///< GAMEPAD_BUTTON_LABEL_Y

constexpr GamepadButtonLabel GAMEPAD_BUTTON_LABEL_CROSS =
  SDL_GAMEPAD_BUTTON_LABEL_CROSS; ///< GAMEPAD_BUTTON_LABEL_CROSS

constexpr GamepadButtonLabel GAMEPAD_BUTTON_LABEL_CIRCLE =
  SDL_GAMEPAD_BUTTON_LABEL_CIRCLE; ///< GAMEPAD_BUTTON_LABEL_CIRCLE

constexpr GamepadButtonLabel GAMEPAD_BUTTON_LABEL_SQUARE =
  SDL_GAMEPAD_BUTTON_LABEL_SQUARE; ///< GAMEPAD_BUTTON_LABEL_SQUARE

constexpr GamepadButtonLabel GAMEPAD_BUTTON_LABEL_TRIANGLE =
  SDL_GAMEPAD_BUTTON_LABEL_TRIANGLE; ///< GAMEPAD_BUTTON_LABEL_TRIANGLE

/**
 * The list of axes available on a gamepad
 *
 * Thumbstick axis values range from JOYSTICK_AXIS_MIN to JOYSTICK_AXIS_MAX, and
 * are centered within ~8000 of zero, though advanced UI will allow users to set
 * or autodetect the dead zone, which varies between gamepads.
 *
 * Trigger axis values range from 0 (released) to JOYSTICK_AXIS_MAX (fully
 * pressed) when reported by Gamepad.GetAxis(). Note that this is not the same
 * range that will be reported by the lower-level Joystick.GetAxis().
 *
 * @since This enum is available since SDL 3.2.0.
 */
using GamepadAxis = SDL_GamepadAxis;

constexpr GamepadAxis GAMEPAD_AXIS_INVALID =
  SDL_GAMEPAD_AXIS_INVALID; ///< GAMEPAD_AXIS_INVALID

constexpr GamepadAxis GAMEPAD_AXIS_LEFTX =
  SDL_GAMEPAD_AXIS_LEFTX; ///< GAMEPAD_AXIS_LEFTX

constexpr GamepadAxis GAMEPAD_AXIS_LEFTY =
  SDL_GAMEPAD_AXIS_LEFTY; ///< GAMEPAD_AXIS_LEFTY

constexpr GamepadAxis GAMEPAD_AXIS_RIGHTX =
  SDL_GAMEPAD_AXIS_RIGHTX; ///< GAMEPAD_AXIS_RIGHTX

constexpr GamepadAxis GAMEPAD_AXIS_RIGHTY =
  SDL_GAMEPAD_AXIS_RIGHTY; ///< GAMEPAD_AXIS_RIGHTY

constexpr GamepadAxis GAMEPAD_AXIS_LEFT_TRIGGER =
  SDL_GAMEPAD_AXIS_LEFT_TRIGGER; ///< GAMEPAD_AXIS_LEFT_TRIGGER

constexpr GamepadAxis GAMEPAD_AXIS_RIGHT_TRIGGER =
  SDL_GAMEPAD_AXIS_RIGHT_TRIGGER; ///< GAMEPAD_AXIS_RIGHT_TRIGGER

constexpr GamepadAxis GAMEPAD_AXIS_COUNT =
  SDL_GAMEPAD_AXIS_COUNT; ///< GAMEPAD_AXIS_COUNT

/**
 * Types of gamepad control bindings.
 *
 * A gamepad is a collection of bindings that map arbitrary joystick buttons,
 * axes and hat switches to specific positions on a generic console-style
 * gamepad. This enum is used as part of GamepadBinding to specify those
 * mappings.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using GamepadBindingType = SDL_GamepadBindingType;

constexpr GamepadBindingType GAMEPAD_BINDTYPE_NONE =
  SDL_GAMEPAD_BINDTYPE_NONE; ///< GAMEPAD_BINDTYPE_NONE

constexpr GamepadBindingType GAMEPAD_BINDTYPE_BUTTON =
  SDL_GAMEPAD_BINDTYPE_BUTTON; ///< GAMEPAD_BINDTYPE_BUTTON

constexpr GamepadBindingType GAMEPAD_BINDTYPE_AXIS =
  SDL_GAMEPAD_BINDTYPE_AXIS; ///< GAMEPAD_BINDTYPE_AXIS

constexpr GamepadBindingType GAMEPAD_BINDTYPE_HAT =
  SDL_GAMEPAD_BINDTYPE_HAT; ///< GAMEPAD_BINDTYPE_HAT

/**
 * A mapping between one joystick input to a gamepad control.
 *
 * A gamepad has a collection of several bindings, to say, for example, when
 * joystick button number 5 is pressed, that should be treated like the
 * gamepad's "start" button.
 *
 * SDL has these bindings built-in for many popular controllers, and can add
 * more with a simple text string. Those strings are parsed into a collection of
 * these structs to make it easier to operate on the data.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetBindings
 */
using GamepadBinding = SDL_GamepadBinding;

/**
 * The structure used to identify an SDL gamepad
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class Gamepad
{
  GamepadRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Gamepad(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from GamepadParam.
   *
   * @param resource a GamepadRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Gamepad(const GamepadRaw resource) noexcept
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Gamepad(const Gamepad& other) = delete;

  /// Move constructor
  constexpr Gamepad(Gamepad&& other) noexcept
    : Gamepad(other.release())
  {
  }

  constexpr Gamepad(const GamepadRef& other) = delete;

  constexpr Gamepad(GamepadRef&& other) = delete;

  /**
   * Open a gamepad for use.
   *
   * @param instance_id the joystick instance ID.
   * @post a gamepad identifier or nullptr if an error occurred; call GetError()
   *       for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.Close
   * @sa IsGamepad
   */
  Gamepad(JoystickID instance_id)
    : m_resource(SDL_OpenGamepad(instance_id))
  {
  }

  /// Destructor
  ~Gamepad() { SDL_CloseGamepad(m_resource); }

  /// Assignment operator.
  constexpr Gamepad& operator=(Gamepad&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Gamepad& operator=(const Gamepad& other) noexcept = default;

public:
  /// Retrieves underlying GamepadRaw.
  constexpr GamepadRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying GamepadRaw and clear this.
  constexpr GamepadRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Gamepad& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to GamepadParam
  constexpr operator GamepadParam() const noexcept { return {m_resource}; }

  /**
   * Close a gamepad previously opened with Gamepad.Gamepad().
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.Gamepad
   */
  void Close();

  /**
   * Get the current mapping of a gamepad.
   *
   * Details about mappings are discussed with AddGamepadMapping().
   *
   * @returns a string that has the gamepad's mapping or nullptr if no mapping
   *          is available; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AddGamepadMapping
   * @sa GetGamepadMappingForID
   * @sa GetGamepadMappingForGUID
   * @sa SetGamepadMapping
   */
  StringResult GetMapping();

  /**
   * Get the properties associated with an opened gamepad.
   *
   * These properties are shared with the underlying joystick object.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `prop::GamepadCap.MONO_LED_BOOLEAN`: true if this gamepad has an LED that
   *   has adjustable brightness
   * - `prop::GamepadCap.RGB_LED_BOOLEAN`: true if this gamepad has an LED that
   *   has adjustable color
   * - `prop::GamepadCap.PLAYER_LED_BOOLEAN`: true if this gamepad has a player
   *   LED
   * - `prop::GamepadCap.RUMBLE_BOOLEAN`: true if this gamepad has left/right
   *   rumble
   * - `prop::GamepadCap.TRIGGER_RUMBLE_BOOLEAN`: true if this gamepad has
   *   simple trigger rumble
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties();

  /**
   * Get the instance ID of an opened gamepad.
   *
   * @returns the instance ID of the specified gamepad on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  JoystickID GetID();

  /**
   * Get the implementation-dependent name for an opened gamepad.
   *
   * @returns the implementation dependent name for the gamepad, or nullptr if
   *          there is no name or the identifier passed is invalid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetGamepadNameForID
   */
  const char* GetName();

  /**
   * Get the implementation-dependent path for an opened gamepad.
   *
   * @returns the implementation dependent path for the gamepad, or nullptr if
   *          there is no path or the identifier passed is invalid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetGamepadPathForID
   */
  const char* GetPath();

  /**
   * Get the type of an opened gamepad.
   *
   * @returns the gamepad type, or GAMEPAD_TYPE_UNKNOWN if it's not available.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetGamepadTypeForID
   */
  GamepadType GetType();

  /**
   * Get the type of an opened gamepad, ignoring any mapping override.
   *
   * @returns the gamepad type, or GAMEPAD_TYPE_UNKNOWN if it's not available.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetRealGamepadTypeForID
   */
  GamepadType GetRealType();

  /**
   * Get the player index of an opened gamepad.
   *
   * For XInput gamepads this returns the XInput user index.
   *
   * @returns the player index for gamepad, or -1 if it's not available.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.SetPlayerIndex
   */
  int GetPlayerIndex();

  /**
   * Set the player index of an opened gamepad.
   *
   * @param player_index player index to assign to this gamepad, or -1 to clear
   *                     the player index and turn off player LEDs.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.GetPlayerIndex
   */
  void SetPlayerIndex(int player_index);

  /**
   * Get the USB vendor ID of an opened gamepad, if available.
   *
   * If the vendor ID isn't available this function returns 0.
   *
   * @returns the USB vendor ID, or zero if unavailable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetGamepadVendorForID
   */
  Uint16 GetVendor();

  /**
   * Get the USB product ID of an opened gamepad, if available.
   *
   * If the product ID isn't available this function returns 0.
   *
   * @returns the USB product ID, or zero if unavailable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetGamepadProductForID
   */
  Uint16 GetProduct();

  /**
   * Get the product version of an opened gamepad, if available.
   *
   * If the product version isn't available this function returns 0.
   *
   * @returns the USB product version, or zero if unavailable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetGamepadProductVersionForID
   */
  Uint16 GetProductVersion();

  /**
   * Get the firmware version of an opened gamepad, if available.
   *
   * If the firmware version isn't available this function returns 0.
   *
   * @returns the gamepad firmware version, or zero if unavailable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint16 GetFirmwareVersion();

  /**
   * Get the serial number of an opened gamepad, if available.
   *
   * Returns the serial number of the gamepad, or nullptr if it is not
   * available.
   *
   * @returns the serial number, or nullptr if unavailable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  const char* GetSerial();

  /**
   * Get the Steam Input handle of an opened gamepad, if available.
   *
   * Returns an InputHandle_t for the gamepad that can be used with Steam Input
   * API: https://partner.steamgames.com/doc/api/ISteamInput
   *
   * @returns the gamepad handle, or 0 if unavailable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint64 GetSteamHandle();

  /**
   * Get the connection state of a gamepad.
   *
   * @returns the connection state on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  JoystickConnectionState GetConnectionState();

  /**
   * Get the battery state of a gamepad.
   *
   * You should never take a battery status as absolute truth. Batteries
   * (especially failing batteries) are delicate hardware, and the values
   * reported here are best estimates based on what that hardware reports. It's
   * not uncommon for older batteries to lose stored power much faster than it
   * reports, or completely drain when reporting it has 20 percent left, etc.
   *
   * @param percent a pointer filled in with the percentage of battery life
   *                left, between 0 and 100, or nullptr to ignore. This will be
   *                filled in with -1 we can't determine a value or there is no
   *                battery.
   * @returns the current battery state.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PowerState GetPowerInfo(int* percent);

  /**
   * Check if a gamepad has been opened and is currently connected.
   *
   * @returns true if the gamepad has been opened and is currently connected, or
   *          false if not.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Connected();

  /**
   * Get the underlying joystick from a gamepad.
   *
   * This function will give you a Joystick object, which allows you to use the
   * Joystick functions with a Gamepad object. This would be useful for getting
   * a joystick's position at any given time, even if it hasn't moved (moving it
   * would produce an event, which would have the axis' value).
   *
   * The pointer returned is owned by the Gamepad. You should not call
   * Joystick.Close() on it, for example, since doing so will likely cause SDL
   * to crash.
   *
   * @returns an Joystick object, or nullptr on failure; call GetError() for
   *          more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  JoystickRef GetJoystick();

  /**
   * Get the SDL joystick layer bindings for a gamepad.
   *
   * @param count a pointer filled in with the number of bindings returned.
   * @returns a nullptr terminated array of pointers to bindings or nullptr on
   *          failure; call GetError() for more information. This is a single
   *          allocation that should be freed with free() when it is no longer
   *          needed.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  SDL_GamepadBinding** GetBindings(int* count);

  /**
   * Query whether a gamepad has a given axis.
   *
   * This merely reports whether the gamepad's mapping defined this axis, as
   * that is all the information SDL has about the physical device.
   *
   * @param axis an axis enum value (an GamepadAxis value).
   * @returns true if the gamepad has this axis, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.HasButton
   * @sa Gamepad.GetAxis
   */
  bool HasAxis(GamepadAxis axis);

  /**
   * Get the current state of an axis control on a gamepad.
   *
   * The axis indices start at index 0.
   *
   * For thumbsticks, the state is a value ranging from -32768 (up/left) to
   * 32767 (down/right).
   *
   * Triggers range from 0 when released to 32767 when fully pressed, and never
   * return a negative value. Note that this differs from the value reported by
   * the lower-level Joystick.GetAxis(), which normally uses the full range.
   *
   * Note that for invalid gamepads or axes, this will return 0. Zero is also a
   * valid value in normal operation; usually it means a centered axis.
   *
   * @param axis an axis index (one of the GamepadAxis values).
   * @returns axis state.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.HasAxis
   * @sa Gamepad.GetButton
   */
  Sint16 GetAxis(GamepadAxis axis);

  /**
   * Query whether a gamepad has a given button.
   *
   * This merely reports whether the gamepad's mapping defined this button, as
   * that is all the information SDL has about the physical device.
   *
   * @param button a button enum value (an GamepadButton value).
   * @returns true if the gamepad has this button, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.HasAxis
   */
  bool HasButton(GamepadButton button);

  /**
   * Get the current state of a button on a gamepad.
   *
   * @param button a button index (one of the GamepadButton values).
   * @returns true if the button is pressed, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.HasButton
   * @sa Gamepad.GetAxis
   */
  bool GetButton(GamepadButton button);

  /**
   * Get the label of a button on a gamepad.
   *
   * @param button a button index (one of the GamepadButton values).
   * @returns the GamepadButtonLabel enum corresponding to the button label.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetGamepadButtonLabelForType
   */
  GamepadButtonLabel GetButtonLabel(GamepadButton button);

  /**
   * Get the number of touchpads on a gamepad.
   *
   * @returns number of touchpads.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.GetNumTouchpadFingers
   */
  int GetNumTouchpads();

  /**
   * Get the number of supported simultaneous fingers on a touchpad on a game
   * gamepad.
   *
   * @param touchpad a touchpad.
   * @returns number of supported simultaneous fingers.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.GetTouchpadFinger
   * @sa Gamepad.GetNumTouchpads
   */
  int GetNumTouchpadFingers(int touchpad);

  /**
   * Get the current state of a finger on a touchpad on a gamepad.
   *
   * @param touchpad a touchpad.
   * @param finger a finger.
   * @param down a pointer filled with true if the finger is down, false
   *             otherwise, may be nullptr.
   * @param x a pointer filled with the x position, normalized 0 to 1, with the
   *          origin in the upper left, may be nullptr.
   * @param y a pointer filled with the y position, normalized 0 to 1, with the
   *          origin in the upper left, may be nullptr.
   * @param pressure a pointer filled with pressure value, may be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.GetNumTouchpadFingers
   */
  void GetTouchpadFinger(int touchpad,
                         int finger,
                         bool* down,
                         float* x,
                         float* y,
                         float* pressure);

  /**
   * Return whether a gamepad has a particular sensor.
   *
   * @param type the type of sensor to query.
   * @returns true if the sensor exists, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.GetSensorData
   * @sa Gamepad.GetSensorDataRate
   * @sa Gamepad.SetSensorEnabled
   */
  bool HasSensor(SensorType type);

  /**
   * Set whether data reporting for a gamepad sensor is enabled.
   *
   * @param type the type of sensor to enable/disable.
   * @param enabled whether data reporting should be enabled.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.HasSensor
   * @sa Gamepad.SensorEnabled
   */
  void SetSensorEnabled(SensorType type, bool enabled);

  /**
   * Query whether sensor data reporting is enabled for a gamepad.
   *
   * @param type the type of sensor to query.
   * @returns true if the sensor is enabled, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.SetSensorEnabled
   */
  bool SensorEnabled(SensorType type);

  /**
   * Get the data rate (number of events per second) of a gamepad sensor.
   *
   * @param type the type of sensor to query.
   * @returns the data rate, or 0.0f if the data rate is not available.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  float GetSensorDataRate(SensorType type);

  /**
   * Get the current state of a gamepad sensor.
   *
   * The number of values and interpretation of the data is sensor dependent.
   * See SDL_sensor.h for the details for each type of sensor.
   *
   * @param type the type of sensor to query.
   * @param data a pointer filled with the current sensor state.
   * @param num_values the number of values to write to data.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void GetSensorData(SensorType type, float* data, int num_values);

  /**
   * Start a rumble effect on a gamepad.
   *
   * Each call to this function cancels any previous rumble effect, and calling
   * it with 0 intensity stops any rumbling.
   *
   * This function requires you to process SDL events or call UpdateJoysticks()
   * to update rumble state.
   *
   * @param low_frequency_rumble the intensity of the low frequency (left)
   *                             rumble motor, from 0 to 0xFFFF.
   * @param high_frequency_rumble the intensity of the high frequency (right)
   *                              rumble motor, from 0 to 0xFFFF.
   * @param duration_ms the duration of the rumble effect, in milliseconds.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Rumble(Uint16 low_frequency_rumble,
              Uint16 high_frequency_rumble,
              Uint32 duration_ms);

  /**
   * Start a rumble effect in the gamepad's triggers.
   *
   * Each call to this function cancels any previous trigger rumble effect, and
   * calling it with 0 intensity stops any rumbling.
   *
   * Note that this is rumbling of the _triggers_ and not the gamepad as a
   * whole. This is currently only supported on Xbox One gamepads. If you want
   * the (more common) whole-gamepad rumble, use Gamepad.Rumble() instead.
   *
   * This function requires you to process SDL events or call UpdateJoysticks()
   * to update rumble state.
   *
   * @param left_rumble the intensity of the left trigger rumble motor, from 0
   *                    to 0xFFFF.
   * @param right_rumble the intensity of the right trigger rumble motor, from 0
   *                     to 0xFFFF.
   * @param duration_ms the duration of the rumble effect, in milliseconds.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.Rumble
   */
  void RumbleTriggers(Uint16 left_rumble,
                      Uint16 right_rumble,
                      Uint32 duration_ms);

  /**
   * Update a gamepad's LED color.
   *
   * An example of a joystick LED is the light on the back of a PlayStation 4's
   * DualShock 4 controller.
   *
   * For gamepads with a single color LED, the maximum of the RGB values will be
   * used as the LED brightness.
   *
   * @param red the intensity of the red LED.
   * @param green the intensity of the green LED.
   * @param blue the intensity of the blue LED.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetLED(Uint8 red, Uint8 green, Uint8 blue);

  /**
   * Send a gamepad specific effect packet.
   *
   * @param data the data to send to the gamepad.
   * @param size the size of the data to send to the gamepad.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SendEffect(const void* data, int size);

  /**
   * Return the sfSymbolsName for a given button on a gamepad on Apple
   * platforms.
   *
   * @param button a button on the gamepad.
   * @returns the sfSymbolsName or nullptr if the name can't be found.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.GetAppleSFSymbolsNameForAxis
   */
  const char* GetAppleSFSymbolsNameForButton(GamepadButton button);

  /**
   * Return the sfSymbolsName for a given axis on a gamepad on Apple platforms.
   *
   * @param axis an axis on the gamepad.
   * @returns the sfSymbolsName or nullptr if the name can't be found.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Gamepad.GetAppleSFSymbolsNameForButton
   */
  const char* GetAppleSFSymbolsNameForAxis(GamepadAxis axis);
};

/// Semi-safe reference for Gamepad.
struct GamepadRef : Gamepad
{
  using Gamepad::Gamepad;

  /**
   * Constructs from GamepadParam.
   *
   * @param resource a GamepadRaw or Gamepad.
   *
   * This does not takes ownership!
   */
  GamepadRef(GamepadParam resource) noexcept
    : Gamepad(resource.value)
  {
  }

  /**
   * Constructs from GamepadParam.
   *
   * @param resource a GamepadRaw or Gamepad.
   *
   * This does not takes ownership!
   */
  GamepadRef(GamepadRaw resource) noexcept
    : Gamepad(resource)
  {
  }

  /// Copy constructor.
  GamepadRef(const GamepadRef& other) noexcept
    : Gamepad(other.get())
  {
  }

  /// Destructor
  ~GamepadRef() { release(); }
};

/**
 * Add support for gamepads that SDL is unaware of or change the binding of an
 * existing gamepad.
 *
 * The mapping string has the format "GUID,name,mapping", where GUID is the
 * string value from GUID.ToString(), name is the human readable string for the
 * device and mappings are gamepad mappings to joystick ones. Under Windows
 * there is a reserved GUID of "xinput" that covers all XInput devices. The
 * mapping format for joystick is:
 *
 * - `bX`: a joystick button, index X
 * - `hX.Y`: hat X with value Y
 * - `aX`: axis X of the joystick
 *
 * Buttons can be used as a gamepad axes and vice versa.
 *
 * If a device with this GUID is already plugged in, SDL will generate an
 * EVENT_GAMEPAD_ADDED event.
 *
 * This string shows an example of a valid mapping for a gamepad:
 *
 * ```c
 * "341a3608000000000000504944564944,Afterglow PS3
 * Controller,a:b1,b:b2,y:b3,x:b0,start:b9,guide:b12,back:b8,dpup:h0.1,dpleft:h0.8,dpdown:h0.4,dpright:h0.2,leftshoulder:b4,rightshoulder:b5,leftstick:b10,rightstick:b11,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:b6,righttrigger:b7"
 * ```
 *
 * @param mapping the mapping string.
 * @returns 1 if a new mapping is added, 0 if an existing mapping is updated, -1
 *          on failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddGamepadMappingsFromFile
 * @sa AddGamepadMappingsFromIO
 * @sa Gamepad.GetMapping
 * @sa GetGamepadMappingForGUID
 * @sa SDL_HINT_GAMECONTROLLERCONFIG
 * @sa SDL_HINT_GAMECONTROLLERCONFIG_FILE
 * @sa EVENT_GAMEPAD_ADDED
 */
inline int AddGamepadMapping(StringParam mapping)
{
  return SDL_AddGamepadMapping(mapping);
}

/**
 * Load a set of gamepad mappings from an IOStream.
 *
 * You can call this function several times, if needed, to load different
 * database files.
 *
 * If a new mapping is loaded for an already known gamepad GUID, the later
 * version will overwrite the one currently loaded.
 *
 * Any new mappings for already plugged in controllers will generate
 * EVENT_GAMEPAD_ADDED events.
 *
 * Mappings not belonging to the current platform or with no platform field
 * specified will be ignored (i.e. mappings for Linux will be ignored in
 * Windows, etc).
 *
 * This function will load the text database entirely in memory before
 * processing it, so take this into consideration if you are in a memory
 * constrained environment.
 *
 * @param src the data stream for the mappings to be added.
 * @param closeio if true, calls IOStream.Close() on `src` before returning,
 *                even in the case of an error.
 * @returns the number of mappings added or -1 on failure; call GetError() for
 *          more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddGamepadMapping
 * @sa AddGamepadMappingsFromFile
 * @sa Gamepad.GetMapping
 * @sa GetGamepadMappingForGUID
 * @sa SDL_HINT_GAMECONTROLLERCONFIG
 * @sa SDL_HINT_GAMECONTROLLERCONFIG_FILE
 * @sa EVENT_GAMEPAD_ADDED
 */
inline int AddGamepadMappingsFromIO(IOStreamParam src, bool closeio)
{
  return SDL_AddGamepadMappingsFromIO(src, closeio);
}

/**
 * Load a set of gamepad mappings from a file.
 *
 * You can call this function several times, if needed, to load different
 * database files.
 *
 * If a new mapping is loaded for an already known gamepad GUID, the later
 * version will overwrite the one currently loaded.
 *
 * Any new mappings for already plugged in controllers will generate
 * EVENT_GAMEPAD_ADDED events.
 *
 * Mappings not belonging to the current platform or with no platform field
 * specified will be ignored (i.e. mappings for Linux will be ignored in
 * Windows, etc).
 *
 * @param file the mappings file to load.
 * @returns the number of mappings added or -1 on failure; call GetError() for
 *          more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddGamepadMapping
 * @sa AddGamepadMappingsFromIO
 * @sa Gamepad.GetMapping
 * @sa GetGamepadMappingForGUID
 * @sa SDL_HINT_GAMECONTROLLERCONFIG
 * @sa SDL_HINT_GAMECONTROLLERCONFIG_FILE
 * @sa EVENT_GAMEPAD_ADDED
 */
inline int AddGamepadMappingsFromFile(StringParam file)
{
  return SDL_AddGamepadMappingsFromFile(file);
}

/**
 * Reinitialize the SDL mapping database to its initial state.
 *
 * This will generate gamepad events as needed if device mappings change.
 *
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void ReloadGamepadMappings() { CheckError(SDL_ReloadGamepadMappings()); }

/**
 * Get the current gamepad mappings.
 *
 * @returns an array of the mapping strings, nullptr-terminated, or nullptr on
 *          failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline OwnArray<char*> GetGamepadMappings()
{
  int count;
  auto data = SDL_GetGamepadMappings(&count);
  return OwnArray<char*>(data);
}

/**
 * Get the gamepad mapping string for a given GUID.
 *
 * @param guid a structure containing the GUID for which a mapping is desired.
 * @returns a mapping string or nullptr on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa JoystickID.GetJoystickGUIDForID
 * @sa Joystick.GetGUID
 */
inline StringResult GetGamepadMappingForGUID(GUID guid)
{
  return StringResult(SDL_GetGamepadMappingForGUID(guid));
}

/**
 * Get the current mapping of a gamepad.
 *
 * Details about mappings are discussed with AddGamepadMapping().
 *
 * @param gamepad the gamepad you want to get the current mapping for.
 * @returns a string that has the gamepad's mapping or nullptr if no mapping is
 *          available; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddGamepadMapping
 * @sa GetGamepadMappingForID
 * @sa GetGamepadMappingForGUID
 * @sa SetGamepadMapping
 */
inline StringResult GetGamepadMapping(GamepadParam gamepad)
{
  return StringResult(SDL_GetGamepadMapping(gamepad));
}

inline StringResult Gamepad::GetMapping()
{
  return SDL::GetGamepadMapping(m_resource);
}

/**
 * Set the current mapping of a joystick or gamepad.
 *
 * Details about mappings are discussed with AddGamepadMapping().
 *
 * @param instance_id the joystick instance ID.
 * @param mapping the mapping to use for this device, or nullptr to clear the
 *                mapping.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AddGamepadMapping
 * @sa Gamepad.GetMapping
 */
inline void SetGamepadMapping(JoystickID instance_id, StringParam mapping)
{
  CheckError(SDL_SetGamepadMapping(instance_id, mapping));
}

/**
 * Return whether a gamepad is currently connected.
 *
 * @returns true if a gamepad is connected, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepads
 */
inline bool HasGamepad() { return SDL_HasGamepad(); }

/**
 * Get a list of currently connected gamepads.
 *
 * @returns a 0 terminated array of joystick instance IDs or nullptr on failure;
 *          call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasGamepad
 * @sa Gamepad.Gamepad
 */
inline OwnArray<JoystickID> GetGamepads()
{
  int count;
  auto r = reinterpret_cast<JoystickID*>(SDL_GetGamepads(&count));
  return OwnArray<JoystickID>(r, count);
}

/**
 * Check if the given joystick is supported by the gamepad interface.
 *
 * @param instance_id the joystick instance ID.
 * @returns true if the given joystick is supported by the gamepad interface,
 *          false if it isn't or it's an invalid index.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetJoysticks
 * @sa Gamepad.Gamepad
 */
inline bool IsGamepad(JoystickID instance_id)
{
  return SDL_IsGamepad(instance_id);
}

/**
 * Get the implementation dependent name of a gamepad.
 *
 * This can be called before any gamepads are opened.
 *
 * @param instance_id the joystick instance ID.
 * @returns the name of the selected gamepad. If no name can be found, this
 *          function returns nullptr; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetName
 * @sa GetGamepads
 */
inline const char* GetGamepadNameForID(JoystickID instance_id)
{
  return SDL_GetGamepadNameForID(instance_id);
}

/**
 * Get the implementation dependent path of a gamepad.
 *
 * This can be called before any gamepads are opened.
 *
 * @param instance_id the joystick instance ID.
 * @returns the path of the selected gamepad. If no path can be found, this
 *          function returns nullptr; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetPath
 * @sa GetGamepads
 */
inline const char* GetGamepadPathForID(JoystickID instance_id)
{
  return SDL_GetGamepadPathForID(instance_id);
}

/**
 * Get the player index of a gamepad.
 *
 * This can be called before any gamepads are opened.
 *
 * @param instance_id the joystick instance ID.
 * @returns the player index of a gamepad, or -1 if it's not available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetPlayerIndex
 * @sa GetGamepads
 */
inline int GetGamepadPlayerIndexForID(JoystickID instance_id)
{
  return SDL_GetGamepadPlayerIndexForID(instance_id);
}

/**
 * Get the implementation-dependent GUID of a gamepad.
 *
 * This can be called before any gamepads are opened.
 *
 * @param instance_id the joystick instance ID.
 * @returns the GUID of the selected gamepad. If called on an invalid index,
 *          this function returns a zero GUID.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GUID.ToString
 * @sa GetGamepads
 */
inline GUID GetGamepadGUIDForID(JoystickID instance_id)
{
  return SDL_GetGamepadGUIDForID(instance_id);
}

/**
 * Get the USB vendor ID of a gamepad, if available.
 *
 * This can be called before any gamepads are opened. If the vendor ID isn't
 * available this function returns 0.
 *
 * @param instance_id the joystick instance ID.
 * @returns the USB vendor ID of the selected gamepad. If called on an invalid
 *          index, this function returns zero.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetVendor
 * @sa GetGamepads
 */
inline Uint16 GetGamepadVendorForID(JoystickID instance_id)
{
  return SDL_GetGamepadVendorForID(instance_id);
}

/**
 * Get the USB product ID of a gamepad, if available.
 *
 * This can be called before any gamepads are opened. If the product ID isn't
 * available this function returns 0.
 *
 * @param instance_id the joystick instance ID.
 * @returns the USB product ID of the selected gamepad. If called on an invalid
 *          index, this function returns zero.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetProduct
 * @sa GetGamepads
 */
inline Uint16 GetGamepadProductForID(JoystickID instance_id)
{
  return SDL_GetGamepadProductForID(instance_id);
}

/**
 * Get the product version of a gamepad, if available.
 *
 * This can be called before any gamepads are opened. If the product version
 * isn't available this function returns 0.
 *
 * @param instance_id the joystick instance ID.
 * @returns the product version of the selected gamepad. If called on an invalid
 *          index, this function returns zero.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetProductVersion
 * @sa GetGamepads
 */
inline Uint16 GetGamepadProductVersionForID(JoystickID instance_id)
{
  return SDL_GetGamepadProductVersionForID(instance_id);
}

/**
 * Get the type of a gamepad.
 *
 * This can be called before any gamepads are opened.
 *
 * @param instance_id the joystick instance ID.
 * @returns the gamepad type.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetType
 * @sa GetGamepads
 * @sa GetRealGamepadTypeForID
 */
inline GamepadType GetGamepadTypeForID(JoystickID instance_id)
{
  return SDL_GetGamepadTypeForID(instance_id);
}

/**
 * Get the type of a gamepad, ignoring any mapping override.
 *
 * This can be called before any gamepads are opened.
 *
 * @param instance_id the joystick instance ID.
 * @returns the gamepad type.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadTypeForID
 * @sa GetGamepads
 * @sa Gamepad.GetRealType
 */
inline GamepadType GetRealGamepadTypeForID(JoystickID instance_id)
{
  return SDL_GetRealGamepadTypeForID(instance_id);
}

/**
 * Get the mapping of a gamepad.
 *
 * This can be called before any gamepads are opened.
 *
 * @param instance_id the joystick instance ID.
 * @returns the mapping string. Returns nullptr if no mapping is available. This
 *          should be freed with free() when it is no longer needed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepads
 * @sa Gamepad.GetMapping
 */
inline char* GetGamepadMappingForID(JoystickID instance_id)
{
  return SDL_GetGamepadMappingForID(instance_id);
}

/**
 * Open a gamepad for use.
 *
 * @param instance_id the joystick instance ID.
 * @returns a gamepad identifier or nullptr if an error occurred; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.Close
 * @sa IsGamepad
 */
inline Gamepad OpenGamepad(JoystickID instance_id)
{
  return Gamepad(instance_id);
}

/**
 * Get the Gamepad associated with a joystick instance ID, if it has been
 * opened.
 *
 * @param instance_id the joystick instance ID of the gamepad.
 * @returns an Gamepad on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline GamepadRef GetGamepadFromID(JoystickID instance_id)
{
  return {CheckError(SDL_GetGamepadFromID(instance_id))};
}

/**
 * Get the Gamepad associated with a player index.
 *
 * @param player_index the player index, which different from the instance ID.
 * @returns the Gamepad associated with a player index.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetPlayerIndex
 * @sa Gamepad.SetPlayerIndex
 */
inline GamepadRef GetGamepadFromPlayerIndex(int player_index)
{
  return {SDL_GetGamepadFromPlayerIndex(player_index)};
}

/**
 * Get the properties associated with an opened gamepad.
 *
 * These properties are shared with the underlying joystick object.
 *
 * The following read-only properties are provided by SDL:
 *
 * - `prop::GamepadCap.MONO_LED_BOOLEAN`: true if this gamepad has an LED that
 *   has adjustable brightness
 * - `prop::GamepadCap.RGB_LED_BOOLEAN`: true if this gamepad has an LED that
 *   has adjustable color
 * - `prop::GamepadCap.PLAYER_LED_BOOLEAN`: true if this gamepad has a player
 *   LED
 * - `prop::GamepadCap.RUMBLE_BOOLEAN`: true if this gamepad has left/right
 *   rumble
 * - `prop::GamepadCap.TRIGGER_RUMBLE_BOOLEAN`: true if this gamepad has simple
 *   trigger rumble
 *
 * @param gamepad a gamepad identifier previously returned by Gamepad.Gamepad().
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesRef GetGamepadProperties(GamepadParam gamepad)
{
  return {CheckError(SDL_GetGamepadProperties(gamepad))};
}

inline PropertiesRef Gamepad::GetProperties()
{
  return SDL::GetGamepadProperties(m_resource);
}

namespace prop::GamepadCap {

constexpr auto MONO_LED_BOOLEAN = SDL_PROP_GAMEPAD_CAP_MONO_LED_BOOLEAN;

constexpr auto RGB_LED_BOOLEAN = SDL_PROP_GAMEPAD_CAP_RGB_LED_BOOLEAN;

constexpr auto PLAYER_LED_BOOLEAN = SDL_PROP_GAMEPAD_CAP_PLAYER_LED_BOOLEAN;

constexpr auto RUMBLE_BOOLEAN = SDL_PROP_GAMEPAD_CAP_RUMBLE_BOOLEAN;

constexpr auto TRIGGER_RUMBLE_BOOLEAN =
  SDL_PROP_GAMEPAD_CAP_TRIGGER_RUMBLE_BOOLEAN;

} // namespace prop::GamepadCap

/**
 * Get the instance ID of an opened gamepad.
 *
 * @param gamepad a gamepad identifier previously returned by Gamepad.Gamepad().
 * @returns the instance ID of the specified gamepad on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline JoystickID GetGamepadID(GamepadParam gamepad)
{
  return CheckError(SDL_GetGamepadID(gamepad));
}

inline JoystickID Gamepad::GetID() { return SDL::GetGamepadID(m_resource); }

/**
 * Get the implementation-dependent name for an opened gamepad.
 *
 * @param gamepad a gamepad identifier previously returned by Gamepad.Gamepad().
 * @returns the implementation dependent name for the gamepad, or nullptr if
 *          there is no name or the identifier passed is invalid.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadNameForID
 */
inline const char* GetGamepadName(GamepadParam gamepad)
{
  return SDL_GetGamepadName(gamepad);
}

inline const char* Gamepad::GetName()
{
  return SDL::GetGamepadName(m_resource);
}

/**
 * Get the implementation-dependent path for an opened gamepad.
 *
 * @param gamepad a gamepad identifier previously returned by Gamepad.Gamepad().
 * @returns the implementation dependent path for the gamepad, or nullptr if
 *          there is no path or the identifier passed is invalid.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadPathForID
 */
inline const char* GetGamepadPath(GamepadParam gamepad)
{
  return SDL_GetGamepadPath(gamepad);
}

inline const char* Gamepad::GetPath()
{
  return SDL::GetGamepadPath(m_resource);
}

/**
 * Get the type of an opened gamepad.
 *
 * @param gamepad the gamepad object to query.
 * @returns the gamepad type, or GAMEPAD_TYPE_UNKNOWN if it's not available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadTypeForID
 */
inline GamepadType GetGamepadType(GamepadParam gamepad)
{
  return SDL_GetGamepadType(gamepad);
}

inline GamepadType Gamepad::GetType()
{
  return SDL::GetGamepadType(m_resource);
}

/**
 * Get the type of an opened gamepad, ignoring any mapping override.
 *
 * @param gamepad the gamepad object to query.
 * @returns the gamepad type, or GAMEPAD_TYPE_UNKNOWN if it's not available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetRealGamepadTypeForID
 */
inline GamepadType GetRealGamepadType(GamepadParam gamepad)
{
  return SDL_GetRealGamepadType(gamepad);
}

inline GamepadType Gamepad::GetRealType()
{
  return SDL::GetRealGamepadType(m_resource);
}

/**
 * Get the player index of an opened gamepad.
 *
 * For XInput gamepads this returns the XInput user index.
 *
 * @param gamepad the gamepad object to query.
 * @returns the player index for gamepad, or -1 if it's not available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.SetPlayerIndex
 */
inline int GetGamepadPlayerIndex(GamepadParam gamepad)
{
  return SDL_GetGamepadPlayerIndex(gamepad);
}

inline int Gamepad::GetPlayerIndex()
{
  return SDL::GetGamepadPlayerIndex(m_resource);
}

/**
 * Set the player index of an opened gamepad.
 *
 * @param gamepad the gamepad object to adjust.
 * @param player_index player index to assign to this gamepad, or -1 to clear
 *                     the player index and turn off player LEDs.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetPlayerIndex
 */
inline void SetGamepadPlayerIndex(GamepadParam gamepad, int player_index)
{
  CheckError(SDL_SetGamepadPlayerIndex(gamepad, player_index));
}

inline void Gamepad::SetPlayerIndex(int player_index)
{
  SDL::SetGamepadPlayerIndex(m_resource, player_index);
}

/**
 * Get the USB vendor ID of an opened gamepad, if available.
 *
 * If the vendor ID isn't available this function returns 0.
 *
 * @param gamepad the gamepad object to query.
 * @returns the USB vendor ID, or zero if unavailable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadVendorForID
 */
inline Uint16 GetGamepadVendor(GamepadParam gamepad)
{
  return SDL_GetGamepadVendor(gamepad);
}

inline Uint16 Gamepad::GetVendor() { return SDL::GetGamepadVendor(m_resource); }

/**
 * Get the USB product ID of an opened gamepad, if available.
 *
 * If the product ID isn't available this function returns 0.
 *
 * @param gamepad the gamepad object to query.
 * @returns the USB product ID, or zero if unavailable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadProductForID
 */
inline Uint16 GetGamepadProduct(GamepadParam gamepad)
{
  return SDL_GetGamepadProduct(gamepad);
}

inline Uint16 Gamepad::GetProduct()
{
  return SDL::GetGamepadProduct(m_resource);
}

/**
 * Get the product version of an opened gamepad, if available.
 *
 * If the product version isn't available this function returns 0.
 *
 * @param gamepad the gamepad object to query.
 * @returns the USB product version, or zero if unavailable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadProductVersionForID
 */
inline Uint16 GetGamepadProductVersion(GamepadParam gamepad)
{
  return SDL_GetGamepadProductVersion(gamepad);
}

inline Uint16 Gamepad::GetProductVersion()
{
  return SDL::GetGamepadProductVersion(m_resource);
}

/**
 * Get the firmware version of an opened gamepad, if available.
 *
 * If the firmware version isn't available this function returns 0.
 *
 * @param gamepad the gamepad object to query.
 * @returns the gamepad firmware version, or zero if unavailable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint16 GetGamepadFirmwareVersion(GamepadParam gamepad)
{
  return SDL_GetGamepadFirmwareVersion(gamepad);
}

inline Uint16 Gamepad::GetFirmwareVersion()
{
  return SDL::GetGamepadFirmwareVersion(m_resource);
}

/**
 * Get the serial number of an opened gamepad, if available.
 *
 * Returns the serial number of the gamepad, or nullptr if it is not available.
 *
 * @param gamepad the gamepad object to query.
 * @returns the serial number, or nullptr if unavailable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetGamepadSerial(GamepadParam gamepad)
{
  return SDL_GetGamepadSerial(gamepad);
}

inline const char* Gamepad::GetSerial()
{
  return SDL::GetGamepadSerial(m_resource);
}

/**
 * Get the Steam Input handle of an opened gamepad, if available.
 *
 * Returns an InputHandle_t for the gamepad that can be used with Steam Input
 * API: https://partner.steamgames.com/doc/api/ISteamInput
 *
 * @param gamepad the gamepad object to query.
 * @returns the gamepad handle, or 0 if unavailable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint64 GetGamepadSteamHandle(GamepadParam gamepad)
{
  return SDL_GetGamepadSteamHandle(gamepad);
}

inline Uint64 Gamepad::GetSteamHandle()
{
  return SDL::GetGamepadSteamHandle(m_resource);
}

/**
 * Get the connection state of a gamepad.
 *
 * @param gamepad the gamepad object to query.
 * @returns the connection state on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline JoystickConnectionState GetGamepadConnectionState(GamepadParam gamepad)
{
  return CheckError(SDL_GetGamepadConnectionState(gamepad));
}

inline JoystickConnectionState Gamepad::GetConnectionState()
{
  return SDL::GetGamepadConnectionState(m_resource);
}

/**
 * Get the battery state of a gamepad.
 *
 * You should never take a battery status as absolute truth. Batteries
 * (especially failing batteries) are delicate hardware, and the values reported
 * here are best estimates based on what that hardware reports. It's not
 * uncommon for older batteries to lose stored power much faster than it
 * reports, or completely drain when reporting it has 20 percent left, etc.
 *
 * @param gamepad the gamepad object to query.
 * @param percent a pointer filled in with the percentage of battery life left,
 *                between 0 and 100, or nullptr to ignore. This will be filled
 *                in with -1 we can't determine a value or there is no battery.
 * @returns the current battery state.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PowerState GetGamepadPowerInfo(GamepadParam gamepad, int* percent)
{
  return SDL_GetGamepadPowerInfo(gamepad, percent);
}

inline PowerState Gamepad::GetPowerInfo(int* percent)
{
  return SDL::GetGamepadPowerInfo(m_resource, percent);
}

/**
 * Check if a gamepad has been opened and is currently connected.
 *
 * @param gamepad a gamepad identifier previously returned by Gamepad.Gamepad().
 * @returns true if the gamepad has been opened and is currently connected, or
 *          false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GamepadConnected(GamepadParam gamepad)
{
  return SDL_GamepadConnected(gamepad);
}

inline bool Gamepad::Connected() { return SDL::GamepadConnected(m_resource); }

/**
 * Get the underlying joystick from a gamepad.
 *
 * This function will give you a Joystick object, which allows you to use the
 * Joystick functions with a Gamepad object. This would be useful for getting a
 * joystick's position at any given time, even if it hasn't moved (moving it
 * would produce an event, which would have the axis' value).
 *
 * The pointer returned is owned by the Gamepad. You should not call
 * Joystick.Close() on it, for example, since doing so will likely cause SDL to
 * crash.
 *
 * @param gamepad the gamepad object that you want to get a joystick from.
 * @returns an Joystick object, or nullptr on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline JoystickRef GetGamepadJoystick(GamepadParam gamepad)
{
  return {SDL_GetGamepadJoystick(gamepad)};
}

inline JoystickRef Gamepad::GetJoystick()
{
  return SDL::GetGamepadJoystick(m_resource);
}

/**
 * Set the state of gamepad event processing.
 *
 * If gamepad events are disabled, you must call UpdateGamepads() yourself and
 * check the state of the gamepad when you want gamepad information.
 *
 * @param enabled whether to process gamepad events or not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GamepadEventsEnabled
 * @sa UpdateGamepads
 */
inline void SetGamepadEventsEnabled(bool enabled)
{
  SDL_SetGamepadEventsEnabled(enabled);
}

/**
 * Query the state of gamepad event processing.
 *
 * If gamepad events are disabled, you must call UpdateGamepads() yourself and
 * check the state of the gamepad when you want gamepad information.
 *
 * @returns true if gamepad events are being processed, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetGamepadEventsEnabled
 */
inline bool GamepadEventsEnabled() { return SDL_GamepadEventsEnabled(); }

/**
 * Get the SDL joystick layer bindings for a gamepad.
 *
 * @param gamepad a gamepad.
 * @param count a pointer filled in with the number of bindings returned.
 * @returns a nullptr terminated array of pointers to bindings or nullptr on
 *          failure; call GetError() for more information. This is a single
 *          allocation that should be freed with free() when it is no longer
 *          needed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline SDL_GamepadBinding** GetGamepadBindings(GamepadParam gamepad, int* count)
{
  return SDL_GetGamepadBindings(gamepad, count);
}

inline SDL_GamepadBinding** Gamepad::GetBindings(int* count)
{
  return SDL::GetGamepadBindings(m_resource, count);
}

/**
 * Manually pump gamepad updates if not using the loop.
 *
 * This function is called automatically by the event loop if events are
 * enabled. Under such circumstances, it will not be necessary to call this
 * function.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void UpdateGamepads() { SDL_UpdateGamepads(); }

/**
 * Convert a string into GamepadType enum.
 *
 * This function is called internally to translate Gamepad mapping strings for
 * the underlying joystick device into the consistent Gamepad mapping. You do
 * not normally need to call this function unless you are parsing Gamepad
 * mappings in your own code.
 *
 * @param str string representing a GamepadType type.
 * @returns the GamepadType enum corresponding to the input string, or
 *          `GAMEPAD_TYPE_UNKNOWN` if no match was found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadStringForType
 */
inline GamepadType GetGamepadTypeFromString(StringParam str)
{
  return SDL_GetGamepadTypeFromString(str);
}

/**
 * Convert from an GamepadType enum to a string.
 *
 * @param type an enum value for a given GamepadType.
 * @returns a string for the given type, or nullptr if an invalid type is
 *          specified. The string returned is of the format used by Gamepad
 *          mapping strings.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadTypeFromString
 */
inline const char* GetGamepadStringForType(GamepadType type)
{
  return SDL_GetGamepadStringForType(type);
}

/**
 * Convert a string into GamepadAxis enum.
 *
 * This function is called internally to translate Gamepad mapping strings for
 * the underlying joystick device into the consistent Gamepad mapping. You do
 * not normally need to call this function unless you are parsing Gamepad
 * mappings in your own code.
 *
 * Note specially that "righttrigger" and "lefttrigger" map to
 * `GAMEPAD_AXIS_RIGHT_TRIGGER` and `GAMEPAD_AXIS_LEFT_TRIGGER`, respectively.
 *
 * @param str string representing a Gamepad axis.
 * @returns the GamepadAxis enum corresponding to the input string, or
 *          `GAMEPAD_AXIS_INVALID` if no match was found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadStringForAxis
 */
inline GamepadAxis GetGamepadAxisFromString(StringParam str)
{
  return SDL_GetGamepadAxisFromString(str);
}

/**
 * Convert from an GamepadAxis enum to a string.
 *
 * @param axis an enum value for a given GamepadAxis.
 * @returns a string for the given axis, or nullptr if an invalid axis is
 *          specified. The string returned is of the format used by Gamepad
 *          mapping strings.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadAxisFromString
 */
inline const char* GetGamepadStringForAxis(GamepadAxis axis)
{
  return SDL_GetGamepadStringForAxis(axis);
}

/**
 * Query whether a gamepad has a given axis.
 *
 * This merely reports whether the gamepad's mapping defined this axis, as that
 * is all the information SDL has about the physical device.
 *
 * @param gamepad a gamepad.
 * @param axis an axis enum value (an GamepadAxis value).
 * @returns true if the gamepad has this axis, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.HasButton
 * @sa Gamepad.GetAxis
 */
inline bool GamepadHasAxis(GamepadParam gamepad, GamepadAxis axis)
{
  return SDL_GamepadHasAxis(gamepad, axis);
}

inline bool Gamepad::HasAxis(GamepadAxis axis)
{
  return SDL::GamepadHasAxis(m_resource, axis);
}

/**
 * Get the current state of an axis control on a gamepad.
 *
 * The axis indices start at index 0.
 *
 * For thumbsticks, the state is a value ranging from -32768 (up/left) to 32767
 * (down/right).
 *
 * Triggers range from 0 when released to 32767 when fully pressed, and never
 * return a negative value. Note that this differs from the value reported by
 * the lower-level Joystick.GetAxis(), which normally uses the full range.
 *
 * Note that for invalid gamepads or axes, this will return 0. Zero is also a
 * valid value in normal operation; usually it means a centered axis.
 *
 * @param gamepad a gamepad.
 * @param axis an axis index (one of the GamepadAxis values).
 * @returns axis state.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.HasAxis
 * @sa Gamepad.GetButton
 */
inline Sint16 GetGamepadAxis(GamepadParam gamepad, GamepadAxis axis)
{
  return SDL_GetGamepadAxis(gamepad, axis);
}

inline Sint16 Gamepad::GetAxis(GamepadAxis axis)
{
  return SDL::GetGamepadAxis(m_resource, axis);
}

/**
 * Convert a string into an GamepadButton enum.
 *
 * This function is called internally to translate Gamepad mapping strings for
 * the underlying joystick device into the consistent Gamepad mapping. You do
 * not normally need to call this function unless you are parsing Gamepad
 * mappings in your own code.
 *
 * @param str string representing a Gamepad axis.
 * @returns the GamepadButton enum corresponding to the input string, or
 *          `GAMEPAD_BUTTON_INVALID` if no match was found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadStringForButton
 */
inline GamepadButton GetGamepadButtonFromString(StringParam str)
{
  return SDL_GetGamepadButtonFromString(str);
}

/**
 * Convert from an GamepadButton enum to a string.
 *
 * @param button an enum value for a given GamepadButton.
 * @returns a string for the given button, or nullptr if an invalid button is
 *          specified. The string returned is of the format used by Gamepad
 *          mapping strings.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadButtonFromString
 */
inline const char* GetGamepadStringForButton(GamepadButton button)
{
  return SDL_GetGamepadStringForButton(button);
}

/**
 * Query whether a gamepad has a given button.
 *
 * This merely reports whether the gamepad's mapping defined this button, as
 * that is all the information SDL has about the physical device.
 *
 * @param gamepad a gamepad.
 * @param button a button enum value (an GamepadButton value).
 * @returns true if the gamepad has this button, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.HasAxis
 */
inline bool GamepadHasButton(GamepadParam gamepad, GamepadButton button)
{
  return SDL_GamepadHasButton(gamepad, button);
}

inline bool Gamepad::HasButton(GamepadButton button)
{
  return SDL::GamepadHasButton(m_resource, button);
}

/**
 * Get the current state of a button on a gamepad.
 *
 * @param gamepad a gamepad.
 * @param button a button index (one of the GamepadButton values).
 * @returns true if the button is pressed, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.HasButton
 * @sa Gamepad.GetAxis
 */
inline bool GetGamepadButton(GamepadParam gamepad, GamepadButton button)
{
  return SDL_GetGamepadButton(gamepad, button);
}

inline bool Gamepad::GetButton(GamepadButton button)
{
  return SDL::GetGamepadButton(m_resource, button);
}

/**
 * Get the label of a button on a gamepad.
 *
 * @param type the type of gamepad to check.
 * @param button a button index (one of the GamepadButton values).
 * @returns the GamepadButtonLabel enum corresponding to the button label.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetButtonLabel
 */
inline GamepadButtonLabel GetGamepadButtonLabelForType(GamepadType type,
                                                       GamepadButton button)
{
  return SDL_GetGamepadButtonLabelForType(type, button);
}

/**
 * Get the label of a button on a gamepad.
 *
 * @param gamepad a gamepad.
 * @param button a button index (one of the GamepadButton values).
 * @returns the GamepadButtonLabel enum corresponding to the button label.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetGamepadButtonLabelForType
 */
inline GamepadButtonLabel GetGamepadButtonLabel(GamepadParam gamepad,
                                                GamepadButton button)
{
  return SDL_GetGamepadButtonLabel(gamepad, button);
}

inline GamepadButtonLabel Gamepad::GetButtonLabel(GamepadButton button)
{
  return SDL::GetGamepadButtonLabel(m_resource, button);
}

/**
 * Get the number of touchpads on a gamepad.
 *
 * @param gamepad a gamepad.
 * @returns number of touchpads.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetNumTouchpadFingers
 */
inline int GetNumGamepadTouchpads(GamepadParam gamepad)
{
  return SDL_GetNumGamepadTouchpads(gamepad);
}

inline int Gamepad::GetNumTouchpads()
{
  return SDL::GetNumGamepadTouchpads(m_resource);
}

/**
 * Get the number of supported simultaneous fingers on a touchpad on a game
 * gamepad.
 *
 * @param gamepad a gamepad.
 * @param touchpad a touchpad.
 * @returns number of supported simultaneous fingers.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetTouchpadFinger
 * @sa Gamepad.GetNumTouchpads
 */
inline int GetNumGamepadTouchpadFingers(GamepadParam gamepad, int touchpad)
{
  return SDL_GetNumGamepadTouchpadFingers(gamepad, touchpad);
}

inline int Gamepad::GetNumTouchpadFingers(int touchpad)
{
  return SDL::GetNumGamepadTouchpadFingers(m_resource, touchpad);
}

/**
 * Get the current state of a finger on a touchpad on a gamepad.
 *
 * @param gamepad a gamepad.
 * @param touchpad a touchpad.
 * @param finger a finger.
 * @param down a pointer filled with true if the finger is down, false
 *             otherwise, may be nullptr.
 * @param x a pointer filled with the x position, normalized 0 to 1, with the
 *          origin in the upper left, may be nullptr.
 * @param y a pointer filled with the y position, normalized 0 to 1, with the
 *          origin in the upper left, may be nullptr.
 * @param pressure a pointer filled with pressure value, may be nullptr.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetNumTouchpadFingers
 */
inline void GetGamepadTouchpadFinger(GamepadParam gamepad,
                                     int touchpad,
                                     int finger,
                                     bool* down,
                                     float* x,
                                     float* y,
                                     float* pressure)
{
  CheckError(SDL_GetGamepadTouchpadFinger(
    gamepad, touchpad, finger, down, x, y, pressure));
}

inline void Gamepad::GetTouchpadFinger(int touchpad,
                                       int finger,
                                       bool* down,
                                       float* x,
                                       float* y,
                                       float* pressure)
{
  SDL::GetGamepadTouchpadFinger(
    m_resource, touchpad, finger, down, x, y, pressure);
}

/**
 * Return whether a gamepad has a particular sensor.
 *
 * @param gamepad the gamepad to query.
 * @param type the type of sensor to query.
 * @returns true if the sensor exists, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetSensorData
 * @sa Gamepad.GetSensorDataRate
 * @sa Gamepad.SetSensorEnabled
 */
inline bool GamepadHasSensor(GamepadParam gamepad, SensorType type)
{
  return SDL_GamepadHasSensor(gamepad, type);
}

inline bool Gamepad::HasSensor(SensorType type)
{
  return SDL::GamepadHasSensor(m_resource, type);
}

/**
 * Set whether data reporting for a gamepad sensor is enabled.
 *
 * @param gamepad the gamepad to update.
 * @param type the type of sensor to enable/disable.
 * @param enabled whether data reporting should be enabled.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.HasSensor
 * @sa Gamepad.SensorEnabled
 */
inline void SetGamepadSensorEnabled(GamepadParam gamepad,
                                    SensorType type,
                                    bool enabled)
{
  CheckError(SDL_SetGamepadSensorEnabled(gamepad, type, enabled));
}

inline void Gamepad::SetSensorEnabled(SensorType type, bool enabled)
{
  SDL::SetGamepadSensorEnabled(m_resource, type, enabled);
}

/**
 * Query whether sensor data reporting is enabled for a gamepad.
 *
 * @param gamepad the gamepad to query.
 * @param type the type of sensor to query.
 * @returns true if the sensor is enabled, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.SetSensorEnabled
 */
inline bool GamepadSensorEnabled(GamepadParam gamepad, SensorType type)
{
  return SDL_GamepadSensorEnabled(gamepad, type);
}

inline bool Gamepad::SensorEnabled(SensorType type)
{
  return SDL::GamepadSensorEnabled(m_resource, type);
}

/**
 * Get the data rate (number of events per second) of a gamepad sensor.
 *
 * @param gamepad the gamepad to query.
 * @param type the type of sensor to query.
 * @returns the data rate, or 0.0f if the data rate is not available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline float GetGamepadSensorDataRate(GamepadParam gamepad, SensorType type)
{
  return SDL_GetGamepadSensorDataRate(gamepad, type);
}

inline float Gamepad::GetSensorDataRate(SensorType type)
{
  return SDL::GetGamepadSensorDataRate(m_resource, type);
}

/**
 * Get the current state of a gamepad sensor.
 *
 * The number of values and interpretation of the data is sensor dependent. See
 * SDL_sensor.h for the details for each type of sensor.
 *
 * @param gamepad the gamepad to query.
 * @param type the type of sensor to query.
 * @param data a pointer filled with the current sensor state.
 * @param num_values the number of values to write to data.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void GetGamepadSensorData(GamepadParam gamepad,
                                 SensorType type,
                                 float* data,
                                 int num_values)
{
  CheckError(SDL_GetGamepadSensorData(gamepad, type, data, num_values));
}

inline void Gamepad::GetSensorData(SensorType type, float* data, int num_values)
{
  SDL::GetGamepadSensorData(m_resource, type, data, num_values);
}

/**
 * Start a rumble effect on a gamepad.
 *
 * Each call to this function cancels any previous rumble effect, and calling it
 * with 0 intensity stops any rumbling.
 *
 * This function requires you to process SDL events or call UpdateJoysticks() to
 * update rumble state.
 *
 * @param gamepad the gamepad to vibrate.
 * @param low_frequency_rumble the intensity of the low frequency (left) rumble
 *                             motor, from 0 to 0xFFFF.
 * @param high_frequency_rumble the intensity of the high frequency (right)
 *                              rumble motor, from 0 to 0xFFFF.
 * @param duration_ms the duration of the rumble effect, in milliseconds.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void RumbleGamepad(GamepadParam gamepad,
                          Uint16 low_frequency_rumble,
                          Uint16 high_frequency_rumble,
                          Uint32 duration_ms)
{
  CheckError(SDL_RumbleGamepad(
    gamepad, low_frequency_rumble, high_frequency_rumble, duration_ms));
}

inline void Gamepad::Rumble(Uint16 low_frequency_rumble,
                            Uint16 high_frequency_rumble,
                            Uint32 duration_ms)
{
  SDL::RumbleGamepad(
    m_resource, low_frequency_rumble, high_frequency_rumble, duration_ms);
}

/**
 * Start a rumble effect in the gamepad's triggers.
 *
 * Each call to this function cancels any previous trigger rumble effect, and
 * calling it with 0 intensity stops any rumbling.
 *
 * Note that this is rumbling of the _triggers_ and not the gamepad as a whole.
 * This is currently only supported on Xbox One gamepads. If you want the (more
 * common) whole-gamepad rumble, use Gamepad.Rumble() instead.
 *
 * This function requires you to process SDL events or call UpdateJoysticks() to
 * update rumble state.
 *
 * @param gamepad the gamepad to vibrate.
 * @param left_rumble the intensity of the left trigger rumble motor, from 0 to
 *                    0xFFFF.
 * @param right_rumble the intensity of the right trigger rumble motor, from 0
 *                     to 0xFFFF.
 * @param duration_ms the duration of the rumble effect, in milliseconds.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.Rumble
 */
inline void RumbleGamepadTriggers(GamepadParam gamepad,
                                  Uint16 left_rumble,
                                  Uint16 right_rumble,
                                  Uint32 duration_ms)
{
  CheckError(
    SDL_RumbleGamepadTriggers(gamepad, left_rumble, right_rumble, duration_ms));
}

inline void Gamepad::RumbleTriggers(Uint16 left_rumble,
                                    Uint16 right_rumble,
                                    Uint32 duration_ms)
{
  SDL::RumbleGamepadTriggers(
    m_resource, left_rumble, right_rumble, duration_ms);
}

/**
 * Update a gamepad's LED color.
 *
 * An example of a joystick LED is the light on the back of a PlayStation 4's
 * DualShock 4 controller.
 *
 * For gamepads with a single color LED, the maximum of the RGB values will be
 * used as the LED brightness.
 *
 * @param gamepad the gamepad to update.
 * @param red the intensity of the red LED.
 * @param green the intensity of the green LED.
 * @param blue the intensity of the blue LED.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void SetGamepadLED(GamepadParam gamepad,
                          Uint8 red,
                          Uint8 green,
                          Uint8 blue)
{
  CheckError(SDL_SetGamepadLED(gamepad, red, green, blue));
}

inline void Gamepad::SetLED(Uint8 red, Uint8 green, Uint8 blue)
{
  SDL::SetGamepadLED(m_resource, red, green, blue);
}

/**
 * Send a gamepad specific effect packet.
 *
 * @param gamepad the gamepad to affect.
 * @param data the data to send to the gamepad.
 * @param size the size of the data to send to the gamepad.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void SendGamepadEffect(GamepadParam gamepad, const void* data, int size)
{
  CheckError(SDL_SendGamepadEffect(gamepad, data, size));
}

inline void Gamepad::SendEffect(const void* data, int size)
{
  SDL::SendGamepadEffect(m_resource, data, size);
}

/**
 * Close a gamepad previously opened with Gamepad.Gamepad().
 *
 * @param gamepad a gamepad identifier previously returned by Gamepad.Gamepad().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.Gamepad
 */
inline void CloseGamepad(GamepadRaw gamepad) { SDL_CloseGamepad(gamepad); }

inline void Gamepad::Close() { CloseGamepad(release()); }

/**
 * Return the sfSymbolsName for a given button on a gamepad on Apple platforms.
 *
 * @param gamepad the gamepad to query.
 * @param button a button on the gamepad.
 * @returns the sfSymbolsName or nullptr if the name can't be found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetAppleSFSymbolsNameForAxis
 */
inline const char* GetGamepadAppleSFSymbolsNameForButton(GamepadParam gamepad,
                                                         GamepadButton button)
{
  return SDL_GetGamepadAppleSFSymbolsNameForButton(gamepad, button);
}

inline const char* Gamepad::GetAppleSFSymbolsNameForButton(GamepadButton button)
{
  return SDL::GetGamepadAppleSFSymbolsNameForButton(m_resource, button);
}

/**
 * Return the sfSymbolsName for a given axis on a gamepad on Apple platforms.
 *
 * @param gamepad the gamepad to query.
 * @param axis an axis on the gamepad.
 * @returns the sfSymbolsName or nullptr if the name can't be found.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Gamepad.GetAppleSFSymbolsNameForButton
 */
inline const char* GetGamepadAppleSFSymbolsNameForAxis(GamepadParam gamepad,
                                                       GamepadAxis axis)
{
  return SDL_GetGamepadAppleSFSymbolsNameForAxis(gamepad, axis);
}

inline const char* Gamepad::GetAppleSFSymbolsNameForAxis(GamepadAxis axis)
{
  return SDL::GetGamepadAppleSFSymbolsNameForAxis(m_resource, axis);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_GAMEPAD_H_ */
