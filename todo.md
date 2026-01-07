Todo for next version
=====================

New Version checklist:

- [ ] Create version branch (b0.7.0);
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

- [x] Fix a few build failure on clang;
- [x] Clean up generated resource code;
- [x] Fix MemoryBarrierRelease and MemoryBarrierAcquire;
- [x] Replace all SDL_\* parameters with \*Raw equivalents;
- [x] Fix GetGamepadBindings()
- [ ] Design a RAII workflow to deal with Lock/Unlock patterns present on
  several APIS;
  - [ ] AudioStreamLock for SDL_LockAudioStream;
  - [ ] CameraFrameLock for SDL_AcquireCameraFrame;
  - [x] JoystickApiLock for SDL_LockJoysticks;
  - [ ] MemoryBarrierLock for SDL_MemoryBarrierAcquire
  - [ ] PropertiesLock for SDL_LockProperties;
  - [ ] SurfaceLock for SDL_LockSurface;
  - [ ] TextureLock for SDL_LockTexture;

Backlog
-------

- [ ] Add windows-prebuilt option mode, downloading the version automatically.
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
