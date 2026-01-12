#ifndef SDL3PP_SENSOR_H_
#define SDL3PP_SENSOR_H_

#include <SDL3/SDL_sensor.h>
#include "SDL3pp_error.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"
#include "SDL3pp_version.h"

namespace SDL {

/**
 * @defgroup CategorySensor Sensors
 *
 * SDL sensor management.
 *
 * These APIs grant access to gyros and accelerometers on various platforms.
 *
 * In order to use these functions, Init() must have been called with the
 * INIT_SENSOR flag. This causes SDL to scan the system for sensors, and load
 * appropriate drivers.
 *
 * @{
 */

// Forward decl
struct Sensor;

/// Alias to raw representation for Sensor.
using SensorRaw = SDL_Sensor*;

// Forward decl
struct SensorRef;

/// Safely wrap Sensor for non owning parameters
struct SensorParam
{
  SensorRaw value; ///< parameter's SensorRaw

  /// Constructs from SensorRaw
  constexpr SensorParam(SensorRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr SensorParam(std::nullptr_t = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const SensorParam& other) const = default;

  /// Converts to underlying SensorRaw
  constexpr operator SensorRaw() const { return value; }
};

/**
 * This is a unique ID for a sensor for the time it is connected to the system,
 * and is never reused for the lifetime of the application.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 */
using SensorID = Uint32;

/**
 * The different sensors defined by SDL.
 *
 * Additional sensors may be available, using platform dependent semantics.
 *
 * Here are the additional Android sensors:
 *
 * https://developer.android.com/reference/android/hardware/SensorEvent.html#values
 *
 * Accelerometer sensor notes:
 *
 * The accelerometer returns the current acceleration in SI meters per second
 * squared. This measurement includes the force of gravity, so a device at rest
 * will have an value of STANDARD_GRAVITY away from the center of the earth,
 * which is a positive Y value.
 *
 * - `values[0]`: Acceleration on the x axis
 * - `values[1]`: Acceleration on the y axis
 * - `values[2]`: Acceleration on the z axis
 *
 * For phones and tablets held in natural orientation and game controllers held
 * in front of you, the axes are defined as follows:
 *
 * - -X ... +X : left ... right
 * - -Y ... +Y : bottom ... top
 * - -Z ... +Z : farther ... closer
 *
 * The accelerometer axis data is not changed when the device is rotated.
 *
 * Gyroscope sensor notes:
 *
 * The gyroscope returns the current rate of rotation in radians per second. The
 * rotation is positive in the counter-clockwise direction. That is, an observer
 * looking from a positive location on one of the axes would see positive
 * rotation on that axis when it appeared to be rotating counter-clockwise.
 *
 * - `values[0]`: Angular speed around the x axis (pitch)
 * - `values[1]`: Angular speed around the y axis (yaw)
 * - `values[2]`: Angular speed around the z axis (roll)
 *
 * For phones and tablets held in natural orientation and game controllers held
 * in front of you, the axes are defined as follows:
 *
 * - -X ... +X : left ... right
 * - -Y ... +Y : bottom ... top
 * - -Z ... +Z : farther ... closer
 *
 * The gyroscope axis data is not changed when the device is rotated.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa Display.GetCurrentOrientation
 */
using SensorType = SDL_SensorType;

constexpr SensorType SENSOR_INVALID =
  SDL_SENSOR_INVALID; ///< Returned for an invalid sensor

constexpr SensorType SENSOR_UNKNOWN =
  SDL_SENSOR_UNKNOWN; ///< Unknown sensor type

constexpr SensorType SENSOR_ACCEL = SDL_SENSOR_ACCEL; ///< Accelerometer

constexpr SensorType SENSOR_GYRO = SDL_SENSOR_GYRO; ///< Gyroscope

/// Accelerometer for left Joy-Con controller and Wii nunchuk
constexpr SensorType SENSOR_ACCEL_L = SDL_SENSOR_ACCEL_L;

constexpr SensorType SENSOR_GYRO_L =
  SDL_SENSOR_GYRO_L; ///< Gyroscope for left Joy-Con controller

constexpr SensorType SENSOR_ACCEL_R =
  SDL_SENSOR_ACCEL_R; ///< Accelerometer for right Joy-Con controller

constexpr SensorType SENSOR_GYRO_R =
  SDL_SENSOR_GYRO_R; ///< Gyroscope for right Joy-Con controller

#if SDL_VERSION_ATLEAST(3, 2, 22)

constexpr SensorType SENSOR_COUNT = SDL_SENSOR_COUNT; ///< SENSOR_COUNT

#endif // SDL_VERSION_ATLEAST(3, 2, 22)

/**
 * The opaque structure used to identify an opened SDL sensor.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class Sensor
{
  SensorRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Sensor(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from SensorParam.
   *
   * @param resource a SensorRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Sensor(const SensorRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr Sensor(const Sensor& other) noexcept = default;

public:
  /// Move constructor
  constexpr Sensor(Sensor&& other) noexcept
    : Sensor(other.release())
  {
  }

  constexpr Sensor(const SensorRef& other) = delete;

  constexpr Sensor(SensorRef&& other) = delete;

  /**
   * Open a sensor for use.
   *
   * @param instance_id the sensor instance ID.
   * @post an Sensor object or nullptr on failure; call GetError() for more
   *       information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  Sensor(SensorID instance_id)
    : m_resource(SDL_OpenSensor(instance_id))
  {
  }

  /// Destructor
  ~Sensor() { SDL_CloseSensor(m_resource); }

  /// Assignment operator.
  constexpr Sensor& operator=(Sensor&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Sensor& operator=(const Sensor& other) noexcept = default;

public:
  /// Retrieves underlying SensorRaw.
  constexpr SensorRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying SensorRaw and clear this.
  constexpr SensorRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Sensor& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to SensorParam
  constexpr operator SensorParam() const noexcept { return {m_resource}; }

  /**
   * Close a sensor previously opened with Sensor.Sensor().
   *
   * @since This function is available since SDL 3.2.0.
   */
  void Close();

