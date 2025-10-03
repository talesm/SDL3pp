#ifndef SDL3PP_HAPTIC_H_
#define SDL3PP_HAPTIC_H_

#include <SDL3/SDL_haptic.h>
#include "SDL3pp_error.h"
#include "SDL3pp_joystick.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryHaptic Category Haptic
 *
 * The SDL haptic subsystem manages haptic (force feedback) devices.
 *
 * The basic usage is as follows:
 *
 * - Initialize the subsystem (INIT_HAPTIC).
 * - Open a haptic device.
 * - Haptic.Haptic() to open from index.
 * - Haptic.Haptic() to open from an existing joystick.
 * - Create an effect (HapticEffect).
 * - Upload the effect with Haptic.CreateEffect().
 * - Run the effect with Haptic.RunEffect().
 * - (optional) Free the effect with Haptic.DestroyEffect().
 * - Close the haptic device with Haptic.Close().
 *
 * Simple rumble example:
 *
 * ```c
 *    Haptic *haptic = nullptr;
 *
 *    // Open the device
 *    HapticID *haptics = GetHaptics(nullptr);
 *    if (haptics) {
 *        haptic = Haptic.Haptic(haptics[0]);
 *        free(haptics);
 *    }
 *    if (haptic == nullptr)
 *       return;
 *
 *    // Initialize simple rumble
 *    if (!Haptic.InitRumble(haptic))
 *       return;
 *
 *    // Play effect at 50% strength for 2 seconds
 *    if (!Haptic.PlayRumble(haptic, 0.5, 2000))
 *       return;
 *    Delay(2000);
 *
 *    // Clean up
 *    Haptic.Close(haptic);
 * ```
 *
 * Complete example:
 *
 * ```c
 * bool test_haptic(Joystick *joystick)
 * {
 *    Haptic *haptic;
 *    HapticEffect effect;
 *    int effect_id;
 *
 *    // Open the device
 *    haptic = Haptic.Haptic(joystick);
 *    if (haptic == nullptr) return false; // Most likely joystick isn't haptic
 *
 *    // See if it can do sine waves
 *    if ((Haptic.GetFeatures(haptic) & HAPTIC_SINE)==0) {
 *       Haptic.Close(haptic); // No sine effect
 *       return false;
 *    }
 *
 *    // Create the effect
 *    memset(&effect, 0, sizeof(HapticEffect)); // 0 is safe default
 *    effect.type = HAPTIC_SINE;
 *    effect.periodic.direction.type = HAPTIC_POLAR; // Polar coordinates
 *    effect.periodic.direction.dir[0] = 18000; // Force comes from south
 *    effect.periodic.period = 1000; // 1000 ms
 *    effect.periodic.magnitude = 20000; // 20000/32767 strength
 *    effect.periodic.length = 5000; // 5 seconds long
 *    effect.periodic.attack_length = 1000; // Takes 1 second to get max
 * strength effect.periodic.fade_length = 1000; // Takes 1 second to fade away
 *
 *    // Upload the effect
 *    effect_id = Haptic.CreateEffect(haptic, &effect);
 *
 *    // Test the effect
 *    Haptic.RunEffect(haptic, effect_id, 1);
 *    Delay(5000); // Wait for the effect to finish
 *
 *    // We destroy the effect, although closing the device also does this
 *    Haptic.DestroyEffect(haptic, effect_id);
 *
 *    // Close the device
 *    Haptic.Close(haptic);
 *
 *    return true; // Success
 * }
 * ```
 *
 * Note that the SDL haptic subsystem is not thread-safe.
 *
 * @{
 */

// Forward decl
struct Haptic;

/// Alias to raw representation for Haptic.
using HapticRaw = SDL_Haptic*;

// Forward decl
struct HapticRef;

/// Safely wrap Haptic for non owning parameters
struct HapticParam
{
  HapticRaw value; ///< parameter's HapticRaw

  /// Constructs from HapticRaw
  constexpr HapticParam(HapticRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr HapticParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const HapticParam& other) const = default;

  /// Converts to underlying HapticRaw
  constexpr operator HapticRaw() const { return value; }
};

using HapticEffectType = Uint32;

/**
 * Constant effect supported.
 *
 * Constant haptic effect.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticCondition
 */
constexpr HapticEffectType HAPTIC_CONSTANT = SDL_HAPTIC_CONSTANT;

/**
 * Sine wave effect supported.
 *
 * Periodic haptic effect that simulates sine waves.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticPeriodic
 */
constexpr HapticEffectType HAPTIC_SINE = SDL_HAPTIC_SINE;

/**
 * Square wave effect supported.
 *
 * Periodic haptic effect that simulates square waves.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticPeriodic
 */
constexpr HapticEffectType HAPTIC_SQUARE = SDL_HAPTIC_SQUARE;

/**
 * Triangle wave effect supported.
 *
 * Periodic haptic effect that simulates triangular waves.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticPeriodic
 */
constexpr HapticEffectType HAPTIC_TRIANGLE = SDL_HAPTIC_TRIANGLE;

/**
 * Sawtoothup wave effect supported.
 *
 * Periodic haptic effect that simulates saw tooth up waves.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticPeriodic
 */
constexpr HapticEffectType HAPTIC_SAWTOOTHUP = SDL_HAPTIC_SAWTOOTHUP;

/**
 * Sawtoothdown wave effect supported.
 *
 * Periodic haptic effect that simulates saw tooth down waves.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticPeriodic
 */
constexpr HapticEffectType HAPTIC_SAWTOOTHDOWN = SDL_HAPTIC_SAWTOOTHDOWN;

/**
 * Ramp effect supported.
 *
 * Ramp haptic effect.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticRamp
 */
constexpr HapticEffectType HAPTIC_RAMP = SDL_HAPTIC_RAMP;

/**
 * Spring effect supported - uses axes position.
 *
 * Condition haptic effect that simulates a spring. Effect is based on the
 * axes position.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticCondition
 */
constexpr HapticEffectType HAPTIC_SPRING = SDL_HAPTIC_SPRING;

/**
 * Damper effect supported - uses axes velocity.
 *
 * Condition haptic effect that simulates dampening. Effect is based on the
 * axes velocity.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticCondition
 */
constexpr HapticEffectType HAPTIC_DAMPER = SDL_HAPTIC_DAMPER;

/**
 * Inertia effect supported - uses axes acceleration.
 *
 * Condition haptic effect that simulates inertia. Effect is based on the axes
 * acceleration.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticCondition
 */
constexpr HapticEffectType HAPTIC_INERTIA = SDL_HAPTIC_INERTIA;

/**
 * Friction effect supported - uses axes movement.
 *
 * Condition haptic effect that simulates friction. Effect is based on the
 * axes movement.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticCondition
 */
constexpr HapticEffectType HAPTIC_FRICTION = SDL_HAPTIC_FRICTION;

/**
 * Left/Right effect supported.
 *
 * Haptic effect for direct control over high/low frequency motors.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticLeftRight
 */
constexpr HapticEffectType HAPTIC_LEFTRIGHT = SDL_HAPTIC_LEFTRIGHT;

/**
 * Reserved for future use.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr HapticEffectType HAPTIC_RESERVED1 = SDL_HAPTIC_RESERVED1;

/**
 * Reserved for future use.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr HapticEffectType HAPTIC_RESERVED2 = SDL_HAPTIC_RESERVED2;

/**
 * Reserved for future use.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr HapticEffectType HAPTIC_RESERVED3 = SDL_HAPTIC_RESERVED3;

/**
 * Custom effect is supported.
 *
 * User defined custom haptic effect.
 *
 * @since This macro is available since SDL 3.2.0.
 */
constexpr HapticEffectType HAPTIC_CUSTOM = SDL_HAPTIC_CUSTOM;

/**
 * Device can set global gain.
 *
 * Device supports setting the global gain.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa Haptic.SetGain
 */
constexpr HapticEffectType HAPTIC_GAIN = SDL_HAPTIC_GAIN;

/**
 * Device can set autocenter.
 *
 * Device supports setting autocenter.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa Haptic.SetAutocenter
 */
constexpr HapticEffectType HAPTIC_AUTOCENTER = SDL_HAPTIC_AUTOCENTER;

/**
 * Device can be queried for effect status.
 *
 * Device supports querying effect status.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa Haptic.GetEffectStatus
 */
constexpr HapticEffectType HAPTIC_STATUS = SDL_HAPTIC_STATUS;

/**
 * Device can be paused.
 *
 * Devices supports being paused.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa Haptic.Pause
 * @sa Haptic.Resume
 */
constexpr HapticEffectType HAPTIC_PAUSE = SDL_HAPTIC_PAUSE;

using HapticDirectionType = Uint8;

/**
 * Uses polar coordinates for the direction.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticDirection
 */
constexpr HapticDirectionType HAPTIC_POLAR = SDL_HAPTIC_POLAR;

/**
 * Uses cartesian coordinates for the direction.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticDirection
 */
constexpr HapticDirectionType HAPTIC_CARTESIAN = SDL_HAPTIC_CARTESIAN;

/**
 * Uses spherical coordinates for the direction.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticDirection
 */
constexpr HapticDirectionType HAPTIC_SPHERICAL = SDL_HAPTIC_SPHERICAL;

/**
 * Use this value to play an effect on the steering wheel axis.
 *
 * This provides better compatibility across platforms and devices as SDL will
 * guess the correct axis.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa HapticDirection
 */
constexpr HapticDirectionType HAPTIC_STEERING_AXIS = SDL_HAPTIC_STEERING_AXIS;

/**
 * Used to play a device an infinite number of times.
 *
 * @since This macro is available since SDL 3.2.0.
 *
 * @sa Haptic.RunEffect
 */
constexpr Uint32 HAPTIC_INFINITY = SDL_HAPTIC_INFINITY;

/**
 * Structure that represents a haptic direction.
 *
 * This is the direction where the force comes from, instead of the direction
 * in which the force is exerted.
 *
 * Directions can be specified by:
 *
 * - HAPTIC_POLAR : Specified by polar coordinates.
 * - HAPTIC_CARTESIAN : Specified by cartesian coordinates.
 * - HAPTIC_SPHERICAL : Specified by spherical coordinates.
 *
 * Cardinal directions of the haptic device are relative to the positioning of
 * the device. North is considered to be away from the user.
 *
 * The following diagram represents the cardinal directions:
 *
 * ```
 *                .--.
 *                |__| .-------.
 *                |=.| |.-----.|
 *                |--| ||     ||
 *                |  | |'-----'|
 *                |__|~')_____('
 *                  [ COMPUTER ]
 *
 *
 *                    North (0,-1)
 *                        ^
 *                        |
 *                        |
 *  (-1,0)  West <----[ HAPTIC ]----> East (1,0)
 *                        |
 *                        |
 *                        v
 *                     South (0,1)
 *
 *
 *                     [ USER ]
 *                       \|||/
 *                       (o o)
 *                 ---ooO-(_)-Ooo---
 * ```
 *
 * If type is HAPTIC_POLAR, direction is encoded by hundredths of a degree
 * starting north and turning clockwise. HAPTIC_POLAR only uses the first
 * `dir` parameter. The cardinal directions would be:
 *
 * - North: 0 (0 degrees)
 * - East: 9000 (90 degrees)
 * - South: 18000 (180 degrees)
 * - West: 27000 (270 degrees)
 *
 * If type is HAPTIC_CARTESIAN, direction is encoded by three positions (X
 * axis, Y axis and Z axis (with 3 axes)). HAPTIC_CARTESIAN uses the first
 * three `dir` parameters. The cardinal directions would be:
 *
 * - North: 0,-1, 0
 * - East: 1, 0, 0
 * - South: 0, 1, 0
 * - West: -1, 0, 0
 *
 * The Z axis represents the height of the effect if supported, otherwise it's
 * unused. In cartesian encoding (1, 2) would be the same as (2, 4), you can
 * use any multiple you want, only the direction matters.
 *
 * If type is HAPTIC_SPHERICAL, direction is encoded by two rotations. The
 * first two `dir` parameters are used. The `dir` parameters are as follows
 * (all values are in hundredths of degrees):
 *
 * - Degrees from (1, 0) rotated towards (0, 1).
 * - Degrees towards (0, 0, 1) (device needs at least 3 axes).
 *
 * Example of force coming from the south with all encodings (force coming
 * from the south means the user will have to pull the stick to counteract):
 *
 * ```c
 *  HapticDirection direction;
 *
 *  // Cartesian directions
 *  direction.type = HAPTIC_CARTESIAN; // Using cartesian direction encoding.
 *  direction.dir[0] = 0; // X position
 *  direction.dir[1] = 1; // Y position
 *  // Assuming the device has 2 axes, we don't need to specify third parameter.
 *
 *  // Polar directions
 *  direction.type = HAPTIC_POLAR; // We'll be using polar direction encoding.
 *  direction.dir[0] = 18000; // Polar only uses first parameter
 *
 *  // Spherical coordinates
 *  direction.type = HAPTIC_SPHERICAL; // Spherical encoding
 *  direction.dir[0] = 9000; // Since we only have two axes we don't need more
 * parameters.
 * ```
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa HAPTIC_POLAR
 * @sa HAPTIC_CARTESIAN
 * @sa HAPTIC_SPHERICAL
 * @sa HAPTIC_STEERING_AXIS
 * @sa HapticEffect
 * @sa Haptic.GetNumAxes
 */
using HapticDirection = SDL_HapticDirection;

/**
 * A structure containing a template for a Constant effect.
 *
 * This struct is exclusively for the HAPTIC_CONSTANT effect.
 *
 * A constant effect applies a constant force in the specified direction to
 * the joystick.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa HAPTIC_CONSTANT
 * @sa HapticEffect
 */
using HapticConstant = SDL_HapticConstant;

/**
 * A structure containing a template for a Periodic effect.
 *
 * The struct handles the following effects:
 *
 * - HAPTIC_SINE
 * - HAPTIC_SQUARE
 * - HAPTIC_TRIANGLE
 * - HAPTIC_SAWTOOTHUP
 * - HAPTIC_SAWTOOTHDOWN
 *
 * A periodic effect consists in a wave-shaped effect that repeats itself over
 * time. The type determines the shape of the wave and the parameters
 * determine the dimensions of the wave.
 *
 * Phase is given by hundredth of a degree meaning that giving the phase a
 * value of 9000 will displace it 25% of its period. Here are sample values:
 *
 * - 0: No phase displacement.
 * - 9000: Displaced 25% of its period.
 * - 18000: Displaced 50% of its period.
 * - 27000: Displaced 75% of its period.
 * - 36000: Displaced 100% of its period, same as 0, but 0 is preferred.
 *
 * Examples:
 *
 * ```
 *   HAPTIC_SINE
 *     __      __      __      __
 *    /  \    /  \    /  \    /
 *   /    @__/    @__/    @__/
 *
 *   HAPTIC_SQUARE
 *    __    __    __    __    __
 *   |  |  |  |  |  |  |  |  |  |
 *   |  |__|  |__|  |__|  |__|  |
 *
 *   HAPTIC_TRIANGLE
 *     /\    /\    /\    /\    /\
 *    /  \  /  \  /  \  /  \  /
 *   /    \/    \/    \/    \/
 *
 *   HAPTIC_SAWTOOTHUP
 *     /|  /|  /|  /|  /|  /|  /|
 *    / | / | / | / | / | / | / |
 *   /  |/  |/  |/  |/  |/  |/  |
 *
 *   HAPTIC_SAWTOOTHDOWN
 *   \  |\  |\  |\  |\  |\  |\  |
 *    \ | \ | \ | \ | \ | \ | \ |
 *     \|  \|  \|  \|  \|  \|  \|
 * ```
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa HAPTIC_SINE
 * @sa HAPTIC_SQUARE
 * @sa HAPTIC_TRIANGLE
 * @sa HAPTIC_SAWTOOTHUP
 * @sa HAPTIC_SAWTOOTHDOWN
 * @sa HapticEffect
 */
using HapticPeriodic = SDL_HapticPeriodic;

/**
 * A structure containing a template for a Condition effect.
 *
 * The struct handles the following effects:
 *
 * - HAPTIC_SPRING: Effect based on axes position.
 * - HAPTIC_DAMPER: Effect based on axes velocity.
 * - HAPTIC_INERTIA: Effect based on axes acceleration.
 * - HAPTIC_FRICTION: Effect based on axes movement.
 *
 * Direction is handled by condition internals instead of a direction member.
 * The condition effect specific members have three parameters. The first
 * refers to the X axis, the second refers to the Y axis and the third refers
 * to the Z axis. The right terms refer to the positive side of the axis and
 * the left terms refer to the negative side of the axis. Please refer to the
 * HapticDirection diagram for which side is positive and which is
 * negative.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa HapticDirection
 * @sa HAPTIC_SPRING
 * @sa HAPTIC_DAMPER
 * @sa HAPTIC_INERTIA
 * @sa HAPTIC_FRICTION
 * @sa HapticEffect
 */
using HapticCondition = SDL_HapticCondition;

/**
 * A structure containing a template for a Ramp effect.
 *
 * This struct is exclusively for the HAPTIC_RAMP effect.
 *
 * The ramp effect starts at start strength and ends at end strength. It
 * augments in linear fashion. If you use attack and fade with a ramp the
 * effects get added to the ramp effect making the effect become quadratic
 * instead of linear.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa HAPTIC_RAMP
 * @sa HapticEffect
 */
using HapticRamp = SDL_HapticRamp;

/**
 * A structure containing a template for a Left/Right effect.
 *
 * This struct is exclusively for the HAPTIC_LEFTRIGHT effect.
 *
 * The Left/Right effect is used to explicitly control the large and small
 * motors, commonly found in modern game controllers. The small (right) motor
 * is high frequency, and the large (left) motor is low frequency.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa HAPTIC_LEFTRIGHT
 * @sa HapticEffect
 */
using HapticLeftRight = SDL_HapticLeftRight;

/**
 * A structure containing a template for the HAPTIC_CUSTOM effect.
 *
 * This struct is exclusively for the HAPTIC_CUSTOM effect.
 *
 * A custom force feedback effect is much like a periodic effect, where the
 * application can define its exact shape. You will have to allocate the data
 * yourself. Data should consist of channels * samples Uint16 samples.
 *
 * If channels is one, the effect is rotated using the defined direction.
 * Otherwise it uses the samples in data for the different axes.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa HAPTIC_CUSTOM
 * @sa HapticEffect
 */
using HapticCustom = SDL_HapticCustom;

/**
 * The generic template for any haptic effect.
 *
 * All values max at 32767 (0x7FFF). Signed values also can be negative. Time
 * values unless specified otherwise are in milliseconds.
 *
 * You can also pass HAPTIC_INFINITY to length instead of a 0-32767 value.
 * Neither delay, interval, attack_length nor fade_length support
 * HAPTIC_INFINITY. Fade will also not be used since effect never ends.
 *
 * Additionally, the HAPTIC_RAMP effect does not support a duration of
 * HAPTIC_INFINITY.
 *
 * Button triggers may not be supported on all devices, it is advised to not
 * use them if possible. Buttons start at index 1 instead of index 0 like the
 * joystick.
 *
 * If both attack_length and fade_level are 0, the envelope is not used,
 * otherwise both values are used.
 *
 * Common parts:
 *
 * ```c
 *  // Replay - All effects have this
 *  Uint32 length;        // Duration of effect (ms).
 *  Uint16 delay;         // Delay before starting effect.
 *
 *  // Trigger - All effects have this
 *  Uint16 button;        // Button that triggers effect.
 *  Uint16 interval;      // How soon before effect can be triggered again.
 *
 *  // Envelope - All effects except condition effects have this
 *  Uint16 attack_length; // Duration of the attack (ms).
 *  Uint16 attack_level;  // Level at the start of the attack.
 *  Uint16 fade_length;   // Duration of the fade out (ms).
 *  Uint16 fade_level;    // Level at the end of the fade.
 * ```
 *
 * Here we have an example of a constant effect evolution in time:
 *
 * ```
 *  Strength
 *  ^
 *  |
 *  |    effect level -->  _________________
 *  |                     /                 \
 *  |                    /                   \
 *  |                   /                     \
 *  |                  /                       \
 *  | attack_level --> |                        \
 *  |                  |                        |  <---  fade_level
 *  |
 *  +--------------------------------------------------> Time
 *                     [--]                 [---]
 *                     attack_length        fade_length
 *
 *  [------------------][-----------------------]
 *  delay               length
 * ```
 *
 * Note either the attack_level or the fade_level may be above the actual
 * effect level.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa HapticConstant
 * @sa HapticPeriodic
 * @sa HapticCondition
 * @sa HapticRamp
 * @sa HapticLeftRight
 * @sa HapticCustom
 */
using HapticEffect = SDL_HapticEffect;

/**
 * This is a unique ID for a haptic device for the time it is connected to the
 * system, and is never reused for the lifetime of the application.
 *
 * If the haptic device is disconnected and reconnected, it will get a new ID.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using HapticID = SDL_HapticID;

/**
 * The haptic structure used to identify an SDL haptic.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa Haptic.Haptic
 * @sa Haptic.Haptic
 * @sa Haptic.Close
 *
 * @cat resource
 */
class Haptic
{
  HapticRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Haptic() = default;

  /**
   * Constructs from HapticParam.
   *
   * @param resource a HapticRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Haptic(const HapticRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr Haptic(const Haptic& other) = delete;

  /// Move constructor
  constexpr Haptic(Haptic&& other)
    : Haptic(other.release())
  {
  }

  constexpr Haptic(const HapticRef& other) = delete;

  constexpr Haptic(HapticRef&& other) = delete;

  /**
   * Open a haptic device for use.
   *
   * The index passed as an argument refers to the N'th haptic device on this
   * system.
   *
   * When opening a haptic device, its gain will be set to maximum and
   * autocenter will be disabled. To modify these values use Haptic.SetGain()
   * and Haptic.SetAutocenter().
   *
   * @param instance_id the haptic device instance ID.
   * @post the device identifier or nullptr on failure; call GetError() for
   *          more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.Close
   * @sa GetHaptics
   * @sa Haptic.Haptic
   * @sa Haptic.OpenFromMouse
   * @sa Haptic.SetAutocenter
   * @sa Haptic.SetGain
   */
  Haptic(HapticID instance_id)
    : m_resource(SDL_OpenHaptic(instance_id))
  {
  }

  /**
   * Open a haptic device for use from a joystick device.
   *
   * You must still close the haptic device separately. It will not be closed
   * with the joystick.
   *
   * When opened from a joystick you should first close the haptic device before
   * closing the joystick device. If not, on some implementations the haptic
   * device will also get unallocated and you'll be unable to use force feedback
   * on that device.
   *
   * @param joystick the Joystick to create a haptic device from.
   * @post a valid haptic device identifier on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.Close
   * @sa IsJoystickHaptic
   */
  Haptic(JoystickParam joystick)
    : m_resource(CheckError(SDL_OpenHapticFromJoystick(joystick)))
  {
  }

  /**
   * Try to open a haptic device from the current mouse.
   *
   * @returns the haptic device identifier or nullptr on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.Close
   * @sa IsMouseHaptic
   */
  static Haptic OpenFromMouse();

  /// Destructor
  ~Haptic() { SDL_CloseHaptic(m_resource); }

  /// Assignment operator.
  Haptic& operator=(Haptic other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying HapticRaw.
  constexpr HapticRaw get() const { return m_resource; }

  /// Retrieves underlying HapticRaw and clear this.
  constexpr HapticRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Haptic& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to HapticParam
  constexpr operator HapticParam() const { return {m_resource}; }

  /**
   * Close a haptic device previously opened with Haptic.Haptic().
   *
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.Haptic
   */
  void Close();

  /**
   * Get the instance ID of an opened haptic device.
   *
   * @returns the instance ID of the specified haptic device on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  HapticID GetID();

  /**
   * Get the implementation dependent name of a haptic device.
   *
   * @returns the name of the selected haptic device. If no name can be found,
   *          this function returns nullptr; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetHapticNameForID
   */
  const char* GetName();

  /**
   * Get the number of effects a haptic device can store.
   *
   * On some platforms this isn't fully supported, and therefore is an
   * approximation. Always check to see if your created effect was actually
   * created and do not rely solely on Haptic.GetMaxEffects().
   *
   * @returns the number of effects the haptic device can store or a negative
   *          error code on failure; call GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.GetMaxEffectsPlaying
   * @sa Haptic.GetFeatures
   */
  int GetMaxEffects();

  /**
   * Get the number of effects a haptic device can play at the same time.
   *
   * This is not supported on all platforms, but will always return a value.
   *
   * @returns the number of effects the haptic device can play at the same time
   *          or -1 on failure; call GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.GetMaxEffects
   * @sa Haptic.GetFeatures
   */
  int GetMaxEffectsPlaying();

  /**
   * Get the haptic device's supported features in bitwise manner.
   *
   * @returns a list of supported haptic features in bitwise manner (OR'd), or 0
   *          on failure; call GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.EffectSupported
   * @sa Haptic.GetMaxEffects
   */
  Uint32 GetFeatures();

  /**
   * Get the number of haptic axes the device has.
   *
   * The number of haptic axes might be useful if working with the
   * HapticDirection effect.
   *
   * @returns the number of axes on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  int GetNumAxes();

  /**
   * Check to see if an effect is supported by a haptic device.
   *
   * @param effect the desired effect to query.
   * @returns true if the effect is supported or false if it isn't.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.CreateEffect
   * @sa Haptic.GetFeatures
   */
  bool EffectSupported(const HapticEffect& effect);

  /**
   * Create a new haptic effect on a specified device.
   *
   * @param effect an HapticEffect structure containing the properties of
   *               the effect to create.
   * @returns the ID of the effect on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.DestroyEffect
   * @sa Haptic.RunEffect
   * @sa Haptic.UpdateEffect
   */
  int CreateEffect(const HapticEffect& effect);

  /**
   * Update the properties of an effect.
   *
   * Can be used dynamically, although behavior when dynamically changing
   * direction may be strange. Specifically the effect may re-upload itself and
   * start playing from the start. You also cannot change the type either when
   * running Haptic.UpdateEffect().
   *
   * @param effect the identifier of the effect to update.
   * @param data an HapticEffect structure containing the new effect
   *             properties to use.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.CreateEffect
   * @sa Haptic.RunEffect
   */
  void UpdateEffect(int effect, const HapticEffect& data);

  /**
   * Run the haptic effect on its associated haptic device.
   *
   * To repeat the effect over and over indefinitely, set `iterations` to
   * `HAPTIC_INFINITY`. (Repeats the envelope - attack and fade.) To make
   * one instance of the effect last indefinitely (so the effect does not fade),
   * set the effect's `length` in its structure/union to `HAPTIC_INFINITY`
   * instead.
   *
   * @param effect the ID of the haptic effect to run.
   * @param iterations the number of iterations to run the effect; use
   *                   `HAPTIC_INFINITY` to repeat forever.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.GetEffectStatus
   * @sa Haptic.StopEffect
   * @sa Haptic.StopEffects
   */
  void RunEffect(int effect, Uint32 iterations);

  /**
   * Stop the haptic effect on its associated haptic device.
   *
   * @param effect the ID of the haptic effect to stop.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.RunEffect
   * @sa Haptic.StopEffects
   */
  void StopEffect(int effect);

  /**
   * Destroy a haptic effect on the device.
   *
   * This will stop the effect if it's running. Effects are automatically
   * destroyed when the device is closed.
   *
   * @param effect the ID of the haptic effect to destroy.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.CreateEffect
   */
  void DestroyEffect(int effect);

  /**
   * Get the status of the current effect on the specified haptic device.
   *
   * Device must support the HAPTIC_STATUS feature.
   *
   * @param effect the ID of the haptic effect to query its status.
   * @returns true if it is playing, false if it isn't playing or haptic status
   *          isn't supported.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.GetFeatures
   */
  bool GetEffectStatus(int effect);

  /**
   * Set the global gain of the specified haptic device.
   *
   * Device must support the HAPTIC_GAIN feature.
   *
   * The user may specify the maximum gain by setting the environment variable
   * `SDL_HAPTIC_GAIN_MAX` which should be between 0 and 100. All calls to
   * Haptic.SetGain() will scale linearly using `SDL_HAPTIC_GAIN_MAX` as the
   * maximum.
   *
   * @param gain value to set the gain to, should be between 0 and 100 (0 -
   *             100).
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.GetFeatures
   */
  void SetGain(int gain);

  /**
   * Set the global autocenter of the device.
   *
   * Autocenter should be between 0 and 100. Setting it to 0 will disable
   * autocentering.
   *
   * Device must support the HAPTIC_AUTOCENTER feature.
   *
   * @param autocenter value to set autocenter to (0-100).
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.GetFeatures
   */
  void SetAutocenter(int autocenter);

  /**
   * Pause a haptic device.
   *
   * Device must support the `HAPTIC_PAUSE` feature. Call Haptic.Resume()
   * to resume playback.
   *
   * Do not modify the effects nor add new ones while the device is paused. That
   * can cause all sorts of weird errors.
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.Resume
   */
  void Pause();

  /**
   * Resume a haptic device.
   *
   * Call to unpause after Haptic.Pause().
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.Pause
   */
  void Resume();

  /**
   * Stop all the currently playing effects on a haptic device.
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.RunEffect
   * @sa Haptic.StopEffects
   */
  void StopEffects();

  /**
   * Check whether rumble is supported on a haptic device.
   *
   * @returns true if the effect is supported or false if it isn't.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.InitRumble
   */
  bool RumbleSupported();

  /**
   * Initialize a haptic device for simple rumble playback.
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.PlayRumble
   * @sa Haptic.StopRumble
   * @sa Haptic.RumbleSupported
   */
  void InitRumble();

  /**
   * Run a simple rumble effect on a haptic device.
   *
   * @param strength strength of the rumble to play as a 0-1 float value.
   * @param length length of the rumble to play in milliseconds.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.InitRumble
   * @sa Haptic.StopRumble
   */
  void PlayRumble(float strength, Uint32 length);

  /**
   * Stop the simple rumble on a haptic device.
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Haptic.PlayRumble
   */
  void StopRumble();
};

/// Semi-safe reference for Haptic.
struct HapticRef : Haptic
{
  /**
   * Constructs from HapticParam.
   *
   * @param resource a HapticRaw or Haptic.
   *
   * This does not takes ownership!
   */
  HapticRef(HapticParam resource)
    : Haptic(resource.value)
  {
  }

  /// Copy constructor.
  HapticRef(const HapticRef& other)
    : Haptic(other.get())
  {
  }

  /// Destructor
  ~HapticRef() { release(); }
};

/**
 * Get a list of currently connected haptic devices.
 *
 * @param count a pointer filled in with the number of haptic devices
 *              returned, may be nullptr.
 * @returns a 0 terminated array of haptic device instance IDs or nullptr on
 *          failure; call GetError() for more information. This should be
 *          freed with free() when it is no longer needed.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.Haptic
 */
inline OwnArray<HapticID> GetHaptics() { return SDL_GetHaptics(); }

/**
 * Get the implementation dependent name of a haptic device.
 *
 * This can be called before any haptic devices are opened.
 *
 * @param instance_id the haptic device instance ID.
 * @returns the name of the selected haptic device. If no name can be found,
 *          this function returns nullptr; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.GetName
 * @sa Haptic.Haptic
 */
inline const char* GetHapticNameForID(HapticID instance_id)
{
  return SDL_GetHapticNameForID(instance_id);
}

/**
 * Open a haptic device for use.
 *
 * The index passed as an argument refers to the N'th haptic device on this
 * system.
 *
 * When opening a haptic device, its gain will be set to maximum and
 * autocenter will be disabled. To modify these values use Haptic.SetGain()
 * and Haptic.SetAutocenter().
 *
 * @param instance_id the haptic device instance ID.
 * @returns the device identifier or nullptr on failure; call GetError() for
 *          more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.Close
 * @sa GetHaptics
 * @sa Haptic.Haptic
 * @sa Haptic.OpenFromMouse
 * @sa Haptic.SetAutocenter
 * @sa Haptic.SetGain
 */
inline Haptic OpenHaptic(HapticID instance_id) { return Haptic(instance_id); }

/**
 * Get the Haptic associated with an instance ID, if it has been opened.
 *
 * @param instance_id the instance ID to get the Haptic for.
 * @returns an Haptic on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline HapticRef GetHapticFromID(HapticID instance_id)
{
  return CheckError(SDL_GetHapticFromID(instance_id));
}

/**
 * Get the instance ID of an opened haptic device.
 *
 * @param haptic the Haptic device to query.
 * @returns the instance ID of the specified haptic device on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline HapticID GetHapticID(HapticParam haptic)
{
  return CheckError(SDL_GetHapticID(haptic));
}

inline HapticID Haptic::GetID() { return SDL::GetHapticID(m_resource); }

/**
 * Get the implementation dependent name of a haptic device.
 *
 * @param haptic the Haptic obtained from JoystickID.OpenJoystick().
 * @returns the name of the selected haptic device. If no name can be found,
 *          this function returns nullptr; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetHapticNameForID
 */
inline const char* GetHapticName(HapticParam haptic)
{
  return SDL_GetHapticName(haptic);
}

inline const char* Haptic::GetName() { return SDL::GetHapticName(m_resource); }

/**
 * Query whether or not the current mouse has haptic capabilities.
 *
 * @returns true if the mouse is haptic or false if it isn't.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.OpenFromMouse
 */
inline bool IsMouseHaptic() { return SDL_IsMouseHaptic(); }

/**
 * Try to open a haptic device from the current mouse.
 *
 * @returns the haptic device identifier or nullptr on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.Close
 * @sa IsMouseHaptic
 */
inline Haptic OpenHapticFromMouse()
{
  return Haptic(SDL_OpenHapticFromMouse());
}

inline Haptic Haptic::OpenFromMouse() { return SDL::OpenHapticFromMouse(); }

/**
 * Query if a joystick has haptic features.
 *
 * @param joystick the Joystick to test for haptic capabilities.
 * @returns true if the joystick is haptic or false if it isn't.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.Haptic
 */
inline bool IsJoystickHaptic(JoystickParam joystick)
{
  return SDL_IsJoystickHaptic(joystick);
}

/**
 * Open a haptic device for use from a joystick device.
 *
 * You must still close the haptic device separately. It will not be closed
 * with the joystick.
 *
 * When opened from a joystick you should first close the haptic device before
 * closing the joystick device. If not, on some implementations the haptic
 * device will also get unallocated and you'll be unable to use force feedback
 * on that device.
 *
 * @param joystick the Joystick to create a haptic device from.
 * @returns a valid haptic device identifier on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.Close
 * @sa IsJoystickHaptic
 */
inline Haptic OpenHapticFromJoystick(JoystickParam joystick)
{
  return Haptic(joystick);
}

/**
 * Close a haptic device previously opened with Haptic.Haptic().
 *
 * @param haptic the Haptic device to close.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.Haptic
 */
inline void CloseHaptic(HapticRaw haptic) { SDL_CloseHaptic(haptic); }

inline void Haptic::Close() { CloseHaptic(release()); }

/**
 * Get the number of effects a haptic device can store.
 *
 * On some platforms this isn't fully supported, and therefore is an
 * approximation. Always check to see if your created effect was actually
 * created and do not rely solely on Haptic.GetMaxEffects().
 *
 * @param haptic the Haptic device to query.
 * @returns the number of effects the haptic device can store or a negative
 *          error code on failure; call GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.GetMaxEffectsPlaying
 * @sa Haptic.GetFeatures
 */
inline int GetMaxHapticEffects(HapticParam haptic)
{
  return SDL_GetMaxHapticEffects(haptic);
}

inline int Haptic::GetMaxEffects()
{
  return SDL::GetMaxHapticEffects(m_resource);
}

/**
 * Get the number of effects a haptic device can play at the same time.
 *
 * This is not supported on all platforms, but will always return a value.
 *
 * @param haptic the Haptic device to query maximum playing effects.
 * @returns the number of effects the haptic device can play at the same time
 *          or -1 on failure; call GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.GetMaxEffects
 * @sa Haptic.GetFeatures
 */
inline int GetMaxHapticEffectsPlaying(HapticParam haptic)
{
  return SDL_GetMaxHapticEffectsPlaying(haptic);
}

inline int Haptic::GetMaxEffectsPlaying()
{
  return SDL::GetMaxHapticEffectsPlaying(m_resource);
}

/**
 * Get the haptic device's supported features in bitwise manner.
 *
 * @param haptic the Haptic device to query.
 * @returns a list of supported haptic features in bitwise manner (OR'd), or 0
 *          on failure; call GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.EffectSupported
 * @sa Haptic.GetMaxEffects
 */
inline Uint32 GetHapticFeatures(HapticParam haptic)
{
  return SDL_GetHapticFeatures(haptic);
}

inline Uint32 Haptic::GetFeatures()
{
  return SDL::GetHapticFeatures(m_resource);
}

/**
 * Get the number of haptic axes the device has.
 *
 * The number of haptic axes might be useful if working with the
 * HapticDirection effect.
 *
 * @param haptic the Haptic device to query.
 * @returns the number of axes on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int GetNumHapticAxes(HapticParam haptic)
{
  return CheckError(SDL_GetNumHapticAxes(haptic));
}

inline int Haptic::GetNumAxes() { return SDL::GetNumHapticAxes(m_resource); }

/**
 * Check to see if an effect is supported by a haptic device.
 *
 * @param haptic the Haptic device to query.
 * @param effect the desired effect to query.
 * @returns true if the effect is supported or false if it isn't.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.CreateEffect
 * @sa Haptic.GetFeatures
 */
inline bool HapticEffectSupported(HapticParam haptic,
                                  const HapticEffect& effect)
{
  return SDL_HapticEffectSupported(haptic, &effect);
}

inline bool Haptic::EffectSupported(const HapticEffect& effect)
{
  return SDL::HapticEffectSupported(m_resource, effect);
}

/**
 * Create a new haptic effect on a specified device.
 *
 * @param haptic an Haptic device to create the effect on.
 * @param effect an HapticEffect structure containing the properties of
 *               the effect to create.
 * @returns the ID of the effect on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.DestroyEffect
 * @sa Haptic.RunEffect
 * @sa Haptic.UpdateEffect
 */
inline int CreateHapticEffect(HapticParam haptic, const HapticEffect& effect)
{
  return CheckError(SDL_CreateHapticEffect(haptic, &effect));
}

inline int Haptic::CreateEffect(const HapticEffect& effect)
{
  return SDL::CreateHapticEffect(m_resource, effect);
}

/**
 * Update the properties of an effect.
 *
 * Can be used dynamically, although behavior when dynamically changing
 * direction may be strange. Specifically the effect may re-upload itself and
 * start playing from the start. You also cannot change the type either when
 * running Haptic.UpdateEffect().
 *
 * @param haptic the Haptic device that has the effect.
 * @param effect the identifier of the effect to update.
 * @param data an HapticEffect structure containing the new effect
 *             properties to use.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.CreateEffect
 * @sa Haptic.RunEffect
 */
inline void UpdateHapticEffect(HapticParam haptic,
                               int effect,
                               const HapticEffect& data)
{
  CheckError(SDL_UpdateHapticEffect(haptic, effect, &data));
}

inline void Haptic::UpdateEffect(int effect, const HapticEffect& data)
{
  SDL::UpdateHapticEffect(m_resource, effect, data);
}

/**
 * Run the haptic effect on its associated haptic device.
 *
 * To repeat the effect over and over indefinitely, set `iterations` to
 * `HAPTIC_INFINITY`. (Repeats the envelope - attack and fade.) To make
 * one instance of the effect last indefinitely (so the effect does not fade),
 * set the effect's `length` in its structure/union to `HAPTIC_INFINITY`
 * instead.
 *
 * @param haptic the Haptic device to run the effect on.
 * @param effect the ID of the haptic effect to run.
 * @param iterations the number of iterations to run the effect; use
 *                   `HAPTIC_INFINITY` to repeat forever.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.GetEffectStatus
 * @sa Haptic.StopEffect
 * @sa Haptic.StopEffects
 */
inline void RunHapticEffect(HapticParam haptic, int effect, Uint32 iterations)
{
  CheckError(SDL_RunHapticEffect(haptic, effect, iterations));
}

inline void Haptic::RunEffect(int effect, Uint32 iterations)
{
  SDL::RunHapticEffect(m_resource, effect, iterations);
}

/**
 * Stop the haptic effect on its associated haptic device.
 *
 * @param haptic the Haptic device to stop the effect on.
 * @param effect the ID of the haptic effect to stop.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.RunEffect
 * @sa Haptic.StopEffects
 */
inline void StopHapticEffect(HapticParam haptic, int effect)
{
  CheckError(SDL_StopHapticEffect(haptic, effect));
}

inline void Haptic::StopEffect(int effect)
{
  SDL::StopHapticEffect(m_resource, effect);
}

/**
 * Destroy a haptic effect on the device.
 *
 * This will stop the effect if it's running. Effects are automatically
 * destroyed when the device is closed.
 *
 * @param haptic the Haptic device to destroy the effect on.
 * @param effect the ID of the haptic effect to destroy.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.CreateEffect
 */
inline void DestroyHapticEffect(HapticParam haptic, int effect)
{
  SDL_DestroyHapticEffect(haptic, effect);
}

inline void Haptic::DestroyEffect(int effect)
{
  SDL::DestroyHapticEffect(m_resource, effect);
}

/**
 * Get the status of the current effect on the specified haptic device.
 *
 * Device must support the HAPTIC_STATUS feature.
 *
 * @param haptic the Haptic device to query for the effect status on.
 * @param effect the ID of the haptic effect to query its status.
 * @returns true if it is playing, false if it isn't playing or haptic status
 *          isn't supported.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.GetFeatures
 */
inline bool GetHapticEffectStatus(HapticParam haptic, int effect)
{
  return SDL_GetHapticEffectStatus(haptic, effect);
}

inline bool Haptic::GetEffectStatus(int effect)
{
  return SDL::GetHapticEffectStatus(m_resource, effect);
}

/**
 * Set the global gain of the specified haptic device.
 *
 * Device must support the HAPTIC_GAIN feature.
 *
 * The user may specify the maximum gain by setting the environment variable
 * `SDL_HAPTIC_GAIN_MAX` which should be between 0 and 100. All calls to
 * Haptic.SetGain() will scale linearly using `SDL_HAPTIC_GAIN_MAX` as the
 * maximum.
 *
 * @param haptic the Haptic device to set the gain on.
 * @param gain value to set the gain to, should be between 0 and 100 (0 -
 *             100).
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.GetFeatures
 */
inline void SetHapticGain(HapticParam haptic, int gain)
{
  CheckError(SDL_SetHapticGain(haptic, gain));
}

inline void Haptic::SetGain(int gain) { SDL::SetHapticGain(m_resource, gain); }

/**
 * Set the global autocenter of the device.
 *
 * Autocenter should be between 0 and 100. Setting it to 0 will disable
 * autocentering.
 *
 * Device must support the HAPTIC_AUTOCENTER feature.
 *
 * @param haptic the Haptic device to set autocentering on.
 * @param autocenter value to set autocenter to (0-100).
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.GetFeatures
 */
inline void SetHapticAutocenter(HapticParam haptic, int autocenter)
{
  CheckError(SDL_SetHapticAutocenter(haptic, autocenter));
}

inline void Haptic::SetAutocenter(int autocenter)
{
  SDL::SetHapticAutocenter(m_resource, autocenter);
}

/**
 * Pause a haptic device.
 *
 * Device must support the `HAPTIC_PAUSE` feature. Call Haptic.Resume()
 * to resume playback.
 *
 * Do not modify the effects nor add new ones while the device is paused. That
 * can cause all sorts of weird errors.
 *
 * @param haptic the Haptic device to pause.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.Resume
 */
inline void PauseHaptic(HapticParam haptic)
{
  CheckError(SDL_PauseHaptic(haptic));
}

inline void Haptic::Pause() { SDL::PauseHaptic(m_resource); }

/**
 * Resume a haptic device.
 *
 * Call to unpause after Haptic.Pause().
 *
 * @param haptic the Haptic device to unpause.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.Pause
 */
inline void ResumeHaptic(HapticParam haptic)
{
  CheckError(SDL_ResumeHaptic(haptic));
}

inline void Haptic::Resume() { SDL::ResumeHaptic(m_resource); }

/**
 * Stop all the currently playing effects on a haptic device.
 *
 * @param haptic the Haptic device to stop.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.RunEffect
 * @sa Haptic.StopEffects
 */
inline void StopHapticEffects(HapticParam haptic)
{
  CheckError(SDL_StopHapticEffects(haptic));
}

inline void Haptic::StopEffects() { SDL::StopHapticEffects(m_resource); }

/**
 * Check whether rumble is supported on a haptic device.
 *
 * @param haptic haptic device to check for rumble support.
 * @returns true if the effect is supported or false if it isn't.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.InitRumble
 */
inline bool HapticRumbleSupported(HapticParam haptic)
{
  return SDL_HapticRumbleSupported(haptic);
}

inline bool Haptic::RumbleSupported()
{
  return SDL::HapticRumbleSupported(m_resource);
}

/**
 * Initialize a haptic device for simple rumble playback.
 *
 * @param haptic the haptic device to initialize for simple rumble playback.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.PlayRumble
 * @sa Haptic.StopRumble
 * @sa Haptic.RumbleSupported
 */
inline void InitHapticRumble(HapticParam haptic)
{
  CheckError(SDL_InitHapticRumble(haptic));
}

inline void Haptic::InitRumble() { SDL::InitHapticRumble(m_resource); }

/**
 * Run a simple rumble effect on a haptic device.
 *
 * @param haptic the haptic device to play the rumble effect on.
 * @param strength strength of the rumble to play as a 0-1 float value.
 * @param length length of the rumble to play in milliseconds.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.InitRumble
 * @sa Haptic.StopRumble
 */
inline void PlayHapticRumble(HapticParam haptic, float strength, Uint32 length)
{
  CheckError(SDL_PlayHapticRumble(haptic, strength, length));
}

inline void Haptic::PlayRumble(float strength, Uint32 length)
{
  SDL::PlayHapticRumble(m_resource, strength, length);
}

/**
 * Stop the simple rumble on a haptic device.
 *
 * @param haptic the haptic device to stop the rumble effect on.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Haptic.PlayRumble
 */
inline void StopHapticRumble(HapticParam haptic)
{
  CheckError(SDL_StopHapticRumble(haptic));
}

inline void Haptic::StopRumble() { SDL::StopHapticRumble(m_resource); }

/// @}

} // namespace SDL

#endif /* SDL3PP_HAPTIC_H_ */
