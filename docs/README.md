SDL3pp {#mainpage}
==================

A "port" of SDL3 in C++.

This basically wrap SDL3 naturally OO looking concepts into proper C++ classes
and objects, trying to as straightforward as possible. It was inspired on
[SDL2pp](https://github.com/libSDL2pp/libSDL2pp), with the addition of flexible
memory management and wrappers for string and callbacks.

## Quick start / TLDR

- Download [the latest release](https://github.com/talesm/SDL3pp/releases);
- [Build](#building) instructions; 
- [Installation](#installing) instructions;
- See [API reference](#ApiByCategory);
- See [Example](#example) and
  [Examples directory](https://github.com/talesm/SDL3pp/tree/main/examples).

## Goals

- Be header only, we are only wrapping SDL here;
- Mostly wrap the naturally OO-looking API into actual OO C++ constructs;
  - Also add little quick improvements like using vocabulary types to better 
    fit C++ idioms;
  - See @ref transformations for more details;
- Put everything into a `SDL` namespace instead of prefixes;
  - For uniformity we also wrap non OO entities at lower priority (like 
    aliases, functions, constants, etc);
- Interfaces should accept both C structs and the C++ wraps, so you can adapt a
  codebase gradually or just choose to use only what you deem necessary.
- Flexible, while we use [RAII](https://en.cppreference.com/w/cpp/language/raii)
  idiom by default, you have the choice to not use it and, for example, manage
  memory yourself.

## Documentation

- See @ref ApiByCategory for some examples.
- See @ref transformations to understand the design approach.
- See @ref DesignNotes to understand the rationally behind some design patterns
  and choices we use.
- See [our source code](https://github.com/talesm/SDL3pp);
- See the [examples directory](https://github.com/talesm/SDL3pp/tree/main/examples)
- See [most recent version of this Doxygen](https://talesm.github.io/SDL3pp/)

## Example

@include window_with_renderer.cpp

See more examples at [examples directory](https://github.com/talesm/SDL3pp/tree/main/examples)

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
