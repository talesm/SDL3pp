Todo for some stability
=======================

Things for a version 0.1

- [x] C++ helpers and shims to better express the API intent:
  - [x] Implement [resource management](https://talesm.github.io/SDL3pp/group__CategoryObjectWrapper.html);
  - [x] Implement [helper](https://talesm.github.io/SDL3pp/classSDL_1_1StringParam.html)
    to transform `std::string`, `std::string_view` into `const char *`;
  - [x] Implement [helpers](https://talesm.github.io/SDL3pp/group__CategoryCallbackWrapper.html)
    to use C++ lambdas and function objects as [callbacks](https://talesm.github.io/SDL3pp/callback.html)
    on SDL;
  - [x] Wrap [RAII interface to SDL allocated pointers](https://talesm.github.io/SDL3pp/group__CategoryOwnPtr.html)
    returned by some SDL APIs;
  - [x] Implement [Optional reference-like type](https://talesm.github.io/SDL3pp/classSDL_1_1OptionalRef.html)
    for simulating an `std::optional<T&>`;
  - [x] Implement [span-like type](https://talesm.github.io/SDL3pp/classSDL_1_1SpanRef.html)
    compatible with our empty-derived structs we use to add methods to things
    like SDL_Rect.
- [ ] Wrap SDL (categories)[https://wiki.libsdl.org/SDL3/APIByCategory]:
  - [x] Basics;
  - [ ] Video;
  - [ ] Event handling:
    - [x] Event handling;
    - [ ] Keyboard support, keycodes and scancodes;
    - [ ] Mouse support.
  - [ ] Audio;
  - [ ] Time;
  - [ ] File I/O;
    - [ ] Filesystem Access;
    - [x] I/O Streams.
  - [ ] Platform and CPU Information;
  - [ ] Additional Functionality;
- [x] Satellites libraries:
  - [x] SDL_image;
  - [x] SDL_ttf;

Backlog
=======

- [ ] Wrap SDL (categories)[https://wiki.libsdl.org/SDL3/APIByCategory]:
  - [ ] Event handling:
    - [ ] Joystick support;
    - [ ] Gamepad support;
    - [ ] Touch support;
    - [ ] Pen support;
    - [ ] Sensors;
    - [ ] HIDAPI.
  - [ ] GPU;
  - [ ] Threads;
  - [ ] File I/O;
    - [ ] Storage Abstraction;
    - [ ] Async I/O.
- [ ] Satellites libraries:
  - [ ] SDL_mixer;
  - [ ] SDL_net
  - [ ] SDL_gfx??
  - [ ] SDL_rtf??

Think about
===========

- Model shared_ptr and weak ptr on resources
