#ifndef SDL3PP_CAMERA_H_
#define SDL3PP_CAMERA_H_

#include <SDL3/SDL_camera.h>
#include "SDL3pp_error.h"
#include "SDL3pp_pixels.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"
#include "SDL3pp_surface.h"

namespace SDL {

/**
 * @defgroup CategoryCamera Camera Support
 *
 * Video capture for the SDL library.
 *
 * This API lets apps read input from video sources, like webcams. Camera
 * devices can be enumerated, queried, and opened. Once opened, it will provide
 * Surface objects as new frames of video come in. These surfaces can be
 * uploaded to an Texture or processed as pixels in memory.
 *
 * Several platforms will alert the user if an app tries to access a camera, and
 * some will present a UI asking the user if your application should be allowed
 * to obtain images at all, which they can deny. A successfully opened camera
 * will not provide images until permission is granted. Applications, after
 * opening a camera device, can see if they were granted access by either
 * polling with the Camera.GetPermissionState() function, or waiting for an
 * EVENT_CAMERA_DEVICE_APPROVED or EVENT_CAMERA_DEVICE_DENIED event. Platforms
 * that don't have any user approval process will report approval immediately.
 *
 * Note that SDL cameras only provide video as individual frames; they will not
 * provide full-motion video encoded in a movie file format, although an app is
 * free to encode the acquired frames into any format it likes. It also does not
 * provide audio from the camera hardware through this API; not only do many
 * webcams not have microphones at all, many people--from streamers to people on
 * Zoom calls--will want to use a separate microphone regardless of the camera.
 * In any case, recorded audio will be available through SDL's audio API no
 * matter what hardware provides the microphone.
 *
 * ## Camera gotchas
 *
 * Consumer-level camera hardware tends to take a little while to warm up, once
 * the device has been opened. Generally most camera apps have some sort of UI
 * to take a picture (a button to snap a pic while a preview is showing, some
 * sort of multi-second countdown for the user to pose, like a photo booth),
 * which puts control in the users' hands, or they are intended to stay on for
 * long times (Pokemon Go, etc).
 *
 * It's not uncommon that a newly-opened camera will provide a couple of
 * completely black frames, maybe followed by some under-exposed images. If
 * taking a single frame automatically, or recording video from a camera's input
 * without the user initiating it from a preview, it could be wise to drop the
 * first several frames (if not the first several _seconds_ worth of frames!)
 * before using images from a camera.
 *
 * @{
 */

// Forward decl
struct Camera;

/// Alias to raw representation for Camera.
using CameraRaw = SDL_Camera*;

// Forward decl
struct CameraRef;

/// Safely wrap Camera for non owning parameters
struct CameraParam
{
  CameraRaw value; ///< parameter's CameraRaw

  /// Constructs from CameraRaw
  constexpr CameraParam(CameraRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr CameraParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const CameraParam& other) const = default;

