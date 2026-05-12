#define SDL3PP_MAIN_USE_CLASS_CALLBACKS
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>

using namespace std::literals;

struct Main : SDL::AppInterface
{
  static constexpr SDL::Point windowSz = {640, 480};

  SDL::Window window{"examples/camera/read-and-draw", windowSz};
  SDL::Renderer renderer{window};

  SDL::Camera camera;
  SDL::Texture texture;

  Main()
  {
    SDL::OwnArray<SDL::CameraID> devices = SDL::GetCameras();
    if (devices.empty()) {
      throw std::runtime_error{"Couldn't find any camera devices! Please "
                               "connect a camera and try again."};
    }
    camera = SDL::OpenCamera(devices[0]);
  }

  SDL::AppResult Event(const SDL::Event& event) final
  {
    if (event.type == SDL::EVENT_QUIT) {
      return SDL::APP_SUCCESS; // end the program, reporting success to the OS.
    } else if (event.type == SDL::EVENT_CAMERA_DEVICE_APPROVED) {
      SDL::Log("Camera use approved by user!");
    } else if (event.type == SDL::EVENT_CAMERA_DEVICE_DENIED) {
      SDL::Log("Camera use denied by user!");
      return SDL::APP_FAILURE;
    }
    return SDL::APP_CONTINUE;
  }

  SDL::AppResult Iterate() final
  {
    Uint64 timestampNS = 0;
    if (auto frame = camera.AcquireFrame(&timestampNS)) {
      /* Some platforms (like Emscripten) don't know _what_ the camera offers
         until the user gives permission, so we build the texture and resize
         the window when we get a first frame from the camera. */
      if (!texture) {
        window.SetSize(frame.GetSize());
        texture = SDL::CreateTexture(renderer,
                                     frame.GetFormat(),
                                     SDL::TEXTUREACCESS_STREAMING,
                                     frame.GetSize());
      }
      if (texture) {
        texture.Update(std::nullopt, frame.GetPixels(), frame.GetPitch());
      }
    }

    renderer.SetDrawColor({0x99, 0x99, 0x99});
    renderer.RenderClear();

    // draw the latest camera frame, if available.
    if (texture) {
      renderer.RenderTexture(texture, std::nullopt, std::nullopt);
    }

    renderer.Present();
    return SDL::APP_CONTINUE;
  }
};

SDL3PP_DEFINE_CLASS_CALLBACKS(Main,
                              SDL::INIT_VIDEO | SDL::INIT_CAMERA,
                              "Example Camera Read and Draw",
                              "1.0",
                              "com.example.camera-read-and-draw")
