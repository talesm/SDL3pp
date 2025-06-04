#ifndef SDL3PP_AUDIO_H_
#define SDL3PP_AUDIO_H_

#include <SDL3/SDL_audio.h>
#include "SDL3pp_iostream.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 *
 * @defgroup CategoryAudio Audio Playback, Recording, and Mixing
 *
 * Audio functionality for the SDL library.
 *
 * All audio in SDL3 revolves around AudioStream. Whether you want to play
 * or record audio, convert it, stream it, buffer it, or mix it, you're going
 * to be passing it through an audio stream.
 *
 * Audio streams are quite flexible; they can accept any amount of data at a
 * time, in any supported format, and output it as needed in any other format,
 * even if the data format changes on either side halfway through.
 *
 * An app opens an audio device and binds any number of audio streams to it,
 * feeding more data to the streams as available. When the device needs more
 * data, it will pull it from all bound streams and mix them together for
 * playback.
 *
 * Audio streams can also use an app-provided callback to supply data
 * on-demand, which maps pretty closely to the SDL2 audio model.
 *
 * SDL also provides a simple .WAV loader in LoadWAV (and LoadWAV
 * if you aren't reading from a file) as a basic means to load sound data into
 * your program.
 *
 * ## Logical audio devices
 *
 * In SDL3, opening a physical device (like a SoundBlaster 16 Pro) gives you a
 * logical device ID that you can bind audio streams to. In almost all cases,
 * logical devices can be used anywhere in the API that a physical device is
 * normally used. However, since each device opening generates a new logical
 * device, different parts of the program (say, a VoIP library, or
 * text-to-speech framework, or maybe some other sort of mixer on top of SDL)
 * can have their own device opens that do not interfere with each other; each
 * logical device will mix its separate audio down to a single buffer, fed to
 * the physical device, behind the scenes. As many logical devices as you like
 * can come and go; SDL will only have to open the physical device at the OS
 * level once, and will manage all the logical devices on top of it
 * internally.
 *
 * One other benefit of logical devices: if you don't open a specific physical
 * device, instead opting for the default, SDL can automatically migrate those
 * logical devices to different hardware as circumstances change: a user
 * plugged in headphones? The system default changed? SDL can transparently
 * migrate the logical devices to the correct physical device seamlessly and
 * keep playing; the app doesn't even have to know it happened if it doesn't
 * want to.
 *
 * ## Simplified audio
 *
 * As a simplified model for when a single source of audio is all that's
 * needed, an app can use AudioStream.OpenAudioDeviceStream, which is a single
 * function to open an audio device, create an audio stream, bind that stream
 * to the newly-opened device, and (optionally) provide a callback for
 * obtaining audio data. When using this function, the primary interface is
 * the AudioStream and the device handle is mostly hidden away; destroying
 * a stream created through this function will also close the device, stream
 * bindings cannot be changed, etc. One other quirk of this is that the device
 * is started in a _paused_ state and must be explicitly resumed; this is
 * partially to offer a clean migration for SDL2 apps and partially because
 * the app might have to do more setup before playback begins; in the
 * non-simplified form, nothing will play until a stream is bound to a device,
 * so they start _unpaused_.
 *
 * ## Channel layouts
 *
 * Audio data passing through SDL is uncompressed PCM data, interleaved. One
 * can provide their own decompression through an MP3, etc, decoder, but SDL
 * does not provide this directly. Each interleaved channel of data is meant
 * to be in a specific order.
 *
 * Abbreviations:
 *
 * - FRONT = single mono speaker
 * - FL = front left speaker
 * - FR = front right speaker
 * - FC = front center speaker
 * - BL = back left speaker
 * - BR = back right speaker
 * - SR = surround right speaker
 * - SL = surround left speaker
 * - BC = back center speaker
 * - LFE = low-frequency speaker
 *
 * These are listed in the order they are laid out in memory, so "FL, FR"
 * means "the front left speaker is laid out in memory first, then the front
 * right, then it repeats for the next audio frame".
 *
 * - 1 channel (mono) layout: FRONT
 * - 2 channels (stereo) layout: FL, FR
 * - 3 channels (2.1) layout: FL, FR, LFE
 * - 4 channels (quad) layout: FL, FR, BL, BR
 * - 5 channels (4.1) layout: FL, FR, LFE, BL, BR
 * - 6 channels (5.1) layout: FL, FR, FC, LFE, BL, BR (last two can also be
 *   SL, SR)
 * - 7 channels (6.1) layout: FL, FR, FC, LFE, BC, SL, SR
 * - 8 channels (7.1) layout: FL, FR, FC, LFE, BL, BR, SL, SR
 *
 * This is the same order as DirectSound expects, but applied to all
 * platforms; SDL will swizzle the channels as necessary if a platform expects
 * something different.
 *
 * AudioStream can also be provided channel maps to change this ordering
 * to whatever is necessary, in other audio processing scenarios.
 *
 * @{
 */

/**
 * Format specifier for audio data.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa AudioFormat
 */
using AudioSpec = SDL_AudioSpec;

// Forward decl
struct AudioDeviceRef;

// Forward decl
struct AudioDevice;

// Forward decl
struct AudioStreamRef;

// Forward decl
struct AudioStream;

// Forward decl
struct AudioStreamLock;

#ifdef SDL3PP_DOC

/**
 * Mask of bits in an AudioFormat that contains the format bit size.
 *
 * Generally one should use AudioFormat.GetBitSize instead of this macro
 * directly.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_AUDIO_MASK_BITSIZE (0xFFu)

/**
 * Mask of bits in an AudioFormat that contain the floating point flag.
 *
 * Generally one should use AudioFormat.IsFloat instead of this macro directly.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_AUDIO_MASK_FLOAT (1u << 8)

/**
 * Mask of bits in an AudioFormat that contain the bigendian flag.
 *
 * Generally one should use AudioFormat.IsBigEndian or
 * AudioFormat.IsLittleEndian instead of this macro directly.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_AUDIO_MASK_BIG_ENDIAN (1u << 12)

/**
 * Mask of bits in an AudioFormat that contain the signed data flag.
 *
 * Generally one should use AudioFormat.IsSigned instead of this macro directly.
 *
 * @since This macro is available since SDL 3.2.0.
 */
#define SDL_AUDIO_MASK_SIGNED (1u << 15)

#endif // SDL3PP_DOC

/**
 * Audio format.
 *
 * @since This enum is available since SDL 3.2.0.
 *
 * @sa AudioFormat.GetBitSize
 * @sa AudioFormat.GetByteSize
 * @sa AudioFormat.IsInt
 * @sa AudioFormat.IsFloat
 * @sa AudioFormat.IsBigEndian
 * @sa AudioFormat.IsLittleEndian
 * @sa AudioFormat.IsSigned
 * @sa AudioFormat.IsUnsigned
 */
class AudioFormat
{
  SDL_AudioFormat m_audioFormat;

public:
  /**
   * Wraps AudioFormat.
   *
   * @param audioFormat the value to be wrapped
   */
  constexpr AudioFormat(SDL_AudioFormat audioFormat = {})
    : m_audioFormat(audioFormat)
  {
  }

  /**
   * Define an AudioFormat value.
   *
   * SDL does not support custom audio formats, so this function is not of much
   * use externally, but it can be illustrative as to what the various bits of
   * an AudioFormat mean.
   *
   * For example, AUDIO_S32LE looks like this:
   *
   * ```c
   * SDL_DEFINE_AUDIO_FORMAT(1, 0, 0, 32)
   * ```
   *
   * @param sign true for signed data, false for unsigned data.
   * @param bigendian true for bigendian data, false for littleendian data.
   * @param flt true for floating point data, false for integer data.
   * @param size number of bits per sample.
   * @post a format value in the style of AudioFormat.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr AudioFormat(bool sign, bool bigendian, bool flt, Uint16 size)
    : m_audioFormat(
        SDL_AudioFormat(SDL_DEFINE_AUDIO_FORMAT(sign, bigendian, flt, size)))
  {
  }

  /**
   * Default comparison operator
   */
  constexpr bool operator==(const AudioFormat& other) const = default;

  /**
   * Compares with the underlying type
   */
  constexpr bool operator==(SDL_AudioFormat audioFormat) const
  {
    return operator==(AudioFormat(audioFormat));
  }

  /**
   * Unwraps to the underlying AudioFormat.
   *
   * @returns the underlying AudioFormat.
   */
  constexpr operator SDL_AudioFormat() const { return m_audioFormat; }

  /**
   * Check if valid.
   *
   * @returns True if valid state, false otherwise.
   */
  constexpr explicit operator bool() const { return m_audioFormat != 0; }

  /**
   * Retrieve the size, in bits, from an AudioFormat.
   *
   * For example, `AudioFormat.GetBitSize(AUDIO_S16)` returns 16.
   *
   * @returns data size in bits.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr Uint16 GetBitSize() { return SDL_AUDIO_BITSIZE(m_audioFormat); }

  /**
   * Retrieve the size, in bytes, from an AudioFormat.
   *
   * For example, `AudioFormat.GetByteSize(AUDIO_S16)` returns 2.
   *
   * @returns data size in bytes.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr Uint16 GetByteSize() { return SDL_AUDIO_BYTESIZE(m_audioFormat); }

  /**
   * Determine if an AudioFormat represents floating point data.
   *
   * For example, `AudioFormat.IsFloat(AUDIO_S16)` returns 0.
   *
   * @returns non-zero if format is floating point, zero otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsFloat() { return SDL_AUDIO_ISFLOAT(m_audioFormat); }

  /**
   * Determine if an AudioFormat represents bigendian data.
   *
   * For example, `AudioFormat.IsBigEndian(AUDIO_S16LE)` returns 0.
   *
   * @returns non-zero if format is bigendian, zero otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsBigEndian() { return SDL_AUDIO_ISBIGENDIAN(m_audioFormat); }

  /**
   * Determine if an AudioFormat represents littleendian data.
   *
   * For example, `AudioFormat.IsLittleEndian(AUDIO_S16BE)` returns 0.
   *
   * @returns non-zero if format is littleendian, zero otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsLittleEndian()
  {
    return SDL_AUDIO_ISLITTLEENDIAN(m_audioFormat);
  }

  /**
   * Determine if an AudioFormat represents signed data.
   *
   * For example, `AudioFormat.IsSigned(AUDIO_U8)` returns 0.
   *
   * @returns non-zero if format is signed, zero otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsSigned() { return SDL_AUDIO_ISSIGNED(m_audioFormat); }

  /**
   * Determine if an AudioFormat represents integer data.
   *
   * For example, `AudioFormat.IsInt(AUDIO_F32)` returns 0.
   *
   * @returns non-zero if format is integer, zero otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsInt() { return SDL_AUDIO_ISINT(m_audioFormat); }

  /**
   * Determine if an AudioFormat represents unsigned data.
   *
   * For example, `AudioFormat.IsUnsigned(AUDIO_S16)` returns 0.
   *
   * @returns non-zero if format is unsigned, zero otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr bool IsUnsigned() { return SDL_AUDIO_ISUNSIGNED(m_audioFormat); }

  /**
   * Get the human readable name of an audio format.
   *
   * @returns the human readable name of the specified audio format or
   *          "AUDIO_UNKNOWN" if the format isn't recognized.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  const char* GetName() const { return SDL_GetAudioFormatName(m_audioFormat); }

  /**
   * Get the appropriate memset value for silencing an audio format.
   *
   * The value returned by this function can be used as the second argument to
   * memset (or memset) to set an audio buffer in a specific format to
   * silence.
   *
   * @returns a byte value that can be passed to memset.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  int GetSilenceValue() const
  {
    return SDL_GetSilenceValueForFormat(m_audioFormat);
  }
};

// Unfortunate name clash with SDL_oldnames.h
#undef AUDIO_U8
#undef AUDIO_S8
#undef AUDIO_S16
#undef AUDIO_S32
#undef AUDIO_F32

constexpr SDL_AudioFormat AUDIO_UNKNOWN =
  SDL_AUDIO_UNKNOWN; ///< Unspecified audio format.

constexpr SDL_AudioFormat AUDIO_U8 = SDL_AUDIO_U8; ///< Unsigned 8-bit samples.

constexpr SDL_AudioFormat AUDIO_S8 = SDL_AUDIO_S8; ///< Signed 8-bit samples.

constexpr SDL_AudioFormat AUDIO_S16LE =
  SDL_AUDIO_S16LE; ///< Signed 16-bit samples.

constexpr SDL_AudioFormat AUDIO_S16BE =
  SDL_AUDIO_S16BE; ///< As above, but big-endian byte order.

constexpr SDL_AudioFormat AUDIO_S32LE =
  SDL_AUDIO_S32LE; ///< 32-bit integer samples

constexpr SDL_AudioFormat AUDIO_S32BE =
  SDL_AUDIO_S32BE; ///< As above, but big-endian byte order.

constexpr SDL_AudioFormat AUDIO_F32LE =
  SDL_AUDIO_F32LE; ///< 32-bit floating point samples

constexpr SDL_AudioFormat AUDIO_F32BE =
  SDL_AUDIO_F32BE; ///< As above, but big-endian byte order.

constexpr SDL_AudioFormat AUDIO_S16 = SDL_AUDIO_S16; ///< AUDIO_S16

constexpr SDL_AudioFormat AUDIO_S32 = SDL_AUDIO_S32; ///< AUDIO_S32

constexpr SDL_AudioFormat AUDIO_F32 = SDL_AUDIO_F32; ///< AUDIO_F32

/**
 * A callback that fires when data is about to be fed to an audio device.
 *
 * This is useful for accessing the final mix, perhaps for writing a
 * visualizer or applying a final effect to the audio data before playback.
 *
 * This callback should run as quickly as possible and not block for any
 * significant time, as this callback delays submission of data to the audio
 * device, which can cause audio playback problems.
 *
 * The postmix callback _must_ be able to handle any audio data format
 * specified in `spec`, which can change between callbacks if the audio device
 * changed. However, this only covers frequency and channel count; data is
 * always provided here in AUDIO_F32 format.
 *
 * The postmix callback runs _after_ logical device gain and audiostream gain
 * have been applied, which is to say you can make the output data louder at
 * this point than the gain settings would suggest.
 *
 * @param userdata a pointer provided by the app through
 *                 AudioDeviceRef.SetPostmixCallback, for its own use.
 * @param spec the current format of audio that is to be submitted to the
 *             audio device.
 * @param buffer the buffer of audio samples to be submitted. The callback can
 *               inspect and/or modify this data.
 * @param buflen the size of `buffer` in bytes.
 *
 * @threadsafety This will run from a background thread owned by SDL. The
 *               application is responsible for locking resources the callback
 *               touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AudioDeviceRef.SetPostmixCallback
 */
using AudioPostmixCallback = SDL_AudioPostmixCallback;

/**
 * A callback that fires when data is about to be fed to an audio device.
 *
 * This is useful for accessing the final mix, perhaps for writing a
 * visualizer or applying a final effect to the audio data before playback.
 *
 * This callback should run as quickly as possible and not block for any
 * significant time, as this callback delays submission of data to the audio
 * device, which can cause audio playback problems.
 *
 * The postmix callback _must_ be able to handle any audio data format
 * specified in `spec`, which can change between callbacks if the audio device
 * changed. However, this only covers frequency and channel count; data is
 * always provided here in AUDIO_F32 format.
 *
 * The postmix callback runs _after_ logical device gain and audiostream gain
 * have been applied, which is to say you can make the output data louder at
 * this point than the gain settings would suggest.
 *
 * @param spec the current format of audio that is to be submitted to the
 *             audio device.
 * @param buffer the buffer of audio samples to be submitted. The callback can
 *               inspect and/or modify this data.
 *
 * @threadsafety This will run from a background thread owned by SDL. The
 *               application is responsible for locking resources the callback
 *               touches that need to be protected.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AudioDeviceRef.SetPostmixCallback
 * @sa AudioPostmixCallback
 */
using AudioPostmixCB =
  std::function<void(const AudioSpec& spec, std::span<float> buffer)>;

/**
 * SDL Audio Device instance IDs.
 *
 * Zero is used to signify an invalid/null device.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 *
 * @sa AudioDevice
 * @sa AudioDeviceRef
 */
struct AudioDeviceRef : Resource<SDL_AudioDeviceID>
{
  using Resource::Resource;

  /// Comparison
  constexpr auto operator<=>(AudioDeviceRef other) const
  {
    return get() <=> other.get();
  }

  /**
   * Get the human-readable name of a specific audio device.
   *
   * @returns the name of the audio device, or nullptr on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa GetAudioPlaybackDevices
   * @sa GetAudioRecordingDevices
   */
  const char* GetName() const { return SDL_GetAudioDeviceName(get()); }

  /**
   * Get the current audio format of a specific audio device.
   *
   * For an opened device, this will report the format the device is currently
   * using. If the device isn't yet opened, this will report the device's
   * preferred format (or a reasonable default if this can't be determined).
   *
   * You may also specify AUDIO_DEVICE_DEFAULT_PLAYBACK or
   * AUDIO_DEVICE_DEFAULT_RECORDING here, which is useful for getting a
   * reasonable recommendation before opening the system-recommended default
   * device.
   *
   * You can also use this to request the current device buffer size. This is
   * specified in sample frames and represents the amount of data SDL will feed
   * to the physical hardware in each chunk. This can be converted to
   * milliseconds of audio with the following equation:
   *
   * `ms = (int) ((((Sint64) frames) * 1000) / spec.freq);`
   *
   * Buffer size is only important if you need low-level control over the audio
   * playback timing. Most apps do not need this.
   *
   * @param sample_frames pointer to store device buffer size, in sample frames.
   *                      Can be nullptr.
   * @returns the device format details on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  AudioSpec GetFormat(int* sample_frames = nullptr) const
  {
    AudioSpec spec;
    CheckError(SDL_GetAudioDeviceFormat(get(), &spec, sample_frames));
    return spec;
  }

  /**
   * Get the current channel map of an audio device.
   *
   * Channel maps are optional; most things do not need them, instead passing
   * data in the [order that SDL expects](CategoryAudio#channel-layouts).
   *
   * Audio devices usually have no remapping applied. This is represented by
   * returning nullptr, and does not signify an error.
   *
   * @returns an array of the current channel mapping, with as many elements as
   *          the current output spec's channels, or nullptr if default.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetInputChannelMap
   */
  OwnArray<int> GetChannelMap() const
  {
    int count;
    auto data = SDL_GetAudioDeviceChannelMap(get(), &count);
    return OwnArray<int>{data, size_t(count)};
  }

  /**
   * Determine if an audio device is physical (instead of logical).
   *
   * An AudioDeviceRef that represents physical hardware is a physical device;
   * there is one for each piece of hardware that SDL can see. Logical devices
   * are created by calling AudioDevice.Open or
   * AudioStream.OpenAudioDeviceStream, and while each is associated with a
   * physical device, there can be any number of logical devices on one physical
   * device.
   *
   * For the most part, logical and physical IDs are interchangeable--if you try
   * to open a logical device, SDL understands to assign that effort to the
   * underlying physical device, etc. However, it might be useful to know if an
   * arbitrary device ID is physical or logical. This function reports which.
   *
   * This function may return either true or false for invalid device IDs.
   *
   * @returns true if devid is a physical device, false if it is logical.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool IsPhysical() const { return SDL_IsAudioDevicePhysical(get()); }

  /**
   * Determine if an audio device is a playback device (instead of recording).
   *
   * This function may return either true or false for invalid device IDs.
   *
   * @returns true if devid is a playback device, false if it is recording.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  bool IsPlayback() const { return SDL_IsAudioDevicePlayback(get()); }

  /**
   * Use this function to pause audio playback on a specified device.
   *
   * This function pauses audio processing for a given device. Any bound audio
   * streams will not progress, and no audio will be generated. Pausing one
   * device does not prevent other unpaused devices from running.
   *
   * Unlike in SDL2, audio devices start in an _unpaused_ state, since an app
   * has to bind a stream before any audio will flow. Pausing a paused device is
   * a legal no-op.
   *
   * Pausing a device can be useful to halt all audio without unbinding all the
   * audio streams. This might be useful while a game is paused, or a level is
   * loading, etc.
   *
   * Physical devices can not be paused or unpaused, only logical devices
   * created through AudioDevice.Open() can be.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDeviceRef.Resume
   * @sa AudioDeviceRef.Paused
   */
  void Pause() { CheckError(SDL_PauseAudioDevice(get())); }

  /**
   * Use this function to unpause audio playback on a specified device.
   *
   * This function unpauses audio processing for a given device that has
   * previously been paused with AudioDeviceRef.Pause(). Once unpaused, any
   * bound audio streams will begin to progress again, and audio can be
   * generated.
   *
   * Unlike in SDL2, audio devices start in an _unpaused_ state, since an app
   * has to bind a stream before any audio will flow. Unpausing an unpaused
   * device is a legal no-op.
   *
   * Physical devices can not be paused or unpaused, only logical devices
   * created through AudioDevice.Open() can be.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDeviceRef.Paused
   * @sa AudioDeviceRef.Pause
   */
  void Resume() { CheckError(SDL_ResumeAudioDevice(get())); }

  /**
   * Use this function to query if an audio device is paused.
   *
   * Unlike in SDL2, audio devices start in an _unpaused_ state, since an app
   * has to bind a stream before any audio will flow.
   *
   * Physical devices can not be paused or unpaused, only logical devices
   * created through AudioDevice.Open() can be. Physical and invalid
   * device IDs will report themselves as unpaused here.
   *
   * @returns true if device is valid and paused, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDeviceRef.Pause
   * @sa AudioDeviceRef.Resume
   */
  bool Paused() const { return SDL_AudioDevicePaused(get()); }

  /**
   * Get the gain of an audio device.
   *
   * The gain of a device is its volume; a larger gain means a louder output,
   * with a gain of zero being silence.
   *
   * Audio devices default to a gain of 1.0f (no change in output).
   *
   * Physical devices may not have their gain changed, only logical devices, and
   * this function will always return -1.0f when used on physical devices.
   *
   * @returns the gain of the device or -1.0f on failure; call GetError()
   *          for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDeviceRef.SetGain
   */
  float GetGain() const { return SDL_GetAudioDeviceGain(get()); }

  /**
   * Change the gain of an audio device.
   *
   * The gain of a device is its volume; a larger gain means a louder output,
   * with a gain of zero being silence.
   *
   * Audio devices default to a gain of 1.0f (no change in output).
   *
   * Physical devices may not have their gain changed, only logical devices, and
   * this function will always return false when used on physical devices. While
   * it might seem attractive to adjust several logical devices at once in this
   * way, it would allow an app or library to interfere with another portion of
   * the program's otherwise-isolated devices.
   *
   * This is applied, along with any per-audiostream gain, during playback to
   * the hardware, and can be continuously changed to create various effects. On
   * recording devices, this will adjust the gain before passing the data into
   * an audiostream; that recording audiostream can then adjust its gain further
   * when outputting the data elsewhere, if it likes, but that second gain is
   * not applied until the data leaves the audiostream again.
   *
   * @param gain the gain. 1.0f is no change, 0.0f is silence.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDeviceRef.GetGain
   */
  void SetGain(float gain) { CheckError(SDL_SetAudioDeviceGain(get(), gain)); }

  /**
   * Bind a list of audio streams to an audio device.
   *
   * Audio data will flow through any bound streams. For a playback device, data
   * for all bound streams will be mixed together and fed to the device. For a
   * recording device, a copy of recorded data will be provided to each bound
   * stream.
   *
   * Audio streams can only be bound to an open device. This operation is
   * atomic--all streams bound in the same call will start processing at the
   * same time, so they can stay in sync. Also: either all streams will be bound
   * or none of them will be.
   *
   * It is an error to bind an already-bound stream; it must be explicitly
   * unbound first.
   *
   * Binding a stream to a device will set its output format for playback
   * devices, and its input format for recording devices, so they match the
   * device's settings. The caller is welcome to change the other end of the
   * stream's format at any time with AudioStreamRef.SetFormat().
   *
   * @param streams an array of audio streams to bind.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDeviceRef.BindAudioStreams
   * @sa AudioStreamRef.Unbind
   * @sa AudioStreamRef.GetDevice
   */
  void BindAudioStreams(std::span<AudioStreamRef> streams);

  /**
   * Bind a single audio stream to an audio device.
   *
   * This is a convenience function, equivalent to calling
   * `AudioDeviceRef.BindAudioStreams(devid, &stream, 1)`.
   *
   * @param stream an audio stream to bind to a device.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDeviceRef.BindAudioStreams
   * @sa AudioStreamRef.Unbind
   * @sa AudioStreamRef.GetDevice
   */
  void BindAudioStream(AudioStreamRef stream);

  /**
   * Set a callback that fires when data is about to be fed to an audio device.
   *
   * This is useful for accessing the final mix, perhaps for writing a
   * visualizer or applying a final effect to the audio data before playback.
   *
   * The buffer is the final mix of all bound audio streams on an opened device;
   * this callback will fire regularly for any device that is both opened and
   * unpaused. If there is no new data to mix, either because no streams are
   * bound to the device or all the streams are empty, this callback will still
   * fire with the entire buffer set to silence.
   *
   * This callback is allowed to make changes to the data; the contents of the
   * buffer after this call is what is ultimately passed along to the hardware.
   *
   * The callback is always provided the data in float format (values from -1.0f
   * to 1.0f), but the number of channels or sample rate may be different than
   * the format the app requested when opening the device; SDL might have had to
   * manage a conversion behind the scenes, or the playback might have jumped to
   * new physical hardware when a system default changed, etc. These details may
   * change between calls. Accordingly, the size of the buffer might change
   * between calls as well.
   *
   * This callback can run at any time, and from any thread; if you need to
   * serialize access to your app's data, you should provide and use a mutex or
   * other synchronization device.
   *
   * All of this to say: there are specific needs this callback can fulfill, but
   * it is not the simplest interface. Apps should generally provide audio in
   * their preferred format through an AudioStreamRef and let SDL handle the
   * difference.
   *
   * This function is extremely time-sensitive; the callback should do the least
   * amount of work possible and return as quickly as it can. The longer the
   * callback runs, the higher the risk of audio dropouts or other problems.
   *
   * This function will block until the audio device is in between iterations,
   * so any existing callback that might be running will finish before this
   * function sets the new callback and returns.
   *
   * Setting a nullptr callback function disables any previously-set callback.
   *
   * @param callback a callback function to be called. Can be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetPostmixCallback(AudioPostmixCB callback);

  /**
   * Set a callback that fires when data is about to be fed to an audio device.
   *
   * This is useful for accessing the final mix, perhaps for writing a
   * visualizer or applying a final effect to the audio data before playback.
   *
   * The buffer is the final mix of all bound audio streams on an opened device;
   * this callback will fire regularly for any device that is both opened and
   * unpaused. If there is no new data to mix, either because no streams are
   * bound to the device or all the streams are empty, this callback will still
   * fire with the entire buffer set to silence.
   *
   * This callback is allowed to make changes to the data; the contents of the
   * buffer after this call is what is ultimately passed along to the hardware.
   *
   * The callback is always provided the data in float format (values from -1.0f
   * to 1.0f), but the number of channels or sample rate may be different than
   * the format the app requested when opening the device; SDL might have had to
   * manage a conversion behind the scenes, or the playback might have jumped to
   * new physical hardware when a system default changed, etc. These details may
   * change between calls. Accordingly, the size of the buffer might change
   * between calls as well.
   *
   * This callback can run at any time, and from any thread; if you need to
   * serialize access to your app's data, you should provide and use a mutex or
   * other synchronization device.
   *
   * All of this to say: there are specific needs this callback can fulfill, but
   * it is not the simplest interface. Apps should generally provide audio in
   * their preferred format through an AudioStreamRef and let SDL handle the
   * difference.
   *
   * This function is extremely time-sensitive; the callback should do the least
   * amount of work possible and return as quickly as it can. The longer the
   * callback runs, the higher the risk of audio dropouts or other problems.
   *
   * This function will block until the audio device is in between iterations,
   * so any existing callback that might be running will finish before this
   * function sets the new callback and returns.
   *
   * Setting a nullptr callback function disables any previously-set callback.
   *
   * @param callback a callback function to be called. Can be nullptr.
   * @param userdata app-controlled pointer passed to callback. Can be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  void SetPostmixCallback(AudioPostmixCallback callback, void* userdata)
  {
    CheckError(SDL_SetAudioPostmixCallback(get(), callback, userdata));
  }

  /**
   * Close a previously-opened audio device.
   *
   * The application should close open audio devices once they are no longer
   * needed.
   *
   * This function may block briefly while pending audio data is played by the
   * hardware, so that applications don't drop the last buffer of data they
   * supplied if terminating immediately afterwards.
   *
   * @param resource an audio device id previously returned by
   *                 AudioDevice.Open().
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDevice.Open
   */
  static void reset(SDL_AudioDeviceID resource)
  {
    KeyValueCallbackWrapper<AudioDeviceRef, AudioPostmixCB>::release(resource);
    SDL_CloseAudioDevice(resource);
  }
};

/**
 * Handle to an owned audioDevice
 *
 * @cat resource
 *
 * @sa AudioDeviceRef
 */
struct AudioDevice : ResourceUnique<AudioDeviceRef>
{
  using ResourceUnique::ResourceUnique;

  /**
   * Open a specific audio device.
   *
   * You can open both playback and recording devices through this function.
   * Playback devices will take data from bound audio streams, mix it, and send
   * it to the hardware. Recording devices will feed any bound audio streams
   * with a copy of any incoming data.
   *
   * An opened audio device starts out with no audio streams bound. To start
   * audio playing, bind a stream and supply audio data to it. Unlike SDL2,
   * there is no audio callback; you only bind audio streams and make sure they
   * have data flowing into them (however, you can simulate SDL2's semantics
   * fairly closely by using AudioStream.OpenAudioDeviceStream instead of this
   * function).
   *
   * If you don't care about opening a specific device, pass a `devid` of either
   * `AUDIO_DEVICE_DEFAULT_PLAYBACK` or
   * `AUDIO_DEVICE_DEFAULT_RECORDING`. In this case, SDL will try to pick
   * the most reasonable default, and may also switch between physical devices
   * seamlessly later, if the most reasonable default changes during the
   * lifetime of this opened device (user changed the default in the OS's system
   * preferences, the default got unplugged so the system jumped to a new
   * default, the user plugged in headphones on a mobile device, etc). Unless
   * you have a good reason to choose a specific device, this is probably what
   * you want.
   *
   * You may request a specific format for the audio device, but there is no
   * promise the device will honor that request for several reasons. As such,
   * it's only meant to be a hint as to what data your app will provide. Audio
   * streams will accept data in whatever format you specify and manage
   * conversion for you as appropriate. AudioDeviceRef.GetFormat can tell you
   * the preferred format for the device before opening and the actual format
   * the device is using after opening.
   *
   * It's legal to open the same device ID more than once; each successful open
   * will generate a new logical AudioDeviceRef that is managed separately
   * from others on the same physical device. This allows libraries to open a
   * device separately from the main app and bind its own streams without
   * conflicting.
   *
   * It is also legal to open a device ID returned by a previous call to this
   * function; doing so just creates another logical device on the same physical
   * device. This may be useful for making logical groupings of audio streams.
   *
   * This function returns the opened device ID on success. This is a new,
   * unique AudioDeviceRef that represents a logical device.
   *
   * Some backends might offer arbitrary devices (for example, a networked audio
   * protocol that can connect to an arbitrary server). For these, as a change
   * from SDL2, you should open a default device ID and use an SDL hint to
   * specify the target if you care, or otherwise let the backend figure out a
   * reasonable default. Most backends don't offer anything like this, and often
   * this would be an end user setting an environment variable for their custom
   * need, and not something an application should specifically manage.
   *
   * When done with an audio device, possibly at the end of the app's life, one
   * should call AudioDevice.Close() on the returned device id.
   *
   * @param devid the device instance id to open, or
   *              AUDIO_DEVICE_DEFAULT_PLAYBACK or
   *              AUDIO_DEVICE_DEFAULT_RECORDING for the most reasonable
   *              default device.
   * @param spec the requested device configuration. Can be nullptr to use
   *             reasonable defaults.
   * @returns the device ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDevice.Close
   * @sa AudioDeviceRef.GetFormat
   */
  static AudioDevice Open(AudioDeviceRef devid,
                          OptionalRef<const SDL_AudioSpec> spec)
  {
    return AudioDevice(CheckError(SDL_OpenAudioDevice(devid, spec)));
  }

  /**
   * Close a previously-opened audio device.
   *
   * The application should close open audio devices once they are no longer
   * needed.
   *
   * This function may block briefly while pending audio data is played by the
   * hardware, so that applications don't drop the last buffer of data they
   * supplied if terminating immediately afterwards.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDevice.Open
   */
  void Close() { reset(); }
};

/**
 * Unsafe Handle to audioDevice
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa AudioDeviceRef
 */
struct AudioDeviceUnsafe : ResourceUnsafe<AudioDeviceRef>
{
  using ResourceUnsafe::ResourceUnsafe;

  /**
   * Constructs AudioDeviceUnsafe from AudioDevice.
   */
  constexpr explicit AudioDeviceUnsafe(AudioDevice&& other)
    : AudioDeviceUnsafe(other.release())
  {
  }
};

/**
 * A value used to request a default playback audio device.
 *
 * Several functions that require an AudioDeviceRef will accept this value
 * to signify the app just wants the system to choose a default device instead
 * of the app providing a specific one.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr AudioDeviceRef AUDIO_DEVICE_DEFAULT_PLAYBACK =
  SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK;

/**
 * A value used to request a default recording audio device.
 *
 * Several functions that require an AudioDeviceRef will accept this value
 * to signify the app just wants the system to choose a default device instead
 * of the app providing a specific one.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr AudioDeviceRef AUDIO_DEVICE_DEFAULT_RECORDING =
  SDL_AUDIO_DEVICE_DEFAULT_RECORDING;

/**
 * Calculate the size of each audio frame (in bytes) from an AudioSpec.
 *
 * This reports on the size of an audio sample frame: stereo Sint16 data (2
 * channels of 2 bytes each) would be 4 bytes per frame, for example.
 *
 * @param x an AudioSpec to query.
 * @returns the number of bytes used per sample frame.
 *
 * @threadsafety It is safe to call this macro from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr int AudioFrameSize(const AudioSpec& x)
{
  return SDL_AUDIO_FRAMESIZE(x);
}

/**
 * A callback that fires when data passes through an AudioStreamRef.
 *
 * Apps can (optionally) register a callback with an audio stream that is
 * called when data is added with AudioStreamRef.PutData, or requested with
 * AudioStreamRef.GetData.
 *
 * Two values are offered here: one is the amount of additional data needed to
 * satisfy the immediate request (which might be zero if the stream already
 * has enough data queued) and the other is the total amount being requested.
 * In a Get call triggering a Put callback, these values can be different. In
 * a Put call triggering a Get callback, these values are always the same.
 *
 * Byte counts might be slightly overestimated due to buffering or resampling,
 * and may change from call to call.
 *
 * This callback is not required to do anything. Generally this is useful for
 * adding/reading data on demand, and the app will often put/get data as
 * appropriate, but the system goes on with the data currently available to it
 * if this callback does nothing.
 *
 * @param stream the SDL audio stream associated with this callback.
 * @param additional_amount the amount of data, in bytes, that is needed right
 *                          now.
 * @param total_amount the total amount of data requested, in bytes, that is
 *                     requested or available.
 * @param userdata an opaque pointer provided by the app for their personal
 *                 use.
 *
 * @threadsafety This callbacks may run from any thread, so if you need to
 *               protect shared data, you should use AudioStreamRef.Lock to
 *               serialize access; this lock will be held before your callback
 *               is called, so your callback does not need to manage the lock
 *               explicitly.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AudioStreamRef.SetGetCallback
 * @sa AudioStreamRef.SetPutCallback
 */
using AudioStreamCallback = SDL_AudioStreamCallback;

/**
 * A callback that fires when data passes through an AudioStreamRef.
 *
 * Apps can (optionally) register a callback with an audio stream that is
 * called when data is added with AudioStreamRef.PutData, or requested with
 * AudioStreamRef.GetData.
 *
 * Two values are offered here: one is the amount of additional data needed to
 * satisfy the immediate request (which might be zero if the stream already
 * has enough data queued) and the other is the total amount being requested.
 * In a Get call triggering a Put callback, these values can be different. In
 * a Put call triggering a Get callback, these values are always the same.
 *
 * Byte counts might be slightly overestimated due to buffering or resampling,
 * and may change from call to call.
 *
 * This callback is not required to do anything. Generally this is useful for
 * adding/reading data on demand, and the app will often put/get data as
 * appropriate, but the system goes on with the data currently available to it
 * if this callback does nothing.
 *
 * @param stream the SDL audio stream associated with this callback.
 * @param additional_amount the amount of data, in bytes, that is needed right
 *                          now.
 * @param total_amount the total amount of data requested, in bytes, that is
 *                     requested or available.
 *
 * @threadsafety This callbacks may run from any thread, so if you need to
 *               protect shared data, you should use AudioStreamRef.Lock to
 *               serialize access; this lock will be held before your callback
 *               is called, so your callback does not need to manage the lock
 *               explicitly.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AudioStreamRef.SetGetCallback
 * @sa AudioStreamRef.SetPutCallback
 * @sa AudioStreamCallback
 */
using AudioStreamCB = std::function<
  void(AudioStreamRef stream, int additional_amount, int total_amount)>;

/**
 * The opaque handle that represents an audio stream.
 *
 * AudioStreamRef is an audio conversion interface.
 *
 * - It can handle resampling data in chunks without generating artifacts,
 *   when it doesn't have the complete buffer available.
 * - It can handle incoming data in any variable size.
 * - It can handle input/output format changes on the fly.
 * - It can remap audio channels between inputs and outputs.
 * - You push data as you have it, and pull it when you need it
 * - It can also function as a basic audio data queue even if you just have
 *   sound that needs to pass from one place to another.
 * - You can hook callbacks up to them when more data is added or requested,
 *   to manage data on-the-fly.
 *
 * Audio streams are the core of the SDL3 audio interface. You create one or
 * more of them, bind them to an opened audio device, and feed data to them
 * (or for recording, consume data from them).
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa AudioStream.Create
 *
 * @cat resource
 *
 * @sa AudioStream.Create
 * @sa AudioStream
 */
struct AudioStreamRef : Resource<SDL_AudioStream*>
{
  using Resource::Resource;

  /**
   * Get the properties associated with an audio stream.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const
  {
    return CheckError(SDL_GetAudioStreamProperties(get()));
  }

  /**
   * Query the current input format of an audio stream.
   *
   * @returns the input audio format.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetFormat
   */
  AudioSpec GetInputFormat() const
  {
    AudioSpec spec;
    GetFormat(&spec, nullptr);
    return spec;
  }

  /**
   * Query the current output format of an audio stream.
   *
   * @returns the output audio format.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetFormat
   */
  AudioSpec GetOutputFormat() const
  {
    AudioSpec spec;
    GetFormat(nullptr, &spec);
    return spec;
  }

  /**
   * Query the current format of an audio stream.
   *
   * @param src_spec where to store the input audio format; ignored if nullptr.
   * @param dst_spec where to store the output audio format; ignored if nullptr.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetFormat
   */
  void GetFormat(AudioSpec* src_spec, AudioSpec* dst_spec) const
  {
    CheckError(SDL_GetAudioStreamFormat(get(), src_spec, dst_spec));
  }

  /**
   * Change the input format of an audio stream.
   *
   * Future calls to and AudioStreamRef.GetAvailable and
   * AudioStreamRef.GetData will reflect the new format, and future calls to
   * AudioStreamRef.PutData must provide data in the new input formats.
   *
   * Data that was previously queued in the stream will still be operated on in
   * the format that was current when it was added, which is to say you can put
   * the end of a sound file in one format to a stream, change formats for the
   * next sound file, and start putting that new data while the previous sound
   * file is still queued, and everything will still play back correctly.
   *
   * If a stream is bound to a device, then the format of the side of the stream
   * bound to a device cannot be changed (src_spec for recording devices,
   * dst_spec for playback devices). Attempts to make a change to this side will
   * be ignored, but this will not report an error. The other side's format can
   * be changed.
   *
   * @param spec the new format of the audio input.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.GetFormat
   * @sa AudioStreamRef.SetFrequencyRatio
   */
  void SetInputFormat(const AudioSpec& spec) { SetFormat(spec, std::nullopt); }

  /**
   * Change the output format of an audio stream.
   *
   * Future calls to and AudioStreamRef.GetAvailable and
   * AudioStreamRef.GetData will reflect the new format, and future calls to
   * AudioStreamRef.PutData must provide data in the new input formats.
   *
   * Data that was previously queued in the stream will still be operated on in
   * the format that was current when it was added, which is to say you can put
   * the end of a sound file in one format to a stream, change formats for the
   * next sound file, and start putting that new data while the previous sound
   * file is still queued, and everything will still play back correctly.
   *
   * If a stream is bound to a device, then the format of the side of the stream
   * bound to a device cannot be changed (src_spec for recording devices,
   * dst_spec for playback devices). Attempts to make a change to this side will
   * be ignored, but this will not report an error. The other side's format can
   * be changed.
   *
   * @param spec the new format of the audio output.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.GetFormat
   * @sa AudioStreamRef.SetFrequencyRatio
   */
  void SetOutputFormat(const AudioSpec& spec) { SetFormat(std::nullopt, spec); }

  /**
   * Change the input and output formats of an audio stream.
   *
   * Future calls to and AudioStreamRef.GetAvailable and AudioStreamRef.GetData
   * will reflect the new format, and future calls to AudioStreamRef.PutData
   * must provide data in the new input formats.
   *
   * Data that was previously queued in the stream will still be operated on in
   * the format that was current when it was added, which is to say you can put
   * the end of a sound file in one format to a stream, change formats for the
   * next sound file, and start putting that new data while the previous sound
   * file is still queued, and everything will still play back correctly.
   *
   * If a stream is bound to a device, then the format of the side of the stream
   * bound to a device cannot be changed (src_spec for recording devices,
   * dst_spec for playback devices). Attempts to make a change to this side will
   * be ignored, but this will not report an error. The other side's format can
   * be changed.
   *
   * @param src_spec the new format of the audio input; if std::nullopt, it is
   *                 not changed.
   * @param dst_spec the new format of the audio output; if std::nullopt, it is
   *                 not changed.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.GetFormat
   * @sa AudioStreamRef.SetFrequencyRatio
   */
  void SetFormat(OptionalRef<const AudioSpec> src_spec,
                 OptionalRef<const AudioSpec> dst_spec)
  {
    CheckError(SDL_SetAudioStreamFormat(get(), src_spec, dst_spec));
  }

  /**
   * Get the frequency ratio of an audio stream.
   *
   * @returns the frequency ratio of the stream or 0.0 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetFrequencyRatio
   */
  float GetFrequencyRatio() const
  {
    return SDL_GetAudioStreamFrequencyRatio(get());
  }

  /**
   * Change the frequency ratio of an audio stream.
   *
   * The frequency ratio is used to adjust the rate at which input data is
   * consumed. Changing this effectively modifies the speed and pitch of the
   * audio. A value greater than 1.0 will play the audio faster, and at a higher
   * pitch. A value less than 1.0 will play the audio slower, and at a lower
   * pitch.
   *
   * This is applied during AudioStreamRef.GetData, and can be continuously
   * changed to create various effects.
   *
   * @param ratio the frequency ratio. 1.0 is normal speed. Must be between 0.01
   *              and 100.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.GetFrequencyRatio
   * @sa AudioStreamRef.SetFormat
   */
  void SetFrequencyRatio(float ratio)
  {
    CheckError(SDL_SetAudioStreamFrequencyRatio(get(), ratio));
  }

  /**
   * Get the gain of an audio stream.
   *
   * The gain of a stream is its volume; a larger gain means a louder output,
   * with a gain of zero being silence.
   *
   * Audio streams default to a gain of 1.0f (no change in output).
   *
   * @returns the gain of the stream or -1.0f on failure; call GetError()
   *          for more information.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetGain
   */
  float GetGain() const { return SDL_GetAudioStreamGain(get()); }

  /**
   * Change the gain of an audio stream.
   *
   * The gain of a stream is its volume; a larger gain means a louder output,
   * with a gain of zero being silence.
   *
   * Audio streams default to a gain of 1.0f (no change in output).
   *
   * This is applied during AudioStreamRef.GetData, and can be continuously
   * changed to create various effects.
   *
   * @param gain the gain. 1.0f is no change, 0.0f is silence.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.GetGain
   */
  void SetGain(float gain) { CheckError(SDL_SetAudioStreamGain(get(), gain)); }

  /**
   * Get the current input channel map of an audio stream.
   *
   * Channel maps are optional; most things do not need them, instead passing
   * data in the [order that SDL expects](CategoryAudio#channel-layouts).
   *
   * Audio streams default to no remapping applied. This is represented by
   * returning nullptr, and does not signify an error.
   *
   * @returns an array of the current channel mapping, with as many elements as
   *          the current output spec's channels, or nullptr if default.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetInputChannelMap
   */
  OwnArray<int> GetInputChannelMap() const
  {
    int count;
    auto data = SDL_GetAudioStreamInputChannelMap(get(), &count);
    if (!data) return {};
    return OwnArray<int>{data, size_t(count)};
  }

  /**
   * Get the current output channel map of an audio stream.
   *
   * Channel maps are optional; most things do not need them, instead passing
   * data in the [order that SDL expects](CategoryAudio#channel-layouts).
   *
   * Audio streams default to no remapping applied. This is represented by
   * returning nullptr, and does not signify an error.
   *
   * @returns an array of the current channel mapping, with as many elements as
   *          the current output spec's channels, or nullptr if default.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetInputChannelMap
   */
  OwnArray<int> GetOutputChannelMap() const
  {
    int count;
    auto data = SDL_GetAudioStreamOutputChannelMap(get(), &count);
    if (!data) return {};
    return OwnArray<int>{data, size_t(count)};
  }

  /**
   * Set the current input channel map of an audio stream.
   *
   * Channel maps are optional; most things do not need them, instead passing
   * data in the [order that SDL expects](CategoryAudio#channel-layouts).
   *
   * The input channel map reorders data that is added to a stream via
   * AudioStreamRef.PutData. Future calls to AudioStreamRef.PutData must provide
   * data in the new channel order.
   *
   * Each item in the array represents an input channel, and its value is the
   * channel that it should be remapped to. To reverse a stereo signal's left
   * and right values, you'd have an array of `{ 1, 0 }`. It is legal to remap
   * multiple channels to the same thing, so `{ 1, 1 }` would duplicate the
   * right channel to both channels of a stereo signal. An element in the
   * channel map set to -1 instead of a valid channel will mute that channel,
   * setting it to a silence value.
   *
   * You cannot change the number of channels through a channel map, just
   * reorder/mute them.
   *
   * Data that was previously queued in the stream will still be operated on in
   * the order that was current when it was added, which is to say you can put
   * the end of a sound file in one order to a stream, change orders for the
   * next sound file, and start putting that new data while the previous sound
   * file is still queued, and everything will still play back correctly.
   *
   * Audio streams default to no remapping applied. Passing a nullptr channel
   * map is legal, and turns off remapping.
   *
   * SDL will copy the channel map; the caller does not have to save this array
   * after this call.
   *
   * If `count` is not equal to the current number of channels in the audio
   * stream's format, this will fail. This is a safety measure to make sure a
   * race condition hasn't changed the format while this call is setting the
   * channel map.
   *
   * Unlike attempting to change the stream's format, the input channel map on a
   * stream bound to a recording device is permitted to change at any time; any
   * data added to the stream from the device after this call will have the new
   * mapping, but previously-added data will still have the prior mapping.
   *
   * @param chmap the new channel map, nullptr to reset to default.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running. Don't change the
   *               stream's format to have a different number of channels from a
   *               a different thread at the same time, though!
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetInputChannelMap
   */
  void SetInputChannelMap(std::span<int> chmap)
  {
    SDL_assert_paranoid(chmap.size() < MAX_SINT32);
    CheckError(
      SDL_SetAudioStreamInputChannelMap(get(), chmap.data(), chmap.size()));
  }

  /**
   * Set the current output channel map of an audio stream.
   *
   * Channel maps are optional; most things do not need them, instead passing
   * data in the [order that SDL expects](CategoryAudio#channel-layouts).
   *
   * The output channel map reorders data that leaving a stream via
   * AudioStreamRef.GetData.
   *
   * Each item in the array represents an input channel, and its value is the
   * channel that it should be remapped to. To reverse a stereo signal's left
   * and right values, you'd have an array of `{ 1, 0 }`. It is legal to remap
   * multiple channels to the same thing, so `{ 1, 1 }` would duplicate the
   * right channel to both channels of a stereo signal. An element in the
   * channel map set to -1 instead of a valid channel will mute that channel,
   * setting it to a silence value.
   *
   * You cannot change the number of channels through a channel map, just
   * reorder/mute them.
   *
   * The output channel map can be changed at any time, as output remapping is
   * applied during AudioStreamRef.GetData.
   *
   * Audio streams default to no remapping applied. Passing a nullptr channel
   * map is legal, and turns off remapping.
   *
   * SDL will copy the channel map; the caller does not have to save this array
   * after this call.
   *
   * If `count` is not equal to the current number of channels in the audio
   * stream's format, this will fail. This is a safety measure to make sure a
   * race condition hasn't changed the format while this call is setting the
   * channel map.
   *
   * Unlike attempting to change the stream's format, the output channel map on
   * a stream bound to a recording device is permitted to change at any time;
   * any data added to the stream after this call will have the new mapping, but
   * previously-added data will still have the prior mapping. When the channel
   * map doesn't match the hardware's channel layout, SDL will convert the data
   * before feeding it to the device for playback.
   *
   * @param chmap the new channel map, nullptr to reset to default.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running. Don't change the
   *               stream's format to have a different number of channels from a
   *               a different thread at the same time, though!
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetInputChannelMap
   */
  void SetOutputChannelMap(std::span<int> chmap)
  {
    SDL_assert_paranoid(chmap.size() < MAX_SINT32);
    CheckError(
      SDL_SetAudioStreamOutputChannelMap(get(), chmap.data(), chmap.size()));
  }

  /**
   * Add data to the stream.
   *
   * This data must match the format/channels/samplerate specified in the latest
   * call to AudioStreamRef.SetFormat, or the format specified when creating the
   * stream if it hasn't been changed.
   *
   * Note that this call simply copies the unconverted data for later. This is
   * different than SDL2, where data was converted during the Put call and the
   * Get call would just dequeue the previously-converted data.
   *
   * @param buf a pointer to the audio data to add.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, but if the
   *               stream has a callback set, the caller might need to manage
   *               extra locking.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.Clear
   * @sa AudioStreamRef.Flush
   * @sa AudioStreamRef.GetData
   * @sa AudioStreamRef.GetQueued
   */
  void PutData(SourceBytes buf)
  {
    SDL_assert_paranoid(buf.size_bytes < MAX_SINT32);
    CheckError(SDL_PutAudioStreamData(get(), buf.data, buf.size_bytes));
  }

  /**
   * Get converted/resampled data from the stream.
   *
   * The input/output data format/channels/samplerate is specified when creating
   * the stream, and can be changed after creation by calling
   * AudioStreamRef.SetFormat.
   *
   * Note that any conversion and resampling necessary is done during this call,
   * and AudioStreamRef.PutData simply queues unconverted data for later. This
   * is different than SDL2, where that work was done while inputting new data
   * to the stream and requesting the output just copied the converted data.
   *
   * @param buf a buffer to fill with audio data.
   * @returns the number of bytes read from the stream or -1 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread, but if the
   *               stream has a callback set, the caller might need to manage
   *               extra locking.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.Clear
   * @sa AudioStreamRef.GetAvailable
   * @sa AudioStreamRef.PutData
   */
  int GetData(TargetBytes buf)
  {
    return SDL_GetAudioStreamData(get(), buf.data, buf.size_bytes);
  }

  /**
   * Get the number of converted/resampled bytes available.
   *
   * The stream may be buffering data behind the scenes until it has enough to
   * resample correctly, so this number might be lower than what you expect, or
   * even be zero. Add more data or flush the stream if you need the data now.
   *
   * If the stream has so much data that it would overflow an int, the return
   * value is clamped to a maximum value, but no queued data is lost; if there
   * are gigabytes of data queued, the app might need to read some of it with
   * AudioStreamRef.GetData before this function's return value is no longer
   * clamped.
   *
   * @returns the number of converted/resampled bytes available or -1 on
   *          failure; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.GetData
   * @sa AudioStreamRef.PutData
   */
  int GetAvailable() const { return SDL_GetAudioStreamAvailable(get()); }

  /**
   * Get the number of bytes currently queued.
   *
   * This is the number of bytes put into a stream as input, not the number that
   * can be retrieved as output. Because of several details, it's not possible
   * to calculate one number directly from the other. If you need to know how
   * much usable data can be retrieved right now, you should use
   * AudioStreamRef.GetAvailable() and not this function.
   *
   * Note that audio streams can change their input format at any time, even if
   * there is still data queued in a different format, so the returned byte
   * count will not necessarily match the number of _sample frames_ available.
   * Users of this API should be aware of format changes they make when feeding
   * a stream and plan accordingly.
   *
   * Queued data is not converted until it is consumed by
   * AudioStreamRef.GetData, so this value should be representative of the exact
   * data that was put into the stream.
   *
   * If the stream has so much data that it would overflow an int, the return
   * value is clamped to a maximum value, but no queued data is lost; if there
   * are gigabytes of data queued, the app might need to read some of it with
   * AudioStreamRef.GetData before this function's return value is no longer
   * clamped.
   *
   * @returns the number of bytes queued or -1 on failure; call GetError()
   *          for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.PutData
   * @sa AudioStreamRef.Clear
   */
  int GetQueued() const { return SDL_GetAudioStreamQueued(get()); }

  /**
   * Tell the stream that you're done sending data, and anything being buffered
   * should be converted/resampled and made available immediately.
   *
   * It is legal to add more data to a stream after flushing, but there may be
   * audio gaps in the output. Generally this is intended to signal the end of
   * input, so the complete output becomes available.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.PutData
   */
  void Flush() { CheckError(SDL_FlushAudioStream(get())); }

  /**
   * Clear any pending data in the stream.
   *
   * This drops any queued data, so there will be nothing to read from the
   * stream until more is added.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.GetAvailable
   * @sa AudioStreamRef.GetData
   * @sa AudioStreamRef.GetQueued
   * @sa AudioStreamRef.PutData
   */
  void Clear() { CheckError(SDL_ClearAudioStream(get())); }

  /**
   * Use this function to pause audio playback on the audio device associated
   * with an audio stream.
   *
   * This function pauses audio processing for a given device. Any bound audio
   * streams will not progress, and no audio will be generated. Pausing one
   * device does not prevent other unpaused devices from running.
   *
   * Pausing a device can be useful to halt all audio without unbinding all the
   * audio streams. This might be useful while a game is paused, or a level is
   * loading, etc.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.ResumeDevice
   */
  void PauseDevice() { CheckError(SDL_PauseAudioStreamDevice(get())); }

  /**
   * Use this function to unpause audio playback on the audio device associated
   * with an audio stream.
   *
   * This function unpauses audio processing for a given device that has
   * previously been paused. Once unpaused, any bound audio streams will begin
   * to progress again, and audio can be generated.
   *
   * Remember, AudioStream.OpenAudioDeviceStream opens device in a paused state,
   * so this function call is required for audio playback to begin on such
   * device.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.PauseDevice
   */
  void ResumeDevice() { CheckError(SDL_ResumeAudioStreamDevice(get())); }

  /**
   * Use this function to query if an audio device associated with a stream is
   * paused.
   *
   * Unlike in SDL2, audio devices start in an _unpaused_ state, since an app
   * has to bind a stream before any audio will flow.
   *
   * @returns true if device is valid and paused, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.PauseDevice
   * @sa AudioStreamRef.ResumeDevice
   */
  bool DevicePaused() const { return SDL_AudioStreamDevicePaused(get()); }

  /**
   * Lock an audio stream for serialized access.
   *
   * Each AudioStreamRef has an internal mutex it uses to protect its data
   * structures from threading conflicts. This function allows an app to lock
   * that mutex, which could be useful if registering callbacks on this stream.
   *
   * One does not need to lock a stream to use in it most cases, as the stream
   * manages this lock internally. However, this lock is held during callbacks,
   * which may run from arbitrary threads at any time, so if an app needs to
   * protect shared data during those callbacks, locking the stream guarantees
   * that the callback is not running while the lock is held.
   *
   * As this is just a wrapper over MutexRef.Lock for an internal lock; it has
   * all the same attributes (recursive locks are allowed, etc).
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamLock.Unlock
   */
  AudioStreamLock Lock();

  /**
   * Set a callback that runs when data is requested from an audio stream.
   *
   * This callback is called _before_ data is obtained from the stream, giving
   * the callback the chance to add more on-demand.
   *
   * The callback can (optionally) call AudioStreamRef.PutData() to add more
   * audio to the stream during this call; if needed, the request that triggered
   * this callback will obtain the new data immediately.
   *
   * The callback's `additional_amount` argument is roughly how many bytes of
   * _unconverted_ data (in the stream's input format) is needed by the caller,
   * although this may overestimate a little for safety. This takes into account
   * how much is already in the stream and only asks for any extra necessary to
   * resolve the request, which means the callback may be asked for zero bytes,
   * and a different amount on each call.
   *
   * The callback is not required to supply exact amounts; it is allowed to
   * supply too much or too little or none at all. The caller will get what's
   * available, up to the amount they requested, regardless of this callback's
   * outcome.
   *
   * Clearing or flushing an audio stream does not call this callback.
   *
   * This function obtains the stream's lock, which means any existing callback
   * (get or put) in progress will finish running before setting the new
   * callback.
   *
   * Setting a nullptr function turns off the callback.
   *
   * @param callback the new callback function to call when data is requested
   *                 from the stream.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetPutCallback
   */
  void SetGetCallback(AudioStreamCB callback);

  /**
   * Set a callback that runs when data is requested from an audio stream.
   *
   * This callback is called _before_ data is obtained from the stream, giving
   * the callback the chance to add more on-demand.
   *
   * The callback can (optionally) call AudioStreamRef.PutData() to add more
   * audio to the stream during this call; if needed, the request that triggered
   * this callback will obtain the new data immediately.
   *
   * The callback's `additional_amount` argument is roughly how many bytes of
   * _unconverted_ data (in the stream's input format) is needed by the caller,
   * although this may overestimate a little for safety. This takes into account
   * how much is already in the stream and only asks for any extra necessary to
   * resolve the request, which means the callback may be asked for zero bytes,
   * and a different amount on each call.
   *
   * The callback is not required to supply exact amounts; it is allowed to
   * supply too much or too little or none at all. The caller will get what's
   * available, up to the amount they requested, regardless of this callback's
   * outcome.
   *
   * Clearing or flushing an audio stream does not call this callback.
   *
   * This function obtains the stream's lock, which means any existing callback
   * (get or put) in progress will finish running before setting the new
   * callback.
   *
   * Setting a nullptr function turns off the callback.
   *
   * @param callback the new callback function to call when data is requested
   *                 from the stream.
   * @param userdata an opaque pointer provided to the callback for its own
   *                 personal use.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetPutCallback
   */
  void SetGetCallback(AudioStreamCallback callback, void* userdata)
  {
    CheckError(SDL_SetAudioStreamGetCallback(get(), callback, userdata));
  }

  /**
   * Set a callback that runs when data is added to an audio stream.
   *
   * This callback is called _after_ the data is added to the stream, giving the
   * callback the chance to obtain it immediately.
   *
   * The callback can (optionally) call AudioStreamRef.GetData() to obtain
   * audio from the stream during this call.
   *
   * The callback's `additional_amount` argument is how many bytes of
   * _converted_ data (in the stream's output format) was provided by the
   * caller, although this may underestimate a little for safety. This value
   * might be less than what is currently available in the stream, if data was
   * already there, and might be less than the caller provided if the stream
   * needs to keep a buffer to aid in resampling. Which means the callback may
   * be provided with zero bytes, and a different amount on each call.
   *
   * The callback may call AudioStreamRef.GetAvailable to see the total amount
   * currently available to read from the stream, instead of the total provided
   * by the current call.
   *
   * The callback is not required to obtain all data. It is allowed to read less
   * or none at all. Anything not read now simply remains in the stream for
   * later access.
   *
   * Clearing or flushing an audio stream does not call this callback.
   *
   * This function obtains the stream's lock, which means any existing callback
   * (get or put) in progress will finish running before setting the new
   * callback.
   *
   * Setting a nullptr function turns off the callback.
   *
   * @param callback the new callback function to call when data is added to the
   *                 stream.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetGetCallback
   */
  void SetPutCallback(AudioStreamCB callback);

  /**
   * Set a callback that runs when data is added to an audio stream.
   *
   * This callback is called _after_ the data is added to the stream, giving the
   * callback the chance to obtain it immediately.
   *
   * The callback can (optionally) call AudioStreamRef.GetData() to obtain audio
   * from the stream during this call.
   *
   * The callback's `additional_amount` argument is how many bytes of
   * _converted_ data (in the stream's output format) was provided by the
   * caller, although this may underestimate a little for safety. This value
   * might be less than what is currently available in the stream, if data was
   * already there, and might be less than the caller provided if the stream
   * needs to keep a buffer to aid in resampling. Which means the callback may
   * be provided with zero bytes, and a different amount on each call.
   *
   * The callback may call AudioStreamRef.GetAvailable to see the total amount
   * currently available to read from the stream, instead of the total provided
   * by the current call.
   *
   * The callback is not required to obtain all data. It is allowed to read less
   * or none at all. Anything not read now simply remains in the stream for
   * later access.
   *
   * Clearing or flushing an audio stream does not call this callback.
   *
   * This function obtains the stream's lock, which means any existing callback
   * (get or put) in progress will finish running before setting the new
   * callback.
   *
   * Setting a nullptr function turns off the callback.
   *
   * @param callback the new callback function to call when data is added to the
   *                 stream.
   * @param userdata an opaque pointer provided to the callback for its own
   *                 personal use.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.SetGetCallback
   */
  void SetPutCallback(AudioStreamCallback callback, void* userdata)
  {
    CheckError(SDL_SetAudioStreamPutCallback(get(), callback, userdata));
  }

  /**
   * Bind a single audio stream to an audio device.
   *
   * This is a convenience function, equivalent to calling
   * `AudioDeviceBase.BindAudioStreams(devid, &stream, 1)`.
   *
   * @param devid the device to bind to.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDeviceRef.BindAudioStreams
   * @sa AudioDeviceRef.UnbindAudioStream
   * @sa AudioStreamRef.GetDevice
   * @sa AudioDeviceRef.BindAudioStream
   */
  void Bind(AudioDeviceRef devid) { devid.BindAudioStream(*this); }

  /**
   * Unbind a single audio stream from its audio device.
   *
   * This is a convenience function, equivalent to calling
   * `UnbindAudioStreams(&stream, 1)`.
   *
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDeviceRef.BindAudioStream
   */
  void Unbind() { SDL_UnbindAudioStream(get()); }

  /**
   * Query an audio stream for its currently-bound device.
   *
   * This reports the audio device that an audio stream is currently bound to.
   *
   * If not bound, or invalid, this returns zero, which is not a valid device
   * ID.
   *
   * @returns the bound audio device, or 0 if not bound or invalid.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDeviceRef.BindAudioStream
   * @sa AudioDeviceRef.BindAudioStreams
   */
  AudioDeviceRef GetDevice() const { return SDL_GetAudioStreamDevice(get()); }

  /**
   * Free an audio stream.
   *
   * This will release all allocated data, including any audio that is still
   * queued. You do not need to manually clear the stream first.
   *
   * If this stream was bound to an audio device, it is unbound during this
   * call. If this stream was created with AudioStream.OpenAudioDeviceStream,
   * the audio device that was opened alongside this stream's creation will be
   * closed, too.
   *
   * @param resource the audio stream to destroy.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.Create
   */
  static void reset(SDL_AudioStream* resource)
  {
    KeyValueWrapper<SDL_AudioStream*, AudioStreamCB, 0>::release(resource);
    KeyValueWrapper<SDL_AudioStream*, AudioStreamCB, 1>::release(resource);
    SDL_DestroyAudioStream(resource);
  }
};

/**
 * Handle to an owned audioStream
 *
 * @cat resource
 *
 * @sa AudioStreamRef
 */
struct AudioStream : ResourceUnique<AudioStreamRef>
{
  using ResourceUnique::ResourceUnique;

  /**
   * Create a new audio stream.
   *
   * @param src_spec the format details of the input audio.
   * @param dst_spec the format details of the output audio.
   * @returns a new audio stream on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.PutData
   * @sa AudioStreamRef.GetData
   * @sa AudioStreamRef.GetAvailable
   * @sa AudioStreamRef.Flush
   * @sa AudioStreamRef.Clear
   * @sa AudioStreamRef.SetFormat
   * @sa AudioStream.Destroy
   */
  static AudioStream Create(OptionalRef<const AudioSpec> src_spec,
                            OptionalRef<const AudioSpec> dst_spec)
  {
    return AudioStream(CheckError(SDL_CreateAudioStream(src_spec, dst_spec)));
  }

  /**
   * Convenience function for straightforward audio init for the common case.
   *
   * If all your app intends to do is provide a single source of PCM audio, this
   * function allows you to do all your audio setup in a single call.
   *
   * This is also intended to be a clean means to migrate apps from SDL2.
   *
   * This function will open an audio device, create a stream and bind it.
   * Unlike other methods of setup, the audio device will be closed when this
   * stream is destroyed, so the app can treat the returned AudioStreamRef as
   * the only object needed to manage audio playback.
   *
   * Also unlike other functions, the audio device begins paused. This is to map
   * more closely to SDL2-style behavior, since there is no extra step here to
   * bind a stream to begin audio flowing. The audio device should be resumed
   * with `AudioStreamRef.ResumeDevice(stream);`
   *
   * This function works with both playback and recording devices.
   *
   * The `spec` parameter represents the app's side of the audio stream. That
   * is, for recording audio, this will be the output format, and for playing
   * audio, this will be the input format. If spec is nullptr, the system will
   * choose the format, and the app can use AudioStreamRef.GetFormat() to obtain
   * this information later.
   *
   * If you don't care about opening a specific audio device, you can (and
   * probably _should_), use AUDIO_DEVICE_DEFAULT_PLAYBACK for playback and
   * AUDIO_DEVICE_DEFAULT_RECORDING for recording.
   *
   * One can optionally provide a callback function; if nullptr, the app is
   * expected to queue audio data for playback (or unqueue audio data if
   * capturing). Otherwise, the callback will begin to fire once the device is
   * unpaused.
   *
   * Destroying the returned stream with AudioStream.Destroy will also close
   * the audio device associated with this stream.
   *
   * @param devid an audio device to open, or AUDIO_DEVICE_DEFAULT_PLAYBACK
   *              or AUDIO_DEVICE_DEFAULT_RECORDING.
   * @param spec the audio stream's data format. Can be std::nullopt.
   * @param callback a callback where the app will provide new data for
   *                 playback, or receive new data for recording. Can be
   *                 nullptr, in which case the app will need to call
   *                 AudioStreamRef.PutData or AudioStreamRef.GetData as
   *                 necessary.
   * @returns an audio stream on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.GetDevice
   * @sa AudioStreamRef.ResumeDevice
   */
  static AudioStream OpenAudioDeviceStream(AudioDeviceRef devid,
                                           OptionalRef<const AudioSpec> spec,
                                           AudioStreamCB callback)
  {
    AudioStream stream = OpenAudioDeviceStream(devid, std::move(spec));
    if (devid.IsPlayback()) {
      stream->SetGetCallback(std::move(callback));
    } else {
      stream->SetPutCallback(std::move(callback));
    }
    return stream;
  }

  /**
   * Convenience function for straightforward audio init for the common case.
   *
   * If all your app intends to do is provide a single source of PCM audio, this
   * function allows you to do all your audio setup in a single call.
   *
   * This is also intended to be a clean means to migrate apps from SDL2.
   *
   * This function will open an audio device, create a stream and bind it.
   * Unlike other methods of setup, the audio device will be closed when this
   * stream is destroyed, so the app can treat the returned AudioStreamRef as
   * the only object needed to manage audio playback.
   *
   * Also unlike other functions, the audio device begins paused. This is to map
   * more closely to SDL2-style behavior, since there is no extra step here to
   * bind a stream to begin audio flowing. The audio device should be resumed
   * with `AudioStreamRef.ResumeDevice(stream);`
   *
   * This function works with both playback and recording devices.
   *
   * The `spec` parameter represents the app's side of the audio stream. That
   * is, for recording audio, this will be the output format, and for playing
   * audio, this will be the input format. If spec is nullptr, the system will
   * choose the format, and the app can use AudioStreamRef.GetFormat() to obtain
   * this information later.
   *
   * If you don't care about opening a specific audio device, you can (and
   * probably _should_), use AUDIO_DEVICE_DEFAULT_PLAYBACK for playback and
   * AUDIO_DEVICE_DEFAULT_RECORDING for recording.
   *
   * One can optionally provide a callback function; if nullptr, the app is
   * expected to queue audio data for playback (or unqueue audio data if
   * capturing). Otherwise, the callback will begin to fire once the device is
   * unpaused.
   *
   * Destroying the returned stream with AudioStream.Destroy will also close
   * the audio device associated with this stream.
   *
   * @param devid an audio device to open, or AUDIO_DEVICE_DEFAULT_PLAYBACK
   *              or AUDIO_DEVICE_DEFAULT_RECORDING.
   * @param spec the audio stream's data format. Can be std::nullopt.
   * @param callback a callback where the app will provide new data for
   *                 playback, or receive new data for recording. Can be
   *                 nullptr, in which case the app will need to call
   *                 AudioStreamRef.PutData or AudioStreamRef.GetData as
   *                 necessary.
   * @param userdata app-controlled pointer passed to callback. Can be nullptr.
   *                 Ignored if callback is nullptr.
   * @returns an audio stream on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.GetDevice
   * @sa AudioStreamRef.ResumeDevice
   */
  static AudioStream OpenAudioDeviceStream(
    AudioDeviceRef devid,
    OptionalRef<const AudioSpec> spec = std::nullopt,
    AudioStreamCallback callback = nullptr,
    void* userdata = nullptr)
  {
    return AudioStream(
      CheckError(SDL_OpenAudioDeviceStream(devid, spec, callback, userdata)));
  }

  /**
   * Free an audio stream.
   *
   * This will release all allocated data, including any audio that is still
   * queued. You do not need to manually clear the stream first.
   *
   * If this stream was bound to an audio device, it is unbound during this
   * call. If this stream was created with AudioStream.OpenAudioDeviceStream,
   * the audio device that was opened alongside this stream's creation will be
   * closed, too.
   *
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.Create
   */
  void Destroy() { reset(); }
};

/**
 * Unsafe Handle to audioStream
 *
 * Must call manually reset() to free.
 *
 * @cat resource
 *
 * @sa AudioStreamRef
 */
struct AudioStreamUnsafe : ResourceUnsafe<AudioStreamRef>
{
  using ResourceUnsafe::ResourceUnsafe;

  /**
   * Constructs AudioStreamUnsafe from AudioStream.
   */
  constexpr explicit AudioStreamUnsafe(AudioStream&& other)
    : AudioStreamUnsafe(other.release())
  {
  }
};

/**
 * Locks a AudioStream.
 */
struct AudioStreamLock : LockBase<AudioStreamRef>
{
  /**
   * Creates an empty lock
   */
  constexpr AudioStreamLock() = default;

  /**
   * Move ctor
   */
  constexpr AudioStreamLock(AudioStreamLock&& other)
    : LockBase(other.release())
  {
  }

  /**
   * Lock an audio stream for serialized access.
   *
   * Each AudioStreamRef has an internal mutex it uses to protect its data
   * structures from threading conflicts. This function allows an app to lock
   * that mutex, which could be useful if registering callbacks on this stream.
   *
   * One does not need to lock a stream to use in it most cases, as the stream
   * manages this lock internally. However, this lock is held during callbacks,
   * which may run from arbitrary threads at any time, so if an app needs to
   * protect shared data during those callbacks, locking the stream guarantees
   * that the callback is not running while the lock is held.
   *
   * As this is just a wrapper over MutexRef.Lock for an internal lock; it has
   * all the same attributes (recursive locks are allowed, etc).
   *
   * @param stream the audio stream to lock.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamLock.Unlock
   */
  AudioStreamLock(AudioStreamRef stream)
    : LockBase(stream.get())
  {
    CheckError(SDL_LockAudioStream(stream));
  }

  /**
   * Destructor
   */
  ~AudioStreamLock() { Unlock(); }

  /**
   * Unlock an audio stream for serialized access.
   *
   * This unlocks an audio stream after a call to AudioStreamRef.Lock.
   *
   * @throws Error on failure.
   *
   * @threadsafety You should only call this from the same thread that
   *               previously called AudioStreamRef.Lock.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStreamRef.Lock
   */
  void Unlock() { CheckError(SDL_UnlockAudioStream(release().get())); }

  /**
   * Same as Unlock(), just for uniformity.
   */
  void reset() { Unlock(); }
};

/**
 * Use this function to get the number of built-in audio drivers.
 *
 * This function returns a hardcoded number. This never returns a negative
 * value; if there are no drivers compiled into this build of SDL, this
 * function returns zero. The presence of a driver in this list does not mean
 * it will function, it just means SDL is capable of interacting with that
 * interface. For example, a build of SDL might have esound support, but if
 * there's no esound server available, SDL's esound driver would fail if used.
 *
 * By default, SDL tries all drivers, in its preferred order, until one is
 * found to be usable.
 *
 * @returns the number of built-in audio drivers.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAudioDriver
 */
inline int GetNumAudioDrivers() { return SDL_GetNumAudioDrivers(); }

/**
 * Use this function to get the name of a built in audio driver.
 *
 * The list of audio drivers is given in the order that they are normally
 * initialized by default; the drivers that seem more reasonable to choose
 * first (as far as the SDL developers believe) are earlier in the list.
 *
 * The names of drivers are all simple, low-ASCII identifiers, like "alsa",
 * "coreaudio" or "wasapi". These never have Unicode characters, and are not
 * meant to be proper names.
 *
 * @param index the index of the audio driver; the value ranges from 0 to
 *              GetNumAudioDrivers() - 1.
 * @returns the name of the audio driver at the requested index, or nullptr if
 * an invalid index was specified.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetNumAudioDrivers
 */
inline const char* GetAudioDriver(int index)
{
  return SDL_GetAudioDriver(index);
}

/**
 * Get the name of the current audio driver.
 *
 * The names of drivers are all simple, low-ASCII identifiers, like "alsa",
 * "coreaudio" or "wasapi". These never have Unicode characters, and are not
 * meant to be proper names.
 *
 * @returns the name of the current audio driver or nullptr if no driver has
 * been initialized.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetCurrentAudioDriver()
{
  return SDL_GetCurrentAudioDriver();
}

/**
 * Get a list of currently-connected audio playback devices.
 *
 * This returns of list of available devices that play sound, perhaps to
 * speakers or headphones ("playback" devices). If you want devices that
 * record audio, like a microphone ("recording" devices), use
 * GetAudioRecordingDevices() instead.
 *
 * This only returns a list of physical devices; it will not have any device
 * IDs returned by AudioDevice.Open().
 *
 * If this function returns nullptr, to signify an error, `*count` will be set
 * to zero.
 *
 * @returns a 0 terminated array of device instance IDs or nullptr on error;
 * call GetError() for more information. This should be freed with free() when
 * it is no longer needed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.Open
 * @sa GetAudioRecordingDevices
 */
inline OwnArray<AudioDeviceRef> GetAudioPlaybackDevices()
{
  int count;
  auto data = CheckError(SDL_GetAudioPlaybackDevices(&count));
  return OwnArray<AudioDeviceRef>{reinterpret_cast<AudioDeviceRef*>(data),
                                  size_t(count)};
}

/**
 * Get a list of currently-connected audio recording devices.
 *
 * This returns of list of available devices that record audio, like a
 * microphone ("recording" devices). If you want devices that play sound,
 * perhaps to speakers or headphones ("playback" devices), use
 * GetAudioPlaybackDevices() instead.
 *
 * This only returns a list of physical devices; it will not have any device
 * IDs returned by AudioDevice.Open().
 *
 * If this function returns nullptr, to signify an error, `*count` will be set
 * to zero.
 *
 * @returns a 0 terminated array of device instance IDs, or nullptr on failure;
 *          call GetError() for more information. This should be freed
 *          with free() when it is no longer needed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.Open
 * @sa GetAudioPlaybackDevices
 */
inline OwnArray<AudioDeviceRef> GetAudioRecordingDevices()
{
  int count;
  auto data = CheckError(SDL_GetAudioRecordingDevices(&count));
  return OwnArray<AudioDeviceRef>{reinterpret_cast<AudioDeviceRef*>(data),
                                  size_t(count)};
}

inline void AudioDeviceRef::BindAudioStreams(std::span<AudioStreamRef> streams)
{
  SDL_assert_paranoid(streams.size() < SDL_MAX_SINT32);
  CheckError(SDL_BindAudioStreams(
    get(),
    reinterpret_cast<SDL_AudioStream* const*>(streams.data()),
    streams.size()));
}

inline void AudioDeviceRef::BindAudioStream(AudioStreamRef stream)
{
  CheckError(SDL_BindAudioStream(get(), stream));
}

/**
 * Unbind a list of audio streams from their audio devices.
 *
 * The streams being unbound do not all have to be on the same device. All
 * streams on the same device will be unbound atomically (data will stop
 * flowing through all unbound streams on the same device at the same time).
 *
 * Unbinding a stream that isn't bound to a device is a legal no-op.
 *
 * @param streams an array of audio streams to unbind. Can be nullptr or contain
 *                nullptr.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDeviceRef.BindAudioStreams
 */
inline void UnbindAudioStreams(std::span<AudioStreamRef> streams)
{
  SDL_assert_paranoid(streams.size() < SDL_MAX_SINT32);
  SDL_UnbindAudioStreams(
    reinterpret_cast<SDL_AudioStream* const*>(streams.data()), streams.size());
}

inline AudioStreamLock AudioStreamRef::Lock() { return AudioStreamLock(*this); }

/**
 * Load the audio data of a WAVE file into memory.
 *
 * Loading a WAVE file requires `src`, `spec`, `audio_buf` and `audio_len` to
 * be valid pointers. The entire data portion of the file is then loaded into
 * memory and decoded if necessary.
 *
 * Supported formats are RIFF WAVE files with the formats PCM (8, 16, 24, and
 * 32 bits), IEEE Float (32 bits), Microsoft ADPCM and IMA ADPCM (4 bits), and
 * A-law and mu-law (8 bits). Other formats are currently unsupported and
 * cause an error.
 *
 * If this function succeeds, the return value is zero and the pointer to the
 * audio data allocated by the function is written to `audio_buf` and its
 * length in bytes to `audio_len`. The AudioSpec members `freq`,
 * `channels`, and `format` are set to the values of the audio data in the
 * buffer.
 *
 * It's necessary to use free() to free the audio data returned in
 * `audio_buf` when it is no longer used.
 *
 * Because of the underspecification of the .WAV format, there are many
 * problematic files in the wild that cause issues with strict decoders. To
 * provide compatibility with these files, this decoder is lenient in regards
 * to the truncation of the file, the fact chunk, and the size of the RIFF
 * chunk. The hints `SDL_HINT_WAVE_RIFF_CHUNK_SIZE`,
 * `SDL_HINT_WAVE_TRUNCATION`, and `SDL_HINT_WAVE_FACT_CHUNK` can be used to
 * tune the behavior of the loading process.
 *
 * Any file that is invalid (due to truncation, corruption, or wrong values in
 * the headers), too big, or unsupported causes an error. Additionally, any
 * critical I/O error from the data source will terminate the loading process
 * with an error. The function returns nullptr on error and in all cases (with
 * the exception of `src` being nullptr), an appropriate error message will be
 * set.
 *
 * It is required that the data source supports seeking.
 *
 * Example:
 *
 * ```cpp
 * LoadWAV(IOStream.FromFile("sample.wav", "rb"), true, &spec);
 * ```
 *
 * Note that the LoadWAV function does this same thing for you, but in a
 * less messy way:
 *
 * ```cpp
 * LoadWAV("sample.wav", spec);
 * ```
 *
 * @param src the data source for the WAVE data.
 * @param spec a pointer to an AudioSpec that will be set to the WAVE
 *             data's format details on successful return.
 * @returns the audio data on success or nullptr on failure.
 *
 * This function throws false if the .WAV file cannot be opened,
 * uses an unknown data format, or is corrupt,
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline OwnArray<Uint8> LoadWAV(IOStreamRef src, AudioSpec* spec)
{
  Uint8* buf;
  Uint32 len;
  if (!SDL_LoadWAV_IO(src.get(), false, spec, &buf, &len)) return {};
  return OwnArray<Uint8>{buf, size_t(len)};
}

/**
 * Loads a WAV from a file path.
 *
 * This is a convenience function that is effectively the same as:
 *
 * ```cpp
 * LoadWAV(IOStream.FromFile(path, "rb"), true, spec, audio_buf, audio_len);
 * ```
 *
 * @param path the file path of the WAV file to open.
 * @param spec a pointer to an AudioSpec that will be set to the WAVE
 *             data's format details on successful return.
 * @returns the audio data on success or nullptr on failure.
 *
 * This function throws false if the .WAV file cannot be opened,
 * uses an unknown data format, or is corrupt,
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa LoadWAV
 */
inline OwnArray<Uint8> LoadWAV(StringParam path, AudioSpec* spec)
{
  Uint8* buf;
  Uint32 len;
  if (!SDL_LoadWAV(path, spec, &buf, &len)) return {};
  return OwnArray<Uint8>{buf, size_t(len)};
}

/**
 * Mix audio data in a specified format.
 *
 * This takes an audio buffer `src` of `len` bytes of `format` data and mixes
 * it into `dst`, performing addition, volume adjustment, and overflow
 * clipping. The buffer pointed to by `dst` must also be `len` bytes of
 * `format` data.
 *
 * This is provided for convenience -- you can mix your own audio data.
 *
 * Do not use this function for mixing together more than two streams of
 * sample data. The output from repeated application of this function may be
 * distorted by clipping, because there is no accumulator with greater range
 * than the input (not to mention this being an inefficient way of doing it).
 *
 * It is a common misconception that this function is required to write audio
 * data to an output stream in an audio callback. While you can do that,
 * MixAudio() is really only needed when you're mixing a single audio
 * stream with a volume adjustment.
 *
 * @param dst the destination for the mixed audio.
 * @param src the source audio buffer to be mixed.
 * @param format the AudioFormat structure representing the desired audio
 *               format.
 * @param volume ranges from 0.0 - 1.0, and should be set to 1.0 for full
 *               audio volume.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void MixAudio(Uint8* dst,
                     SourceBytes src,
                     AudioFormat format,
                     float volume)
{
  SDL_assert_paranoid(src.size_bytes < SDL_MAX_SINT32);
  CheckError(SDL_MixAudio(
    dst, static_cast<const Uint8*>(src.data), format, src.size_bytes, volume));
}

/**
 * Mix audio data in a specified format.
 *
 * This takes an audio buffer `src` of `len` bytes of `format` data and mixes
 * it into `dst`, performing addition, volume adjustment, and overflow
 * clipping. The buffer pointed to by `dst` must also be `len` bytes of
 * `format` data.
 *
 * This is provided for convenience -- you can mix your own audio data.
 *
 * Do not use this function for mixing together more than two streams of
 * sample data. The output from repeated application of this function may be
 * distorted by clipping, because there is no accumulator with greater range
 * than the input (not to mention this being an inefficient way of doing it).
 *
 * It is a common misconception that this function is required to write audio
 * data to an output stream in an audio callback. While you can do that,
 * MixAudio() is really only needed when you're mixing a single audio
 * stream with a volume adjustment.
 *
 * @param dst the destination for the mixed audio.
 * @param src the source audio buffer to be mixed.
 * @param format the AudioFormat structure representing the desired audio
 *               format.
 * @param volume ranges from 0.0 - 1.0, and should be set to 1.0 for full
 *               audio volume.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void MixAudio(TargetBytes dst,
                     SourceBytes src,
                     AudioFormat format,
                     float volume)
{
  if (dst.size_bytes < src.size_bytes) src.size_bytes = dst.size_bytes;
  MixAudio(static_cast<Uint8*>(dst.data), src, format, volume);
}

/**
 * Convert some audio data of one format to another format.
 *
 * Please note that this function is for convenience, but should not be used
 * to resample audio in blocks, as it will introduce audio artifacts on the
 * boundaries. You should only use this function if you are converting audio
 * data in its entirety in one call. If you want to convert audio in smaller
 * chunks, use an AudioStreamRef, which is designed for this situation.
 *
 * Internally, this function creates and destroys an AudioStreamRef on each
 * use, so it's also less efficient than using one directly, if you need to
 * convert multiple times.
 *
 * @param src_spec the format details of the input audio.
 * @param src_data the audio data to be converted.
 * @param dst_spec the format details of the output audio.
 * @returns the converted audio data on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline OwnArray<Uint8> ConvertAudioSamples(const AudioSpec& src_spec,
                                           SourceBytes src_data,
                                           const AudioSpec& dst_spec)
{
  SDL_assert_paranoid(src_data.size_bytes < SDL_MAX_SINT32);
  Uint8* buf;
  int len;
  CheckError(SDL_ConvertAudioSamples(&src_spec,
                                     static_cast<const Uint8*>(src_data.data),
                                     src_data.size_bytes,
                                     &dst_spec,
                                     &buf,
                                     &len));
  return OwnArray<Uint8>{buf, size_t(len)};
}

#pragma region impl
/// @}

inline void AudioDeviceRef::SetPostmixCallback(AudioPostmixCB callback)
{
  using Wrapper = KeyValueCallbackWrapper<AudioDeviceRef, AudioPostmixCB>;

  auto cb = Wrapper::Wrap(get(), std::move(callback));
  if (!SDL_SetAudioPostmixCallback(
        get(),
        [](void* userdata, const AudioSpec* spec, float* buffer, int buflen) {
          Wrapper::Call(userdata, *spec, std::span{buffer, size_t(buflen)});
        },
        cb)) {
    Wrapper::release(get());
    throw Error{};
  }
}

inline void AudioStreamRef::SetGetCallback(AudioStreamCB callback)
{
  using Wrapper = KeyValueCallbackWrapper<SDL_AudioStream*, AudioStreamCB, 0>;
  if (!SDL_SetAudioStreamGetCallback(
        get(),
        [](void* userdata,
           SDL_AudioStream* stream,
           int additional_amount,
           int total_amount) {
          Wrapper::Call(userdata, stream, additional_amount, total_amount);
        },
        Wrapper::Wrap(get(), std::move(callback)))) {
    Wrapper::release(get());
    throw Error{};
  }
}

inline void AudioStreamRef::SetPutCallback(AudioStreamCB callback)
{
  using Wrapper = KeyValueCallbackWrapper<SDL_AudioStream*, AudioStreamCB, 1>;
  if (!SDL_SetAudioStreamPutCallback(
        get(),
        [](void* userdata,
           SDL_AudioStream* stream,
           int additional_amount,
           int total_amount) {
          Wrapper::Call(userdata, stream, additional_amount, total_amount);
        },
        Wrapper::Wrap(get(), std::move(callback)))) {
    Wrapper::release(get());
    throw Error{};
  }
}

#pragma endregion impl
} // namespace SDL

#endif /* SDL3PP_AUDIO_H_ */