  /**
   * Get the properties associated with a sensor.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties();

  /**
   * Get the implementation dependent name of a sensor.
   *
   * @returns the sensor name or nullptr on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  const char* GetName();

  /**
   * Get the type of a sensor.
   *
   * @returns the SensorType type, or `SENSOR_INVALID` if `sensor` is nullptr.
   *
   * @since This function is available since SDL 3.2.0.
   */
  SensorType GetType();

  /**
   * Get the platform dependent type of a sensor.
   *
   * @returns the sensor platform dependent type, or -1 if `sensor` is nullptr.
   *
   * @since This function is available since SDL 3.2.0.
   */
  int GetNonPortableType();

  /**
   * Get the instance ID of a sensor.
   *
   * @returns the sensor instance ID, or 0 on failure; call GetError() for more
   *          information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  SensorID GetID();

  /**
   * Get the current state of an opened sensor.
   *
   * The number of values and interpretation of the data is sensor dependent.
   *
   * @param data a pointer filled with the current sensor state.
   * @param num_values the number of values to write to data.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void GetData(float* data, int num_values);
};

/// Semi-safe reference for Sensor.
struct SensorRef : Sensor
{
  using Sensor::Sensor;

  /**
   * Constructs from SensorParam.
   *
   * @param resource a SensorRaw or Sensor.
   *
   * This does not takes ownership!
   */
  SensorRef(SensorParam resource) noexcept
    : Sensor(resource.value)
  {
  }

  /**
   * Constructs from SensorParam.
   *
   * @param resource a SensorRaw or Sensor.
   *
   * This does not takes ownership!
   */
  SensorRef(SensorRaw resource) noexcept
    : Sensor(resource)
  {
  }

  /// Copy constructor.
  constexpr SensorRef(const SensorRef& other) noexcept = default;

