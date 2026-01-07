#ifndef SDL3PP_JOYSTICK_H_
#define SDL3PP_JOYSTICK_H_

#include <SDL3/SDL_joystick.h>
#include "SDL3pp_error.h"
#include "SDL3pp_guid.h"
#include "SDL3pp_mutex.h"
#include "SDL3pp_power.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_rect.h"
#include "SDL3pp_sensor.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryJoystick Joystick Support
 *
 * SDL joystick support.
 *
 * This is the lower-level joystick handling. If you want the simpler option,
 * where what each button does is well-defined, you should use the gamepad API
 * instead.
 *
 * The term "instance_id" is the current instantiation of a joystick device in
 * the system, if the joystick is removed and then re-inserted then it will get
 * a new instance_id, instance_id's are monotonically increasing identifiers of
 * a joystick plugged in.
 *
 * The term "player_index" is the number assigned to a player on a specific
 * controller. For XInput controllers this returns the XInput user index. Many
 * joysticks will not be able to supply this information.
 *
 * GUID is used as a stable 128-bit identifier for a joystick device that does
 * not change over time. It identifies class of the device (a X360 wired
 * controller for example). This identifier is platform dependent.
 *
 * In order to use these functions, Init() must have been called with the
 * INIT_JOYSTICK flag. This causes SDL to scan the system for joysticks, and
 * load appropriate drivers.
 *
 * If you would like to receive joystick updates while the application is in the
 * background, you should set the following hint before calling Init():
 * SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS
 *
 * SDL can provide virtual joysticks as well: the app defines an imaginary
 * controller with AttachVirtualJoystick(), and then can provide inputs for it
 * via Joystick.SetVirtualAxis(), Joystick.SetVirtualButton(), etc. As this data
 * is supplied, it will look like a normal joystick to SDL, just not backed by a
 * hardware driver. This has been used to make unusual devices, like VR headset
 * controllers, look like normal joysticks, or provide recording/playback of
 * game inputs, etc.
 *
 * @{
 */

// Forward decl
struct Joystick;

/// Alias to raw representation for Joystick.
using JoystickRaw = SDL_Joystick*;

// Forward decl
struct JoystickRef;

/// Safely wrap Joystick for non owning parameters
struct JoystickParam
{
  JoystickRaw value; ///< parameter's JoystickRaw

  /// Constructs from JoystickRaw
  constexpr JoystickParam(JoystickRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr JoystickParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const JoystickParam& other) const = default;

  /// Converts to underlying JoystickRaw
  constexpr operator JoystickRaw() const { return value; }
};

/// Alias to raw representation for JoystickID.
using JoystickIDRaw = SDL_JoystickID;

// Forward decl
struct JoystickID;

// Forward decl
struct JoystickApiLock;

/**
 * An enum of some common joystick types.
 *
 * In some cases, SDL can identify a low-level joystick as being a certain type
 * of device, and will report it through Joystick.GetType (or
 * JoystickID.GetJoystickTypeForID).
 *
 * This is by no means a complete list of everything that can be plugged into a
 * computer.
 *
 * You may refer to [XInput Controller
 * Types](https://learn.microsoft.com/en-us/windows/win32/xinput/xinput-and-controller-subtypes)
 * table for a general understanding of each joystick type.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using JoystickType = SDL_JoystickType;

constexpr JoystickType JOYSTICK_TYPE_UNKNOWN =
  SDL_JOYSTICK_TYPE_UNKNOWN; ///< JOYSTICK_TYPE_UNKNOWN

constexpr JoystickType JOYSTICK_TYPE_GAMEPAD =
  SDL_JOYSTICK_TYPE_GAMEPAD; ///< JOYSTICK_TYPE_GAMEPAD

constexpr JoystickType JOYSTICK_TYPE_WHEEL =
  SDL_JOYSTICK_TYPE_WHEEL; ///< JOYSTICK_TYPE_WHEEL

constexpr JoystickType JOYSTICK_TYPE_ARCADE_STICK =
  SDL_JOYSTICK_TYPE_ARCADE_STICK; ///< JOYSTICK_TYPE_ARCADE_STICK

constexpr JoystickType JOYSTICK_TYPE_FLIGHT_STICK =
  SDL_JOYSTICK_TYPE_FLIGHT_STICK; ///< JOYSTICK_TYPE_FLIGHT_STICK

constexpr JoystickType JOYSTICK_TYPE_DANCE_PAD =
  SDL_JOYSTICK_TYPE_DANCE_PAD; ///< JOYSTICK_TYPE_DANCE_PAD

constexpr JoystickType JOYSTICK_TYPE_GUITAR =
  SDL_JOYSTICK_TYPE_GUITAR; ///< JOYSTICK_TYPE_GUITAR

constexpr JoystickType JOYSTICK_TYPE_DRUM_KIT =
  SDL_JOYSTICK_TYPE_DRUM_KIT; ///< JOYSTICK_TYPE_DRUM_KIT

constexpr JoystickType JOYSTICK_TYPE_ARCADE_PAD =
  SDL_JOYSTICK_TYPE_ARCADE_PAD; ///< JOYSTICK_TYPE_ARCADE_PAD

constexpr JoystickType JOYSTICK_TYPE_THROTTLE =
  SDL_JOYSTICK_TYPE_THROTTLE; ///< JOYSTICK_TYPE_THROTTLE

constexpr JoystickType JOYSTICK_TYPE_COUNT =
  SDL_JOYSTICK_TYPE_COUNT; ///< JOYSTICK_TYPE_COUNT

/**
 * This is a unique ID for a joystick for the time it is connected to the
 * system, and is never reused for the lifetime of the application.
 *
 * If the joystick is disconnected and reconnected, it will get a new ID.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
class JoystickID
{
  JoystickIDRaw m_joystickID;

public:
  /**
   * Wraps JoystickID.
   *
   * @param joystickID the value to be wrapped
   */
  constexpr JoystickID(JoystickIDRaw joystickID = {}) noexcept
    : m_joystickID(joystickID)
  {
  }

  /**
   * Unwraps to the underlying JoystickID.
   *
   * @returns the underlying JoystickIDRaw.
   */
  constexpr operator JoystickIDRaw() const noexcept { return m_joystickID; }

  /**
   * Get the implementation dependent name of a joystick.
   *
   * This can be called before any joysticks are opened.
   *
   * @returns the name of the selected joystick. If no name can be found, this
   *          function returns nullptr; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetName
   * @sa GetJoysticks
   */
  const char* GetJoystickNameForID();

  /**
   * Get the implementation dependent path of a joystick.
   *
   * This can be called before any joysticks are opened.
   *
   * @returns the path of the selected joystick. If no path can be found, this
   *          function returns nullptr; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetPath
   * @sa GetJoysticks
   */
  const char* GetJoystickPathForID();

  /**
   * Get the player index of a joystick.
   *
   * This can be called before any joysticks are opened.
   *
   * @returns the player index of a joystick, or -1 if it's not available.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetPlayerIndex
   * @sa GetJoysticks
   */
  int GetJoystickPlayerIndexForID();

  /**
   * Get the implementation-dependent GUID of a joystick.
   *
   * This can be called before any joysticks are opened.
   *
   * @returns the GUID of the selected joystick. If called with an invalid
   *          instance_id, this function returns a zero GUID.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetGUID
   * @sa GUID.ToString
   */
  GUID GetJoystickGUIDForID();

  /**
   * Get the USB vendor ID of a joystick, if available.
   *
   * This can be called before any joysticks are opened. If the vendor ID isn't
   * available this function returns 0.
   *
   * @returns the USB vendor ID of the selected joystick. If called with an
   *          invalid instance_id, this function returns 0.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetVendor
   * @sa GetJoysticks
   */
  Uint16 GetJoystickVendorForID();

  /**
   * Get the USB product ID of a joystick, if available.
   *
   * This can be called before any joysticks are opened. If the product ID isn't
   * available this function returns 0.
   *
   * @returns the USB product ID of the selected joystick. If called with an
   *          invalid instance_id, this function returns 0.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetProduct
   * @sa GetJoysticks
   */
  Uint16 GetJoystickProductForID();

  /**
   * Get the product version of a joystick, if available.
   *
   * This can be called before any joysticks are opened. If the product version
   * isn't available this function returns 0.
   *
   * @returns the product version of the selected joystick. If called with an
   *          invalid instance_id, this function returns 0.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetProductVersion
   * @sa GetJoysticks
   */
  Uint16 GetJoystickProductVersionForID();

  /**
   * Get the type of a joystick, if available.
   *
   * This can be called before any joysticks are opened.
   *
   * @returns the JoystickType of the selected joystick. If called with an
   *          invalid instance_id, this function returns
   *          `JOYSTICK_TYPE_UNKNOWN`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetType
   * @sa GetJoysticks
   */
  JoystickType GetJoystickTypeForID();

  /**
   * Open a joystick for use.
   *
   * The joystick subsystem must be initialized before a joystick can be opened
   * for use.
   *
   * @returns an Joystick on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.Close
   */
  Joystick OpenJoystick();

  /**
   * Get the Joystick associated with an instance ID, if it has been opened.
   *
   * @returns an Joystick on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  JoystickRef GetJoystickFromID();

  /**
   * Detach a virtual joystick.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AttachVirtualJoystick
   */
  void DetachVirtualJoystick();

  /**
   * Query whether or not a joystick is virtual.
   *
   * @returns true if the joystick is virtual, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool IsJoystickVirtual();
};

/**
 * Possible connection states for a joystick device.
 *
 * This is used by Joystick.GetConnectionState to report how a device is
 * connected to the system.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using JoystickConnectionState = SDL_JoystickConnectionState;

constexpr JoystickConnectionState JOYSTICK_CONNECTION_INVALID =
  SDL_JOYSTICK_CONNECTION_INVALID; ///< JOYSTICK_CONNECTION_INVALID

constexpr JoystickConnectionState JOYSTICK_CONNECTION_UNKNOWN =
  SDL_JOYSTICK_CONNECTION_UNKNOWN; ///< JOYSTICK_CONNECTION_UNKNOWN

constexpr JoystickConnectionState JOYSTICK_CONNECTION_WIRED =
  SDL_JOYSTICK_CONNECTION_WIRED; ///< JOYSTICK_CONNECTION_WIRED

constexpr JoystickConnectionState JOYSTICK_CONNECTION_WIRELESS =
  SDL_JOYSTICK_CONNECTION_WIRELESS; ///< JOYSTICK_CONNECTION_WIRELESS

/**
 * Represents tbe HatState for a Joystick.
 *
 */
using HatState = Uint8;

constexpr Uint8 HAT_CENTERED = SDL_HAT_CENTERED; ///< CENTERED

constexpr Uint8 HAT_UP = SDL_HAT_UP; ///< UP

constexpr Uint8 HAT_RIGHT = SDL_HAT_RIGHT; ///< RIGHT

constexpr Uint8 HAT_DOWN = SDL_HAT_DOWN; ///< DOWN

constexpr Uint8 HAT_LEFT = SDL_HAT_LEFT; ///< LEFT

constexpr Uint8 HAT_RIGHTUP = SDL_HAT_RIGHTUP; ///< RIGHTUP

constexpr Uint8 HAT_RIGHTDOWN = SDL_HAT_RIGHTDOWN; ///< RIGHTDOWN

constexpr Uint8 HAT_LEFTUP = SDL_HAT_LEFTUP; ///< LEFTUP

constexpr Uint8 HAT_LEFTDOWN = SDL_HAT_LEFTDOWN; ///< LEFTDOWN

/**
 * The joystick structure used to identify an SDL joystick.
 *
 * This is opaque data.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class Joystick
{
  JoystickRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Joystick(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from JoystickParam.
   *
   * @param resource a JoystickRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Joystick(const JoystickRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr Joystick(const Joystick& other) noexcept = default;

public:
  /// Move constructor
  constexpr Joystick(Joystick&& other) noexcept
    : Joystick(other.release())
  {
  }

  constexpr Joystick(const JoystickRef& other) = delete;

  constexpr Joystick(JoystickRef&& other) = delete;

  /**
   * Open a joystick for use.
   *
   * The joystick subsystem must be initialized before a joystick can be opened
   * for use.
   *
   * @param instance_id the joystick instance ID.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.Close
   */
  Joystick(JoystickID instance_id)
    : m_resource(CheckError(SDL_OpenJoystick(instance_id)))
  {
  }

  /// Destructor
  ~Joystick() { SDL_CloseJoystick(m_resource); }

  /// Assignment operator.
  constexpr Joystick& operator=(Joystick&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Joystick& operator=(const Joystick& other) noexcept = default;

public:
  /// Retrieves underlying JoystickRaw.
  constexpr JoystickRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying JoystickRaw and clear this.
  constexpr JoystickRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Joystick& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to JoystickParam
  constexpr operator JoystickParam() const noexcept { return {m_resource}; }

  /**
   * Close a joystick previously opened with JoystickID.OpenJoystick().
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa JoystickID.OpenJoystick
   */
  void Close();

  /**
   * Set the state of an axis on an opened virtual joystick.
   *
   * Please note that values set here will not be applied until the next call to
   * UpdateJoysticks, which can either be called directly, or can be called
   * indirectly through various other SDL APIs, including, but not limited to
   * the following: PollEvent, PumpEvents, WaitEventTimeout, WaitEvent.
   *
   * Note that when sending trigger axes, you should scale the value to the full
   * range of Sint16. For example, a trigger at rest would have the value of
   * `JOYSTICK_AXIS_MIN`.
   *
   * @param axis the index of the axis on the virtual joystick to update.
   * @param value the new value for the specified axis.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.SetVirtualButton
   * @sa Joystick.SetVirtualBall
   * @sa Joystick.SetVirtualHat
   * @sa Joystick.SetVirtualTouchpad
   * @sa SDL_SetJoystickVirtualSensorData
   */
  void SetVirtualAxis(int axis, Sint16 value);

  /**
   * Generate ball motion on an opened virtual joystick.
   *
   * Please note that values set here will not be applied until the next call to
   * UpdateJoysticks, which can either be called directly, or can be called
   * indirectly through various other SDL APIs, including, but not limited to
   * the following: PollEvent, PumpEvents, WaitEventTimeout, WaitEvent.
   *
   * @param ball the index of the ball on the virtual joystick to update.
   * @param xrel the relative motion on the X axis.
   * @param yrel the relative motion on the Y axis.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.SetVirtualAxis
   * @sa Joystick.SetVirtualButton
   * @sa Joystick.SetVirtualHat
   * @sa Joystick.SetVirtualTouchpad
   * @sa SDL_SetJoystickVirtualSensorData
   */
  void SetVirtualBall(int ball, Sint16 xrel, Sint16 yrel);

  /**
   * Set the state of a button on an opened virtual joystick.
   *
   * Please note that values set here will not be applied until the next call to
   * UpdateJoysticks, which can either be called directly, or can be called
   * indirectly through various other SDL APIs, including, but not limited to
   * the following: PollEvent, PumpEvents, WaitEventTimeout, WaitEvent.
   *
   * @param button the index of the button on the virtual joystick to update.
   * @param down true if the button is pressed, false otherwise.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.SetVirtualAxis
   * @sa Joystick.SetVirtualBall
   * @sa Joystick.SetVirtualHat
   * @sa Joystick.SetVirtualTouchpad
   * @sa SDL_SetJoystickVirtualSensorData
   */
  void SetVirtualButton(int button, bool down);

  /**
   * Set the state of a hat on an opened virtual joystick.
   *
   * Please note that values set here will not be applied until the next call to
   * UpdateJoysticks, which can either be called directly, or can be called
   * indirectly through various other SDL APIs, including, but not limited to
   * the following: PollEvent, PumpEvents, WaitEventTimeout, WaitEvent.
   *
   * @param hat the index of the hat on the virtual joystick to update.
   * @param value the new value for the specified hat.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.SetVirtualAxis
   * @sa Joystick.SetVirtualButton
   * @sa Joystick.SetVirtualBall
   * @sa Joystick.SetVirtualTouchpad
   * @sa SDL_SetJoystickVirtualSensorData
   */
  void SetVirtualHat(int hat, Uint8 value);

  /**
   * Set touchpad finger state on an opened virtual joystick.
   *
   * Please note that values set here will not be applied until the next call to
   * UpdateJoysticks, which can either be called directly, or can be called
   * indirectly through various other SDL APIs, including, but not limited to
   * the following: PollEvent, PumpEvents, WaitEventTimeout, WaitEvent.
   *
   * @param touchpad the index of the touchpad on the virtual joystick to
   *                 update.
   * @param finger the index of the finger on the touchpad to set.
   * @param down true if the finger is pressed, false if the finger is released.
   * @param p the x, y coordinates of the finger on the touchpad, normalized 0
   *          to 1, with the origin in the upper left.
   * @param pressure the pressure of the finger.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.SetVirtualAxis
   * @sa Joystick.SetVirtualButton
   * @sa Joystick.SetVirtualBall
   * @sa Joystick.SetVirtualHat
   * @sa SDL_SetJoystickVirtualSensorData
   */
  void SetVirtualTouchpad(int touchpad,
                          int finger,
                          bool down,
                          const FPointRaw& p,
                          float pressure);

  /**
   * Send a sensor update for an opened virtual joystick.
   *
   * Please note that values set here will not be applied until the next call to
   * UpdateJoysticks, which can either be called directly, or can be called
   * indirectly through various other SDL APIs, including, but not limited to
   * the following: PollEvent, PumpEvents, WaitEventTimeout, WaitEvent.
   *
   * @param type the type of the sensor on the virtual joystick to update.
   * @param sensor_timestamp a 64-bit timestamp in nanoseconds associated with
   *                         the sensor reading.
   * @param data the data associated with the sensor reading.
   * @param num_values the number of values pointed to by `data`.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.SetVirtualAxis
   * @sa Joystick.SetVirtualButton
   * @sa Joystick.SetVirtualBall
   * @sa Joystick.SetVirtualHat
   * @sa Joystick.SetVirtualTouchpad
   */
  void SendVirtualSensorData(SensorType type,
                             Uint64 sensor_timestamp,
                             const float* data,
                             int num_values);

  /**
   * Get the properties associated with a joystick.
   *
   * The following read-only properties are provided by SDL:
   *
   * - `prop::JoystickCap.MONO_LED_BOOLEAN`: true if this joystick has an LED
   *   that has adjustable brightness
   * - `prop::JoystickCap.RGB_LED_BOOLEAN`: true if this joystick has an LED
   *   that has adjustable color
   * - `prop::JoystickCap.PLAYER_LED_BOOLEAN`: true if this joystick has a
   *   player LED
   * - `prop::JoystickCap.RUMBLE_BOOLEAN`: true if this joystick has left/right
   *   rumble
   * - `prop::JoystickCap.TRIGGER_RUMBLE_BOOLEAN`: true if this joystick has
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
   * Get the implementation dependent name of a joystick.
   *
   * @returns the name of the selected joystick. If no name can be found, this
   *          function returns nullptr; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa JoystickID.GetJoystickNameForID
   */
  const char* GetName();

  /**
   * Get the implementation dependent path of a joystick.
   *
   * @returns the path of the selected joystick. If no path can be found, this
   *          function returns nullptr; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa JoystickID.GetJoystickPathForID
   */
  const char* GetPath();

  /**
   * Get the player index of an opened joystick.
   *
   * For XInput controllers this returns the XInput user index. Many joysticks
   * will not be able to supply this information.
   *
   * @returns the player index, or -1 if it's not available.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.SetPlayerIndex
   */
  int GetPlayerIndex();

  /**
   * Set the player index of an opened joystick.
   *
   * @param player_index player index to assign to this joystick, or -1 to clear
   *                     the player index and turn off player LEDs.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetPlayerIndex
   */
  void SetPlayerIndex(int player_index);

  /**
   * Get the implementation-dependent GUID for the joystick.
   *
   * This function requires an open joystick.
   *
   * @returns the GUID of the given joystick. If called on an invalid index,
   *          this function returns a zero GUID; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa JoystickID.GetJoystickGUIDForID
   * @sa GUID.ToString
   */
  GUID GetGUID();

  /**
   * Get the USB vendor ID of an opened joystick, if available.
   *
   * If the vendor ID isn't available this function returns 0.
   *
   * @returns the USB vendor ID of the selected joystick, or 0 if unavailable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa JoystickID.GetJoystickVendorForID
   */
  Uint16 GetVendor();

  /**
   * Get the USB product ID of an opened joystick, if available.
   *
   * If the product ID isn't available this function returns 0.
   *
   * @returns the USB product ID of the selected joystick, or 0 if unavailable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa JoystickID.GetJoystickProductForID
   */
  Uint16 GetProduct();

  /**
   * Get the product version of an opened joystick, if available.
   *
   * If the product version isn't available this function returns 0.
   *
   * @returns the product version of the selected joystick, or 0 if unavailable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa JoystickID.GetJoystickProductVersionForID
   */
  Uint16 GetProductVersion();

  /**
   * Get the firmware version of an opened joystick, if available.
   *
   * If the firmware version isn't available this function returns 0.
   *
   * @returns the firmware version of the selected joystick, or 0 if
   *          unavailable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Uint16 GetFirmwareVersion();

  /**
   * Get the serial number of an opened joystick, if available.
   *
   * Returns the serial number of the joystick, or nullptr if it is not
   * available.
   *
   * @returns the serial number of the selected joystick, or nullptr if
   *          unavailable.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  const char* GetSerial();

  /**
   * Get the type of an opened joystick.
   *
   * @returns the JoystickType of the selected joystick.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa JoystickID.GetJoystickTypeForID
   */
  JoystickType GetType();

  /**
   * Get the status of a specified joystick.
   *
   * @returns true if the joystick has been opened, false if it has not; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Connected();

  /**
   * Get the instance ID of an opened joystick.
   *
   * @returns the instance ID of the specified joystick on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  JoystickID GetID();

  /**
   * Get the number of general axis controls on a joystick.
   *
   * Often, the directional pad on a game controller will either look like 4
   * separate buttons or a POV hat, and not axes, but all of this is up to the
   * device and platform.
   *
   * @returns the number of axis controls/number of axes on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetAxis
   * @sa Joystick.GetNumBalls
   * @sa Joystick.GetNumButtons
   * @sa Joystick.GetNumHats
   */
  int GetNumAxes();

  /**
   * Get the number of trackballs on a joystick.
   *
   * Joystick trackballs have only relative motion events associated with them
   * and their state cannot be polled.
   *
   * Most joysticks do not have trackballs.
   *
   * @returns the number of trackballs on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetBall
   * @sa Joystick.GetNumAxes
   * @sa Joystick.GetNumButtons
   * @sa Joystick.GetNumHats
   */
  int GetNumBalls();

  /**
   * Get the number of POV hats on a joystick.
   *
   * @returns the number of POV hats on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetHat
   * @sa Joystick.GetNumAxes
   * @sa Joystick.GetNumBalls
   * @sa Joystick.GetNumButtons
   */
  int GetNumHats();

  /**
   * Get the number of buttons on a joystick.
   *
   * @returns the number of buttons on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetButton
   * @sa Joystick.GetNumAxes
   * @sa Joystick.GetNumBalls
   * @sa Joystick.GetNumHats
   */
  int GetNumButtons();

  /**
   * Get the current state of an axis control on a joystick.
   *
   * SDL makes no promises about what part of the joystick any given axis refers
   * to. Your game should have some sort of configuration UI to let users
   * specify what each axis should be bound to. Alternately, SDL's higher-level
   * Game Controller API makes a great effort to apply order to this lower-level
   * interface, so you know that a specific axis is the "left thumb stick," etc.
   *
   * The value returned by Joystick.GetAxis() is a signed integer (-32768 to
   * 32767) representing the current position of the axis. It may be necessary
   * to impose certain tolerances on these values to account for jitter.
   *
   * @param axis the axis to query; the axis indices start at index 0.
   * @returns a 16-bit signed integer representing the current position of the
   *          axis or 0 on failure; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetNumAxes
   */
  Sint16 GetAxis(int axis);

  /**
   * Get the initial state of an axis control on a joystick.
   *
   * The state is a value ranging from -32768 to 32767.
   *
   * The axis indices start at index 0.
   *
   * @param axis the axis to query; the axis indices start at index 0.
   * @param state upon return, the initial value is supplied here.
   * @returns true if this axis has any initial value, or false if not.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool GetAxisInitialState(int axis, Sint16* state);

  /**
   * Get the ball axis change since the last poll.
   *
   * Trackballs can only return relative motion since the last call to
   * Joystick.GetBall(), these motion deltas are placed into `dx` and `dy`.
   *
   * Most joysticks do not have trackballs.
   *
   * @param ball the ball index to query; ball indices start at index 0.
   * @param dx stores the difference in the x axis position since the last poll.
   * @param dy stores the difference in the y axis position since the last poll.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetNumBalls
   */
  void GetBall(int ball, int* dx, int* dy);

  /**
   * Get the current state of a POV hat on a joystick.
   *
   * The returned value will be one of the `SDL_HAT_*` values.
   *
   * @param hat the hat index to get the state from; indices start at index 0.
   * @returns the current hat position.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetNumHats
   */
  Uint8 GetHat(int hat);

  /**
   * Get the current state of a button on a joystick.
   *
   * @param button the button index to get the state from; indices start at
   *               index 0.
   * @returns true if the button is pressed, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Joystick.GetNumButtons
   */
  bool GetButton(int button);

  /**
   * Start a rumble effect.
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
   * @returns true, or false if rumble isn't supported on this joystick.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool Rumble(Uint16 low_frequency_rumble,
              Uint16 high_frequency_rumble,
              Uint32 duration_ms);

  /**
   * Start a rumble effect in the joystick's triggers.
   *
   * Each call to this function cancels any previous trigger rumble effect, and
   * calling it with 0 intensity stops any rumbling.
   *
   * Note that this is rumbling of the _triggers_ and not the game controller as
   * a whole. This is currently only supported on Xbox One controllers. If you
   * want the (more common) whole-controller rumble, use Joystick.Rumble()
   * instead.
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
   * @sa Joystick.Rumble
   */
  void RumbleTriggers(Uint16 left_rumble,
                      Uint16 right_rumble,
                      Uint32 duration_ms);

  /**
   * Update a joystick's LED color.
   *
   * An example of a joystick LED is the light on the back of a PlayStation 4's
   * DualShock 4 controller.
   *
   * For joysticks with a single color LED, the maximum of the RGB values will
   * be used as the LED brightness.
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
   * Send a joystick specific effect packet.
   *
   * @param data the data to send to the joystick.
   * @param size the size of the data to send to the joystick.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SendEffect(const void* data, int size);

  /**
   * Get the connection state of a joystick.
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
   * Get the battery state of a joystick.
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
   * @returns the current battery state or `POWERSTATE_ERROR` on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PowerState GetPowerInfo(int* percent);
};

/// Semi-safe reference for Joystick.
struct JoystickRef : Joystick
{
  using Joystick::Joystick;

  /**
   * Constructs from JoystickParam.
   *
   * @param resource a JoystickRaw or Joystick.
   *
   * This does not takes ownership!
   */
  JoystickRef(JoystickParam resource) noexcept
    : Joystick(resource.value)
  {
  }

  /**
   * Constructs from JoystickParam.
   *
   * @param resource a JoystickRaw or Joystick.
   *
   * This does not takes ownership!
   */
  JoystickRef(JoystickRaw resource) noexcept
    : Joystick(resource)
  {
  }

  /// Copy constructor.
  constexpr JoystickRef(const JoystickRef& other) noexcept = default;

  /// Destructor
  ~JoystickRef() { release(); }
};

/**
 * The largest value an Joystick's axis can report.
 *
 * @since This constant is available since SDL 3.2.0.
 *
 * @sa JOYSTICK_AXIS_MIN
 */
constexpr int JOYSTICK_AXIS_MAX = SDL_JOYSTICK_AXIS_MAX;

/**
 * The smallest value an Joystick's axis can report.
 *
 * This is a negative number!
 *
 * @since This constant is available since SDL 3.2.0.
 *
 * @sa JOYSTICK_AXIS_MAX
 */
constexpr int JOYSTICK_AXIS_MIN = SDL_JOYSTICK_AXIS_MIN;

/**
 * Locking for atomic access to the joystick API.
 *
 * The SDL joystick functions are thread-safe, however you can lock the
 * joysticks while processing to guarantee that the joystick list won't change
 * and joystick and gamepad events will not be delivered.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
class JoystickApiLock
{
  bool m_lock;

public:
  /**
   * Locking for atomic access to the joystick API.
   *
   * The SDL joystick functions are thread-safe, however you can lock the
   * joysticks while processing to guarantee that the joystick list won't change
   * and joystick and gamepad events will not be delivered.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  JoystickApiLock();

  /**
   * Locking for atomic access to the joystick API.
   *
   * The SDL joystick functions are thread-safe, however you can lock the
   * joysticks while processing to guarantee that the joystick list won't change
   * and joystick and gamepad events will not be delivered.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  JoystickApiLock(const JoystickApiLock& other) = delete;

  /// Move constructor
  constexpr JoystickApiLock(JoystickApiLock&& other) noexcept
    : m_lock(other.m_lock)
  {
    other.m_lock = {};
  }

  /**
   * Unlocking for atomic access to the joystick API.
   *
   * @threadsafety This should be called from the same thread that called
   *               LockJoysticks().
   *
   * @since This function is available since SDL 3.2.0.
   */
  ~JoystickApiLock() { reset(); }

  JoystickApiLock& operator=(const JoystickApiLock& other) = delete;

  /// Assignment operator
  JoystickApiLock& operator=(JoystickApiLock&& other)
  {
    std::swap(m_lock, other.m_lock);
    return *this;
  }

  /// True if not locked.
  constexpr operator bool() const { return bool(m_lock); }

  /**
   * Unlocking for atomic access to the joystick API.
   *
   * @threadsafety This should be called from the same thread that called
   *               LockJoysticks().
   *
   * @since This function is available since SDL 3.2.0.
   */
  void reset();
};

