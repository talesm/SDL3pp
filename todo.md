Todo for simplifying
====================

Things for a version 0.2

- [x] Simplify resource model, no more confusion of Base, Owner, Shared!
  - [x] The main resource has all methods (eg Surface for SDL_Surface);
  - [x] Resources with refcount will increment it on copy;
  - [x] Resources without refcount have the copy disallowed;
    - [x] a derived type with Ref suffix allows copy, but won't destroy the
      resource;
  - [x] on weak resources, it instead allows copy, but won't destroy the
    resource;
    - [x] a derived type with Scope suffix does not allow copy and destroy the
      resource;
  - [x] a class Param suffix (like WindowParam) allows taking both wrapped and
    unwrapped resource as parameters seamlessly;
- [x] Wrap both resources and wrappers now have the underlying type aliased
  with the Raw suffix (as in WindowRaw);
- [x] Keep freestanding versions of methods-like functions alongside the class
  function versions;
- [ ] Automatically detect struct-like and classify as either resources or
  wrappers;
  - [x] wrap functions having the struct as first parameter automatically;
  - [ ] detect refcount as shared;
  - [x] Wrap structs with methods looking like *Destroy* function as resource;
  - [x] wrap others as wrappers;
- [ ] Basic wrapping all missing API.
  - [x] SDL_pen.h
  - [x] SDL_sensor.h
  - [x] SDL_touch.h
  - [x] SDL_vulkan.h
  - [x] SDL_metal.h
  - [x] SDL_camera.h
  - [x] SDL_asyncio.h
  - [ ] SDL_joystick.h
  - [ ] SDL_gamepad.h
  - [ ] SDL_hidapi.h
  - [ ] SDL_haptic.h
  - [ ] SDL_gpu.h

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
  - [ ] File I/O;
    - [ ] Async I/O.
- [ ] Satellites libraries:
  - [ ] SDL_mixer;
  - [ ] SDL_net
  - [ ] SDL_gfx??

Think about
===========

- Build and test on github
