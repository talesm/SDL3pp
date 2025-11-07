Todo for next version
====================

Replace parse and parse-xml for a antlr parser, convert to typescript

- [x] Convert to typescript:
  - [x] Install typescript;
  - [x] Convert utils to ts;
  - [x] Convert generate to ts;
  - [x] Convert transform to ts;
  - [x] Update CMake to only look for dist/*.js;
- [x] Make antlr parser:
  - [x] Create command `parse-new` to use it;
  - [x] Create restricted grammar for C, enough to parse SDL library;
  - [x] Achieve parity with current `parse` and remove it.
  - [x] Achieve parity with current `parse-xml` and remove it.
- [x] Add miss-detected functions:
  - [x] SDL_main.h:
    - [x] SDL_RegisterApp() (windows only);
    - [x] SDL_UnregisterApp() (windows only);
  - [x] SDL_gpu.h:
    - [x] SDL_GDKSuspendGPU() (GDK only);
    - [x] SDL_GDKResumeGPU() (GDK only);
  - [x] SDL_stdinc.h:
    - [x] SDL_lltoa
    - [x] SDL_ulltoa
    - [x] SDL_strtoll
    - [x] SDL_strtoull
  - [x] SDL_camera.h:
    - [x] SDL_GetCameraSupportedFormats
- [ ] Reflow documentation paragraphs.

Backlog
=======

- [ ] Design a RAII workflow to deal with Lock/Unlock patterns present on
  several APIS;
- [ ] Generate archive with the includes only;
- [ ] Generate archive with the amalgamation only;
- [ ] Support to vcpkg;
- [ ] All SDL_*ID representing devices must have uniform interface:
  - [ ] MouseID
  - [ ] JoystickID
  - [ ] DisplayID
  - [ ] AudioDeviceID
  - [ ] ...
- [ ] Recognize refcount as shared;
- [ ] Satellites libraries:
  - [ ] SDL_mixer;
  - [ ] SDL_net
  - [ ] SDL_gfx??

Think about
===========

- Build and test on github
