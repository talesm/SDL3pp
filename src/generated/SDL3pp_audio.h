#ifndef SDL3PP_AUDIO_H_
#define SDL3PP_AUDIO_H_

#include <SDL3/SDL_audio.h>
#include "SDL3pp_iostream.h"
#include "SDL3pp_properties.h"
#include "SDL3pp_stdinc.h"

namespace SDL {

/**
 * @defgroup CategoryAudio Category Audio
 *
 * Audio functionality for the SDL library.
 *
 * All audio in SDL3 revolves around AudioStream. Whether you want to play or
 * record audio, convert it, stream it, buffer it, or mix it, you're going to be
 * passing it through an audio stream.
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
 * Audio streams can also use an app-provided callback to supply data on-demand,
 * which maps pretty closely to the SDL2 audio model.
 *
 * SDL also provides a simple .WAV loader in LoadWAV (and LoadWAV if you aren't
 * reading from a file) as a basic means to load sound data into your program.
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
 * level once, and will manage all the logical devices on top of it internally.
 *
 * One other benefit of logical devices: if you don't open a specific physical
 * device, instead opting for the default, SDL can automatically migrate those
 * logical devices to different hardware as circumstances change: a user plugged
 * in headphones? The system default changed? SDL can transparently migrate the
 * logical devices to the correct physical device seamlessly and keep playing;
 * the app doesn't even have to know it happened if it doesn't want to.
 *
 * ## Simplified audio
 *
 * As a simplified model for when a single source of audio is all that's needed,
 * an app can use AudioStream.AudioStream, which is a single function to open an
 * audio device, create an audio stream, bind that stream to the newly-opened
 * device, and (optionally) provide a callback for obtaining audio data. When
 * using this function, the primary interface is the AudioStream and the device
 * handle is mostly hidden away; destroying a stream created through this
 * function will also close the device, stream bindings cannot be changed, etc.
 * One other quirk of this is that the device is started in a _paused_ state and
 * must be explicitly resumed; this is partially to offer a clean migration for
 * SDL2 apps and partially because the app might have to do more setup before
 * playback begins; in the non-simplified form, nothing will play until a stream
 * is bound to a device, so they start _unpaused_.
 *
 * ## Channel layouts
 *
 * Audio data passing through SDL is uncompressed PCM data, interleaved. One can
 * provide their own decompression through an MP3, etc, decoder, but SDL does
 * not provide this directly. Each interleaved channel of data is meant to be in
 * a specific order.
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
 * These are listed in the order they are laid out in memory, so "FL, FR" means
 * "the front left speaker is laid out in memory first, then the front right,
 * then it repeats for the next audio frame".
 *
 * - 1 channel (mono) layout: FRONT
 * - 2 channels (stereo) layout: FL, FR
 * - 3 channels (2.1) layout: FL, FR, LFE
 * - 4 channels (quad) layout: FL, FR, BL, BR
 * - 5 channels (4.1) layout: FL, FR, LFE, BL, BR
 * - 6 channels (5.1) layout: FL, FR, FC, LFE, BL, BR (last two can also be SL,
 *   SR)
 * - 7 channels (6.1) layout: FL, FR, FC, LFE, BC, SL, SR
 * - 8 channels (7.1) layout: FL, FR, FC, LFE, BL, BR, SL, SR
 *
 * This is the same order as DirectSound expects, but applied to all platforms;
 * SDL will swizzle the channels as necessary if a platform expects something
 * different.
 *
 * AudioStream can also be provided channel maps to change this ordering to
 * whatever is necessary, in other audio processing scenarios.
 *
 * @{
 */

/// Alias to raw representation for AudioFormat.
using AudioFormatRaw = SDL_AudioFormat;

// Forward decl
struct AudioFormat;

// Forward decl
struct AudioDevice;

/// Alias to raw representation for AudioDevice.
using AudioDeviceID = SDL_AudioDeviceID;

// Forward decl
struct AudioDeviceRef;

/// Safely wrap AudioDevice for non owning parameters
struct AudioDeviceParam
{
  AudioDeviceID value; ///< parameter's AudioDeviceID

  /// Constructs from AudioDeviceID
  constexpr AudioDeviceParam(AudioDeviceID value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr AudioDeviceParam(std::nullptr_t _ = nullptr)
    : value(0)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const AudioDeviceParam& other) const = default;

  /// Converts to underlying AudioDeviceID
  constexpr operator AudioDeviceID() const { return value; }
};

// Forward decl
struct AudioStream;

/// Alias to raw representation for AudioStream.
using AudioStreamRaw = SDL_AudioStream*;

// Forward decl
struct AudioStreamRef;

/// Safely wrap AudioStream for non owning parameters
struct AudioStreamParam
{
  AudioStreamRaw value; ///< parameter's AudioStreamRaw

  /// Constructs from AudioStreamRaw
  constexpr AudioStreamParam(AudioStreamRaw value)
    : value(value)
  {
  }

  /// Constructs null/invalid
  constexpr AudioStreamParam(std::nullptr_t _ = nullptr)
    : value(nullptr)
  {
  }

  /// Converts to bool
  constexpr explicit operator bool() const { return !!value; }

  /// Comparison
  constexpr auto operator<=>(const AudioStreamParam& other) const = default;

  /// Converts to underlying AudioStreamRaw
  constexpr operator AudioStreamRaw() const { return value; }
};

// Forward decl
struct AudioStreamLock;

/**
 * Mask of bits in an AudioFormat that contains the format bit size.
 *
 * Generally one should use AudioFormat.GetBitSize instead of this macro
 * directly.
 *
 * @since This constant is available since SDL 3.2.0.
 */
constexpr Uint32 AUDIO_MASK_BITSIZE = SDL_AUDIO_MASK_BITSIZE;

/**
 * Mask of bits in an AudioFormat that contain the floating point flag.
 *
 * Generally one should use AudioFormat.IsFloat instead of this macro directly.
 *
 * @since This constant is available since SDL 3.2.0.
 */
constexpr Uint32 AUDIO_MASK_FLOAT = SDL_AUDIO_MASK_FLOAT;

/**
 * Mask of bits in an AudioFormat that contain the bigendian flag.
 *
 * Generally one should use AudioFormat.IsBigEndian or
 * AudioFormat.IsLittleEndian instead of this macro directly.
 *
 * @since This constant is available since SDL 3.2.0.
 */
constexpr Uint32 AUDIO_MASK_BIG_ENDIAN = SDL_AUDIO_MASK_BIG_ENDIAN;

/**
 * Mask of bits in an AudioFormat that contain the signed data flag.
 *
 * Generally one should use AudioFormat.IsSigned instead of this macro directly.
 *
 * @since This constant is available since SDL 3.2.0.
 */
constexpr Uint32 AUDIO_MASK_SIGNED = SDL_AUDIO_MASK_SIGNED;

/**
 * Format specifier for audio data.
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa AudioFormat
 */
using AudioSpec = SDL_AudioSpec;

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
  AudioFormatRaw m_audioFormat;

public:
  /**
   * Wraps AudioFormat.
   *
   * @param audioFormat the value to be wrapped
   */
  constexpr AudioFormat(AudioFormatRaw audioFormat = {}) noexcept
    : m_audioFormat(audioFormat)
  {
  }

  /**
   * Define an AudioFormat value.
   *
   * SDL does not support custom audio formats, so this macro is not of much use
   * externally, but it can be illustrative as to what the various bits of an
   * AudioFormat mean.
   *
   * For example, AUDIO_S32LE looks like this:
   *
   * ```c
   * AudioFormat.AudioFormat(1, 0, 0, 32)
   * ```
   *
   * @param signed 1 for signed data, 0 for unsigned data.
   * @param bigendian 1 for bigendian data, 0 for littleendian data.
   * @param flt 1 for floating point data, 0 for integer data.
   * @param size number of bits per sample.
   * @post a format value in the style of AudioFormat.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  constexpr AudioFormat(bool sign, bool bigendian, bool flt, Uint16 size)
    : m_audioFormat(SDL_DEFINE_AUDIO_FORMAT(sign, bigendian, flt, size))
  {
  }

  /**
   * Unwraps to the underlying AudioFormat.
   *
   * @returns the underlying AudioFormatRaw.
   */
  constexpr operator AudioFormatRaw() const noexcept { return m_audioFormat; }

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
  constexpr Uint16 GetBitSize() const;

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
  constexpr Uint16 GetByteSize() const;

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
  constexpr bool IsFloat() const;

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
  constexpr bool IsBigEndian() const;

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
  constexpr bool IsLittleEndian() const;

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
  constexpr bool IsSigned() const;

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
  constexpr bool IsInt() const;

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
  constexpr bool IsUnsigned() const;

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
  const char* GetName() const;

