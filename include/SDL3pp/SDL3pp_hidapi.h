#ifndef SDL3PP_HIDAPI_H_
#define SDL3PP_HIDAPI_H_

#include <SDL3/SDL_hidapi.h>
#include "SDL3pp_error.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryHIDAPI HIDAPI
 *
 * Header file for SDL HIDAPI functions.
 *
 * This is an adaptation of the original HIDAPI interface by Alan Ott, and
 * includes source code licensed under the following license:
 *
 * ```
 * HIDAPI - Multi-Platform library for
 * communication with HID devices.
 *
 * Copyright 2009, Alan Ott, Signal 11 Software.
 * All Rights Reserved.
 *
 * This software may be used by anyone for any reason so
 * long as the copyright notice in the source files
 * remains intact.
 * ```
 *
 * (Note that this license is the same as item three of SDL's zlib license, so
 * it adds no new requirements on the user.)
 *
 * If you would like a version of SDL without this code, you can build SDL with
 * SDL_HIDAPI_DISABLED defined to 1. You might want to do this for example on
 * iOS or tvOS to avoid a dependency on the CoreBluetooth framework.
 *
 * @{
 */

// Forward decl
struct HidDevice;

/// Alias to raw representation for HidDevice.
using HidDeviceRaw = SDL_hid_device*;

// Forward decl
struct HidDeviceRef;

/// Safely wrap HidDevice for non owning parameters
struct HidDeviceParam
{
  HidDeviceRaw value; ///< parameter's HidDeviceRaw

  /// Constructs from HidDeviceRaw
  constexpr HidDeviceParam(HidDeviceRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr HidDeviceParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const HidDeviceParam& other) const = default;

  /// Converts to underlying HidDeviceRaw
  constexpr operator HidDeviceRaw() const { return value; }
};

/**
 * HID underlying bus types.
 *
 * @since This enum is available since SDL 3.2.0.
 */
using hid_bus_type = SDL_hid_bus_type;

constexpr hid_bus_type HID_API_BUS_UNKNOWN =
  SDL_HID_API_BUS_UNKNOWN; ///< Unknown bus type

/**
 * USB bus Specifications:
 *
 * - https://usb.org/hid
 */
constexpr hid_bus_type HID_API_BUS_USB = SDL_HID_API_BUS_USB;

/**
 * Bluetooth or Bluetooth LE bus Specifications:
 *
 * * https://www.bluetooth.com/specifications/specs/human-interface-device-profile-1-1-1/
 * * https://www.bluetooth.com/specifications/specs/hid-service-1-0/
 * * https://www.bluetooth.com/specifications/specs/hid-over-gatt-profile-1-0/
 */
constexpr hid_bus_type HID_API_BUS_BLUETOOTH = SDL_HID_API_BUS_BLUETOOTH;

/**
 * I2C bus Specifications:
 *
 * * https://docs.microsoft.com/previous-versions/windows/hardware/design/dn642101(v=vs.85)
 */
constexpr hid_bus_type HID_API_BUS_I2C = SDL_HID_API_BUS_I2C;

/**
 * SPI bus Specifications:
 *
 * - https://www.microsoft.com/download/details.aspx?id=103325
 */
constexpr hid_bus_type HID_API_BUS_SPI = SDL_HID_API_BUS_SPI;

/**
 * Information about a connected HID device
 *
 * @since This struct is available since SDL 3.2.0.
 */
using hid_device_info = SDL_hid_device_info;

/**
 * An opaque handle representing an open HID device.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @cat resource
 */
class HidDevice
{
  HidDeviceRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr HidDevice() = default;

  /**
   * Constructs from HidDeviceParam.
   *
   * @param resource a HidDeviceRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit HidDevice(const HidDeviceRaw resource)
    : m_resource(resource)
  {
  }

  /// Copy constructor
  constexpr HidDevice(const HidDevice& other) = delete;

  /// Move constructor
  constexpr HidDevice(HidDevice&& other)
    : HidDevice(other.release())
  {
  }

  constexpr HidDevice(const HidDeviceRef& other) = delete;

  constexpr HidDevice(HidDeviceRef&& other) = delete;

  /**
   * Open a HID device using a Vendor ID (VID), Product ID (PID) and optionally
   * a serial number.
   *
   * If `serial_number` is nullptr, the first device with the specified VID and
   * PID is opened.
   *
   * @param vendor_id the Vendor ID (VID) of the device to open.
   * @param product_id the Product ID (PID) of the device to open.
   * @param serial_number the Serial Number of the device to open (Optionally
   *                      nullptr).
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  HidDevice(unsigned short vendor_id,
            unsigned short product_id,
            const wchar_t* serial_number)
    : m_resource(CheckError(SDL_hid_open(vendor_id, product_id, serial_number)))
  {
  }

  /**
   * Open a HID device by its path name.
   *
   * The path name be determined by calling hid_enumerate(), or a
   * platform-specific path name can be used (eg: /dev/hidraw0 on Linux).
   *
   * @param path the path name of the device to open.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  HidDevice(StringParam path)
    : m_resource(CheckError(SDL_hid_open_path(path)))
  {
  }

  /// Destructor
  ~HidDevice() { SDL_hid_close(m_resource); }

  /// Assignment operator.
  HidDevice& operator=(HidDevice other)
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

  /// Retrieves underlying HidDeviceRaw.
  constexpr HidDeviceRaw get() const { return m_resource; }

  /// Retrieves underlying HidDeviceRaw and clear this.
  constexpr HidDeviceRaw release()
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const HidDevice& other) const = default;

  /// Comparison
  constexpr bool operator==(std::nullptr_t _) const { return !m_resource; }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!m_resource; }

  /// Converts to HidDeviceParam
  constexpr operator HidDeviceParam() const { return {m_resource}; }

  /**
   * Close a HID device.
   *
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void close();

  /**
   * Write an Output report to a HID device.
   *
   * The first byte of `data` must contain the Report ID. For devices which only
   * support a single report, this must be set to 0x0. The remaining bytes
   * contain the report data. Since the Report ID is mandatory, calls to
   * HidDevice.write() will always contain one more byte than the report
   * contains. For example, if a hid report is 16 bytes long, 17 bytes must be
   * passed to HidDevice.write(), the Report ID (or 0x0, for devices with a
   * single report), followed by the report data (16 bytes). In this example,
   * the length passed in would be 17.
   *
   * HidDevice.write() will send the data on the first OUT endpoint, if one
   * exists. If it does not, it will send the data through the Control Endpoint
   * (Endpoint 0).
   *
   * @param data the data to send, including the report number as the first
   *             byte.
   * @returns the actual number of bytes written and -1 on on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  int write(SourceBytes data);

  /**
   * Read an Input report from a HID device with timeout.
   *
   * Input reports are returned to the host through the INTERRUPT IN endpoint.
   * The first byte will contain the Report number if the device uses numbered
   * reports.
   *
   * @param data a buffer to put the read data into.
   * @param timeout timeout in milliseconds
   * @returns the actual number of bytes read and -1 on on failure; call
   *          GetError() for more information. If no packet was available to be
   *          read within the timeout period, this function returns 0.
   *
   * @since This function is available since SDL 3.2.0.
   */
  int read_timeout(TargetBytes data, Milliseconds timeout);

  /**
   * Read an Input report from a HID device.
   *
   * Input reports are returned to the host through the INTERRUPT IN endpoint.
   * The first byte will contain the Report number if the device uses numbered
   * reports.
   *
   * @param data a buffer to put the read data into.
   * @returns the actual number of bytes read and -1 on failure; call GetError()
   *          for more information. If no packet was available to be read and
   *          the handle is in non-blocking mode, this function returns 0.
   *
   * @since This function is available since SDL 3.2.0.
   */
  int read(TargetBytes data);

  /**
   * Set the device handle to be non-blocking.
   *
   * In non-blocking mode calls to HidDevice.read() will return immediately with
   * a value of 0 if there is no data to be read. In blocking mode,
   * HidDevice.read() will wait (block) until there is data to read before
   * returning.
   *
   * Nonblocking can be turned on and off at any time.
   *
   * @param nonblock enable or not the nonblocking reads - true to enable
   *                 nonblocking - false to disable nonblocking.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void set_nonblocking(bool nonblock);

  /**
   * Send a Feature report to the device.
   *
   * Feature reports are sent over the Control endpoint as a Set_Report
   * transfer. The first byte of `data` must contain the Report ID. For devices
   * which only support a single report, this must be set to 0x0. The remaining
   * bytes contain the report data. Since the Report ID is mandatory, calls to
   * HidDevice.send_feature_report() will always contain one more byte than the
   * report contains. For example, if a hid report is 16 bytes long, 17 bytes
   * must be passed to HidDevice.send_feature_report(): the Report ID (or 0x0,
   * for devices which do not use numbered reports), followed by the report data
   * (16 bytes). In this example, the length passed in would be 17.
   *
   * @param data the data to send, including the report number as the first
   *             byte.
   * @returns the actual number of bytes written and -1 on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  int send_feature_report(SourceBytes data);

  /**
   * Get a feature report from a HID device.
   *
   * Set the first byte of `data` to the Report ID of the report to be read.
   * Make sure to allow space for this extra byte in `data`. Upon return, the
   * first byte will still contain the Report ID, and the report data will start
   * in data[1].
   *
   * @param data a buffer to put the read data into, including the Report ID.
   *             Set the first byte of `data` to the Report ID of the report to
   *             be read, or set it to zero if your device does not use numbered
   *             reports.
   * @returns the number of bytes read plus one for the report ID (which is
   *          still in the first byte), or -1 on on failure; call GetError() for
   *          more information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  int get_feature_report(TargetBytes data);

  /**
   * Get an input report from a HID device.
   *
   * Set the first byte of `data` to the Report ID of the report to be read.
   * Make sure to allow space for this extra byte in `data`. Upon return, the
   * first byte will still contain the Report ID, and the report data will start
   * in data[1].
   *
   * @param data a buffer to put the read data into, including the Report ID.
   *             Set the first byte of `data` to the Report ID of the report to
   *             be read, or set it to zero if your device does not use numbered
   *             reports.
   * @returns the number of bytes read plus one for the report ID (which is
   *          still in the first byte), or -1 on on failure; call GetError() for
   *          more information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  int get_input_report(TargetBytes data);

  /**
   * Get The Manufacturer String from a HID device.
   *
   * @param string a wide string buffer to put the data into.
   * @param maxlen the length of the buffer in multiples of wchar_t.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void get_manufacturer_string(wchar_t* string, size_t maxlen);

  /**
   * Get The Product String from a HID device.
   *
   * @param string a wide string buffer to put the data into.
   * @param maxlen the length of the buffer in multiples of wchar_t.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void get_product_string(wchar_t* string, size_t maxlen);

  /**
   * Get The Serial Number String from a HID device.
   *
   * @param string a wide string buffer to put the data into.
   * @param maxlen the length of the buffer in multiples of wchar_t.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void get_serial_number_string(wchar_t* string, size_t maxlen);

  /**
   * Get a string from a HID device, based on its string index.
   *
   * @param string_index the index of the string to get.
   * @param string a wide string buffer to put the data into.
   * @param maxlen the length of the buffer in multiples of wchar_t.
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void get_indexed_string(int string_index, wchar_t* string, size_t maxlen);

  /**
   * Get the device info from a HID device.
   *
   * @returns a pointer to the hid_device_info for this hid_device on success.
   *          This struct is valid until the device is closed with
   *          HidDevice.close().
   * @throws Error on failure.
   *
   * @since This function is available since SDL 3.2.0.
   */
  hid_device_info* get_device_info();

  /**
   * Get a report descriptor from a HID device.
   *
   * User has to provide a preallocated buffer where descriptor will be copied
   * to. The recommended size for a preallocated buffer is 4096 bytes.
   *
   * @param buf the buffer to copy descriptor into.
   * @returns the number of bytes actually copied or -1 on failure; call
   *          GetError() for more information.
   *
   * @since This function is available since SDL 3.2.0.
   */
  int get_report_descriptor(TargetBytes buf);
};

/// Semi-safe reference for HidDevice.
struct HidDeviceRef : HidDevice
{
  /**
   * Constructs from HidDeviceParam.
   *
   * @param resource a HidDeviceRaw or HidDevice.
   *
   * This does not takes ownership!
   */
  HidDeviceRef(HidDeviceParam resource)
    : HidDevice(resource.value)
  {
  }

  /// Copy constructor.
  HidDeviceRef(const HidDeviceRef& other)
    : HidDevice(other.get())
  {
  }

  /// Destructor
  ~HidDeviceRef() { release(); }
};

/**
 * Initialize the HIDAPI library.
 *
 * This function initializes the HIDAPI library. Calling it is not strictly
 * necessary, as it will be called automatically by hid_enumerate() and any of
 * the SDL_hid_open_*() functions if it is needed. This function should be
 * called at the beginning of execution however, if there is a chance of HIDAPI
 * handles being opened by different threads simultaneously.
 *
 * Each call to this function should have a matching call to hid_exit()
 *
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa hid_exit
 */
inline void hid_init() { CheckErrorIfNot(SDL_hid_init(), 0); }

/**
 * Finalize the HIDAPI library.
 *
 * This function frees all of the static data associated with HIDAPI. It should
 * be called at the end of execution to avoid memory leaks.
 *
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa hid_init
 */
inline void hid_exit() { CheckErrorIfNot(SDL_hid_exit(), 0); }

/**
 * Check to see if devices may have been added or removed.
 *
 * Enumerating the HID devices is an expensive operation, so you can call this
 * to see if there have been any system device changes since the last call to
 * this function. A change in the counter returned doesn't necessarily mean that
 * anything has changed, but you can call hid_enumerate() to get an updated
 * device list.
 *
 * Calling this function for the first time may cause a thread or other system
 * resource to be allocated to track device change notifications.
 *
 * @returns a change counter that is incremented with each potential device
 *          change, or 0 if device change detection isn't available.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa hid_enumerate
 */
inline Uint32 hid_device_change_count()
{
  return SDL_hid_device_change_count();
}

/**
 * Enumerate the HID Devices.
 *
 * This function returns a linked list of all the HID devices attached to the
 * system which match vendor_id and product_id. If `vendor_id` is set to 0 then
 * any vendor matches. If `product_id` is set to 0 then any product matches. If
 * `vendor_id` and `product_id` are both set to 0, then all HID devices will be
 * returned.
 *
 * By default SDL will only enumerate controllers, to reduce risk of hanging or
 * crashing on bad drivers, but SDL_HINT_HIDAPI_ENUMERATE_ONLY_CONTROLLERS can
 * be set to "0" to enumerate all HID devices.
 *
 * @param vendor_id the Vendor ID (VID) of the types of device to open, or 0 to
 *                  match any vendor.
 * @param product_id the Product ID (PID) of the types of device to open, or 0
 *                   to match any product.
 * @returns a pointer to a linked list of type hid_device_info, containing
 *          information about the HID devices attached to the system on success.
 *          Free this linked list by calling hid_free_enumeration().
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa hid_device_change_count
 */
inline hid_device_info* hid_enumerate(unsigned short vendor_id,
                                      unsigned short product_id)
{
  return CheckError(SDL_hid_enumerate(vendor_id, product_id));
}

/**
 * Free an enumeration linked list.
 *
 * This function frees a linked list created by hid_enumerate().
 *
 * @param devs pointer to a list of struct_device returned from hid_enumerate().
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void hid_free_enumeration(hid_device_info* devs)
{
  SDL_hid_free_enumeration(devs);
}

/**
 * Open a HID device using a Vendor ID (VID), Product ID (PID) and optionally a
 * serial number.
 *
 * If `serial_number` is nullptr, the first device with the specified VID and
 * PID is opened.
 *
 * @param vendor_id the Vendor ID (VID) of the device to open.
 * @param product_id the Product ID (PID) of the device to open.
 * @param serial_number the Serial Number of the device to open (Optionally
 *                      nullptr).
 * @returns a HidDevice object on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline HidDevice hid_open(unsigned short vendor_id,
                          unsigned short product_id,
                          const wchar_t* serial_number)
{
  return HidDevice(vendor_id, product_id, serial_number);
}

/**
 * Open a HID device by its path name.
 *
 * The path name be determined by calling hid_enumerate(), or a
 * platform-specific path name can be used (eg: /dev/hidraw0 on Linux).
 *
 * @param path the path name of the device to open.
 * @returns a HidDevice object on success.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline HidDevice hid_open_path(StringParam path)
{
  return HidDevice(std::move(path));
}

/**
 * Write an Output report to a HID device.
 *
 * The first byte of `data` must contain the Report ID. For devices which only
 * support a single report, this must be set to 0x0. The remaining bytes contain
 * the report data. Since the Report ID is mandatory, calls to HidDevice.write()
 * will always contain one more byte than the report contains. For example, if a
 * hid report is 16 bytes long, 17 bytes must be passed to HidDevice.write(),
 * the Report ID (or 0x0, for devices with a single report), followed by the
 * report data (16 bytes). In this example, the length passed in would be 17.
 *
 * HidDevice.write() will send the data on the first OUT endpoint, if one
 * exists. If it does not, it will send the data through the Control Endpoint
 * (Endpoint 0).
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @param data the data to send, including the report number as the first byte.
 * @returns the actual number of bytes written and -1 on on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int hid_write(HidDeviceParam dev, SourceBytes data)
{
  return SDL_hid_write(dev, data.data_as<Uint8>(), data.size_bytes());
}

inline int HidDevice::write(SourceBytes data)
{
  return SDL::hid_write(m_resource, std::move(data));
}

/**
 * Read an Input report from a HID device with timeout.
 *
 * Input reports are returned to the host through the INTERRUPT IN endpoint. The
 * first byte will contain the Report number if the device uses numbered
 * reports.
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @param data a buffer to put the read data into.
 * @param timeout timeout in milliseconds
 * @returns the actual number of bytes read and -1 on on failure; call
 *          GetError() for more information. If no packet was available to be
 *          read within the timeout period, this function returns 0.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int hid_read_timeout(HidDeviceParam dev,
                            TargetBytes data,
                            Milliseconds timeout)
{
  return SDL_hid_read_timeout(
    dev, data.data_as<Uint8>(), data.size_bytes(), timeout.count());
}

inline int HidDevice::read_timeout(TargetBytes data, Milliseconds timeout)
{
  return SDL::hid_read_timeout(m_resource, std::move(data), timeout);
}

/**
 * Read an Input report from a HID device.
 *
 * Input reports are returned to the host through the INTERRUPT IN endpoint. The
 * first byte will contain the Report number if the device uses numbered
 * reports.
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @param data a buffer to put the read data into.
 * @returns the actual number of bytes read and -1 on failure; call GetError()
 *          for more information. If no packet was available to be read and the
 *          handle is in non-blocking mode, this function returns 0.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int hid_read(HidDeviceParam dev, TargetBytes data)
{
  return SDL_hid_read(dev, data.data_as<Uint8>(), data.size_bytes());
}

inline int HidDevice::read(TargetBytes data)
{
  return SDL::hid_read(m_resource, std::move(data));
}

/**
 * Set the device handle to be non-blocking.
 *
 * In non-blocking mode calls to HidDevice.read() will return immediately with a
 * value of 0 if there is no data to be read. In blocking mode, HidDevice.read()
 * will wait (block) until there is data to read before returning.
 *
 * Nonblocking can be turned on and off at any time.
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @param nonblock enable or not the nonblocking reads - true to enable
 *                 nonblocking - false to disable nonblocking.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void hid_set_nonblocking(HidDeviceParam dev, bool nonblock)
{
  CheckErrorIfNot(SDL_hid_set_nonblocking(dev, nonblock), 0);
}

inline void HidDevice::set_nonblocking(bool nonblock)
{
  SDL::hid_set_nonblocking(m_resource, nonblock);
}

/**
 * Send a Feature report to the device.
 *
 * Feature reports are sent over the Control endpoint as a Set_Report transfer.
 * The first byte of `data` must contain the Report ID. For devices which only
 * support a single report, this must be set to 0x0. The remaining bytes contain
 * the report data. Since the Report ID is mandatory, calls to
 * HidDevice.send_feature_report() will always contain one more byte than the
 * report contains. For example, if a hid report is 16 bytes long, 17 bytes must
 * be passed to HidDevice.send_feature_report(): the Report ID (or 0x0, for
 * devices which do not use numbered reports), followed by the report data (16
 * bytes). In this example, the length passed in would be 17.
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @param data the data to send, including the report number as the first byte.
 * @returns the actual number of bytes written and -1 on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int hid_send_feature_report(HidDeviceParam dev, SourceBytes data)
{
  return SDL_hid_send_feature_report(
    dev, data.data_as<Uint8>(), data.size_bytes());
}

inline int HidDevice::send_feature_report(SourceBytes data)
{
  return SDL::hid_send_feature_report(m_resource, std::move(data));
}

/**
 * Get a feature report from a HID device.
 *
 * Set the first byte of `data` to the Report ID of the report to be read. Make
 * sure to allow space for this extra byte in `data`. Upon return, the first
 * byte will still contain the Report ID, and the report data will start in
 * data[1].
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @param data a buffer to put the read data into, including the Report ID. Set
 *             the first byte of `data` to the Report ID of the report to be
 *             read, or set it to zero if your device does not use numbered
 *             reports.
 * @returns the number of bytes read plus one for the report ID (which is still
 *          in the first byte), or -1 on on failure; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int hid_get_feature_report(HidDeviceParam dev, TargetBytes data)
{
  return SDL_hid_get_feature_report(
    dev, data.data_as<Uint8>(), data.size_bytes());
}

inline int HidDevice::get_feature_report(TargetBytes data)
{
  return SDL::hid_get_feature_report(m_resource, std::move(data));
}

/**
 * Get an input report from a HID device.
 *
 * Set the first byte of `data` to the Report ID of the report to be read. Make
 * sure to allow space for this extra byte in `data`. Upon return, the first
 * byte will still contain the Report ID, and the report data will start in
 * data[1].
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @param data a buffer to put the read data into, including the Report ID. Set
 *             the first byte of `data` to the Report ID of the report to be
 *             read, or set it to zero if your device does not use numbered
 *             reports.
 * @returns the number of bytes read plus one for the report ID (which is still
 *          in the first byte), or -1 on on failure; call GetError() for more
 *          information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int hid_get_input_report(HidDeviceParam dev, TargetBytes data)
{
  return SDL_hid_get_input_report(
    dev, data.data_as<Uint8>(), data.size_bytes());
}

inline int HidDevice::get_input_report(TargetBytes data)
{
  return SDL::hid_get_input_report(m_resource, std::move(data));
}

/**
 * Close a HID device.
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void hid_close(HidDeviceRaw dev)
{
  CheckErrorIfNot(SDL_hid_close(dev), 0);
}

inline void HidDevice::close() { hid_close(release()); }

/**
 * Get The Manufacturer String from a HID device.
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @param string a wide string buffer to put the data into.
 * @param maxlen the length of the buffer in multiples of wchar_t.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void hid_get_manufacturer_string(HidDeviceParam dev,
                                        wchar_t* string,
                                        size_t maxlen)
{
  CheckErrorIfNot(SDL_hid_get_manufacturer_string(dev, string, maxlen), 0);
}

inline void HidDevice::get_manufacturer_string(wchar_t* string, size_t maxlen)
{
  SDL::hid_get_manufacturer_string(m_resource, string, maxlen);
}

/**
 * Get The Product String from a HID device.
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @param string a wide string buffer to put the data into.
 * @param maxlen the length of the buffer in multiples of wchar_t.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void hid_get_product_string(HidDeviceParam dev,
                                   wchar_t* string,
                                   size_t maxlen)
{
  CheckErrorIfNot(SDL_hid_get_product_string(dev, string, maxlen), 0);
}

inline void HidDevice::get_product_string(wchar_t* string, size_t maxlen)
{
  SDL::hid_get_product_string(m_resource, string, maxlen);
}

/**
 * Get The Serial Number String from a HID device.
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @param string a wide string buffer to put the data into.
 * @param maxlen the length of the buffer in multiples of wchar_t.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void hid_get_serial_number_string(HidDeviceParam dev,
                                         wchar_t* string,
                                         size_t maxlen)
{
  CheckErrorIfNot(SDL_hid_get_serial_number_string(dev, string, maxlen), 0);
}

inline void HidDevice::get_serial_number_string(wchar_t* string, size_t maxlen)
{
  SDL::hid_get_serial_number_string(m_resource, string, maxlen);
}

/**
 * Get a string from a HID device, based on its string index.
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @param string_index the index of the string to get.
 * @param string a wide string buffer to put the data into.
 * @param maxlen the length of the buffer in multiples of wchar_t.
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void hid_get_indexed_string(HidDeviceParam dev,
                                   int string_index,
                                   wchar_t* string,
                                   size_t maxlen)
{
  CheckErrorIfNot(SDL_hid_get_indexed_string(dev, string_index, string, maxlen),
                  0);
}

inline void HidDevice::get_indexed_string(int string_index,
                                          wchar_t* string,
                                          size_t maxlen)
{
  SDL::hid_get_indexed_string(m_resource, string_index, string, maxlen);
}

/**
 * Get the device info from a HID device.
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @returns a pointer to the hid_device_info for this hid_device on success.
 *          This struct is valid until the device is closed with
 *          HidDevice.close().
 * @throws Error on failure.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline hid_device_info* hid_get_device_info(HidDeviceParam dev)
{
  return CheckError(SDL_hid_get_device_info(dev));
}

inline hid_device_info* HidDevice::get_device_info()
{
  return SDL::hid_get_device_info(m_resource);
}

/**
 * Get a report descriptor from a HID device.
 *
 * User has to provide a preallocated buffer where descriptor will be copied to.
 * The recommended size for a preallocated buffer is 4096 bytes.
 *
 * @param dev a device handle returned from HidDevice.HidDevice().
 * @param buf the buffer to copy descriptor into.
 * @returns the number of bytes actually copied or -1 on failure; call
 *          GetError() for more information.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int hid_get_report_descriptor(HidDeviceParam dev, TargetBytes buf)
{
  return SDL_hid_get_report_descriptor(
    dev, buf.data_as<Uint8>(), buf.size_bytes());
}

inline int HidDevice::get_report_descriptor(TargetBytes buf)
{
  return SDL::hid_get_report_descriptor(m_resource, std::move(buf));
}

/**
 * Start or stop a BLE scan on iOS and tvOS to pair Steam Controllers.
 *
 * @param active true to start the scan, false to stop the scan.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void hid_ble_scan(bool active) { SDL_hid_ble_scan(active); }

/// @}

} // namespace SDL

#endif /* SDL3PP_HIDAPI_H_ */
