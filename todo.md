Todo for next version
=====================

Reword callback handling.

- [ ] Remove hacky and leaky stored callback wrapping, except for the safer
  callOnce;
- [ ] Make std::function's *CB generation explicit;
- [ ] Make templated version of callback, where the userdata can be any pointer
  type;

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