/**
 * Locking for atomic access to the joystick API.
 *
 * The SDL joystick functions are thread-safe, however you can lock the
 * joysticks while processing to guarantee that the joystick list won't change
 * and joystick and gamepad events will not be delivered.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void LockJoysticks() { SDL_LockJoysticks(); }

inline JoystickApiLock::JoystickApiLock()
  : m_lock(true)
{
  LockJoysticks();
}

/**
 * Unlocking for atomic access to the joystick API.
 *
 * @threadsafety This should be called from the same thread that called
 *               LockJoysticks().
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void UnlockJoysticks() { SDL_UnlockJoysticks(); }

inline void JoystickApiLock::reset()
{
  if (!m_lock) return;
  UnlockJoysticks();
  m_lock = false;
}

/**
 * Return whether a joystick is currently connected.
 *
 * @returns true if a joystick is connected, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetJoysticks
 */
inline bool HasJoystick() { return SDL_HasJoystick(); }

/**
 * Get a list of currently connected joysticks.
 *
 * @returns a 0 terminated array of joystick instance IDs or nullptr on failure;
 *          call GetError() for more information. This should be freed with
 *          free() when it is no longer needed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa HasJoystick
 * @sa JoystickID.OpenJoystick
 */
inline OwnArray<JoystickID> GetJoysticks()
{
  int count;
  auto r = reinterpret_cast<JoystickID*>(SDL_GetJoysticks(&count));
  return OwnArray<JoystickID>(r, count);
}

