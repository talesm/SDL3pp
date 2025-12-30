# SDL3pp

A "port" of SDL3 in C++.

This basically wrap SDL3 naturally OO looking concepts into proper C++ classes
and objects, trying to as straightforward as possible. It was inspired on
[SDL2pp](https://github.com/libSDL2pp/libSDL2pp), with the addition of flexible
memory management and wrappers for string and callbacks.

## Quick start / TLDR

- You have 3 options to add SDL3pp to your project:
  1. Download [the single header](./amalgamation/SDL3pp/) (as
     [zip](https://github.com/talesm/SDL3pp/releases/download/0.5.8/SDL3pp_amalgamated-0.5.8.zip)
     or
     [tarball](https://github.com/talesm/SDL3pp/releases/download/0.5.8/SDL3pp_amalgamated-0.5.8.tar.gz))
     and add to your project;
  2. You can also download the
     [the latest release](https://github.com/talesm/SDL3pp/releases/download/0.5.8/SDL3pp-0.5.8.zip)
     (also available as
     [tarball](https://github.com/talesm/SDL3pp/releases/download/0.5.8/SDL3pp-0.5.8.tar.gz))
     then [build](#building) and [install](#installing) the project;
  3. If you use CMake, you can also set up
     [FetchContent](#using-cmakes-fetchcontent) to automatically do that for
     you;
- See [the generated docs](https://talesm.github.io/SDL3pp/) and particularly
  the [API reference](https://talesm.github.io/SDL3pp/ApiByCategory.html);
- See [Example](#example) and [Examples directory](./examples/).

## Goals

- Be header only, we are only wrapping SDL here;
- Mostly wrap the naturally OO-looking API into actual OO C++ constructs;
- Put everything into a `SDL` namespace instead of prefixes;
- Interfaces should accept both C structs and the C++ wraps, so you can adapt a
  codebase gradually or just choose to use only what you deem necessary.
- Flexible, while we use
  [RAII](https://en.cppreference.com/w/cpp/language/raii) idiom by default, you
  have the choice to not use it and, for example, manage memory yourself.

## Example

```cpp
#include <iostream>
#include <SDL3pp/SDL3pp.h>
#include <SDL3pp/SDL3pp_main.h>
 
using namespace std::chrono_literals;
 
int main(int argc, char** argv)
{
  SDL::Init(SDL::INIT_VIDEO);
  constexpr SDL::Point WINDOW_SZ = {400, 400};
  auto [window, renderer] = SDL::CreateWindowAndRenderer("Test", WINDOW_SZ);
 
  SDL::Texture characterTexture{
    renderer, std::format("{}../assets/smiley.png", SDL::GetBasePath())};
  SDL::FRect characterRect(SDL::FPoint(WINDOW_SZ) / 2 - SDL::FPoint{64, 64},
                           {128, 128});
 
  bool running = true;
  while (running) {
    while (auto ev = SDL::PollEvent()) {
      if (ev->type == SDL::EVENT_QUIT) { running = false; }
    }
    renderer.SetDrawColorFloat({.5f, .5f, .5f, 1.f});
    renderer.RenderClear();
    renderer.SetDrawColorFloat({0.f, 0.725f, 0.f, 1.f});
    renderer.RenderFillRect(SDL::FRect{10, 10, 380, 380});
    renderer.RenderTexture(characterTexture, {}, characterRect);
 
    renderer.Present();
    SDL::Delay(1ns);
  }
 
  return 0;
}
```

[examples directory](https://github.com/talesm/SDL3pp/tree/main/examples)

## Building

If aren't using the "full" version, ensure you have the necessary dependencies
installed:

- SDL3
- SDL3_image (optional)
- SDL3_ttf (optional)

Assuming you are on the source dir, you can build with:

```sh
cmake -S . -B build
cmake --build build
```

If CMake can't find the dependencies, you might have to pass them through
cmake-gui or using the following:

```sh
cmake -DSDL3_DIR=path-to-SDL3-dir build
cmake -DSDL3_image_DIR=path-to-SDL3_image-dir build
cmake -DSDL3_ttf_DIR=path-to-SDL3_ttf-dir build
```

## Installing

If you have SDL3 already set up on your project, you can just copy the contents
of [amalgamation/](./amalgamation/SDL3pp/) or [include](./include/SDL3pp/)
directly to your project.

### System installation

If you like to have a whole system intallation, you can checkout the project
[build](#building), and then you can install on you system with cmake:

```sh
cmake --install build
```

Alteratively can move into a custom location with:

```sh
cmake --install build --install-prefix <directory>
```

## Using Cmake's FetchContent

If you are already using CMake for your project, you can use the following
command to download from git automatically:

```cmake
include(FetchContent)
# set(SDL3PP_FORCE_BUNDLED ON) # Enable this to force CMake to download SDL, SDL_image and SDL_ttf
FetchContent_Declare(SDL3ppExternal
  URL https://github.com/talesm/SDL3pp/releases/download/0.5.8/SDL3pp-0.5.8.tar.gz
)
FetchContent_MakeAvailable(SDL3ppExternal)

# Link to your project
target_link_libraries(MyProject
  PRIVATE SDL3pp::SDL3pp
)
```
