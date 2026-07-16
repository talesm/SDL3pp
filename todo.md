Todo for next version
=====================

- [x] Upgrade SDL to 3.4.12;
- [x] Fix fallback definitions for SavePNG and SavePNG_IO reachable only when
  SDL3_image is not found;
- [x] Improvements over Properties:
  - [x] Create PropertyProxy.As to be explicit over casting;
  - [x] Create prop_cast<> to be explicit over casting;
- [x] Wrap SDL_timer conversion macros:
  - [x] SDL_MS_PER_SECOND is constexpr constant;
  - [x] SDL_US_PER_SECOND is constexpr constant;
  - [x] SDL_NS_PER_SECOND is constexpr constant;
  - [x] SDL_NS_PER_MS is constexpr constant;
  - [x] SDL_NS_PER_US is constexpr constant;
  - [x] SDL_SECONDS_TO_NS has int resulting version SecondsToNs;
  - [x] SDL_NS_TO_SECONDS has int resulting version MsToSeconds;
  - [x] SDL_MS_TO_NS is constexpr function
  - [x] SDL_NS_TO_MS is constexpr function
  - [x] SDL_US_TO_NS is constexpr function
  - [x] SDL_NS_TO_US is constexpr function
- [ ] Fix compatibility break when multiplying float by Point

Backlog
-------

- [ ] Ensure Properties follow all applicable conventions of iterator and range;
- [ ] Create ResourceConstBase
  - [ ] When resource has ResourceConstRef, derive from ResourceConstBase
    - It has only the immutable methods from ResourceBase;
  - [ ] ResourceBase derives from ResourceConstBase
  - [ ] Make ResourceConstRef derive from ResourceConstBase
- [ ] Revamp README, with examples and clearer instructions to integrate to
  project and everything else moved to other files;
- [ ] Docs improvements:
  - [ ] Update [./docs/transformation.md](transformation);
  - [ ] Update [./docs/callback.md](callback);
- [ ] Create ReadPixelRaw/WritePixelRaw on surface;
- [ ] Make string based ctors for Scancode && Keycode explicit;
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
  - [ ] SDL_gfx
- [ ] Add examples for input;

Version release checklist
-------------------------

- [ ] Merge `main` to `releases`;
- [ ] Update [changelog](changelog.md);
- [ ] Update version on [CMakeLists line 1](CMakeLists.txt);
- [ ] Update version on [gen-transform](scripts/gen-transform.js);
- [ ] Update [README Quick start links](README.md#quick-start--tldr) to new tag
  link;
- [ ] Update [README FetchContent](README.md#using-cmakes-fetchcontent)
  instructions to the new tag;
- [ ] build SDL3pp_zip_amalgamation and SDL3pp_zip_headers;
- [ ] Commit & Push changes;
- [ ] Create new Release on <https://github.com/talesm/SDL3pp>
  - [ ] Don't forget go add SDL3pp_zip_amalgamation and SDL3pp_zip_full to the
    downloads!
- [ ] Create new Release in the UI, using the changelog text;
- [ ] Merge `releases` into `main`.