  /**
   * Get the appropriate memset value for silencing an audio format.
   *
   * The value returned by this function can be used as the second argument to
   * memset (or memset) to set an audio buffer in a specific format to silence.
   *
   * @returns a byte value that can be passed to memset.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  int GetSilenceValue() const;
};

constexpr AudioFormat AUDIO_UNKNOWN =
  SDL_AUDIO_UNKNOWN; ///< Unspecified audio format

constexpr AudioFormat AUDIO_U8 = SDL_AUDIO_U8; ///< Unsigned 8-bit samples

constexpr AudioFormat AUDIO_S8 = SDL_AUDIO_S8; ///< Signed 8-bit samples

constexpr AudioFormat AUDIO_S16LE = SDL_AUDIO_S16LE; ///< Signed 16-bit samples

constexpr AudioFormat AUDIO_S16BE =
  SDL_AUDIO_S16BE; ///< As above, but big-endian byte order

constexpr AudioFormat AUDIO_S32LE = SDL_AUDIO_S32LE; ///< 32-bit integer samples

constexpr AudioFormat AUDIO_S32BE =
  SDL_AUDIO_S32BE; ///< As above, but big-endian byte order

constexpr AudioFormat AUDIO_F32LE =
  SDL_AUDIO_F32LE; ///< 32-bit floating point samples

constexpr AudioFormat AUDIO_F32BE =
  SDL_AUDIO_F32BE; ///< As above, but big-endian byte order

constexpr AudioFormat AUDIO_S16 = SDL_AUDIO_S16; ///< AUDIO_S16

constexpr AudioFormat AUDIO_S32 = SDL_AUDIO_S32; ///< AUDIO_S32

constexpr AudioFormat AUDIO_F32 = SDL_AUDIO_F32; ///< AUDIO_F32

/**
 * Define an AudioFormat value.
 *
 * SDL does not support custom audio formats, so this macro is not of much use
 * externally, but it can be illustrative as to what the various bits of an
 * AudioFormat mean.
 *
 * For example, AUDIO_S32LE looks like this:
 *
 * ```c
 * AudioFormat.AudioFormat(1, 0, 0, 32)
 * ```
 *
 * @param signed 1 for signed data, 0 for unsigned data.
 * @param bigendian 1 for bigendian data, 0 for littleendian data.
 * @param flt 1 for floating point data, 0 for integer data.
 * @param size number of bits per sample.
 * @returns a format value in the style of AudioFormat.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr AudioFormat DefineAudioFormat(bool sign,
                                        bool bigendian,
                                        bool flt,
                                        Uint16 size)
{
  return AudioFormat(sign, bigendian, flt, size);
}

/**
 * Retrieve the size, in bits, from an AudioFormat.
 *
 * For example, `AudioFormat.GetBitSize(AUDIO_S16)` returns 16.
 *
 * @param x an AudioFormat value.
 * @returns data size in bits.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr Uint16 AudioBitSize(AudioFormatRaw x) { return SDL_AUDIO_BITSIZE(x); }

constexpr Uint16 AudioFormat::GetBitSize() const
{
  return SDL::AudioBitSize(m_audioFormat);
}

/**
 * Retrieve the size, in bytes, from an AudioFormat.
 *
 * For example, `AudioFormat.GetByteSize(AUDIO_S16)` returns 2.
 *
 * @param x an AudioFormat value.
 * @returns data size in bytes.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr Uint16 AudioByteSize(AudioFormatRaw x)
{
  return SDL_AUDIO_BYTESIZE(x);
}

constexpr Uint16 AudioFormat::GetByteSize() const
{
  return SDL::AudioByteSize(m_audioFormat);
}

/**
 * Determine if an AudioFormat represents floating point data.
 *
 * For example, `AudioFormat.IsFloat(AUDIO_S16)` returns 0.
 *
 * @param x an AudioFormat value.
 * @returns non-zero if format is floating point, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsAudioFloat(AudioFormatRaw x) { return SDL_AUDIO_ISFLOAT(x); }

constexpr bool AudioFormat::IsFloat() const
{
  return SDL::IsAudioFloat(m_audioFormat);
}

/**
 * Determine if an AudioFormat represents bigendian data.
 *
 * For example, `AudioFormat.IsBigEndian(AUDIO_S16LE)` returns 0.
 *
 * @param x an AudioFormat value.
 * @returns non-zero if format is bigendian, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsAudioBigENDIAN(AudioFormatRaw x)
{
  return SDL_AUDIO_ISBIGENDIAN(x);
}

constexpr bool AudioFormat::IsBigEndian() const
{
  return SDL::IsAudioBigENDIAN(m_audioFormat);
}

/**
 * Determine if an AudioFormat represents littleendian data.
 *
 * For example, `AudioFormat.IsLittleEndian(AUDIO_S16BE)` returns 0.
 *
 * @param x an AudioFormat value.
 * @returns non-zero if format is littleendian, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsAudioLittleEndian(AudioFormatRaw x)
{
  return SDL_AUDIO_ISLITTLEENDIAN(x);
}

constexpr bool AudioFormat::IsLittleEndian() const
{
  return SDL::IsAudioLittleEndian(m_audioFormat);
}

/**
 * Determine if an AudioFormat represents signed data.
 *
 * For example, `AudioFormat.IsSigned(AUDIO_U8)` returns 0.
 *
 * @param x an AudioFormat value.
 * @returns non-zero if format is signed, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsAudioSigned(AudioFormatRaw x) { return SDL_AUDIO_ISSIGNED(x); }

constexpr bool AudioFormat::IsSigned() const
{
  return SDL::IsAudioSigned(m_audioFormat);
}

/**
 * Determine if an AudioFormat represents integer data.
 *
 * For example, `AudioFormat.IsInt(AUDIO_F32)` returns 0.
 *
 * @param x an AudioFormat value.
 * @returns non-zero if format is integer, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsAudioInt(AudioFormatRaw x) { return SDL_AUDIO_ISINT(x); }

constexpr bool AudioFormat::IsInt() const
{
  return SDL::IsAudioInt(m_audioFormat);
}

/**
 * Determine if an AudioFormat represents unsigned data.
 *
 * For example, `AudioFormat.IsUnsigned(AUDIO_S16)` returns 0.
 *
 * @param x an AudioFormat value.
 * @returns non-zero if format is unsigned, zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr bool IsAudioUnsigned(AudioFormatRaw x)
{
  return SDL_AUDIO_ISUNSIGNED(x);
}

constexpr bool AudioFormat::IsUnsigned() const
{
  return SDL::IsAudioUnsigned(m_audioFormat);
}

/**
 * A callback that fires when data is about to be fed to an audio device.
 *
 * This is useful for accessing the final mix, perhaps for writing a visualizer
 * or applying a final effect to the audio data before playback.
 *
 * This callback should run as quickly as possible and not block for any
 * significant time, as this callback delays submission of data to the audio
 * device, which can cause audio playback problems.
 *
 * The postmix callback _must_ be able to handle any audio data format specified
 * in `spec`, which can change between callbacks if the audio device changed.
 * However, this only covers frequency and channel count; data is always
 * provided here in AUDIO_F32 format.
 *
 * The postmix callback runs _after_ logical device gain and audiostream gain
 * have been applied, which is to say you can make the output data louder at
 * this point than the gain settings would suggest.
 *
 * @param userdata a pointer provided by the app through
 *                 AudioDevice.SetPostmixCallback, for its own use.
 * @param spec the current format of audio that is to be submitted to the audio
 *             device.
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
 * @sa AudioDevice.SetPostmixCallback
 */
using AudioPostmixCallback = void(SDLCALL*)(void* userdata,
                                            const AudioSpec* spec,
                                            float* buffer,
                                            int buflen);

/**
 * A callback that fires when data is about to be fed to an audio device.
 *
 * This is useful for accessing the final mix, perhaps for writing a visualizer
 * or applying a final effect to the audio data before playback.
 *
 * This callback should run as quickly as possible and not block for any
 * significant time, as this callback delays submission of data to the audio
 * device, which can cause audio playback problems.
 *
 * The postmix callback _must_ be able to handle any audio data format specified
 * in `spec`, which can change between callbacks if the audio device changed.
 * However, this only covers frequency and channel count; data is always
 * provided here in AUDIO_F32 format.
 *
 * The postmix callback runs _after_ logical device gain and audiostream gain
 * have been applied, which is to say you can make the output data louder at
 * this point than the gain settings would suggest.
 *
 * @param spec the current format of audio that is to be submitted to the audio
 *             device.
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
 * @sa AudioDevice.SetPostmixCallback
 *
 * @sa AudioPostmixCallback
 */
using AudioPostmixCB =
  MakeFrontCallback<void(const AudioSpec* spec, float* buffer, int buflen)>;

/**
 * A callback that fires when data passes through an AudioStream.
 *
 * Apps can (optionally) register a callback with an audio stream that is called
 * when data is added with AudioStream.PutData, or requested with
 * AudioStream.GetData.
 *
 * Two values are offered here: one is the amount of additional data needed to
 * satisfy the immediate request (which might be zero if the stream already has
 * enough data queued) and the other is the total amount being requested. In a
 * Get call triggering a Put callback, these values can be different. In a Put
 * call triggering a Get callback, these values are always the same.
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
 * @param userdata an opaque pointer provided by the app for their personal use.
 *
 * @threadsafety This callbacks may run from any thread, so if you need to
 *               protect shared data, you should use AudioStream.Lock to
 *               serialize access; this lock will be held before your callback
 *               is called, so your callback does not need to manage the lock
 *               explicitly.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AudioStream.SetGetCallback
 * @sa AudioStream.SetPutCallback
 */
using AudioStreamCallback = void(SDLCALL*)(void* userdata,
                                           AudioStreamRaw stream,
                                           int additional_amount,
                                           int total_amount);

/**
 * A callback that fires when data passes through an AudioStream.
 *
 * Apps can (optionally) register a callback with an audio stream that is called
 * when data is added with AudioStream.PutData, or requested with
 * AudioStream.GetData.
 *
 * Two values are offered here: one is the amount of additional data needed to
 * satisfy the immediate request (which might be zero if the stream already has
 * enough data queued) and the other is the total amount being requested. In a
 * Get call triggering a Put callback, these values can be different. In a Put
 * call triggering a Get callback, these values are always the same.
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
 *               protect shared data, you should use AudioStream.Lock to
 *               serialize access; this lock will be held before your callback
 *               is called, so your callback does not need to manage the lock
 *               explicitly.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @sa AudioStream.SetGetCallback
 * @sa AudioStream.SetPutCallback
 * @sa AudioStreamCallback
 */
using AudioStreamCB = MakeFrontCallback<
  void(AudioStreamRaw stream, int additional_amount, int total_amount)>;

/**
 * SDL Audio Device instance IDs.
 *
 * Zero is used to signify an invalid/null device.
 *
 * @since This datatype is available since SDL 3.2.0.
 *
 * @cat resource
 */
class AudioDevice
{
  AudioDeviceID m_resource = 0;

public:
  /// Default ctor
  constexpr AudioDevice(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from AudioDeviceParam.
   *
   * @param resource a AudioDeviceID to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit AudioDevice(const AudioDeviceID resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr AudioDevice(const AudioDevice& other) noexcept = default;

public:
  /// Move constructor
  constexpr AudioDevice(AudioDevice&& other) noexcept
    : AudioDevice(other.release())
  {
  }

  constexpr AudioDevice(const AudioDeviceRef& other) = delete;

  constexpr AudioDevice(AudioDeviceRef&& other) = delete;

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
   * fairly closely by using AudioStream.AudioStream instead of this function).
   *
   * If you don't care about opening a specific device, pass a `devid` of either
   * `AUDIO_DEVICE_DEFAULT_PLAYBACK` or `AUDIO_DEVICE_DEFAULT_RECORDING`. In
   * this case, SDL will try to pick the most reasonable default, and may also
   * switch between physical devices seamlessly later, if the most reasonable
   * default changes during the lifetime of this opened device (user changed the
   * default in the OS's system preferences, the default got unplugged so the
   * system jumped to a new default, the user plugged in headphones on a mobile
   * device, etc). Unless you have a good reason to choose a specific device,
   * this is probably what you want.
   *
   * You may request a specific format for the audio device, but there is no
   * promise the device will honor that request for several reasons. As such,
   * it's only meant to be a hint as to what data your app will provide. Audio
   * streams will accept data in whatever format you specify and manage
   * conversion for you as appropriate. AudioDevice.GetFormat can tell you the
   * preferred format for the device before opening and the actual format the
   * device is using after opening.
   *
   * It's legal to open the same device ID more than once; each successful open
   * will generate a new logical AudioDevice that is managed separately from
   * others on the same physical device. This allows libraries to open a device
   * separately from the main app and bind its own streams without conflicting.
   *
   * It is also legal to open a device ID returned by a previous call to this
   * function; doing so just creates another logical device on the same physical
   * device. This may be useful for making logical groupings of audio streams.
   *
   * This function returns the opened device ID on success. This is a new,
   * unique AudioDevice that represents a logical device.
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
   *              AUDIO_DEVICE_DEFAULT_RECORDING for the most reasonable default
   *              device.
   * @param spec the requested device configuration. Can be nullptr to use
   *             reasonable defaults.
   * @post the device ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDevice.Close
   * @sa AudioDevice.GetFormat
   */
  AudioDevice(AudioDeviceParam devid, OptionalRef<const AudioSpec> spec)
    : m_resource(CheckError(SDL_OpenAudioDevice(devid, spec)))
  {
  }

  /// Destructor
  ~AudioDevice() { SDL_CloseAudioDevice(m_resource); }

  /// Assignment operator.
  constexpr AudioDevice& operator=(AudioDevice&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr AudioDevice& operator=(const AudioDevice& other) noexcept = default;

public:
  /// Retrieves underlying AudioDeviceID.
  constexpr AudioDeviceID get() const noexcept { return m_resource; }

  /// Retrieves underlying AudioDeviceID and clear this.
  constexpr AudioDeviceID release() noexcept
  {
    auto r = m_resource;
    m_resource = 0;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const AudioDevice& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to AudioDeviceParam
  constexpr operator AudioDeviceParam() const noexcept { return {m_resource}; }

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
   * @sa AudioDevice.AudioDevice
   */
  void Close();

  constexpr auto operator<=>(AudioDeviceParam other) const = default;

  /**
   * Get the human-readable name of a specific audio device.
   *
   * **WARNING**: this function will work with AUDIO_DEVICE_DEFAULT_PLAYBACK and
   * AUDIO_DEVICE_DEFAULT_RECORDING, returning the current default physical
   * devices' names. However, as the default device may change at any time, it
   * is likely better to show a generic name to the user, like "System default
   * audio device" or perhaps "default [currently %s]". Do not store this name
   * to disk to reidentify the device in a later run of the program, as the
   * default might change in general, and the string will be the name of a
   * specific device and not the abstract system default.
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
  const char* GetName() const;

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
   * @param spec on return, will be filled with device details.
   * @param sample_frames pointer to store device buffer size, in sample frames.
   *                      Can be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  AudioSpec GetFormat(int* sample_frames = nullptr) const;

  /**
   * Get the current channel map of an audio device.
   *
   * Channel maps are optional; most things do not need them, instead passing
   * data in the [order that SDL expects](CategoryAudio#channel-layouts).
   *
   * Audio devices usually have no remapping applied. This is represented by
   * returning nullptr, and does not signify an error.
   *
   * @param count On output, set to number of channels in the map. Can be
   *              nullptr.
   * @returns an array of the current channel mapping, with as many elements as
   *          the current output spec's channels, or nullptr if default. This
   *          should be freed with free() when it is no longer needed.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.SetInputChannelMap
   */
  OwnArray<int> GetChannelMap() const;

  /**
   * Determine if an audio device is physical (instead of logical).
   *
   * An AudioDevice that represents physical hardware is a physical device;
   * there is one for each piece of hardware that SDL can see. Logical devices
   * are created by calling AudioDevice.AudioDevice or AudioStream.AudioStream,
   * and while each is associated with a physical device, there can be any
   * number of logical devices on one physical device.
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
  bool IsPhysical() const;

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
  bool IsPlayback() const;

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
   * created through AudioDevice.AudioDevice() can be.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDevice.Resume
   * @sa AudioDevice.Paused
   */
  void Pause();

  /**
   * Use this function to unpause audio playback on a specified device.
   *
   * This function unpauses audio processing for a given device that has
   * previously been paused with AudioDevice.Pause(). Once unpaused, any bound
   * audio streams will begin to progress again, and audio can be generated.
   *
   * Unlike in SDL2, audio devices start in an _unpaused_ state, since an app
   * has to bind a stream before any audio will flow. Unpausing an unpaused
   * device is a legal no-op.
   *
   * Physical devices can not be paused or unpaused, only logical devices
   * created through AudioDevice.AudioDevice() can be.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDevice.Paused
   * @sa AudioDevice.Pause
   */
  void Resume();

  /**
   * Use this function to query if an audio device is paused.
   *
   * Unlike in SDL2, audio devices start in an _unpaused_ state, since an app
   * has to bind a stream before any audio will flow.
   *
   * Physical devices can not be paused or unpaused, only logical devices
   * created through AudioDevice.AudioDevice() can be. Physical and invalid
   * device IDs will report themselves as unpaused here.
   *
   * @returns true if device is valid and paused, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDevice.Pause
   * @sa AudioDevice.Resume
   */
  bool Paused() const;

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
   * @returns the gain of the device or -1.0f on failure; call GetError() for
   *          more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDevice.SetGain
   */
  float GetGain() const;

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
   * @sa AudioDevice.GetGain
   */
  void SetGain(float gain);

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
   * stream's format at any time with AudioStream.SetFormat(). If the other end
   * of the stream's format has never been set (the audio stream was created
   * with a nullptr audio spec), this function will set it to match the device
   * end's format.
   *
   * @param streams an array of audio streams to bind.
   * @param num_streams number streams listed in the `streams` array.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDevice.BindAudioStreams
   * @sa AudioStream.Unbind
   * @sa AudioStream.GetDevice
   */
  void BindAudioStreams(std::span<AudioStreamRef> streams);

  /**
   * Bind a single audio stream to an audio device.
   *
   * This is a convenience function, equivalent to calling
   * `AudioDevice.BindAudioStreams(devid, &stream, 1)`.
   *
   * @param stream an audio stream to bind to a device.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDevice.BindAudioStreams
   * @sa AudioStream.Unbind
   * @sa AudioStream.GetDevice
   */
  void BindAudioStream(AudioStreamParam stream);

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
   * their preferred format through an AudioStream and let SDL handle the
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
  void SetPostmixCallback(AudioPostmixCallback callback, void* userdata);

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
   * their preferred format through an AudioStream and let SDL handle the
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
  void SetPostmixCallback(AudioPostmixCB callback);

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
   * stream is destroyed, so the app can treat the returned AudioStream as the
   * only object needed to manage audio playback.
   *
   * Also unlike other functions, the audio device begins paused. This is to map
   * more closely to SDL2-style behavior, since there is no extra step here to
   * bind a stream to begin audio flowing. The audio device should be resumed
   * with AudioStream.ResumeDevice().
   *
   * This function works with both playback and recording devices.
   *
   * The `spec` parameter represents the app's side of the audio stream. That
   * is, for recording audio, this will be the output format, and for playing
   * audio, this will be the input format. If spec is nullptr, the system will
   * choose the format, and the app can use AudioStream.GetFormat() to obtain
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
   * Destroying the returned stream with AudioStream.Destroy will also close the
   * audio device associated with this stream.
   *
   * @param spec the audio stream's data format. Can be nullptr.
   * @param callback a callback where the app will provide new data for
   *                 playback, or receive new data for recording. Can be
   *                 nullptr, in which case the app will need to call
   *                 AudioStream.PutData or AudioStream.GetData as necessary.
   * @param userdata app-controlled pointer passed to callback. Can be nullptr.
   *                 Ignored if callback is nullptr.
   * @returns an audio stream on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.GetDevice
   * @sa AudioStream.ResumeDevice
   */
  AudioStreamRef OpenStream(const AudioSpec& spec,
                            AudioStreamCallback callback,
                            void* userdata);

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
   * stream is destroyed, so the app can treat the returned AudioStream as the
   * only object needed to manage audio playback.
   *
   * Also unlike other functions, the audio device begins paused. This is to map
   * more closely to SDL2-style behavior, since there is no extra step here to
   * bind a stream to begin audio flowing. The audio device should be resumed
   * with AudioStream.ResumeDevice().
   *
   * This function works with both playback and recording devices.
   *
   * The `spec` parameter represents the app's side of the audio stream. That
   * is, for recording audio, this will be the output format, and for playing
   * audio, this will be the input format. If spec is nullptr, the system will
   * choose the format, and the app can use AudioStream.GetFormat() to obtain
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
   * Destroying the returned stream with AudioStream.Destroy will also close the
   * audio device associated with this stream.
   *
   * @param spec the audio stream's data format. Can be nullptr.
   * @param callback a callback where the app will provide new data for
   *                 playback, or receive new data for recording. Can be
   *                 nullptr, in which case the app will need to call
   *                 AudioStream.PutData or AudioStream.GetData as necessary.
   * @param userdata app-controlled pointer passed to callback. Can be nullptr.
   *                 Ignored if callback is nullptr.
   * @returns an audio stream on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.GetDevice
   * @sa AudioStream.ResumeDevice
   */
  AudioStream OpenStream(OptionalRef<const AudioSpec> spec,
                         AudioStreamCB callback);
};

/// Semi-safe reference for AudioDevice.
struct AudioDeviceRef : AudioDevice
{
  using AudioDevice::AudioDevice;

  /**
   * Constructs from AudioDeviceParam.
   *
   * @param resource a AudioDeviceID or AudioDevice.
   *
   * This does not takes ownership!
   */
  AudioDeviceRef(AudioDeviceParam resource) noexcept
    : AudioDevice(resource.value)
  {
  }

  /**
   * Constructs from AudioDeviceParam.
   *
   * @param resource a AudioDeviceID or AudioDevice.
   *
   * This does not takes ownership!
   */
  AudioDeviceRef(AudioDeviceID resource) noexcept
    : AudioDevice(resource)
  {
  }

  /// Copy constructor.
  constexpr AudioDeviceRef(const AudioDeviceRef& other) noexcept = default;

  /// Destructor
  ~AudioDeviceRef() { release(); }
};

/**
 * A value used to request a default playback audio device.
 *
 * Several functions that require an AudioDevice will accept this value to
 * signify the app just wants the system to choose a default device instead of
 * the app providing a specific one.
 *
 * @since This constant is available since SDL 3.2.0.
 */
constexpr AudioDeviceRaw AUDIO_DEVICE_DEFAULT_PLAYBACK =
  SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK;

/**
 * A value used to request a default recording audio device.
 *
 * Several functions that require an AudioDevice will accept this value to
 * signify the app just wants the system to choose a default device instead of
 * the app providing a specific one.
 *
 * @since This constant is available since SDL 3.2.0.
 */
constexpr AudioDeviceRaw AUDIO_DEVICE_DEFAULT_RECORDING =
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
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
constexpr int AudioFrameSize(const AudioSpec& x)
{
  return SDL_AUDIO_FRAMESIZE(&x);
}

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * A callback that fires for completed AudioStream.PutDataNoCopy() data.
 *
 * When using AudioStream.PutDataNoCopy() to provide data to an AudioStream,
 * it's not safe to dispose of the data until the stream has completely consumed
 * it. Often times it's difficult to know exactly when this has happened.
 *
 * This callback fires once when the stream no longer needs the buffer, allowing
 * the app to easily free or reuse it.
 *
 * @param userdata an opaque pointer provided by the app for their personal use.
 * @param buf the pointer provided to AudioStream.PutDataNoCopy().
 * @param buflen the size of buffer, in bytes, provided to
 *               AudioStream.PutDataNoCopy().
 *
 * @threadsafety This callbacks may run from any thread, so if you need to
 *               protect shared data, you should use AudioStream.Lock to
 *               serialize access; this lock will be held before your callback
 *               is called, so your callback does not need to manage the lock
 *               explicitly.
 *
 * @since This datatype is available since SDL 3.4.0.
 *
 * @sa AudioStream.SetGetCallback
 * @sa AudioStream.SetPutCallback
 */
using AudioStreamDataCompleteCallback = void(SDLCALL*)(void* userdata,
                                                       const void* buf,
                                                       int buflen);

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * A callback that fires for completed AudioStream.PutDataNoCopy() data.
 *
 * When using AudioStream.PutDataNoCopy() to provide data to an AudioStream,
 * it's not safe to dispose of the data until the stream has completely consumed
 * it. Often times it's difficult to know exactly when this has happened.
 *
 * This callback fires once when the stream no longer needs the buffer, allowing
 * the app to easily free or reuse it.
 *
 * @param buf the pointer provided to AudioStream.PutDataNoCopy().
 * @param buflen the size of buffer, in bytes, provided to
 *               AudioStream.PutDataNoCopy().
 *
 * @threadsafety This callbacks may run from any thread, so if you need to
 *               protect shared data, you should use AudioStream.Lock to
 *               serialize access; this lock will be held before your callback
 *               is called, so your callback does not need to manage the lock
 *               explicitly.
 *
 * @since This datatype is available since SDL 3.4.0.
 *
 * @sa AudioStream.SetGetCallback
 * @sa AudioStream.SetPutCallback
 * @sa AudioStreamDataCompleteCallback
 */
using AudioStreamDataCompleteCB =
  std::function<void(const void* buf, int buflen)>;

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * The opaque handle that represents an audio stream.
 *
 * AudioStream is an audio conversion interface.
 *
 * - It can handle resampling data in chunks without generating artifacts, when
 *   it doesn't have the complete buffer available.
 * - It can handle incoming data in any variable size.
 * - It can handle input/output format changes on the fly.
 * - It can remap audio channels between inputs and outputs.
 * - You push data as you have it, and pull it when you need it
 * - It can also function as a basic audio data queue even if you just have
 *   sound that needs to pass from one place to another.
 * - You can hook callbacks up to them when more data is added or requested, to
 *   manage data on-the-fly.
 *
 * Audio streams are the core of the SDL3 audio interface. You create one or
 * more of them, bind them to an opened audio device, and feed data to them (or
 * for recording, consume data from them).
 *
 * @since This struct is available since SDL 3.2.0.
 *
 * @sa AudioStream.AudioStream
 *
 * @cat resource
 */
class AudioStream
{
  AudioStreamRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr AudioStream(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from AudioStreamParam.
   *
   * @param resource a AudioStreamRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit AudioStream(const AudioStreamRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr AudioStream(const AudioStream& other) noexcept = default;

public:
  /// Move constructor
  constexpr AudioStream(AudioStream&& other) noexcept
    : AudioStream(other.release())
  {
  }

  constexpr AudioStream(const AudioStreamRef& other) = delete;

  constexpr AudioStream(AudioStreamRef&& other) = delete;

  /**
   * Create a new audio stream.
   *
   * @param src_spec the format details of the input audio.
   * @param dst_spec the format details of the output audio.
   * @post a new audio stream on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.PutData
   * @sa AudioStream.GetData
   * @sa AudioStream.GetAvailable
   * @sa AudioStream.Flush
   * @sa AudioStream.Clear
   * @sa AudioStream.SetFormat
   * @sa AudioStream.Destroy
   */
  AudioStream(OptionalRef<const AudioSpec> src_spec,
              OptionalRef<const AudioSpec> dst_spec)
    : m_resource(CheckError(SDL_CreateAudioStream(src_spec, dst_spec)))
  {
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
   * stream is destroyed, so the app can treat the returned AudioStream as the
   * only object needed to manage audio playback.
   *
   * Also unlike other functions, the audio device begins paused. This is to map
   * more closely to SDL2-style behavior, since there is no extra step here to
   * bind a stream to begin audio flowing. The audio device should be resumed
   * with AudioStream.ResumeDevice().
   *
   * This function works with both playback and recording devices.
   *
   * The `spec` parameter represents the app's side of the audio stream. That
   * is, for recording audio, this will be the output format, and for playing
   * audio, this will be the input format. If spec is nullptr, the system will
   * choose the format, and the app can use AudioStream.GetFormat() to obtain
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
   * Destroying the returned stream with AudioStream.Destroy will also close the
   * audio device associated with this stream.
   *
   * @param devid an audio device to open, or AUDIO_DEVICE_DEFAULT_PLAYBACK or
   *              AUDIO_DEVICE_DEFAULT_RECORDING.
   * @param spec the audio stream's data format. Can be nullptr.
   * @param callback a callback where the app will provide new data for
   *                 playback, or receive new data for recording. Can be
   *                 nullptr, in which case the app will need to call
   *                 AudioStream.PutData or AudioStream.GetData as necessary.
   * @param userdata app-controlled pointer passed to callback. Can be nullptr.
   *                 Ignored if callback is nullptr.
   * @post an audio stream on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.GetDevice
   * @sa AudioStream.ResumeDevice
   */
  AudioStream(AudioDeviceParam devid,
              OptionalRef<const AudioSpec> spec = std::nullopt,
              AudioStreamCallback callback = nullptr,
              void* userdata = nullptr)
    : m_resource(
        CheckError(SDL_OpenAudioDeviceStream(devid, spec, callback, userdata)))
  {
  }

  /// Default ctor
  AudioStream(AudioDeviceParam devid,
              OptionalRef<const AudioSpec> spec,
              AudioStreamCB callback);

  /// Destructor
  ~AudioStream() { SDL_DestroyAudioStream(m_resource); }

  /// Assignment operator.
  constexpr AudioStream& operator=(AudioStream&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr AudioStream& operator=(const AudioStream& other) noexcept = default;

public:
  /// Retrieves underlying AudioStreamRaw.
  constexpr AudioStreamRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying AudioStreamRaw and clear this.
  constexpr AudioStreamRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const AudioStream& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /// Converts to AudioStreamParam
  constexpr operator AudioStreamParam() const noexcept { return {m_resource}; }

  /**
   * Free an audio stream.
   *
   * This will release all allocated data, including any audio that is still
   * queued. You do not need to manually clear the stream first.
   *
   * If this stream was bound to an audio device, it is unbound during this
   * call. If this stream was created with AudioStream.AudioStream, the audio
   * device that was opened alongside this stream's creation will be closed,
   * too.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.AudioStream
   */
  void Destroy();

  /**
   * Get the properties associated with an audio stream.
   *
   * The application can hang any data it wants here, but the following
   * properties are understood by SDL:
   *
   * - `prop::AudioStream._AUTO_CLEANUP_BOOLEAN`: if true (the default), the
   *   stream be automatically cleaned up when the audio subsystem quits. If set
   *   to false, the streams will persist beyond that. This property is ignored
   *   for streams created through AudioStream.AudioStream(), and will always be
   *   cleaned up. Streams that are not cleaned up will still be unbound from
   *   devices when the audio subsystem quits. This property was added in SDL
   *   3.4.0.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   */
  PropertiesRef GetProperties() const;

  AudioSpec GetInputFormat() const { static_assert(false, "Not implemented"); }

  AudioSpec GetOutputFormat() const { static_assert(false, "Not implemented"); }

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
   * @sa AudioStream.SetFormat
   */
  void GetFormat(AudioSpec* src_spec, AudioSpec* dst_spec) const;

  void SetInputFormat(const AudioSpec& spec)
  {
    static_assert(false, "Not implemented");
  }

  void SetOutputFormat(const AudioSpec& spec)
  {
    static_assert(false, "Not implemented");
  }

  /**
   * Change the input and output formats of an audio stream.
   *
   * Future calls to and AudioStream.GetAvailable and AudioStream.GetData will
   * reflect the new format, and future calls to AudioStream.PutData must
   * provide data in the new input formats.
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
   * @param src_spec the new format of the audio input; if nullptr, it is not
   *                 changed.
   * @param dst_spec the new format of the audio output; if nullptr, it is not
   *                 changed.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.GetFormat
   * @sa AudioStream.SetFrequencyRatio
   */
  void SetFormat(OptionalRef<const AudioSpec> src_spec,
                 OptionalRef<const AudioSpec> dst_spec);

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
   * @sa AudioStream.SetFrequencyRatio
   */
  float GetFrequencyRatio() const;

  /**
   * Change the frequency ratio of an audio stream.
   *
   * The frequency ratio is used to adjust the rate at which input data is
   * consumed. Changing this effectively modifies the speed and pitch of the
   * audio. A value greater than 1.0f will play the audio faster, and at a
   * higher pitch. A value less than 1.0f will play the audio slower, and at a
   * lower pitch. 1.0f means play at normal speed.
   *
   * This is applied during AudioStream.GetData, and can be continuously changed
   * to create various effects.
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
   * @sa AudioStream.GetFrequencyRatio
   * @sa AudioStream.SetFormat
   */
  void SetFrequencyRatio(float ratio);

  /**
   * Get the gain of an audio stream.
   *
   * The gain of a stream is its volume; a larger gain means a louder output,
   * with a gain of zero being silence.
   *
   * Audio streams default to a gain of 1.0f (no change in output).
   *
   * @returns the gain of the stream or -1.0f on failure; call GetError() for
   *          more information.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.SetGain
   */
  float GetGain() const;

  /**
   * Change the gain of an audio stream.
   *
   * The gain of a stream is its volume; a larger gain means a louder output,
   * with a gain of zero being silence.
   *
   * Audio streams default to a gain of 1.0f (no change in output).
   *
   * This is applied during AudioStream.GetData, and can be continuously changed
   * to create various effects.
   *
   * @param gain the gain. 1.0f is no change, 0.0f is silence.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.GetGain
   */
  void SetGain(float gain);

  /**
   * Get the current input channel map of an audio stream.
   *
   * Channel maps are optional; most things do not need them, instead passing
   * data in the [order that SDL expects](CategoryAudio#channel-layouts).
   *
   * Audio streams default to no remapping applied. This is represented by
   * returning nullptr, and does not signify an error.
   *
   * @param count On output, set to number of channels in the map. Can be
   *              nullptr.
   * @returns an array of the current channel mapping, with as many elements as
   *          the current output spec's channels, or nullptr if default. This
   *          should be freed with free() when it is no longer needed.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.SetInputChannelMap
   */
  OwnArray<int> GetInputChannelMap() const;

  /**
   * Get the current output channel map of an audio stream.
   *
   * Channel maps are optional; most things do not need them, instead passing
   * data in the [order that SDL expects](CategoryAudio#channel-layouts).
   *
   * Audio streams default to no remapping applied. This is represented by
   * returning nullptr, and does not signify an error.
   *
   * @param count On output, set to number of channels in the map. Can be
   *              nullptr.
   * @returns an array of the current channel mapping, with as many elements as
   *          the current output spec's channels, or nullptr if default. This
   *          should be freed with free() when it is no longer needed.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.SetInputChannelMap
   */
  OwnArray<int> GetOutputChannelMap() const;

  /**
   * Set the current input channel map of an audio stream.
   *
   * Channel maps are optional; most things do not need them, instead passing
   * data in the [order that SDL expects](CategoryAudio#channel-layouts).
   *
   * The input channel map reorders data that is added to a stream via
   * AudioStream.PutData. Future calls to AudioStream.PutData must provide data
   * in the new channel order.
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
   * @param count The number of channels in the map.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running. Don't change the
   *               stream's format to have a different number of channels from a
   *               different thread at the same time, though!
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.SetInputChannelMap
   */
  void SetInputChannelMap(std::span<int> chmap);

  /**
   * Set the current output channel map of an audio stream.
   *
   * Channel maps are optional; most things do not need them, instead passing
   * data in the [order that SDL expects](CategoryAudio#channel-layouts).
   *
   * The output channel map reorders data that is leaving a stream via
   * AudioStream.GetData.
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
   * applied during AudioStream.GetData.
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
   * @param count The number of channels in the map.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, as it holds
   *               a stream-specific mutex while running. Don't change the
   *               stream's format to have a different number of channels from a
   *               a different thread at the same time, though!
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.SetInputChannelMap
   */
  void SetOutputChannelMap(std::span<int> chmap);

  /**
   * Add data to the stream.
   *
   * This data must match the format/channels/samplerate specified in the latest
   * call to AudioStream.SetFormat, or the format specified when creating the
   * stream if it hasn't been changed.
   *
   * Note that this call simply copies the unconverted data for later. This is
   * different than SDL2, where data was converted during the Put call and the
   * Get call would just dequeue the previously-converted data.
   *
   * @param buf a pointer to the audio data to add.
   * @param len the number of bytes to write to the stream.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, but if the
   *               stream has a callback set, the caller might need to manage
   *               extra locking.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.Clear
   * @sa AudioStream.Flush
   * @sa AudioStream.GetData
   * @sa AudioStream.GetQueued
   */
  void PutData(SourceBytes buf);

#if SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Add external data to an audio stream without copying it.
   *
   * Unlike AudioStream.PutData(), this function does not make a copy of the
   * provided data, instead storing the provided pointer. This means that the
   * put operation does not need to allocate and copy the data, but the original
   * data must remain available until the stream is done with it, either by
   * being read from the stream in its entirety, or a call to
   * AudioStream.Clear() or AudioStream.Destroy().
   *
   * The data must match the format/channels/samplerate specified in the latest
   * call to AudioStream.SetFormat, or the format specified when creating the
   * stream if it hasn't been changed.
   *
   * An optional callback may be provided, which is called when the stream no
   * longer needs the data. Once this callback fires, the stream will not access
   * the data again. This callback will fire for any reason the data is no
   * longer needed, including clearing or destroying the stream.
   *
   * Note that there is still an allocation to store tracking information, so
   * this function is more efficient for larger blocks of data. If you're
   * planning to put a few samples at a time, it will be more efficient to use
   * AudioStream.PutData(), which allocates and buffers in blocks.
   *
   * @param buf a pointer to the audio data to add.
   * @param len the number of bytes to add to the stream.
   * @param callback the callback function to call when the data is no longer
   *                 needed by the stream. May be nullptr.
   * @param userdata an opaque pointer provided to the callback for its own
   *                 personal use.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, but if the
   *               stream has a callback set, the caller might need to manage
   *               extra locking.
   *
   * @since This function is available since SDL 3.4.0.
   *
   * @sa AudioStream.Clear
   * @sa AudioStream.Flush
   * @sa AudioStream.GetData
   * @sa AudioStream.GetQueued
   */
  void PutDataNoCopy(SourceBytes buf,
                     AudioStreamDataCompleteCallback callback,
                     void* userdata);

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Add external data to an audio stream without copying it.
   *
   * Unlike AudioStream.PutData(), this function does not make a copy of the
   * provided data, instead storing the provided pointer. This means that the
   * put operation does not need to allocate and copy the data, but the original
   * data must remain available until the stream is done with it, either by
   * being read from the stream in its entirety, or a call to
   * AudioStream.Clear() or AudioStream.Destroy().
   *
   * The data must match the format/channels/samplerate specified in the latest
   * call to AudioStream.SetFormat, or the format specified when creating the
   * stream if it hasn't been changed.
   *
   * An optional callback may be provided, which is called when the stream no
   * longer needs the data. Once this callback fires, the stream will not access
   * the data again. This callback will fire for any reason the data is no
   * longer needed, including clearing or destroying the stream.
   *
   * Note that there is still an allocation to store tracking information, so
   * this function is more efficient for larger blocks of data. If you're
   * planning to put a few samples at a time, it will be more efficient to use
   * AudioStream.PutData(), which allocates and buffers in blocks.
   *
   * @param buf a pointer to the audio data to add.
   * @param len the number of bytes to add to the stream.
   * @param callback the callback function to call when the data is no longer
   *                 needed by the stream. May be nullptr.
   * @param userdata an opaque pointer provided to the callback for its own
   *                 personal use.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, but if the
   *               stream has a callback set, the caller might need to manage
   *               extra locking.
   *
   * @since This function is available since SDL 3.4.0.
   *
   * @sa AudioStream.Clear
   * @sa AudioStream.Flush
   * @sa AudioStream.GetData
   * @sa AudioStream.GetQueued
   */
  void PutDataNoCopy(SourceBytes buf, AudioStreamDataCompleteCB callback);

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Get converted/resampled data from the stream.
   *
   * The input/output data format/channels/samplerate is specified when creating
   * the stream, and can be changed after creation by calling
   * AudioStream.SetFormat.
   *
   * Note that any conversion and resampling necessary is done during this call,
   * and AudioStream.PutData simply queues unconverted data for later. This is
   * different than SDL2, where that work was done while inputting new data to
   * the stream and requesting the output just copied the converted data.
   *
   * @param buf a buffer to fill with audio data.
   * @param len the maximum number of bytes to fill.
   * @returns the number of bytes read from the stream or -1 on failure; call
   *          GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread, but if the
   *               stream has a callback set, the caller might need to manage
   *               extra locking.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.Clear
   * @sa AudioStream.GetAvailable
   * @sa AudioStream.PutData
   */
  int GetData(TargetBytes buf);

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
   * AudioStream.GetData before this function's return value is no longer
   * clamped.
   *
   * @returns the number of converted/resampled bytes available or -1 on
   *          failure; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.GetData
   * @sa AudioStream.PutData
   */
  int GetAvailable() const;

  /**
   * Get the number of bytes currently queued.
   *
   * This is the number of bytes put into a stream as input, not the number that
   * can be retrieved as output. Because of several details, it's not possible
   * to calculate one number directly from the other. If you need to know how
   * much usable data can be retrieved right now, you should use
   * AudioStream.GetAvailable() and not this function.
   *
   * Note that audio streams can change their input format at any time, even if
   * there is still data queued in a different format, so the returned byte
   * count will not necessarily match the number of _sample frames_ available.
   * Users of this API should be aware of format changes they make when feeding
   * a stream and plan accordingly.
   *
   * Queued data is not converted until it is consumed by AudioStream.GetData,
   * so this value should be representative of the exact data that was put into
   * the stream.
   *
   * If the stream has so much data that it would overflow an int, the return
   * value is clamped to a maximum value, but no queued data is lost; if there
   * are gigabytes of data queued, the app might need to read some of it with
   * AudioStream.GetData before this function's return value is no longer
   * clamped.
   *
   * @returns the number of bytes queued or -1 on failure; call GetError() for
   *          more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.PutData
   * @sa AudioStream.Clear
   */
  int GetQueued() const;

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
   * @sa AudioStream.PutData
   */
  void Flush();

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
   * @sa AudioStream.GetAvailable
   * @sa AudioStream.GetData
   * @sa AudioStream.GetQueued
   * @sa AudioStream.PutData
   */
  void Clear();

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
   * @sa AudioStream.ResumeDevice
   */
  void PauseDevice();

  /**
   * Use this function to unpause audio playback on the audio device associated
   * with an audio stream.
   *
   * This function unpauses audio processing for a given device that has
   * previously been paused. Once unpaused, any bound audio streams will begin
   * to progress again, and audio can be generated.
   *
   * AudioStream.AudioStream opens audio devices in a paused state, so this
   * function call is required for audio playback to begin on such devices.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.PauseDevice
   */
  void ResumeDevice();

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
   * @sa AudioStream.PauseDevice
   * @sa AudioStream.ResumeDevice
   */
  bool DevicePaused() const;

  /**
   * Lock an audio stream for serialized access.
   *
   * Each AudioStream has an internal mutex it uses to protect its data
   * structures from threading conflicts. This function allows an app to lock
   * that mutex, which could be useful if registering callbacks on this stream.
   *
   * One does not need to lock a stream to use in it most cases, as the stream
   * manages this lock internally. However, this lock is held during callbacks,
   * which may run from arbitrary threads at any time, so if an app needs to
   * protect shared data during those callbacks, locking the stream guarantees
   * that the callback is not running while the lock is held.
   *
   * As this is just a wrapper over Mutex.Lock for an internal lock; it has all
   * the same attributes (recursive locks are allowed, etc).
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.Unlock
   */
  void Lock();

  /**
   * Unlock an audio stream for serialized access.
   *
   * This unlocks an audio stream after a call to AudioStream.Lock.
   *
   * @throws Error on failure.
   *
   * @threadsafety You should only call this from the same thread that
   *               previously called AudioStream.Lock.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.Lock
   */
  void Unlock();

  /**
   * Set a callback that runs when data is requested from an audio stream.
   *
   * This callback is called _before_ data is obtained from the stream, giving
   * the callback the chance to add more on-demand.
   *
   * The callback can (optionally) call AudioStream.PutData() to add more audio
   * to the stream during this call; if needed, the request that triggered this
   * callback will obtain the new data immediately.
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
   * @sa AudioStream.SetPutCallback
   */
  void SetGetCallback(AudioStreamCallback callback, void* userdata);

  /**
   * Set a callback that runs when data is requested from an audio stream.
   *
   * This callback is called _before_ data is obtained from the stream, giving
   * the callback the chance to add more on-demand.
   *
   * The callback can (optionally) call AudioStream.PutData() to add more audio
   * to the stream during this call; if needed, the request that triggered this
   * callback will obtain the new data immediately.
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
   * @sa AudioStream.SetPutCallback
   */
  void SetGetCallback(AudioStreamCB callback);

  /**
   * Set a callback that runs when data is added to an audio stream.
   *
   * This callback is called _after_ the data is added to the stream, giving the
   * callback the chance to obtain it immediately.
   *
   * The callback can (optionally) call AudioStream.GetData() to obtain audio
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
   * The callback may call AudioStream.GetAvailable to see the total amount
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
   * @sa AudioStream.SetGetCallback
   */
  void SetPutCallback(AudioStreamCallback callback, void* userdata);

  /**
   * Set a callback that runs when data is added to an audio stream.
   *
   * This callback is called _after_ the data is added to the stream, giving the
   * callback the chance to obtain it immediately.
   *
   * The callback can (optionally) call AudioStream.GetData() to obtain audio
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
   * The callback may call AudioStream.GetAvailable to see the total amount
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
   * @sa AudioStream.SetGetCallback
   */
  void SetPutCallback(AudioStreamCB callback);

  /**
   * Unbind a single audio stream from its audio device.
   *
   * This is a convenience function, equivalent to calling
   * `UnbindAudioStreams(&stream, 1)`.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioDevice.BindAudioStream
   */
  void Unbind();

  /**
   * Query an audio stream for its currently-bound device.
   *
   * This reports the logical audio device that an audio stream is currently
   * bound to.
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
   * @sa AudioDevice.BindAudioStream
   * @sa AudioDevice.BindAudioStreams
   */
  AudioDeviceRef GetDevice() const;

#if SDL_VERSION_ATLEAST(3, 4, 0)

  /**
   * Add data to the stream with each channel in a separate array.
   *
   * This data must match the format/channels/samplerate specified in the latest
   * call to AudioStream.SetFormat, or the format specified when creating the
   * stream if it hasn't been changed.
   *
   * The data will be interleaved and queued. Note that AudioStream only
   * operates on interleaved data, so this is simply a convenience function for
   * easily queueing data from sources that provide separate arrays. There is no
   * equivalent function to retrieve planar data.
   *
   * The arrays in `channel_buffers` are ordered as they are to be interleaved;
   * the first array will be the first sample in the interleaved data. Any
   * individual array may be nullptr; in this case, silence will be interleaved
   * for that channel.
   *
   * `num_channels` specifies how many arrays are in `channel_buffers`. This can
   * be used as a safety to prevent overflow, in case the stream format has
   * changed elsewhere. If more channels are specified than the current input
   * spec, they are ignored. If less channels are specified, the missing arrays
   * are treated as if they are nullptr (silence is written to those channels).
   * If the count is -1, SDL will assume the array count matches the current
   * input spec.
   *
   * Note that `num_samples` is the number of _samples per array_. This can also
   * be thought of as the number of _sample frames_ to be queued. A value of 1
   * with stereo arrays will queue two samples to the stream. This is different
   * than AudioStream.PutData, which wants the size of a single array in bytes.
   *
   * @param channel_buffers a pointer to an array of arrays, one array per
   *                        channel.
   * @param num_channels the number of arrays in `channel_buffers` or -1.
   * @param num_samples the number of _samples_ per array to write to the
   *                    stream.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread, but if the
   *               stream has a callback set, the caller might need to manage
   *               extra locking.
   *
   * @since This function is available since SDL 3.4.0.
   *
   * @sa AudioStream.Clear
   * @sa AudioStream.Flush
   * @sa AudioStream.GetData
   * @sa AudioStream.GetQueued
   */
  void PutPlanarData(const void* const* channel_buffers,
                     int num_channels,
                     int num_samples);

#endif // SDL_VERSION_ATLEAST(3, 4, 0)
};

/// Semi-safe reference for AudioStream.
struct AudioStreamRef : AudioStream
{
  using AudioStream::AudioStream;

  /**
   * Constructs from AudioStreamParam.
   *
   * @param resource a AudioStreamRaw or AudioStream.
   *
   * This does not takes ownership!
   */
  AudioStreamRef(AudioStreamParam resource) noexcept
    : AudioStream(resource.value)
  {
  }

  /**
   * Constructs from AudioStreamParam.
   *
   * @param resource a AudioStreamRaw or AudioStream.
   *
   * This does not takes ownership!
   */
  AudioStreamRef(AudioStreamRaw resource) noexcept
    : AudioStream(resource)
  {
  }

  /// Copy constructor.
  constexpr AudioStreamRef(const AudioStreamRef& other) noexcept = default;

  /// Destructor
  ~AudioStreamRef() { release(); }
};

/**
 * Lock an audio stream for serialized access.
 *
 * Each AudioStream has an internal mutex it uses to protect its data structures
 * from threading conflicts. This function allows an app to lock that mutex,
 * which could be useful if registering callbacks on this stream.
 *
 * One does not need to lock a stream to use in it most cases, as the stream
 * manages this lock internally. However, this lock is held during callbacks,
 * which may run from arbitrary threads at any time, so if an app needs to
 * protect shared data during those callbacks, locking the stream guarantees
 * that the callback is not running while the lock is held.
 *
 * As this is just a wrapper over Mutex.Lock for an internal lock; it has all
 * the same attributes (recursive locks are allowed, etc).
 *
 * @param stream the audio stream to lock.
 * @returns true on success or false on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.Unlock
 */
class AudioStreamLock
{
  AudioStreamRef m_lock;

public:
  /**
   * Lock an audio stream for serialized access.
   *
   * Each AudioStream has an internal mutex it uses to protect its data
   * structures from threading conflicts. This function allows an app to lock
   * that mutex, which could be useful if registering callbacks on this stream.
   *
   * One does not need to lock a stream to use in it most cases, as the stream
   * manages this lock internally. However, this lock is held during callbacks,
   * which may run from arbitrary threads at any time, so if an app needs to
   * protect shared data during those callbacks, locking the stream guarantees
   * that the callback is not running while the lock is held.
   *
   * As this is just a wrapper over Mutex.Lock for an internal lock; it has all
   * the same attributes (recursive locks are allowed, etc).
   *
   * @param stream the audio stream to lock.
   * @post true on success or false on failure; call GetError() for more
   *       information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.Unlock
   */
  AudioStreamLock(AudioStreamRef resource);

  /**
   * Lock an audio stream for serialized access.
   *
   * Each AudioStream has an internal mutex it uses to protect its data
   * structures from threading conflicts. This function allows an app to lock
   * that mutex, which could be useful if registering callbacks on this stream.
   *
   * One does not need to lock a stream to use in it most cases, as the stream
   * manages this lock internally. However, this lock is held during callbacks,
   * which may run from arbitrary threads at any time, so if an app needs to
   * protect shared data during those callbacks, locking the stream guarantees
   * that the callback is not running while the lock is held.
   *
   * As this is just a wrapper over Mutex.Lock for an internal lock; it has all
   * the same attributes (recursive locks are allowed, etc).
   *
   * @param stream the audio stream to lock.
   * @post true on success or false on failure; call GetError() for more
   *       information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.Unlock
   */
  AudioStreamLock(const AudioStreamLock& other) = delete;

  /// Move constructor
  constexpr AudioStreamLock(AudioStreamLock&& other) noexcept
    : m_lock(other.m_lock)
  {
    other.m_lock = {};
  }

  /**
   * Unlock an audio stream for serialized access.
   *
   * This unlocks an audio stream after a call to AudioStream.Lock.
   *
   * @param stream the audio stream to unlock.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety You should only call this from the same thread that
   *               previously called AudioStream.Lock.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.Lock
   */
  ~AudioStreamLock() { reset(); }

  AudioStreamLock& operator=(const AudioStreamLock& other) = delete;

  /// Assignment operator
  AudioStreamLock& operator=(AudioStreamLock&& other)
  {
    std::swap(m_lock, other.m_lock);
    return *this;
  }

  /// True if not locked.
  constexpr operator bool() const { return bool(m_lock); }

  /**
   * Unlock an audio stream for serialized access.
   *
   * This unlocks an audio stream after a call to AudioStream.Lock.
   *
   * @param stream the audio stream to unlock.
   * @returns true on success or false on failure; call GetError() for more
   *          information.
   *
   * @threadsafety You should only call this from the same thread that
   *               previously called AudioStream.Lock.
   *
   * @since This function is available since SDL 3.2.0.
   *
   * @sa AudioStream.Lock
   */
  void reset();

  /// Releases the lock without unlocking.
  void release() { m_lock.release(); }
};

/**
 * Use this function to get the number of built-in audio drivers.
 *
 * This function returns a hardcoded number. This never returns a negative
 * value; if there are no drivers compiled into this build of SDL, this function
 * returns zero. The presence of a driver in this list does not mean it will
 * function, it just means SDL is capable of interacting with that interface.
 * For example, a build of SDL might have esound support, but if there's no
 * esound server available, SDL's esound driver would fail if used.
 *
 * By default, SDL tries all drivers, in its preferred order, until one is found
 * to be usable.
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
 * initialized by default; the drivers that seem more reasonable to choose first
 * (as far as the SDL developers believe) are earlier in the list.
 *
 * The names of drivers are all simple, low-ASCII identifiers, like "alsa",
 * "coreaudio" or "wasapi". These never have Unicode characters, and are not
 * meant to be proper names.
 *
 * @param index the index of the audio driver; the value ranges from 0 to
 *              GetNumAudioDrivers() - 1.
 * @returns the name of the audio driver at the requested index, or nullptr if
 *          an invalid index was specified.
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
 *          been initialized.
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
 * speakers or headphones ("playback" devices). If you want devices that record
 * audio, like a microphone ("recording" devices), use
 * GetAudioRecordingDevices() instead.
 *
 * This only returns a list of physical devices; it will not have any device IDs
 * returned by AudioDevice.AudioDevice().
 *
 * If this function returns nullptr, to signify an error, `*count` will be set
 * to zero.
 *
 * @param count a pointer filled in with the number of devices returned, may be
 *              nullptr.
 * @returns a 0 terminated array of device instance IDs or nullptr on error;
 *          call GetError() for more information. This should be freed with
 *          free() when it is no longer needed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.AudioDevice
 * @sa GetAudioRecordingDevices
 */
inline OwnArray<AudioDeviceRef> GetAudioPlaybackDevices()
{
  return SDL_GetAudioPlaybackDevices();
}

/**
 * Get a list of currently-connected audio recording devices.
 *
 * This returns of list of available devices that record audio, like a
 * microphone ("recording" devices). If you want devices that play sound,
 * perhaps to speakers or headphones ("playback" devices), use
 * GetAudioPlaybackDevices() instead.
 *
 * This only returns a list of physical devices; it will not have any device IDs
 * returned by AudioDevice.AudioDevice().
 *
 * If this function returns nullptr, to signify an error, `*count` will be set
 * to zero.
 *
 * @param count a pointer filled in with the number of devices returned, may be
 *              nullptr.
 * @returns a 0 terminated array of device instance IDs, or nullptr on failure;
 *          call GetError() for more information. This should be freed with
 *          free() when it is no longer needed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.AudioDevice
 * @sa GetAudioPlaybackDevices
 */
inline OwnArray<AudioDeviceRef> GetAudioRecordingDevices()
{
  return SDL_GetAudioRecordingDevices();
}

/**
 * Get the human-readable name of a specific audio device.
 *
 * **WARNING**: this function will work with AUDIO_DEVICE_DEFAULT_PLAYBACK and
 * AUDIO_DEVICE_DEFAULT_RECORDING, returning the current default physical
 * devices' names. However, as the default device may change at any time, it is
 * likely better to show a generic name to the user, like "System default audio
 * device" or perhaps "default [currently %s]". Do not store this name to disk
 * to reidentify the device in a later run of the program, as the default might
 * change in general, and the string will be the name of a specific device and
 * not the abstract system default.
 *
 * @param devid the instance ID of the device to query.
 * @returns the name of the audio device, or nullptr on failure; call GetError()
 *          for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa GetAudioPlaybackDevices
 * @sa GetAudioRecordingDevices
 */
inline const char* GetAudioDeviceName(AudioDeviceParam devid)
{
  return SDL_GetAudioDeviceName(devid);
}

inline const char* AudioDevice::GetName() const
{
  return SDL::GetAudioDeviceName(m_resource);
}

/**
 * Get the current audio format of a specific audio device.
 *
 * For an opened device, this will report the format the device is currently
 * using. If the device isn't yet opened, this will report the device's
 * preferred format (or a reasonable default if this can't be determined).
 *
 * You may also specify AUDIO_DEVICE_DEFAULT_PLAYBACK or
 * AUDIO_DEVICE_DEFAULT_RECORDING here, which is useful for getting a reasonable
 * recommendation before opening the system-recommended default device.
 *
 * You can also use this to request the current device buffer size. This is
 * specified in sample frames and represents the amount of data SDL will feed to
 * the physical hardware in each chunk. This can be converted to milliseconds of
 * audio with the following equation:
 *
 * `ms = (int) ((((Sint64) frames) * 1000) / spec.freq);`
 *
 * Buffer size is only important if you need low-level control over the audio
 * playback timing. Most apps do not need this.
 *
 * @param devid the instance ID of the device to query.
 * @param spec on return, will be filled with device details.
 * @param sample_frames pointer to store device buffer size, in sample frames.
 *                      Can be nullptr.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline AudioSpec GetAudioDeviceFormat(AudioDeviceParam devid,
                                      int* sample_frames = nullptr)
{
  return CheckError(SDL_GetAudioDeviceFormat(devid, sample_frames));
}

inline AudioSpec AudioDevice::GetFormat(int* sample_frames) const
{
  return SDL::GetAudioDeviceFormat(m_resource, sample_frames);
}

/**
 * Get the current channel map of an audio device.
 *
 * Channel maps are optional; most things do not need them, instead passing data
 * in the [order that SDL expects](CategoryAudio#channel-layouts).
 *
 * Audio devices usually have no remapping applied. This is represented by
 * returning nullptr, and does not signify an error.
 *
 * @param devid the instance ID of the device to query.
 * @param count On output, set to number of channels in the map. Can be nullptr.
 * @returns an array of the current channel mapping, with as many elements as
 *          the current output spec's channels, or nullptr if default. This
 *          should be freed with free() when it is no longer needed.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.SetInputChannelMap
 */
inline OwnArray<int> GetAudioDeviceChannelMap(AudioDeviceParam devid)
{
  return SDL_GetAudioDeviceChannelMap(devid);
}

inline OwnArray<int> AudioDevice::GetChannelMap() const
{
  return SDL::GetAudioDeviceChannelMap(m_resource);
}

/**
 * Open a specific audio device.
 *
 * You can open both playback and recording devices through this function.
 * Playback devices will take data from bound audio streams, mix it, and send it
 * to the hardware. Recording devices will feed any bound audio streams with a
 * copy of any incoming data.
 *
 * An opened audio device starts out with no audio streams bound. To start audio
 * playing, bind a stream and supply audio data to it. Unlike SDL2, there is no
 * audio callback; you only bind audio streams and make sure they have data
 * flowing into them (however, you can simulate SDL2's semantics fairly closely
 * by using AudioStream.AudioStream instead of this function).
 *
 * If you don't care about opening a specific device, pass a `devid` of either
 * `AUDIO_DEVICE_DEFAULT_PLAYBACK` or `AUDIO_DEVICE_DEFAULT_RECORDING`. In this
 * case, SDL will try to pick the most reasonable default, and may also switch
 * between physical devices seamlessly later, if the most reasonable default
 * changes during the lifetime of this opened device (user changed the default
 * in the OS's system preferences, the default got unplugged so the system
 * jumped to a new default, the user plugged in headphones on a mobile device,
 * etc). Unless you have a good reason to choose a specific device, this is
 * probably what you want.
 *
 * You may request a specific format for the audio device, but there is no
 * promise the device will honor that request for several reasons. As such, it's
 * only meant to be a hint as to what data your app will provide. Audio streams
 * will accept data in whatever format you specify and manage conversion for you
 * as appropriate. AudioDevice.GetFormat can tell you the preferred format for
 * the device before opening and the actual format the device is using after
 * opening.
 *
 * It's legal to open the same device ID more than once; each successful open
 * will generate a new logical AudioDevice that is managed separately from
 * others on the same physical device. This allows libraries to open a device
 * separately from the main app and bind its own streams without conflicting.
 *
 * It is also legal to open a device ID returned by a previous call to this
 * function; doing so just creates another logical device on the same physical
 * device. This may be useful for making logical groupings of audio streams.
 *
 * This function returns the opened device ID on success. This is a new, unique
 * AudioDevice that represents a logical device.
 *
 * Some backends might offer arbitrary devices (for example, a networked audio
 * protocol that can connect to an arbitrary server). For these, as a change
 * from SDL2, you should open a default device ID and use an SDL hint to specify
 * the target if you care, or otherwise let the backend figure out a reasonable
 * default. Most backends don't offer anything like this, and often this would
 * be an end user setting an environment variable for their custom need, and not
 * something an application should specifically manage.
 *
 * When done with an audio device, possibly at the end of the app's life, one
 * should call AudioDevice.Close() on the returned device id.
 *
 * @param devid the device instance id to open, or AUDIO_DEVICE_DEFAULT_PLAYBACK
 *              or AUDIO_DEVICE_DEFAULT_RECORDING for the most reasonable
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
 * @sa AudioDevice.GetFormat
 */
inline AudioDevice OpenAudioDevice(AudioDeviceParam devid,
                                   const AudioSpec& spec)
{
  return AudioDevice(devid, spec);
}

/**
 * Determine if an audio device is physical (instead of logical).
 *
 * An AudioDevice that represents physical hardware is a physical device; there
 * is one for each piece of hardware that SDL can see. Logical devices are
 * created by calling AudioDevice.AudioDevice or AudioStream.AudioStream, and
 * while each is associated with a physical device, there can be any number of
 * logical devices on one physical device.
 *
 * For the most part, logical and physical IDs are interchangeable--if you try
 * to open a logical device, SDL understands to assign that effort to the
 * underlying physical device, etc. However, it might be useful to know if an
 * arbitrary device ID is physical or logical. This function reports which.
 *
 * This function may return either true or false for invalid device IDs.
 *
 * @param devid the device ID to query.
 * @returns true if devid is a physical device, false if it is logical.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool IsAudioDevicePhysical(AudioDeviceParam devid)
{
  return SDL_IsAudioDevicePhysical(devid);
}

inline bool AudioDevice::IsPhysical() const
{
  return SDL::IsAudioDevicePhysical(m_resource);
}

/**
 * Determine if an audio device is a playback device (instead of recording).
 *
 * This function may return either true or false for invalid device IDs.
 *
 * @param devid the device ID to query.
 * @returns true if devid is a playback device, false if it is recording.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline bool IsAudioDevicePlayback(AudioDeviceParam devid)
{
  return SDL_IsAudioDevicePlayback(devid);
}

inline bool AudioDevice::IsPlayback() const
{
  return SDL::IsAudioDevicePlayback(m_resource);
}

/**
 * Use this function to pause audio playback on a specified device.
 *
 * This function pauses audio processing for a given device. Any bound audio
 * streams will not progress, and no audio will be generated. Pausing one device
 * does not prevent other unpaused devices from running.
 *
 * Unlike in SDL2, audio devices start in an _unpaused_ state, since an app has
 * to bind a stream before any audio will flow. Pausing a paused device is a
 * legal no-op.
 *
 * Pausing a device can be useful to halt all audio without unbinding all the
 * audio streams. This might be useful while a game is paused, or a level is
 * loading, etc.
 *
 * Physical devices can not be paused or unpaused, only logical devices created
 * through AudioDevice.AudioDevice() can be.
 *
 * @param devid a device opened by AudioDevice.AudioDevice().
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.Resume
 * @sa AudioDevice.Paused
 */
inline void PauseAudioDevice(AudioDeviceParam devid)
{
  CheckError(SDL_PauseAudioDevice(devid));
}

inline void AudioDevice::Pause() { SDL::PauseAudioDevice(m_resource); }

/**
 * Use this function to unpause audio playback on a specified device.
 *
 * This function unpauses audio processing for a given device that has
 * previously been paused with AudioDevice.Pause(). Once unpaused, any bound
 * audio streams will begin to progress again, and audio can be generated.
 *
 * Unlike in SDL2, audio devices start in an _unpaused_ state, since an app has
 * to bind a stream before any audio will flow. Unpausing an unpaused device is
 * a legal no-op.
 *
 * Physical devices can not be paused or unpaused, only logical devices created
 * through AudioDevice.AudioDevice() can be.
 *
 * @param devid a device opened by AudioDevice.AudioDevice().
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.Paused
 * @sa AudioDevice.Pause
 */
inline void ResumeAudioDevice(AudioDeviceParam devid)
{
  CheckError(SDL_ResumeAudioDevice(devid));
}

inline void AudioDevice::Resume() { SDL::ResumeAudioDevice(m_resource); }

/**
 * Use this function to query if an audio device is paused.
 *
 * Unlike in SDL2, audio devices start in an _unpaused_ state, since an app has
 * to bind a stream before any audio will flow.
 *
 * Physical devices can not be paused or unpaused, only logical devices created
 * through AudioDevice.AudioDevice() can be. Physical and invalid device IDs
 * will report themselves as unpaused here.
 *
 * @param devid a device opened by AudioDevice.AudioDevice().
 * @returns true if device is valid and paused, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.Pause
 * @sa AudioDevice.Resume
 */
inline bool AudioDevicePaused(AudioDeviceParam devid)
{
  return SDL_AudioDevicePaused(devid);
}

inline bool AudioDevice::Paused() const
{
  return SDL::AudioDevicePaused(m_resource);
}

/**
 * Get the gain of an audio device.
 *
 * The gain of a device is its volume; a larger gain means a louder output, with
 * a gain of zero being silence.
 *
 * Audio devices default to a gain of 1.0f (no change in output).
 *
 * Physical devices may not have their gain changed, only logical devices, and
 * this function will always return -1.0f when used on physical devices.
 *
 * @param devid the audio device to query.
 * @returns the gain of the device or -1.0f on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.SetGain
 */
inline float GetAudioDeviceGain(AudioDeviceParam devid)
{
  return SDL_GetAudioDeviceGain(devid);
}

inline float AudioDevice::GetGain() const
{
  return SDL::GetAudioDeviceGain(m_resource);
}

/**
 * Change the gain of an audio device.
 *
 * The gain of a device is its volume; a larger gain means a louder output, with
 * a gain of zero being silence.
 *
 * Audio devices default to a gain of 1.0f (no change in output).
 *
 * Physical devices may not have their gain changed, only logical devices, and
 * this function will always return false when used on physical devices. While
 * it might seem attractive to adjust several logical devices at once in this
 * way, it would allow an app or library to interfere with another portion of
 * the program's otherwise-isolated devices.
 *
 * This is applied, along with any per-audiostream gain, during playback to the
 * hardware, and can be continuously changed to create various effects. On
 * recording devices, this will adjust the gain before passing the data into an
 * audiostream; that recording audiostream can then adjust its gain further when
 * outputting the data elsewhere, if it likes, but that second gain is not
 * applied until the data leaves the audiostream again.
 *
 * @param devid the audio device on which to change gain.
 * @param gain the gain. 1.0f is no change, 0.0f is silence.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, as it holds a
 *               stream-specific mutex while running.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.GetGain
 */
inline void SetAudioDeviceGain(AudioDeviceParam devid, float gain)
{
  CheckError(SDL_SetAudioDeviceGain(devid, gain));
}

inline void AudioDevice::SetGain(float gain)
{
  SDL::SetAudioDeviceGain(m_resource, gain);
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
 * @param devid an audio device id previously returned by
 *              AudioDevice.AudioDevice().
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.AudioDevice
 */
inline void CloseAudioDevice(AudioDeviceID devid)
{
  SDL_CloseAudioDevice(devid);
}

inline void AudioDevice::Close() { CloseAudioDevice(release()); }

/**
 * Bind a list of audio streams to an audio device.
 *
 * Audio data will flow through any bound streams. For a playback device, data
 * for all bound streams will be mixed together and fed to the device. For a
 * recording device, a copy of recorded data will be provided to each bound
 * stream.
 *
 * Audio streams can only be bound to an open device. This operation is
 * atomic--all streams bound in the same call will start processing at the same
 * time, so they can stay in sync. Also: either all streams will be bound or
 * none of them will be.
 *
 * It is an error to bind an already-bound stream; it must be explicitly unbound
 * first.
 *
 * Binding a stream to a device will set its output format for playback devices,
 * and its input format for recording devices, so they match the device's
 * settings. The caller is welcome to change the other end of the stream's
 * format at any time with AudioStream.SetFormat(). If the other end of the
 * stream's format has never been set (the audio stream was created with a
 * nullptr audio spec), this function will set it to match the device end's
 * format.
 *
 * @param devid an audio device to bind a stream to.
 * @param streams an array of audio streams to bind.
 * @param num_streams number streams listed in the `streams` array.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.BindAudioStreams
 * @sa AudioStream.Unbind
 * @sa AudioStream.GetDevice
 */
inline void BindAudioStreams(AudioDeviceParam devid,
                             std::span<AudioStreamRef> streams)
{
  CheckError(SDL_BindAudioStreams(devid, streams));
}

inline void AudioDevice::BindAudioStreams(std::span<AudioStreamRef> streams)
{
  SDL::BindAudioStreams(m_resource, streams);
}

/**
 * Bind a single audio stream to an audio device.
 *
 * This is a convenience function, equivalent to calling
 * `AudioDevice.BindAudioStreams(devid, &stream, 1)`.
 *
 * @param devid an audio device to bind a stream to.
 * @param stream an audio stream to bind to a device.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.BindAudioStreams
 * @sa AudioStream.Unbind
 * @sa AudioStream.GetDevice
 */
inline void BindAudioStream(AudioDeviceParam devid, AudioStreamParam stream)
{
  CheckError(SDL_BindAudioStream(devid, stream));
}

inline void AudioDevice::BindAudioStream(AudioStreamParam stream)
{
  SDL::BindAudioStream(m_resource, stream);
}

/**
 * Unbind a list of audio streams from their audio devices.
 *
 * The streams being unbound do not all have to be on the same device. All
 * streams on the same device will be unbound atomically (data will stop flowing
 * through all unbound streams on the same device at the same time).
 *
 * Unbinding a stream that isn't bound to a device is a legal no-op.
 *
 * @param streams an array of audio streams to unbind. Can be nullptr or contain
 *                nullptr.
 * @param num_streams number streams listed in the `streams` array.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.BindAudioStreams
 */
inline void UnbindAudioStreams(std::span<AudioStreamRef> streams)
{
  SDL_UnbindAudioStreams(streams);
}

/**
 * Unbind a single audio stream from its audio device.
 *
 * This is a convenience function, equivalent to calling
 * `UnbindAudioStreams(&stream, 1)`.
 *
 * @param stream an audio stream to unbind from a device. Can be nullptr.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.BindAudioStream
 */
inline void UnbindAudioStream(AudioStreamParam stream)
{
  SDL_UnbindAudioStream(stream);
}

inline void AudioStream::Unbind() { SDL::UnbindAudioStream(m_resource); }

/**
 * Query an audio stream for its currently-bound device.
 *
 * This reports the logical audio device that an audio stream is currently bound
 * to.
 *
 * If not bound, or invalid, this returns zero, which is not a valid device ID.
 *
 * @param stream the audio stream to query.
 * @returns the bound audio device, or 0 if not bound or invalid.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioDevice.BindAudioStream
 * @sa AudioDevice.BindAudioStreams
 */
inline AudioDeviceRef GetAudioStreamDevice(AudioStreamParam stream)
{
  return SDL_GetAudioStreamDevice(stream);
}

inline AudioDeviceRef AudioStream::GetDevice() const
{
  return SDL::GetAudioStreamDevice(m_resource);
}

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
 * @sa AudioStream.PutData
 * @sa AudioStream.GetData
 * @sa AudioStream.GetAvailable
 * @sa AudioStream.Flush
 * @sa AudioStream.Clear
 * @sa AudioStream.SetFormat
 * @sa AudioStream.Destroy
 */
inline AudioStream CreateAudioStream(const AudioSpec& src_spec,
                                     const AudioSpec& dst_spec)
{
  return AudioStream(src_spec, dst_spec);
}

/**
 * Get the properties associated with an audio stream.
 *
 * The application can hang any data it wants here, but the following properties
 * are understood by SDL:
 *
 * - `prop::AudioStream._AUTO_CLEANUP_BOOLEAN`: if true (the default), the
 *   stream be automatically cleaned up when the audio subsystem quits. If set
 *   to false, the streams will persist beyond that. This property is ignored
 *   for streams created through AudioStream.AudioStream(), and will always be
 *   cleaned up. Streams that are not cleaned up will still be unbound from
 *   devices when the audio subsystem quits. This property was added in SDL
 *   3.4.0.
 *
 * @param stream the AudioStream to query.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline PropertiesRef GetAudioStreamProperties(AudioStreamParam stream)
{
  return CheckError(SDL_GetAudioStreamProperties(stream));
}

inline PropertiesRef AudioStream::GetProperties() const
{
  return SDL::GetAudioStreamProperties(m_resource);
}

namespace prop::AudioStream {

#if SDL_VERSION_ATLEAST(3, 4, 0)

constexpr auto _AUTO_CLEANUP_BOOLEAN =
  SDL_PROP_AUDIOSTREAM_AUTO_CLEANUP_BOOLEAN;

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

} // namespace prop::AudioStream

/**
 * Query the current format of an audio stream.
 *
 * @param stream the AudioStream to query.
 * @param src_spec where to store the input audio format; ignored if nullptr.
 * @param dst_spec where to store the output audio format; ignored if nullptr.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, as it holds a
 *               stream-specific mutex while running.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.SetFormat
 */
inline void GetAudioStreamFormat(AudioStreamParam stream,
                                 AudioSpec* src_spec,
                                 AudioSpec* dst_spec)
{
  CheckError(SDL_GetAudioStreamFormat(stream, src_spec, dst_spec));
}

inline void AudioStream::GetFormat(AudioSpec* src_spec,
                                   AudioSpec* dst_spec) const
{
  SDL::GetAudioStreamFormat(m_resource, src_spec, dst_spec);
}

/**
 * Change the input and output formats of an audio stream.
 *
 * Future calls to and AudioStream.GetAvailable and AudioStream.GetData will
 * reflect the new format, and future calls to AudioStream.PutData must provide
 * data in the new input formats.
 *
 * Data that was previously queued in the stream will still be operated on in
 * the format that was current when it was added, which is to say you can put
 * the end of a sound file in one format to a stream, change formats for the
 * next sound file, and start putting that new data while the previous sound
 * file is still queued, and everything will still play back correctly.
 *
 * If a stream is bound to a device, then the format of the side of the stream
 * bound to a device cannot be changed (src_spec for recording devices, dst_spec
 * for playback devices). Attempts to make a change to this side will be
 * ignored, but this will not report an error. The other side's format can be
 * changed.
 *
 * @param stream the stream the format is being changed.
 * @param src_spec the new format of the audio input; if nullptr, it is not
 *                 changed.
 * @param dst_spec the new format of the audio output; if nullptr, it is not
 *                 changed.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, as it holds a
 *               stream-specific mutex while running.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.GetFormat
 * @sa AudioStream.SetFrequencyRatio
 */
inline void SetAudioStreamFormat(AudioStreamParam stream,
                                 OptionalRef<const AudioSpec> src_spec,
                                 OptionalRef<const AudioSpec> dst_spec)
{
  CheckError(SDL_SetAudioStreamFormat(stream, src_spec, dst_spec));
}

inline void AudioStream::SetFormat(OptionalRef<const AudioSpec> src_spec,
                                   OptionalRef<const AudioSpec> dst_spec)
{
  SDL::SetAudioStreamFormat(m_resource, src_spec, dst_spec);
}

/**
 * Get the frequency ratio of an audio stream.
 *
 * @param stream the AudioStream to query.
 * @returns the frequency ratio of the stream or 0.0 on failure; call GetError()
 *          for more information.
 *
 * @threadsafety It is safe to call this function from any thread, as it holds a
 *               stream-specific mutex while running.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.SetFrequencyRatio
 */
inline float GetAudioStreamFrequencyRatio(AudioStreamParam stream)
{
  return SDL_GetAudioStreamFrequencyRatio(stream);
}

inline float AudioStream::GetFrequencyRatio() const
{
  return SDL::GetAudioStreamFrequencyRatio(m_resource);
}

/**
 * Change the frequency ratio of an audio stream.
 *
 * The frequency ratio is used to adjust the rate at which input data is
 * consumed. Changing this effectively modifies the speed and pitch of the
 * audio. A value greater than 1.0f will play the audio faster, and at a higher
 * pitch. A value less than 1.0f will play the audio slower, and at a lower
 * pitch. 1.0f means play at normal speed.
 *
 * This is applied during AudioStream.GetData, and can be continuously changed
 * to create various effects.
 *
 * @param stream the stream on which the frequency ratio is being changed.
 * @param ratio the frequency ratio. 1.0 is normal speed. Must be between 0.01
 *              and 100.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, as it holds a
 *               stream-specific mutex while running.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.GetFrequencyRatio
 * @sa AudioStream.SetFormat
 */
inline void SetAudioStreamFrequencyRatio(AudioStreamParam stream, float ratio)
{
  CheckError(SDL_SetAudioStreamFrequencyRatio(stream, ratio));
}

inline void AudioStream::SetFrequencyRatio(float ratio)
{
  SDL::SetAudioStreamFrequencyRatio(m_resource, ratio);
}

/**
 * Get the gain of an audio stream.
 *
 * The gain of a stream is its volume; a larger gain means a louder output, with
 * a gain of zero being silence.
 *
 * Audio streams default to a gain of 1.0f (no change in output).
 *
 * @param stream the AudioStream to query.
 * @returns the gain of the stream or -1.0f on failure; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread, as it holds a
 *               stream-specific mutex while running.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.SetGain
 */
inline float GetAudioStreamGain(AudioStreamParam stream)
{
  return SDL_GetAudioStreamGain(stream);
}

inline float AudioStream::GetGain() const
{
  return SDL::GetAudioStreamGain(m_resource);
}

/**
 * Change the gain of an audio stream.
 *
 * The gain of a stream is its volume; a larger gain means a louder output, with
 * a gain of zero being silence.
 *
 * Audio streams default to a gain of 1.0f (no change in output).
 *
 * This is applied during AudioStream.GetData, and can be continuously changed
 * to create various effects.
 *
 * @param stream the stream on which the gain is being changed.
 * @param gain the gain. 1.0f is no change, 0.0f is silence.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, as it holds a
 *               stream-specific mutex while running.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.GetGain
 */
inline void SetAudioStreamGain(AudioStreamParam stream, float gain)
{
  CheckError(SDL_SetAudioStreamGain(stream, gain));
}

inline void AudioStream::SetGain(float gain)
{
  SDL::SetAudioStreamGain(m_resource, gain);
}

/**
 * Get the current input channel map of an audio stream.
 *
 * Channel maps are optional; most things do not need them, instead passing data
 * in the [order that SDL expects](CategoryAudio#channel-layouts).
 *
 * Audio streams default to no remapping applied. This is represented by
 * returning nullptr, and does not signify an error.
 *
 * @param stream the AudioStream to query.
 * @param count On output, set to number of channels in the map. Can be nullptr.
 * @returns an array of the current channel mapping, with as many elements as
 *          the current output spec's channels, or nullptr if default. This
 *          should be freed with free() when it is no longer needed.
 *
 * @threadsafety It is safe to call this function from any thread, as it holds a
 *               stream-specific mutex while running.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.SetInputChannelMap
 */
inline OwnArray<int> GetAudioStreamInputChannelMap(AudioStreamParam stream)
{
  return SDL_GetAudioStreamInputChannelMap(stream);
}

inline OwnArray<int> AudioStream::GetInputChannelMap() const
{
  return SDL::GetAudioStreamInputChannelMap(m_resource);
}

/**
 * Get the current output channel map of an audio stream.
 *
 * Channel maps are optional; most things do not need them, instead passing data
 * in the [order that SDL expects](CategoryAudio#channel-layouts).
 *
 * Audio streams default to no remapping applied. This is represented by
 * returning nullptr, and does not signify an error.
 *
 * @param stream the AudioStream to query.
 * @param count On output, set to number of channels in the map. Can be nullptr.
 * @returns an array of the current channel mapping, with as many elements as
 *          the current output spec's channels, or nullptr if default. This
 *          should be freed with free() when it is no longer needed.
 *
 * @threadsafety It is safe to call this function from any thread, as it holds a
 *               stream-specific mutex while running.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.SetInputChannelMap
 */
inline OwnArray<int> GetAudioStreamOutputChannelMap(AudioStreamParam stream)
{
  return SDL_GetAudioStreamOutputChannelMap(stream);
}

inline OwnArray<int> AudioStream::GetOutputChannelMap() const
{
  return SDL::GetAudioStreamOutputChannelMap(m_resource);
}

/**
 * Set the current input channel map of an audio stream.
 *
 * Channel maps are optional; most things do not need them, instead passing data
 * in the [order that SDL expects](CategoryAudio#channel-layouts).
 *
 * The input channel map reorders data that is added to a stream via
 * AudioStream.PutData. Future calls to AudioStream.PutData must provide data in
 * the new channel order.
 *
 * Each item in the array represents an input channel, and its value is the
 * channel that it should be remapped to. To reverse a stereo signal's left and
 * right values, you'd have an array of `{ 1, 0 }`. It is legal to remap
 * multiple channels to the same thing, so `{ 1, 1 }` would duplicate the right
 * channel to both channels of a stereo signal. An element in the channel map
 * set to -1 instead of a valid channel will mute that channel, setting it to a
 * silence value.
 *
 * You cannot change the number of channels through a channel map, just
 * reorder/mute them.
 *
 * Data that was previously queued in the stream will still be operated on in
 * the order that was current when it was added, which is to say you can put the
 * end of a sound file in one order to a stream, change orders for the next
 * sound file, and start putting that new data while the previous sound file is
 * still queued, and everything will still play back correctly.
 *
 * Audio streams default to no remapping applied. Passing a nullptr channel map
 * is legal, and turns off remapping.
 *
 * SDL will copy the channel map; the caller does not have to save this array
 * after this call.
 *
 * If `count` is not equal to the current number of channels in the audio
 * stream's format, this will fail. This is a safety measure to make sure a race
 * condition hasn't changed the format while this call is setting the channel
 * map.
 *
 * Unlike attempting to change the stream's format, the input channel map on a
 * stream bound to a recording device is permitted to change at any time; any
 * data added to the stream from the device after this call will have the new
 * mapping, but previously-added data will still have the prior mapping.
 *
 * @param stream the AudioStream to change.
 * @param chmap the new channel map, nullptr to reset to default.
 * @param count The number of channels in the map.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, as it holds a
 *               stream-specific mutex while running. Don't change the stream's
 *               format to have a different number of channels from a different
 *               thread at the same time, though!
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.SetInputChannelMap
 */
inline void SetAudioStreamInputChannelMap(AudioStreamParam stream,
                                          std::span<int> chmap)
{
  CheckError(SDL_SetAudioStreamInputChannelMap(stream, chmap));
}

inline void AudioStream::SetInputChannelMap(std::span<int> chmap)
{
  SDL::SetAudioStreamInputChannelMap(m_resource, chmap);
}

/**
 * Set the current output channel map of an audio stream.
 *
 * Channel maps are optional; most things do not need them, instead passing data
 * in the [order that SDL expects](CategoryAudio#channel-layouts).
 *
 * The output channel map reorders data that is leaving a stream via
 * AudioStream.GetData.
 *
 * Each item in the array represents an input channel, and its value is the
 * channel that it should be remapped to. To reverse a stereo signal's left and
 * right values, you'd have an array of `{ 1, 0 }`. It is legal to remap
 * multiple channels to the same thing, so `{ 1, 1 }` would duplicate the right
 * channel to both channels of a stereo signal. An element in the channel map
 * set to -1 instead of a valid channel will mute that channel, setting it to a
 * silence value.
 *
 * You cannot change the number of channels through a channel map, just
 * reorder/mute them.
 *
 * The output channel map can be changed at any time, as output remapping is
 * applied during AudioStream.GetData.
 *
 * Audio streams default to no remapping applied. Passing a nullptr channel map
 * is legal, and turns off remapping.
 *
 * SDL will copy the channel map; the caller does not have to save this array
 * after this call.
 *
 * If `count` is not equal to the current number of channels in the audio
 * stream's format, this will fail. This is a safety measure to make sure a race
 * condition hasn't changed the format while this call is setting the channel
 * map.
 *
 * Unlike attempting to change the stream's format, the output channel map on a
 * stream bound to a recording device is permitted to change at any time; any
 * data added to the stream after this call will have the new mapping, but
 * previously-added data will still have the prior mapping. When the channel map
 * doesn't match the hardware's channel layout, SDL will convert the data before
 * feeding it to the device for playback.
 *
 * @param stream the AudioStream to change.
 * @param chmap the new channel map, nullptr to reset to default.
 * @param count The number of channels in the map.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, as it holds a
 *               stream-specific mutex while running. Don't change the stream's
 *               format to have a different number of channels from a a
 *               different thread at the same time, though!
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.SetInputChannelMap
 */
inline void SetAudioStreamOutputChannelMap(AudioStreamParam stream,
                                           std::span<int> chmap)
{
  CheckError(SDL_SetAudioStreamOutputChannelMap(stream, chmap));
}

inline void AudioStream::SetOutputChannelMap(std::span<int> chmap)
{
  SDL::SetAudioStreamOutputChannelMap(m_resource, chmap);
}

/**
 * Add data to the stream.
 *
 * This data must match the format/channels/samplerate specified in the latest
 * call to AudioStream.SetFormat, or the format specified when creating the
 * stream if it hasn't been changed.
 *
 * Note that this call simply copies the unconverted data for later. This is
 * different than SDL2, where data was converted during the Put call and the Get
 * call would just dequeue the previously-converted data.
 *
 * @param stream the stream the audio data is being added to.
 * @param buf a pointer to the audio data to add.
 * @param len the number of bytes to write to the stream.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, but if the
 *               stream has a callback set, the caller might need to manage
 *               extra locking.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.Clear
 * @sa AudioStream.Flush
 * @sa AudioStream.GetData
 * @sa AudioStream.GetQueued
 */
inline void PutAudioStreamData(AudioStreamParam stream, SourceBytes buf)
{
  CheckError(SDL_PutAudioStreamData(stream, buf.data(), buf.size_bytes()));
}

inline void AudioStream::PutData(SourceBytes buf)
{
  SDL::PutAudioStreamData(m_resource, std::move(buf));
}

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Add external data to an audio stream without copying it.
 *
 * Unlike AudioStream.PutData(), this function does not make a copy of the
 * provided data, instead storing the provided pointer. This means that the put
 * operation does not need to allocate and copy the data, but the original data
 * must remain available until the stream is done with it, either by being read
 * from the stream in its entirety, or a call to AudioStream.Clear() or
 * AudioStream.Destroy().
 *
 * The data must match the format/channels/samplerate specified in the latest
 * call to AudioStream.SetFormat, or the format specified when creating the
 * stream if it hasn't been changed.
 *
 * An optional callback may be provided, which is called when the stream no
 * longer needs the data. Once this callback fires, the stream will not access
 * the data again. This callback will fire for any reason the data is no longer
 * needed, including clearing or destroying the stream.
 *
 * Note that there is still an allocation to store tracking information, so this
 * function is more efficient for larger blocks of data. If you're planning to
 * put a few samples at a time, it will be more efficient to use
 * AudioStream.PutData(), which allocates and buffers in blocks.
 *
 * @param stream the stream the audio data is being added to.
 * @param buf a pointer to the audio data to add.
 * @param len the number of bytes to add to the stream.
 * @param callback the callback function to call when the data is no longer
 *                 needed by the stream. May be nullptr.
 * @param userdata an opaque pointer provided to the callback for its own
 *                 personal use.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, but if the
 *               stream has a callback set, the caller might need to manage
 *               extra locking.
 *
 * @since This function is available since SDL 3.4.0.
 *
 * @sa AudioStream.Clear
 * @sa AudioStream.Flush
 * @sa AudioStream.GetData
 * @sa AudioStream.GetQueued
 */
inline void PutAudioStreamDataNoCopy(AudioStreamParam stream,
                                     SourceBytes buf,
                                     AudioStreamDataCompleteCallback callback,
                                     void* userdata)
{
  CheckError(SDL_PutAudioStreamDataNoCopy(
    stream, buf.data(), buf.size_bytes(), callback, userdata));
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Add external data to an audio stream without copying it.
 *
 * Unlike AudioStream.PutData(), this function does not make a copy of the
 * provided data, instead storing the provided pointer. This means that the put
 * operation does not need to allocate and copy the data, but the original data
 * must remain available until the stream is done with it, either by being read
 * from the stream in its entirety, or a call to AudioStream.Clear() or
 * AudioStream.Destroy().
 *
 * The data must match the format/channels/samplerate specified in the latest
 * call to AudioStream.SetFormat, or the format specified when creating the
 * stream if it hasn't been changed.
 *
 * An optional callback may be provided, which is called when the stream no
 * longer needs the data. Once this callback fires, the stream will not access
 * the data again. This callback will fire for any reason the data is no longer
 * needed, including clearing or destroying the stream.
 *
 * Note that there is still an allocation to store tracking information, so this
 * function is more efficient for larger blocks of data. If you're planning to
 * put a few samples at a time, it will be more efficient to use
 * AudioStream.PutData(), which allocates and buffers in blocks.
 *
 * @param stream the stream the audio data is being added to.
 * @param buf a pointer to the audio data to add.
 * @param len the number of bytes to add to the stream.
 * @param callback the callback function to call when the data is no longer
 *                 needed by the stream. May be nullptr.
 * @param userdata an opaque pointer provided to the callback for its own
 *                 personal use.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, but if the
 *               stream has a callback set, the caller might need to manage
 *               extra locking.
 *
 * @since This function is available since SDL 3.4.0.
 *
 * @sa AudioStream.Clear
 * @sa AudioStream.Flush
 * @sa AudioStream.GetData
 * @sa AudioStream.GetQueued
 */
inline void PutAudioStreamDataNoCopy(AudioStreamParam stream,
                                     SourceBytes buf,
                                     AudioStreamDataCompleteCB callback)
{
  static_assert(false, "Not implemented");
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

inline void AudioStream::PutDataNoCopy(SourceBytes buf,
                                       AudioStreamDataCompleteCallback callback,
                                       void* userdata)
{
  SDL::PutAudioStreamDataNoCopy(m_resource, std::move(buf), callback, userdata);
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

inline void AudioStream::PutDataNoCopy(SourceBytes buf,
                                       AudioStreamDataCompleteCB callback)
{
  SDL::PutAudioStreamDataNoCopy(m_resource, std::move(buf), callback);
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Add data to the stream with each channel in a separate array.
 *
 * This data must match the format/channels/samplerate specified in the latest
 * call to AudioStream.SetFormat, or the format specified when creating the
 * stream if it hasn't been changed.
 *
 * The data will be interleaved and queued. Note that AudioStream only operates
 * on interleaved data, so this is simply a convenience function for easily
 * queueing data from sources that provide separate arrays. There is no
 * equivalent function to retrieve planar data.
 *
 * The arrays in `channel_buffers` are ordered as they are to be interleaved;
 * the first array will be the first sample in the interleaved data. Any
 * individual array may be nullptr; in this case, silence will be interleaved
 * for that channel.
 *
 * `num_channels` specifies how many arrays are in `channel_buffers`. This can
 * be used as a safety to prevent overflow, in case the stream format has
 * changed elsewhere. If more channels are specified than the current input
 * spec, they are ignored. If less channels are specified, the missing arrays
 * are treated as if they are nullptr (silence is written to those channels). If
 * the count is -1, SDL will assume the array count matches the current input
 * spec.
 *
 * Note that `num_samples` is the number of _samples per array_. This can also
 * be thought of as the number of _sample frames_ to be queued. A value of 1
 * with stereo arrays will queue two samples to the stream. This is different
 * than AudioStream.PutData, which wants the size of a single array in bytes.
 *
 * @param stream the stream the audio data is being added to.
 * @param channel_buffers a pointer to an array of arrays, one array per
 *                        channel.
 * @param num_channels the number of arrays in `channel_buffers` or -1.
 * @param num_samples the number of _samples_ per array to write to the stream.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread, but if the
 *               stream has a callback set, the caller might need to manage
 *               extra locking.
 *
 * @since This function is available since SDL 3.4.0.
 *
 * @sa AudioStream.Clear
 * @sa AudioStream.Flush
 * @sa AudioStream.GetData
 * @sa AudioStream.GetQueued
 */
inline void PutAudioStreamPlanarData(AudioStreamParam stream,
                                     const void* const* channel_buffers,
                                     int num_channels,
                                     int num_samples)
{
  CheckError(SDL_PutAudioStreamPlanarData(
    stream, channel_buffers, num_channels, num_samples));
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

#if SDL_VERSION_ATLEAST(3, 4, 0)

inline void AudioStream::PutPlanarData(const void* const* channel_buffers,
                                       int num_channels,
                                       int num_samples)
{
  SDL::PutAudioStreamPlanarData(
    m_resource, channel_buffers, num_channels, num_samples);
}

#endif // SDL_VERSION_ATLEAST(3, 4, 0)

/**
 * Get converted/resampled data from the stream.
 *
 * The input/output data format/channels/samplerate is specified when creating
 * the stream, and can be changed after creation by calling
 * AudioStream.SetFormat.
 *
 * Note that any conversion and resampling necessary is done during this call,
 * and AudioStream.PutData simply queues unconverted data for later. This is
 * different than SDL2, where that work was done while inputting new data to the
 * stream and requesting the output just copied the converted data.
 *
 * @param stream the stream the audio is being requested from.
 * @param buf a buffer to fill with audio data.
 * @param len the maximum number of bytes to fill.
 * @returns the number of bytes read from the stream or -1 on failure; call
 *          GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread, but if the
 *               stream has a callback set, the caller might need to manage
 *               extra locking.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.Clear
 * @sa AudioStream.GetAvailable
 * @sa AudioStream.PutData
 */
inline int GetAudioStreamData(AudioStreamParam stream, TargetBytes buf)
{
  return SDL_GetAudioStreamData(stream, buf.data(), buf.size_bytes());
}

inline int AudioStream::GetData(TargetBytes buf)
{
  return SDL::GetAudioStreamData(m_resource, std::move(buf));
}

/**
 * Get the number of converted/resampled bytes available.
 *
 * The stream may be buffering data behind the scenes until it has enough to
 * resample correctly, so this number might be lower than what you expect, or
 * even be zero. Add more data or flush the stream if you need the data now.
 *
 * If the stream has so much data that it would overflow an int, the return
 * value is clamped to a maximum value, but no queued data is lost; if there are
 * gigabytes of data queued, the app might need to read some of it with
 * AudioStream.GetData before this function's return value is no longer clamped.
 *
 * @param stream the audio stream to query.
 * @returns the number of converted/resampled bytes available or -1 on failure;
 *          call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.GetData
 * @sa AudioStream.PutData
 */
inline int GetAudioStreamAvailable(AudioStreamParam stream)
{
  return SDL_GetAudioStreamAvailable(stream);
}

inline int AudioStream::GetAvailable() const
{
  return SDL::GetAudioStreamAvailable(m_resource);
}

/**
 * Get the number of bytes currently queued.
 *
 * This is the number of bytes put into a stream as input, not the number that
 * can be retrieved as output. Because of several details, it's not possible to
 * calculate one number directly from the other. If you need to know how much
 * usable data can be retrieved right now, you should use
 * AudioStream.GetAvailable() and not this function.
 *
 * Note that audio streams can change their input format at any time, even if
 * there is still data queued in a different format, so the returned byte count
 * will not necessarily match the number of _sample frames_ available. Users of
 * this API should be aware of format changes they make when feeding a stream
 * and plan accordingly.
 *
 * Queued data is not converted until it is consumed by AudioStream.GetData, so
 * this value should be representative of the exact data that was put into the
 * stream.
 *
 * If the stream has so much data that it would overflow an int, the return
 * value is clamped to a maximum value, but no queued data is lost; if there are
 * gigabytes of data queued, the app might need to read some of it with
 * AudioStream.GetData before this function's return value is no longer clamped.
 *
 * @param stream the audio stream to query.
 * @returns the number of bytes queued or -1 on failure; call GetError() for
 *          more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.PutData
 * @sa AudioStream.Clear
 */
inline int GetAudioStreamQueued(AudioStreamParam stream)
{
  return SDL_GetAudioStreamQueued(stream);
}

inline int AudioStream::GetQueued() const
{
  return SDL::GetAudioStreamQueued(m_resource);
}

/**
 * Tell the stream that you're done sending data, and anything being buffered
 * should be converted/resampled and made available immediately.
 *
 * It is legal to add more data to a stream after flushing, but there may be
 * audio gaps in the output. Generally this is intended to signal the end of
 * input, so the complete output becomes available.
 *
 * @param stream the audio stream to flush.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.PutData
 */
inline void FlushAudioStream(AudioStreamParam stream)
{
  CheckError(SDL_FlushAudioStream(stream));
}

inline void AudioStream::Flush() { SDL::FlushAudioStream(m_resource); }

/**
 * Clear any pending data in the stream.
 *
 * This drops any queued data, so there will be nothing to read from the stream
 * until more is added.
 *
 * @param stream the audio stream to clear.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.GetAvailable
 * @sa AudioStream.GetData
 * @sa AudioStream.GetQueued
 * @sa AudioStream.PutData
 */
inline void ClearAudioStream(AudioStreamParam stream)
{
  CheckError(SDL_ClearAudioStream(stream));
}

inline void AudioStream::Clear() { SDL::ClearAudioStream(m_resource); }

/**
 * Use this function to pause audio playback on the audio device associated with
 * an audio stream.
 *
 * This function pauses audio processing for a given device. Any bound audio
 * streams will not progress, and no audio will be generated. Pausing one device
 * does not prevent other unpaused devices from running.
 *
 * Pausing a device can be useful to halt all audio without unbinding all the
 * audio streams. This might be useful while a game is paused, or a level is
 * loading, etc.
 *
 * @param stream the audio stream associated with the audio device to pause.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.ResumeDevice
 */
inline void PauseAudioStreamDevice(AudioStreamParam stream)
{
  CheckError(SDL_PauseAudioStreamDevice(stream));
}

inline void AudioStream::PauseDevice()
{
  SDL::PauseAudioStreamDevice(m_resource);
}

/**
 * Use this function to unpause audio playback on the audio device associated
 * with an audio stream.
 *
 * This function unpauses audio processing for a given device that has
 * previously been paused. Once unpaused, any bound audio streams will begin to
 * progress again, and audio can be generated.
 *
 * AudioStream.AudioStream opens audio devices in a paused state, so this
 * function call is required for audio playback to begin on such devices.
 *
 * @param stream the audio stream associated with the audio device to resume.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.PauseDevice
 */
inline void ResumeAudioStreamDevice(AudioStreamParam stream)
{
  CheckError(SDL_ResumeAudioStreamDevice(stream));
}

inline void AudioStream::ResumeDevice()
{
  SDL::ResumeAudioStreamDevice(m_resource);
}

/**
 * Use this function to query if an audio device associated with a stream is
 * paused.
 *
 * Unlike in SDL2, audio devices start in an _unpaused_ state, since an app has
 * to bind a stream before any audio will flow.
 *
 * @param stream the audio stream associated with the audio device to query.
 * @returns true if device is valid and paused, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.PauseDevice
 * @sa AudioStream.ResumeDevice
 */
inline bool AudioStreamDevicePaused(AudioStreamParam stream)
{
  return SDL_AudioStreamDevicePaused(stream);
}

inline bool AudioStream::DevicePaused() const
{
  return SDL::AudioStreamDevicePaused(m_resource);
}

/**
 * Lock an audio stream for serialized access.
 *
 * Each AudioStream has an internal mutex it uses to protect its data structures
 * from threading conflicts. This function allows an app to lock that mutex,
 * which could be useful if registering callbacks on this stream.
 *
 * One does not need to lock a stream to use in it most cases, as the stream
 * manages this lock internally. However, this lock is held during callbacks,
 * which may run from arbitrary threads at any time, so if an app needs to
 * protect shared data during those callbacks, locking the stream guarantees
 * that the callback is not running while the lock is held.
 *
 * As this is just a wrapper over Mutex.Lock for an internal lock; it has all
 * the same attributes (recursive locks are allowed, etc).
 *
 * @param stream the audio stream to lock.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.Unlock
 */
inline void LockAudioStream(AudioStreamParam stream)
{
  CheckError(SDL_LockAudioStream(stream));
}

inline void AudioStream::Lock() { SDL::LockAudioStream(m_resource); }

inline AudioStreamLock::AudioStreamLock(AudioStreamRef resource)
  : m_lock(std::move(resource))
{
  LockAudioStream(m_lock);
}

/**
 * Unlock an audio stream for serialized access.
 *
 * This unlocks an audio stream after a call to AudioStream.Lock.
 *
 * @param stream the audio stream to unlock.
 * @throws Error on failure.
 *
 * @threadsafety You should only call this from the same thread that previously
 *               called AudioStream.Lock.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.Lock
 */
inline void UnlockAudioStream(AudioStreamParam stream)
{
  CheckError(SDL_UnlockAudioStream(stream));
}

inline void AudioStream::Unlock() { SDL::UnlockAudioStream(m_resource); }

inline void AudioStreamLock::reset()
{
  if (!m_lock) return;
  UnlockAudioStream(m_lock);
  m_lock = {};
}

/**
 * Set a callback that runs when data is requested from an audio stream.
 *
 * This callback is called _before_ data is obtained from the stream, giving the
 * callback the chance to add more on-demand.
 *
 * The callback can (optionally) call AudioStream.PutData() to add more audio to
 * the stream during this call; if needed, the request that triggered this
 * callback will obtain the new data immediately.
 *
 * The callback's `additional_amount` argument is roughly how many bytes of
 * _unconverted_ data (in the stream's input format) is needed by the caller,
 * although this may overestimate a little for safety. This takes into account
 * how much is already in the stream and only asks for any extra necessary to
 * resolve the request, which means the callback may be asked for zero bytes,
 * and a different amount on each call.
 *
 * The callback is not required to supply exact amounts; it is allowed to supply
 * too much or too little or none at all. The caller will get what's available,
 * up to the amount they requested, regardless of this callback's outcome.
 *
 * Clearing or flushing an audio stream does not call this callback.
 *
 * This function obtains the stream's lock, which means any existing callback
 * (get or put) in progress will finish running before setting the new callback.
 *
 * Setting a nullptr function turns off the callback.
 *
 * @param stream the audio stream to set the new callback on.
 * @param callback the new callback function to call when data is requested from
 *                 the stream.
 * @param userdata an opaque pointer provided to the callback for its own
 *                 personal use.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.SetPutCallback
 */
inline void SetAudioStreamGetCallback(AudioStreamParam stream,
                                      AudioStreamCallback callback,
                                      void* userdata)
{
  CheckError(SDL_SetAudioStreamGetCallback(stream, callback, userdata));
}

/**
 * Set a callback that runs when data is requested from an audio stream.
 *
 * This callback is called _before_ data is obtained from the stream, giving the
 * callback the chance to add more on-demand.
 *
 * The callback can (optionally) call AudioStream.PutData() to add more audio to
 * the stream during this call; if needed, the request that triggered this
 * callback will obtain the new data immediately.
 *
 * The callback's `additional_amount` argument is roughly how many bytes of
 * _unconverted_ data (in the stream's input format) is needed by the caller,
 * although this may overestimate a little for safety. This takes into account
 * how much is already in the stream and only asks for any extra necessary to
 * resolve the request, which means the callback may be asked for zero bytes,
 * and a different amount on each call.
 *
 * The callback is not required to supply exact amounts; it is allowed to supply
 * too much or too little or none at all. The caller will get what's available,
 * up to the amount they requested, regardless of this callback's outcome.
 *
 * Clearing or flushing an audio stream does not call this callback.
 *
 * This function obtains the stream's lock, which means any existing callback
 * (get or put) in progress will finish running before setting the new callback.
 *
 * Setting a nullptr function turns off the callback.
 *
 * @param stream the audio stream to set the new callback on.
 * @param callback the new callback function to call when data is requested from
 *                 the stream.
 * @param userdata an opaque pointer provided to the callback for its own
 *                 personal use.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.SetPutCallback
 */
inline void SetAudioStreamGetCallback(AudioStreamParam stream,
                                      AudioStreamCB callback)
{
  static_assert(false, "Not implemented");
}

inline void AudioStream::SetGetCallback(AudioStreamCallback callback,
                                        void* userdata)
{
  SDL::SetAudioStreamGetCallback(m_resource, callback, userdata);
}

inline void AudioStream::SetGetCallback(AudioStreamCB callback)
{
  SDL::SetAudioStreamGetCallback(m_resource, callback);
}

/**
 * Set a callback that runs when data is added to an audio stream.
 *
 * This callback is called _after_ the data is added to the stream, giving the
 * callback the chance to obtain it immediately.
 *
 * The callback can (optionally) call AudioStream.GetData() to obtain audio from
 * the stream during this call.
 *
 * The callback's `additional_amount` argument is how many bytes of _converted_
 * data (in the stream's output format) was provided by the caller, although
 * this may underestimate a little for safety. This value might be less than
 * what is currently available in the stream, if data was already there, and
 * might be less than the caller provided if the stream needs to keep a buffer
 * to aid in resampling. Which means the callback may be provided with zero
 * bytes, and a different amount on each call.
 *
 * The callback may call AudioStream.GetAvailable to see the total amount
 * currently available to read from the stream, instead of the total provided by
 * the current call.
 *
 * The callback is not required to obtain all data. It is allowed to read less
 * or none at all. Anything not read now simply remains in the stream for later
 * access.
 *
 * Clearing or flushing an audio stream does not call this callback.
 *
 * This function obtains the stream's lock, which means any existing callback
 * (get or put) in progress will finish running before setting the new callback.
 *
 * Setting a nullptr function turns off the callback.
 *
 * @param stream the audio stream to set the new callback on.
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
 * @sa AudioStream.SetGetCallback
 */
inline void SetAudioStreamPutCallback(AudioStreamParam stream,
                                      AudioStreamCallback callback,
                                      void* userdata)
{
  CheckError(SDL_SetAudioStreamPutCallback(stream, callback, userdata));
}

/**
 * Set a callback that runs when data is added to an audio stream.
 *
 * This callback is called _after_ the data is added to the stream, giving the
 * callback the chance to obtain it immediately.
 *
 * The callback can (optionally) call AudioStream.GetData() to obtain audio from
 * the stream during this call.
 *
 * The callback's `additional_amount` argument is how many bytes of _converted_
 * data (in the stream's output format) was provided by the caller, although
 * this may underestimate a little for safety. This value might be less than
 * what is currently available in the stream, if data was already there, and
 * might be less than the caller provided if the stream needs to keep a buffer
 * to aid in resampling. Which means the callback may be provided with zero
 * bytes, and a different amount on each call.
 *
 * The callback may call AudioStream.GetAvailable to see the total amount
 * currently available to read from the stream, instead of the total provided by
 * the current call.
 *
 * The callback is not required to obtain all data. It is allowed to read less
 * or none at all. Anything not read now simply remains in the stream for later
 * access.
 *
 * Clearing or flushing an audio stream does not call this callback.
 *
 * This function obtains the stream's lock, which means any existing callback
 * (get or put) in progress will finish running before setting the new callback.
 *
 * Setting a nullptr function turns off the callback.
 *
 * @param stream the audio stream to set the new callback on.
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
 * @sa AudioStream.SetGetCallback
 */
inline void SetAudioStreamPutCallback(AudioStreamParam stream,
                                      AudioStreamCB callback)
{
  static_assert(false, "Not implemented");
}

inline void AudioStream::SetPutCallback(AudioStreamCallback callback,
                                        void* userdata)
{
  SDL::SetAudioStreamPutCallback(m_resource, callback, userdata);
}

inline void AudioStream::SetPutCallback(AudioStreamCB callback)
{
  SDL::SetAudioStreamPutCallback(m_resource, callback);
}

/**
 * Free an audio stream.
 *
 * This will release all allocated data, including any audio that is still
 * queued. You do not need to manually clear the stream first.
 *
 * If this stream was bound to an audio device, it is unbound during this call.
 * If this stream was created with AudioStream.AudioStream, the audio device
 * that was opened alongside this stream's creation will be closed, too.
 *
 * @param stream the audio stream to destroy.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.AudioStream
 */
inline void DestroyAudioStream(AudioStreamRaw stream)
{
  SDL_DestroyAudioStream(stream);
}

inline void AudioStream::Destroy() { DestroyAudioStream(release()); }

/**
 * Convenience function for straightforward audio init for the common case.
 *
 * If all your app intends to do is provide a single source of PCM audio, this
 * function allows you to do all your audio setup in a single call.
 *
 * This is also intended to be a clean means to migrate apps from SDL2.
 *
 * This function will open an audio device, create a stream and bind it. Unlike
 * other methods of setup, the audio device will be closed when this stream is
 * destroyed, so the app can treat the returned AudioStream as the only object
 * needed to manage audio playback.
 *
 * Also unlike other functions, the audio device begins paused. This is to map
 * more closely to SDL2-style behavior, since there is no extra step here to
 * bind a stream to begin audio flowing. The audio device should be resumed with
 * AudioStream.ResumeDevice().
 *
 * This function works with both playback and recording devices.
 *
 * The `spec` parameter represents the app's side of the audio stream. That is,
 * for recording audio, this will be the output format, and for playing audio,
 * this will be the input format. If spec is nullptr, the system will choose the
 * format, and the app can use AudioStream.GetFormat() to obtain this
 * information later.
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
 * Destroying the returned stream with AudioStream.Destroy will also close the
 * audio device associated with this stream.
 *
 * @param devid an audio device to open, or AUDIO_DEVICE_DEFAULT_PLAYBACK or
 *              AUDIO_DEVICE_DEFAULT_RECORDING.
 * @param spec the audio stream's data format. Can be nullptr.
 * @param callback a callback where the app will provide new data for playback,
 *                 or receive new data for recording. Can be nullptr, in which
 *                 case the app will need to call AudioStream.PutData or
 *                 AudioStream.GetData as necessary.
 * @param userdata app-controlled pointer passed to callback. Can be nullptr.
 *                 Ignored if callback is nullptr.
 * @returns an audio stream on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.GetDevice
 * @sa AudioStream.ResumeDevice
 */
inline AudioStream OpenAudioDeviceStream(AudioDeviceParam devid,
                                         const AudioSpec& spec,
                                         AudioStreamCallback callback,
                                         void* userdata)
{
  return AudioStream(devid, spec, callback, userdata);
}

/**
 * Convenience function for straightforward audio init for the common case.
 *
 * If all your app intends to do is provide a single source of PCM audio, this
 * function allows you to do all your audio setup in a single call.
 *
 * This is also intended to be a clean means to migrate apps from SDL2.
 *
 * This function will open an audio device, create a stream and bind it. Unlike
 * other methods of setup, the audio device will be closed when this stream is
 * destroyed, so the app can treat the returned AudioStream as the only object
 * needed to manage audio playback.
 *
 * Also unlike other functions, the audio device begins paused. This is to map
 * more closely to SDL2-style behavior, since there is no extra step here to
 * bind a stream to begin audio flowing. The audio device should be resumed with
 * AudioStream.ResumeDevice().
 *
 * This function works with both playback and recording devices.
 *
 * The `spec` parameter represents the app's side of the audio stream. That is,
 * for recording audio, this will be the output format, and for playing audio,
 * this will be the input format. If spec is nullptr, the system will choose the
 * format, and the app can use AudioStream.GetFormat() to obtain this
 * information later.
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
 * Destroying the returned stream with AudioStream.Destroy will also close the
 * audio device associated with this stream.
 *
 * @param devid an audio device to open, or AUDIO_DEVICE_DEFAULT_PLAYBACK or
 *              AUDIO_DEVICE_DEFAULT_RECORDING.
 * @param spec the audio stream's data format. Can be nullptr.
 * @param callback a callback where the app will provide new data for playback,
 *                 or receive new data for recording. Can be nullptr, in which
 *                 case the app will need to call AudioStream.PutData or
 *                 AudioStream.GetData as necessary.
 * @param userdata app-controlled pointer passed to callback. Can be nullptr.
 *                 Ignored if callback is nullptr.
 * @returns an audio stream on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa AudioStream.GetDevice
 * @sa AudioStream.ResumeDevice
 */
inline AudioStream OpenAudioDeviceStream(AudioDeviceParam devid,
                                         OptionalRef<const AudioSpec> spec,
                                         AudioStreamCB callback)
{
  return AudioStream(devid, spec, callback);
}

inline AudioStreamRef AudioDevice::OpenStream(const AudioSpec& spec,
                                              AudioStreamCallback callback,
                                              void* userdata)
{
  return AudioStream(m_resource, spec, callback, userdata);
}

inline AudioStream AudioDevice::OpenStream(OptionalRef<const AudioSpec> spec,
                                           AudioStreamCB callback)
{
  return SDL::OpenAudioDeviceStream(m_resource, spec, callback);
}

inline AudioStream::AudioStream(AudioDeviceParam devid,
                                OptionalRef<const AudioSpec> spec,
                                AudioStreamCB callback)
{
}

/**
 * Set a callback that fires when data is about to be fed to an audio device.
 *
 * This is useful for accessing the final mix, perhaps for writing a visualizer
 * or applying a final effect to the audio data before playback.
 *
 * The buffer is the final mix of all bound audio streams on an opened device;
 * this callback will fire regularly for any device that is both opened and
 * unpaused. If there is no new data to mix, either because no streams are bound
 * to the device or all the streams are empty, this callback will still fire
 * with the entire buffer set to silence.
 *
 * This callback is allowed to make changes to the data; the contents of the
 * buffer after this call is what is ultimately passed along to the hardware.
 *
 * The callback is always provided the data in float format (values from -1.0f
 * to 1.0f), but the number of channels or sample rate may be different than the
 * format the app requested when opening the device; SDL might have had to
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
 * their preferred format through an AudioStream and let SDL handle the
 * difference.
 *
 * This function is extremely time-sensitive; the callback should do the least
 * amount of work possible and return as quickly as it can. The longer the
 * callback runs, the higher the risk of audio dropouts or other problems.
 *
 * This function will block until the audio device is in between iterations, so
 * any existing callback that might be running will finish before this function
 * sets the new callback and returns.
 *
 * Setting a nullptr callback function disables any previously-set callback.
 *
 * @param devid the ID of an opened audio device.
 * @param callback a callback function to be called. Can be nullptr.
 * @param userdata app-controlled pointer passed to callback. Can be nullptr.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void SetAudioPostmixCallback(AudioDeviceParam devid,
                                    AudioPostmixCallback callback,
                                    void* userdata)
{
  CheckError(SDL_SetAudioPostmixCallback(devid, callback, userdata));
}

/**
 * Set a callback that fires when data is about to be fed to an audio device.
 *
 * This is useful for accessing the final mix, perhaps for writing a visualizer
 * or applying a final effect to the audio data before playback.
 *
 * The buffer is the final mix of all bound audio streams on an opened device;
 * this callback will fire regularly for any device that is both opened and
 * unpaused. If there is no new data to mix, either because no streams are bound
 * to the device or all the streams are empty, this callback will still fire
 * with the entire buffer set to silence.
 *
 * This callback is allowed to make changes to the data; the contents of the
 * buffer after this call is what is ultimately passed along to the hardware.
 *
 * The callback is always provided the data in float format (values from -1.0f
 * to 1.0f), but the number of channels or sample rate may be different than the
 * format the app requested when opening the device; SDL might have had to
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
 * their preferred format through an AudioStream and let SDL handle the
 * difference.
 *
 * This function is extremely time-sensitive; the callback should do the least
 * amount of work possible and return as quickly as it can. The longer the
 * callback runs, the higher the risk of audio dropouts or other problems.
 *
 * This function will block until the audio device is in between iterations, so
 * any existing callback that might be running will finish before this function
 * sets the new callback and returns.
 *
 * Setting a nullptr callback function disables any previously-set callback.
 *
 * @param devid the ID of an opened audio device.
 * @param callback a callback function to be called. Can be nullptr.
 * @param userdata app-controlled pointer passed to callback. Can be nullptr.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline void SetAudioPostmixCallback(AudioDeviceParam devid,
                                    AudioPostmixCB callback)
{
  static_assert(false, "Not implemented");
}

inline void AudioDevice::SetPostmixCallback(AudioPostmixCallback callback,
                                            void* userdata)
{
  SDL::SetAudioPostmixCallback(m_resource, callback, userdata);
}

inline void AudioDevice::SetPostmixCallback(AudioPostmixCB callback)
{
  SDL::SetAudioPostmixCallback(m_resource, callback);
}

/**
 * Load the audio data of a WAVE file into memory.
 *
 * Loading a WAVE file requires `src`, `spec`, `audio_buf` and `audio_len` to be
 * valid pointers. The entire data portion of the file is then loaded into
 * memory and decoded if necessary.
 *
 * Supported formats are RIFF WAVE files with the formats PCM (8, 16, 24, and 32
 * bits), IEEE Float (32 bits), Microsoft ADPCM and IMA ADPCM (4 bits), and
 * A-law and mu-law (8 bits). Other formats are currently unsupported and cause
 * an error.
 *
 * If this function succeeds, the return value is zero and the pointer to the
 * audio data allocated by the function is written to `audio_buf` and its length
 * in bytes to `audio_len`. The AudioSpec members `freq`, `channels`, and
 * `format` are set to the values of the audio data in the buffer.
 *
 * It's necessary to use free() to free the audio data returned in `audio_buf`
 * when it is no longer used.
 *
 * Because of the underspecification of the .WAV format, there are many
 * problematic files in the wild that cause issues with strict decoders. To
 * provide compatibility with these files, this decoder is lenient in regards to
 * the truncation of the file, the fact chunk, and the size of the RIFF chunk.
 * The hints `SDL_HINT_WAVE_RIFF_CHUNK_SIZE`, `SDL_HINT_WAVE_TRUNCATION`, and
 * `SDL_HINT_WAVE_FACT_CHUNK` can be used to tune the behavior of the loading
 * process.
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
 * ```c
 * LoadWAV(IOStream.FromFile("sample.wav", "rb"), true, &spec, &buf, &len);
 * ```
 *
 * Note that the LoadWAV function does this same thing for you, but in a less
 * messy way:
 *
 * ```c
 * LoadWAV("sample.wav", &spec, &buf, &len);
 * ```
 *
 * @param src the data source for the WAVE data.
 * @param closeio if true, calls IOStream.Close() on `src` before returning,
 *                even in the case of an error.
 * @param spec a pointer to an AudioSpec that will be set to the WAVE data's
 *             format details on successful return.
 * @param audio_buf a pointer filled with the audio data, allocated by the
 *                  function.
 * @param audio_len a pointer filled with the length of the audio data buffer in
 *                  bytes.
 * @throws Error on failure.
 *
 * This function returns false if the .WAV file cannot be opened, uses an
 * unknown data format, or is corrupt; call GetError() for more information.
 *
 * When the application is done with the data returned in `audio_buf`, it should
 * call free() to dispose of it.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa free
 * @sa LoadWAV
 */
inline OwnArray<Uint8> LoadWAV(IOStreamParam src,
                               AudioSpec* spec,
                               bool closeio = false)
{
  return CheckError(SDL_LoadWAV_IO(src, spec, closeio));
}

/**
 * Loads a WAV from a file path.
 *
 * This is a convenience function that is effectively the same as:
 *
 * ```c
 * LoadWAV(IOStream.FromFile(path, "rb"), true, spec, audio_buf, audio_len);
 * ```
 *
 * @param path the file path of the WAV file to open.
 * @param spec a pointer to an AudioSpec that will be set to the WAVE data's
 *             format details on successful return.
 * @param audio_buf a pointer filled with the audio data, allocated by the
 *                  function.
 * @param audio_len a pointer filled with the length of the audio data buffer in
 *                  bytes.
 * @throws Error on failure.
 *
 * This function returns false if the .WAV file cannot be opened, uses an
 * unknown data format, or is corrupt; call GetError() for more information.
 *
 * When the application is done with the data returned in `audio_buf`, it should
 * call free() to dispose of it.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 *
 * @sa free
 * @sa LoadWAV
 */
inline OwnArray<Uint8> LoadWAV(StringParam path, AudioSpec* spec)
{
  return CheckError(SDL_LoadWAV(path, spec));
}

/**
 * Mix audio data in a specified format.
 *
 * This takes an audio buffer `src` of `len` bytes of `format` data and mixes it
 * into `dst`, performing addition, volume adjustment, and overflow clipping.
 * The buffer pointed to by `dst` must also be `len` bytes of `format` data.
 *
 * This is provided for convenience -- you can mix your own audio data.
 *
 * Do not use this function for mixing together more than two streams of sample
 * data. The output from repeated application of this function may be distorted
 * by clipping, because there is no accumulator with greater range than the
 * input (not to mention this being an inefficient way of doing it).
 *
 * It is a common misconception that this function is required to write audio
 * data to an output stream in an audio callback. While you can do that,
 * MixAudio() is really only needed when you're mixing a single audio stream
 * with a volume adjustment.
 *
 * @param dst the destination for the mixed audio.
 * @param src the source audio buffer to be mixed.
 * @param format the AudioFormat structure representing the desired audio
 *               format.
 * @param len the length of the audio buffer in bytes.
 * @param volume ranges from 0.0 - 1.0, and should be set to 1.0 for full audio
 *               volume.
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
  CheckError(SDL_MixAudio(dst, src.data(), src.size_bytes(), format, volume));
}

/**
 * Mix audio data in a specified format.
 *
 * This takes an audio buffer `src` of `len` bytes of `format` data and mixes it
 * into `dst`, performing addition, volume adjustment, and overflow clipping.
 * The buffer pointed to by `dst` must also be `len` bytes of `format` data.
 *
 * This is provided for convenience -- you can mix your own audio data.
 *
 * Do not use this function for mixing together more than two streams of sample
 * data. The output from repeated application of this function may be distorted
 * by clipping, because there is no accumulator with greater range than the
 * input (not to mention this being an inefficient way of doing it).
 *
 * It is a common misconception that this function is required to write audio
 * data to an output stream in an audio callback. While you can do that,
 * MixAudio() is really only needed when you're mixing a single audio stream
 * with a volume adjustment.
 *
 * @param dst the destination for the mixed audio.
 * @param src the source audio buffer to be mixed.
 * @param format the AudioFormat structure representing the desired audio
 *               format.
 * @param len the length of the audio buffer in bytes.
 * @param volume ranges from 0.0 - 1.0, and should be set to 1.0 for full audio
 *               volume.
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
  static_assert(false, "Not implemented");
}

/**
 * Convert some audio data of one format to another format.
 *
 * Please note that this function is for convenience, but should not be used to
 * resample audio in blocks, as it will introduce audio artifacts on the
 * boundaries. You should only use this function if you are converting audio
 * data in its entirety in one call. If you want to convert audio in smaller
 * chunks, use an AudioStream, which is designed for this situation.
 *
 * Internally, this function creates and destroys an AudioStream on each use, so
 * it's also less efficient than using one directly, if you need to convert
 * multiple times.
 *
 * @param src_spec the format details of the input audio.
 * @param src_data the audio data to be converted.
 * @param src_len the len of src_data.
 * @param dst_spec the format details of the output audio.
 * @param dst_data will be filled with a pointer to converted audio data, which
 *                 should be freed with free(). On error, it will be nullptr.
 * @param dst_len will be filled with the len of dst_data.
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
  return CheckError(SDL_ConvertAudioSamples(
    &src_spec, src_data.data(), src_data.size_bytes(), &dst_spec));
}

/**
 * Get the human readable name of an audio format.
 *
 * @param format the audio format to query.
 * @returns the human readable name of the specified audio format or
 *          "AUDIO_UNKNOWN" if the format isn't recognized.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline const char* GetAudioFormatName(AudioFormatRaw format)
{
  return SDL_GetAudioFormatName(format);
}

inline const char* AudioFormat::GetName() const
{
  return SDL::GetAudioFormatName(m_audioFormat);
}

/**
 * Get the appropriate memset value for silencing an audio format.
 *
 * The value returned by this function can be used as the second argument to
 * memset (or memset) to set an audio buffer in a specific format to silence.
 *
 * @param format the audio data format to query.
 * @returns a byte value that can be passed to memset.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL 3.2.0.
 */
inline int GetSilenceValueForFormat(AudioFormatRaw format)
{
  return SDL_GetSilenceValueForFormat(format);
}

inline int AudioFormat::GetSilenceValue() const
{
  return SDL::GetSilenceValueForFormat(m_audioFormat);
}

/// @}

} // namespace SDL

#endif /* SDL3PP_AUDIO_H_ */
