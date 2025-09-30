Changelog
=========

## Version 0.1

This version is a proof of concept with most used features by this author and most of the low hanging fruits to wrap to.

- C++ helpers and shims to better express the API intent:
  - Implement [resource management](https://talesm.github.io/SDL3pp/group__CategoryObjectWrapper.html);
  - Implement [helper](https://talesm.github.io/SDL3pp/classSDL_1_1StringParam.html)
    to transform `std::string`, `std::string_view` into `const char *`;
  - Implement [helpers](https://talesm.github.io/SDL3pp/group__CategoryCallbackWrapper.html)
    to use C++ lambdas and function objects as [callbacks](https://talesm.github.io/SDL3pp/callback.html)
    on SDL;
  - Wrap [RAII interface to SDL allocated pointers](https://talesm.github.io/SDL3pp/group__CategoryOwnPtr.html)
    returned by some SDL APIs;
  - Implement [Optional reference-like type](https://talesm.github.io/SDL3pp/classSDL_1_1OptionalRef.html)
    for simulating an `std::optional<T&>`;
  - Implement [span-like type](https://talesm.github.io/SDL3pp/classSDL_1_1SpanRef.html)
    compatible with our empty-derived structs we use to add methods to things
    like SDL_Rect.
- Wrap SDL (categories)[https://wiki.libsdl.org/SDL3/APIByCategory]:
  - Basics;
  - Video;
  - Event handling:
    - Event handling;
    - Keyboard support, keycodes and scancodes;
    - Mouse support.
  - Audio;
  - Time;
  - File I/O;
    - Filesystem Access;
    - I/O Streams.
  - Platform and CPU Information;
  - Additional Functionality;
- Satellites libraries:
  - SDL_image;
  - SDL_ttf;