/**
 * Get the implementation dependent name of a joystick.
 *
 * This can be called before any joysticks are opened.
 *
 * @param instance_id the joystick instance ID.
 * @returns the name of the selected joystick. If no name can be found, this
 *          function returns nullptr; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetName
 * @sa GetJoysticks
 */
inline const char* GetJoystickNameForID(JoystickID instance_id)
{
  return SDL_GetJoystickNameForID(instance_id);
}

inline const char* JoystickID::GetJoystickNameForID()
{
  return SDL::GetJoystickNameForID(m_joystickID);
}

/**
 * Get the implementation dependent path of a joystick.
 *
 * This can be called before any joysticks are opened.
 *
 * @param instance_id the joystick instance ID.
 * @returns the path of the selected joystick. If no path can be found, this
 *          function returns nullptr; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetPath
 * @sa GetJoysticks
 */
inline const char* GetJoystickPathForID(JoystickID instance_id)
{
  return SDL_GetJoystickPathForID(instance_id);
}

inline const char* JoystickID::GetJoystickPathForID()
{
  return SDL::GetJoystickPathForID(m_joystickID);
}

/**
 * Get the player index of a joystick.
 *
 * This can be called before any joysticks are opened.
 *
 * @param instance_id the joystick instance ID.
 * @returns the player index of a joystick, or -1 if it's not available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetPlayerIndex
 * @sa GetJoysticks
 */
