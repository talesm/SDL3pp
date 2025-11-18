Changelog
=========

## Version 0.4

This version had an overall review of documentation and the scripts that
generate the bindings, which allowed me to fix some function signatures.

On the binding generation side, we replaced the previous two source parsers to a
single new more accurate one. Previously we had one based on doxygen generated
XML which was very precise, but a missed a few functions; and one doing a ad-hoc
parsing of source files, which was very buggy. The new one uses a grammar
library and is way more reliable for our purposes.

We also added a simplified grammar for the documentation blocks, which allowed
to better transform them and fix formatting issues to make them more readable.
This also allowed to fix the signature of a few functions.

- Fix bug (#25) where the calling SDL::ReadAs<>() failed to compile on Clang.
- Convert cppfier scripts to typescript;
- Replace the XML and the adhoc header parser to a single, grammar based one;
  - Removed the `xml` cppfier command and replaced the `parse` with the new one;
- Parsing the documentation blocks content, so we can more accurately detect
  their tags and better format them.
- Add miss-detected functions:
  - SDL_main.h:
    - SDL_RegisterApp() (windows only);
    - SDL_UnregisterApp() (windows only);
  - SDL_gpu.h:
    - SDL_GDKSuspendGPU() (GDK only);
    - SDL_GDKResumeGPU() (GDK only);
- Fix Signature:
  - SDL_stdinc.h:
    - SDL::lltoa
    - SDL::ulltoa
    - SDL::strtoll
    - SDL::strtoull
  - SDL_camera.h:
    - SDL::GetCameraSupportedFormats
  - SDL_gamepad.h
    - SDL::GetGamepadMappings
    - SDL::GetGamepadMappingForGUID
    - SDL::GetGamepadMapping

## Version 0.3.1

This version reviews install script, adds functions as constants for macros and
fix errors with retro-compatibility:

- Make install script (contrib from @brccabral);
- Fix constants not properly guarded by SDL_VERSION_ATLEAST();
- Review macros that could be constexpr function or constant.
  - SDL_audio.h macros turned constexpr constants: 
    - SDL_AUDIO_MASK_BITSIZE;
    - SDL_AUDIO_MASK_FLOAT;
    - SDL_AUDIO_MASK_BIG_ENDIAN;
    - SDL_AUDIO_MASK_SIGNED;
  - SDL_render.h macros turned constexpr constants
    - SDL_SOFTWARE_RENDERER;
    - SDL_RENDERER_VSYNC_DISABLED;
    - SDL_RENDERER_VSYNC_ADAPTIVE;
    - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;
  - SDL_stdinc.h macros turned constexpr constants:
    - SDL_FLT_EPSILON;
    - SDL_ICONV_ERROR;
    - SDL_ICONV_E2BIG;
    - SDL_ICONV_EILSEQ;
    - SDL_ICONV_EINVAL;
  - SDL_stdinc.h macros turned functions:
    - SDL_INIT_INTERFACE (Renamed as InitInterface);
    - SDL_copyp;
    - SDL_iconv_utf8_locale;
    - SDL_iconv_utf8_ucs2;
    - SDL_iconv_utf8_ucs4;
    - SDL_iconv_wchar_utf8;
  - SDL_ttf.h macros turned constexpr constants:
    - TTF_FONT_WEIGHT_THIN;
    - TTF_FONT_WEIGHT_EXTRA_LIGHT;
    - TTF_FONT_WEIGHT_LIGHT;
    - TTF_FONT_WEIGHT_NORMAL;
    - TTF_FONT_WEIGHT_MEDIUM;
    - TTF_FONT_WEIGHT_SEMI_BOLD;
    - TTF_FONT_WEIGHT_BOLD;
    - TTF_FONT_WEIGHT_EXTRA_BOLD;
    - TTF_FONT_WEIGHT_BLACK;
    - TTF_FONT_WEIGHT_EXTRA_BLACK;
  - SDL_video.h macros turned constexpr constants:
    - SDL_WINDOWPOS_UNDEFINED_MASK;
    - SDL_WINDOWPOS_UNDEFINED;
    - SDL_WINDOWPOS_CENTERED_MASK;
    - SDL_WINDOWPOS_CENTERED;
  - SDL_video.h macros turned constexpr functions:
    - SDL_WINDOWPOS_UNDEFINED_DISPLAY;
    - SDL_WINDOWPOS_ISUNDEFINED;
    - SDL_WINDOWPOS_CENTERED_DISPLAY;
    - SDL_WINDOWPOS_ISCENTERED.

## Version 0.3

This version reviews the API and add missing free-standing functions. 
Also does a general clean up on the code.

- Remove generated docs from repository
  - use actions to generate and upload;
- Fix comparisons for wrappers (the ones using extending);
- Review older files to ensure class and freestanding functions are in sync;
  - SDL_audio.h
  - SDL_pixels.h
  - SDL_properties.h
  - SDL_rect.h
  - SDL_render.h
  - SDL_surface.h
  - SDL_video.h
  - SDL_image.h
- Review and simplify older transformations:
  - SDL_audio.h
  - SDL_pixels.h
  - SDL_properties.h
  - SDL_rect.h
  - SDL_render.h
  - SDL_surface.h
  - SDL_video.h
  - SDL_image.h

## Version 0.2.1

Small bufix + examples

- Fix camera interface
- Add examples:
  - asyncio;
  - audio;
  - camera

## Version 0.2

This versions wraps all API and simplifies the way resources are handled.

- Simplify resource model, no more confusion of Base, Owner, Shared!
  - The main resource has all methods (eg Surface for SDL_Surface);
  - Resources with refcount will increment it on copy;
  - Resources without refcount have the copy disallowed;
    - a derived type with Ref suffix allows copy, but won't destroy the
      resource;
  - on weak resources, it instead allows copy, but won't destroy the
    resource;
    - a derived type with Scope suffix does not allow copy and destroy the
      resource;
  - a class Param suffix (like WindowParam) allows taking both wrapped and
    unwrapped resource as parameters seamlessly;
- Wrap both resources and wrappers now have the underlying type aliased
  with the Raw suffix (as in WindowRaw);
- Keep freestanding versions of methods-like functions alongside the class
  function versions;
- Automatically detect struct-like and classify as either resources or
  wrappers;
  - wrap functions having the struct as first parameter automatically;
  - Wrap structs with methods looking like *Destroy* function as resource;
  - wrap others as wrappers;
- Basic wrapping all missing API.
  - SDL_pen.h
  - SDL_sensor.h
  - SDL_touch.h
  - SDL_vulkan.h
  - SDL_metal.h
  - SDL_camera.h
  - SDL_asyncio.h
  - SDL_joystick.h
  - SDL_gamepad.h
  - SDL_hidapi.h
  - SDL_haptic.h
  - SDL_gpu.h

## Version 0.1

This version is a proof of concept with most used features by this author and most of the low hanging fruits to wrap to.

- C++ helpers and shims to better express the API intent:
  - Implement [resource management](https://talesm.github.io/SDL3pp/group__CategoryObjectWrapper.html);
  - Implement [helper](https://talesm.github.io/SDL3pp/classSDL_1_1StringParam.html)
    to transform `std::string`, `std::string_view` into `const char *`;
  - Implement [helpers](https://talesm.github.io/SDL3pp/group__CategoryCallbackWrapper.html)
    to use C++ lambdas and function objects as [callbacks](https://talesm.github.io/SDL3pp/callback.html)
    on SDL;
  - Wrap [RAII interface to SDL allocated pointers](https://talesm.github.io/SDL3pp/group__CategoryOwnPtr.html)
    returned by some SDL APIs;
  - Implement [Optional reference-like type](https://talesm.github.io/SDL3pp/classSDL_1_1OptionalRef.html)
    for simulating an `std::optional<T&>`;
  - Implement [span-like type](https://talesm.github.io/SDL3pp/classSDL_1_1SpanRef.html)
    compatible with our empty-derived structs we use to add methods to things
    like SDL_Rect.
- Wrap SDL (categories)[https://wiki.libsdl.org/SDL3/APIByCategory]:
  - Basics;
  - Video;
  - Event handling:
    - Event handling;
    - Keyboard support, keycodes and scancodes;
    - Mouse support.
  - Audio;
  - Time;
  - File I/O;
    - Filesystem Access;
    - I/O Streams.
  - Platform and CPU Information;
  - Additional Functionality;
- Satellites libraries:
  - SDL_image;
  - SDL_ttf;
