/**
 * @file 04_bytepusher.cpp
 *
 * Based on SDL3's bytepusher.c
 *
 * An implementation of the BytePusher VM.
 *
 * For example programs and more information about BytePusher, see
 * https://esolangs.org/wiki/BytePusher
 *
 * This code is public domain. Feel free to use it for any purpose!
 */
#define SDL3PP_MAIN_USE_CLASS_CALLBACKS
#include <cstdarg>
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::literals;

constexpr int SCREEN_W = 256;
constexpr int SCREEN_H = 256;
constexpr int RAM_SIZE = 0x1000000;
constexpr int FRAMES_PER_SECOND = 60;
constexpr int SAMPLES_PER_FRAME = 256;
constexpr Uint64 NS_PER_SECOND = SDL_NS_PER_SECOND;
constexpr int MAX_AUDIO_LATENCY_FRAMES = 5;

constexpr int IO_KEYBOARD = 0;
constexpr int IO_PC = 2;
constexpr int IO_SCREEN_PAGE = 5;
constexpr int IO_AUDIO_BANK = 6;

struct BytePusher : SDL::AppInterface
{
  Uint8 ram[RAM_SIZE + 8];
  Uint64 last_tick;
  Uint64 tick_acc = NS_PER_SECOND;
  SDL::Window window;
  SDL::Renderer renderer;
  SDL::Palette palette;
  SDL::Texture texture;
  SDL::AudioStream audiostream;
  char status[SCREEN_W / 8];
  int status_ticks = 0;
  Uint16 keystate = 0;
  bool display_help = true;
  bool positional_input = false;

  BytePusher(SDL::Window w, SDL::Renderer r)
    : window(std::move(w))
    , renderer(std::move(r))
  {
    palette = SDL::CreatePalette(256);
    int i = 0;
    for (int r = 0; r < 6; ++r) {
      for (int g = 0; g < 6; ++g) {
        for (int b = 0; b < 6; ++b, ++i) {
          SDL::Color color(Uint8(r * 0x33), Uint8(g * 0x33), Uint8(b * 0x33));
          palette->colors[i] = color;
        }
      }
    }
    for (; i < 256; ++i) palette->colors[i] = SDL::Color(0, 0, 0);
    texture = SDL::CreateTexture(renderer,
                                 SDL::PIXELFORMAT_INDEX8,
                                 SDL::TEXTUREACCESS_STREAMING,
                                 {SCREEN_W, SCREEN_H});
    texture.SetPalette(palette);
    texture.SetScaleMode(SDL::SCALEMODE_NEAREST);

    constexpr SDL::AudioSpec audiospec = {
      SDL::AUDIO_S8, 1, SAMPLES_PER_FRAME * FRAMES_PER_SECOND};

    audiostream =
      SDL::OpenAudioDeviceStream(SDL::AUDIO_DEVICE_DEFAULT_PLAYBACK, audiospec);
    audiostream.SetGain(0.1f); // examples are loud!
    audiostream.ResumeDevice();

    set_status("renderer: {}", renderer.GetName());

    last_tick = SDL_GetTicksNS();
  }

  Uint16 read_u16(Uint32 addr) const
  {
    const Uint8* ptr = &ram[addr];
    return ((Uint16)ptr[0] << 8) | ((Uint16)ptr[1]);
  }

  Uint32 read_u24(Uint32 addr) const
  {
    const Uint8* ptr = &ram[addr];
    return ((Uint32)ptr[0] << 16) | ((Uint32)ptr[1] << 8) | ((Uint32)ptr[2]);
  }

  void set_status(std::string_view fmt, auto... args)
  {
    auto buf = std::vformat(fmt, std::make_format_args(args...));
    SDL::strlcpy(status, buf, sizeof(status));
    status_ticks = FRAMES_PER_SECOND * 3;
  }

