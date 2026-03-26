Todo for next version
=====================

New Version checklist:

- [ ] Update [changelog](changelog.md);
- [ ] Update version on [CMakeLists line 1](CMakeLists.txt#1);
- [ ] Update version on [gen-transform](scripts/gen-transform.js#7);
- [ ] Update [README Quick start links](README.md#quick-start--tldr) to new tag
  link;
- [ ] Update [README FetchContent](README.md#using-cmakes-fetchcontent)
  instructions to the new tag;
- [ ] build SDL3pp_zip_amalgamation and SDL3pp_zip_headers;
- [ ] Create new Release on <https://github.com/talesm/SDL3pp>
  - [ ] Don't forget go add SDL3pp_zip_amalgamation and SDL3pp_zip_full to the
    downloads!
- [ ] Merge branch into main

Next version
------------

- [x] Make StringParam differentiate `nullptr` from "";
- [x] On Windows, downloads pre-compiled versions, if possible.
- [ ] Disambiguate between SDL_image and SDL's load/save surface functions;
  - [x] Remove wrappers for IMG_LoadBMP_IO, IMG_SaveBMP and IMG_SaveBMP_IO, as
    they just call SDL's version of these;
- [x] Remove Surface::Load\* functions:
  - [x] Remove Surface::LoadBMP_IO(), use freestanding LoadBMP_IO() instead;
  - [x] Remove Surface::LoadBMP(), use freestanding LoadBMP() instead;
  - [x] Remove Surface::LoadPNG_IO(), use freestanding LoadPNG_IO() instead;
  - [x] Remove Surface::LoadPNG(), use freestanding LoadPNG() instead;
- [x] Fix bug preventing compilation on VisualStudio
  - [x] Use of gnu extension `?:`;
  - [x] Use of invalid constexpr;
- [x] Fix thread ctor for wrapped functions;
- [x] Warnings fixes:
  - [x] operator-> const for resources;
  - [x] Fix Camera::AcquireCameraFrame;
  - [x] Add SDL_assert on all narrowing due to different between std and SDL
    size types width;
  - [x] Don't default shared resources' copy assignment operators;
  - [x] Remove copy assignment operators on non-copyable resources;
  - [x] Remove copy constructors on non-copyable resources;
  - [x] Simplify scoped resources constructors and assignment operators;
  - [x] Fix several minor warnings;
- [ ] Properly encapsulate Param and ConstParam legacy;
- [x] Remove src/generated/ from version control;
- [x] Added CI tests for linux/Clang and Windows/MSVC;

Backlog
-------

- [ ] Documentation to refer to free functions instead of ctors whenever
  possible;
- [ ] Add windows-prebuilt option mode, downloading the version automatically.
- [ ] Create GetPixel/SetPixel on surface
- [ ] All SDL_*ID representing devices must have uniform interface:
  - [ ] AudioDeviceID;
  - [ ] CameraID;
  - [ ] DisplayID;
  - [ ] HapticID;
  - [ ] HapticEffectID;
  - [ ] JoystickID;
  - [ ] KeyboardID;
  - [ ] MouseID;
  - [ ] PenID;
  - [ ] PropertiesID;
  - [ ] SensorID;
  - [ ] ThreadID;
  - [ ] TimerID;
  - [ ] TouchID;
  - [ ] WindowID;
- [ ] Satellites libraries:
  - [ ] SDL_mixer;
  - [ ] SDL_net
  - [ ] SDL_gfx
- [ ] Add examples for input;

Think about
-----------

- Build and test on github
- Check if possible to make std::function based callbacks could be made a
  concept.
