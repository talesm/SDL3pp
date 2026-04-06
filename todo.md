Todo for next version
=====================

- [x] Docs improvements:
  - [x] Fix [./docs/DesignNotes.dox](DesignNotes) links;
  - [x] Use interactive SGV graphs;
  - [x] Remove [./docs/constructor-tag.md](constructor tag);
- [x] Add `<=>` operator for Point, FPoint;
- [ ] Revamp README, with examples and clearer instructions to integrate to
  project and everything else moved to other files;
- [ ] Bump SDL version to 3.4.4;
- [x] Use a nicer theme for generated docs;

Backlog
-------

- [ ] Simplify class callbacks:
  - [ ] Use inheritance;
  - [ ] Have base class with common methods;
  - [ ] Document it;
- [ ] Docs improvements:
  - [ ] Update [./docs/DesignNotes.dox](resources) to remove ResourceParam info;
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
- [ ] Create new Release on <https://github.com/talesm/SDL3pp>
  - [ ] Don't forget go add SDL3pp_zip_amalgamation and SDL3pp_zip_full to the
    downloads!
- [ ] Commit & Push changes;
- [ ] Create new Release in the UI, using the changelog text;
- [ ] Merge `releases` into `main`.