  bool load(SDL::IOStreamRef stream)
  {
    display_help = true; // will set to false if load succeeds.

    SDL::memset(ram, 0, RAM_SIZE);

    if (!stream) return false;

    bool ok = true;
    for (size_t bytes_read = 0; bytes_read < RAM_SIZE;) {
      size_t read = stream.Read({&ram[bytes_read], RAM_SIZE - bytes_read});
      bytes_read += read;
      if (read == 0) {
        ok = stream.GetStatus() == SDL::IO_STATUS_EOF;
        break;
      }
    }
    audiostream.Clear();

    display_help = !ok;
    return ok;
  }

  static std::string_view filename(std::string_view path)
  {
    if (auto pos = path.find_last_of("/\\"); pos != std::string_view::npos) {
      return path.substr(pos);
    }
    return path;
  }

  bool load_file(const char* path)
  {
    if (load(SDL::IOFromFile(path, "rb"))) {
      set_status("loaded {}", filename(path));
      return true;
    } else {
      set_status("load failed: {}", filename(path));
      return false;
    }
  }

  void print(SDL::Point p, SDL::StringParam str)
  {
    renderer.SetDrawColor({0, 0, 0, SDL_ALPHA_OPAQUE});
    SDL::RenderDebugText(renderer, p + SDL::FPoint(1), str);
    renderer.SetDrawColor({0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE});
    SDL::RenderDebugText(renderer, p, str);
    renderer.SetDrawColor({0, 0, 0, SDL_ALPHA_OPAQUE});
  }

  SDL::AppResult Iterate() final
  {
    Uint64 tick = SDL_GetTicksNS();
    Uint64 delta = tick - last_tick;
    last_tick = tick;

    tick_acc += delta * FRAMES_PER_SECOND;
    bool updated = tick_acc >= NS_PER_SECOND;
    bool skip_audio = tick_acc >= MAX_AUDIO_LATENCY_FRAMES * NS_PER_SECOND;

    if (skip_audio) audiostream.Clear();

    while (tick_acc >= NS_PER_SECOND) {
      tick_acc -= NS_PER_SECOND;

      ram[IO_KEYBOARD] = Uint8(keystate >> 8);
      ram[IO_KEYBOARD + 1] = Uint8(keystate);

      Uint32 pc = read_u24(IO_PC);
      for (int i = 0; i < SCREEN_W * SCREEN_H; ++i) {
        Uint32 src = read_u24(pc);
        Uint32 dst = read_u24(pc + 3);
        ram[dst] = ram[src];
        pc = read_u24(pc + 6);
      }

      if (!skip_audio || tick_acc < NS_PER_SECOND) {
        Uint32 audioBank = read_u16(IO_AUDIO_BANK) << 8;
        audiostream.PutData({&ram[audioBank], SAMPLES_PER_FRAME});
      }
    }
    if (updated) {
      Uint32 screenPage = ram[IO_SCREEN_PAGE] << 16;
      texture.Update({}, &ram[screenPage], SCREEN_W);
    }

    renderer.RenderClear();

    if (display_help) {
      print({4, 4}, "Drop a BytePusher file in this");
      print({8, 12}, "window to load and run it!");
      print({4, 28}, "Press ENTER to switch between");
      print({8, 36}, "positional and symbolic input.");
    } else {
      renderer.RenderTexture(texture, {}, {});
    }

    if (status_ticks > 0) {
      if (updated) status_ticks--;
      print({4, SCREEN_H - 12}, status);
    }

    renderer.Present();
    return SDL::APP_CONTINUE;
  }

  static Uint16 keycode_mask(SDL_Keycode key)
  {
    int index;
    if (key >= SDLK_0 && key <= SDLK_9) {
      index = key - SDLK_0;
    } else if (key >= SDLK_A && key <= SDLK_F) {
      index = key - SDLK_A + 10;
    } else {
      return 0;
    }
    return Uint16(1 << index);
  }

