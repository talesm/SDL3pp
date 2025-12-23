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

Next version
------------

Simplify generation

- [x] Make source from transformation independent from the linked one;
- [x] Fix SDL_ttf build;

Backlog
-------

- [ ] Add windows-prebuilt option mode, downloading the version automatically.
- [ ] Design a RAII workflow to deal with Lock/Unlock patterns present on
  several APIS;
- [ ] Generate archive with the includes only;
- [ ] Generate archive with the amalgamation only;
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
  - [ ] SDL_gfx
- [ ] Create unified lock interface pattern:
  - [ ] for SDL_AcquireCameraFrame;
  - [ ] for SDL_LockSurface;
  - [ ] for SDL_LockTexture;
  - [ ] for SDL_LockAudioStream;
  - [ ] for SDL_LockProperties;
- [ ] Add examples for input;

Think about
-----------

- Build and test on github
- Check if possible to make std::function based callbacks could be made a
  concept.
