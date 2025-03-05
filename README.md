SDL3pp
======

A "port" of SDL3 in C++.

This basically wrap SDL3 naturally OO looking concepts into proper 
C++ classes and objects, trying to as straightforward as possible.
It was inspired on [SDL2pp](https://github.com/libSDL2pp/libSDL2pp), 
with the addition of flexible memory management and wrappers for 
string and callbacks.

See API reference [here](https://talesm.github.io/SDL3pp/).

## Goals

- Be header only, we are mostly wrapping thing here
- Intefaces should accept both C structs and the C++ wraps, so you
can adapt a codebase gradually or just choose to use only what 
you deem necessary.
- Flexible, while we use RAII and such idioms, you have
the choice to not use them and, for example, manage memory yourself.

## Examples

TODO

## Documentation

- See [Transformations](./transformation.md) to understand the design approach.
- Todo file (TODO)
- See [Doxygen](https://talesm.github.io/SDL3pp/)

## Building

Assuming you are on the source dir, you can build with:

```sh
cmake -S . -B build
cmake --build build
```

Installing is not supported yet, but hopefully will be in the future.
