/*
 * This example code creates a simple audio stream for playing sound, and
 * loads a .wav file that is pushed through the stream in a loop.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * The .wav file is a sample from Will Provost's song, The Living Proof,
 * used with permission.
 *
 *    From the album The Living Proof
 *    Publisher: 5 Guys Named Will
 *    Copyright 1996 Will Provost
 *    https://itunes.apple.com/us/album/the-living-proof/id4153978
 *    http://www.amazon.com/The-Living-Proof-Will-Provost/dp/B00004R8RH
 *
 * Originally taken from SDL's example load-wav.c
 */
#include <SDL3pp/SDL3pp.h>

#define SDL3PP_MAIN_USE_CALLBACKS
#include <SDL3pp/SDL3pp_main.h>

struct Main
{
  static constexpr SDL::Point windowSz = {640, 480};

  static SDL::AppResult Init(Main** m, SDL::AppArgs args)
  {
    SDL::SetAppMetadata(
      "Example Audio Load Wave", "1.0", "com.example.audio-load-wav");
    SDL::Init(SDL::INIT_VIDEO | SDL::INIT_AUDIO);
    *m = new Main();
    return SDL::APP_CONTINUE;
  }
  SDL::Window window = SDL::Window("examples/audio/load-wav", windowSz);
  SDL::Renderer renderer = SDL::Renderer(window);
  SDL::AudioStream stream;
  SDL::OwnArray<Uint8> wav_data;

  Main()
  {
    SDL::AudioSpec spec;
    wav_data = SDL::CheckError(SDL::LoadWAV(
      std::format("{}../assets/sample.wav", SDL::GetBasePath()), &spec));

    stream = SDL::AudioStream(SDL::AUDIO_DEVICE_DEFAULT_PLAYBACK, spec);
    stream.ResumeDevice();
  }

  SDL::AppResult Iterate()
  {
    /* see if we need to feed the audio stream more data yet.
        We're being lazy here, but if there's less than the entire wav file left
       to play, just shove a whole copy of it into the queue, so we always have
       _tons_ of data queued for playback. */
    if (stream.GetQueued() < wav_data.size()) {
      // feed the new data to the stream. It will queue at the end, and trickle
      // out as the hardware needs more data.
      stream.PutData(wav_data);
    }

    // we're not doing anything with the renderer, so just blank it out.
    renderer.RenderClear();
    renderer.Present();

    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CALLBACKS(Main)