inline int GetJoystickPlayerIndexForID(JoystickID instance_id)
{
  return SDL_GetJoystickPlayerIndexForID(instance_id);
}

inline int JoystickID::GetJoystickPlayerIndexForID()
{
  return SDL::GetJoystickPlayerIndexForID(m_joystickID);
}

/**
 * Get the implementation-dependent GUID of a joystick.
 *
 * This can be called before any joysticks are opened.
 *
 * @param instance_id the joystick instance ID.
 * @returns the GUID of the selected joystick. If called with an invalid
 *          instance_id, this function returns a zero GUID.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetGUID
 * @sa GUID.ToString
 */
inline GUID GetJoystickGUIDForID(JoystickID instance_id)
{
  return SDL_GetJoystickGUIDForID(instance_id);
}

inline GUID JoystickID::GetJoystickGUIDForID()
{
  return SDL::GetJoystickGUIDForID(m_joystickID);
}

/**
 * Get the USB vendor ID of a joystick, if available.
 *
 * This can be called before any joysticks are opened. If the vendor ID isn't
 * available this function returns 0.
 *
 * @param instance_id the joystick instance ID.
 * @returns the USB vendor ID of the selected joystick. If called with an
 *          invalid instance_id, this function returns 0.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetVendor
 * @sa GetJoysticks
 */
inline Uint16 GetJoystickVendorForID(JoystickID instance_id)
{
  return SDL_GetJoystickVendorForID(instance_id);
}

inline Uint16 JoystickID::GetJoystickVendorForID()
{
  return SDL::GetJoystickVendorForID(m_joystickID);
}

/**
 * Get the USB product ID of a joystick, if available.
 *
 * This can be called before any joysticks are opened. If the product ID isn't
 * available this function returns 0.
 *
 * @param instance_id the joystick instance ID.
 * @returns the USB product ID of the selected joystick. If called with an
 *          invalid instance_id, this function returns 0.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetProduct
 * @sa GetJoysticks
 */
inline Uint16 GetJoystickProductForID(JoystickID instance_id)
{
  return SDL_GetJoystickProductForID(instance_id);
}

