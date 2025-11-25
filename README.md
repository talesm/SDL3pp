SDL3pp
======

A "port" of SDL3 in C++.

This basically wrap SDL3 naturally OO looking concepts into proper C++ classes
and objects, trying to as straightforward as possible. It was inspired on
[SDL2pp](https://github.com/libSDL2pp/libSDL2pp), with the addition of flexible
memory management and wrappers for string and callbacks.

## Quick start / TLDR

- Download [the single header](./amalgamation/SDL3pp/) (as
  [zip](https://github.com/talesm/SDL3pp/releases/download/0.4.1/SDL3pp_amalgamated.zip)
  or
  [tarball](https://github.com/talesm/SDL3pp/releases/download/0.4.1/SDL3pp_amalgamated.tar.gz))
  and add to your project;
- Alternatively, download
  [the latest release](https://github.com/talesm/SDL3pp/releases) then follow:
  - [Build](#building) instructions;
  - [Installation](#installing) instructions;
- See [API reference](https://talesm.github.io/SDL3pp/ApiByCategory.html);
- See [Example](#example) and [Examples directory](./examples/).


## Goals

- Be header only, we are only wrapping SDL here;
- Mostly wrap the naturally OO-looking API into actual OO C++ constructs;
  - Also add little quick improvements like using vocabulary types to better fit
    C++ idioms;
  - See [Transformations](https://talesm.github.io/SDL3pp/transformations.html)
    for more details;
- Put everything into a `SDL` namespace instead of prefixes;
  - For uniformity we also wrap non OO entities at lower priority (like aliases,
    functions, constants, etc) into SDL namespace;
- Interfaces should accept both C structs and the C++ wraps, so you can adapt a
  codebase gradually or just choose to use only what you deem necessary.
- Flexible, while we use
  [RAII](https://en.cppreference.com/w/cpp/language/raii) idiom by default, you
  have the choice to not use it and, for example, manage memory yourself.

## Documentation

- See most recent [Doxygen](https://talesm.github.io/SDL3pp/)
  - See [Api reference](https://talesm.github.io/SDL3pp/ApiByCategory.html);
  - See [Transformations](https://talesm.github.io/SDL3pp/transformations.html)
    to understand the strategies we adopt to map we do from C to C++;
  - See [Design notes](https://talesm.github.io/SDL3pp/group__DesignNotes.html)
    to understand the rationally behind some design patterns and choices we use.
- See the
  [examples directory](https://github.com/talesm/SDL3pp/tree/main/examples) for
  some examples;
- See our roadmap-ish [todo](todo.md).

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

Assuming you are on the source dir, you can build with:

```sh
cmake -S . -B build
cmake --build build
```

## Installing

There are a few ways to install SDL3pp:

### Copy into project

The most basic way is to copy the contents of
[amalgamation/](./amalgamation/SDL3pp/) or [include](./include/SDL3pp/) directly
to your project.

### System installation:

After [build](#building), you can install on you system with cmake:

```sh
cmake --install build
```

Alteratively can move into a custom location with:

```sh
cmake --install build --install-prefix <directory>
```
