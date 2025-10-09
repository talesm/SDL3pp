Changelog
=========

## Version 0.2.1

Small bufix + examples

- Fix camera interface
- Add examples:
  - asyncio;
  - audio;
  - camera

## Version 0.2

This versions wraps all API and simplifies the way resources are handled.

- Simplify resource model, no more confusion of Base, Owner, Shared!
  - The main resource has all methods (eg Surface for SDL_Surface);
  - Resources with refcount will increment it on copy;
  - Resources without refcount have the copy disallowed;
    - a derived type with Ref suffix allows copy, but won't destroy the
      resource;
  - on weak resources, it instead allows copy, but won't destroy the
    resource;
    - a derived type with Scope suffix does not allow copy and destroy the
      resource;
  - a class Param suffix (like WindowParam) allows taking both wrapped and
    unwrapped resource as parameters seamlessly;
- Wrap both resources and wrappers now have the underlying type aliased
  with the Raw suffix (as in WindowRaw);
- Keep freestanding versions of methods-like functions alongside the class
  function versions;
- Automatically detect struct-like and classify as either resources or
  wrappers;
  - wrap functions having the struct as first parameter automatically;
  - Wrap structs with methods looking like *Destroy* function as resource;
  - wrap others as wrappers;
- Basic wrapping all missing API.
  - SDL_pen.h
  - SDL_sensor.h
  - SDL_touch.h
  - SDL_vulkan.h
  - SDL_metal.h
  - SDL_camera.h
  - SDL_asyncio.h
  - SDL_joystick.h
  - SDL_gamepad.h
  - SDL_hidapi.h
  - SDL_haptic.h
  - SDL_gpu.h

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
