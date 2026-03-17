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
- [x] Wrap SDL_mixer;
- [ ] Remove overloading of `*_IO` variants:
  - [x] SDL::LoadWAV_IO();
  - [x] SDL::LoadFile_IO();
  - [x] SDL::SaveFile_IO();
  - [x] SDL::LoadAVIF_IO
  - [x] SDL::LoadBMP_IO
  - [x] SDL::LoadICO_IO
  - [x] SDL::LoadCUR_IO
  - [x] SDL::LoadGIF_IO
  - [x] SDL::LoadJPG_IO
  - [x] SDL::LoadJXL_IO
  - [x] SDL::LoadLBM_IO
  - [x] SDL::LoadPCX_IO
  - [x] SDL::LoadPNG_IO
  - [x] SDL::LoadPNM_IO
  - [x] SDL::LoadSVG_IO
  - [x] SDL::LoadQOI_IO
  - [x] SDL::LoadTGA_IO
  - [x] SDL::LoadTIF_IO
  - [x] SDL::LoadXCF_IO
  - [x] SDL::LoadXPM_IO
  - [x] SDL::LoadXV_IO
  - [x] SDL::LoadWEBP_IO
  - [x] SDL::LoadSizedSVG_IO
- [x] Error handling fixes:
  - [x] SDL::GetClipboardMimeTypes();
  - [x] SDL::GetGamepadMappings();
  - [x] SDL::GetGamepadJoystick();
  - [x] SDL::AcquireGPUCommandBuffer();
  - [x] SDL::MapGPUTransferBuffer();
  - [x] SDL::SubmitGPUCommandBufferAndAcquireFence();
  - [x] SDL::GetHapticFeatures();
  - [x] SDL::AttachVirtualJoystick();
  - [x] SDL::GetMouseNameForID();
  - [x] SDL::GetPenDeviceType();
  - [x] SDL::GetRendererName();
  - [x] SDL::GetRendererFromTexture();
  - [x] SDL::GetSensorID();
  - [x] SDL::GetAndroidJNIEnv();
  - [x] SDL::GetAndroidActivity();
  - [x] SDL::GetTouchDeviceName();
  - [x] SDL::CreateAnimationEncoder();
  - [x] SDL::CreateAnimationEncoder_IO();
  - [x] SDL::CreateAnimationEncoderWithProperties();
  - [x] SDL::CreateAnimationDecoder();
  - [x] SDL::CreateAnimationDecoder_IO();
  - [x] SDL::CreateAnimationDecoderWithProperties();
  - [x] SDL::GetGlyphImage();
  - [x] SDL::GetGlyphImageForIndex();
- [x] No longer throws:
  - [x] SDL::GetEventFilter();
  - [x] SDL::GetWindowFromEvent();
  - [x] SDL::TryLockMutex();
  - [x] SDL::TryLockRWLockForReading();
  - [x] SDL::TryLockRWLockForReading();
  - [x] SDL::GetRenderMetalLayer();
  - [x] SDL::GetRenderMetalCommandEncoder();
  - [x] SDL::GetWindowParent();
  - [x] SDL::SyncWindow();
  - [x] SDL::EGL_GetWindowSurface();
- [ ] Fix warnings:
  - [ ] Default initializing resources as nullptr;
  - [ ] lock's get() const.

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
