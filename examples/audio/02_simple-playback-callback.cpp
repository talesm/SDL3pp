/*
 * This example code creates a simple audio stream for playing sound, and
 * generates a sine wave sound effect for it to play as time goes on. Unlike
 * the previous example, this uses a callback to generate sound.
 *
 * This might be the path of least resistance if you're moving an SDL2
 * program's audio code to SDL3.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Originally taken from SDL's example simple-playback-callback.c
 */
#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  static SDL::AppResult Init(Main** m, SDL::AppArgs args)
  {
    SDL::SetAppMetadata("Example Simple Audio Playback Callback",
                        "1.0",
                        "com.example.audio-simple-playback-callback");
    SDL::Init(SDL::INIT_VIDEO | SDL::INIT_AUDIO);
    *m = new Main();
    return SDL::APP_CONTINUE;
  }
  SDL::Window window = {"examples/audio/simple-playback-callback", windowSz};
  SDL::Renderer renderer = SDL::Renderer(window);
  SDL::AudioStream stream{
    SDL::AUDIO_DEVICE_DEFAULT_PLAYBACK,
    SDL::AudioSpec{.format = SDL::AUDIO_F32, .channels = 1, .freq = 8000},
    [this](SDL::AudioStreamRef stream,
           int additional_amount,
           int total_amount) {
      FeedTheAudioStreamMore(stream, additional_amount, total_amount);
    }};
  int current_sine_sample = 0;

  Main() { stream.ResumeDevice(); }

  void FeedTheAudioStreamMore(SDL::AudioStreamRef stream,
                              int additional_amount,
                              int total_amount)
  {
    /* total_amount is how much data the audio stream is eating right now,
       additional_amount is how much more it needs than what it currently has
       queued (which might be zero!). You can supply any amount of data here; it
       will take what it needs and use the extra later. If you don't give it
       enough, it will take everything and then feed silence to the hardware for
       the rest. Ideally, though, we always give it what it needs and no extra,
       so we aren't buffering more than necessary. */
    additional_amount /= sizeof(float); // convert from bytes to samples
    while (additional_amount > 0) {
      float samples[128]; /* this will feed 128 samples each iteration until we
                             have enough. */
      const int total = SDL::min(additional_amount, SDL_arraysize(samples));

      /* generate a 440Hz pure tone */
      for (int i = 0; i < total; i++) {
        const int freq = 440;
        const float phase = current_sine_sample * freq / 8000.0f;
        samples[i] = SDL_sinf(phase * 2 * SDL_PI_F);
        current_sine_sample++;
      }

      /* wrapping around to avoid floating-point errors */
      current_sine_sample %= 8000;

      /* feed the new data to the stream. It will queue at the end, and trickle
       * out as the hardware needs more data. */
      stream.PutData(std::span{samples}.subspan(0, total));
      additional_amount -= total; /* subtract what we've just fed the stream. */
    }
  }

  SDL::AppResult Iterate()
  {
    // we're not doing anything with the renderer, so just blank it out.
    renderer.RenderClear();
    renderer.Present();

    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main)