  /// Converts to underlying CameraRaw
  constexpr operator CameraRaw() const { return value; }
};

// Forward decl
struct CameraFrame;

/**
 * This is a unique ID for a camera device for the time it is connected to the
 * system, and is never reused for the lifetime of the application.
 *
 * If the device is disconnected and reconnected, it will get a new ID.
 *
 * The value 0 is an invalid ID.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa GetCameras
 */
using CameraID = SDL_CameraID;

/**
 * The details of an output format for a camera device.
 *
 * Cameras often support multiple formats; each one will be encapsulated in this
 * struct.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa GetCameraSupportedFormats
 * @sa Camera.GetFormat
 */
using CameraSpec = SDL_CameraSpec;

/**
 * The position of camera in relation to system device.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa GetCameraPosition
 */
using CameraPosition = SDL_CameraPosition;

constexpr CameraPosition CAMERA_POSITION_UNKNOWN =
  SDL_CAMERA_POSITION_UNKNOWN; ///< CAMERA_POSITION_UNKNOWN

constexpr CameraPosition CAMERA_POSITION_FRONT_FACING =
  SDL_CAMERA_POSITION_FRONT_FACING; ///< CAMERA_POSITION_FRONT_FACING

constexpr CameraPosition CAMERA_POSITION_BACK_FACING =
  SDL_CAMERA_POSITION_BACK_FACING; ///< CAMERA_POSITION_BACK_FACING

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * The current state of a request for camera access.
 *
 * @since This enum is available since SDL 3.4.0.
 *
 * @sa Camera.GetPermissionState
 */
using CameraPermissionState = SDL_CameraPermissionState;

constexpr CameraPermissionState CAMERA_PERMISSION_STATE_DENIED =
  SDL_CAMERA_PERMISSION_STATE_DENIED; ///< CAMERA_PERMISSION_STATE_DENIED

constexpr CameraPermissionState CAMERA_PERMISSION_STATE_PENDING =
  SDL_CAMERA_PERMISSION_STATE_PENDING; ///< CAMERA_PERMISSION_STATE_PENDING

constexpr CameraPermissionState CAMERA_PERMISSION_STATE_APPROVED =
  SDL_CAMERA_PERMISSION_STATE_APPROVED; ///< CAMERA_PERMISSION_STATE_APPROVED
#else

/**
 * The current state of a request for camera access.
 *
 * @since This enum is available since SDL 3.4.0.
 *
 * @sa Camera.GetPermissionState
 */
using CameraPermissionState = int;

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * The opaque structure used to identify an opened SDL camera.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class Camera
{
  CameraRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Camera(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from CameraParam.
   *
   * @param resource a CameraRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Camera(const CameraRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr Camera(const Camera& other) noexcept = default;

public:
  /// Move constructor
  constexpr Camera(Camera&& other) noexcept
    : Camera(other.release())
  {
  }

  constexpr Camera(const CameraRef& other) = delete;

  constexpr Camera(CameraRef&& other) = delete;

  /**
   * Open a video recording device (a "camera").
   *
   * You can open the device with any reasonable spec, and if the hardware can't
   * directly support it, it will convert data seamlessly to the requested
   * format. This might incur overhead, including scaling of image data.
   *
   * If you would rather accept whatever format the device offers, you can pass
   * a nullptr spec here and it will choose one for you (and you can use
   * Surface's conversion/scaling functions directly if necessary).
   *
   * You can call Camera.GetFormat() to get the actual data format if passing a
   * nullptr spec here. You can see the exact specs a device can support without
   * conversion with GetCameraSupportedFormats().
   *
   * SDL will not attempt to emulate framerate; it will try to set the hardware
   * to the rate closest to the requested speed, but it won't attempt to limit
   * or duplicate frames artificially; call Camera.GetFormat() to see the actual
   * framerate of the opened the device, and check your timestamps if this is
   * crucial to your app!
   *
   * Note that the camera is not usable until the user approves its use! On some
   * platforms, the operating system will prompt the user to permit access to
   * the camera, and they can choose Yes or No at that point. Until they do, the
   * camera will not be usable. The app should either wait for an
   * EVENT_CAMERA_DEVICE_APPROVED (or EVENT_CAMERA_DEVICE_DENIED) event, or poll
   * Camera.GetPermissionState() occasionally until it returns non-zero. On
   * platforms that don't require explicit user approval (and perhaps in places
   * where the user previously permitted access), the approval event might come
   * immediately, but it might come seconds, minutes, or hours later!
   *
   * @param instance_id the camera device instance ID.
   * @param spec the desired format for data the device will provide. Can be
   *             std::nullopt.
   * @post an Camera object on success..
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetCameras
   * @sa Camera.GetFormat
   */
  Camera(CameraID instance_id, OptionalRef<const CameraSpec> spec = {})
    : m_resource(SDL_OpenCamera(instance_id, spec))
  {
  }

  /// Destructor
  ~Camera() { SDL_CloseCamera(m_resource); }

  /// Assignment operator.
  constexpr Camera& operator=(Camera&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Camera& operator=(const Camera& other) noexcept = default;

public:
  /// Retrieves underlying CameraRaw.
  constexpr CameraRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying CameraRaw and clear this.
  constexpr CameraRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Camera& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to CameraParam
  constexpr operator CameraParam() const noexcept { return {m_resource}; }

  /**
   * Use this function to shut down camera processing and close the camera
   * device.
   *
   * @threadsafety It is safe to call this function from any thread, but no
   *               thread may reference `device` once this function is called.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Camera.Camera
   */
  void Close();

  /**
   * Query if camera access has been approved by the user.
   *
   * Cameras will not function between when the device is opened by the app and
   * when the user permits access to the hardware. On some platforms, this
   * presents as a popup dialog where the user has to explicitly approve access;
   * on others the approval might be implicit and not alert the user at all.
   *
   * This function can be used to check the status of that approval. It will
   * return CAMERA_PERMISSION_STATE_PENDING if waiting for user response,
   * CAMERA_PERMISSION_STATE_APPROVED if the camera is approved for use, and
   * CAMERA_PERMISSION_STATE_DENIED if the user denied access.
   *
   * Instead of polling with this function, you can wait for a
   * EVENT_CAMERA_DEVICE_APPROVED (or EVENT_CAMERA_DEVICE_DENIED) event in the
   * standard SDL event loop, which is guaranteed to be sent once when
   * permission to use the camera is decided.
   *
   * If a camera is declined, there's nothing to be done but call Camera.Close()
   * to dispose of it.
   *
   * @returns an CameraPermissionState value indicating if access is granted, or
   *          `CAMERA_PERMISSION_STATE_PENDING` if the decision is still
   *          pending.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Camera.Camera
   * @sa Camera.Close
   */
  CameraPermissionState GetPermissionState();

  /**
   * Get the instance ID of an opened camera.
   *
   * @returns the instance ID of the specified camera on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Camera.Camera
   */
  CameraID GetID();

  /**
   * Get the properties associated with an opened camera.
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
   * Get the spec that a camera is using when generating images.
   *
   * Note that this might not be the native format of the hardware, as SDL might
   * be converting to this format behind the scenes.
   *
   * If the system is waiting for the user to approve access to the camera, as
   * some platforms require, this will return false, but this isn't necessarily
   * a fatal error; you should either wait for an EVENT_CAMERA_DEVICE_APPROVED
   * (or EVENT_CAMERA_DEVICE_DENIED) event, or poll Camera.GetPermissionState()
   * occasionally until it returns non-zero.
   *
   * @returns the CameraSpec or std::nullopt if waiting for user approval.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Camera.Camera
   */
  std::optional<CameraSpec> GetFormat();

  /**
   * Acquire a frame.
   *
   * The frame is a memory pointer to the image data, whose size and format are
   * given by the spec requested when opening the device.
   *
   * This is a non blocking API. If there is a frame available, a non-nullptr
   * surface is returned, and timestampNS will be filled with a non-zero value.
   *
   * Note that an error case can also return nullptr, but a nullptr by itself is
   * normal and just signifies that a new frame is not yet available. Note that
   * even if a camera device fails outright (a USB camera is unplugged while in
   * use, etc), SDL will send an event separately to notify the app, but
   * continue to provide blank frames at ongoing intervals until Camera.Close()
   * is called, so real failure here is almost always an out of memory
   * condition.
   *
   * After use, the frame should be released with Camera.ReleaseFrame(). If you
   * don't do this, the system may stop providing more video!
   *
   * Do not call Surface.Destroy() on the returned surface! It must be given
   * back to the camera subsystem with Camera.ReleaseFrame!
   *
   * If the system is waiting for the user to approve access to the camera, as
   * some platforms require, this will return nullptr (no frames available); you
   * should either wait for an EVENT_CAMERA_DEVICE_APPROVED (or
   * EVENT_CAMERA_DEVICE_DENIED) event, or poll Camera.GetPermissionState()
   * occasionally until it returns non-zero.
   *
   * @param timestampNS a pointer filled in with the frame's timestamp, or 0 on
   *                    error. Can be nullptr.
   * @returns a new frame of video on success, nullptr if none is currently
   *          available.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Camera.ReleaseFrame
   */
  CameraFrame AcquireFrame(Uint64* timestampNS = nullptr);

  /**
   * Release a frame of video acquired from a camera.
   *
   * Let the back-end re-use the internal buffer for camera.
   *
   * This function _must_ be called only on surface objects returned by
   * Camera.AcquireFrame(). This function should be called as quickly as
   * possible after acquisition, as SDL keeps a small FIFO queue of surfaces for
   * video frames; if surfaces aren't released in a timely manner, SDL may drop
   * upcoming video frames from the camera.
   *
   * If the app needs to keep the surface for a significant time, they should
   * make a copy of it and release the original.
   *
   * The app should not use the surface again after calling this function;
   * assume the surface is freed and the pointer is invalid.
   *
   * @param lock the video frame surface to release.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Camera.AcquireFrame
   */
  void ReleaseFrame(CameraFrame&& lock);
};

/// Semi-safe reference for Camera.
struct CameraRef : Camera
{
  using Camera::Camera;

  /**
   * Constructs from CameraParam.
   *
   * @param resource a CameraRaw or Camera.
   *
   * This does not takes ownership!
   */
  CameraRef(CameraParam resource) noexcept
    : Camera(resource.value)
  {
  }

  /**
   * Constructs from CameraParam.
   *
   * @param resource a CameraRaw or Camera.
   *
   * This does not takes ownership!
   */
  CameraRef(CameraRaw resource) noexcept
    : Camera(resource)
  {
  }

  /// Copy constructor.
  constexpr CameraRef(const CameraRef& other) noexcept = default;

  /// Destructor
  ~CameraRef() { release(); }
};

/// Camera Frame.
class CameraFrame : public Surface
{
  CameraRef m_lock;

public:
  /**
   * Acquire a frame.
   *
   * The frame is a memory pointer to the image data, whose size and format are
   * given by the spec requested when opening the device.
   *
   * This is a non blocking API. If there is a frame available, a non-nullptr
   * surface is returned, and timestampNS will be filled with a non-zero value.
   *
   * Note that an error case can also return nullptr, but a nullptr by itself is
   * normal and just signifies that a new frame is not yet available. Note that
   * even if a camera device fails outright (a USB camera is unplugged while in
   * use, etc), SDL will send an event separately to notify the app, but
   * continue to provide blank frames at ongoing intervals until Camera.Close()
   * is called, so real failure here is almost always an out of memory
   * condition.
   *
   * After use, the frame should be released with Camera.ReleaseFrame(). If you
   * don't do this, the system may stop providing more video!
   *
   * Do not call Surface.Destroy() on the returned surface! It must be given
   * back to the camera subsystem with Camera.ReleaseFrame!
   *
   * If the system is waiting for the user to approve access to the camera, as
   * some platforms require, this will return nullptr (no frames available); you
   * should either wait for an EVENT_CAMERA_DEVICE_APPROVED (or
   * EVENT_CAMERA_DEVICE_DENIED) event, or poll Camera.GetPermissionState()
   * occasionally until it returns non-zero.
   *
   * @param resource opened camera device.
   * @param timestampNS a pointer filled in with the frame's timestamp, or 0 on
   *                    error. Can be nullptr.
   * @post a new frame of video on success, nullptr if none is currently
   *       available.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Camera.ReleaseFrame
   */
  CameraFrame(CameraRef resource, Uint64* timestampNS = nullptr);

  /// Copy constructor
  CameraFrame(const CameraFrame& other) = delete;

  /// Move constructor
  constexpr CameraFrame(CameraFrame&& other) noexcept
    : Surface(std::move(other))
    , m_lock(other.m_lock)
  {
    other.m_lock = {};
  }

  /**
   * Release a frame of video acquired from a camera.
   *
   * Let the back-end re-use the internal buffer for camera.
   *
   * This function _must_ be called only on surface objects returned by
   * Camera.AcquireFrame(). This function should be called as quickly as
   * possible after acquisition, as SDL keeps a small FIFO queue of surfaces for
   * video frames; if surfaces aren't released in a timely manner, SDL may drop
   * upcoming video frames from the camera.
   *
   * If the app needs to keep the surface for a significant time, they should
   * make a copy of it and release the original.
   *
   * The app should not use the surface again after calling this function;
   * assume the surface is freed and the pointer is invalid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Camera.AcquireFrame
   */
  ~CameraFrame() { reset(); }

  CameraFrame& operator=(const CameraFrame& other) = delete;

  /// Assignment operator
  CameraFrame& operator=(CameraFrame&& other) noexcept
  {
    std::swap(m_lock, other.m_lock);
    return *this;
  }

  /// True if not locked.
  constexpr operator bool() const
  {
    return bool(m_lock) && Surface::operator bool();
  }

  /**
   * Release a frame of video acquired from a camera.
   *
   * Let the back-end re-use the internal buffer for camera.
   *
   * This function _must_ be called only on surface objects returned by
   * Camera.AcquireFrame(). This function should be called as quickly as
   * possible after acquisition, as SDL keeps a small FIFO queue of surfaces for
   * video frames; if surfaces aren't released in a timely manner, SDL may drop
   * upcoming video frames from the camera.
   *
   * If the app needs to keep the surface for a significant time, they should
   * make a copy of it and release the original.
   *
   * The app should not use the surface again after calling this function;
   * assume the surface is freed and the pointer is invalid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa Camera.AcquireFrame
   */
  void reset();

  /// Get the reference to locked resource.
  CameraRef get() { return m_lock; }

  /// Releases the lock without unlocking.
  void release()
  {
    Surface::release();
    m_lock.release();
  }
};

/**
 * Use this function to get the number of built-in camera drivers.
 *
 * This function returns a hardcoded number. This never returns a negative
 * value; if there are no drivers compiled into this build of SDL, this function
 * returns zero. The presence of a driver in this list does not mean it will
 * function, it just means SDL is capable of interacting with that interface.
 * For example, a build of SDL might have v4l2 support, but if there's no kernel
 * support available, SDL's v4l2 driver would fail if used.
 *
 * By default, SDL tries all drivers, in its preferred order, until one is found
 * to be usable.
 *
 * @returns the number of built-in camera drivers.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetCameraDriver
 */
inline int GetNumCameraDrivers() { return SDL_GetNumCameraDrivers(); }

/**
 * Use this function to get the name of a built in camera driver.
 *
 * The list of camera drivers is given in the order that they are normally
 * initialized by default; the drivers that seem more reasonable to choose first
 * (as far as the SDL developers believe) are earlier in the list.
 *
 * The names of drivers are all simple, low-ASCII identifiers, like "v4l2",
 * "coremedia" or "android". These never have Unicode characters, and are not
 * meant to be proper names.
 *
 * @param index the index of the camera driver; the value ranges from 0 to
 *              GetNumCameraDrivers() - 1.
 * @returns the name of the camera driver at the requested index, or nullptr if
 *          an invalid index was specified.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetNumCameraDrivers
 */
inline const char* GetCameraDriver(int index)
{
  return SDL_GetCameraDriver(index);
}

/**
 * Get the name of the current camera driver.
 *
 * The names of drivers are all simple, low-ASCII identifiers, like "v4l2",
 * "coremedia" or "android". These never have Unicode characters, and are not
 * meant to be proper names.
 *
 * @returns the name of the current camera driver or nullptr if no driver has
 *          been initialized.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetCurrentCameraDriver()
{
  return SDL_GetCurrentCameraDriver();
}

/**
 * Get a list of currently connected camera devices.
 *
 * @returns a 0 terminated array of camera instance IDs or nullptr on failure;
 *          call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Camera.Camera
 */
inline OwnArray<CameraID> GetCameras()
{
  int count;
  auto data = SDL_GetCameras(&count);
  return OwnArray<CameraID>(data, count);
}

/**
 * Get the list of native formats/sizes a camera supports.
 *
 * This returns a list of all formats and frame sizes that a specific camera can
 * offer. This is useful if your app can accept a variety of image formats and
 * sizes and so want to find the optimal spec that doesn't require conversion.
 *
 * This function isn't strictly required; if you call Camera.Camera with a
 * nullptr spec, SDL will choose a native format for you, and if you instead
 * specify a desired format, it will transparently convert to the requested
 * format on your behalf.
 *
 * If `count` is not nullptr, it will be filled with the number of elements in
 * the returned array.
 *
 * Note that it's legal for a camera to supply an empty list. This is what will
 * happen on Emscripten builds, since that platform won't tell _anything_ about
 * available cameras until you've opened one, and won't even tell if there _is_
 * a camera until the user has given you permission to check through a scary
 * warning popup.
 *
 * @param instance_id the camera device instance ID.
 * @returns a nullptr terminated array of pointers to CameraSpec or nullptr on
 *          failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetCameras
 * @sa Camera.Camera
 */
inline OwnArray<CameraSpec*> GetCameraSupportedFormats(CameraID instance_id)
{
  int count;
  auto data = SDL_GetCameraSupportedFormats(instance_id, &count);
  return OwnArray<CameraSpec*>(data, count);
}

/**
 * Get the human-readable device name for a camera.
 *
 * @param instance_id the camera device instance ID.
 * @returns a human-readable device name or nullptr on failure; call GetError()
 *          for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetCameras
 */
inline const char* GetCameraName(CameraID instance_id)
{
  return SDL_GetCameraName(instance_id);
}

/**
 * Get the position of the camera in relation to the system.
 *
 * Most platforms will report UNKNOWN, but mobile devices, like phones, can
 * often make a distinction between cameras on the front of the device (that
 * points towards the user, for taking "selfies") and cameras on the back (for
 * filming in the direction the user is facing).
 *
 * @param instance_id the camera device instance ID.
 * @returns the position of the camera on the system hardware.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetCameras
 */
inline CameraPosition GetCameraPosition(CameraID instance_id)
{
  return SDL_GetCameraPosition(instance_id);
}

/**
 * Open a video recording device (a "camera").
 *
 * You can open the device with any reasonable spec, and if the hardware can't
 * directly support it, it will convert data seamlessly to the requested format.
 * This might incur overhead, including scaling of image data.
 *
 * If you would rather accept whatever format the device offers, you can pass a
 * nullptr spec here and it will choose one for you (and you can use Surface's
 * conversion/scaling functions directly if necessary).
 *
 * You can call Camera.GetFormat() to get the actual data format if passing a
 * nullptr spec here. You can see the exact specs a device can support without
 * conversion with GetCameraSupportedFormats().
 *
 * SDL will not attempt to emulate framerate; it will try to set the hardware to
 * the rate closest to the requested speed, but it won't attempt to limit or
 * duplicate frames artificially; call Camera.GetFormat() to see the actual
 * framerate of the opened the device, and check your timestamps if this is
 * crucial to your app!
 *
 * Note that the camera is not usable until the user approves its use! On some
 * platforms, the operating system will prompt the user to permit access to the
 * camera, and they can choose Yes or No at that point. Until they do, the
 * camera will not be usable. The app should either wait for an
 * EVENT_CAMERA_DEVICE_APPROVED (or EVENT_CAMERA_DEVICE_DENIED) event, or poll
 * Camera.GetPermissionState() occasionally until it returns non-zero. On
 * platforms that don't require explicit user approval (and perhaps in places
 * where the user previously permitted access), the approval event might come
 * immediately, but it might come seconds, minutes, or hours later!
 *
 * @param instance_id the camera device instance ID.
 * @param spec the desired format for data the device will provide. Can be
 *             std::nullopt.
 * @returns an Camera object on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetCameras
 * @sa Camera.GetFormat
 */
inline Camera OpenCamera(CameraID instance_id,
                         OptionalRef<const CameraSpec> spec = {})
{
  return Camera(instance_id, spec);
}

/**
 * Query if camera access has been approved by the user.
 *
 * Cameras will not function between when the device is opened by the app and
 * when the user permits access to the hardware. On some platforms, this
 * presents as a popup dialog where the user has to explicitly approve access;
 * on others the approval might be implicit and not alert the user at all.
 *
 * This function can be used to check the status of that approval. It will
 * return CAMERA_PERMISSION_STATE_PENDING if waiting for user response,
 * CAMERA_PERMISSION_STATE_APPROVED if the camera is approved for use, and
 * CAMERA_PERMISSION_STATE_DENIED if the user denied access.
 *
 * Instead of polling with this function, you can wait for a
 * EVENT_CAMERA_DEVICE_APPROVED (or EVENT_CAMERA_DEVICE_DENIED) event in the
 * standard SDL event loop, which is guaranteed to be sent once when permission
 * to use the camera is decided.
 *
 * If a camera is declined, there's nothing to be done but call Camera.Close()
 * to dispose of it.
 *
 * @param camera the opened camera device to query.
 * @returns an CameraPermissionState value indicating if access is granted, or
 *          `CAMERA_PERMISSION_STATE_PENDING` if the decision is still pending.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Camera.Camera
 * @sa Camera.Close
 */
inline CameraPermissionState GetCameraPermissionState(CameraParam camera)
{
  return SDL_GetCameraPermissionState(camera);
}

inline CameraPermissionState Camera::GetPermissionState()
{
  return SDL::GetCameraPermissionState(m_resource);
}

/**
 * Get the instance ID of an opened camera.
 *
 * @param camera an Camera to query.
 * @returns the instance ID of the specified camera on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Camera.Camera
 */
inline CameraID GetCameraID(CameraParam camera)
{
  return CheckError(SDL_GetCameraID(camera));
}

inline CameraID Camera::GetID() { return SDL::GetCameraID(m_resource); }

/**
 * Get the properties associated with an opened camera.
 *
 * @param camera the Camera obtained from Camera.Camera().
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesRef GetCameraProperties(CameraParam camera)
{
  return {CheckError(SDL_GetCameraProperties(camera))};
}

inline PropertiesRef Camera::GetProperties()
{
  return SDL::GetCameraProperties(m_resource);
}

/**
 * Get the spec that a camera is using when generating images.
 *
 * Note that this might not be the native format of the hardware, as SDL might
 * be converting to this format behind the scenes.
 *
 * If the system is waiting for the user to approve access to the camera, as
 * some platforms require, this will return false, but this isn't necessarily a
 * fatal error; you should either wait for an EVENT_CAMERA_DEVICE_APPROVED (or
 * EVENT_CAMERA_DEVICE_DENIED) event, or poll Camera.GetPermissionState()
 * occasionally until it returns non-zero.
 *
 * @param camera opened camera device.
 * @returns the CameraSpec or std::nullopt if waiting for user approval.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Camera.Camera
 */
inline std::optional<CameraSpec> GetCameraFormat(CameraParam camera)
{
  if (CameraSpec spec; SDL_GetCameraFormat(camera, &spec)) return spec;
  return std::nullopt;
}

inline std::optional<CameraSpec> Camera::GetFormat()
{
  return SDL::GetCameraFormat(m_resource);
}

/**
 * Acquire a frame.
 *
 * The frame is a memory pointer to the image data, whose size and format are
 * given by the spec requested when opening the device.
 *
 * This is a non blocking API. If there is a frame available, a non-nullptr
 * surface is returned, and timestampNS will be filled with a non-zero value.
 *
 * Note that an error case can also return nullptr, but a nullptr by itself is
 * normal and just signifies that a new frame is not yet available. Note that
 * even if a camera device fails outright (a USB camera is unplugged while in
 * use, etc), SDL will send an event separately to notify the app, but continue
 * to provide blank frames at ongoing intervals until Camera.Close() is called,
 * so real failure here is almost always an out of memory condition.
 *
 * After use, the frame should be released with Camera.ReleaseFrame(). If you
 * don't do this, the system may stop providing more video!
 *
 * Do not call Surface.Destroy() on the returned surface! It must be given back
 * to the camera subsystem with Camera.ReleaseFrame!
 *
 * If the system is waiting for the user to approve access to the camera, as
 * some platforms require, this will return nullptr (no frames available); you
 * should either wait for an EVENT_CAMERA_DEVICE_APPROVED (or
 * EVENT_CAMERA_DEVICE_DENIED) event, or poll Camera.GetPermissionState()
 * occasionally until it returns non-zero.
 *
 * @param camera opened camera device.
 * @param timestampNS a pointer filled in with the frame's timestamp, or 0 on
 *                    error. Can be nullptr.
 * @returns a new frame of video on success, nullptr if none is currently
 *          available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Camera.ReleaseFrame
 */
inline Surface AcquireCameraFrame(CameraParam camera,
                                  Uint64* timestampNS = nullptr)
{
  return Surface::Borrow(SDL_AcquireCameraFrame(camera, timestampNS));
}

inline CameraFrame Camera::AcquireFrame(Uint64* timestampNS)
{
  return {CameraRef(*this)};
}

inline CameraFrame::CameraFrame(CameraRef resource, Uint64* timestampNS)
  : Surface(AcquireCameraFrame(resource, timestampNS))
  , m_lock(std::move(resource))
{
  if (!*this) m_lock.release();
}

/**
 * Release a frame of video acquired from a camera.
 *
 * Let the back-end re-use the internal buffer for camera.
 *
 * This function _must_ be called only on surface objects returned by
 * Camera.AcquireFrame(). This function should be called as quickly as possible
 * after acquisition, as SDL keeps a small FIFO queue of surfaces for video
 * frames; if surfaces aren't released in a timely manner, SDL may drop upcoming
 * video frames from the camera.
 *
 * If the app needs to keep the surface for a significant time, they should make
 * a copy of it and release the original.
 *
 * The app should not use the surface again after calling this function; assume
 * the surface is freed and the pointer is invalid.
 *
 * @param camera opened camera device.
 * @param frame the video frame surface to release.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Camera.AcquireFrame
 */
inline void ReleaseCameraFrame(CameraParam camera, SurfaceParam frame)
{
  SDL_ReleaseCameraFrame(camera, frame);
}

inline void Camera::ReleaseFrame(CameraFrame&& lock)
{
  SDL_assert_paranoid(lock.get() == *this);
  lock.reset();
}

inline void CameraFrame::reset()
{
  if (!m_lock) return;
  ReleaseCameraFrame(m_lock, Surface::release());
  m_lock = {};
}

/**
 * Use this function to shut down camera processing and close the camera device.
 *
 * @param camera opened camera device.
 *
 * @threadsafety It is safe to call this function from any thread, but no thread
 *               may reference `device` once this function is called.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa Camera.Camera
 */
inline void CloseCamera(CameraRaw camera) { SDL_CloseCamera(camera); }

inline void Camera::Close() { CloseCamera(release()); }

/// @}

} // namespace SDL

#endif /* SDL3PP_CAMERA_H_ */
