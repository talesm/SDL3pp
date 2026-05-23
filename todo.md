Todo for next version
=====================

- [x] Add missing wrapper for MIX_PROP_PLAY_START_ORDER_NUMBER (this was added
  on SDL_mixer 3.2.2);
- [x] Add experimental support for SDL3_net;
- [x] Improve SDL3pp_rect.h:
  - [x] Single argument constructor for both (F)Point;
  - [x] Fix (F)Rect constructor from 2 (F)Points;
  - [x] Multiply, Module, Division between 2 Point;
  - [x] Multiply, Division between 2 FPoint;
  - [x] Implicit convert Point to FPoint;
  - [x] Explicit convert FPoint to Point and FRect to Rect;
  - [x] (F)Rect have \[GS]etPosition and SetSize;
  - [x] (F)Rect have position() and size(), that returns reference.
- [ ] Fix ObjectRef(nullptr).

Backlog
-------

- [ ] Remove old entry point;
- [ ] Add a constructor to Properties similar to std::map initializer list;
- [ ] Revamp README, with examples and clearer instructions to integrate to
  project and everything else moved to other files;
- [ ] Docs improvements:
  - [ ] Update [./docs/transformation.md](transformation);
  - [ ] Update [./docs/callback.md](callback);
- [ ] Create ReadPixelRaw/WritePixelRaw on surface;
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
  - [ ] SDL_net
  - [ ] SDL_gfx
- [ ] Add examples for input;

Think about
-----------

- Build and test on github
- Check if possible to make std::function based callbacks could be made a
  concept.

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