  static Uint16 scancode_mask(SDL_Scancode scancode)
  {
    int index;
    switch (scancode) {
    case SDL_SCANCODE_1: index = 0x1; break;
    case SDL_SCANCODE_2: index = 0x2; break;
    case SDL_SCANCODE_3: index = 0x3; break;
    case SDL_SCANCODE_4: index = 0xc; break;
    case SDL_SCANCODE_Q: index = 0x4; break;
    case SDL_SCANCODE_W: index = 0x5; break;
    case SDL_SCANCODE_E: index = 0x6; break;
    case SDL_SCANCODE_R: index = 0xd; break;
    case SDL_SCANCODE_A: index = 0x7; break;
    case SDL_SCANCODE_S: index = 0x8; break;
    case SDL_SCANCODE_D: index = 0x9; break;
    case SDL_SCANCODE_F: index = 0xe; break;
    case SDL_SCANCODE_Z: index = 0xa; break;
    case SDL_SCANCODE_X: index = 0x0; break;
    case SDL_SCANCODE_C: index = 0xb; break;
    case SDL_SCANCODE_V: index = 0xf; break;
    default: return 0;
    }
    return Uint16(1 << index);
  }

  SDL::AppResult Event(const SDL::Event& event) final
  {
    switch (event.type) {
    case SDL::EVENT_QUIT: return SDL::APP_SUCCESS;
    case SDL::EVENT_DROP_FILE: load_file(event.drop.data); break;
    case SDL::EVENT_KEY_DOWN:
#ifndef __EMSCRIPTEN__
      if (event.key.key == SDLK_ESCAPE) return SDL::APP_SUCCESS;
#endif
      if (event.key.key == SDLK_RETURN) {
        positional_input = !positional_input;
        keystate = 0;
        if (positional_input) {
          set_status("switched to positional input");
        } else {
          set_status("switched to symbolic input");
        }
      }
      if (positional_input) {
        keystate |= scancode_mask(event.key.scancode);
      } else {
        keystate |= keycode_mask(event.key.key);
      }
      break;
    case SDL::EVENT_KEY_UP:
      if (positional_input) {
        keystate &= ~scancode_mask(event.key.scancode);
      } else {
        keystate &= ~keycode_mask(event.key.key);
      }
      break;
    default: break;
    }
    return SDL::APP_CONTINUE;
  }
};

static const struct
{
  const char* key;
  const char* value;
} extended_metadata[] = {
  {SDL_PROP_APP_METADATA_URL_STRING,
   "https://examples.libsdl.org/SDL3/demo/04-bytepusher/"},
  {SDL_PROP_APP_METADATA_CREATOR_STRING, "SDL team"},
  {SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "Placed in the public domain"},
  {SDL_PROP_APP_METADATA_TYPE_STRING, "game"}};

extern "C" SDLMAIN_DECLSPEC SDL::AppInterface* SDLCALL SDL_AppCreate(int,
                                                                     char*[])
{
  SDL::SetAppMetadata("Example Template", "1.0", "com.example.template");
  SDL::Init(SDL::INIT_VIDEO | SDL::INIT_AUDIO);

  auto primary_display = SDL::GetPrimaryDisplay();
  auto usable_bounds = primary_display.GetUsableBounds();
  int zoom = SDL::min((usable_bounds.w - usable_bounds.x) * 2 / 3 / SCREEN_W,
                      (usable_bounds.h - usable_bounds.y) * 2 / 3 / SCREEN_H);
  if (zoom < 1) zoom = 1;
  SDL::Window window;
  SDL::Renderer renderer;
  SDL::CreateWindowAndRenderer("SDL 3 BytePusher",
                               {SCREEN_W * zoom, SCREEN_H * zoom},
                               SDL::WINDOW_RESIZABLE,
                               &window,
                               &renderer);

  renderer.SetLogicalPresentation({SCREEN_W, SCREEN_H},
                                  SDL::LOGICAL_PRESENTATION_INTEGER_SCALE);

  return new BytePusher{std::move(window), std::move(renderer)};
}