  /// Destructor
  ~SensorRef() { release(); }
};

/**
 * A constant to represent standard gravity for accelerometer sensors.
 *
 * The accelerometer returns the current acceleration in SI meters per second
 * squared. This measurement includes the force of gravity, so a device at rest
 * will have an value of STANDARD_GRAVITY away from the center of the earth,
 * which is a positive Y value.
 *
 * @since This constant is available since SDL 3.2.0.
 */
constexpr float STANDARD_GRAVITY = SDL_STANDARD_GRAVITY;

/**
 * Get a list of currently connected sensors.
 *
 * @returns a 0 terminated array of sensor instance IDs or nullptr on failure;
 *          call GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline OwnArray<SensorID> GetSensors()
{
  int count = 0;
  auto data = SDL_GetSensors(&count);
  return OwnArray<SensorID>(data, size_t(count));
}

/**
 * Get the implementation dependent name of a sensor.
 *
 * This can be called before any sensors are opened.
 *
 * @param instance_id the sensor instance ID.
 * @returns the sensor name, or nullptr if `instance_id` is not valid.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetSensorNameForID(SensorID instance_id)
{
  return SDL_GetSensorNameForID(instance_id);
}

/**
 * Get the type of a sensor.
 *
 * This can be called before any sensors are opened.
 *
 * @param instance_id the sensor instance ID.
 * @returns the SensorType, or `SENSOR_INVALID` if `instance_id` is not valid.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline SensorType GetSensorTypeForID(SensorID instance_id)
{
  return SDL_GetSensorTypeForID(instance_id);
}

/**
 * Get the platform dependent type of a sensor.
 *
 * This can be called before any sensors are opened.
 *
 * @param instance_id the sensor instance ID.
 * @returns the sensor platform dependent type, or -1 if `instance_id` is not
 *          valid.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int GetSensorNonPortableTypeForID(SensorID instance_id)
{
  return SDL_GetSensorNonPortableTypeForID(instance_id);
}

/**
 * Open a sensor for use.
 *
 * @param instance_id the sensor instance ID.
 * @returns an Sensor object or nullptr on failure; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline Sensor OpenSensor(SensorID instance_id) { return Sensor(instance_id); }

/**
 * Return the Sensor associated with an instance ID.
 *
 * @param instance_id the sensor instance ID.
 * @returns an Sensor object or nullptr on failure; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline SensorRef GetSensorFromID(SensorID instance_id)
{
  return {SDL_GetSensorFromID(instance_id)};
}

/**
 * Get the properties associated with a sensor.
 *
 * @param sensor the Sensor object.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesRef GetSensorProperties(SensorParam sensor)
{
  return {CheckError(SDL_GetSensorProperties(sensor))};
}

inline PropertiesRef Sensor::GetProperties()
{
  return SDL::GetSensorProperties(m_resource);
}

/**
 * Get the implementation dependent name of a sensor.
 *
 * @param sensor the Sensor object.
 * @returns the sensor name or nullptr on failure; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetSensorName(SensorParam sensor)
{
  return SDL_GetSensorName(sensor);
}

inline const char* Sensor::GetName() { return SDL::GetSensorName(m_resource); }

/**
 * Get the type of a sensor.
 *
 * @param sensor the Sensor object to inspect.
 * @returns the SensorType type, or `SENSOR_INVALID` if `sensor` is nullptr.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline SensorType GetSensorType(SensorParam sensor)
{
  return SDL_GetSensorType(sensor);
}

inline SensorType Sensor::GetType() { return SDL::GetSensorType(m_resource); }

/**
 * Get the platform dependent type of a sensor.
 *
 * @param sensor the Sensor object to inspect.
 * @returns the sensor platform dependent type, or -1 if `sensor` is nullptr.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int GetSensorNonPortableType(SensorParam sensor)
{
  return SDL_GetSensorNonPortableType(sensor);
}

inline int Sensor::GetNonPortableType()
{
  return SDL::GetSensorNonPortableType(m_resource);
}

/**
 * Get the instance ID of a sensor.
 *
 * @param sensor the Sensor object to inspect.
 * @returns the sensor instance ID, or 0 on failure; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline SensorID GetSensorID(SensorParam sensor)
{
  return SDL_GetSensorID(sensor);
}

inline SensorID Sensor::GetID() { return SDL::GetSensorID(m_resource); }

/**
 * Get the current state of an opened sensor.
 *
 * The number of values and interpretation of the data is sensor dependent.
 *
 * @param sensor the Sensor object to query.
 * @param data a pointer filled with the current sensor state.
 * @param num_values the number of values to write to data.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void GetSensorData(SensorParam sensor, float* data, int num_values)
{
  CheckError(SDL_GetSensorData(sensor, data, num_values));
}

inline void Sensor::GetData(float* data, int num_values)
{
  SDL::GetSensorData(m_resource, data, num_values);
}

/**
 * Close a sensor previously opened with Sensor.Sensor().
 *
 * @param sensor the Sensor object to close.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void CloseSensor(SensorRaw sensor) { SDL_CloseSensor(sensor); }

inline void Sensor::Close() { CloseSensor(release()); }

/**
 * Update the current state of the open sensors.
 *
 * This is called automatically by the event loop if sensor events are enabled.
 *
 * This needs to be called from the thread that initialized the sensor
 * subsystem.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void UpdateSensors() { SDL_UpdateSensors(); }

/// @}

} // namespace SDL

#endif /* SDL3PP_SENSOR_H_ */