inline Uint16 JoystickID::GetJoystickProductForID()
{
  return SDL::GetJoystickProductForID(m_joystickID);
}

/**
 * Get the product version of a joystick, if available.
 *
 * This can be called before any joysticks are opened. If the product version
 * isn't available this function returns 0.
 *
 * @param instance_id the joystick instance ID.
 * @returns the product version of the selected joystick. If called with an
 *          invalid instance_id, this function returns 0.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetProductVersion
 * @sa GetJoysticks
 */
inline Uint16 GetJoystickProductVersionForID(JoystickID instance_id)
{
  return SDL_GetJoystickProductVersionForID(instance_id);
}

inline Uint16 JoystickID::GetJoystickProductVersionForID()
{
  return SDL::GetJoystickProductVersionForID(m_joystickID);
}

/**
 * Get the type of a joystick, if available.
 *
 * This can be called before any joysticks are opened.
 *
 * @param instance_id the joystick instance ID.
 * @returns the JoystickType of the selected joystick. If called with an invalid
 *          instance_id, this function returns `JOYSTICK_TYPE_UNKNOWN`.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetType
 * @sa GetJoysticks
 */
inline JoystickType GetJoystickTypeForID(JoystickID instance_id)
{
  return SDL_GetJoystickTypeForID(instance_id);
}

inline JoystickType JoystickID::GetJoystickTypeForID()
{
  return SDL::GetJoystickTypeForID(m_joystickID);
}

/**
 * Open a joystick for use.
 *
 * The joystick subsystem must be initialized before a joystick can be opened
 * for use.
 *
 * @param instance_id the joystick instance ID.
 * @returns an Joystick on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.Close
 */
inline Joystick OpenJoystick(JoystickID instance_id)
{
  return Joystick(instance_id);
}

inline Joystick JoystickID::OpenJoystick() { return Joystick(m_joystickID); }

/**
 * Get the Joystick associated with an instance ID, if it has been opened.
 *
 * @param instance_id the instance ID to get the Joystick for.
 * @returns an Joystick on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline JoystickRef GetJoystickFromID(JoystickID instance_id)
{
  return {CheckError(SDL_GetJoystickFromID(instance_id))};
}

inline JoystickRef JoystickID::GetJoystickFromID()
{
  return SDL::GetJoystickFromID(m_joystickID);
}

/**
 * Get the Joystick associated with a player index.
 *
 * @param player_index the player index to get the Joystick for.
 * @returns an Joystick on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetPlayerIndex
 * @sa Joystick.SetPlayerIndex
 */
inline JoystickRef GetJoystickFromPlayerIndex(int player_index)
{
  return {CheckError(SDL_GetJoystickFromPlayerIndex(player_index))};
}

/**
 * The structure that describes a virtual joystick touchpad.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa VirtualJoystickDesc
 */
using VirtualJoystickTouchpadDesc = SDL_VirtualJoystickTouchpadDesc;

/**
 * The structure that describes a virtual joystick sensor.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa VirtualJoystickDesc
 */
using VirtualJoystickSensorDesc = SDL_VirtualJoystickSensorDesc;

/**
 * The structure that describes a virtual joystick.
 *
 * This structure should be initialized using InitInterface(). All elements of
 * this structure are optional.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa AttachVirtualJoystick
 * @sa InitInterface
 * @sa VirtualJoystickSensorDesc
 * @sa VirtualJoystickTouchpadDesc
 */
using VirtualJoystickDesc = SDL_VirtualJoystickDesc;

/**
 * Attach a new virtual joystick.
 *
 * Apps can create virtual joysticks, that exist without hardware directly
 * backing them, and have program-supplied inputs. Once attached, a virtual
 * joystick looks like any other joystick that SDL can access. These can be used
 * to make other things look like joysticks, or provide pre-recorded input, etc.
 *
 * Once attached, the app can send joystick inputs to the new virtual joystick
 * using Joystick.SetVirtualAxis(), etc.
 *
 * When no longer needed, the virtual joystick can be removed by calling
 * JoystickID.DetachVirtualJoystick().
 *
 * @param desc joystick description, initialized using InitInterface().
 * @returns the joystick instance ID, or 0 on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa JoystickID.DetachVirtualJoystick
 * @sa Joystick.SetVirtualAxis
 * @sa Joystick.SetVirtualButton
 * @sa Joystick.SetVirtualBall
 * @sa Joystick.SetVirtualHat
 * @sa Joystick.SetVirtualTouchpad
 * @sa SDL_SetJoystickVirtualSensorData
 */
inline JoystickID AttachVirtualJoystick(const VirtualJoystickDesc& desc)
{
  return SDL_AttachVirtualJoystick(&desc);
}

/**
 * Detach a virtual joystick.
 *
 * @param instance_id the joystick instance ID, previously returned from
 *                    AttachVirtualJoystick().
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AttachVirtualJoystick
 */
inline void DetachVirtualJoystick(JoystickID instance_id)
{
  CheckError(SDL_DetachVirtualJoystick(instance_id));
}

inline void JoystickID::DetachVirtualJoystick()
{
  SDL::DetachVirtualJoystick(m_joystickID);
}

/**
 * Query whether or not a joystick is virtual.
 *
 * @param instance_id the joystick instance ID.
 * @returns true if the joystick is virtual, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool IsJoystickVirtual(JoystickID instance_id)
{
  return SDL_IsJoystickVirtual(instance_id);
}

inline bool JoystickID::IsJoystickVirtual()
{
  return SDL::IsJoystickVirtual(m_joystickID);
}

/**
 * Set the state of an axis on an opened virtual joystick.
 *
 * Please note that values set here will not be applied until the next call to
 * UpdateJoysticks, which can either be called directly, or can be called
 * indirectly through various other SDL APIs, including, but not limited to the
 * following: PollEvent, PumpEvents, WaitEventTimeout, WaitEvent.
 *
 * Note that when sending trigger axes, you should scale the value to the full
 * range of Sint16. For example, a trigger at rest would have the value of
 * `JOYSTICK_AXIS_MIN`.
 *
 * @param joystick the virtual joystick on which to set state.
 * @param axis the index of the axis on the virtual joystick to update.
 * @param value the new value for the specified axis.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.SetVirtualButton
 * @sa Joystick.SetVirtualBall
 * @sa Joystick.SetVirtualHat
 * @sa Joystick.SetVirtualTouchpad
 * @sa SDL_SetJoystickVirtualSensorData
 */
inline void SetJoystickVirtualAxis(JoystickParam joystick,
                                   int axis,
                                   Sint16 value)
{
  CheckError(SDL_SetJoystickVirtualAxis(joystick, axis, value));
}

inline void Joystick::SetVirtualAxis(int axis, Sint16 value)
{
  SDL::SetJoystickVirtualAxis(m_resource, axis, value);
}

/**
 * Generate ball motion on an opened virtual joystick.
 *
 * Please note that values set here will not be applied until the next call to
 * UpdateJoysticks, which can either be called directly, or can be called
 * indirectly through various other SDL APIs, including, but not limited to the
 * following: PollEvent, PumpEvents, WaitEventTimeout, WaitEvent.
 *
 * @param joystick the virtual joystick on which to set state.
 * @param ball the index of the ball on the virtual joystick to update.
 * @param xrel the relative motion on the X axis.
 * @param yrel the relative motion on the Y axis.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.SetVirtualAxis
 * @sa Joystick.SetVirtualButton
 * @sa Joystick.SetVirtualHat
 * @sa Joystick.SetVirtualTouchpad
 * @sa SDL_SetJoystickVirtualSensorData
 */
inline void SetJoystickVirtualBall(JoystickParam joystick,
                                   int ball,
                                   Sint16 xrel,
                                   Sint16 yrel)
{
  CheckError(SDL_SetJoystickVirtualBall(joystick, ball, xrel, yrel));
}

inline void Joystick::SetVirtualBall(int ball, Sint16 xrel, Sint16 yrel)
{
  SDL::SetJoystickVirtualBall(m_resource, ball, xrel, yrel);
}

/**
 * Set the state of a button on an opened virtual joystick.
 *
 * Please note that values set here will not be applied until the next call to
 * UpdateJoysticks, which can either be called directly, or can be called
 * indirectly through various other SDL APIs, including, but not limited to the
 * following: PollEvent, PumpEvents, WaitEventTimeout, WaitEvent.
 *
 * @param joystick the virtual joystick on which to set state.
 * @param button the index of the button on the virtual joystick to update.
 * @param down true if the button is pressed, false otherwise.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.SetVirtualAxis
 * @sa Joystick.SetVirtualBall
 * @sa Joystick.SetVirtualHat
 * @sa Joystick.SetVirtualTouchpad
 * @sa SDL_SetJoystickVirtualSensorData
 */
