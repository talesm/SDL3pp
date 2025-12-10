Todo for next version
=====================

New Version checklist:

- [ ] Update version on [CMakeLists line 1](CMakeLists.txt#1);
- [ ] Update
  [README Quick start links for amalgamation](README.md#quick-start--tldr) to
  new tag link;
- [ ] Update README ExternalProject instructions to the new tag;
- [ ] Update [changelog](changelog.md);
- [ ] build SDL3pp_zip_amalgamation and SDL3pp_zip_full;
- [ ] Create new Release on <https://github.com/talesm/SDL3pp>
  - [ ] Don't forget go add SDL3pp_zip_amalgamation and SDL3pp_zip_full to the
    downloads!

Next version
------------

Reword callback handling.

- [x] Remove hacky and leaky stored callback wrapping, except for the safer
  callOnce, and use a more lightweight function wrapper that fit in a single
  pointer.
- [x] Use less error prone unified diffs to patch the API.

Backlog
-------

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

Think about
-----------

- Build and test on github
- Check if possible to make std::function based callbacks could be made a
  concept.
