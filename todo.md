Todo for next version
=====================

- [x] Improvements over Properties:
  - [x] Create PropertyProxy.As to be explicit over casting;
  - [x] Create prop_cast<> to be explicit over casting;

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