inline void SetJoystickVirtualButton(JoystickParam joystick,
                                     int button,
                                     bool down)
{
  CheckError(SDL_SetJoystickVirtualButton(joystick, button, down));
}

inline void Joystick::SetVirtualButton(int button, bool down)
{
  SDL::SetJoystickVirtualButton(m_resource, button, down);
}

/**
 * Set the state of a hat on an opened virtual joystick.
 *
 * Please note that values set here will not be applied until the next call to
 * UpdateJoysticks, which can either be called directly, or can be called
 * indirectly through various other SDL APIs, including, but not limited to the
 * following: PollEvent, PumpEvents, WaitEventTimeout, WaitEvent.
 *
 * @param joystick the virtual joystick on which to set state.
 * @param hat the index of the hat on the virtual joystick to update.
 * @param value the new value for the specified hat.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.SetVirtualAxis
 * @sa Joystick.SetVirtualButton
 * @sa Joystick.SetVirtualBall
 * @sa Joystick.SetVirtualTouchpad
 * @sa SDL_SetJoystickVirtualSensorData
 */
inline void SetJoystickVirtualHat(JoystickParam joystick, int hat, Uint8 value)
{
  CheckError(SDL_SetJoystickVirtualHat(joystick, hat, value));
}

inline void Joystick::SetVirtualHat(int hat, Uint8 value)
{
  SDL::SetJoystickVirtualHat(m_resource, hat, value);
}

/**
 * Set touchpad finger state on an opened virtual joystick.
 *
 * Please note that values set here will not be applied until the next call to
 * UpdateJoysticks, which can either be called directly, or can be called
 * indirectly through various other SDL APIs, including, but not limited to the
 * following: PollEvent, PumpEvents, WaitEventTimeout, WaitEvent.
 *
 * @param joystick the virtual joystick on which to set state.
 * @param touchpad the index of the touchpad on the virtual joystick to update.
 * @param finger the index of the finger on the touchpad to set.
 * @param down true if the finger is pressed, false if the finger is released.
 * @param p the x, y coordinates of the finger on the touchpad, normalized 0 to
 *          1, with the origin in the upper left.
 * @param pressure the pressure of the finger.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.SetVirtualAxis
 * @sa Joystick.SetVirtualButton
 * @sa Joystick.SetVirtualBall
 * @sa Joystick.SetVirtualHat
 * @sa SDL_SetJoystickVirtualSensorData
 */
inline void SetJoystickVirtualTouchpad(JoystickParam joystick,
                                       int touchpad,
                                       int finger,
                                       bool down,
                                       const FPointRaw& p,
                                       float pressure)
{
  CheckError(SDL_SetJoystickVirtualTouchpad(
    joystick, touchpad, finger, down, p.x, p.y, pressure));
}

inline void Joystick::SetVirtualTouchpad(int touchpad,
                                         int finger,
                                         bool down,
                                         const FPointRaw& p,
                                         float pressure)
{
  SDL::SetJoystickVirtualTouchpad(
    m_resource, touchpad, finger, down, p, pressure);
}

/**
 * Send a sensor update for an opened virtual joystick.
 *
 * Please note that values set here will not be applied until the next call to
 * UpdateJoysticks, which can either be called directly, or can be called
 * indirectly through various other SDL APIs, including, but not limited to the
 * following: PollEvent, PumpEvents, WaitEventTimeout, WaitEvent.
 *
 * @param joystick the virtual joystick on which to set state.
 * @param type the type of the sensor on the virtual joystick to update.
 * @param sensor_timestamp a 64-bit timestamp in nanoseconds associated with the
 *                         sensor reading.
 * @param data the data associated with the sensor reading.
 * @param num_values the number of values pointed to by `data`.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.SetVirtualAxis
 * @sa Joystick.SetVirtualButton
 * @sa Joystick.SetVirtualBall
 * @sa Joystick.SetVirtualHat
 * @sa Joystick.SetVirtualTouchpad
 */
inline void SendJoystickVirtualSensorData(JoystickParam joystick,
                                          SensorType type,
                                          Uint64 sensor_timestamp,
                                          const float* data,
                                          int num_values)
{
  CheckError(SDL_SendJoystickVirtualSensorData(
    joystick, type, sensor_timestamp, data, num_values));
}

inline void Joystick::SendVirtualSensorData(SensorType type,
                                            Uint64 sensor_timestamp,
                                            const float* data,
                                            int num_values)
{
  SDL::SendJoystickVirtualSensorData(
    m_resource, type, sensor_timestamp, data, num_values);
}

/**
 * Get the properties associated with a joystick.
 *
 * The following read-only properties are provided by SDL:
 *
 * - `prop::JoystickCap.MONO_LED_BOOLEAN`: true if this joystick has an LED that
 *   has adjustable brightness
 * - `prop::JoystickCap.RGB_LED_BOOLEAN`: true if this joystick has an LED that
 *   has adjustable color
 * - `prop::JoystickCap.PLAYER_LED_BOOLEAN`: true if this joystick has a player
 *   LED
 * - `prop::JoystickCap.RUMBLE_BOOLEAN`: true if this joystick has left/right
 *   rumble
 * - `prop::JoystickCap.TRIGGER_RUMBLE_BOOLEAN`: true if this joystick has
 *   simple trigger rumble
 *
 * @param joystick the Joystick obtained from JoystickID.OpenJoystick().
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesRef GetJoystickProperties(JoystickParam joystick)
{
  return {CheckError(SDL_GetJoystickProperties(joystick))};
}

inline PropertiesRef Joystick::GetProperties()
{
  return SDL::GetJoystickProperties(m_resource);
}

namespace prop::JoystickCap {

constexpr auto MONO_LED_BOOLEAN = SDL_PROP_JOYSTICK_CAP_MONO_LED_BOOLEAN;

constexpr auto RGB_LED_BOOLEAN = SDL_PROP_JOYSTICK_CAP_RGB_LED_BOOLEAN;

constexpr auto PLAYER_LED_BOOLEAN = SDL_PROP_JOYSTICK_CAP_PLAYER_LED_BOOLEAN;

constexpr auto RUMBLE_BOOLEAN = SDL_PROP_JOYSTICK_CAP_RUMBLE_BOOLEAN;

constexpr auto TRIGGER_RUMBLE_BOOLEAN =
  SDL_PROP_JOYSTICK_CAP_TRIGGER_RUMBLE_BOOLEAN;

} // namespace prop::JoystickCap

/**
 * Get the implementation dependent name of a joystick.
 *
 * @param joystick the Joystick obtained from JoystickID.OpenJoystick().
 * @returns the name of the selected joystick. If no name can be found, this
 *          function returns nullptr; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa JoystickID.GetJoystickNameForID
 */
inline const char* GetJoystickName(JoystickParam joystick)
{
  return SDL_GetJoystickName(joystick);
}

inline const char* Joystick::GetName()
{
  return SDL::GetJoystickName(m_resource);
}

/**
 * Get the implementation dependent path of a joystick.
 *
 * @param joystick the Joystick obtained from JoystickID.OpenJoystick().
 * @returns the path of the selected joystick. If no path can be found, this
 *          function returns nullptr; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa JoystickID.GetJoystickPathForID
 */
inline const char* GetJoystickPath(JoystickParam joystick)
{
  return SDL_GetJoystickPath(joystick);
}

inline const char* Joystick::GetPath()
{
  return SDL::GetJoystickPath(m_resource);
}

/**
 * Get the player index of an opened joystick.
 *
 * For XInput controllers this returns the XInput user index. Many joysticks
 * will not be able to supply this information.
 *
 * @param joystick the Joystick obtained from JoystickID.OpenJoystick().
 * @returns the player index, or -1 if it's not available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.SetPlayerIndex
 */
inline int GetJoystickPlayerIndex(JoystickParam joystick)
{
  return SDL_GetJoystickPlayerIndex(joystick);
}

inline int Joystick::GetPlayerIndex()
{
  return SDL::GetJoystickPlayerIndex(m_resource);
}

/**
 * Set the player index of an opened joystick.
 *
 * @param joystick the Joystick obtained from JoystickID.OpenJoystick().
 * @param player_index player index to assign to this joystick, or -1 to clear
 *                     the player index and turn off player LEDs.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetPlayerIndex
 */
inline void SetJoystickPlayerIndex(JoystickParam joystick, int player_index)
{
  CheckError(SDL_SetJoystickPlayerIndex(joystick, player_index));
}

inline void Joystick::SetPlayerIndex(int player_index)
{
  SDL::SetJoystickPlayerIndex(m_resource, player_index);
}

/**
 * Get the implementation-dependent GUID for the joystick.
 *
 * This function requires an open joystick.
 *
 * @param joystick the Joystick obtained from JoystickID.OpenJoystick().
 * @returns the GUID of the given joystick. If called on an invalid index, this
 *          function returns a zero GUID; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa JoystickID.GetJoystickGUIDForID
 * @sa GUID.ToString
 */
