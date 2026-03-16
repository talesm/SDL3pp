#ifndef SDL3PP_MIXER_H_
#define SDL3PP_MIXER_H_

#include "SDL3pp_audio.h"
#include "SDL3pp_version.h"

#if defined(SDL3PP_ENABLE_MIXER) || defined(SDL3PP_DOC)

#include <SDL3_mixer/SDL_mixer.h>

namespace SDL {

/**
 * @defgroup CategorySDLMixer Category SDLMixer
 *
 * SDL_mixer is a library to make complicated audio processing tasks easier.
 *
 * It offers audio file decoding, mixing multiple sounds together, basic 3D
 * positional audio, and various audio effects.
 *
 * It can mix sound to multiple audio devices in real time, or generate mixed
 * audio data to a memory buffer for any other use. It can do both at the same
 * time!
 *
 * To use the library, first call MIX.Init(). Then create a mixer with
 * Mixer.Mixer() (or Mixer.Mixer() to render to memory).
 *
 * Once you have a mixer, you can load sound data with Audio.Audio(). Data gets
 * loaded once and can be played over and over.
 *
 * When loading audio, SDL_mixer can parse out several metadata tag formats,
 * such as ID3 and APE tags, and exposes this information through the
 * Audio.GetProperties() function.
 *
 * To play audio, you create a track with Track.Track(). You need one track for
 * each sound that will be played simultaneously; think of tracks as individual
 * sliders on a mixer board. You might have loaded hundreds of audio files, but
 * you probably only have a handful of tracks that you assign those loaded files
 * to when they are ready to play, and reuse those tracks with different audio
 * later. Tracks take their input from a Audio (static data to be played
 * multiple times) or an AudioStream (streaming PCM audio the app supplies,
 * possibly as needed). A third option is to supply an IOStream, to load and
 * decode on the fly, which might be more efficient for background music that is
 * only used once, etc.
 *
 * Assign input to a Track with Track.SetAudio(), Track.SetAudioStream(), or
 * Track.SetIOStream().
 *
 * Once a track has an input, start it playing with Track.Play(). There are many
 * options to this function to dictate mixing features: looping, fades, etc.
 *
 * Tracks can be tagged with arbitrary strings, like "music" or "ingame" or
 * "ui". These tags can be used to start, stop, and pause a selection of tracks
 * at the same moment.
 *
 * All significant portions of the mixing pipeline have callbacks, so that an
 * app can hook in to the appropriate locations to examine or modify audio data
 * as it passes through the mixer: a "raw" callback for raw PCM data decoded
 * from an audio file without any modifications, a "cooked" callback for that
 * same data after all transformations (fade, positioning, etc) have been
 * processed, a "stopped" callback for when the track finishes mixing, a
 * "postmix" callback for the final mixed audio about to be sent to the audio
 * hardware to play. Additionally, you can use Group objects to mix a subset of
 * playing tracks and access the data before it is mixed in with other tracks.
 * All of this is optional, but allows for powerful access and control of the
 * mixing process.
 *
 * SDL_mixer can also be used for decoding audio files without actually
 * rendering a mix. This is done with AudioDecoder. Even though SDL_mixer
 * handles decoding transparently when used as the audio engine for an app, and
 * probably won't need this interface in that normal scenario, this can be
 * useful when using a different audio library to access many file formats.
 *
 * This library offers several features on top of mixing sounds together: a
 * track can have its own gain, to adjust its volume, in addition to a master
 * gain applied as well. One can set the "frequency ratio" of a track or the
 * final mixed output, to speed it up or slow it down, which also adjusts its
 * pitch. A channel map can also be applied per-track, to change what speaker a
 * given channel of audio is output to.
 *
 * Almost all timing in SDL_mixer is in _sample frames_. Stereo PCM audio data
 * in Sint16 format takes 4 bytes per sample frame (2 bytes per sample times 2
 * channels), for example. This allows everything in SDL_mixer to run at
 * sample-perfect accuracy, and it lets it run without concern for wall clock
 * time--you can produce audio faster than real-time, if desired. The problem,
 * though, is different pieces of audio at different _sample rates_ will produce
 * a different number of sample frames for the same length of time. To deal with
 * this, conversion routines are offered: Track.MSToFrames(),
 * Track.FramesToMS(), etc. Functions that operate on multiple tracks at once
 * will deal with time in milliseconds, so it can do these conversions
 * internally; be sure to read the documentation for these small quirks!
 *
 * SDL_mixer offers basic positional audio: a simple 3D positioning API through
 * Track.Set3DPosition() and Track.SetStereo(). The former will do simple
 * distance attenuation and spatialization--on a stereo setup, you will hear
 * sounds move from left to right--and on a surround-sound configuration,
 * individual tracks can move around the user. The latter, Track.SetStereo(),
 * will force a sound to the Front Left and Front Right speakers and let the app
 * pan it left and right as desired. Either effect can be useful for different
 * situations. SDL_mixer is not meant to be a full 3D audio engine, but rather
 * Good Enough for many purposes; if something more powerful in terms of 3D
 * audio is needed, consider a proper 3D library like OpenAL.
 *
 * @{
 */

// Forward decl
struct Mixer;

/// Alias to raw representation for Mixer.
using MixerRaw = MIX_Mixer*;

// Forward decl
struct MixerRef;

// Forward decl
struct Audio;

/// Alias to raw representation for Audio.
using AudioRaw = MIX_Audio*;

// Forward decl
struct AudioRef;

// Forward decl
struct Track;

/// Alias to raw representation for Track.
using TrackRaw = MIX_Track*;

// Forward decl
struct TrackRef;

// Forward decl
struct Group;

/// Alias to raw representation for Group.
using GroupRaw = MIX_Group*;

// Forward decl
struct GroupRef;

// Forward decl
struct AudioDecoder;

/// Alias to raw representation for AudioDecoder.
using AudioDecoderRaw = MIX_AudioDecoder*;

// Forward decl
struct AudioDecoderRef;

/**
 * A callback that fires when all mixing has completed.
 *
 * This callback is fired when the mixer has completed all its work. If this
 * mixer was created with Mixer.Mixer(), the data provided by this callback is
 * what is being sent to the audio hardware, minus last conversions for format
 * requirements. If this mixer was created with Mixer.Mixer(), this is what is
 * being output from Mixer.Generate(), after final conversions.
 *
 * The audio data passed through here is _not_ const data; the app is permitted
 * to change it in any way it likes, and those changes will replace the final
 * mixer pipeline output.
 *
 * An audiospec is provided. SDL_mixer always does its mixing work in 32-bit
 * float samples, even if the inputs or final output are not floating point. As
 * such, `spec->format` will always be `AUDIO_F32` and `pcm` hardcoded to be a
 * float pointer.
 *
 * `samples` is the number of float values pointed to by `pcm`: samples, not
 * sample frames! There are no promises how many samples will be provided
 * per-callback, and this number can vary wildly from call to call, depending on
 * many factors.
 *
 * @param userdata an opaque pointer provided by the app for its personal use.
 * @param mixer the mixer that is generating audio.
 * @param spec the format of the data in `pcm`.
 * @param pcm the raw PCM data in float32 format.
 * @param samples the number of float values pointed to by `pcm`.
 *
 * @since This datatype is available since SDL_mixer 3.0.0.
 *
 * @sa Mixer.SetPostMixCallback
 */
using PostMixCallback = void(SDLCALL*)(void* userdata,
                                       MixerRaw mixer,
                                       const AudioSpec* spec,
                                       float* pcm,
                                       int samples);

/**
 * An opaque object that represents a mixer.
 *
 * The Mixer is the toplevel object for this library. To use SDL_mixer, you must
 * have at least one, but are allowed to have several. Each mixer is responsible
 * for generating a single output stream of mixed audio, usually to an audio
 * device for realtime playback.
 *
 * Mixers are either created to feed an audio device (through Mixer.Mixer()), or
 * to generate audio to a buffer in memory, where it can be used for anything
 * (through Mixer.Mixer()).
 *
 * @since This datatype is available since SDL_mixer 3.0.0.
 *
 * @cat resource
 */
class Mixer
{
  MixerRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Mixer(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from MixerRef.
   *
   * @param resource a MixerRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Mixer(const MixerRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr Mixer(const Mixer& other) noexcept = default;

public:
  /// Move constructor
  constexpr Mixer(Mixer&& other) noexcept
    : Mixer(other.release())
  {
  }

  constexpr Mixer(const MixerRef& other) = delete;

  constexpr Mixer(MixerRef&& other) = delete;

  /**
   * Create a mixer that plays sound directly to an audio device.
   *
   * This is usually the function you want, vs Mixer.Mixer().
   *
   * You can choose a specific device ID to open, following SDL's usual rules,
   * but often the correct choice is to specify AUDIO_DEVICE_DEFAULT_PLAYBACK
   * and let SDL figure out what device to use (and seamlessly transition you to
   * new hardware if the default changes).
   *
   * Only playback devices make sense here. Attempting to open a recording
   * device will fail.
   *
   * This will call Init(INIT_AUDIO) internally; it's safe to call Init() before
   * this call, too, if you intend to enumerate audio devices to choose one to
   * open here.
   *
   * An audio format can be requested, and the system will try to set the
   * hardware to those specifications, or as close as possible, but this is just
   * a hint. SDL_mixer will handle all data conversion behind the scenes in any
   * case, and specifying a nullptr spec is a reasonable choice. The best reason
   * to specify a format is because you know all your data is in that format and
   * it might save some unnecessary CPU time on conversion.
   *
   * The actual device format chosen is available through Mixer.GetFormat().
   *
   * Once a mixer is created, next steps are usually to load audio (through
   * Audio.Audio() and friends), create a track (Track.Track()), and play that
   * audio through that track.
   *
   * When done with the mixer, it can be destroyed with Mixer.Destroy().
   *
   * @param devid the device to open for playback, or
   *              AUDIO_DEVICE_DEFAULT_PLAYBACK for the default.
   * @param spec the audio format to request from the device. May be nullptr.
   * @post a mixer that can be used to play audio on success.
   * @throws Error on failure.
   *
   * @threadsafety This function should only be called on the main thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Mixer.Mixer
   * @sa Mixer.Destroy
   */
  Mixer(AudioDeviceRef devid, const AudioSpec& spec);

  /**
   * Create a mixer that generates audio to a memory buffer.
   *
   * Usually you want Mixer.Mixer() instead of this function. The mixer created
   * here can be used with Mixer.Generate() to produce more data on demand, as
   * fast as desired.
   *
   * An audio format must be specified. This is the format it will output in.
   * This cannot be nullptr.
   *
   * Once a mixer is created, next steps are usually to load audio (through
   * Audio.Audio() and friends), create a track (Track.Track()), and play that
   * audio through that track.
   *
   * When done with the mixer, it can be destroyed with Mixer.Destroy().
   *
   * @param spec the audio format that mixer will generate.
   * @post a mixer that can be used to generate audio on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Mixer.Mixer
   * @sa Mixer.Destroy
   */
  Mixer(const AudioSpec& spec);

  /// Destructor
  ~Mixer() { MIX_DestroyMixer(m_resource); }

  /// Assignment operator.
  constexpr Mixer& operator=(Mixer&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Mixer& operator=(const Mixer& other) noexcept = default;

public:
  /// Retrieves underlying MixerRaw.
  constexpr MixerRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying MixerRaw and clear this.
  constexpr MixerRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Mixer& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /**
   * Free a mixer.
   *
   * If this mixer was created with Mixer.Mixer(), this function will also close
   * the audio device and call QuitSubSystem(INIT_AUDIO).
   *
   * Any Group or Track created for this mixer will also be destroyed. Do not
   * access them again or attempt to destroy them after the device is destroyed.
   * Audio objects will not be destroyed, since they can be shared between
   * mixers (but those will all be destroyed during MIX.Quit()).
   *
   * @threadsafety If this is used with a Mixer from Mixer.Mixer, then this
   *               function should only be called on the main thread. If this is
   *               used with a Mixer from Mixer.Mixer, then it is safe to call
   *               this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Mixer.Mixer
   * @sa Mixer.Mixer
   */
  void Destroy();

  /**
   * Get the properties associated with a mixer.
   *
   * The following read-only properties are provided by SDL_mixer:
   *
   * - `prop::Mixer.DEVICE_NUMBER`: the AudioDevice that this mixer has opened
   *   for playback. This will be zero (no device) if the mixer was created with
   *   Mix_CreateMixer() instead of Mix_CreateMixerDevice().
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  PropertiesRef GetProperties();

  /**
   * Get the audio format a mixer is generating.
   *
   * Generally you don't need this information, as SDL_mixer will convert data
   * as necessary between inputs you provide and its output format, but it might
   * be useful if trying to match your inputs to reduce conversion and
   * resampling costs.
   *
   * For mixers created with Mixer.Mixer(), this is the format of the audio
   * device (and may change later if the device itself changes; SDL_mixer will
   * seamlessly handle this change internally, though).
   *
   * For mixers created with Mixer.Mixer(), this is the format that
   * Mixer.Generate() will produce, as requested at create time, and does not
   * change.
   *
   * Note that internally, SDL_mixer will work in AUDIO_F32 format before
   * outputting the format specified here, so it would be more efficient to
   * match input data to that, not the final output format.
   *
   * @param spec where to store the mixer audio format.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  void GetFormat(AudioSpec* spec);

  /**
   * Lock a mixer by obtaining its internal mutex.
   *
   * While locked, the mixer will not be able to mix more audio or change its
   * internal state in another thread. Those other threads will block until the
   * mixer is unlocked again.
   *
   * Under the hood, this function calls Mutex.Lock(), so all the same rules
   * apply: the lock can be recursive, it must be unlocked the same number of
   * times from the same thread that locked it, etc.
   *
   * Just about every SDL_mixer API _also_ locks the mixer while doing its work,
   * as does the SDL audio device thread while actual mixing is in progress, so
   * basic use of this library never requires the app to explicitly lock the
   * device to be thread safe. There are two scenarios where this can be useful,
   * however:
   *
   * - The app has a provided a callback that the mixing thread might call, and
   *   there is some app state that needs to be protected against race
   *   conditions as changes are made and mixing progresses simultaneously. Any
   *   lock can be used for this, but this is a conveniently-available lock.
   * - The app wants to make multiple, atomic changes to the mix. For example,
   *   to start several tracks at the exact same moment, one would lock the
   *   mixer, call Track.Play multiple times, and then unlock again; all the
   *   tracks will start mixing on the same sample frame.
   *
   * Each call to this function must be paired with a call to Mixer.Unlock from
   * the same thread. It is safe to lock a mixer multiple times; it remains
   * locked until the final matching unlock call.
   *
   * Do not lock the mixer for significant amounts of time, or it can cause
   * audio dropouts. Just do simply things quickly and unlock again.
   *
   * Locking a nullptr mixer is a safe no-op.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Mixer.Unlock
   */
  void Lock();

  /**
   * Unlock a mixer previously locked by a call to Mixer.Lock().
   *
   * While locked, the mixer will not be able to mix more audio or change its
   * internal state another thread. Those other threads will block until the
   * mixer is unlocked again.
   *
   * Under the hood, this function calls Mutex.Lock(), so all the same rules
   * apply: the lock can be recursive, it must be unlocked the same number of
   * times from the same thread that locked it, etc.
   *
   * Unlocking a nullptr mixer is a safe no-op.
   *
   * @threadsafety This call must be paired with a previous Mixer.Lock call on
   *               the same thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Mixer.Lock
   */
  void Unlock();

  /**
   * Load audio for playback from an IOStream.
   *
   * In normal usage, apps should load audio once, maybe at startup, then play
   * it multiple times.
   *
   * When loading audio, it will be cached fully in RAM in its original data
   * format. Each time it plays, the data will be decoded. For example, an MP3
   * will be stored in memory in MP3 format and be decompressed on the fly
   * during playback. This is a tradeoff between i/o overhead and memory usage.
   *
   * If `predecode` is true, the data will be decompressed during load and
   * stored as raw PCM data. This might dramatically increase loading time and
   * memory usage, but there will be no need to decompress data during playback.
   *
   * (One could also use Track.SetIOStream() to bypass loading the data into RAM
   * upfront at all, but this offers still different tradeoffs. The correct
   * approach depends on the app's needs and employing different approaches in
   * different situations can make sense.)
   *
   * Audio objects can be shared between mixers. This function takes a Mixer, to
   * imply this is the most likely place it will be used and loading should try
   * to match its audio format, but the resulting audio can be used elsewhere.
   * If `mixer` is nullptr, SDL_mixer will set reasonable defaults.
   *
   * Once a Audio is created, it can be assigned to a Track with
   * Track.SetAudio(), or played without any management with Mixer.PlayAudio().
   *
   * When done with a Audio, it can be freed with Audio.Destroy().
   *
   * This function loads data from an IOStream. There is also a version that
   * loads from a path on the filesystem (Audio.Audio()), and one that accepts
   * properties for ultimate control (LoadAudioWithProperties()).
   *
   * The IOStream provided must be able to seek, or loading will fail. If the
   * stream can't seek (data is coming from an HTTP connection, etc), consider
   * caching the data to memory or disk first and creating a new stream to read
   * from there.
   *
   * @param io the IOStream to load data from.
   * @param predecode if true, data will be fully uncompressed before returning.
   * @param closeio true if SDL_mixer should close `io` before returning
   *                (success or failure).
   * @returns an audio object that can be used to make sound on a mixer, or
   *          nullptr on failure; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.Destroy
   * @sa Track.SetAudio
   * @sa Audio.Audio
   */
  Audio LoadAudio_IO(IOStreamRef io, bool predecode, bool closeio = false);

  /**
   * Load audio for playback from a file.
   *
   * This is equivalent to calling:
   *
   * ```cpp
   * Audio audio(mixer, IOStream.FromFile(path, "rb"), predecode, true);
   * ```
   *
   * This function loads data from a path on the filesystem. There is also a
   * version that loads from an IOStream (Audio.Audio()), and one that accepts
   * properties for ultimate control (LoadAudioWithProperties()).
   *
   * @param path the path on the filesystem to load data from.
   * @param predecode if true, data will be fully uncompressed before returning.
   * @returns an audio object that can be used to make sound on a mixer, or
   *          nullptr on failure; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.Destroy
   * @sa Track.SetAudio
   * @sa Audio.Audio
   */
  Audio LoadAudio(StringParam path, bool predecode);

  /**
   * Load audio for playback from a memory buffer without making a copy.
   *
   * When loading audio through most other LoadAudio functions, the data will be
   * cached fully in RAM in its original data format, for decoding on-demand.
   * This function does most of the same work as those functions, but instead
   * uses a buffer of memory provided by the app that it does not make a copy
   * of.
   *
   * This buffer must live for the entire time the returned Audio lives, as the
   * mixer will access the buffer whenever it needs to mix more data.
   *
   * This function is meant to maximize efficiency: if the data is already in
   * memory and can remain there, don't copy it. This data can be in any
   * supported audio file format (WAV, MP3, etc); it will be decoded on the fly
   * while mixing. Unlike Mixer.LoadAudio(), there is no `predecode` option
   * offered here, as this is meant to optimize for data that's already in
   * memory and intends to exist there for significant time; since predecoding
   * would only need the file format data once, upfront, one could simply wrap
   * it in SDL_CreateIOFromConstMem() and pass that to Mixer.LoadAudio_IO().
   *
   * Audio objects can be shared between multiple mixers. The `mixer` parameter
   * just suggests the most likely mixer to use this audio, in case some
   * optimization might be applied, but this is not required, and a nullptr
   * mixer may be specified.
   *
   * If `free_when_done` is true, SDL_mixer will call `free(data)` when the
   * returned Audio is eventually destroyed. This can be useful when the data is
   * not static, but rather loaded elsewhere for this specific Audio and simply
   * wants to avoid the extra copy.
   *
   * As audio format information is obtained from the file format metadata, this
   * isn't useful for raw PCM data; in that case, use Mixer.LoadRawAudioNoCopy()
   * instead, which offers an AudioSpec.
   *
   * Once a Audio is created, it can be assigned to a Track with
   * Track.SetAudio(), or played without any management with Mixer.PlayAudio().
   *
   * When done with a Audio, it can be freed with Audio.Destroy().
   *
   * @param data the buffer where the audio data lives.
   * @param datalen the size, in bytes, of the buffer.
   * @param free_when_done if true, `data` will be given to free() when the
   *                       Audio is destroyed.
   * @returns an audio object that can be used to make sound on a mixer on
   *          success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.Destroy
   * @sa Track.SetAudio
   * @sa Mixer.LoadRawAudioNoCopy
   * @sa Audio.Audio
   */
  Audio LoadAudioNoCopy(SourceBytes data, bool free_when_done);

  /**
   * Load raw PCM data from an IOStream.
   *
   * There are other options for _streaming_ raw PCM: an AudioStream can be
   * connected to a track, as can an IOStream, and will read from those sources
   * on-demand when it is time to mix the audio. This function is useful for
   * loading static audio data that is meant to be played multiple times.
   *
   * This function will load the raw data in its entirety and cache it in RAM.
   *
   * Audio objects can be shared between multiple mixers. The `mixer` parameter
   * just suggests the most likely mixer to use this audio, in case some
   * optimization might be applied, but this is not required, and a nullptr
   * mixer may be specified.
   *
   * @param io the IOStream to load data from.
   * @param spec what format the raw data is in.
   * @param closeio true if SDL_mixer should close `io` before returning
   *                (success or failure).
   * @returns an audio object that can be used to make sound on a mixer on
   *          success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.Destroy
   * @sa Track.SetAudio
   * @sa Audio.Audio
   * @sa Mixer.LoadRawAudioNoCopy
   * @sa Mixer.LoadAudio_IO
   */
  Audio LoadRawAudio_IO(IOStreamRef io,
                        const AudioSpec& spec,
                        bool closeio = false);

  /**
   * Load raw PCM data from a memory buffer.
   *
   * There are other options for _streaming_ raw PCM: an AudioStream can be
   * connected to a track, as can an IOStream, and will read from those sources
   * on-demand when it is time to mix the audio. This function is useful for
   * loading static audio data that is meant to be played multiple times.
   *
   * This function will load the raw data in its entirety and cache it in RAM,
   * allocating a copy. If the original data will outlive the created Audio, you
   * can use Mixer.LoadRawAudioNoCopy() to avoid extra allocations and copies.
   *
   * Audio objects can be shared between multiple mixers. The `mixer` parameter
   * just suggests the most likely mixer to use this audio, in case some
   * optimization might be applied, but this is not required, and a nullptr
   * mixer may be specified.
   *
   * @param data the raw PCM data to load.
   * @param datalen the size, in bytes, of the raw PCM data.
   * @param spec what format the raw data is in.
   * @returns an audio object that can be used to make sound on a mixer on
   *          success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.Destroy
   * @sa Track.SetAudio
   * @sa Audio.Audio
   * @sa Mixer.LoadRawAudioNoCopy
   * @sa Mixer.LoadAudio_IO
   */
  Audio LoadRawAudio(SourceBytes data, const AudioSpec& spec);

  /**
   * Load raw PCM data from a memory buffer without making a copy.
   *
   * This buffer must live for the entire time the returned Audio lives, as the
   * mixer will access the buffer whenever it needs to mix more data.
   *
   * This function is meant to maximize efficiency: if the data is already in
   * memory and can remain there, don't copy it. But it can also lead to some
   * interesting tricks, like changing the buffer's contents to alter multiple
   * playing tracks at once. (But, of course, be careful when being too clever.)
   *
   * Audio objects can be shared between multiple mixers. The `mixer` parameter
   * just suggests the most likely mixer to use this audio, in case some
   * optimization might be applied, but this is not required, and a nullptr
   * mixer may be specified.
   *
   * If `free_when_done` is true, SDL_mixer will call `free(data)` when the
   * returned Audio is eventually destroyed. This can be useful when the data is
   * not static, but rather composed dynamically for this specific Audio and
   * simply wants to avoid the extra copy.
   *
   * @param data the buffer where the raw PCM data lives.
   * @param datalen the size, in bytes, of the buffer.
   * @param spec what format the raw data is in.
   * @param free_when_done if true, `data` will be given to free() when the
   *                       Audio is destroyed.
   * @returns an audio object that can be used to make sound on a mixer on
   *          success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.Destroy
   * @sa Track.SetAudio
   * @sa Audio.Audio
   * @sa Audio.Audio
   * @sa Mixer.LoadAudio_IO
   */
  Audio LoadRawAudioNoCopy(SourceBytes data,
                           const AudioSpec& spec,
                           bool free_when_done);

  /**
   * Create a Audio that generates a sinewave.
   *
   * This is useful just to have _something_ to play, perhaps for testing or
   * debugging purposes.
   *
   * You specify its frequency in Hz (determines the pitch of the sinewave's
   * audio) and amplitude (determines the volume of the sinewave: 1.0f is very
   * loud, 0.0f is silent).
   *
   * A number of milliseconds of audio to generate can be specified. Specifying
   * a value less than zero will generate infinite audio (when assigned to a
   * Track, the sinewave will play forever).
   *
   * Audio objects can be shared between multiple mixers. The `mixer` parameter
   * just suggests the most likely mixer to use this audio, in case some
   * optimization might be applied, but this is not required, and a nullptr
   * mixer may be specified.
   *
   * @param hz the sinewave's frequency in Hz.
   * @param amplitude the sinewave's amplitude from 0.0f to 1.0f.
   * @param ms the maximum number of milliseconds of audio to generate, or less
   *           than zero to generate infinite audio.
   * @returns an audio object that can be used to make sound on a mixer on
   *          success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.Destroy
   * @sa Track.SetAudio
   * @sa Mixer.LoadAudio_IO
   */
  Audio CreateSineWaveAudio(int hz, float amplitude, Sint64 ms);

  /**
   * Create a new track on a mixer.
   *
   * A track provides a single source of audio. All currently-playing tracks
   * will be processed and mixed together to form the final output from the
   * mixer.
   *
   * There are no limits to the number of tracks one may create, beyond running
   * out of memory, but in normal practice there are a small number of tracks
   * that are reused between all loaded audio as appropriate.
   *
   * Tracks are unique to a specific Mixer and can't be transferred between
   * them.
   *
   * @returns a new Track on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Destroy
   */
  TrackRef CreateTrack();

  /**
   * Get all tracks with a specific tag.
   *
   * Tracks are not provided in any guaranteed order.
   *
   * @param tag the tag to search.
   * @param count a pointer filled in with the number of tracks returned, can be
   *              nullptr.
   * @returns an array of the tracks, nullptr-terminated on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  MIX_Track** GetTaggedTracks(StringParam tag, int* count);

  /**
   * Start (or restart) mixing all tracks with a specific tag for playback.
   *
   * This function follows all the same rules as Track.Play(); please refer to
   * its documentation for the details. Unlike that function, Mixer.PlayTag()
   * operates on multiple tracks at once that have the specified tag applied,
   * via Track.Tag().
   *
   * If all of your tagged tracks have different sample rates, it would make
   * sense to use the `*_MILLISECONDS_NUMBER` properties in your `options`,
   * instead of `*_FRAMES_NUMBER`, and let SDL_mixer figure out how to apply it
   * to each track.
   *
   * This function returns true if all tagged tracks are started (or restarted).
   * If any track fails, this function returns false, but all tracks that could
   * start will still be started even when this function reports failure.
   *
   * From the point of view of the mixing process, all tracks that successfully
   * (re)start will do so at the exact same moment.
   *
   * @param tag the tag to use when searching for tracks.
   * @param options the set of options that will be applied to each track.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Play
   * @sa Track.Tag
   * @sa Track.Stop
   * @sa Track.Pause
   * @sa Track.Playing
   */
  void PlayTag(StringParam tag, PropertiesRef options);

  /**
   * Play a Audio from start to finish without any management.
   *
   * This is what we term a "fire-and-forget" sound. Internally, SDL_mixer will
   * manage a temporary track to mix the specified Audio, cleaning it up when
   * complete. No options can be provided for how to do the mixing, like
   * Track.Play() offers, and since the track is not available to the caller, no
   * adjustments can be made to mixing over time.
   *
   * This is not the function to build an entire game of any complexity around,
   * but it can be convenient to play simple, one-off sounds that can't be
   * stopped early. An example would be a voice saying "GAME OVER" during an
   * unpausable endgame sequence.
   *
   * There are no limits to the number of fire-and-forget sounds that can mix at
   * once (short of running out of memory), and SDL_mixer keeps an internal pool
   * of temporary tracks it creates as needed and reuses when available.
   *
   * @param audio the audio input to play.
   * @returns true if the track has begun mixing, false on error; call
   *          GetError() for details.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Play
   * @sa Mixer.LoadAudio
   */
  bool PlayAudio(AudioRef audio);

  /**
   * Halt all currently-playing tracks, possibly fading out over time.
   *
   * If `fade_out_ms` is > 0, the tracks do not stop mixing immediately, but
   * rather fades to silence over that many milliseconds before stopping. Note
   * that this is different than Track.Stop(), which wants sample frames; this
   * function takes milliseconds because different tracks might have different
   * sample rates.
   *
   * If a track ends normally while the fade-out is still in progress, the audio
   * stops there; the fade is not adjusted to be shorter if it will last longer
   * than the audio remaining.
   *
   * Once a track has completed any fadeout and come to a stop, it will call its
   * TrackStoppedCallback, if any. It is legal to assign the track a new input
   * and/or restart it during this callback. This function does not prevent new
   * play requests from being made.
   *
   * @param fade_out_ms the number of milliseconds to spend fading out to
   *                    silence before halting. 0 to stop immediately.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Stop
   */
  void StopAllTracks(Sint64 fade_out_ms);

  /**
   * Halt all tracks with a specific tag, possibly fading out over time.
   *
   * If `fade_out_ms` is > 0, the tracks do not stop mixing immediately, but
   * rather fades to silence over that many milliseconds before stopping. Note
   * that this is different than Track.Stop(), which wants sample frames; this
   * function takes milliseconds because different tracks might have different
   * sample rates.
   *
   * If a track ends normally while the fade-out is still in progress, the audio
   * stops there; the fade is not adjusted to be shorter if it will last longer
   * than the audio remaining.
   *
   * Once a track has completed any fadeout and come to a stop, it will call its
   * TrackStoppedCallback, if any. It is legal to assign the track a new input
   * and/or restart it during this callback. This function does not prevent new
   * play requests from being made.
   *
   * @param tag the tag to use when searching for tracks.
   * @param fade_out_ms the number of milliseconds to spend fading out to
   *                    silence before halting. 0 to stop immediately.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Stop
   * @sa Track.Tag
   */
  void StopTag(StringParam tag, Sint64 fade_out_ms);

  /**
   * Pause all currently-playing tracks.
   *
   * A paused track is not considered "stopped," so its TrackStoppedCallback
   * will not fire if paused, but it won't change state by default, generate
   * audio, or generally make progress, until it is resumed.
   *
   * This function makes all tracks on the specified mixer that are currently
   * playing move to a paused state. They can later be resumed.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Resume
   * @sa Mixer.ResumeAllTracks
   */
  void PauseAllTracks();

  /**
   * Pause all tracks with a specific tag.
   *
   * A paused track is not considered "stopped," so its TrackStoppedCallback
   * will not fire if paused, but it won't change state by default, generate
   * audio, or generally make progress, until it is resumed.
   *
   * This function makes all currently-playing tracks on the specified mixer,
   * with a specific tag, move to a paused state. They can later be resumed.
   *
   * Tracks that match the specified tag that aren't currently playing are
   * ignored.
   *
   * @param tag the tag to use when searching for tracks.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Pause
   * @sa Track.Resume
   * @sa Mixer.ResumeTag
   * @sa Track.Tag
   */
  void PauseTag(StringParam tag);

  /**
   * Resume all currently-paused tracks.
   *
   * A paused track is not considered "stopped," so its TrackStoppedCallback
   * will not fire if paused, but it won't change state by default, generate
   * audio, or generally make progress, until it is resumed.
   *
   * This function makes all tracks on the specified mixer that are currently
   * paused move to a playing state.
   *
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Pause
   * @sa Mixer.PauseAllTracks
   */
  void ResumeAllTracks();

  /**
   * Resume all tracks with a specific tag.
   *
   * A paused track is not considered "stopped," so its TrackStoppedCallback
   * will not fire if paused, but it won't change state by default, generate
   * audio, or generally make progress, until it is resumed.
   *
   * This function makes all currently-paused tracks on the specified mixer,
   * with a specific tag, move to a playing state.
   *
   * Tracks that match the specified tag that aren't currently paused are
   * ignored.
   *
   * @param tag the tag to use when searching for tracks.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Resume
   * @sa Track.Pause
   * @sa Mixer.PauseTag
   * @sa Track.Tag
   */
  void ResumeTag(StringParam tag);

  /**
   * Set a mixer's master gain control.
   *
   * Each mixer has a master gain, to adjust the volume of the entire mix. Each
   * sample passing through the pipeline is modulated by this gain value. A gain
   * of zero will generate silence, 1.0f will not change the mixed volume, and
   * larger than 1.0f will increase the volume. Negative values are illegal.
   * There is no maximum gain specified, but this can quickly get extremely
   * loud, so please be careful with this setting.
   *
   * A mixer's master gain defaults to 1.0f.
   *
   * This value can be changed at any time to adjust the future mix.
   *
   * @param gain the new gain value.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Mixer.GetGain
   * @sa Track.SetGain
   */
  void SetGain(float gain);

  /**
   * Get a mixer's master gain control.
   *
   * This returns the last value set through Mixer.SetGain(), or 1.0f if no
   * value has ever been explicitly set.
   *
   * @returns the mixer's current master gain.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Mixer.SetGain
   * @sa Track.GetGain
   */
  float GetGain();

  /**
   * Set the gain control of all tracks with a specific tag.
   *
   * Each track has its own gain, to adjust its overall volume. Each sample from
   * this track is modulated by this gain value. A gain of zero will generate
   * silence, 1.0f will not change the mixed volume, and larger than 1.0f will
   * increase the volume. Negative values are illegal. There is no maximum gain
   * specified, but this can quickly get extremely loud, so please be careful
   * with this setting.
   *
   * A track's gain defaults to 1.0f.
   *
   * This will change the gain control on tracks on the specified mixer that
   * have the specified tag.
   *
   * From the point of view of the mixing process, all tracks that successfully
   * change gain values will do so at the exact same moment.
   *
   * This value can be changed at any time to adjust the future mix.
   *
   * @param tag the tag to use when searching for tracks.
   * @param gain the new gain value.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.GetGain
   * @sa Track.SetGain
   * @sa Mixer.SetGain
   * @sa Track.Tag
   */
  void SetTagGain(StringParam tag, float gain);

  /**
   * Set a mixer's master frequency ratio.
   *
   * Each mixer has a master frequency ratio, that affects the entire mix. This
   * can cause the final output to change speed and pitch. A value greater than
   * 1.0f will play the audio faster, and at a higher pitch. A value less than
   * 1.0f will play the audio slower, and at a lower pitch. 1.0f is normal
   * speed.
   *
   * Each track _also_ has a frequency ratio; it will be applied when mixing
   * that track's audio regardless of the master setting. The master setting
   * affects the final output after all mixing has been completed.
   *
   * A mixer's master frequency ratio defaults to 1.0f.
   *
   * This value can be changed at any time to adjust the future mix.
   *
   * @param ratio the frequency ratio. Must be between 0.01f and 100.0f.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Mixer.GetFrequencyRatio
   * @sa Track.SetFrequencyRatio
   */
  void SetFrequencyRatio(float ratio);

  /**
   * Get a mixer's master frequency ratio.
   *
   * This returns the last value set through Mixer.SetFrequencyRatio(), or 1.0f
   * if no value has ever been explicitly set.
   *
   * @returns the mixer's current master frequency ratio.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Mixer.SetFrequencyRatio
   * @sa Track.GetFrequencyRatio
   */
  float GetFrequencyRatio();

  /**
   * Create a mixing group.
   *
   * Tracks are assigned to a mixing group (or if unassigned, they live in a
   * mixer's internal default group). All tracks in a group are mixed together
   * and the app can access this mixed data before it is mixed with all other
   * groups to produce the final output.
   *
   * This can be a useful feature, but is completely optional; apps can ignore
   * mixing groups entirely and still have a full experience with SDL_mixer.
   *
   * After creating a group, assign tracks to it with Track.SetGroup(). Use
   * Group.SetPostMixCallback() to access the group's mixed data.
   *
   * A mixing group can be destroyed with Group.Destroy() when no longer needed.
   * Destroying the mixer will also destroy all its still-existing mixing
   * groups.
   *
   * @returns a newly-created mixing group on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Group.Destroy
   * @sa Track.SetGroup
   * @sa Group.SetPostMixCallback
   */
  GroupRef CreateGroup();

  /**
   * Set a callback that fires when all mixing has completed.
   *
   * After all mixer groups have processed, their buffers are mixed together
   * into a single buffer for the final output, at which point a callback can be
   * fired. This lets an app view the data at the last moment before mixing
   * completes. It can also change the data in any way it pleases during this
   * callback, and the mixer will continue as if this data is the final output.
   *
   * Each mixer has its own unique callback.
   *
   * Passing a nullptr callback here is legal; it disables this mixer's
   * callback.
   *
   * @param cb the function to call when the mixer mixes. May be nullptr.
   * @param userdata an opaque pointer provided to the callback for its own
   *                 personal use.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa PostMixCallback
   */
  void SetPostMixCallback(PostMixCallback cb, void* userdata);

  /**
   * Generate mixer output when not driving an audio device.
   *
   * SDL_mixer allows the creation of Mixer objects that are not connected to an
   * audio device, by calling Mixer.Mixer() instead of Mixer.Mixer(). Such
   * mixers will not generate output until explicitly requested through this
   * function.
   *
   * The caller may request as much audio as desired, so long as `buflen` is a
   * multiple of the sample frame size specified when creating the mixer (for
   * example, if requesting stereo Sint16 audio, buflen must be a multiple of 4:
   * 2 bytes-per-channel times 2 channels).
   *
   * The mixer will mix as quickly as possible; since it works in sample frames
   * instead of time, it can potentially generate enormous amounts of audio in a
   * small amount of time.
   *
   * On success, this always fills `buffer` with `buflen` bytes of audio; if all
   * playing tracks finish mixing, it will fill the remaining buffer with
   * silence.
   *
   * Each call to this function will pick up where it left off, playing tracks
   * will continue to mix from the point the previous call completed, etc. The
   * mixer state can be changed between each call in any way desired: tracks can
   * be added, played, stopped, changed, removed, etc. Effectively this function
   * does the same thing SDL_mixer does internally when the audio device needs
   * more audio to play.
   *
   * This function can not be used with mixers from Mixer.Mixer(); those
   * generate audio as needed internally.
   *
   * This function returns the number of _bytes_ of real audio mixed, which
   * might be less than `buflen`. While all `buflen` bytes of `buffer` will be
   * initialized, if available tracks to mix run out, the end of the buffer will
   * be initialized with silence; this silence will not be counted in the return
   * value, so the caller has the option to identify how much of the buffer has
   * legitimate contents vs appended silence. As such, any value >= 0 signifies
   * success. A return value of -1 means failure (out of memory, invalid
   * parameters, etc).
   *
   * @param buffer a pointer to a buffer to store audio in.
   * @param buflen the number of bytes to store in buffer.
   * @returns The number of bytes of mixed audio, discounting appended silence,
   *          on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Mixer.Mixer
   */
  int Generate(void* buffer, int buflen);
};

/**
 * Reference for Mixer.
 *
 * This does not take ownership!
 */
struct MixerRef : Mixer
{
  using Mixer::Mixer;

  /**
   * Constructs from raw Mixer.
   *
   * @param resource a MixerRaw.
   *
   * This does not takes ownership!
   */
  MixerRef(MixerRaw resource) noexcept
    : Mixer(resource)
  {
  }

  /**
   * Constructs from Mixer.
   *
   * @param resource a Mixer.
   *
   * This does not takes ownership!
   */
  constexpr MixerRef(const Mixer& resource) noexcept
    : Mixer(resource.get())
  {
  }

  /// Copy constructor.
  constexpr MixerRef(const MixerRef& other) noexcept
    : Mixer(other.get())
  {
  }

  /// Move constructor.
  constexpr MixerRef(MixerRef&& other) noexcept
    : Mixer(other.release())
  {
  }

  /// Destructor
  ~MixerRef() { release(); }

  /// Assignment operator.
  constexpr MixerRef& operator=(MixerRef other) noexcept
  {
    std::swap(*this, other);
    return *this;
  }

  /// Converts to MixerRaw
  constexpr operator MixerRaw() const noexcept { return get(); }
};

/**
 * An opaque object that represents audio data.
 *
 * Generally you load audio data (in whatever file format) into SDL_mixer with
 * Audio.Audio() or one of its several variants, producing a Audio object.
 *
 * A Audio represents static audio data; it could be background music, or maybe
 * a laser gun sound effect. It is loaded into RAM and can be played multiple
 * times, possibly on different tracks at the same time.
 *
 * Unlike most other objects, Audio objects can be shared between mixers.
 *
 * @since This datatype is available since SDL_mixer 3.0.0.
 *
 * @cat resource
 */
class Audio
{
  AudioRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Audio(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from AudioRef.
   *
   * @param resource a AudioRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Audio(const AudioRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr Audio(const Audio& other) noexcept = default;

public:
  /// Move constructor
  constexpr Audio(Audio&& other) noexcept
    : Audio(other.release())
  {
  }

  constexpr Audio(const AudioRef& other) = delete;

  constexpr Audio(AudioRef&& other) = delete;

  /**
   * Load audio for playback from an IOStream.
   *
   * In normal usage, apps should load audio once, maybe at startup, then play
   * it multiple times.
   *
   * When loading audio, it will be cached fully in RAM in its original data
   * format. Each time it plays, the data will be decoded. For example, an MP3
   * will be stored in memory in MP3 format and be decompressed on the fly
   * during playback. This is a tradeoff between i/o overhead and memory usage.
   *
   * If `predecode` is true, the data will be decompressed during load and
   * stored as raw PCM data. This might dramatically increase loading time and
   * memory usage, but there will be no need to decompress data during playback.
   *
   * (One could also use Track.SetIOStream() to bypass loading the data into RAM
   * upfront at all, but this offers still different tradeoffs. The correct
   * approach depends on the app's needs and employing different approaches in
   * different situations can make sense.)
   *
   * Audio objects can be shared between mixers. This function takes a Mixer, to
   * imply this is the most likely place it will be used and loading should try
   * to match its audio format, but the resulting audio can be used elsewhere.
   * If `mixer` is nullptr, SDL_mixer will set reasonable defaults.
   *
   * Once a Audio is created, it can be assigned to a Track with
   * Track.SetAudio(), or played without any management with Mixer.PlayAudio().
   *
   * When done with a Audio, it can be freed with Audio.Destroy().
   *
   * This function loads data from an IOStream. There is also a version that
   * loads from a path on the filesystem (Audio.Audio()), and one that accepts
   * properties for ultimate control (Audio.Audio()).
   *
   * The IOStream provided must be able to seek, or loading will fail. If the
   * stream can't seek (data is coming from an HTTP connection, etc), consider
   * caching the data to memory or disk first and creating a new stream to read
   * from there.
   *
   * @param mixer a mixer this audio is intended to be used with. May be
   *              nullptr.
   * @param io the IOStream to load data from.
   * @param predecode if true, data will be fully uncompressed before returning.
   * @param closeio true if SDL_mixer should close `io` before returning
   *                (success or failure).
   * @post an audio object that can be used to make sound on a mixer, or nullptr
   *       on failure; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.Destroy
   * @sa Track.SetAudio
   * @sa Audio.Audio
   * @sa Audio.Audio
   */
  Audio(MixerRef mixer, IOStreamRef io, bool predecode, bool closeio = false);

  /**
   * Load audio for playback from a file.
   *
   * This is equivalent to calling:
   *
   * ```c
   * Audio.Audio(mixer, IOStream.FromFile(path, "rb"), predecode, true);
   * ```
   *
   * This function loads data from a path on the filesystem. There is also a
   * version that loads from an IOStream (Audio.Audio()), and one that accepts
   * properties for ultimate control (Audio.Audio()).
   *
   * @param mixer a mixer this audio is intended to be used with. May be
   *              nullptr.
   * @param path the path on the filesystem to load data from.
   * @param predecode if true, data will be fully uncompressed before returning.
   * @post an audio object that can be used to make sound on a mixer, or nullptr
   *       on failure; call GetError() for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.Destroy
   * @sa Track.SetAudio
   * @sa Audio.Audio
   * @sa Audio.Audio
   */
  Audio(MixerRef mixer, StringParam path, bool predecode);

  /**
   * Load audio for playback through a collection of properties.
   *
   * Please see Audio.Audio() for a description of what the various LoadAudio
   * functions do. This function uses properties to dictate how it operates, and
   * exposes functionality the other functions don't provide.
   *
   * Properties are discussed in [SDL's
   * documentation](https://wiki.libsdl.org/SDL3/CategoryProperties) .
   *
   * These are the supported properties:
   *
   * - `prop::Audio.LOAD_IOSTREAM_POINTER`: a pointer to an IOStream to be used
   *   to load audio data. Required. This stream must be able to seek!
   * - `prop::Audio.LOAD_CLOSEIO_BOOLEAN`: true if SDL_mixer should close the
   *   IOStream before returning (success or failure).
   * - `prop::Audio.LOAD_PREDECODE_BOOLEAN`: true if SDL_mixer should fully
   *   decode and decompress the data before returning. Otherwise it will be
   *   stored in its original state and decompressed on demand.
   * - `prop::Audio.LOAD_PREFERRED_MIXER_POINTER`: a pointer to a Mixer, in case
   *   steps can be made to match its format when decoding. Optional.
   * - `prop::Audio.LOAD_SKIP_METADATA_TAGS_BOOLEAN`: true to skip parsing
   *   metadata tags, like ID3 and APE tags. This can be used to speed up
   *   loading _if the data definitely doesn't have these tags_. Some decoders
   *   will fail if these tags are present when this property is true.
   * - `prop::Audio.DECODER_STRING`: the name of the decoder to use for this
   *   data. Optional. If not specified, SDL_mixer will examine the data and
   *   choose the best decoder. These names are the same returned from
   *   GetAudioDecoder().
   *
   * Specific decoders might accept additional custom properties, such as where
   * to find soundfonts for MIDI playback, etc.
   *
   * @param props a set of properties on how to load audio.
   * @post an audio object that can be used to make sound on a mixer on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.Destroy
   * @sa Track.SetAudio
   * @sa Audio.Audio
   * @sa Audio.Audio
   */
  Audio(PropertiesRef props);

  /**
   * Load raw PCM data from an IOStream.
   *
   * There are other options for _streaming_ raw PCM: an AudioStream can be
   * connected to a track, as can an IOStream, and will read from those sources
   * on-demand when it is time to mix the audio. This function is useful for
   * loading static audio data that is meant to be played multiple times.
   *
   * This function will load the raw data in its entirety and cache it in RAM.
   *
   * Audio objects can be shared between multiple mixers. The `mixer` parameter
   * just suggests the most likely mixer to use this audio, in case some
   * optimization might be applied, but this is not required, and a nullptr
   * mixer may be specified.
   *
   * @param mixer a mixer this audio is intended to be used with. May be
   *              nullptr.
   * @param io the IOStream to load data from.
   * @param spec what format the raw data is in.
   * @param closeio true if SDL_mixer should close `io` before returning
   *                (success or failure).
   * @post an audio object that can be used to make sound on a mixer on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.Destroy
   * @sa Track.SetAudio
   * @sa Audio.Audio
   * @sa Mixer.LoadRawAudioNoCopy
   * @sa Mixer.LoadAudio_IO
   */
  Audio(MixerRef mixer,
        IOStreamRef io,
        const AudioSpec& spec,
        bool closeio = false);

  /**
   * Load raw PCM data from a memory buffer.
   *
   * There are other options for _streaming_ raw PCM: an AudioStream can be
   * connected to a track, as can an IOStream, and will read from those sources
   * on-demand when it is time to mix the audio. This function is useful for
   * loading static audio data that is meant to be played multiple times.
   *
   * This function will load the raw data in its entirety and cache it in RAM,
   * allocating a copy. If the original data will outlive the created Audio, you
   * can use Mixer.LoadRawAudioNoCopy() to avoid extra allocations and copies.
   *
   * Audio objects can be shared between multiple mixers. The `mixer` parameter
   * just suggests the most likely mixer to use this audio, in case some
   * optimization might be applied, but this is not required, and a nullptr
   * mixer may be specified.
   *
   * @param mixer a mixer this audio is intended to be used with. May be
   *              nullptr.
   * @param data the raw PCM data to load.
   * @param datalen the size, in bytes, of the raw PCM data.
   * @param spec what format the raw data is in.
   * @post an audio object that can be used to make sound on a mixer on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.Destroy
   * @sa Track.SetAudio
   * @sa Audio.Audio
   * @sa Mixer.LoadRawAudioNoCopy
   * @sa Mixer.LoadAudio_IO
   */
  Audio(MixerRef mixer, SourceBytes data, const AudioSpec& spec);

  /// Destructor
  ~Audio() { MIX_DestroyAudio(m_resource); }

  /// Assignment operator.
  constexpr Audio& operator=(Audio&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Audio& operator=(const Audio& other) noexcept = default;

public:
  /// Retrieves underlying AudioRaw.
  constexpr AudioRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying AudioRaw and clear this.
  constexpr AudioRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Audio& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /**
   * Destroy the specified audio.
   *
   * Audio is reference-counted internally, so this function only unrefs it. If
   * doing so causes the reference count to drop to zero, the Audio will be
   * deallocated. This allows the system to safely operate if the audio is still
   * assigned to a Track at the time of destruction. The actual destroying will
   * happen when the track stops using it.
   *
   * But from the caller's perspective, once this function is called, it should
   * assume the `audio` pointer has become invalid.
   *
   * Destroying a nullptr Audio is a legal no-op.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  void Destroy();

  /**
   * Get the properties associated with a Audio.
   *
   * SDL_mixer offers some properties of its own, but this can also be a
   * convenient place to store app-specific data.
   *
   * A Properties is created the first time this function is called for a given
   * Audio, if necessary.
   *
   * The following read-only properties are provided by SDL_mixer:
   *
   * - `prop::MixMetadata.TITLE_STRING`: the audio's title ("Smells Like Teen
   *   Spirit").
   * - `prop::MixMetadata.ARTIST_STRING`: the audio's artist name ("Nirvana").
   * - `prop::MixMetadata.ALBUM_STRING`: the audio's album name ("Nevermind").
   * - `prop::MixMetadata.COPYRIGHT_STRING`: the audio's copyright info
   *   ("Copyright (c) 1991")
   * - `prop::MixMetadata.TRACK_NUMBER`: the audio's track number on the album
   *   (1)
   * - `prop::MixMetadata.TOTAL_TRACKS_NUMBER`: the total tracks on the album
   *   (13)
   * - `prop::MixMetadata.YEAR_NUMBER`: the year the audio was released (1991)
   * - `prop::MixMetadata.DURATION_FRAMES_NUMBER`: The sample frames worth of
   *   PCM data that comprise this audio. It might be off by a little if the
   *   decoder only knows the duration as a unit of time.
   * - `prop::MixMetadata.DURATION_INFINITE_BOOLEAN`: if true, audio never runs
   *   out of sound to generate. This isn't necessarily always known to
   *   SDL_mixer, though.
   *
   * Other properties, documented with Audio.Audio(), may also be present.
   *
   * Note that the metadata properties are whatever SDL_mixer finds in things
   * like ID3 tags, and they often have very little standardized formatting, may
   * be missing, and can be completely wrong if the original data is
   * untrustworthy (like an MP3 from a P2P file sharing service).
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  PropertiesRef GetProperties();

  /**
   * Get the length of a Audio's playback in sample frames.
   *
   * This information is also available via the
   * prop::MixMetadata.DURATION_FRAMES_NUMBER property, but it's common enough
   * to provide a simple accessor function.
   *
   * This reports the length of the data in _sample frames_, so sample-perfect
   * mixing can be possible. Sample frames are only meaningful as a measure of
   * time if the sample rate (frequency) is also known. To convert from sample
   * frames to milliseconds, use Audio.FramesToMS().
   *
   * Not all audio file formats can report the complete length of the data they
   * will produce through decoding: some can't calculate it, some might produce
   * infinite audio.
   *
   * Also, some file formats can only report duration as a unit of time, which
   * means SDL_mixer might have to estimate sample frames from that information.
   * With less precision, the reported duration might be off by a few sample
   * frames in either direction.
   *
   * This will return a value >= 0 if a duration is known. It might also return
   * DURATION_UNKNOWN or DURATION_INFINITE.
   *
   * @returns the length of the audio in sample frames, or DURATION_UNKNOWN or
   *          DURATION_INFINITE.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  Sint64 GetDuration();

  /**
   * Query the initial audio format of a Audio.
   *
   * Note that some audio files can change format in the middle; some explicitly
   * support this, but a more common example is two MP3 files concatenated
   * together. In many cases, SDL_mixer will correctly handle these sort of
   * files, but this function will only report the initial format a file uses.
   *
   * @param spec on success, audio format details will be stored here.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  void GetFormat(AudioSpec* spec);

  /**
   * Convert milliseconds to sample frames for a Audio's format.
   *
   * This calculates time based on the audio's initial format, even if the
   * format would change mid-stream.
   *
   * If `ms` is < 0, this returns -1.
   *
   * @param ms the milliseconds to convert to audio-specific sample frames.
   * @returns Converted number of sample frames, or -1 for errors/no input; call
   *          GetError() for details.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.FramesToMS
   */
  Sint64 MSToFrames(Sint64 ms);

  /**
   * Convert sample frames for a Audio's format to milliseconds.
   *
   * This calculates time based on the audio's initial format, even if the
   * format would change mid-stream.
   *
   * Sample frames are more precise than milliseconds, so out of necessity, this
   * function will approximate by rounding down to the closest full millisecond.
   *
   * If `frames` is < 0, this returns -1.
   *
   * @param frames the audio-specific sample frames to convert to milliseconds.
   * @returns Converted number of milliseconds, or -1 for errors/no input; call
   *          GetError() for details.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.MSToFrames
   */
  Sint64 FramesToMS(Sint64 frames);
};

/**
 * Reference for Audio.
 *
 * This does not take ownership!
 */
struct AudioRef : Audio
{
  using Audio::Audio;

  /**
   * Constructs from raw Audio.
   *
   * @param resource a AudioRaw.
   *
   * This does not takes ownership!
   */
  AudioRef(AudioRaw resource) noexcept
    : Audio(resource)
  {
  }

  /**
   * Constructs from Audio.
   *
   * @param resource a Audio.
   *
   * This does not takes ownership!
   */
  constexpr AudioRef(const Audio& resource) noexcept
    : Audio(resource.get())
  {
  }

  /// Copy constructor.
  constexpr AudioRef(const AudioRef& other) noexcept
    : Audio(other.get())
  {
  }

  /// Move constructor.
  constexpr AudioRef(AudioRef&& other) noexcept
    : Audio(other.release())
  {
  }

  /// Destructor
  ~AudioRef() { release(); }

  /// Assignment operator.
  constexpr AudioRef& operator=(AudioRef other) noexcept
  {
    std::swap(*this, other);
    return *this;
  }

  /// Converts to AudioRaw
  constexpr operator AudioRaw() const noexcept { return get(); }
};

/**
 * A set of per-channel gains for tracks using Track.SetStereo().
 *
 * When forcing a track to stereo, the app can specify a per-channel gain, to
 * further adjust the left or right outputs.
 *
 * When mixing audio that has been forced to stereo, each channel is modulated
 * by these values. A value of 1.0f produces no change, 0.0f produces silence.
 *
 * A simple panning effect would be to set `left` to the desired value and
 * `right` to `1.0f - left`.
 *
 * @since This struct is available since SDL_mixer 3.0.0.
 *
 * @sa Track.SetStereo
 */
using StereoGains = MIX_StereoGains;

/**
 * 3D coordinates for Track.Set3DPosition.
 *
 * The coordinates use a "right-handed" coordinate system, like OpenGL and
 * OpenAL.
 *
 * @since This struct is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Set3DPosition
 */
using Point3D = MIX_Point3D;

/**
 * A callback that fires when a Track is stopped.
 *
 * This callback is fired when a track completes playback, either because it ran
 * out of data to mix (and all loops were completed as well), or it was
 * explicitly stopped by the app. Pausing a track will not fire this callback.
 *
 * It is legal to adjust the track, including changing its input and restarting
 * it. If this is done because it ran out of data in the middle of mixing, the
 * mixer will start mixing the new track state in its current run without any
 * gap in the audio.
 *
 * This callback will not fire when a playing track is destroyed.
 *
 * @param userdata an opaque pointer provided by the app for its personal use.
 * @param track the track that has stopped.
 *
 * @since This datatype is available since SDL_mixer 3.0.0.
 *
 * @sa Track.SetStoppedCallback
 */
using TrackStoppedCallback = void(SDLCALL*)(void* userdata, TrackRaw track);

/**
 * A callback that fires when a Track is mixing at various stages.
 *
 * This callback is fired for different parts of the mixing pipeline, and gives
 * the app visibility into the audio data that is being generated at various
 * stages.
 *
 * The audio data passed through here is _not_ const data; the app is permitted
 * to change it in any way it likes, and those changes will propagate through
 * the mixing pipeline.
 *
 * An audiospec is provided. Different tracks might be in different formats, and
 * an app needs to be able to handle that, but SDL_mixer always does its mixing
 * work in 32-bit float samples, even if the inputs or final output are not
 * floating point. As such, `spec->format` will always be `AUDIO_F32` and `pcm`
 * hardcoded to be a float pointer.
 *
 * `samples` is the number of float values pointed to by `pcm`: samples, not
 * sample frames! There are no promises how many samples will be provided
 * per-callback, and this number can vary wildly from call to call, depending on
 * many factors.
 *
 * Making changes to the track during this callback is undefined behavior.
 * Change the data in `pcm` but not the track itself.
 *
 * @param userdata an opaque pointer provided by the app for its personal use.
 * @param track the track that is being mixed.
 * @param spec the format of the data in `pcm`.
 * @param pcm the raw PCM data in float32 format.
 * @param samples the number of float values pointed to by `pcm`.
 *
 * @since This datatype is available since SDL_mixer 3.0.0.
 *
 * @sa Track.SetRawCallback
 * @sa Track.SetCookedCallback
 */
using TrackMixCallback = void(SDLCALL*)(void* userdata,
                                        TrackRaw track,
                                        const AudioSpec* spec,
                                        float* pcm,
                                        int samples);

/**
 * An opaque object that represents a source of sound output to be mixed.
 *
 * A Mixer has an arbitrary number of tracks, and each track manages its own
 * unique audio to be mixed together.
 *
 * Tracks also have other properties: gain, loop points, fading, 3D position,
 * and other attributes that alter the produced sound; many can be altered
 * during playback.
 *
 * @since This datatype is available since SDL_mixer 3.0.0.
 *
 * @cat resource
 */
class Track
{
  TrackRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Track(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from TrackRef.
   *
   * @param resource a TrackRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Track(const TrackRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr Track(const Track& other) noexcept = default;

public:
  /// Move constructor
  constexpr Track(Track&& other) noexcept
    : Track(other.release())
  {
  }

  constexpr Track(const TrackRef& other) = delete;

  constexpr Track(TrackRef&& other) = delete;

  /**
   * Create a new track on a mixer.
   *
   * A track provides a single source of audio. All currently-playing tracks
   * will be processed and mixed together to form the final output from the
   * mixer.
   *
   * There are no limits to the number of tracks one may create, beyond running
   * out of memory, but in normal practice there are a small number of tracks
   * that are reused between all loaded audio as appropriate.
   *
   * Tracks are unique to a specific Mixer and can't be transferred between
   * them.
   *
   * @param mixer the mixer on which to create this track.
   * @post a new Track on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Destroy
   */
  Track(MixerRef mixer);

  /// Destructor
  ~Track() { MIX_DestroyTrack(m_resource); }

  /// Assignment operator.
  constexpr Track& operator=(Track&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Track& operator=(const Track& other) noexcept = default;

public:
  /// Retrieves underlying TrackRaw.
  constexpr TrackRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying TrackRaw and clear this.
  constexpr TrackRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Track& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /**
   * Destroy the specified track.
   *
   * If the track is currently playing, it will be stopped immediately, without
   * any fadeout. If there is a callback set through Track.SetStoppedCallback(),
   * it will _not_ be called.
   *
   * If the mixer is currently mixing in another thread, this will block until
   * it finishes.
   *
   * Destroying a nullptr Track is a legal no-op.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  void Destroy();

  /**
   * Get the properties associated with a track.
   *
   * Currently SDL_mixer assigns no properties of its own to a track, but this
   * can be a convenient place to store app-specific data.
   *
   * A Properties is created the first time this function is called for a given
   * track.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  PropertiesRef GetProperties();

  /**
   * Get the Mixer that owns a Track.
   *
   * This is the mixer pointer that was passed to Track.Track().
   *
   * @returns the mixer associated with the track on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  MixerRef GetMixer();

  /**
   * Set a Track's input to a Audio.
   *
   * A Audio is audio data stored in RAM (possibly still in a compressed form).
   * One Audio can be assigned to multiple tracks at once.
   *
   * Once a track has a valid input, it can start mixing sound by calling
   * Track.Play(), or possibly Mixer.PlayTag().
   *
   * Calling this function with a nullptr audio input is legal, and removes any
   * input from the track. If the track was currently playing, the next time the
   * mixer runs, it'll notice this and mark the track as stopped, calling any
   * assigned TrackStoppedCallback.
   *
   * It is legal to change the input of a track while it's playing, however some
   * states, like loop points, may cease to make sense with the new audio. In
   * such a case, one can call Track.Play again to adjust parameters.
   *
   * The track will hold a reference to the provided Audio, so it is safe to
   * call Audio.Destroy() on it while the track is still using it. The track
   * will drop its reference (and possibly free the resources) once it is no
   * longer using the Audio.
   *
   * @param audio the new audio input to set. May be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  void SetAudio(AudioRef audio);

  /**
   * Set a Track's input to an AudioStream.
   *
   * Using an audio stream allows the application to generate any type of audio,
   * in any format, possibly procedurally or on-demand, and mix in with all
   * other tracks.
   *
   * When a track uses an audio stream, it will call AudioStream.GetData as it
   * needs more audio to mix. The app can either buffer data to the stream ahead
   * of time, or set a callback on the stream to provide data as needed. Please
   * refer to SDL's documentation for details.
   *
   * A given audio stream may only be assigned to a single track at a time;
   * duplicate assignments won't return an error, but assigning a stream to
   * multiple tracks will cause each track to read from the stream arbitrarily,
   * causing confusion and incorrect mixing.
   *
   * Once a track has a valid input, it can start mixing sound by calling
   * Track.Play(), or possibly Mixer.PlayTag().
   *
   * Calling this function with a nullptr audio stream is legal, and removes any
   * input from the track. If the track was currently playing, the next time the
   * mixer runs, it'll notice this and mark the track as stopped, calling any
   * assigned TrackStoppedCallback.
   *
   * It is legal to change the input of a track while it's playing, however some
   * states, like loop points, may cease to make sense with the new audio. In
   * such a case, one can call Track.Play again to adjust parameters.
   *
   * The provided audio stream must remain valid until the track no longer needs
   * it (either by changing the track's input or destroying the track).
   *
   * @param stream the audio stream to use as the track's input.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  void SetAudioStream(AudioStreamRef stream);

  /**
   * Set a Track's input to an IOStream.
   *
   * This is not the recommended way to set a track's input, but this can be
   * useful for a very specific scenario: a large file, to be played once, that
   * must be read from disk in small chunks as needed. In most cases, however,
   * it is preferable to create a Audio ahead of time and use Track.SetAudio()
   * instead.
   *
   * The stream supplied here should provide an audio file in a supported
   * format. SDL_mixer will parse it during this call to make sure it's valid,
   * and then will read file data from the stream as it needs to decode more
   * during mixing.
   *
   * The stream must be able to seek through the complete set of data, or this
   * function will fail.
   *
   * A given IOStream may only be assigned to a single track at a time;
   * duplicate assignments won't return an error, but assigning a stream to
   * multiple tracks will cause each track to read from the stream arbitrarily,
   * causing confusion, incorrect mixing, or failure to decode.
   *
   * Once a track has a valid input, it can start mixing sound by calling
   * Track.Play(), or possibly Mixer.PlayTag().
   *
   * Calling this function with a nullptr stream is legal, and removes any input
   * from the track. If the track was currently playing, the next time the mixer
   * runs, it'll notice this and mark the track as stopped, calling any assigned
   * TrackStoppedCallback.
   *
   * It is legal to change the input of a track while it's playing, however some
   * states, like loop points, may cease to make sense with the new audio. In
   * such a case, one can call Track.Play again to adjust parameters.
   *
   * The provided stream must remain valid until the track no longer needs it
   * (either by changing the track's input or destroying the track).
   *
   * @param io the new i/o stream to use as the track's input.
   * @param closeio if true, close the stream when done with it.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.SetRawIOStream
   */
  void SetIOStream(IOStreamRef io, bool closeio);

  /**
   * Set a Track's input to an IOStream providing raw PCM data.
   *
   * This is not the recommended way to set a track's input, but this can be
   * useful for a very specific scenario: a large file, to be played once, that
   * must be read from disk in small chunks as needed. In most cases, however,
   * it is preferable to create a Audio ahead of time and use Track.SetAudio()
   * instead.
   *
   * Also, an Track.SetAudioStream() can _also_ provide raw PCM audio to a
   * track, via an AudioStream, which might be preferable unless the data is
   * already coming directly from an IOStream.
   *
   * The stream supplied here should provide an audio in raw PCM format.
   *
   * A given IOStream may only be assigned to a single track at a time;
   * duplicate assignments won't return an error, but assigning a stream to
   * multiple tracks will cause each track to read from the stream arbitrarily,
   * causing confusion and incorrect mixing.
   *
   * Once a track has a valid input, it can start mixing sound by calling
   * Track.Play(), or possibly Mixer.PlayTag().
   *
   * Calling this function with a nullptr stream is legal, and removes any input
   * from the track. If the track was currently playing, the next time the mixer
   * runs, it'll notice this and mark the track as stopped, calling any assigned
   * TrackStoppedCallback.
   *
   * It is legal to change the input of a track while it's playing, however some
   * states, like loop points, may cease to make sense with the new audio. In
   * such a case, one can call Track.Play again to adjust parameters.
   *
   * The provided stream must remain valid until the track no longer needs it
   * (either by changing the track's input or destroying the track).
   *
   * @param io the new i/o stream to use as the track's input.
   * @param spec the format of the PCM data that the IOStream will provide.
   * @param closeio if true, close the stream when done with it.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.SetAudioStream
   * @sa Track.SetIOStream
   */
  void SetRawIOStream(IOStreamRef io, const AudioSpec& spec, bool closeio);

  /**
   * Assign an arbitrary tag to a track.
   *
   * A tag can be any valid C string in UTF-8 encoding. It can be useful to
   * group tracks in various ways. For example, everything in-game might be
   * marked as "game", so when the user brings up the settings menu, the app can
   * pause all tracks involved in gameplay at once, but keep background music
   * and menu sound effects running.
   *
   * A track can have as many tags as desired, until the machine runs out of
   * memory.
   *
   * It's legal to add the same tag to a track more than once; the extra
   * attempts will report success but not change anything.
   *
   * Tags can later be removed with Track.Untag().
   *
   * @param tag the tag to add.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Untag
   */
  void Tag(StringParam tag);

  /**
   * Remove an arbitrary tag from a track.
   *
   * A tag can be any valid C string in UTF-8 encoding. It can be useful to
   * group tracks in various ways. For example, everything in-game might be
   * marked as "game", so when the user brings up the settings menu, the app can
   * pause all tracks involved in gameplay at once, but keep background music
   * and menu sound effects running.
   *
   * It's legal to remove a tag that the track doesn't have; this function
   * doesn't report errors, so this simply does nothing.
   *
   * Specifying a nullptr tag will remove all tags on a track.
   *
   * @param tag the tag to remove, or nullptr to remove all current tags.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Tag
   */
  void Untag(StringParam tag);

  /**
   * Get the tags currently associated with a track.
   *
   * Tags are not provided in any guaranteed order.
   *
   * @param count a pointer filled in with the number of tags returned, can be
   *              nullptr.
   * @returns an array of the tags, nullptr-terminated on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  char** GetTags(int* count);

  /**
   * Seek a playing track to a new position in its input.
   *
   * (Not to be confused with Track.Set3DPosition(), which is positioning of the
   * track in 3D space, not the playback position of its audio data.)
   *
   * On a playing track, the next time the mixer runs, it will start mixing from
   * the new position.
   *
   * Position is defined in _sample frames_ of decoded audio, not units of time,
   * so that sample-perfect mixing can be achieved. To instead operate in units
   * of time, use Track.MSToFrames() to get the approximate sample frames for a
   * given tick.
   *
   * This function requires an input that can seek (so it can not be used if the
   * input was set with Track.SetAudioStream()), and a audio file format that
   * allows seeking. SDL_mixer's decoders for some file formats do not offer
   * seeking, or can only seek to times, not exact sample frames, in which case
   * the final position may be off by some amount of sample frames. Please check
   * your audio data and file bug reports if appropriate.
   *
   * It's legal to call this function on a track that is stopped, but a future
   * call to Track.Play() will reset the start position anyhow. Paused tracks
   * will resume at the new input position.
   *
   * @param frames the sample frame position to seek to.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.GetPlaybackPosition
   */
  void SetPlaybackPosition(Sint64 frames);

  /**
   * Get the current input position of a playing track.
   *
   * (Not to be confused with Track.Get3DPosition(), which is positioning of the
   * track in 3D space, not the playback position of its audio data.)
   *
   * Position is defined in _sample frames_ of decoded audio, not units of time,
   * so that sample-perfect mixing can be achieved. To instead operate in units
   * of time, use Track.FramesToMS() to convert the return value to
   * milliseconds.
   *
   * Stopped and paused tracks will report the position when they halted.
   * Playing tracks will report the current position, which will change over
   * time.
   *
   * @returns the track's current sample frame position, or -1 on error; call
   *          GetError() for details.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.SetPlaybackPosition
   */
  Sint64 GetPlaybackPosition();

  /**
   * Query whether a given track is fading.
   *
   * This specifically checks if the track is _not stopped_ (paused or playing),
   * and it is fading in or out, and returns the number of frames remaining in
   * the fade.
   *
   * If fading out, the returned value will be negative. When fading in, the
   * returned value will be positive. If not fading, this function returns zero.
   *
   * On various errors (MIX.Init() was not called, the track is nullptr), this
   * returns 0, but there is no mechanism to distinguish errors from tracks that
   * aren't fading.
   *
   * @returns less than 0 if the track is fading out, greater than 0 if fading
   *          in, zero otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  Sint64 GetFadeFrames();

  /**
   * Query how many loops remain for a given track.
   *
   * This returns the number of loops still pending; if a track will eventually
   * complete and loop to play again one more time, this will return 1. If a
   * track _was_ looping but is on its final iteration of the loop (will stop
   * when this iteration completes), this will return zero.
   *
   * A track that is looping infinitely will return -1. This value does not
   * report an error in this case.
   *
   * A track that is stopped (not playing and not paused) will have zero loops
   * remaining.
   *
   * On various errors (MIX.Init() was not called, the track is nullptr), this
   * returns zero, but there is no mechanism to distinguish errors from
   * non-looping tracks.
   *
   * @returns the number of pending loops, zero if not looping, and -1 if
   *          looping infinitely.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  int GetLoops();

  /**
   * Change the number of times a currently-playing track will loop.
   *
   * This replaces any previously-set remaining loops. A value of 1 will loop to
   * the start of playback one time. Zero will not loop at all. A value of -1
   * requests infinite loops. If the input is not seekable and `num_loops` isn't
   * zero, this function will report success but the track will stop at the
   * point it should loop.
   *
   * The new loop count replaces any previous state, even if the track has
   * already looped.
   *
   * This has no effect on a track that is stopped, or rather, starting a
   * stopped track later will set a new loop count, replacing this value.
   * Stopped tracks can specify a loop count while starting via
   * prop::Play.LOOPS_NUMBER. This function is intended to alter that count in
   * the middle of playback.
   *
   * @param num_loops new number of times to loop. Zero to disable looping, -1
   *                  to loop infinitely.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.GetLoops
   */
  void SetLoops(int num_loops);

  /**
   * Query the Audio assigned to a track.
   *
   * This returns the Audio object currently assigned to `track` through a call
   * to Track.SetAudio(). If there is none assigned, or the track has an input
   * that isn't a Audio (such as an AudioStream or IOStream), this will return
   * nullptr.
   *
   * On various errors (MIX.Init() was not called, the track is nullptr), this
   * returns nullptr, but there is no mechanism to distinguish errors from
   * tracks without a valid input.
   *
   * @returns a Audio if available, nullptr if not.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.GetAudioStream
   */
  AudioRef GetAudio();

  /**
   * Query the AudioStream assigned to a track.
   *
   * This returns the AudioStream object currently assigned to `track` through a
   * call to Track.SetAudioStream(). If there is none assigned, or the track has
   * an input that isn't an AudioStream (such as a Audio or IOStream), this will
   * return nullptr.
   *
   * On various errors (MIX.Init() was not called, the track is nullptr), this
   * returns nullptr, but there is no mechanism to distinguish errors from
   * tracks without a valid input.
   *
   * @returns an AudioStream if available, nullptr if not.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.GetAudio
   */
  AudioStreamRef GetAudioStream();

  /**
   * Return the number of sample frames remaining to be mixed in a track.
   *
   * If the track is playing or paused, and its total duration is known, this
   * will report how much audio is left to mix. If the track is playing, future
   * calls to this function will report different values.
   *
   * Remaining audio is defined in _sample frames_ of decoded audio, not units
   * of time, so that sample-perfect mixing can be achieved. To instead operate
   * in units of time, use Track.FramesToMS() to convert the return value to
   * milliseconds.
   *
   * This function does not take into account fade-outs or looping, just the
   * current mixing position vs the duration of the track.
   *
   * If the duration of the track isn't known, or `track` is nullptr, this
   * function returns -1. A stopped track reports 0.
   *
   * @returns the total sample frames still to be mixed, or -1 if unknown.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  Sint64 GetRemaining();

  /**
   * Convert milliseconds to sample frames for a track's current format.
   *
   * This calculates time based on the track's current input format, which can
   * change when its input does, and also if that input changes formats
   * mid-stream (for example, if decoding a file that is two MP3s concatenated
   * together).
   *
   * On various errors (MIX.Init() was not called, the track is nullptr), this
   * returns -1. If the track has no input, this returns -1. If `ms` is < 0,
   * this returns -1.
   *
   * @param ms the milliseconds to convert to track-specific sample frames.
   * @returns Converted number of sample frames, or -1 for errors/no input; call
   *          GetError() for details.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.FramesToMS
   */
  Sint64 MSToFrames(Sint64 ms);

  /**
   * Convert sample frames for a track's current format to milliseconds.
   *
   * This calculates time based on the track's current input format, which can
   * change when its input does, and also if that input changes formats
   * mid-stream (for example, if decoding a file that is two MP3s concatenated
   * together).
   *
   * Sample frames are more precise than milliseconds, so out of necessity, this
   * function will approximate by rounding down to the closest full millisecond.
   *
   * On various errors (MIX.Init() was not called, the track is nullptr), this
   * returns -1. If the track has no input, this returns -1. If `frames` is < 0,
   * this returns -1.
   *
   * @param frames the track-specific sample frames to convert to milliseconds.
   * @returns Converted number of milliseconds, or -1 for errors/no input; call
   *          GetError() for details.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.MSToFrames
   */
  Sint64 FramesToMS(Sint64 frames);

  /**
   * Start (or restart) mixing a track for playback.
   *
   * The track will use whatever input was last assigned to it when playing; an
   * input must be assigned to this track or this function will fail. Inputs are
   * assigned with calls to Track.SetAudio(), Track.SetAudioStream(), or
   * Track.SetIOStream().
   *
   * If the track is already playing, or paused, this will restart the track
   * with the newly-specified parameters.
   *
   * As there are several parameters, and more may be added in the future, they
   * are specified with an Properties. The parameters have reasonable defaults,
   * and specifying a 0 for `options` will choose defaults for everything.
   *
   * Properties are discussed in [SDL's
   * documentation](https://wiki.libsdl.org/SDL3/CategoryProperties) .
   *
   * These are the supported properties:
   *
   * - `prop::Play.LOOPS_NUMBER`: The number of times to loop the track when it
   *   reaches the end. A value of 1 will loop to the start one time. Zero will
   *   not loop at all. A value of -1 requests infinite loops. If the input is
   *   not seekable and this value isn't zero, this function will report success
   *   but the track will stop at the point it should loop. Default 0.
   * - `prop::Play.MAX_FRAME_NUMBER`: Mix at most to this sample frame position
   *   in the track. This will be treated as if the input reach EOF at this
   *   point in the audio file. If -1, mix all available audio without a limit.
   *   Default -1.
   * - `prop::Play.MAX_MILLISECONDS_NUMBER`: The same as using the
   *   prop::Play.MAX_FRAME_NUMBER property, but the value is specified in
   *   milliseconds instead of sample frames. If both properties are specified,
   *   the sample frames value is favored. Default -1.
   * - `prop::Play.START_FRAME_NUMBER`: Start mixing from this sample frame
   *   position in the track's input. A value <= 0 will begin from the start of
   *   the track's input. If the input is not seekable and this value is > 0,
   *   this function will report failure. Default 0.
   * - `prop::Play.START_MILLISECOND_NUMBER`: The same as using the
   *   prop::Play.START_FRAME_NUMBER property, but the value is specified in
   *   milliseconds instead of sample frames. If both properties are specified,
   *   the sample frames value is favored. Default 0.
   * - `prop::Play.LOOP_START_FRAME_NUMBER`: If the track is looping, this is
   *   the sample frame position that the track will loop back to; this lets one
   *   play an intro at the start of a track on the first iteration, but have a
   *   loop point somewhere in the middle thereafter. A value <= 0 will begin
   *   the loop from the start of the track's input. Default 0.
   * - `prop::Play.LOOP_START_MILLISECOND_NUMBER`: The same as using the
   *   prop::Play.LOOP_START_FRAME_NUMBER property, but the value is specified
   *   in milliseconds instead of sample frames. If both properties are
   *   specified, the sample frames value is favored. Default 0.
   * - `prop::Play.FADE_IN_FRAMES_NUMBER`: The number of sample frames over
   *   which to fade in the newly-started track. The track will begin mixing
   *   silence and reach full volume smoothly over this many sample frames. If
   *   the track loops before the fade-in is complete, it will continue to fade
   *   correctly from the loop point. A value <= 0 will disable fade-in, so the
   *   track starts mixing at full volume. Default 0.
   * - `prop::Play.FADE_IN_MILLISECONDS_NUMBER`: The same as using the
   *   prop::Play.FADE_IN_FRAMES_NUMBER property, but the value is specified in
   *   milliseconds instead of sample frames. If both properties are specified,
   *   the sample frames value is favored. Default 0.
   * - `prop::Play.FADE_IN_START_GAIN_FLOAT`: If fading in, start fading from
   *   this volume level. 0.0f is silence and 1.0f is full volume, every in
   *   between is a linear change in gain. The specified value will be clamped
   *   between 0.0f and 1.0f. Default 0.0f.
   * - `prop::Play.APPEND_SILENCE_FRAMES_NUMBER`: At the end of mixing this
   *   track, after all loops are complete, append this many sample frames of
   *   silence as if it were part of the audio file. This allows for apps to
   *   implement effects in callbacks, like reverb, that need to generate
   *   samples past the end of the stream's audio, or perhaps introduce a delay
   *   before starting a new sound on the track without having to manage it
   *   directly. A value <= 0 generates no silence before stopping the track.
   *   Default 0.
   * - `prop::Play.APPEND_SILENCE_MILLISECONDS_NUMBER`: The same as using the
   *   prop::Play.APPEND_SILENCE_FRAMES_NUMBER property, but the value is
   *   specified in milliseconds instead of sample frames. If both properties
   *   are specified, the sample frames value is favored. Default 0.
   * - `prop::Play.HALT_WHEN_EXHAUSTED_BOOLEAN`: If true, when input is
   *   completely consumed for the track, the mixer will mark the track as
   *   stopped (and call any appropriate TrackStoppedCallback, etc); to play
   *   more, the track will need to be restarted. If false, the track will just
   *   not contribute to the mix, but it will not be marked as stopped. There
   *   may be clever logic tricks this exposes generally, but this property is
   *   specifically useful when the track's input is an AudioStream assigned via
   *   Track.SetAudioStream(). Setting this property to true can be useful when
   *   pushing a complete piece of audio to the stream that has a definite
   *   ending, as the track will operate like any other audio was applied.
   *   Setting to false means as new data is added to the stream, the mixer will
   *   start using it as soon as possible, which is useful when audio should
   *   play immediately as it drips in: new VoIP packets, etc. Note that in this
   *   situation, if the audio runs out when needed, there _will_ be gaps in the
   *   mixed output, so try to buffer enough data to avoid this when possible.
   *   Note that a track is not consider exhausted until all its loops and
   *   appended silence have been mixed (and also, that loops don't mean
   *   anything when the input is an AudioStream). Default true.
   *
   * If this function fails, mixing of this track will not start (or restart, if
   * it was already started).
   *
   * @param options a set of properties that control playback. May be zero.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Mixer.PlayTag
   * @sa Mixer.PlayAudio
   * @sa Track.Stop
   * @sa Track.Pause
   * @sa Track.Playing
   */
  void Play(PropertiesRef options);

  /**
   * Halt a currently-playing track, possibly fading out over time.
   *
   * If `fade_out_frames` is > 0, the track does not stop mixing immediately,
   * but rather fades to silence over that many sample frames before stopping.
   * Sample frames are specific to the input assigned to the track, to allow for
   * sample-perfect mixing. Track.MSToFrames() can be used to convert
   * milliseconds to an appropriate value here.
   *
   * If the track ends normally while the fade-out is still in progress, the
   * audio stops there; the fade is not adjusted to be shorter if it will last
   * longer than the audio remaining.
   *
   * Once a track has completed any fadeout and come to a stop, it will call its
   * TrackStoppedCallback, if any. It is legal to assign the track a new input
   * and/or restart it during this callback.
   *
   * It is legal to halt a track that's already stopped. It does nothing, and
   * returns true.
   *
   * @param fade_out_frames the number of sample frames to spend fading out to
   *                        silence before halting. 0 to stop immediately.
   * @returns true if the track has stopped, false on error; call GetError() for
   *          details.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Play
   */
  bool Stop(Sint64 fade_out_frames);

  /**
   * Pause a currently-playing track.
   *
   * A paused track is not considered "stopped," so its TrackStoppedCallback
   * will not fire if paused, but it won't change state by default, generate
   * audio, or generally make progress, until it is resumed.
   *
   * It is legal to pause a track that's in any state (playing, already paused,
   * or stopped). Unless the track is currently playing, pausing does nothing,
   * and returns true. A false return is only used to signal errors here (such
   * as MIX.Init not being called or `track` being nullptr).
   *
   * @returns true if the track has paused, false on error; call GetError() for
   *          details.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Resume
   */
  bool Pause();

  /**
   * Resume a currently-paused track.
   *
   * A paused track is not considered "stopped," so its TrackStoppedCallback
   * will not fire if paused, but it won't change state by default, generate
   * audio, or generally make progress, until it is resumed.
   *
   * It is legal to resume a track that's in any state (playing, paused, or
   * stopped). Unless the track is currently paused, resuming does nothing, and
   * returns true. A false return is only used to signal errors here (such as
   * MIX.Init not being called or `track` being nullptr).
   *
   * @returns true if the track has resumed, false on error; call GetError() for
   *          details.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Pause
   */
  bool Resume();

  /**
   * Query if a track is currently playing.
   *
   * If this returns true, the track is currently contributing to the mixer's
   * output (it's "playing"). It is not stopped nor paused.
   *
   * On various errors (MIX.Init() was not called, the track is nullptr), this
   * returns false, but there is no mechanism to distinguish errors from
   * non-playing tracks.
   *
   * @returns true if playing, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Play
   * @sa Track.Pause
   * @sa Track.Resume
   * @sa Track.Stop
   * @sa Track.Paused
   */
  bool Playing();

  /**
   * Query if a track is currently paused.
   *
   * If this returns true, the track is not currently contributing to the
   * mixer's output but will when resumed (it's "paused"). It is not playing nor
   * stopped.
   *
   * On various errors (MIX.Init() was not called, the track is nullptr), this
   * returns false, but there is no mechanism to distinguish errors from
   * non-playing tracks.
   *
   * @returns true if paused, false otherwise.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Play
   * @sa Track.Pause
   * @sa Track.Resume
   * @sa Track.Stop
   * @sa Track.Playing
   */
  bool Paused();

  /**
   * Set a track's gain control.
   *
   * Each track has its own gain, to adjust its overall volume. Each sample from
   * this track is modulated by this gain value. A gain of zero will generate
   * silence, 1.0f will not change the mixed volume, and larger than 1.0f will
   * increase the volume. Negative values are illegal. There is no maximum gain
   * specified, but this can quickly get extremely loud, so please be careful
   * with this setting.
   *
   * A track's gain defaults to 1.0f.
   *
   * This value can be changed at any time to adjust the future mix.
   *
   * @param gain the new gain value.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.GetGain
   * @sa Mixer.SetGain
   */
  void SetGain(float gain);

  /**
   * Get a track's gain control.
   *
   * This returns the last value set through Track.SetGain(), or 1.0f if no
   * value has ever been explicitly set.
   *
   * @returns the track's current gain.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.SetGain
   * @sa Mixer.GetGain
   */
  float GetGain();

  /**
   * Change the frequency ratio of a track.
   *
   * The frequency ratio is used to adjust the rate at which audio data is
   * consumed. Changing this effectively modifies the speed and pitch of the
   * track's audio. A value greater than 1.0f will play the audio faster, and at
   * a higher pitch. A value less than 1.0f will play the audio slower, and at a
   * lower pitch. 1.0f is normal speed.
   *
   * The default value is 1.0f.
   *
   * This value can be changed at any time to adjust the future mix.
   *
   * @param ratio the frequency ratio. Must be between 0.01f and 100.0f.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.GetFrequencyRatio
   */
  void SetFrequencyRatio(float ratio);

  /**
   * Query the frequency ratio of a track.
   *
   * The frequency ratio is used to adjust the rate at which audio data is
   * consumed. Changing this effectively modifies the speed and pitch of the
   * track's audio. A value greater than 1.0f will play the audio faster, and at
   * a higher pitch. A value less than 1.0f will play the audio slower, and at a
   * lower pitch. 1.0f is normal speed.
   *
   * The default value is 1.0f.
   *
   * On various errors (MIX.Init() was not called, the track is nullptr), this
   * returns 0.0f. Since this is not a valid value to set, this can be seen as
   * an error state.
   *
   * @returns the current frequency ratio, or 0.0f on failure; call GetError()
   *          for more information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.GetFrequencyRatio
   */
  float GetFrequencyRatio();

  /**
   * Set the current output channel map of a track.
   *
   * Channel maps are optional; most things do not need them, instead passing
   * data in the order that SDL expects.
   *
   * The output channel map reorders track data after transformations and before
   * it is mixed into a mixer group. This can be useful for reversing stereo
   * channels, for example.
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
   * Tracks default to no remapping applied. Passing a nullptr channel map is
   * legal, and turns off remapping.
   *
   * SDL_mixer will copy the channel map; the caller does not have to save this
   * array after this call.
   *
   * @param chmap the new channel map, nullptr to reset to default.
   * @param count The number of channels in the map.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  void SetOutputChannelMap(const int* chmap, int count);

  /**
   * Force a track to stereo output, with optionally left/right panning.
   *
   * This will cause the output of the track to convert to stereo, and then mix
   * it only onto the Front Left and Front Right speakers, regardless of the
   * speaker configuration. The left and right channels are modulated by
   * `gains`, which can be used to produce panning effects. This function may be
   * called to adjust the gains at any time.
   *
   * If `gains` is not nullptr, this track will be switched into forced-stereo
   * mode. If `gains` is nullptr, this will disable spatialization (both the
   * forced-stereo mode of this function and full 3D spatialization of
   * Track.Set3DPosition()).
   *
   * Negative gains are clamped to zero; there is no clamp for maximum, so one
   * could set the value > 1.0f to make a channel louder.
   *
   * The track's 3D position, reported by Track.Get3DPosition(), will be reset
   * to (0, 0, 0).
   *
   * @param gains the per-channel gains, or nullptr to disable spatialization.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Set3DPosition
   */
  void SetStereo(const StereoGains& gains);

  /**
   * Set a track's position in 3D space.
   *
   * (Please note that SDL_mixer is not intended to be a extremely powerful 3D
   * API. It lacks 3D features that other APIs like OpenAL offer: there's no
   * doppler effect, distance models, rolloff, etc. This is meant to be Good
   * Enough for games that can use some positional sounds and can even take
   * advantage of surround-sound configurations.)
   *
   * If `position` is not nullptr, this track will be switched into 3D
   * positional mode. If `position` is nullptr, this will disable positional
   * mixing (both the full 3D spatialization of this function and forced-stereo
   * mode of Track.SetStereo()).
   *
   * In 3D positional mode, SDL_mixer will mix this track as if it were
   * positioned in 3D space, including distance attenuation (quieter as it gets
   * further from the listener) and spatialization (positioned on the correct
   * speakers to suggest direction, either with stereo outputs or full surround
   * sound).
   *
   * For a mono speaker output, spatialization is effectively disabled but
   * distance attenuation will still work, which is all you can really do with a
   * single speaker.
   *
   * The coordinate system operates like OpenGL or OpenAL: a "right-handed"
   * coordinate system. See Point3D for the details.
   *
   * The listener is always at coordinate (0,0,0) and can't be changed.
   *
   * The track's input will be converted to mono (1 channel) so it can be
   * rendered across the correct speakers.
   *
   * @param position the new 3D position for the track. May be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Get3DPosition
   * @sa Track.SetStereo
   */
  void Set3DPosition(const Point3D& position);

  /**
   * Get a track's current position in 3D space.
   *
   * If 3D positioning isn't enabled for this track, through a call to
   * Track.Set3DPosition(), this will return (0,0,0).
   *
   * @param position on successful return, will contain the track's position.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Track.Set3DPosition
   */
  void Get3DPosition(Point3D* position);

  /**
   * Assign a track to a mixing group.
   *
   * All tracks in a group are mixed together, and that output is made available
   * to the app before it is mixed into the final output.
   *
   * Tracks can only be in one group at a time, and the track and group must
   * have been created on the same Mixer.
   *
   * Setting a track to a nullptr group will remove it from any app-created
   * groups, and reassign it to the mixer's internal default group.
   *
   * @param group the new mixing group to assign to. May be nullptr.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Group.Group
   * @sa Group.SetPostMixCallback
   */
  void SetGroup(GroupRef group);

  /**
   * Set a callback that fires when a Track is stopped.
   *
   * When a track completes playback, either because it ran out of data to mix
   * (and all loops were completed as well), or it was explicitly stopped by the
   * app, it will fire the callback specified here.
   *
   * Each track has its own unique callback.
   *
   * Passing a nullptr callback here is legal; it disables this track's
   * callback.
   *
   * Pausing a track will not fire the callback, nor will the callback fire on a
   * playing track that is being destroyed.
   *
   * It is legal to adjust the track, including changing its input and
   * restarting it. If this is done because it ran out of data in the middle of
   * mixing, the mixer will start mixing the new track state in its current run
   * without any gap in the audio.
   *
   * @param cb the function to call when the track stops. May be nullptr.
   * @param userdata an opaque pointer provided to the callback for its own
   *                 personal use.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa TrackStoppedCallback
   */
  void SetStoppedCallback(TrackStoppedCallback cb, void* userdata);

  /**
   * Set a callback that fires when a Track has initial decoded audio.
   *
   * As a track needs to mix more data, it pulls from its input (a Audio, an
   * AudioStream, etc). This input might be a compressed file format, like MP3,
   * so a little more data is uncompressed from it.
   *
   * Once the track has PCM data to start operating on, it can fire a callback
   * before _any_ changes to the raw PCM input have happened. This lets an app
   * view the data before it has gone through transformations such as gain, 3D
   * positioning, fading, etc. It can also change the data in any way it pleases
   * during this callback, and the mixer will continue as if this data came
   * directly from the input.
   *
   * Each track has its own unique raw callback.
   *
   * Passing a nullptr callback here is legal; it disables this track's
   * callback.
   *
   * @param cb the function to call when the track mixes. May be nullptr.
   * @param userdata an opaque pointer provided to the callback for its own
   *                 personal use.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa TrackMixCallback
   * @sa Track.SetCookedCallback
   */
  void SetRawCallback(TrackMixCallback cb, void* userdata);

  /**
   * Set a callback that fires when the mixer has transformed a track's audio.
   *
   * As a track needs to mix more data, it pulls from its input (a Audio, an
   * AudioStream, etc). This input might be a compressed file format, like MP3,
   * so a little more data is uncompressed from it.
   *
   * Once the track has PCM data to start operating on, and its raw callback has
   * completed, it will begin to transform the audio: gain, fading, frequency
   * ratio, 3D positioning, etc.
   *
   * A callback can be fired after all these transformations, but before the
   * transformed data is mixed into other tracks. This lets an app view the data
   * at the last moment that it is still a part of this track. It can also
   * change the data in any way it pleases during this callback, and the mixer
   * will continue as if this data came directly from the input.
   *
   * Each track has its own unique cooked callback.
   *
   * Passing a nullptr callback here is legal; it disables this track's
   * callback.
   *
   * @param cb the function to call when the track mixes. May be nullptr.
   * @param userdata an opaque pointer provided to the callback for its own
   *                 personal use.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa TrackMixCallback
   * @sa Track.SetRawCallback
   */
  void SetCookedCallback(TrackMixCallback cb, void* userdata);
};

/**
 * Reference for Track.
 *
 * This does not take ownership!
 */
struct TrackRef : Track
{
  using Track::Track;

  /**
   * Constructs from raw Track.
   *
   * @param resource a TrackRaw.
   *
   * This does not takes ownership!
   */
  TrackRef(TrackRaw resource) noexcept
    : Track(resource)
  {
  }

  /**
   * Constructs from Track.
   *
   * @param resource a Track.
   *
   * This does not takes ownership!
   */
  constexpr TrackRef(const Track& resource) noexcept
    : Track(resource.get())
  {
  }

  /// Copy constructor.
  constexpr TrackRef(const TrackRef& other) noexcept
    : Track(other.get())
  {
  }

  /// Move constructor.
  constexpr TrackRef(TrackRef&& other) noexcept
    : Track(other.release())
  {
  }

  /// Destructor
  ~TrackRef() { release(); }

  /// Assignment operator.
  constexpr TrackRef& operator=(TrackRef other) noexcept
  {
    std::swap(*this, other);
    return *this;
  }

  /// Converts to TrackRaw
  constexpr operator TrackRaw() const noexcept { return get(); }
};

/**
 * A callback that fires when a Group has completed mixing.
 *
 * This callback is fired when a mixing group has finished mixing: all tracks in
 * the group have mixed into a single buffer and are prepared to be mixed into
 * all other groups for the final mix output.
 *
 * The audio data passed through here is _not_ const data; the app is permitted
 * to change it in any way it likes, and those changes will propagate through
 * the mixing pipeline.
 *
 * An audiospec is provided. Different groups might be in different formats, and
 * an app needs to be able to handle that, but SDL_mixer always does its mixing
 * work in 32-bit float samples, even if the inputs or final output are not
 * floating point. As such, `spec->format` will always be `AUDIO_F32` and `pcm`
 * hardcoded to be a float pointer.
 *
 * `samples` is the number of float values pointed to by `pcm`: samples, not
 * sample frames! There are no promises how many samples will be provided
 * per-callback, and this number can vary wildly from call to call, depending on
 * many factors.
 *
 * @param userdata an opaque pointer provided by the app for its personal use.
 * @param group the group that is being mixed.
 * @param spec the format of the data in `pcm`.
 * @param pcm the raw PCM data in float32 format.
 * @param samples the number of float values pointed to by `pcm`.
 *
 * @since This datatype is available since SDL_mixer 3.0.0.
 *
 * @sa Group.SetPostMixCallback
 */
using GroupMixCallback = void(SDLCALL*)(void* userdata,
                                        GroupRaw group,
                                        const AudioSpec* spec,
                                        float* pcm,
                                        int samples);

/**
 * An opaque object that represents a grouping of tracks.
 *
 * SDL_mixer offers callbacks at various stages of the mixing pipeline to allow
 * apps to view and manipulate data as it is transformed. Sometimes it is useful
 * to hook in at a point where several tracks--but not all tracks-- have been
 * mixed. For example, when a game is in some options menu, perhaps adjusting
 * game audio but not UI sounds could be useful.
 *
 * SDL_mixer allows you to assign several tracks to a group, and receive a
 * callback when that group has finished mixing, with a buffer of just that
 * group's mixed audio, before it mixes into the final output.
 *
 * @since This datatype is available since SDL_mixer 3.0.0.
 *
 * @cat resource
 */
class Group
{
  GroupRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr Group(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from GroupRef.
   *
   * @param resource a GroupRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit Group(const GroupRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr Group(const Group& other) noexcept = default;

public:
  /// Move constructor
  constexpr Group(Group&& other) noexcept
    : Group(other.release())
  {
  }

  constexpr Group(const GroupRef& other) = delete;

  constexpr Group(GroupRef&& other) = delete;

  /**
   * Create a mixing group.
   *
   * Tracks are assigned to a mixing group (or if unassigned, they live in a
   * mixer's internal default group). All tracks in a group are mixed together
   * and the app can access this mixed data before it is mixed with all other
   * groups to produce the final output.
   *
   * This can be a useful feature, but is completely optional; apps can ignore
   * mixing groups entirely and still have a full experience with SDL_mixer.
   *
   * After creating a group, assign tracks to it with Track.SetGroup(). Use
   * Group.SetPostMixCallback() to access the group's mixed data.
   *
   * A mixing group can be destroyed with Group.Destroy() when no longer needed.
   * Destroying the mixer will also destroy all its still-existing mixing
   * groups.
   *
   * @param mixer the mixer on which to create a mixing group.
   * @post a newly-created mixing group on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Group.Destroy
   * @sa Track.SetGroup
   * @sa Group.SetPostMixCallback
   */
  Group(MixerRef mixer);

  /// Destructor
  ~Group() { MIX_DestroyGroup(m_resource); }

  /// Assignment operator.
  constexpr Group& operator=(Group&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr Group& operator=(const Group& other) noexcept = default;

public:
  /// Retrieves underlying GroupRaw.
  constexpr GroupRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying GroupRaw and clear this.
  constexpr GroupRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const Group& other) const noexcept = default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /**
   * Destroy a mixing group.
   *
   * Any tracks currently assigned to this group will be reassigned to the
   * mixer's internal default group.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Group.Group
   */
  void Destroy();

  /**
   * Get the properties associated with a group.
   *
   * Currently SDL_mixer assigns no properties of its own to a group, but this
   * can be a convenient place to store app-specific data.
   *
   * A Properties is created the first time this function is called for a given
   * group.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  PropertiesRef GetProperties();

  /**
   * Get the Mixer that owns a Group.
   *
   * This is the mixer pointer that was passed to Group.Group().
   *
   * @returns the mixer associated with the group on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  MixerRef GetMixer();

  /**
   * Set a callback that fires when a mixer group has completed mixing.
   *
   * After all playing tracks in a mixer group have pulled in more data from
   * their inputs, transformed it, and mixed together into a single buffer, a
   * callback can be fired. This lets an app view the data at the last moment
   * that it is still a part of this group. It can also change the data in any
   * way it pleases during this callback, and the mixer will continue as if this
   * data came directly from the group's mix buffer.
   *
   * Each group has its own unique callback. Tracks that aren't in an explicit
   * Group are mixed in an internal grouping that is not available to the app.
   *
   * Passing a nullptr callback here is legal; it disables this group's
   * callback.
   *
   * @param cb the function to call when the group mixes. May be nullptr.
   * @param userdata an opaque pointer provided to the callback for its own
   *                 personal use.
   * @throws Error on failure.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa GroupMixCallback
   */
  void SetPostMixCallback(GroupMixCallback cb, void* userdata);
};

/**
 * Reference for Group.
 *
 * This does not take ownership!
 */
struct GroupRef : Group
{
  using Group::Group;

  /**
   * Constructs from raw Group.
   *
   * @param resource a GroupRaw.
   *
   * This does not takes ownership!
   */
  GroupRef(GroupRaw resource) noexcept
    : Group(resource)
  {
  }

  /**
   * Constructs from Group.
   *
   * @param resource a Group.
   *
   * This does not takes ownership!
   */
  constexpr GroupRef(const Group& resource) noexcept
    : Group(resource.get())
  {
  }

  /// Copy constructor.
  constexpr GroupRef(const GroupRef& other) noexcept
    : Group(other.get())
  {
  }

  /// Move constructor.
  constexpr GroupRef(GroupRef&& other) noexcept
    : Group(other.release())
  {
  }

  /// Destructor
  ~GroupRef() { release(); }

  /// Assignment operator.
  constexpr GroupRef& operator=(GroupRef other) noexcept
  {
    std::swap(*this, other);
    return *this;
  }

  /// Converts to GroupRaw
  constexpr operator GroupRaw() const noexcept { return get(); }
};

#ifdef SDL3PP_DOC

/**
 * The current major version of SDL_mixer headers.
 *
 * If this were SDL_mixer version 3.2.1, this value would be 3.
 *
 * @since This macro is available since SDL_mixer 3.0.0.
 */
#define SDL_MIXER_MAJOR_VERSION

/**
 * The current minor version of the SDL_mixer headers.
 *
 * If this were SDL_mixer version 3.2.1, this value would be 2.
 *
 * @since This macro is available since SDL_mixer 3.0.0.
 */
#define SDL_MIXER_MINOR_VERSION

/**
 * The current micro (or patchlevel) version of the SDL_mixer headers.
 *
 * If this were SDL_mixer version 3.2.1, this value would be 1.
 *
 * @since This macro is available since SDL_mixer 3.0.0.
 */
#define SDL_MIXER_MICRO_VERSION

/**
 * This is the current version number macro of the SDL_mixer headers.
 *
 * @since This macro is available since SDL_mixer 3.0.0.
 *
 * @sa MIX.Version
 */
#define SDL_MIXER_VERSION                                                      \
  SDL_VERSIONNUM(                                                              \
    SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_MICRO_VERSION)

/**
 * This macro will evaluate to true if compiled with SDL_mixer at least X.Y.Z.
 *
 * @since This macro is available since SDL_mixer 3.0.0.
 */
#define SDL_MIXER_VERSION_ATLEAST(X, Y, Z)                                     \
  ((SDL_MIXER_MAJOR_VERSION >= X) &&                                           \
   (SDL_MIXER_MAJOR_VERSION > X || SDL_MIXER_MINOR_VERSION >= Y) &&            \
   (SDL_MIXER_MAJOR_VERSION > X || SDL_MIXER_MINOR_VERSION > Y ||              \
    SDL_MIXER_MICRO_VERSION >= Z))

#endif // SDL3PP_DOC

namespace MIX {

/**
 * Get the version of SDL_mixer that is linked against your program.
 *
 * If you are linking to SDL_mixer dynamically, then it is possible that the
 * current version will be different than the version you compiled against. This
 * function returns the current version, while SDL_MIXER_VERSION is the version
 * you compiled with.
 *
 * This function may be called safely at any time, even before MIX.Init().
 *
 * @returns the version of the linked library.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa SDL_MIXER_VERSION
 */
inline int Version() { return MIX_Version(); }

/**
 * Initialize the SDL_mixer library.
 *
 * This must be successfully called once before (almost) any other SDL_mixer
 * function can be used.
 *
 * It is safe to call this multiple times; the library will only initialize
 * once, and won't deinitialize until MIX.Quit() has been called a matching
 * number of times. Extra attempts to init report success.
 *
 * @throws Error on failure.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa MIX.Quit
 */
inline void Init() { CheckError(MIX_Init()); }

/**
 * Deinitialize the SDL_mixer library.
 *
 * This must be called when done with the library, probably at the end of your
 * program.
 *
 * It is safe to call this multiple times; the library will only deinitialize
 * once, when this function is called the same number of times as MIX.Init was
 * successfully called.
 *
 * Once you have successfully deinitialized the library, it is safe to call
 * MIX.Init to reinitialize it for further use.
 *
 * On successful deinitialization, SDL_mixer will destroy almost all created
 * objects, including objects of type:
 *
 * - Mixer
 * - Track
 * - Audio
 * - Group
 * - AudioDecoder
 *
 * ...which is to say: it's possible a single call to this function will clean
 * up anything it allocated, stop all audio output, close audio devices, etc.
 * Don't attempt to destroy objects after this call. The app is still encouraged
 * to manage their resources carefully and clean up first, treating this
 * function as a safety net against memory leaks.
 *
 * @threadsafety This function is not thread safe.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa MIX.Init
 */
inline void Quit() { MIX_Quit(); }

} // namespace MIX

/**
 * Report the number of audio decoders available for use.
 *
 * An audio decoder is what turns specific audio file formats into usable PCM
 * data. For example, there might be an MP3 decoder, or a WAV decoder, etc.
 * SDL_mixer probably has several decoders built in.
 *
 * The return value can be used to call GetAudioDecoder() in a loop.
 *
 * The number of decoders available is decided during MIX.Init() and does not
 * change until the library is deinitialized.
 *
 * @returns the number of decoders available.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa GetAudioDecoder
 */
inline int GetNumAudioDecoders() { return MIX_GetNumAudioDecoders(); }

/**
 * Report the name of a specific audio decoders.
 *
 * An audio decoder is what turns specific audio file formats into usable PCM
 * data. For example, there might be an MP3 decoder, or a WAV decoder, etc.
 * SDL_mixer probably has several decoders built in.
 *
 * The names are capital English letters and numbers, low-ASCII. They don't
 * necessarily map to a specific file format; Some decoders, like "XMP" operate
 * on multiple file types, and more than one decoder might handle the same file
 * type, like "DRMP3" vs "MPG123". Note that in that last example, neither
 * decoder is called "MP3".
 *
 * The index of a specific decoder is decided during MIX.Init() and does not
 * change until the library is deinitialized. Valid indices are between zero and
 * the return value of GetNumAudioDecoders().
 *
 * The returned pointer is const memory owned by SDL_mixer; do not free it.
 *
 * @param index the index of the decoder to query.
 * @returns a UTF-8 (really, ASCII) string of the decoder's name, or nullptr if
 *          `index` is invalid.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa GetNumAudioDecoders
 */
inline const char* GetAudioDecoder(int index)
{
  return MIX_GetAudioDecoder(index);
}

/**
 * Create a mixer that plays sound directly to an audio device.
 *
 * This is usually the function you want, vs Mixer.Mixer().
 *
 * You can choose a specific device ID to open, following SDL's usual rules, but
 * often the correct choice is to specify AUDIO_DEVICE_DEFAULT_PLAYBACK and let
 * SDL figure out what device to use (and seamlessly transition you to new
 * hardware if the default changes).
 *
 * Only playback devices make sense here. Attempting to open a recording device
 * will fail.
 *
 * This will call Init(INIT_AUDIO) internally; it's safe to call Init() before
 * this call, too, if you intend to enumerate audio devices to choose one to
 * open here.
 *
 * An audio format can be requested, and the system will try to set the hardware
 * to those specifications, or as close as possible, but this is just a hint.
 * SDL_mixer will handle all data conversion behind the scenes in any case, and
 * specifying a nullptr spec is a reasonable choice. The best reason to specify
 * a format is because you know all your data is in that format and it might
 * save some unnecessary CPU time on conversion.
 *
 * The actual device format chosen is available through Mixer.GetFormat().
 *
 * Once a mixer is created, next steps are usually to load audio (through
 * Audio.Audio() and friends), create a track (Track.Track()), and play that
 * audio through that track.
 *
 * When done with the mixer, it can be destroyed with Mixer.Destroy().
 *
 * @param devid the device to open for playback, or
 *              AUDIO_DEVICE_DEFAULT_PLAYBACK for the default.
 * @param spec the audio format to request from the device. May be nullptr.
 * @returns a mixer that can be used to play audio on success.
 * @throws Error on failure.
 *
 * @threadsafety This function should only be called on the main thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Mixer.Mixer
 * @sa Mixer.Destroy
 */
inline Mixer CreateMixerDevice(AudioDeviceRef devid, const AudioSpec& spec)
{
  return Mixer(devid, spec);
}

inline Mixer::Mixer(AudioDeviceRef devid, const AudioSpec& spec)
  : m_resource(CheckError(MIX_CreateMixerDevice(devid, &spec)))
{
}

inline Mixer::Mixer(const AudioSpec& spec)
  : m_resource(CheckError(MIX_CreateMixer(&spec)))
{
}

/**
 * Create a mixer that generates audio to a memory buffer.
 *
 * Usually you want Mixer.Mixer() instead of this function. The mixer created
 * here can be used with Mixer.Generate() to produce more data on demand, as
 * fast as desired.
 *
 * An audio format must be specified. This is the format it will output in. This
 * cannot be nullptr.
 *
 * Once a mixer is created, next steps are usually to load audio (through
 * Audio.Audio() and friends), create a track (Track.Track()), and play that
 * audio through that track.
 *
 * When done with the mixer, it can be destroyed with Mixer.Destroy().
 *
 * @param spec the audio format that mixer will generate.
 * @returns a mixer that can be used to generate audio on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Mixer.Mixer
 * @sa Mixer.Destroy
 */
inline Mixer CreateMixer(const AudioSpec& spec) { return Mixer(spec); }

/**
 * Free a mixer.
 *
 * If this mixer was created with Mixer.Mixer(), this function will also close
 * the audio device and call QuitSubSystem(INIT_AUDIO).
 *
 * Any Group or Track created for this mixer will also be destroyed. Do not
 * access them again or attempt to destroy them after the device is destroyed.
 * Audio objects will not be destroyed, since they can be shared between mixers
 * (but those will all be destroyed during MIX.Quit()).
 *
 * @param mixer the mixer to destroy.
 *
 * @threadsafety If this is used with a Mixer from Mixer.Mixer, then this
 *               function should only be called on the main thread. If this is
 *               used with a Mixer from Mixer.Mixer, then it is safe to call
 *               this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Mixer.Mixer
 * @sa Mixer.Mixer
 */
inline void DestroyMixer(MixerRaw mixer) { MIX_DestroyMixer(mixer); }

inline void Mixer::Destroy() { DestroyMixer(release()); }

/**
 * Get the properties associated with a mixer.
 *
 * The following read-only properties are provided by SDL_mixer:
 *
 * - `prop::Mixer.DEVICE_NUMBER`: the AudioDevice that this mixer has opened for
 *   playback. This will be zero (no device) if the mixer was created with
 *   Mix_CreateMixer() instead of Mix_CreateMixerDevice().
 *
 * @param mixer the mixer to query.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline PropertiesRef GetMixerProperties(MixerRef mixer)
{
  return CheckError(MIX_GetMixerProperties(mixer));
}

inline PropertiesRef Mixer::GetProperties()
{
  return SDL::GetMixerProperties(m_resource);
}

namespace prop::Mixer {

constexpr auto DEVICE_NUMBER = MIX_PROP_MIXER_DEVICE_NUMBER;

} // namespace prop::Mixer

/**
 * Get the audio format a mixer is generating.
 *
 * Generally you don't need this information, as SDL_mixer will convert data as
 * necessary between inputs you provide and its output format, but it might be
 * useful if trying to match your inputs to reduce conversion and resampling
 * costs.
 *
 * For mixers created with Mixer.Mixer(), this is the format of the audio device
 * (and may change later if the device itself changes; SDL_mixer will seamlessly
 * handle this change internally, though).
 *
 * For mixers created with Mixer.Mixer(), this is the format that
 * Mixer.Generate() will produce, as requested at create time, and does not
 * change.
 *
 * Note that internally, SDL_mixer will work in AUDIO_F32 format before
 * outputting the format specified here, so it would be more efficient to match
 * input data to that, not the final output format.
 *
 * @param mixer the mixer to query.
 * @param spec where to store the mixer audio format.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline void GetMixerFormat(MixerRef mixer, AudioSpec* spec)
{
  CheckError(MIX_GetMixerFormat(mixer, spec));
}

inline void Mixer::GetFormat(AudioSpec* spec)
{
  SDL::GetMixerFormat(m_resource, spec);
}

/**
 * Lock a mixer by obtaining its internal mutex.
 *
 * While locked, the mixer will not be able to mix more audio or change its
 * internal state in another thread. Those other threads will block until the
 * mixer is unlocked again.
 *
 * Under the hood, this function calls Mutex.Lock(), so all the same rules
 * apply: the lock can be recursive, it must be unlocked the same number of
 * times from the same thread that locked it, etc.
 *
 * Just about every SDL_mixer API _also_ locks the mixer while doing its work,
 * as does the SDL audio device thread while actual mixing is in progress, so
 * basic use of this library never requires the app to explicitly lock the
 * device to be thread safe. There are two scenarios where this can be useful,
 * however:
 *
 * - The app has a provided a callback that the mixing thread might call, and
 *   there is some app state that needs to be protected against race conditions
 *   as changes are made and mixing progresses simultaneously. Any lock can be
 *   used for this, but this is a conveniently-available lock.
 * - The app wants to make multiple, atomic changes to the mix. For example, to
 *   start several tracks at the exact same moment, one would lock the mixer,
 *   call Track.Play multiple times, and then unlock again; all the tracks will
 *   start mixing on the same sample frame.
 *
 * Each call to this function must be paired with a call to Mixer.Unlock from
 * the same thread. It is safe to lock a mixer multiple times; it remains locked
 * until the final matching unlock call.
 *
 * Do not lock the mixer for significant amounts of time, or it can cause audio
 * dropouts. Just do simply things quickly and unlock again.
 *
 * Locking a nullptr mixer is a safe no-op.
 *
 * @param mixer the mixer to lock. May be nullptr.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Mixer.Unlock
 */
inline void LockMixer(MixerRef mixer) { MIX_LockMixer(mixer); }

inline void Mixer::Lock() { SDL::LockMixer(m_resource); }

/**
 * Unlock a mixer previously locked by a call to Mixer.Lock().
 *
 * While locked, the mixer will not be able to mix more audio or change its
 * internal state another thread. Those other threads will block until the mixer
 * is unlocked again.
 *
 * Under the hood, this function calls Mutex.Lock(), so all the same rules
 * apply: the lock can be recursive, it must be unlocked the same number of
 * times from the same thread that locked it, etc.
 *
 * Unlocking a nullptr mixer is a safe no-op.
 *
 * @param mixer the mixer to unlock. May be nullptr.
 *
 * @threadsafety This call must be paired with a previous Mixer.Lock call on the
 *               same thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Mixer.Lock
 */
inline void UnlockMixer(MixerRef mixer) { MIX_UnlockMixer(mixer); }

inline void Mixer::Unlock() { SDL::UnlockMixer(m_resource); }

/**
 * Load audio for playback from an IOStream.
 *
 * In normal usage, apps should load audio once, maybe at startup, then play it
 * multiple times.
 *
 * When loading audio, it will be cached fully in RAM in its original data
 * format. Each time it plays, the data will be decoded. For example, an MP3
 * will be stored in memory in MP3 format and be decompressed on the fly during
 * playback. This is a tradeoff between i/o overhead and memory usage.
 *
 * If `predecode` is true, the data will be decompressed during load and stored
 * as raw PCM data. This might dramatically increase loading time and memory
 * usage, but there will be no need to decompress data during playback.
 *
 * (One could also use Track.SetIOStream() to bypass loading the data into RAM
 * upfront at all, but this offers still different tradeoffs. The correct
 * approach depends on the app's needs and employing different approaches in
 * different situations can make sense.)
 *
 * Audio objects can be shared between mixers. This function takes a Mixer, to
 * imply this is the most likely place it will be used and loading should try to
 * match its audio format, but the resulting audio can be used elsewhere. If
 * `mixer` is nullptr, SDL_mixer will set reasonable defaults.
 *
 * Once a Audio is created, it can be assigned to a Track with Track.SetAudio(),
 * or played without any management with Mixer.PlayAudio().
 *
 * When done with a Audio, it can be freed with Audio.Destroy().
 *
 * This function loads data from an IOStream. There is also a version that loads
 * from a path on the filesystem (Audio.Audio()), and one that accepts
 * properties for ultimate control (LoadAudioWithProperties()).
 *
 * The IOStream provided must be able to seek, or loading will fail. If the
 * stream can't seek (data is coming from an HTTP connection, etc), consider
 * caching the data to memory or disk first and creating a new stream to read
 * from there.
 *
 * @param mixer a mixer this audio is intended to be used with. May be nullptr.
 * @param io the IOStream to load data from.
 * @param predecode if true, data will be fully uncompressed before returning.
 * @param closeio true if SDL_mixer should close `io` before returning (success
 *                or failure).
 * @returns an audio object that can be used to make sound on a mixer, or
 *          nullptr on failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Audio.Destroy
 * @sa Track.SetAudio
 * @sa Audio.Audio
 * @sa LoadAudioWithProperties
 */
inline Audio LoadAudio_IO(MixerRef mixer,
                          IOStreamRef io,
                          bool predecode,
                          bool closeio = false)
{
  return Audio(mixer, io, predecode, closeio);
}

inline Audio Mixer::LoadAudio_IO(IOStreamRef io, bool predecode, bool closeio)
{
  return Audio(m_resource, io, predecode, closeio);
}

inline Audio::Audio(MixerRef mixer,
                    IOStreamRef io,
                    bool predecode,
                    bool closeio)
  : m_resource(MIX_LoadAudio_IO(mixer, io, predecode, closeio))
{
}

inline Audio::Audio(MixerRef mixer, StringParam path, bool predecode)
  : m_resource(MIX_LoadAudio(mixer, path, predecode))
{
}

inline Audio::Audio(PropertiesRef props)
  : m_resource(CheckError(MIX_LoadAudioWithProperties(props)))
{
}

inline Audio::Audio(MixerRef mixer,
                    IOStreamRef io,
                    const AudioSpec& spec,
                    bool closeio)
  : m_resource(CheckError(MIX_LoadRawAudio_IO(mixer, io, &spec, closeio)))
{
}

inline Audio::Audio(MixerRef mixer, SourceBytes data, const AudioSpec& spec)
  : m_resource(CheckError(
      MIX_LoadRawAudio(mixer, data.data(), data.size_bytes(), &spec)))
{
}

/**
 * Load audio for playback from a file.
 *
 * This is equivalent to calling:
 *
 * ```cpp
 * Audio audio(mixer, IOStream.FromFile(path, "rb"), predecode, true);
 * ```
 *
 * This function loads data from a path on the filesystem. There is also a
 * version that loads from an IOStream (Audio.Audio()), and one that accepts
 * properties for ultimate control (LoadAudioWithProperties()).
 *
 * @param mixer a mixer this audio is intended to be used with. May be nullptr.
 * @param path the path on the filesystem to load data from.
 * @param predecode if true, data will be fully uncompressed before returning.
 * @returns an audio object that can be used to make sound on a mixer, or
 *          nullptr on failure; call GetError() for more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Audio.Destroy
 * @sa Track.SetAudio
 * @sa Mixer.LoadAudio_IO
 * @sa LoadAudioWithProperties
 */
inline Audio LoadAudio(MixerRef mixer, StringParam path, bool predecode)
{
  return Audio(mixer, std::move(path), predecode);
}

inline Audio Mixer::LoadAudio(StringParam path, bool predecode)
{
  return Audio(m_resource, std::move(path), predecode);
}

/**
 * Load audio for playback from a memory buffer without making a copy.
 *
 * When loading audio through most other LoadAudio functions, the data will be
 * cached fully in RAM in its original data format, for decoding on-demand. This
 * function does most of the same work as those functions, but instead uses a
 * buffer of memory provided by the app that it does not make a copy of.
 *
 * This buffer must live for the entire time the returned Audio lives, as the
 * mixer will access the buffer whenever it needs to mix more data.
 *
 * This function is meant to maximize efficiency: if the data is already in
 * memory and can remain there, don't copy it. This data can be in any supported
 * audio file format (WAV, MP3, etc); it will be decoded on the fly while
 * mixing. Unlike Audio.Audio(), there is no `predecode` option offered here, as
 * this is meant to optimize for data that's already in memory and intends to
 * exist there for significant time; since predecoding would only need the file
 * format data once, upfront, one could simply wrap it in
 * SDL_CreateIOFromConstMem() and pass that to Mixer.LoadAudio_IO().
 *
 * Audio objects can be shared between multiple mixers. The `mixer` parameter
 * just suggests the most likely mixer to use this audio, in case some
 * optimization might be applied, but this is not required, and a nullptr mixer
 * may be specified.
 *
 * If `free_when_done` is true, SDL_mixer will call `free(data)` when the
 * returned Audio is eventually destroyed. This can be useful when the data is
 * not static, but rather loaded elsewhere for this specific Audio and simply
 * wants to avoid the extra copy.
 *
 * As audio format information is obtained from the file format metadata, this
 * isn't useful for raw PCM data; in that case, use Mixer.LoadRawAudioNoCopy()
 * instead, which offers an AudioSpec.
 *
 * Once a Audio is created, it can be assigned to a Track with Track.SetAudio(),
 * or played without any management with Mixer.PlayAudio().
 *
 * When done with a Audio, it can be freed with Audio.Destroy().
 *
 * @param mixer a mixer this audio is intended to be used with. May be nullptr.
 * @param data the buffer where the audio data lives.
 * @param datalen the size, in bytes, of the buffer.
 * @param free_when_done if true, `data` will be given to free() when the Audio
 *                       is destroyed.
 * @returns an audio object that can be used to make sound on a mixer on
 *          success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Audio.Destroy
 * @sa Track.SetAudio
 * @sa Mixer.LoadRawAudioNoCopy
 * @sa Mixer.LoadAudio_IO
 */
inline Audio LoadAudioNoCopy(MixerRef mixer,
                             SourceBytes data,
                             bool free_when_done)
{
  return Audio(CheckError(MIX_LoadAudioNoCopy(
    mixer, data.data(), data.size_bytes(), free_when_done)));
}

inline Audio Mixer::LoadAudioNoCopy(SourceBytes data, bool free_when_done)
{
  return SDL::LoadAudioNoCopy(m_resource, std::move(data), free_when_done);
}

/**
 * Load audio for playback through a collection of properties.
 *
 * Please see Audio.Audio() for a description of what the various LoadAudio
 * functions do. This function uses properties to dictate how it operates, and
 * exposes functionality the other functions don't provide.
 *
 * Properties are discussed in [SDL's
 * documentation](https://wiki.libsdl.org/SDL3/CategoryProperties) .
 *
 * These are the supported properties:
 *
 * - `prop::Audio.LOAD_IOSTREAM_POINTER`: a pointer to an IOStream to be used to
 *   load audio data. Required. This stream must be able to seek!
 * - `prop::Audio.LOAD_CLOSEIO_BOOLEAN`: true if SDL_mixer should close the
 *   IOStream before returning (success or failure).
 * - `prop::Audio.LOAD_PREDECODE_BOOLEAN`: true if SDL_mixer should fully decode
 *   and decompress the data before returning. Otherwise it will be stored in
 *   its original state and decompressed on demand.
 * - `prop::Audio.LOAD_PREFERRED_MIXER_POINTER`: a pointer to a Mixer, in case
 *   steps can be made to match its format when decoding. Optional.
 * - `prop::Audio.LOAD_SKIP_METADATA_TAGS_BOOLEAN`: true to skip parsing
 *   metadata tags, like ID3 and APE tags. This can be used to speed up loading
 *   _if the data definitely doesn't have these tags_. Some decoders will fail
 *   if these tags are present when this property is true.
 * - `prop::Audio.DECODER_STRING`: the name of the decoder to use for this data.
 *   Optional. If not specified, SDL_mixer will examine the data and choose the
 *   best decoder. These names are the same returned from GetAudioDecoder().
 *
 * Specific decoders might accept additional custom properties, such as where to
 * find soundfonts for MIDI playback, etc.
 *
 * @param props a set of properties on how to load audio.
 * @returns an audio object that can be used to make sound on a mixer on
 *          success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Audio.Destroy
 * @sa Track.SetAudio
 * @sa Audio.Audio
 */
inline Audio LoadAudioWithProperties(PropertiesRef props)
{
  return Audio(props);
}

namespace prop::Audio {

constexpr auto LOAD_IOSTREAM_POINTER = MIX_PROP_AUDIO_LOAD_IOSTREAM_POINTER;

constexpr auto LOAD_CLOSEIO_BOOLEAN = MIX_PROP_AUDIO_LOAD_CLOSEIO_BOOLEAN;

constexpr auto LOAD_PREDECODE_BOOLEAN = MIX_PROP_AUDIO_LOAD_PREDECODE_BOOLEAN;

constexpr auto LOAD_PREFERRED_MIXER_POINTER =
  MIX_PROP_AUDIO_LOAD_PREFERRED_MIXER_POINTER;

constexpr auto LOAD_SKIP_METADATA_TAGS_BOOLEAN =
  MIX_PROP_AUDIO_LOAD_SKIP_METADATA_TAGS_BOOLEAN;

constexpr auto DECODER_STRING = MIX_PROP_AUDIO_DECODER_STRING;

} // namespace prop::Audio

/**
 * Load raw PCM data from an IOStream.
 *
 * There are other options for _streaming_ raw PCM: an AudioStream can be
 * connected to a track, as can an IOStream, and will read from those sources
 * on-demand when it is time to mix the audio. This function is useful for
 * loading static audio data that is meant to be played multiple times.
 *
 * This function will load the raw data in its entirety and cache it in RAM.
 *
 * Audio objects can be shared between multiple mixers. The `mixer` parameter
 * just suggests the most likely mixer to use this audio, in case some
 * optimization might be applied, but this is not required, and a nullptr mixer
 * may be specified.
 *
 * @param mixer a mixer this audio is intended to be used with. May be nullptr.
 * @param io the IOStream to load data from.
 * @param spec what format the raw data is in.
 * @param closeio true if SDL_mixer should close `io` before returning (success
 *                or failure).
 * @returns an audio object that can be used to make sound on a mixer on
 *          success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Audio.Destroy
 * @sa Track.SetAudio
 * @sa Audio.Audio
 * @sa Mixer.LoadRawAudioNoCopy
 */
inline Audio LoadRawAudio_IO(MixerRef mixer,
                             IOStreamRef io,
                             const AudioSpec& spec,
                             bool closeio = false)
{
  return Audio(mixer, io, spec, closeio);
}

inline Audio Mixer::LoadRawAudio_IO(IOStreamRef io,
                                    const AudioSpec& spec,
                                    bool closeio)
{
  return Audio(m_resource, io, spec, closeio);
}

/**
 * Load raw PCM data from a memory buffer.
 *
 * There are other options for _streaming_ raw PCM: an AudioStream can be
 * connected to a track, as can an IOStream, and will read from those sources
 * on-demand when it is time to mix the audio. This function is useful for
 * loading static audio data that is meant to be played multiple times.
 *
 * This function will load the raw data in its entirety and cache it in RAM,
 * allocating a copy. If the original data will outlive the created Audio, you
 * can use Mixer.LoadRawAudioNoCopy() to avoid extra allocations and copies.
 *
 * Audio objects can be shared between multiple mixers. The `mixer` parameter
 * just suggests the most likely mixer to use this audio, in case some
 * optimization might be applied, but this is not required, and a nullptr mixer
 * may be specified.
 *
 * @param mixer a mixer this audio is intended to be used with. May be nullptr.
 * @param data the raw PCM data to load.
 * @param datalen the size, in bytes, of the raw PCM data.
 * @param spec what format the raw data is in.
 * @returns an audio object that can be used to make sound on a mixer on
 *          success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Audio.Destroy
 * @sa Track.SetAudio
 * @sa Audio.Audio
 * @sa Mixer.LoadRawAudioNoCopy
 */
inline Audio LoadRawAudio(MixerRef mixer,
                          SourceBytes data,
                          const AudioSpec& spec)
{
  return Audio(mixer, std::move(data), spec);
}

inline Audio Mixer::LoadRawAudio(SourceBytes data, const AudioSpec& spec)
{
  return Audio(m_resource, std::move(data), spec);
}

/**
 * Load raw PCM data from a memory buffer without making a copy.
 *
 * This buffer must live for the entire time the returned Audio lives, as the
 * mixer will access the buffer whenever it needs to mix more data.
 *
 * This function is meant to maximize efficiency: if the data is already in
 * memory and can remain there, don't copy it. But it can also lead to some
 * interesting tricks, like changing the buffer's contents to alter multiple
 * playing tracks at once. (But, of course, be careful when being too clever.)
 *
 * Audio objects can be shared between multiple mixers. The `mixer` parameter
 * just suggests the most likely mixer to use this audio, in case some
 * optimization might be applied, but this is not required, and a nullptr mixer
 * may be specified.
 *
 * If `free_when_done` is true, SDL_mixer will call `free(data)` when the
 * returned Audio is eventually destroyed. This can be useful when the data is
 * not static, but rather composed dynamically for this specific Audio and
 * simply wants to avoid the extra copy.
 *
 * @param mixer a mixer this audio is intended to be used with. May be nullptr.
 * @param data the buffer where the raw PCM data lives.
 * @param datalen the size, in bytes, of the buffer.
 * @param spec what format the raw data is in.
 * @param free_when_done if true, `data` will be given to free() when the Audio
 *                       is destroyed.
 * @returns an audio object that can be used to make sound on a mixer on
 *          success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Audio.Destroy
 * @sa Track.SetAudio
 * @sa Audio.Audio
 * @sa Audio.Audio
 */
inline Audio LoadRawAudioNoCopy(MixerRef mixer,
                                SourceBytes data,
                                const AudioSpec& spec,
                                bool free_when_done)
{
  return Audio(CheckError(MIX_LoadRawAudioNoCopy(
    mixer, data.data(), data.size_bytes(), &spec, free_when_done)));
}

inline Audio Mixer::LoadRawAudioNoCopy(SourceBytes data,
                                       const AudioSpec& spec,
                                       bool free_when_done)
{
  return SDL::LoadRawAudioNoCopy(
    m_resource, std::move(data), spec, free_when_done);
}

/**
 * Create a Audio that generates a sinewave.
 *
 * This is useful just to have _something_ to play, perhaps for testing or
 * debugging purposes.
 *
 * You specify its frequency in Hz (determines the pitch of the sinewave's
 * audio) and amplitude (determines the volume of the sinewave: 1.0f is very
 * loud, 0.0f is silent).
 *
 * A number of milliseconds of audio to generate can be specified. Specifying a
 * value less than zero will generate infinite audio (when assigned to a Track,
 * the sinewave will play forever).
 *
 * Audio objects can be shared between multiple mixers. The `mixer` parameter
 * just suggests the most likely mixer to use this audio, in case some
 * optimization might be applied, but this is not required, and a nullptr mixer
 * may be specified.
 *
 * @param mixer a mixer this audio is intended to be used with. May be nullptr.
 * @param hz the sinewave's frequency in Hz.
 * @param amplitude the sinewave's amplitude from 0.0f to 1.0f.
 * @param ms the maximum number of milliseconds of audio to generate, or less
 *           than zero to generate infinite audio.
 * @returns an audio object that can be used to make sound on a mixer on
 *          success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Audio.Destroy
 * @sa Track.SetAudio
 * @sa Audio.Audio
 */
inline Audio CreateSineWaveAudio(MixerRef mixer,
                                 int hz,
                                 float amplitude,
                                 Sint64 ms)
{
  return Audio(CheckError(MIX_CreateSineWaveAudio(mixer, hz, amplitude, ms)));
}

inline Audio Mixer::CreateSineWaveAudio(int hz, float amplitude, Sint64 ms)
{
  return SDL::CreateSineWaveAudio(m_resource, hz, amplitude, ms);
}

/**
 * Get the properties associated with a Audio.
 *
 * SDL_mixer offers some properties of its own, but this can also be a
 * convenient place to store app-specific data.
 *
 * A Properties is created the first time this function is called for a given
 * Audio, if necessary.
 *
 * The following read-only properties are provided by SDL_mixer:
 *
 * - `prop::MixMetadata.TITLE_STRING`: the audio's title ("Smells Like Teen
 *   Spirit").
 * - `prop::MixMetadata.ARTIST_STRING`: the audio's artist name ("Nirvana").
 * - `prop::MixMetadata.ALBUM_STRING`: the audio's album name ("Nevermind").
 * - `prop::MixMetadata.COPYRIGHT_STRING`: the audio's copyright info
 *   ("Copyright (c) 1991")
 * - `prop::MixMetadata.TRACK_NUMBER`: the audio's track number on the album (1)
 * - `prop::MixMetadata.TOTAL_TRACKS_NUMBER`: the total tracks on the album (13)
 * - `prop::MixMetadata.YEAR_NUMBER`: the year the audio was released (1991)
 * - `prop::MixMetadata.DURATION_FRAMES_NUMBER`: The sample frames worth of PCM
 *   data that comprise this audio. It might be off by a little if the decoder
 *   only knows the duration as a unit of time.
 * - `prop::MixMetadata.DURATION_INFINITE_BOOLEAN`: if true, audio never runs
 *   out of sound to generate. This isn't necessarily always known to SDL_mixer,
 *   though.
 *
 * Other properties, documented with LoadAudioWithProperties(), may also be
 * present.
 *
 * Note that the metadata properties are whatever SDL_mixer finds in things like
 * ID3 tags, and they often have very little standardized formatting, may be
 * missing, and can be completely wrong if the original data is untrustworthy
 * (like an MP3 from a P2P file sharing service).
 *
 * @param audio the audio to query.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline PropertiesRef GetAudioProperties(AudioRef audio)
{
  return CheckError(MIX_GetAudioProperties(audio));
}

inline PropertiesRef Audio::GetProperties()
{
  return SDL::GetAudioProperties(m_resource);
}

namespace prop::MixMetadata {

constexpr auto TITLE_STRING = MIX_PROP_METADATA_TITLE_STRING;

constexpr auto ARTIST_STRING = MIX_PROP_METADATA_ARTIST_STRING;

constexpr auto ALBUM_STRING = MIX_PROP_METADATA_ALBUM_STRING;

constexpr auto COPYRIGHT_STRING = MIX_PROP_METADATA_COPYRIGHT_STRING;

constexpr auto TRACK_NUMBER = MIX_PROP_METADATA_TRACK_NUMBER;

constexpr auto TOTAL_TRACKS_NUMBER = MIX_PROP_METADATA_TOTAL_TRACKS_NUMBER;

constexpr auto YEAR_NUMBER = MIX_PROP_METADATA_YEAR_NUMBER;

constexpr auto DURATION_FRAMES_NUMBER =
  MIX_PROP_METADATA_DURATION_FRAMES_NUMBER;

constexpr auto DURATION_INFINITE_BOOLEAN =
  MIX_PROP_METADATA_DURATION_INFINITE_BOOLEAN;

} // namespace prop::MixMetadata

/**
 * Get the length of a Audio's playback in sample frames.
 *
 * This information is also available via the
 * prop::MixMetadata.DURATION_FRAMES_NUMBER property, but it's common enough to
 * provide a simple accessor function.
 *
 * This reports the length of the data in _sample frames_, so sample-perfect
 * mixing can be possible. Sample frames are only meaningful as a measure of
 * time if the sample rate (frequency) is also known. To convert from sample
 * frames to milliseconds, use Audio.FramesToMS().
 *
 * Not all audio file formats can report the complete length of the data they
 * will produce through decoding: some can't calculate it, some might produce
 * infinite audio.
 *
 * Also, some file formats can only report duration as a unit of time, which
 * means SDL_mixer might have to estimate sample frames from that information.
 * With less precision, the reported duration might be off by a few sample
 * frames in either direction.
 *
 * This will return a value >= 0 if a duration is known. It might also return
 * DURATION_UNKNOWN or DURATION_INFINITE.
 *
 * @param audio the audio to query.
 * @returns the length of the audio in sample frames, or DURATION_UNKNOWN or
 *          DURATION_INFINITE.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline Sint64 GetAudioDuration(AudioRef audio)
{
  return MIX_GetAudioDuration(audio);
}

inline Sint64 Audio::GetDuration() { return SDL::GetAudioDuration(m_resource); }

/// Unknown duration, when the length of the audio can't be determined.
constexpr Sint64 DURATION_UNKNOWN = MIX_DURATION_UNKNOWN;

/// Infinite duration, when the audio never runs out of sound to generate.
constexpr Sint64 DURATION_INFINITE = MIX_DURATION_INFINITE;

/**
 * Query the initial audio format of a Audio.
 *
 * Note that some audio files can change format in the middle; some explicitly
 * support this, but a more common example is two MP3 files concatenated
 * together. In many cases, SDL_mixer will correctly handle these sort of files,
 * but this function will only report the initial format a file uses.
 *
 * @param audio the audio to query.
 * @param spec on success, audio format details will be stored here.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline void GetAudioFormat(AudioRef audio, AudioSpec* spec)
{
  CheckError(MIX_GetAudioFormat(audio, spec));
}

inline void Audio::GetFormat(AudioSpec* spec)
{
  SDL::GetAudioFormat(m_resource, spec);
}

/**
 * Destroy the specified audio.
 *
 * Audio is reference-counted internally, so this function only unrefs it. If
 * doing so causes the reference count to drop to zero, the Audio will be
 * deallocated. This allows the system to safely operate if the audio is still
 * assigned to a Track at the time of destruction. The actual destroying will
 * happen when the track stops using it.
 *
 * But from the caller's perspective, once this function is called, it should
 * assume the `audio` pointer has become invalid.
 *
 * Destroying a nullptr Audio is a legal no-op.
 *
 * @param audio the audio to destroy.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline void DestroyAudio(AudioRaw audio) { MIX_DestroyAudio(audio); }

inline void Audio::Destroy() { DestroyAudio(release()); }

/**
 * Create a new track on a mixer.
 *
 * A track provides a single source of audio. All currently-playing tracks will
 * be processed and mixed together to form the final output from the mixer.
 *
 * There are no limits to the number of tracks one may create, beyond running
 * out of memory, but in normal practice there are a small number of tracks that
 * are reused between all loaded audio as appropriate.
 *
 * Tracks are unique to a specific Mixer and can't be transferred between them.
 *
 * @param mixer the mixer on which to create this track.
 * @returns a new Track on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Destroy
 */
inline Track CreateTrack(MixerRef mixer) { return Track(mixer); }

inline TrackRef Mixer::CreateTrack() { return Track(m_resource); }

inline Track::Track(MixerRef mixer)
  : m_resource(CheckError(MIX_CreateTrack(mixer)))
{
}

/**
 * Destroy the specified track.
 *
 * If the track is currently playing, it will be stopped immediately, without
 * any fadeout. If there is a callback set through Track.SetStoppedCallback(),
 * it will _not_ be called.
 *
 * If the mixer is currently mixing in another thread, this will block until it
 * finishes.
 *
 * Destroying a nullptr Track is a legal no-op.
 *
 * @param track the track to destroy.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline void DestroyTrack(TrackRaw track) { MIX_DestroyTrack(track); }

inline void Track::Destroy() { DestroyTrack(release()); }

/**
 * Get the properties associated with a track.
 *
 * Currently SDL_mixer assigns no properties of its own to a track, but this can
 * be a convenient place to store app-specific data.
 *
 * A Properties is created the first time this function is called for a given
 * track.
 *
 * @param track the track to query.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline PropertiesRef GetTrackProperties(TrackRef track)
{
  return CheckError(MIX_GetTrackProperties(track));
}

inline PropertiesRef Track::GetProperties()
{
  return SDL::GetTrackProperties(m_resource);
}

/**
 * Get the Mixer that owns a Track.
 *
 * This is the mixer pointer that was passed to Track.Track().
 *
 * @param track the track to query.
 * @returns the mixer associated with the track on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline MixerRef GetTrackMixer(TrackRef track)
{
  return CheckError(MIX_GetTrackMixer(track));
}

inline MixerRef Track::GetMixer() { return SDL::GetTrackMixer(m_resource); }

/**
 * Set a Track's input to a Audio.
 *
 * A Audio is audio data stored in RAM (possibly still in a compressed form).
 * One Audio can be assigned to multiple tracks at once.
 *
 * Once a track has a valid input, it can start mixing sound by calling
 * Track.Play(), or possibly Mixer.PlayTag().
 *
 * Calling this function with a nullptr audio input is legal, and removes any
 * input from the track. If the track was currently playing, the next time the
 * mixer runs, it'll notice this and mark the track as stopped, calling any
 * assigned TrackStoppedCallback.
 *
 * It is legal to change the input of a track while it's playing, however some
 * states, like loop points, may cease to make sense with the new audio. In such
 * a case, one can call Track.Play again to adjust parameters.
 *
 * The track will hold a reference to the provided Audio, so it is safe to call
 * Audio.Destroy() on it while the track is still using it. The track will drop
 * its reference (and possibly free the resources) once it is no longer using
 * the Audio.
 *
 * @param track the track on which to set a new audio input.
 * @param audio the new audio input to set. May be nullptr.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline void SetTrackAudio(TrackRef track, AudioRef audio)
{
  CheckError(MIX_SetTrackAudio(track, audio));
}

inline void Track::SetAudio(AudioRef audio)
{
  SDL::SetTrackAudio(m_resource, audio);
}

/**
 * Set a Track's input to an AudioStream.
 *
 * Using an audio stream allows the application to generate any type of audio,
 * in any format, possibly procedurally or on-demand, and mix in with all other
 * tracks.
 *
 * When a track uses an audio stream, it will call AudioStream.GetData as it
 * needs more audio to mix. The app can either buffer data to the stream ahead
 * of time, or set a callback on the stream to provide data as needed. Please
 * refer to SDL's documentation for details.
 *
 * A given audio stream may only be assigned to a single track at a time;
 * duplicate assignments won't return an error, but assigning a stream to
 * multiple tracks will cause each track to read from the stream arbitrarily,
 * causing confusion and incorrect mixing.
 *
 * Once a track has a valid input, it can start mixing sound by calling
 * Track.Play(), or possibly Mixer.PlayTag().
 *
 * Calling this function with a nullptr audio stream is legal, and removes any
 * input from the track. If the track was currently playing, the next time the
 * mixer runs, it'll notice this and mark the track as stopped, calling any
 * assigned TrackStoppedCallback.
 *
 * It is legal to change the input of a track while it's playing, however some
 * states, like loop points, may cease to make sense with the new audio. In such
 * a case, one can call Track.Play again to adjust parameters.
 *
 * The provided audio stream must remain valid until the track no longer needs
 * it (either by changing the track's input or destroying the track).
 *
 * @param track the track on which to set a new audio input.
 * @param stream the audio stream to use as the track's input.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline void SetTrackAudioStream(TrackRef track, AudioStreamRef stream)
{
  CheckError(MIX_SetTrackAudioStream(track, stream));
}

inline void Track::SetAudioStream(AudioStreamRef stream)
{
  SDL::SetTrackAudioStream(m_resource, stream);
}

/**
 * Set a Track's input to an IOStream.
 *
 * This is not the recommended way to set a track's input, but this can be
 * useful for a very specific scenario: a large file, to be played once, that
 * must be read from disk in small chunks as needed. In most cases, however, it
 * is preferable to create a Audio ahead of time and use Track.SetAudio()
 * instead.
 *
 * The stream supplied here should provide an audio file in a supported format.
 * SDL_mixer will parse it during this call to make sure it's valid, and then
 * will read file data from the stream as it needs to decode more during mixing.
 *
 * The stream must be able to seek through the complete set of data, or this
 * function will fail.
 *
 * A given IOStream may only be assigned to a single track at a time; duplicate
 * assignments won't return an error, but assigning a stream to multiple tracks
 * will cause each track to read from the stream arbitrarily, causing confusion,
 * incorrect mixing, or failure to decode.
 *
 * Once a track has a valid input, it can start mixing sound by calling
 * Track.Play(), or possibly Mixer.PlayTag().
 *
 * Calling this function with a nullptr stream is legal, and removes any input
 * from the track. If the track was currently playing, the next time the mixer
 * runs, it'll notice this and mark the track as stopped, calling any assigned
 * TrackStoppedCallback.
 *
 * It is legal to change the input of a track while it's playing, however some
 * states, like loop points, may cease to make sense with the new audio. In such
 * a case, one can call Track.Play again to adjust parameters.
 *
 * The provided stream must remain valid until the track no longer needs it
 * (either by changing the track's input or destroying the track).
 *
 * @param track the track on which to set a new audio input.
 * @param io the new i/o stream to use as the track's input.
 * @param closeio if true, close the stream when done with it.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.SetRawIOStream
 */
inline void SetTrackIOStream(TrackRef track, IOStreamRef io, bool closeio)
{
  CheckError(MIX_SetTrackIOStream(track, io, closeio));
}

inline void Track::SetIOStream(IOStreamRef io, bool closeio)
{
  SDL::SetTrackIOStream(m_resource, io, closeio);
}

/**
 * Set a Track's input to an IOStream providing raw PCM data.
 *
 * This is not the recommended way to set a track's input, but this can be
 * useful for a very specific scenario: a large file, to be played once, that
 * must be read from disk in small chunks as needed. In most cases, however, it
 * is preferable to create a Audio ahead of time and use Track.SetAudio()
 * instead.
 *
 * Also, an Track.SetAudioStream() can _also_ provide raw PCM audio to a track,
 * via an AudioStream, which might be preferable unless the data is already
 * coming directly from an IOStream.
 *
 * The stream supplied here should provide an audio in raw PCM format.
 *
 * A given IOStream may only be assigned to a single track at a time; duplicate
 * assignments won't return an error, but assigning a stream to multiple tracks
 * will cause each track to read from the stream arbitrarily, causing confusion
 * and incorrect mixing.
 *
 * Once a track has a valid input, it can start mixing sound by calling
 * Track.Play(), or possibly Mixer.PlayTag().
 *
 * Calling this function with a nullptr stream is legal, and removes any input
 * from the track. If the track was currently playing, the next time the mixer
 * runs, it'll notice this and mark the track as stopped, calling any assigned
 * TrackStoppedCallback.
 *
 * It is legal to change the input of a track while it's playing, however some
 * states, like loop points, may cease to make sense with the new audio. In such
 * a case, one can call Track.Play again to adjust parameters.
 *
 * The provided stream must remain valid until the track no longer needs it
 * (either by changing the track's input or destroying the track).
 *
 * @param track the track on which to set a new audio input.
 * @param io the new i/o stream to use as the track's input.
 * @param spec the format of the PCM data that the IOStream will provide.
 * @param closeio if true, close the stream when done with it.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.SetAudioStream
 * @sa Track.SetIOStream
 */
inline void SetTrackRawIOStream(TrackRef track,
                                IOStreamRef io,
                                const AudioSpec& spec,
                                bool closeio)
{
  CheckError(MIX_SetTrackRawIOStream(track, io, &spec, closeio));
}

inline void Track::SetRawIOStream(IOStreamRef io,
                                  const AudioSpec& spec,
                                  bool closeio)
{
  SDL::SetTrackRawIOStream(m_resource, io, spec, closeio);
}

/**
 * Assign an arbitrary tag to a track.
 *
 * A tag can be any valid C string in UTF-8 encoding. It can be useful to group
 * tracks in various ways. For example, everything in-game might be marked as
 * "game", so when the user brings up the settings menu, the app can pause all
 * tracks involved in gameplay at once, but keep background music and menu sound
 * effects running.
 *
 * A track can have as many tags as desired, until the machine runs out of
 * memory.
 *
 * It's legal to add the same tag to a track more than once; the extra attempts
 * will report success but not change anything.
 *
 * Tags can later be removed with Track.Untag().
 *
 * @param track the track to add a tag to.
 * @param tag the tag to add.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Untag
 */
inline void TagTrack(TrackRef track, StringParam tag)
{
  CheckError(MIX_TagTrack(track, tag));
}

inline void Track::Tag(StringParam tag)
{
  SDL::TagTrack(m_resource, std::move(tag));
}

/**
 * Remove an arbitrary tag from a track.
 *
 * A tag can be any valid C string in UTF-8 encoding. It can be useful to group
 * tracks in various ways. For example, everything in-game might be marked as
 * "game", so when the user brings up the settings menu, the app can pause all
 * tracks involved in gameplay at once, but keep background music and menu sound
 * effects running.
 *
 * It's legal to remove a tag that the track doesn't have; this function doesn't
 * report errors, so this simply does nothing.
 *
 * Specifying a nullptr tag will remove all tags on a track.
 *
 * @param track the track from which to remove a tag.
 * @param tag the tag to remove, or nullptr to remove all current tags.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Tag
 */
inline void UntagTrack(TrackRef track, StringParam tag)
{
  MIX_UntagTrack(track, tag);
}

inline void Track::Untag(StringParam tag)
{
  SDL::UntagTrack(m_resource, std::move(tag));
}

/**
 * Get the tags currently associated with a track.
 *
 * Tags are not provided in any guaranteed order.
 *
 * @param track the track to query.
 * @param count a pointer filled in with the number of tags returned, can be
 *              nullptr.
 * @returns an array of the tags, nullptr-terminated on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline char** GetTrackTags(TrackRef track, int* count)
{
  return CheckError(MIX_GetTrackTags(track, count));
}

inline char** Track::GetTags(int* count)
{
  return SDL::GetTrackTags(m_resource, count);
}

/**
 * Get all tracks with a specific tag.
 *
 * Tracks are not provided in any guaranteed order.
 *
 * @param mixer the mixer to query.
 * @param tag the tag to search.
 * @param count a pointer filled in with the number of tracks returned, can be
 *              nullptr.
 * @returns an array of the tracks, nullptr-terminated on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline MIX_Track** GetTaggedTracks(MixerRef mixer, StringParam tag, int* count)
{
  return CheckError(MIX_GetTaggedTracks(mixer, tag, count));
}

inline MIX_Track** Mixer::GetTaggedTracks(StringParam tag, int* count)
{
  return SDL::GetTaggedTracks(m_resource, std::move(tag), count);
}

/**
 * Seek a playing track to a new position in its input.
 *
 * (Not to be confused with Track.Set3DPosition(), which is positioning of the
 * track in 3D space, not the playback position of its audio data.)
 *
 * On a playing track, the next time the mixer runs, it will start mixing from
 * the new position.
 *
 * Position is defined in _sample frames_ of decoded audio, not units of time,
 * so that sample-perfect mixing can be achieved. To instead operate in units of
 * time, use Track.MSToFrames() to get the approximate sample frames for a given
 * tick.
 *
 * This function requires an input that can seek (so it can not be used if the
 * input was set with Track.SetAudioStream()), and a audio file format that
 * allows seeking. SDL_mixer's decoders for some file formats do not offer
 * seeking, or can only seek to times, not exact sample frames, in which case
 * the final position may be off by some amount of sample frames. Please check
 * your audio data and file bug reports if appropriate.
 *
 * It's legal to call this function on a track that is stopped, but a future
 * call to Track.Play() will reset the start position anyhow. Paused tracks will
 * resume at the new input position.
 *
 * @param track the track to change.
 * @param frames the sample frame position to seek to.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.GetPlaybackPosition
 */
inline void SetTrackPlaybackPosition(TrackRef track, Sint64 frames)
{
  CheckError(MIX_SetTrackPlaybackPosition(track, frames));
}

inline void Track::SetPlaybackPosition(Sint64 frames)
{
  SDL::SetTrackPlaybackPosition(m_resource, frames);
}

/**
 * Get the current input position of a playing track.
 *
 * (Not to be confused with Track.Get3DPosition(), which is positioning of the
 * track in 3D space, not the playback position of its audio data.)
 *
 * Position is defined in _sample frames_ of decoded audio, not units of time,
 * so that sample-perfect mixing can be achieved. To instead operate in units of
 * time, use Track.FramesToMS() to convert the return value to milliseconds.
 *
 * Stopped and paused tracks will report the position when they halted. Playing
 * tracks will report the current position, which will change over time.
 *
 * @param track the track to change.
 * @returns the track's current sample frame position, or -1 on error; call
 *          GetError() for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.SetPlaybackPosition
 */
inline Sint64 GetTrackPlaybackPosition(TrackRef track)
{
  return MIX_GetTrackPlaybackPosition(track);
}

inline Sint64 Track::GetPlaybackPosition()
{
  return SDL::GetTrackPlaybackPosition(m_resource);
}

/**
 * Query whether a given track is fading.
 *
 * This specifically checks if the track is _not stopped_ (paused or playing),
 * and it is fading in or out, and returns the number of frames remaining in the
 * fade.
 *
 * If fading out, the returned value will be negative. When fading in, the
 * returned value will be positive. If not fading, this function returns zero.
 *
 * On various errors (MIX.Init() was not called, the track is nullptr), this
 * returns 0, but there is no mechanism to distinguish errors from tracks that
 * aren't fading.
 *
 * @param track the track to query.
 * @returns less than 0 if the track is fading out, greater than 0 if fading in,
 *          zero otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline Sint64 GetTrackFadeFrames(TrackRef track)
{
  return MIX_GetTrackFadeFrames(track);
}

inline Sint64 Track::GetFadeFrames()
{
  return SDL::GetTrackFadeFrames(m_resource);
}

/**
 * Query how many loops remain for a given track.
 *
 * This returns the number of loops still pending; if a track will eventually
 * complete and loop to play again one more time, this will return 1. If a track
 * _was_ looping but is on its final iteration of the loop (will stop when this
 * iteration completes), this will return zero.
 *
 * A track that is looping infinitely will return -1. This value does not report
 * an error in this case.
 *
 * A track that is stopped (not playing and not paused) will have zero loops
 * remaining.
 *
 * On various errors (MIX.Init() was not called, the track is nullptr), this
 * returns zero, but there is no mechanism to distinguish errors from
 * non-looping tracks.
 *
 * @param track the track to query.
 * @returns the number of pending loops, zero if not looping, and -1 if looping
 *          infinitely.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline int GetTrackLoops(TrackRef track) { return MIX_GetTrackLoops(track); }

inline int Track::GetLoops() { return SDL::GetTrackLoops(m_resource); }

/**
 * Change the number of times a currently-playing track will loop.
 *
 * This replaces any previously-set remaining loops. A value of 1 will loop to
 * the start of playback one time. Zero will not loop at all. A value of -1
 * requests infinite loops. If the input is not seekable and `num_loops` isn't
 * zero, this function will report success but the track will stop at the point
 * it should loop.
 *
 * The new loop count replaces any previous state, even if the track has already
 * looped.
 *
 * This has no effect on a track that is stopped, or rather, starting a stopped
 * track later will set a new loop count, replacing this value. Stopped tracks
 * can specify a loop count while starting via prop::Play.LOOPS_NUMBER. This
 * function is intended to alter that count in the middle of playback.
 *
 * @param track the track to configure.
 * @param num_loops new number of times to loop. Zero to disable looping, -1 to
 *                  loop infinitely.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.GetLoops
 */
inline void SetTrackLoops(TrackRef track, int num_loops)
{
  CheckError(MIX_SetTrackLoops(track, num_loops));
}

inline void Track::SetLoops(int num_loops)
{
  SDL::SetTrackLoops(m_resource, num_loops);
}

/**
 * Query the Audio assigned to a track.
 *
 * This returns the Audio object currently assigned to `track` through a call to
 * Track.SetAudio(). If there is none assigned, or the track has an input that
 * isn't a Audio (such as an AudioStream or IOStream), this will return nullptr.
 *
 * On various errors (MIX.Init() was not called, the track is nullptr), this
 * returns nullptr, but there is no mechanism to distinguish errors from tracks
 * without a valid input.
 *
 * @param track the track to query.
 * @returns a Audio if available, nullptr if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.GetAudioStream
 */
inline AudioRef GetTrackAudio(TrackRef track)
{
  return MIX_GetTrackAudio(track);
}

inline AudioRef Track::GetAudio() { return SDL::GetTrackAudio(m_resource); }

/**
 * Query the AudioStream assigned to a track.
 *
 * This returns the AudioStream object currently assigned to `track` through a
 * call to Track.SetAudioStream(). If there is none assigned, or the track has
 * an input that isn't an AudioStream (such as a Audio or IOStream), this will
 * return nullptr.
 *
 * On various errors (MIX.Init() was not called, the track is nullptr), this
 * returns nullptr, but there is no mechanism to distinguish errors from tracks
 * without a valid input.
 *
 * @param track the track to query.
 * @returns an AudioStream if available, nullptr if not.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.GetAudio
 */
inline AudioStreamRef GetTrackAudioStream(TrackRef track)
{
  return MIX_GetTrackAudioStream(track);
}

inline AudioStreamRef Track::GetAudioStream()
{
  return SDL::GetTrackAudioStream(m_resource);
}

/**
 * Return the number of sample frames remaining to be mixed in a track.
 *
 * If the track is playing or paused, and its total duration is known, this will
 * report how much audio is left to mix. If the track is playing, future calls
 * to this function will report different values.
 *
 * Remaining audio is defined in _sample frames_ of decoded audio, not units of
 * time, so that sample-perfect mixing can be achieved. To instead operate in
 * units of time, use Track.FramesToMS() to convert the return value to
 * milliseconds.
 *
 * This function does not take into account fade-outs or looping, just the
 * current mixing position vs the duration of the track.
 *
 * If the duration of the track isn't known, or `track` is nullptr, this
 * function returns -1. A stopped track reports 0.
 *
 * @param track the track to query.
 * @returns the total sample frames still to be mixed, or -1 if unknown.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline Sint64 GetTrackRemaining(TrackRef track)
{
  return MIX_GetTrackRemaining(track);
}

inline Sint64 Track::GetRemaining()
{
  return SDL::GetTrackRemaining(m_resource);
}

/**
 * Convert milliseconds to sample frames for a track's current format.
 *
 * This calculates time based on the track's current input format, which can
 * change when its input does, and also if that input changes formats mid-stream
 * (for example, if decoding a file that is two MP3s concatenated together).
 *
 * On various errors (MIX.Init() was not called, the track is nullptr), this
 * returns -1. If the track has no input, this returns -1. If `ms` is < 0, this
 * returns -1.
 *
 * @param track the track to query.
 * @param ms the milliseconds to convert to track-specific sample frames.
 * @returns Converted number of sample frames, or -1 for errors/no input; call
 *          GetError() for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.FramesToMS
 */
inline Sint64 TrackMSToFrames(TrackRef track, Sint64 ms)
{
  return MIX_TrackMSToFrames(track, ms);
}

inline Sint64 Track::MSToFrames(Sint64 ms)
{
  return SDL::TrackMSToFrames(m_resource, ms);
}

/**
 * Convert sample frames for a track's current format to milliseconds.
 *
 * This calculates time based on the track's current input format, which can
 * change when its input does, and also if that input changes formats mid-stream
 * (for example, if decoding a file that is two MP3s concatenated together).
 *
 * Sample frames are more precise than milliseconds, so out of necessity, this
 * function will approximate by rounding down to the closest full millisecond.
 *
 * On various errors (MIX.Init() was not called, the track is nullptr), this
 * returns -1. If the track has no input, this returns -1. If `frames` is < 0,
 * this returns -1.
 *
 * @param track the track to query.
 * @param frames the track-specific sample frames to convert to milliseconds.
 * @returns Converted number of milliseconds, or -1 for errors/no input; call
 *          GetError() for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.MSToFrames
 */
inline Sint64 TrackFramesToMS(TrackRef track, Sint64 frames)
{
  return MIX_TrackFramesToMS(track, frames);
}

inline Sint64 Track::FramesToMS(Sint64 frames)
{
  return SDL::TrackFramesToMS(m_resource, frames);
}

/**
 * Convert milliseconds to sample frames for a Audio's format.
 *
 * This calculates time based on the audio's initial format, even if the format
 * would change mid-stream.
 *
 * If `ms` is < 0, this returns -1.
 *
 * @param audio the audio to query.
 * @param ms the milliseconds to convert to audio-specific sample frames.
 * @returns Converted number of sample frames, or -1 for errors/no input; call
 *          GetError() for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Audio.FramesToMS
 */
inline Sint64 AudioMSToFrames(AudioRef audio, Sint64 ms)
{
  return MIX_AudioMSToFrames(audio, ms);
}

inline Sint64 Audio::MSToFrames(Sint64 ms)
{
  return SDL::AudioMSToFrames(m_resource, ms);
}

/**
 * Convert sample frames for a Audio's format to milliseconds.
 *
 * This calculates time based on the audio's initial format, even if the format
 * would change mid-stream.
 *
 * Sample frames are more precise than milliseconds, so out of necessity, this
 * function will approximate by rounding down to the closest full millisecond.
 *
 * If `frames` is < 0, this returns -1.
 *
 * @param audio the audio to query.
 * @param frames the audio-specific sample frames to convert to milliseconds.
 * @returns Converted number of milliseconds, or -1 for errors/no input; call
 *          GetError() for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Audio.MSToFrames
 */
inline Sint64 AudioFramesToMS(AudioRef audio, Sint64 frames)
{
  return MIX_AudioFramesToMS(audio, frames);
}

inline Sint64 Audio::FramesToMS(Sint64 frames)
{
  return SDL::AudioFramesToMS(m_resource, frames);
}

/**
 * Convert milliseconds to sample frames at a specific sample rate.
 *
 * If `sample_rate` is <= 0, this returns -1. If `ms` is < 0, this returns -1.
 *
 * @param sample_rate the sample rate to use for conversion.
 * @param ms the milliseconds to convert to rate-specific sample frames.
 * @returns Converted number of sample frames, or -1 for errors; call GetError()
 *          for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa FramesToMS
 */
inline Sint64 MSToFrames(int sample_rate, Sint64 ms)
{
  return MIX_MSToFrames(sample_rate, ms);
}

/**
 * Convert sample frames, at a specific sample rate, to milliseconds.
 *
 * Sample frames are more precise than milliseconds, so out of necessity, this
 * function will approximate by rounding down to the closest full millisecond.
 *
 * If `sample_rate` is <= 0, this returns -1. If `frames` is < 0, this returns
 *
 * - 1.
 *
 * @param sample_rate the sample rate to use for conversion.
 * @param frames the rate-specific sample frames to convert to milliseconds.
 * @returns Converted number of milliseconds, or -1 for errors; call GetError()
 *          for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa MSToFrames
 */
inline Sint64 FramesToMS(int sample_rate, Sint64 frames)
{
  return MIX_FramesToMS(sample_rate, frames);
}

/**
 * Start (or restart) mixing a track for playback.
 *
 * The track will use whatever input was last assigned to it when playing; an
 * input must be assigned to this track or this function will fail. Inputs are
 * assigned with calls to Track.SetAudio(), Track.SetAudioStream(), or
 * Track.SetIOStream().
 *
 * If the track is already playing, or paused, this will restart the track with
 * the newly-specified parameters.
 *
 * As there are several parameters, and more may be added in the future, they
 * are specified with an Properties. The parameters have reasonable defaults,
 * and specifying a 0 for `options` will choose defaults for everything.
 *
 * Properties are discussed in [SDL's
 * documentation](https://wiki.libsdl.org/SDL3/CategoryProperties) .
 *
 * These are the supported properties:
 *
 * - `prop::Play.LOOPS_NUMBER`: The number of times to loop the track when it
 *   reaches the end. A value of 1 will loop to the start one time. Zero will
 *   not loop at all. A value of -1 requests infinite loops. If the input is not
 *   seekable and this value isn't zero, this function will report success but
 *   the track will stop at the point it should loop. Default 0.
 * - `prop::Play.MAX_FRAME_NUMBER`: Mix at most to this sample frame position in
 *   the track. This will be treated as if the input reach EOF at this point in
 *   the audio file. If -1, mix all available audio without a limit. Default -1.
 * - `prop::Play.MAX_MILLISECONDS_NUMBER`: The same as using the
 *   prop::Play.MAX_FRAME_NUMBER property, but the value is specified in
 *   milliseconds instead of sample frames. If both properties are specified,
 *   the sample frames value is favored. Default -1.
 * - `prop::Play.START_FRAME_NUMBER`: Start mixing from this sample frame
 *   position in the track's input. A value <= 0 will begin from the start of
 *   the track's input. If the input is not seekable and this value is > 0, this
 *   function will report failure. Default 0.
 * - `prop::Play.START_MILLISECOND_NUMBER`: The same as using the
 *   prop::Play.START_FRAME_NUMBER property, but the value is specified in
 *   milliseconds instead of sample frames. If both properties are specified,
 *   the sample frames value is favored. Default 0.
 * - `prop::Play.LOOP_START_FRAME_NUMBER`: If the track is looping, this is the
 *   sample frame position that the track will loop back to; this lets one play
 *   an intro at the start of a track on the first iteration, but have a loop
 *   point somewhere in the middle thereafter. A value <= 0 will begin the loop
 *   from the start of the track's input. Default 0.
 * - `prop::Play.LOOP_START_MILLISECOND_NUMBER`: The same as using the
 *   prop::Play.LOOP_START_FRAME_NUMBER property, but the value is specified in
 *   milliseconds instead of sample frames. If both properties are specified,
 *   the sample frames value is favored. Default 0.
 * - `prop::Play.FADE_IN_FRAMES_NUMBER`: The number of sample frames over which
 *   to fade in the newly-started track. The track will begin mixing silence and
 *   reach full volume smoothly over this many sample frames. If the track loops
 *   before the fade-in is complete, it will continue to fade correctly from the
 *   loop point. A value <= 0 will disable fade-in, so the track starts mixing
 *   at full volume. Default 0.
 * - `prop::Play.FADE_IN_MILLISECONDS_NUMBER`: The same as using the
 *   prop::Play.FADE_IN_FRAMES_NUMBER property, but the value is specified in
 *   milliseconds instead of sample frames. If both properties are specified,
 *   the sample frames value is favored. Default 0.
 * - `prop::Play.FADE_IN_START_GAIN_FLOAT`: If fading in, start fading from this
 *   volume level. 0.0f is silence and 1.0f is full volume, every in between is
 *   a linear change in gain. The specified value will be clamped between 0.0f
 *   and 1.0f. Default 0.0f.
 * - `prop::Play.APPEND_SILENCE_FRAMES_NUMBER`: At the end of mixing this track,
 *   after all loops are complete, append this many sample frames of silence as
 *   if it were part of the audio file. This allows for apps to implement
 *   effects in callbacks, like reverb, that need to generate samples past the
 *   end of the stream's audio, or perhaps introduce a delay before starting a
 *   new sound on the track without having to manage it directly. A value <= 0
 *   generates no silence before stopping the track. Default 0.
 * - `prop::Play.APPEND_SILENCE_MILLISECONDS_NUMBER`: The same as using the
 *   prop::Play.APPEND_SILENCE_FRAMES_NUMBER property, but the value is
 *   specified in milliseconds instead of sample frames. If both properties are
 *   specified, the sample frames value is favored. Default 0.
 * - `prop::Play.HALT_WHEN_EXHAUSTED_BOOLEAN`: If true, when input is completely
 *   consumed for the track, the mixer will mark the track as stopped (and call
 *   any appropriate TrackStoppedCallback, etc); to play more, the track will
 *   need to be restarted. If false, the track will just not contribute to the
 *   mix, but it will not be marked as stopped. There may be clever logic tricks
 *   this exposes generally, but this property is specifically useful when the
 *   track's input is an AudioStream assigned via Track.SetAudioStream().
 *   Setting this property to true can be useful when pushing a complete piece
 *   of audio to the stream that has a definite ending, as the track will
 *   operate like any other audio was applied. Setting to false means as new
 *   data is added to the stream, the mixer will start using it as soon as
 *   possible, which is useful when audio should play immediately as it drips
 *   in: new VoIP packets, etc. Note that in this situation, if the audio runs
 *   out when needed, there _will_ be gaps in the mixed output, so try to buffer
 *   enough data to avoid this when possible. Note that a track is not consider
 *   exhausted until all its loops and appended silence have been mixed (and
 *   also, that loops don't mean anything when the input is an AudioStream).
 *   Default true.
 *
 * If this function fails, mixing of this track will not start (or restart, if
 * it was already started).
 *
 * @param track the track to start (or restart) mixing.
 * @param options a set of properties that control playback. May be zero.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Mixer.PlayTag
 * @sa Mixer.PlayAudio
 * @sa Track.Stop
 * @sa Track.Pause
 * @sa Track.Playing
 */
inline void PlayTrack(TrackRef track, PropertiesRef options)
{
  CheckError(MIX_PlayTrack(track, options));
}

inline void Track::Play(PropertiesRef options)
{
  SDL::PlayTrack(m_resource, options);
}

namespace prop::Play {

constexpr auto LOOPS_NUMBER = MIX_PROP_PLAY_LOOPS_NUMBER;

constexpr auto MAX_FRAME_NUMBER = MIX_PROP_PLAY_MAX_FRAME_NUMBER;

constexpr auto MAX_MILLISECONDS_NUMBER = MIX_PROP_PLAY_MAX_MILLISECONDS_NUMBER;

constexpr auto START_FRAME_NUMBER = MIX_PROP_PLAY_START_FRAME_NUMBER;

constexpr auto START_MILLISECOND_NUMBER =
  MIX_PROP_PLAY_START_MILLISECOND_NUMBER;

constexpr auto LOOP_START_FRAME_NUMBER = MIX_PROP_PLAY_LOOP_START_FRAME_NUMBER;

constexpr auto LOOP_START_MILLISECOND_NUMBER =
  MIX_PROP_PLAY_LOOP_START_MILLISECOND_NUMBER;

constexpr auto FADE_IN_FRAMES_NUMBER = MIX_PROP_PLAY_FADE_IN_FRAMES_NUMBER;

constexpr auto FADE_IN_MILLISECONDS_NUMBER =
  MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER;

constexpr auto FADE_IN_START_GAIN_FLOAT =
  MIX_PROP_PLAY_FADE_IN_START_GAIN_FLOAT;

constexpr auto APPEND_SILENCE_FRAMES_NUMBER =
  MIX_PROP_PLAY_APPEND_SILENCE_FRAMES_NUMBER;

constexpr auto APPEND_SILENCE_MILLISECONDS_NUMBER =
  MIX_PROP_PLAY_APPEND_SILENCE_MILLISECONDS_NUMBER;

constexpr auto HALT_WHEN_EXHAUSTED_BOOLEAN =
  MIX_PROP_PLAY_HALT_WHEN_EXHAUSTED_BOOLEAN;

} // namespace prop::Play

/**
 * Start (or restart) mixing all tracks with a specific tag for playback.
 *
 * This function follows all the same rules as Track.Play(); please refer to its
 * documentation for the details. Unlike that function, Mixer.PlayTag() operates
 * on multiple tracks at once that have the specified tag applied, via
 * Track.Tag().
 *
 * If all of your tagged tracks have different sample rates, it would make sense
 * to use the `*_MILLISECONDS_NUMBER` properties in your `options`, instead of
 * `*_FRAMES_NUMBER`, and let SDL_mixer figure out how to apply it to each
 * track.
 *
 * This function returns true if all tagged tracks are started (or restarted).
 * If any track fails, this function returns false, but all tracks that could
 * start will still be started even when this function reports failure.
 *
 * From the point of view of the mixing process, all tracks that successfully
 * (re)start will do so at the exact same moment.
 *
 * @param mixer the mixer on which to look for tagged tracks.
 * @param tag the tag to use when searching for tracks.
 * @param options the set of options that will be applied to each track.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Play
 * @sa Track.Tag
 * @sa Track.Stop
 * @sa Track.Pause
 * @sa Track.Playing
 */
inline void PlayTag(MixerRef mixer, StringParam tag, PropertiesRef options)
{
  CheckError(MIX_PlayTag(mixer, tag, options));
}

inline void Mixer::PlayTag(StringParam tag, PropertiesRef options)
{
  SDL::PlayTag(m_resource, std::move(tag), options);
}

/**
 * Play a Audio from start to finish without any management.
 *
 * This is what we term a "fire-and-forget" sound. Internally, SDL_mixer will
 * manage a temporary track to mix the specified Audio, cleaning it up when
 * complete. No options can be provided for how to do the mixing, like
 * Track.Play() offers, and since the track is not available to the caller, no
 * adjustments can be made to mixing over time.
 *
 * This is not the function to build an entire game of any complexity around,
 * but it can be convenient to play simple, one-off sounds that can't be stopped
 * early. An example would be a voice saying "GAME OVER" during an unpausable
 * endgame sequence.
 *
 * There are no limits to the number of fire-and-forget sounds that can mix at
 * once (short of running out of memory), and SDL_mixer keeps an internal pool
 * of temporary tracks it creates as needed and reuses when available.
 *
 * @param mixer the mixer on which to play this audio.
 * @param audio the audio input to play.
 * @returns true if the track has begun mixing, false on error; call GetError()
 *          for details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Play
 * @sa Audio.Audio
 */
inline bool PlayAudio(MixerRef mixer, AudioRef audio)
{
  return MIX_PlayAudio(mixer, audio);
}

inline bool Mixer::PlayAudio(AudioRef audio)
{
  return SDL::PlayAudio(m_resource, audio);
}

/**
 * Halt a currently-playing track, possibly fading out over time.
 *
 * If `fade_out_frames` is > 0, the track does not stop mixing immediately, but
 * rather fades to silence over that many sample frames before stopping. Sample
 * frames are specific to the input assigned to the track, to allow for
 * sample-perfect mixing. Track.MSToFrames() can be used to convert milliseconds
 * to an appropriate value here.
 *
 * If the track ends normally while the fade-out is still in progress, the audio
 * stops there; the fade is not adjusted to be shorter if it will last longer
 * than the audio remaining.
 *
 * Once a track has completed any fadeout and come to a stop, it will call its
 * TrackStoppedCallback, if any. It is legal to assign the track a new input
 * and/or restart it during this callback.
 *
 * It is legal to halt a track that's already stopped. It does nothing, and
 * returns true.
 *
 * @param track the track to halt.
 * @param fade_out_frames the number of sample frames to spend fading out to
 *                        silence before halting. 0 to stop immediately.
 * @returns true if the track has stopped, false on error; call GetError() for
 *          details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Play
 */
inline bool StopTrack(TrackRef track, Sint64 fade_out_frames)
{
  return MIX_StopTrack(track, fade_out_frames);
}

inline bool Track::Stop(Sint64 fade_out_frames)
{
  return SDL::StopTrack(m_resource, fade_out_frames);
}

/**
 * Halt all currently-playing tracks, possibly fading out over time.
 *
 * If `fade_out_ms` is > 0, the tracks do not stop mixing immediately, but
 * rather fades to silence over that many milliseconds before stopping. Note
 * that this is different than Track.Stop(), which wants sample frames; this
 * function takes milliseconds because different tracks might have different
 * sample rates.
 *
 * If a track ends normally while the fade-out is still in progress, the audio
 * stops there; the fade is not adjusted to be shorter if it will last longer
 * than the audio remaining.
 *
 * Once a track has completed any fadeout and come to a stop, it will call its
 * TrackStoppedCallback, if any. It is legal to assign the track a new input
 * and/or restart it during this callback. This function does not prevent new
 * play requests from being made.
 *
 * @param mixer the mixer on which to stop all tracks.
 * @param fade_out_ms the number of milliseconds to spend fading out to silence
 *                    before halting. 0 to stop immediately.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Stop
 */
inline void StopAllTracks(MixerRef mixer, Sint64 fade_out_ms)
{
  CheckError(MIX_StopAllTracks(mixer, fade_out_ms));
}

inline void Mixer::StopAllTracks(Sint64 fade_out_ms)
{
  SDL::StopAllTracks(m_resource, fade_out_ms);
}

/**
 * Halt all tracks with a specific tag, possibly fading out over time.
 *
 * If `fade_out_ms` is > 0, the tracks do not stop mixing immediately, but
 * rather fades to silence over that many milliseconds before stopping. Note
 * that this is different than Track.Stop(), which wants sample frames; this
 * function takes milliseconds because different tracks might have different
 * sample rates.
 *
 * If a track ends normally while the fade-out is still in progress, the audio
 * stops there; the fade is not adjusted to be shorter if it will last longer
 * than the audio remaining.
 *
 * Once a track has completed any fadeout and come to a stop, it will call its
 * TrackStoppedCallback, if any. It is legal to assign the track a new input
 * and/or restart it during this callback. This function does not prevent new
 * play requests from being made.
 *
 * @param mixer the mixer on which to stop tracks.
 * @param tag the tag to use when searching for tracks.
 * @param fade_out_ms the number of milliseconds to spend fading out to silence
 *                    before halting. 0 to stop immediately.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Stop
 * @sa Track.Tag
 */
inline void StopTag(MixerRef mixer, StringParam tag, Sint64 fade_out_ms)
{
  CheckError(MIX_StopTag(mixer, tag, fade_out_ms));
}

inline void Mixer::StopTag(StringParam tag, Sint64 fade_out_ms)
{
  SDL::StopTag(m_resource, std::move(tag), fade_out_ms);
}

/**
 * Pause a currently-playing track.
 *
 * A paused track is not considered "stopped," so its TrackStoppedCallback will
 * not fire if paused, but it won't change state by default, generate audio, or
 * generally make progress, until it is resumed.
 *
 * It is legal to pause a track that's in any state (playing, already paused, or
 * stopped). Unless the track is currently playing, pausing does nothing, and
 * returns true. A false return is only used to signal errors here (such as
 * MIX.Init not being called or `track` being nullptr).
 *
 * @param track the track to pause.
 * @returns true if the track has paused, false on error; call GetError() for
 *          details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Resume
 */
inline bool PauseTrack(TrackRef track) { return MIX_PauseTrack(track); }

inline bool Track::Pause() { return SDL::PauseTrack(m_resource); }

/**
 * Pause all currently-playing tracks.
 *
 * A paused track is not considered "stopped," so its TrackStoppedCallback will
 * not fire if paused, but it won't change state by default, generate audio, or
 * generally make progress, until it is resumed.
 *
 * This function makes all tracks on the specified mixer that are currently
 * playing move to a paused state. They can later be resumed.
 *
 * @param mixer the mixer on which to pause all tracks.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Resume
 * @sa Mixer.ResumeAllTracks
 */
inline void PauseAllTracks(MixerRef mixer)
{
  CheckError(MIX_PauseAllTracks(mixer));
}

inline void Mixer::PauseAllTracks() { SDL::PauseAllTracks(m_resource); }

/**
 * Pause all tracks with a specific tag.
 *
 * A paused track is not considered "stopped," so its TrackStoppedCallback will
 * not fire if paused, but it won't change state by default, generate audio, or
 * generally make progress, until it is resumed.
 *
 * This function makes all currently-playing tracks on the specified mixer, with
 * a specific tag, move to a paused state. They can later be resumed.
 *
 * Tracks that match the specified tag that aren't currently playing are
 * ignored.
 *
 * @param mixer the mixer on which to pause tracks.
 * @param tag the tag to use when searching for tracks.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Pause
 * @sa Track.Resume
 * @sa Mixer.ResumeTag
 * @sa Track.Tag
 */
inline void PauseTag(MixerRef mixer, StringParam tag)
{
  CheckError(MIX_PauseTag(mixer, tag));
}

inline void Mixer::PauseTag(StringParam tag)
{
  SDL::PauseTag(m_resource, std::move(tag));
}

/**
 * Resume a currently-paused track.
 *
 * A paused track is not considered "stopped," so its TrackStoppedCallback will
 * not fire if paused, but it won't change state by default, generate audio, or
 * generally make progress, until it is resumed.
 *
 * It is legal to resume a track that's in any state (playing, paused, or
 * stopped). Unless the track is currently paused, resuming does nothing, and
 * returns true. A false return is only used to signal errors here (such as
 * MIX.Init not being called or `track` being nullptr).
 *
 * @param track the track to resume.
 * @returns true if the track has resumed, false on error; call GetError() for
 *          details.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Pause
 */
inline bool ResumeTrack(TrackRef track) { return MIX_ResumeTrack(track); }

inline bool Track::Resume() { return SDL::ResumeTrack(m_resource); }

/**
 * Resume all currently-paused tracks.
 *
 * A paused track is not considered "stopped," so its TrackStoppedCallback will
 * not fire if paused, but it won't change state by default, generate audio, or
 * generally make progress, until it is resumed.
 *
 * This function makes all tracks on the specified mixer that are currently
 * paused move to a playing state.
 *
 * @param mixer the mixer on which to resume all tracks.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Pause
 * @sa Mixer.PauseAllTracks
 */
inline void ResumeAllTracks(MixerRef mixer)
{
  CheckError(MIX_ResumeAllTracks(mixer));
}

inline void Mixer::ResumeAllTracks() { SDL::ResumeAllTracks(m_resource); }

/**
 * Resume all tracks with a specific tag.
 *
 * A paused track is not considered "stopped," so its TrackStoppedCallback will
 * not fire if paused, but it won't change state by default, generate audio, or
 * generally make progress, until it is resumed.
 *
 * This function makes all currently-paused tracks on the specified mixer, with
 * a specific tag, move to a playing state.
 *
 * Tracks that match the specified tag that aren't currently paused are ignored.
 *
 * @param mixer the mixer on which to resume tracks.
 * @param tag the tag to use when searching for tracks.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Resume
 * @sa Track.Pause
 * @sa Mixer.PauseTag
 * @sa Track.Tag
 */
inline void ResumeTag(MixerRef mixer, StringParam tag)
{
  CheckError(MIX_ResumeTag(mixer, tag));
}

inline void Mixer::ResumeTag(StringParam tag)
{
  SDL::ResumeTag(m_resource, std::move(tag));
}

/**
 * Query if a track is currently playing.
 *
 * If this returns true, the track is currently contributing to the mixer's
 * output (it's "playing"). It is not stopped nor paused.
 *
 * On various errors (MIX.Init() was not called, the track is nullptr), this
 * returns false, but there is no mechanism to distinguish errors from
 * non-playing tracks.
 *
 * @param track the track to query.
 * @returns true if playing, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Play
 * @sa Track.Pause
 * @sa Track.Resume
 * @sa Track.Stop
 * @sa Track.Paused
 */
inline bool TrackPlaying(TrackRef track) { return MIX_TrackPlaying(track); }

inline bool Track::Playing() { return SDL::TrackPlaying(m_resource); }

/**
 * Query if a track is currently paused.
 *
 * If this returns true, the track is not currently contributing to the mixer's
 * output but will when resumed (it's "paused"). It is not playing nor stopped.
 *
 * On various errors (MIX.Init() was not called, the track is nullptr), this
 * returns false, but there is no mechanism to distinguish errors from
 * non-playing tracks.
 *
 * @param track the track to query.
 * @returns true if paused, false otherwise.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Play
 * @sa Track.Pause
 * @sa Track.Resume
 * @sa Track.Stop
 * @sa Track.Playing
 */
inline bool TrackPaused(TrackRef track) { return MIX_TrackPaused(track); }

inline bool Track::Paused() { return SDL::TrackPaused(m_resource); }

/**
 * Set a mixer's master gain control.
 *
 * Each mixer has a master gain, to adjust the volume of the entire mix. Each
 * sample passing through the pipeline is modulated by this gain value. A gain
 * of zero will generate silence, 1.0f will not change the mixed volume, and
 * larger than 1.0f will increase the volume. Negative values are illegal. There
 * is no maximum gain specified, but this can quickly get extremely loud, so
 * please be careful with this setting.
 *
 * A mixer's master gain defaults to 1.0f.
 *
 * This value can be changed at any time to adjust the future mix.
 *
 * @param mixer the mixer to adjust.
 * @param gain the new gain value.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Mixer.GetGain
 * @sa Track.SetGain
 */
inline void SetMixerGain(MixerRef mixer, float gain)
{
  CheckError(MIX_SetMixerGain(mixer, gain));
}

inline void Mixer::SetGain(float gain) { SDL::SetMixerGain(m_resource, gain); }

/**
 * Get a mixer's master gain control.
 *
 * This returns the last value set through Mixer.SetGain(), or 1.0f if no value
 * has ever been explicitly set.
 *
 * @param mixer the mixer to query.
 * @returns the mixer's current master gain.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Mixer.SetGain
 * @sa Track.GetGain
 */
inline float GetMixerGain(MixerRef mixer) { return MIX_GetMixerGain(mixer); }

inline float Mixer::GetGain() { return SDL::GetMixerGain(m_resource); }

/**
 * Set a track's gain control.
 *
 * Each track has its own gain, to adjust its overall volume. Each sample from
 * this track is modulated by this gain value. A gain of zero will generate
 * silence, 1.0f will not change the mixed volume, and larger than 1.0f will
 * increase the volume. Negative values are illegal. There is no maximum gain
 * specified, but this can quickly get extremely loud, so please be careful with
 * this setting.
 *
 * A track's gain defaults to 1.0f.
 *
 * This value can be changed at any time to adjust the future mix.
 *
 * @param track the track to adjust.
 * @param gain the new gain value.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.GetGain
 * @sa Mixer.SetGain
 */
inline void SetTrackGain(TrackRef track, float gain)
{
  CheckError(MIX_SetTrackGain(track, gain));
}

inline void Track::SetGain(float gain) { SDL::SetTrackGain(m_resource, gain); }

/**
 * Get a track's gain control.
 *
 * This returns the last value set through Track.SetGain(), or 1.0f if no value
 * has ever been explicitly set.
 *
 * @param track the track to query.
 * @returns the track's current gain.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.SetGain
 * @sa Mixer.GetGain
 */
inline float GetTrackGain(TrackRef track) { return MIX_GetTrackGain(track); }

inline float Track::GetGain() { return SDL::GetTrackGain(m_resource); }

/**
 * Set the gain control of all tracks with a specific tag.
 *
 * Each track has its own gain, to adjust its overall volume. Each sample from
 * this track is modulated by this gain value. A gain of zero will generate
 * silence, 1.0f will not change the mixed volume, and larger than 1.0f will
 * increase the volume. Negative values are illegal. There is no maximum gain
 * specified, but this can quickly get extremely loud, so please be careful with
 * this setting.
 *
 * A track's gain defaults to 1.0f.
 *
 * This will change the gain control on tracks on the specified mixer that have
 * the specified tag.
 *
 * From the point of view of the mixing process, all tracks that successfully
 * change gain values will do so at the exact same moment.
 *
 * This value can be changed at any time to adjust the future mix.
 *
 * @param mixer the mixer on which to look for tagged tracks.
 * @param tag the tag to use when searching for tracks.
 * @param gain the new gain value.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.GetGain
 * @sa Track.SetGain
 * @sa Mixer.SetGain
 * @sa Track.Tag
 */
inline void SetTagGain(MixerRef mixer, StringParam tag, float gain)
{
  CheckError(MIX_SetTagGain(mixer, tag, gain));
}

inline void Mixer::SetTagGain(StringParam tag, float gain)
{
  SDL::SetTagGain(m_resource, std::move(tag), gain);
}

/**
 * Set a mixer's master frequency ratio.
 *
 * Each mixer has a master frequency ratio, that affects the entire mix. This
 * can cause the final output to change speed and pitch. A value greater than
 * 1.0f will play the audio faster, and at a higher pitch. A value less than
 * 1.0f will play the audio slower, and at a lower pitch. 1.0f is normal speed.
 *
 * Each track _also_ has a frequency ratio; it will be applied when mixing that
 * track's audio regardless of the master setting. The master setting affects
 * the final output after all mixing has been completed.
 *
 * A mixer's master frequency ratio defaults to 1.0f.
 *
 * This value can be changed at any time to adjust the future mix.
 *
 * @param mixer the mixer to adjust.
 * @param ratio the frequency ratio. Must be between 0.01f and 100.0f.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Mixer.GetFrequencyRatio
 * @sa Track.SetFrequencyRatio
 */
inline void SetMixerFrequencyRatio(MixerRef mixer, float ratio)
{
  CheckError(MIX_SetMixerFrequencyRatio(mixer, ratio));
}

inline void Mixer::SetFrequencyRatio(float ratio)
{
  SDL::SetMixerFrequencyRatio(m_resource, ratio);
}

/**
 * Get a mixer's master frequency ratio.
 *
 * This returns the last value set through Mixer.SetFrequencyRatio(), or 1.0f if
 * no value has ever been explicitly set.
 *
 * @param mixer the mixer to query.
 * @returns the mixer's current master frequency ratio.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Mixer.SetFrequencyRatio
 * @sa Track.GetFrequencyRatio
 */
inline float GetMixerFrequencyRatio(MixerRef mixer)
{
  return MIX_GetMixerFrequencyRatio(mixer);
}

inline float Mixer::GetFrequencyRatio()
{
  return SDL::GetMixerFrequencyRatio(m_resource);
}

/**
 * Change the frequency ratio of a track.
 *
 * The frequency ratio is used to adjust the rate at which audio data is
 * consumed. Changing this effectively modifies the speed and pitch of the
 * track's audio. A value greater than 1.0f will play the audio faster, and at a
 * higher pitch. A value less than 1.0f will play the audio slower, and at a
 * lower pitch. 1.0f is normal speed.
 *
 * The default value is 1.0f.
 *
 * This value can be changed at any time to adjust the future mix.
 *
 * @param track the track on which to change the frequency ratio.
 * @param ratio the frequency ratio. Must be between 0.01f and 100.0f.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.GetFrequencyRatio
 */
inline void SetTrackFrequencyRatio(TrackRef track, float ratio)
{
  CheckError(MIX_SetTrackFrequencyRatio(track, ratio));
}

inline void Track::SetFrequencyRatio(float ratio)
{
  SDL::SetTrackFrequencyRatio(m_resource, ratio);
}

/**
 * Query the frequency ratio of a track.
 *
 * The frequency ratio is used to adjust the rate at which audio data is
 * consumed. Changing this effectively modifies the speed and pitch of the
 * track's audio. A value greater than 1.0f will play the audio faster, and at a
 * higher pitch. A value less than 1.0f will play the audio slower, and at a
 * lower pitch. 1.0f is normal speed.
 *
 * The default value is 1.0f.
 *
 * On various errors (MIX.Init() was not called, the track is nullptr), this
 * returns 0.0f. Since this is not a valid value to set, this can be seen as an
 * error state.
 *
 * @param track the track on which to query the frequency ratio.
 * @returns the current frequency ratio, or 0.0f on failure; call GetError() for
 *          more information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.GetFrequencyRatio
 */
inline float GetTrackFrequencyRatio(TrackRef track)
{
  return MIX_GetTrackFrequencyRatio(track);
}

inline float Track::GetFrequencyRatio()
{
  return SDL::GetTrackFrequencyRatio(m_resource);
}

/**
 * Set the current output channel map of a track.
 *
 * Channel maps are optional; most things do not need them, instead passing data
 * in the order that SDL expects.
 *
 * The output channel map reorders track data after transformations and before
 * it is mixed into a mixer group. This can be useful for reversing stereo
 * channels, for example.
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
 * Tracks default to no remapping applied. Passing a nullptr channel map is
 * legal, and turns off remapping.
 *
 * SDL_mixer will copy the channel map; the caller does not have to save this
 * array after this call.
 *
 * @param track the track to change.
 * @param chmap the new channel map, nullptr to reset to default.
 * @param count The number of channels in the map.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline void SetTrackOutputChannelMap(TrackRef track,
                                     const int* chmap,
                                     int count)
{
  CheckError(MIX_SetTrackOutputChannelMap(track, chmap, count));
}

inline void Track::SetOutputChannelMap(const int* chmap, int count)
{
  SDL::SetTrackOutputChannelMap(m_resource, chmap, count);
}

/**
 * Force a track to stereo output, with optionally left/right panning.
 *
 * This will cause the output of the track to convert to stereo, and then mix it
 * only onto the Front Left and Front Right speakers, regardless of the speaker
 * configuration. The left and right channels are modulated by `gains`, which
 * can be used to produce panning effects. This function may be called to adjust
 * the gains at any time.
 *
 * If `gains` is not nullptr, this track will be switched into forced-stereo
 * mode. If `gains` is nullptr, this will disable spatialization (both the
 * forced-stereo mode of this function and full 3D spatialization of
 * Track.Set3DPosition()).
 *
 * Negative gains are clamped to zero; there is no clamp for maximum, so one
 * could set the value > 1.0f to make a channel louder.
 *
 * The track's 3D position, reported by Track.Get3DPosition(), will be reset to
 * (0, 0, 0).
 *
 * @param track the track to adjust.
 * @param gains the per-channel gains, or nullptr to disable spatialization.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Set3DPosition
 */
inline void SetTrackStereo(TrackRef track, const StereoGains& gains)
{
  CheckError(MIX_SetTrackStereo(track, &gains));
}

inline void Track::SetStereo(const StereoGains& gains)
{
  SDL::SetTrackStereo(m_resource, gains);
}

/**
 * Set a track's position in 3D space.
 *
 * (Please note that SDL_mixer is not intended to be a extremely powerful 3D
 * API. It lacks 3D features that other APIs like OpenAL offer: there's no
 * doppler effect, distance models, rolloff, etc. This is meant to be Good
 * Enough for games that can use some positional sounds and can even take
 * advantage of surround-sound configurations.)
 *
 * If `position` is not nullptr, this track will be switched into 3D positional
 * mode. If `position` is nullptr, this will disable positional mixing (both the
 * full 3D spatialization of this function and forced-stereo mode of
 * Track.SetStereo()).
 *
 * In 3D positional mode, SDL_mixer will mix this track as if it were positioned
 * in 3D space, including distance attenuation (quieter as it gets further from
 * the listener) and spatialization (positioned on the correct speakers to
 * suggest direction, either with stereo outputs or full surround sound).
 *
 * For a mono speaker output, spatialization is effectively disabled but
 * distance attenuation will still work, which is all you can really do with a
 * single speaker.
 *
 * The coordinate system operates like OpenGL or OpenAL: a "right-handed"
 * coordinate system. See Point3D for the details.
 *
 * The listener is always at coordinate (0,0,0) and can't be changed.
 *
 * The track's input will be converted to mono (1 channel) so it can be rendered
 * across the correct speakers.
 *
 * @param track the track for which to set 3D position.
 * @param position the new 3D position for the track. May be nullptr.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Get3DPosition
 * @sa Track.SetStereo
 */
inline void SetTrack3DPosition(TrackRef track, const Point3D& position)
{
  CheckError(MIX_SetTrack3DPosition(track, &position));
}

inline void Track::Set3DPosition(const Point3D& position)
{
  SDL::SetTrack3DPosition(m_resource, position);
}

/**
 * Get a track's current position in 3D space.
 *
 * If 3D positioning isn't enabled for this track, through a call to
 * Track.Set3DPosition(), this will return (0,0,0).
 *
 * @param track the track to query.
 * @param position on successful return, will contain the track's position.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Track.Set3DPosition
 */
inline void GetTrack3DPosition(TrackRef track, Point3D* position)
{
  CheckError(MIX_GetTrack3DPosition(track, position));
}

inline void Track::Get3DPosition(Point3D* position)
{
  SDL::GetTrack3DPosition(m_resource, position);
}

/**
 * Create a mixing group.
 *
 * Tracks are assigned to a mixing group (or if unassigned, they live in a
 * mixer's internal default group). All tracks in a group are mixed together and
 * the app can access this mixed data before it is mixed with all other groups
 * to produce the final output.
 *
 * This can be a useful feature, but is completely optional; apps can ignore
 * mixing groups entirely and still have a full experience with SDL_mixer.
 *
 * After creating a group, assign tracks to it with Track.SetGroup(). Use
 * Group.SetPostMixCallback() to access the group's mixed data.
 *
 * A mixing group can be destroyed with Group.Destroy() when no longer needed.
 * Destroying the mixer will also destroy all its still-existing mixing groups.
 *
 * @param mixer the mixer on which to create a mixing group.
 * @returns a newly-created mixing group on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Group.Destroy
 * @sa Track.SetGroup
 * @sa Group.SetPostMixCallback
 */
inline Group CreateGroup(MixerRef mixer) { return Group(mixer); }

inline GroupRef Mixer::CreateGroup() { return Group(m_resource); }

inline Group::Group(MixerRef mixer)
  : m_resource(CheckError(MIX_CreateGroup(mixer)))
{
}

/**
 * Destroy a mixing group.
 *
 * Any tracks currently assigned to this group will be reassigned to the mixer's
 * internal default group.
 *
 * @param group the mixing group to destroy.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Group.Group
 */
inline void DestroyGroup(GroupRaw group) { MIX_DestroyGroup(group); }

inline void Group::Destroy() { DestroyGroup(release()); }

/**
 * Get the properties associated with a group.
 *
 * Currently SDL_mixer assigns no properties of its own to a group, but this can
 * be a convenient place to store app-specific data.
 *
 * A Properties is created the first time this function is called for a given
 * group.
 *
 * @param group the group to query.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline PropertiesRef GetGroupProperties(GroupRef group)
{
  return CheckError(MIX_GetGroupProperties(group));
}

inline PropertiesRef Group::GetProperties()
{
  return SDL::GetGroupProperties(m_resource);
}

/**
 * Get the Mixer that owns a Group.
 *
 * This is the mixer pointer that was passed to Group.Group().
 *
 * @param group the group to query.
 * @returns the mixer associated with the group on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline MixerRef GetGroupMixer(GroupRef group)
{
  return CheckError(MIX_GetGroupMixer(group));
}

inline MixerRef Group::GetMixer() { return SDL::GetGroupMixer(m_resource); }

/**
 * Assign a track to a mixing group.
 *
 * All tracks in a group are mixed together, and that output is made available
 * to the app before it is mixed into the final output.
 *
 * Tracks can only be in one group at a time, and the track and group must have
 * been created on the same Mixer.
 *
 * Setting a track to a nullptr group will remove it from any app-created
 * groups, and reassign it to the mixer's internal default group.
 *
 * @param track the track to set mixing group assignment.
 * @param group the new mixing group to assign to. May be nullptr.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Group.Group
 * @sa Group.SetPostMixCallback
 */
inline void SetTrackGroup(TrackRef track, GroupRef group)
{
  CheckError(MIX_SetTrackGroup(track, group));
}

inline void Track::SetGroup(GroupRef group)
{
  SDL::SetTrackGroup(m_resource, group);
}

/**
 * Set a callback that fires when a Track is stopped.
 *
 * When a track completes playback, either because it ran out of data to mix
 * (and all loops were completed as well), or it was explicitly stopped by the
 * app, it will fire the callback specified here.
 *
 * Each track has its own unique callback.
 *
 * Passing a nullptr callback here is legal; it disables this track's callback.
 *
 * Pausing a track will not fire the callback, nor will the callback fire on a
 * playing track that is being destroyed.
 *
 * It is legal to adjust the track, including changing its input and restarting
 * it. If this is done because it ran out of data in the middle of mixing, the
 * mixer will start mixing the new track state in its current run without any
 * gap in the audio.
 *
 * @param track the track to assign this callback to.
 * @param cb the function to call when the track stops. May be nullptr.
 * @param userdata an opaque pointer provided to the callback for its own
 *                 personal use.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa TrackStoppedCallback
 */
inline void SetTrackStoppedCallback(TrackRef track,
                                    TrackStoppedCallback cb,
                                    void* userdata)
{
  CheckError(MIX_SetTrackStoppedCallback(track, cb, userdata));
}

inline void Track::SetStoppedCallback(TrackStoppedCallback cb, void* userdata)
{
  SDL::SetTrackStoppedCallback(m_resource, cb, userdata);
}

/**
 * Set a callback that fires when a Track has initial decoded audio.
 *
 * As a track needs to mix more data, it pulls from its input (a Audio, an
 * AudioStream, etc). This input might be a compressed file format, like MP3, so
 * a little more data is uncompressed from it.
 *
 * Once the track has PCM data to start operating on, it can fire a callback
 * before _any_ changes to the raw PCM input have happened. This lets an app
 * view the data before it has gone through transformations such as gain, 3D
 * positioning, fading, etc. It can also change the data in any way it pleases
 * during this callback, and the mixer will continue as if this data came
 * directly from the input.
 *
 * Each track has its own unique raw callback.
 *
 * Passing a nullptr callback here is legal; it disables this track's callback.
 *
 * @param track the track to assign this callback to.
 * @param cb the function to call when the track mixes. May be nullptr.
 * @param userdata an opaque pointer provided to the callback for its own
 *                 personal use.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa TrackMixCallback
 * @sa Track.SetCookedCallback
 */
inline void SetTrackRawCallback(TrackRef track,
                                TrackMixCallback cb,
                                void* userdata)
{
  CheckError(MIX_SetTrackRawCallback(track, cb, userdata));
}

inline void Track::SetRawCallback(TrackMixCallback cb, void* userdata)
{
  SDL::SetTrackRawCallback(m_resource, cb, userdata);
}

/**
 * Set a callback that fires when the mixer has transformed a track's audio.
 *
 * As a track needs to mix more data, it pulls from its input (a Audio, an
 * AudioStream, etc). This input might be a compressed file format, like MP3, so
 * a little more data is uncompressed from it.
 *
 * Once the track has PCM data to start operating on, and its raw callback has
 * completed, it will begin to transform the audio: gain, fading, frequency
 * ratio, 3D positioning, etc.
 *
 * A callback can be fired after all these transformations, but before the
 * transformed data is mixed into other tracks. This lets an app view the data
 * at the last moment that it is still a part of this track. It can also change
 * the data in any way it pleases during this callback, and the mixer will
 * continue as if this data came directly from the input.
 *
 * Each track has its own unique cooked callback.
 *
 * Passing a nullptr callback here is legal; it disables this track's callback.
 *
 * @param track the track to assign this callback to.
 * @param cb the function to call when the track mixes. May be nullptr.
 * @param userdata an opaque pointer provided to the callback for its own
 *                 personal use.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa TrackMixCallback
 * @sa Track.SetRawCallback
 */
inline void SetTrackCookedCallback(TrackRef track,
                                   TrackMixCallback cb,
                                   void* userdata)
{
  CheckError(MIX_SetTrackCookedCallback(track, cb, userdata));
}

inline void Track::SetCookedCallback(TrackMixCallback cb, void* userdata)
{
  SDL::SetTrackCookedCallback(m_resource, cb, userdata);
}

/**
 * Set a callback that fires when a mixer group has completed mixing.
 *
 * After all playing tracks in a mixer group have pulled in more data from their
 * inputs, transformed it, and mixed together into a single buffer, a callback
 * can be fired. This lets an app view the data at the last moment that it is
 * still a part of this group. It can also change the data in any way it pleases
 * during this callback, and the mixer will continue as if this data came
 * directly from the group's mix buffer.
 *
 * Each group has its own unique callback. Tracks that aren't in an explicit
 * Group are mixed in an internal grouping that is not available to the app.
 *
 * Passing a nullptr callback here is legal; it disables this group's callback.
 *
 * @param group the mixing group to assign this callback to.
 * @param cb the function to call when the group mixes. May be nullptr.
 * @param userdata an opaque pointer provided to the callback for its own
 *                 personal use.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa GroupMixCallback
 */
inline void SetGroupPostMixCallback(GroupRef group,
                                    GroupMixCallback cb,
                                    void* userdata)
{
  CheckError(MIX_SetGroupPostMixCallback(group, cb, userdata));
}

inline void Group::SetPostMixCallback(GroupMixCallback cb, void* userdata)
{
  SDL::SetGroupPostMixCallback(m_resource, cb, userdata);
}

/**
 * Set a callback that fires when all mixing has completed.
 *
 * After all mixer groups have processed, their buffers are mixed together into
 * a single buffer for the final output, at which point a callback can be fired.
 * This lets an app view the data at the last moment before mixing completes. It
 * can also change the data in any way it pleases during this callback, and the
 * mixer will continue as if this data is the final output.
 *
 * Each mixer has its own unique callback.
 *
 * Passing a nullptr callback here is legal; it disables this mixer's callback.
 *
 * @param mixer the mixer to assign this callback to.
 * @param cb the function to call when the mixer mixes. May be nullptr.
 * @param userdata an opaque pointer provided to the callback for its own
 *                 personal use.
 * @throws Error on failure.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa PostMixCallback
 */
inline void SetPostMixCallback(MixerRef mixer,
                               PostMixCallback cb,
                               void* userdata)
{
  CheckError(MIX_SetPostMixCallback(mixer, cb, userdata));
}

inline void Mixer::SetPostMixCallback(PostMixCallback cb, void* userdata)
{
  SDL::SetPostMixCallback(m_resource, cb, userdata);
}

/**
 * Generate mixer output when not driving an audio device.
 *
 * SDL_mixer allows the creation of Mixer objects that are not connected to an
 * audio device, by calling Mixer.Mixer() instead of Mixer.Mixer(). Such mixers
 * will not generate output until explicitly requested through this function.
 *
 * The caller may request as much audio as desired, so long as `buflen` is a
 * multiple of the sample frame size specified when creating the mixer (for
 * example, if requesting stereo Sint16 audio, buflen must be a multiple of 4: 2
 * bytes-per-channel times 2 channels).
 *
 * The mixer will mix as quickly as possible; since it works in sample frames
 * instead of time, it can potentially generate enormous amounts of audio in a
 * small amount of time.
 *
 * On success, this always fills `buffer` with `buflen` bytes of audio; if all
 * playing tracks finish mixing, it will fill the remaining buffer with silence.
 *
 * Each call to this function will pick up where it left off, playing tracks
 * will continue to mix from the point the previous call completed, etc. The
 * mixer state can be changed between each call in any way desired: tracks can
 * be added, played, stopped, changed, removed, etc. Effectively this function
 * does the same thing SDL_mixer does internally when the audio device needs
 * more audio to play.
 *
 * This function can not be used with mixers from Mixer.Mixer(); those generate
 * audio as needed internally.
 *
 * This function returns the number of _bytes_ of real audio mixed, which might
 * be less than `buflen`. While all `buflen` bytes of `buffer` will be
 * initialized, if available tracks to mix run out, the end of the buffer will
 * be initialized with silence; this silence will not be counted in the return
 * value, so the caller has the option to identify how much of the buffer has
 * legitimate contents vs appended silence. As such, any value >= 0 signifies
 * success. A return value of -1 means failure (out of memory, invalid
 * parameters, etc).
 *
 * @param mixer the mixer for which to generate more audio.
 * @param buffer a pointer to a buffer to store audio in.
 * @param buflen the number of bytes to store in buffer.
 * @returns The number of bytes of mixed audio, discounting appended silence, on
 *          success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Mixer.Mixer
 */
inline int Generate(MixerRef mixer, void* buffer, int buflen)
{
  return CheckError(MIX_Generate(mixer, buffer, buflen));
}

inline int Mixer::Generate(void* buffer, int buflen)
{
  return SDL::Generate(m_resource, buffer, buflen);
}

/**
 * An opaque object that represents an audio decoder.
 *
 * Most apps won't need this, as SDL_mixer's usual interfaces will decode audio
 * as needed. However, if one wants to decode an audio file into a memory buffer
 * without playing it, this interface offers that.
 *
 * These objects are created with AudioDecoder.AudioDecoder() or
 * AudioDecoder.AudioDecoder(), and then can use AudioDecoder.DecodeAudio() to
 * retrieve the raw PCM data.
 *
 * @since This struct is available since SDL_mixer 3.0.0.
 *
 * @cat resource
 */
class AudioDecoder
{
  AudioDecoderRaw m_resource = nullptr;

public:
  /// Default ctor
  constexpr AudioDecoder(std::nullptr_t = nullptr) noexcept
    : m_resource(0)
  {
  }

  /**
   * Constructs from AudioDecoderRef.
   *
   * @param resource a AudioDecoderRaw to be wrapped.
   *
   * This assumes the ownership, call release() if you need to take back.
   */
  constexpr explicit AudioDecoder(const AudioDecoderRaw resource) noexcept
    : m_resource(resource)
  {
  }

protected:
  /// Copy constructor
  constexpr AudioDecoder(const AudioDecoder& other) noexcept = default;

public:
  /// Move constructor
  constexpr AudioDecoder(AudioDecoder&& other) noexcept
    : AudioDecoder(other.release())
  {
  }

  constexpr AudioDecoder(const AudioDecoderRef& other) = delete;

  constexpr AudioDecoder(AudioDecoderRef&& other) = delete;

  /**
   * Create a AudioDecoder from a path on the filesystem.
   *
   * Most apps won't need this, as SDL_mixer's usual interfaces will decode
   * audio as needed. However, if one wants to decode an audio file into a
   * memory buffer without playing it, this interface offers that.
   *
   * This function allows properties to be specified. This is intended to supply
   * file-specific settings, such as where to find SoundFonts for a MIDI file,
   * etc. In most cases, the caller should pass a zero to specify no extra
   * properties.
   *
   * Properties are discussed in [SDL's
   * documentation](https://wiki.libsdl.org/SDL3/CategoryProperties) .
   *
   * When done with the audio decoder, it can be destroyed with
   * AudioDecoder.Destroy().
   *
   * This function requires SDL_mixer to have been initialized with a successful
   * call to MIX.Init(), but does not need an actual Mixer to have been created.
   *
   * @param path the path on the filesystem from which to load data.
   * @param props decoder-specific properties. May be zero.
   * @post an audio decoder, ready to decode.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa AudioDecoder.AudioDecoder
   * @sa AudioDecoder.DecodeAudio
   * @sa AudioDecoder.Destroy
   */
  AudioDecoder(StringParam path, PropertiesRef props);

  /**
   * Create a AudioDecoder from an IOStream.
   *
   * Most apps won't need this, as SDL_mixer's usual interfaces will decode
   * audio as needed. However, if one wants to decode an audio file into a
   * memory buffer without playing it, this interface offers that.
   *
   * This function allows properties to be specified. This is intended to supply
   * file-specific settings, such as where to find SoundFonts for a MIDI file,
   * etc. In most cases, the caller should pass a zero to specify no extra
   * properties.
   *
   * If `closeio` is true, then `io` will be closed when this decoder is done
   * with it. If this function fails and `closeio` is true, then `io` will be
   * closed before this function returns.
   *
   * When done with the audio decoder, it can be destroyed with
   * AudioDecoder.Destroy().
   *
   * This function requires SDL_mixer to have been initialized with a successful
   * call to MIX.Init(), but does not need an actual Mixer to have been created.
   *
   * @param io the i/o stream from which to load data.
   * @param closeio if true, close the i/o stream when done with it.
   * @param props decoder-specific properties. May be zero.
   * @post an audio decoder, ready to decode.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa AudioDecoder.AudioDecoder
   * @sa AudioDecoder.DecodeAudio
   * @sa AudioDecoder.Destroy
   */
  AudioDecoder(IOStreamRef io, bool closeio, PropertiesRef props);

  /// Destructor
  ~AudioDecoder() { MIX_DestroyAudioDecoder(m_resource); }

  /// Assignment operator.
  constexpr AudioDecoder& operator=(AudioDecoder&& other) noexcept
  {
    std::swap(m_resource, other.m_resource);
    return *this;
  }

protected:
  /// Assignment operator.
  constexpr AudioDecoder& operator=(const AudioDecoder& other) noexcept =
    default;

public:
  /// Retrieves underlying AudioDecoderRaw.
  constexpr AudioDecoderRaw get() const noexcept { return m_resource; }

  /// Retrieves underlying AudioDecoderRaw and clear this.
  constexpr AudioDecoderRaw release() noexcept
  {
    auto r = m_resource;
    m_resource = nullptr;
    return r;
  }

  /// Comparison
  constexpr auto operator<=>(const AudioDecoder& other) const noexcept =
    default;

  /// Converts to bool
  constexpr explicit operator bool() const noexcept { return !!m_resource; }

  /**
   * Destroy the specified audio decoder.
   *
   * Destroying a nullptr AudioDecoder is a legal no-op.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  void Destroy();

  /**
   * Get the properties associated with a AudioDecoder.
   *
   * SDL_mixer offers some properties of its own, but this can also be a
   * convenient place to store app-specific data.
   *
   * A Properties is created the first time this function is called for a given
   * AudioDecoder, if necessary.
   *
   * The file-specific metadata exposed through this function is identical to
   * those available through Audio.GetProperties(). Please refer to that
   * function's documentation for details.
   *
   * @returns a valid property ID on success.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   *
   * @sa Audio.GetProperties
   */
  PropertiesRef GetProperties();

  /**
   * Query the initial audio format of a AudioDecoder.
   *
   * Note that some audio files can change format in the middle; some explicitly
   * support this, but a more common example is two MP3 files concatenated
   * together. In many cases, SDL_mixer will correctly handle these sort of
   * files, but this function will only report the initial format a file uses.
   *
   * @param spec on success, audio format details will be stored here.
   * @throws Error on failure.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  void GetFormat(AudioSpec* spec);

  /**
   * Decode more audio from a AudioDecoder.
   *
   * Data is decoded on demand in whatever format is requested. The format is
   * permitted to change between calls.
   *
   * This function will return the number of bytes decoded, which may be less
   * than requested if there was an error or end-of-file. A return value of zero
   * means the entire file was decoded, -1 means an unrecoverable error
   * happened.
   *
   * @param buffer the memory buffer to store decoded audio.
   * @param buflen the maximum number of bytes to store to `buffer`.
   * @param spec the format that audio data will be stored to `buffer`.
   * @returns number of bytes decoded, or -1 on error; call GetError() for more
   *          information.
   *
   * @threadsafety It is safe to call this function from any thread.
   *
   * @since This function is available since SDL_mixer 3.0.0.
   */
  int DecodeAudio(void* buffer, int buflen, const AudioSpec& spec);
};

/**
 * Reference for AudioDecoder.
 *
 * This does not take ownership!
 */
struct AudioDecoderRef : AudioDecoder
{
  using AudioDecoder::AudioDecoder;

  /**
   * Constructs from raw AudioDecoder.
   *
   * @param resource a AudioDecoderRaw.
   *
   * This does not takes ownership!
   */
  AudioDecoderRef(AudioDecoderRaw resource) noexcept
    : AudioDecoder(resource)
  {
  }

  /**
   * Constructs from AudioDecoder.
   *
   * @param resource a AudioDecoder.
   *
   * This does not takes ownership!
   */
  constexpr AudioDecoderRef(const AudioDecoder& resource) noexcept
    : AudioDecoder(resource.get())
  {
  }

  /// Copy constructor.
  constexpr AudioDecoderRef(const AudioDecoderRef& other) noexcept
    : AudioDecoder(other.get())
  {
  }

  /// Move constructor.
  constexpr AudioDecoderRef(AudioDecoderRef&& other) noexcept
    : AudioDecoder(other.release())
  {
  }

  /// Destructor
  ~AudioDecoderRef() { release(); }

  /// Assignment operator.
  constexpr AudioDecoderRef& operator=(AudioDecoderRef other) noexcept
  {
    std::swap(*this, other);
    return *this;
  }

  /// Converts to AudioDecoderRaw
  constexpr operator AudioDecoderRaw() const noexcept { return get(); }
};

/**
 * Create a AudioDecoder from a path on the filesystem.
 *
 * Most apps won't need this, as SDL_mixer's usual interfaces will decode audio
 * as needed. However, if one wants to decode an audio file into a memory buffer
 * without playing it, this interface offers that.
 *
 * This function allows properties to be specified. This is intended to supply
 * file-specific settings, such as where to find SoundFonts for a MIDI file,
 * etc. In most cases, the caller should pass a zero to specify no extra
 * properties.
 *
 * Properties are discussed in [SDL's
 * documentation](https://wiki.libsdl.org/SDL3/CategoryProperties) .
 *
 * When done with the audio decoder, it can be destroyed with
 * AudioDecoder.Destroy().
 *
 * This function requires SDL_mixer to have been initialized with a successful
 * call to MIX.Init(), but does not need an actual Mixer to have been created.
 *
 * @param path the path on the filesystem from which to load data.
 * @param props decoder-specific properties. May be zero.
 * @returns an audio decoder, ready to decode.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa AudioDecoder.AudioDecoder
 * @sa AudioDecoder.DecodeAudio
 * @sa AudioDecoder.Destroy
 */
inline AudioDecoder CreateAudioDecoder(StringParam path, PropertiesRef props)
{
  return AudioDecoder(std::move(path), props);
}

inline AudioDecoder::AudioDecoder(StringParam path, PropertiesRef props)
  : m_resource(MIX_CreateAudioDecoder(path, props))
{
}

inline AudioDecoder::AudioDecoder(IOStreamRef io,
                                  bool closeio,
                                  PropertiesRef props)
  : m_resource(MIX_CreateAudioDecoder_IO(io, closeio, props))
{
}

/**
 * Create a AudioDecoder from an IOStream.
 *
 * Most apps won't need this, as SDL_mixer's usual interfaces will decode audio
 * as needed. However, if one wants to decode an audio file into a memory buffer
 * without playing it, this interface offers that.
 *
 * This function allows properties to be specified. This is intended to supply
 * file-specific settings, such as where to find SoundFonts for a MIDI file,
 * etc. In most cases, the caller should pass a zero to specify no extra
 * properties.
 *
 * If `closeio` is true, then `io` will be closed when this decoder is done with
 * it. If this function fails and `closeio` is true, then `io` will be closed
 * before this function returns.
 *
 * When done with the audio decoder, it can be destroyed with
 * AudioDecoder.Destroy().
 *
 * This function requires SDL_mixer to have been initialized with a successful
 * call to MIX.Init(), but does not need an actual Mixer to have been created.
 *
 * @param io the i/o stream from which to load data.
 * @param closeio if true, close the i/o stream when done with it.
 * @param props decoder-specific properties. May be zero.
 * @returns an audio decoder, ready to decode.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa AudioDecoder.AudioDecoder
 * @sa AudioDecoder.DecodeAudio
 * @sa AudioDecoder.Destroy
 */
inline AudioDecoder CreateAudioDecoder_IO(IOStreamRef io,
                                          bool closeio,
                                          PropertiesRef props)
{
  return AudioDecoder(io, closeio, props);
}

/**
 * Destroy the specified audio decoder.
 *
 * Destroying a nullptr AudioDecoder is a legal no-op.
 *
 * @param audiodecoder the audio to destroy.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline void DestroyAudioDecoder(AudioDecoderRaw audiodecoder)
{
  MIX_DestroyAudioDecoder(audiodecoder);
}

inline void AudioDecoder::Destroy() { DestroyAudioDecoder(release()); }

/**
 * Get the properties associated with a AudioDecoder.
 *
 * SDL_mixer offers some properties of its own, but this can also be a
 * convenient place to store app-specific data.
 *
 * A Properties is created the first time this function is called for a given
 * AudioDecoder, if necessary.
 *
 * The file-specific metadata exposed through this function is identical to
 * those available through Audio.GetProperties(). Please refer to that
 * function's documentation for details.
 *
 * @param audiodecoder the audio decoder to query.
 * @returns a valid property ID on success.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 *
 * @sa Audio.GetProperties
 */
inline PropertiesRef GetAudioDecoderProperties(AudioDecoderRef audiodecoder)
{
  return CheckError(MIX_GetAudioDecoderProperties(audiodecoder));
}

inline PropertiesRef AudioDecoder::GetProperties()
{
  return SDL::GetAudioDecoderProperties(m_resource);
}

/**
 * Query the initial audio format of a AudioDecoder.
 *
 * Note that some audio files can change format in the middle; some explicitly
 * support this, but a more common example is two MP3 files concatenated
 * together. In many cases, SDL_mixer will correctly handle these sort of files,
 * but this function will only report the initial format a file uses.
 *
 * @param audiodecoder the audio decoder to query.
 * @param spec on success, audio format details will be stored here.
 * @throws Error on failure.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline void GetAudioDecoderFormat(AudioDecoderRef audiodecoder, AudioSpec* spec)
{
  CheckError(MIX_GetAudioDecoderFormat(audiodecoder, spec));
}

inline void AudioDecoder::GetFormat(AudioSpec* spec)
{
  SDL::GetAudioDecoderFormat(m_resource, spec);
}

/**
 * Decode more audio from a AudioDecoder.
 *
 * Data is decoded on demand in whatever format is requested. The format is
 * permitted to change between calls.
 *
 * This function will return the number of bytes decoded, which may be less than
 * requested if there was an error or end-of-file. A return value of zero means
 * the entire file was decoded, -1 means an unrecoverable error happened.
 *
 * @param audiodecoder the decoder from which to retrieve more data.
 * @param buffer the memory buffer to store decoded audio.
 * @param buflen the maximum number of bytes to store to `buffer`.
 * @param spec the format that audio data will be stored to `buffer`.
 * @returns number of bytes decoded, or -1 on error; call GetError() for more
 *          information.
 *
 * @threadsafety It is safe to call this function from any thread.
 *
 * @since This function is available since SDL_mixer 3.0.0.
 */
inline int DecodeAudio(AudioDecoderRef audiodecoder,
                       void* buffer,
                       int buflen,
                       const AudioSpec& spec)
{
  return MIX_DecodeAudio(audiodecoder, buffer, buflen, &spec);
}

inline int AudioDecoder::DecodeAudio(void* buffer,
                                     int buflen,
                                     const AudioSpec& spec)
{
  return SDL::DecodeAudio(m_resource, buffer, buflen, spec);
}

/// @}

} // namespace SDL

#endif /* defined(SDL3PP_ENABLE_MIXER) || defined(SDL3PP_DOC) */

#endif /* SDL3PP_MIXER_H_ */
