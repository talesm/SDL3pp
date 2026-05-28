Todo for next version
=====================

Breaking changes:

- [x] Remove old entry point (break compat);
- [x] Properties::Create is now a constructor;
- [x] Refactor resource to become less hack-y (probably breaking compat):
  - [x] Create new ResourceBase and make ResourceOwned derive from it;
    - It has the constructors to nullptr (default) and constructor from Raw;
    - Everything is trivial;
    - Dtor is trivial, but protected
  - [x] Move all methods to ResourceBase, except constructors;
  - [x] Create ResourceBaseT absorbs common functionality from new ResourceBase
  - [x] New ResourceRef is derived explicitly from ResourceBase:
    - Can be also constructed from Any const ResourceBase &;
    - Can be converted to raw resource;
  - [x] New ResourceRefT becomes template to new ResourceRef
- [x] Rename StartTextInput overload with properties to
  StartTextInputWithProperties;
- [x] Rename Borrow() to borrow();
- [x] Make TextEngine behave as ResourceBase:
  - [x] Remove TextEngineBase::Destroy();
  - [x] Remove restrictions on copy;
  - [x] Make TextEngineRef based on ResourceRefT;
- [x] Remove pair returning overload of CreateWindowAndRenderer()
- [x] Remove Window returning overload of CreateWindowAndRenderer()

Non-breaking changes

- [x] Document refers to free functions instead of method versions;
- [x] Fix ObjectRef(nullptr).
- [x] Add standalone functions for window related methods defined in different
  files:
  - [x] StartTextInput();
  - [x] StartTextInputWithProperties();
  - [x] TextInputActive();
  - [x] StopTextInput();
  - [x] ClearComposition();
  - [x] SetTextInputArea();
  - [x] GetTextInputArea();
  - [x] ScreenKeyboardShown
  - [x] WarpMouseInWindow();
  - [x] SetWindowRelativeMouseMode();
  - [x] GetWindowRelativeMouseMode();
  - [x] GetRenderer();
- [x] TextEngineRef now has access to its methods;

Backlog
-------

- [ ] Create ResourceConstBase
  - [ ] When resource has ResourceConstRef, derive from ResourceConstBase
    - It has only the immutable methods from ResourceBase;
  - [ ] ResourceBase derives from ResourceConstBase
  - [ ] Make ResourceConstRef derive from ResourceConstBase
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