inline GUID GetJoystickGUID(JoystickParam joystick)
{
  return SDL_GetJoystickGUID(joystick);
}

inline GUID Joystick::GetGUID() { return SDL::GetJoystickGUID(m_resource); }

/**
 * Get the USB vendor ID of an opened joystick, if available.
 *
 * If the vendor ID isn't available this function returns 0.
 *
 * @param joystick the Joystick obtained from JoystickID.OpenJoystick().
 * @returns the USB vendor ID of the selected joystick, or 0 if unavailable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa JoystickID.GetJoystickVendorForID
 */
inline Uint16 GetJoystickVendor(JoystickParam joystick)
{
  return SDL_GetJoystickVendor(joystick);
}

inline Uint16 Joystick::GetVendor()
{
  return SDL::GetJoystickVendor(m_resource);
}

/**
 * Get the USB product ID of an opened joystick, if available.
 *
 * If the product ID isn't available this function returns 0.
 *
 * @param joystick the Joystick obtained from JoystickID.OpenJoystick().
 * @returns the USB product ID of the selected joystick, or 0 if unavailable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa JoystickID.GetJoystickProductForID
 */
inline Uint16 GetJoystickProduct(JoystickParam joystick)
{
  return SDL_GetJoystickProduct(joystick);
}

inline Uint16 Joystick::GetProduct()
{
  return SDL::GetJoystickProduct(m_resource);
}

/**
 * Get the product version of an opened joystick, if available.
 *
 * If the product version isn't available this function returns 0.
 *
 * @param joystick the Joystick obtained from JoystickID.OpenJoystick().
 * @returns the product version of the selected joystick, or 0 if unavailable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa JoystickID.GetJoystickProductVersionForID
 */
inline Uint16 GetJoystickProductVersion(JoystickParam joystick)
{
  return SDL_GetJoystickProductVersion(joystick);
}

inline Uint16 Joystick::GetProductVersion()
{
  return SDL::GetJoystickProductVersion(m_resource);
}

/**
 * Get the firmware version of an opened joystick, if available.
 *
 * If the firmware version isn't available this function returns 0.
 *
 * @param joystick the Joystick obtained from JoystickID.OpenJoystick().
 * @returns the firmware version of the selected joystick, or 0 if unavailable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Uint16 GetJoystickFirmwareVersion(JoystickParam joystick)
{
  return SDL_GetJoystickFirmwareVersion(joystick);
}

inline Uint16 Joystick::GetFirmwareVersion()
{
  return SDL::GetJoystickFirmwareVersion(m_resource);
}

/**
 * Get the serial number of an opened joystick, if available.
 *
 * Returns the serial number of the joystick, or nullptr if it is not available.
 *
 * @param joystick the Joystick obtained from JoystickID.OpenJoystick().
 * @returns the serial number of the selected joystick, or nullptr if
 *          unavailable.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetJoystickSerial(JoystickParam joystick)
{
  return SDL_GetJoystickSerial(joystick);
}

inline const char* Joystick::GetSerial()
{
  return SDL::GetJoystickSerial(m_resource);
}

/**
 * Get the type of an opened joystick.
 *
 * @param joystick the Joystick obtained from JoystickID.OpenJoystick().
 * @returns the JoystickType of the selected joystick.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa JoystickID.GetJoystickTypeForID
 */
inline JoystickType GetJoystickType(JoystickParam joystick)
{
  return SDL_GetJoystickType(joystick);
}

inline JoystickType Joystick::GetType()
{
  return SDL::GetJoystickType(m_resource);
}

/**
 * Get the device information encoded in a GUID structure.
 *
 * @param guid the GUID you wish to get info about.
 * @param vendor a pointer filled in with the device VID, or 0 if not available.
 * @param product a pointer filled in with the device PID, or 0 if not
 *                available.
 * @param version a pointer filled in with the device version, or 0 if not
 *                available.
 * @param crc16 a pointer filled in with a CRC used to distinguish different
 *              products with the same VID/PID, or 0 if not available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa JoystickID.GetJoystickGUIDForID
 */
inline void GetJoystickGUIDInfo(GUID guid,
                                Uint16* vendor,
                                Uint16* product,
                                Uint16* version,
                                Uint16* crc16)
{
  SDL_GetJoystickGUIDInfo(guid, vendor, product, version, crc16);
}

/**
 * Get the status of a specified joystick.
 *
 * @param joystick the joystick to query.
 * @returns true if the joystick has been opened, false if it has not; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool JoystickConnected(JoystickParam joystick)
{
  return SDL_JoystickConnected(joystick);
}

inline bool Joystick::Connected() { return SDL::JoystickConnected(m_resource); }

/**
 * Get the instance ID of an opened joystick.
 *
 * @param joystick an Joystick structure containing joystick information.
 * @returns the instance ID of the specified joystick on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline JoystickID GetJoystickID(JoystickParam joystick)
{
  return CheckError(SDL_GetJoystickID(joystick));
}

inline JoystickID Joystick::GetID() { return SDL::GetJoystickID(m_resource); }

/**
 * Get the number of general axis controls on a joystick.
 *
 * Often, the directional pad on a game controller will either look like 4
 * separate buttons or a POV hat, and not axes, but all of this is up to the
 * device and platform.
 *
 * @param joystick an Joystick structure containing joystick information.
 * @returns the number of axis controls/number of axes on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetAxis
 * @sa Joystick.GetNumBalls
 * @sa Joystick.GetNumButtons
 * @sa Joystick.GetNumHats
 */
inline int GetNumJoystickAxes(JoystickParam joystick)
{
  return CheckError(SDL_GetNumJoystickAxes(joystick));
}

inline int Joystick::GetNumAxes()
{
  return SDL::GetNumJoystickAxes(m_resource);
}

/**
 * Get the number of trackballs on a joystick.
 *
 * Joystick trackballs have only relative motion events associated with them and
 * their state cannot be polled.
 *
 * Most joysticks do not have trackballs.
 *
 * @param joystick an Joystick structure containing joystick information.
 * @returns the number of trackballs on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetBall
 * @sa Joystick.GetNumAxes
 * @sa Joystick.GetNumButtons
 * @sa Joystick.GetNumHats
 */
inline int GetNumJoystickBalls(JoystickParam joystick)
{
  return CheckError(SDL_GetNumJoystickBalls(joystick));
}

inline int Joystick::GetNumBalls()
{
  return SDL::GetNumJoystickBalls(m_resource);
}

/**
 * Get the number of POV hats on a joystick.
 *
 * @param joystick an Joystick structure containing joystick information.
 * @returns the number of POV hats on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetHat
 * @sa Joystick.GetNumAxes
 * @sa Joystick.GetNumBalls
 * @sa Joystick.GetNumButtons
 */
inline int GetNumJoystickHats(JoystickParam joystick)
{
  return CheckError(SDL_GetNumJoystickHats(joystick));
}

inline int Joystick::GetNumHats()
{
  return SDL::GetNumJoystickHats(m_resource);
}

/**
 * Get the number of buttons on a joystick.
 *
 * @param joystick an Joystick structure containing joystick information.
 * @returns the number of buttons on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetButton
 * @sa Joystick.GetNumAxes
 * @sa Joystick.GetNumBalls
 * @sa Joystick.GetNumHats
 */
inline int GetNumJoystickButtons(JoystickParam joystick)
{
  return CheckError(SDL_GetNumJoystickButtons(joystick));
}

inline int Joystick::GetNumButtons()
{
  return SDL::GetNumJoystickButtons(m_resource);
}

/**
 * Set the state of joystick event processing.
 *
 * If joystick events are disabled, you must call UpdateJoysticks() yourself and
 * check the state of the joystick when you want joystick information.
 *
 * @param enabled whether to process joystick events or not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa JoystickEventsEnabled
 * @sa UpdateJoysticks
 */
inline void SetJoystickEventsEnabled(bool enabled)
{
  SDL_SetJoystickEventsEnabled(enabled);
}

/**
 * Query the state of joystick event processing.
 *
 * If joystick events are disabled, you must call UpdateJoysticks() yourself and
 * check the state of the joystick when you want joystick information.
 *
 * @returns true if joystick events are being processed, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa SetJoystickEventsEnabled
 */
inline bool JoystickEventsEnabled() { return SDL_JoystickEventsEnabled(); }

/**
 * Update the current state of the open joysticks.
 *
 * This is called automatically by the event loop if any joystick events are
 * enabled.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void UpdateJoysticks() { SDL_UpdateJoysticks(); }

/**
 * Get the current state of an axis control on a joystick.
 *
 * SDL makes no promises about what part of the joystick any given axis refers
 * to. Your game should have some sort of configuration UI to let users specify
 * what each axis should be bound to. Alternately, SDL's higher-level Game
 * Controller API makes a great effort to apply order to this lower-level
 * interface, so you know that a specific axis is the "left thumb stick," etc.
 *
 * The value returned by Joystick.GetAxis() is a signed integer (-32768 to
 * 32767) representing the current position of the axis. It may be necessary to
 * impose certain tolerances on these values to account for jitter.
 *
 * @param joystick an Joystick structure containing joystick information.
 * @param axis the axis to query; the axis indices start at index 0.
 * @returns a 16-bit signed integer representing the current position of the
 *          axis or 0 on failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetNumAxes
 */
