SDL3pp
======

A "port" of SDL3 in C++.

This basically wrap SDL3 naturally OO looking concepts into proper 
C++ classes and objects, trying to as straightforward as possible.
It was inspired on [SDL2pp](https://github.com/libSDL2pp/libSDL2pp), 
with the addition of flexible memory management and wrappers for 
string and callbacks.

## Quick start / TLDR

- Download [the single header](./amalgamation/SDL3pp/SDL3pp.h) and add to your project;
- See [API reference](https://talesm.github.io/SDL3pp/ApiByCategory.html);
- See [Examples directory](./examples/).

## Goals

- Be header only, we are mostly wrapping thing here;
  - We even have a 
    [the single header to just put in your project](./amalgamation/SDL3pp/SDL3pp.h)
- Mostly wrap the naturally OO-looking API into actual OO C++ constructs;
  - Also add little quick improvements like using vocabulary types to better 
    fit C++ idioms;
  - See [Transformations](https://talesm.github.io/SDL3pp/transformations.html) for more details;
- Put everything into a `SDL` namespace instead of prefixes;
  - For uniformity we also wrap non OO entities at lower priority (like 
    aliases, functions, constants, etc);
- Interfaces should accept both C structs and the C++ wraps, so you can adapt a
  codebase gradually or just choose to use only what you deem necessary.
- Flexible, while we use [RAII](https://en.cppreference.com/w/cpp/language/raii)
  idiom by default, you have the choice to not use it and, for example, manage
  memory yourself.

## Documentation

- See most recent [Doxygen](https://talesm.github.io/SDL3pp/)
  - See [Api reference](https://talesm.github.io/SDL3pp/ApiByCategory.html);
  - See [Transformations](https://talesm.github.io/SDL3pp/transformations.html)
    to understand the strategies we adopt to map we do from C to C++;
  - See [Design notes](https://talesm.github.io/SDL3pp/group__DesignNotes.html)
    to understand the rationally behind some design patterns and choices we use.
- See the [examples directory](https://github.com/talesm/SDL3pp/tree/main/examples)
  for some examples;
- See our roadmap-like [todo](todo.md).

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

Installing is not supported yet, but hopefully will be in the future.
