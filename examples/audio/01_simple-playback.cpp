/*
 * This example code creates a simple audio stream for playing sound, and
 * generates a sine wave sound effect for it to play as time goes on. This
 * is the simplest way to get up and running with procedural sound.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally taken from SDL's example simple-playback.c
 */
#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  SDL::SDL init{SDL::INIT_VIDEO, SDL::INIT_AUDIO};
  SDL::Window window{"examples/audio/simple-playback", windowSz};
  SDL::Renderer renderer{window};
  SDL::AudioStream stream = SDL::AudioStream::OpenAudioDeviceStream(
    SDL::AUDIO_DEVICE_DEFAULT_PLAYBACK,
    SDL::AudioSpec{.format = SDL::AUDIO_F32, .channels = 1, .freq = 8000});
  int current_sine_sample = 0;

  Main() { stream->ResumeDevice(); }

  SDL::AppResult Iterate()
  {
    /* see if we need to feed the audio stream more data yet.
      We're being lazy here, but if there's less than half a second queued,
      generate more. A sine wave is unchanging audio--easy to stream--but for
      video games, you'll want to generate significantly _less_ audio ahead of
      time! */
    // 8000 float samples per second. Half of that.
    constexpr int minimum_audio = (8000 * sizeof(float)) / 2;

    if (stream->GetQueued() < minimum_audio) {
      // this will feed 512 samples each frame until we get to our maximum.
      static float samples[512];
      /* generate a 440Hz pure tone */
      for (int i = 0; i < SDL_arraysize(samples); i++) {
        const int freq = 440;
        const float phase = current_sine_sample * freq / 8000.0f;
        samples[i] = SDL::sin(phase * 2 * SDL_PI_F);
        current_sine_sample++;
      }

      // wrapping around to avoid floating-point errors
      current_sine_sample %= 8000;

      // feed the new data to the stream. It will queue at the end, and trickle
      // out as the hardware needs more data.
      stream->PutData(samples);
    }

    // we're not doing anything with the renderer, so just blank it out.
    renderer.RenderClear();
    renderer.Present();

    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main,
                        "Example Simple Audio Simple Playback",
                        "1.0",
                        "com.example.audio-simple-playback")