inline Sint16 GetJoystickAxis(JoystickParam joystick, int axis)
{
  return SDL_GetJoystickAxis(joystick, axis);
}

inline Sint16 Joystick::GetAxis(int axis)
{
  return SDL::GetJoystickAxis(m_resource, axis);
}

/**
 * Get the initial state of an axis control on a joystick.
 *
 * The state is a value ranging from -32768 to 32767.
 *
 * The axis indices start at index 0.
 *
 * @param joystick an Joystick structure containing joystick information.
 * @param axis the axis to query; the axis indices start at index 0.
 * @param state upon return, the initial value is supplied here.
 * @returns true if this axis has any initial value, or false if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool GetJoystickAxisInitialState(JoystickParam joystick,
                                        int axis,
                                        Sint16* state)
{
  return SDL_GetJoystickAxisInitialState(joystick, axis, state);
}

inline bool Joystick::GetAxisInitialState(int axis, Sint16* state)
{
  return SDL::GetJoystickAxisInitialState(m_resource, axis, state);
}

/**
 * Get the ball axis change since the last poll.
 *
 * Trackballs can only return relative motion since the last call to
 * Joystick.GetBall(), these motion deltas are placed into `dx` and `dy`.
 *
 * Most joysticks do not have trackballs.
 *
 * @param joystick the Joystick to query.
 * @param ball the ball index to query; ball indices start at index 0.
 * @param dx stores the difference in the x axis position since the last poll.
 * @param dy stores the difference in the y axis position since the last poll.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetNumBalls
 */
inline void GetJoystickBall(JoystickParam joystick, int ball, int* dx, int* dy)
{
  CheckError(SDL_GetJoystickBall(joystick, ball, dx, dy));
}

inline void Joystick::GetBall(int ball, int* dx, int* dy)
{
  SDL::GetJoystickBall(m_resource, ball, dx, dy);
}

/**
 * Get the current state of a POV hat on a joystick.
 *
 * The returned value will be one of the `SDL_HAT_*` values.
 *
 * @param joystick an Joystick structure containing joystick information.
 * @param hat the hat index to get the state from; indices start at index 0.
 * @returns the current hat position.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetNumHats
 */
inline Uint8 GetJoystickHat(JoystickParam joystick, int hat)
{
  return SDL_GetJoystickHat(joystick, hat);
}

inline Uint8 Joystick::GetHat(int hat)
{
  return SDL::GetJoystickHat(m_resource, hat);
}

/**
 * Get the current state of a button on a joystick.
 *
 * @param joystick an Joystick structure containing joystick information.
 * @param button the button index to get the state from; indices start at index
 *               0.
 * @returns true if the button is pressed, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Joystick.GetNumButtons
 */
inline bool GetJoystickButton(JoystickParam joystick, int button)
{
  return SDL_GetJoystickButton(joystick, button);
}

inline bool Joystick::GetButton(int button)
{
  return SDL::GetJoystickButton(m_resource, button);
}

/**
 * Start a rumble effect.
 *
 * Each call to this function cancels any previous rumble effect, and calling it
 * with 0 intensity stops any rumbling.
 *
 * This function requires you to process SDL events or call UpdateJoysticks() to
 * update rumble state.
 *
 * @param joystick the joystick to vibrate.
 * @param low_frequency_rumble the intensity of the low frequency (left) rumble
 *                             motor, from 0 to 0xFFFF.
 * @param high_frequency_rumble the intensity of the high frequency (right)
 *                              rumble motor, from 0 to 0xFFFF.
 * @param duration_ms the duration of the rumble effect, in milliseconds.
 * @returns true, or false if rumble isn't supported on this joystick.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool RumbleJoystick(JoystickParam joystick,
                           Uint16 low_frequency_rumble,
                           Uint16 high_frequency_rumble,
                           Uint32 duration_ms)
{
  return SDL_RumbleJoystick(
    joystick, low_frequency_rumble, high_frequency_rumble, duration_ms);
}

inline bool Joystick::Rumble(Uint16 low_frequency_rumble,
                             Uint16 high_frequency_rumble,
                             Uint32 duration_ms)
{
  return SDL::RumbleJoystick(
    m_resource, low_frequency_rumble, high_frequency_rumble, duration_ms);
}

/**
 * Start a rumble effect in the joystick's triggers.
 *
 * Each call to this function cancels any previous trigger rumble effect, and
 * calling it with 0 intensity stops any rumbling.
 *
 * Note that this is rumbling of the _triggers_ and not the game controller as a
 * whole. This is currently only supported on Xbox One controllers. If you want
 * the (more common) whole-controller rumble, use Joystick.Rumble() instead.
 *
 * This function requires you to process SDL events or call UpdateJoysticks() to
 * update rumble state.
 *
 * @param joystick the joystick to vibrate.
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
 * @sa Joystick.Rumble
 */
inline void RumbleJoystickTriggers(JoystickParam joystick,
                                   Uint16 left_rumble,
                                   Uint16 right_rumble,
                                   Uint32 duration_ms)
{
  CheckError(SDL_RumbleJoystickTriggers(
    joystick, left_rumble, right_rumble, duration_ms));
}

inline void Joystick::RumbleTriggers(Uint16 left_rumble,
                                     Uint16 right_rumble,
                                     Uint32 duration_ms)
{
  SDL::RumbleJoystickTriggers(
    m_resource, left_rumble, right_rumble, duration_ms);
}

/**
 * Update a joystick's LED color.
 *
 * An example of a joystick LED is the light on the back of a PlayStation 4's
 * DualShock 4 controller.
 *
 * For joysticks with a single color LED, the maximum of the RGB values will be
 * used as the LED brightness.
 *
 * @param joystick the joystick to update.
 * @param red the intensity of the red LED.
 * @param green the intensity of the green LED.
 * @param blue the intensity of the blue LED.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void SetJoystickLED(JoystickParam joystick,
                           Uint8 red,
                           Uint8 green,
                           Uint8 blue)
{
  CheckError(SDL_SetJoystickLED(joystick, red, green, blue));
}

inline void Joystick::SetLED(Uint8 red, Uint8 green, Uint8 blue)
{
  SDL::SetJoystickLED(m_resource, red, green, blue);
}

/**
 * Send a joystick specific effect packet.
 *
 * @param joystick the joystick to affect.
 * @param data the data to send to the joystick.
 * @param size the size of the data to send to the joystick.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void SendJoystickEffect(JoystickParam joystick,
                               const void* data,
                               int size)
{
  CheckError(SDL_SendJoystickEffect(joystick, data, size));
}

inline void Joystick::SendEffect(const void* data, int size)
{
  SDL::SendJoystickEffect(m_resource, data, size);
}

/**
 * Close a joystick previously opened with JoystickID.OpenJoystick().
 *
 * @param joystick the joystick device to close.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa JoystickID.OpenJoystick
 */
inline void CloseJoystick(JoystickRaw joystick) { SDL_CloseJoystick(joystick); }

inline void Joystick::Close() { CloseJoystick(release()); }

/**
 * Get the connection state of a joystick.
 *
 * @param joystick the joystick to query.
 * @returns the connection state on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline JoystickConnectionState GetJoystickConnectionState(
  JoystickParam joystick)
{
  return CheckError(SDL_GetJoystickConnectionState(joystick));
}

inline JoystickConnectionState Joystick::GetConnectionState()
{
  return SDL::GetJoystickConnectionState(m_resource);
}

/**
 * Get the battery state of a joystick.
 *
 * You should never take a battery status as absolute truth. Batteries
 * (especially failing batteries) are delicate hardware, and the values reported
 * here are best estimates based on what that hardware reports. It's not
 * uncommon for older batteries to lose stored power much faster than it
 * reports, or completely drain when reporting it has 20 percent left, etc.
 *
 * @param joystick the joystick to query.
 * @param percent a pointer filled in with the percentage of battery life left,
 *                between 0 and 100, or nullptr to ignore. This will be filled
 *                in with -1 we can't determine a value or there is no battery.
 * @returns the current battery state or `POWERSTATE_ERROR` on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PowerState GetJoystickPowerInfo(JoystickParam joystick, int* percent)
{
  return SDL_GetJoystickPowerInfo(joystick, percent);
}

inline PowerState Joystick::GetPowerInfo(int* percent)
{
  return SDL::GetJoystickPowerInfo(m_resource, percent);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_JOYSTICK_H_ */
