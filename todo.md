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
- [ ] Merge branch into main

Next version
------------

- [x] Merge \*Ref with \*Param resource classes into a single class:
  - [x] Make \*Ref always non-owning
  - [x] Replace \*Param with \*Ref;
  - [x] Remove \*Param;
  - [x] Rename \*ConstRef to \*ConstParam
- [ ] Wrap SDL_mixer;
- [ ] Remove overloading of `*_IO` variants, use `*FromIO` or similar;
- [x] Error handling fixes:
  - [x] SDL_GetClipboardMimeTypes();
  - [x] SDL_GetGamepadMappings();
  - [x] SDL_GetGamepadJoystick();
  - [x] SDL_AcquireGPUCommandBuffer();
  - [x] SDL_MapGPUTransferBuffer();
  - [x] SDL_SubmitGPUCommandBufferAndAcquireFence();
  - [x] SDL_GetHapticFeatures();
  - [x] SDL_AttachVirtualJoystick();
  - [x] SDL_GetMouseNameForID();
  - [x] SDL_GetPenDeviceType();
  - [x] SDL_GetRendererName();
  - [x] SDL_GetRendererFromTexture();
  - [x] SDL_GetSensorID();
  - [x] SDL_GetAndroidJNIEnv();
  - [x] SDL_GetAndroidActivity();
  - [x] SDL_GetTouchDeviceName();
  - [x] IMG_CreateAnimationEncoder();
  - [x] IMG_CreateAnimationEncoder_IO();
  - [x] IMG_CreateAnimationEncoderWithProperties();
  - [x] IMG_CreateAnimationDecoder();
  - [x] IMG_CreateAnimationDecoder_IO();
  - [x] IMG_CreateAnimationDecoderWithProperties();
  - [x] TTF_GetGlyphImage();
  - [x] TTF_GetGlyphImageForIndex();
- [ ] No longer throws:
  - [ ] SDL_GetEventFilter();
  - [ ] SDL_GetWindowFromEvent();
  - [ ] SDL_TryLockMutex();
  - [ ] SDL_TryLockRWLockForReading();
  - [ ] SDL_TryLockRWLockForReading();
  - [ ] SDL_GetRenderMetalLayer();
  - [ ] SDL_GetRenderMetalCommandEncoder();
  - [ ] SDL_GetWindowParent();
  - [ ] SDL_SyncWindow();
  - [ ] SDL_EGL_GetWindowSurface();

Backlog
-------

- [ ] Add windows-prebuilt option mode, downloading the version automatically.
- [ ] Create GetPixel/SetPixel on surface
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
  - [ ] SDL_mixer;
  - [ ] SDL_net
  - [ ] SDL_gfx
- [ ] Add examples for input;

Think about
-----------

- Build and test on github
- Check if possible to make std::function based callbacks could be made a
  concept.
