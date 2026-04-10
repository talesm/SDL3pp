Todo for next version
=====================

- [x] Add missing tray free functions when previously had only methods:
  - [x] SetTrayEntryCallback with TrayCB callback;
  - [x] AppendTrayEntry;
- [x] Add TrayEntry constructor;
- [x] Docs improvements:
  - [x] Add namespace tab;
  - [x] Document namespace `prop`;
  - [x] Document namespace `prop::AudioStream`;
  - [x] Document namespace `prop::Display`;
  - [x] Document namespace `prop::FileDialog`;
  - [x] Document namespace `prop::GamepadCap`;
  - [x] Document namespace `prop::GPU*`;
  - [x] Document namespace `prop::Hidapi`;
  - [x] Document namespace `prop::IOStream`;
  - [x] Document namespace `prop::JoystickCap`;
  - [x] Document namespace `prop::Process`;
  - [x] Document namespace `prop::Renderer`;
  - [x] Document namespace `prop::Renderer`;
  - [x] Document namespace `prop::Surface`;
  - [x] Document namespace `prop::TextInput`;
  - [x] Document namespace `prop::Thread`;
  - [x] Document namespace `prop::AnimationEncoder`;
  - [x] Document namespace `prop::AnimationDecoder`;
  - [x] Document namespace `prop::Metadata`;
  - [x] Document namespace `prop::Audio`;
  - [x] Document namespace `prop::Mixer`;
  - [x] Document namespace `prop::MixMetadata`;
  - [x] Document namespace `prop::Play`;
  - [x] Document namespace `prop::Font`;
  - [x] Document namespace `prop::GpuFontEngine`;
  - [x] Document namespace `prop::RendererTextEngine`;
- [ ] Resource improvements:
  - [x] Create template base class for non-owning (Like TrayEntry);
  - [x] Create template base class for unique owning (Like Window);
  - [x] Create template base class for shared owning (Like Surface);

Backlog
-------

- [ ] (SDL3pp_image) Rename prop::Metadata to AnimationMetadata or something
  similar? Or even under AnimationDecoder;
- [ ] (SDL3pp_mixer) Rename prop::MixMetadata to AudioMetadata or something
  similar? Or even under Audio;
- [ ] Add a constructor to Properties similar to std::map initializer list;
- [ ] Revamp README, with examples and clearer instructions to integrate to
  project and everything else moved to other files;
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
- [ ] Commit & Push changes;
- [ ] Create new Release on <https://github.com/talesm/SDL3pp>
  - [ ] Don't forget go add SDL3pp_zip_amalgamation and SDL3pp_zip_full to the
    downloads!
- [ ] Create new Release in the UI, using the changelog text;
- [ ] Merge `releases` into `main`.
