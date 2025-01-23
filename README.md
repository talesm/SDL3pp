SDL3pp
======

A "port" of SDL3 in C++.

This basically wrap SDL3 naturally OO looking concepts into proper 
C++ classes and objects, trying to as straightforward as possible.

## Goals

- Be header only, we are mostly wrapping thing here
- Intefaces should accept both C structs and the C++ wraps, so you
can adapt a codebase gradually or just choose to use only what 
you deem necessary.
- Flexible, while we want to implement RAII and such idioms, you have
can choose to not use any of it and manage memory yourself.

## Examples

TODO

## Documentation

- Todo file (TODO)
- Doxygen (TODO)

## Building

Assuming you are on the source dir, you can build with:

```sh
cmake -S . -B build
cmake --build build
```

Installing is not supported yet, but hopefully will be in the future.
