Todo for next version
=====================

- [x] Upgrade SDL to 3.4.20;
- [x] Remove amalgamation targets from build and from README;
- [x] Make string based ctors for Scancode && Keycode explicit;
- [ ] Remove misleading autoclose parameters:
  - [x] LoadFile_IO
  - [x] SaveFile_IO
  - [ ] LoadBMP_IO;
  - [ ] LoadTrustedPNG_IO;
  - [ ] SaveBMP_IO;
  - [ ] SaveTrustedPNG_IO;
  - [ ] LoadSurface_IO;
  - [ ] LoadSurfaceTyped_IO;
  - [ ] LoadTexture_IO;
  - [ ] LoadTextureTyped_IO;
  - [ ] LoadGPUTexture_IO;
  - [ ] LoadGPUTextureTyped_IO;
  - [ ] LoadPNG_IO;
  - [ ] SaveTyped_IO;
  - [ ] SaveAVIF_IO;
  - [ ] SaveCUR_IO;
  - [ ] SaveGIF_IO;
  - [ ] SaveICO_IO;
  - [ ] SaveJPG_IO;
  - [ ] SavePNG_IO;
  - [ ] SaveTGA_IO;
  - [ ] SaveWEBP_IO;
  - [ ] LoadAnimation_IO;
  - [ ] LoadAnimationTyped_IO;
  - [ ] SaveAnimationTyped_IO;
  - [ ] SaveANIAnimation_IO;
  - [ ] SaveAPNGAnimation_IO;
  - [ ] SaveAVIFAnimation_IO;
  - [ ] SaveGIFAnimation_IO;
  - [ ] SaveWEBPAnimation_IO;
  - [ ] CreateAnimationEncoder_IO;
  - [ ] CreateAnimationDecoder_IO;
  - [x] LoadWAV_IO;
  - [ ] LoadAudio_IO;
  - [ ] LoadRawAudio_IO;
  - [ ] LoadRawAudioNoCopy (just remove free_when_done);
  - [ ] SetTrackIOStream;
  - [ ] SetTrackRawIOStream;
  - [ ] CreateAudioDecoder_IO;
  - [ ] OpenFontIO;

Backlog
-------

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
