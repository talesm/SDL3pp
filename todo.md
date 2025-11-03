Todo for next version
====================

Replace parse and parse-xml for a antlr parser, convert to typescript

- [ ] Convert to typescript:
  - [ ] Install typescript;
  - [ ] Convert generate to ts;
  - [ ] Convert transform to ts;
  - [ ] create tsconfig to generate index.js, including the js files;
  - [ ] Update CMake to only look for index.js;
- [ ] Make antlr parser:
  - [ ] Create command `parse-new` to use it;
  - [ ] Create restricted grammar for C, enough to parse SDL library;
  - [ ] Achieve parity with current `parse` and `parse-xml` and remove them.

Backlog
=======

- [ ] Design a RAII workflow to deal with Lock/Unlock patterns present on
  several APIS;
- [ ] Generate archive with the includes only;
- [ ] Generate archive with the amalgamation only;
- [ ] Support to vcpkg;
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
  - [ ] SDL_gfx??

Think about
===========

- Build and test on github
