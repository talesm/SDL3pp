/*
 * This example code loads two .wav files, puts them an audio streams and
 * binds them for playback, repeating both sounds on loop. This shows several
 * streams mixing into a single playback device.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally taken from SDL's example multiple-streams.c
 */
#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Sound
{
  SDL::OwnArray<Uint8> wav_data;
  SDL::AudioStream stream;

  Sound(SDL::AudioDevice& audio_device, std::string_view fname)
  {
    SDL::AudioSpec spec;
    wav_data = SDL::CheckError(SDL::LoadWAV(
      std::format("{}../assets/{}", SDL::GetBasePath(), fname), &spec));
    /* Create an audio stream. Set the source format to the wav's format (what
       we'll input), leave the dest format std::nullopt here (it'll change to
       what the device wants once we bind it). */
    stream = SDL::AudioStream(spec, std::nullopt);
    audio_device.BindAudioStream(stream);
  }
};

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  static SDL::AppResult Init(Main** m, SDL::AppArgs args)
  {
    SDL::SetAppMetadata("Example Audio Multiple Streams",
                        "1.0",
                        "com.example.audio-multiple-streams");
    SDL::Init(SDL::INIT_VIDEO | SDL::INIT_AUDIO);
    *m = new Main();
    return SDL::APP_CONTINUE;
  }
  SDL::Window window{"examples/audio/multiple-streams", windowSz};
  SDL::Renderer renderer{window};
  SDL::AudioDevice audio_device{SDL::AUDIO_DEVICE_DEFAULT_PLAYBACK, {}};
  Sound sounds[2] = {{audio_device, "sample.wav"}, {audio_device, "sword.wav"}};

  SDL::AppResult Iterate()
  {
    for (auto& sound : sounds) {
      if (sound.stream.GetQueued() < int(sound.wav_data.size())) {
        sound.stream.PutData(sound.wav_data);
      }
    }

    // just blank the screen.
    renderer.RenderClear();
    renderer.Present();

    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main)
