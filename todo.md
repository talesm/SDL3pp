Todo for next version
====================

- [x] Make install script (contrib from @brccabral)
- [x] Review macros that could be constexpr function or constant.
  - [x] SDL_AUDIO_MASK_BITSIZE
  - [x] SDL_AUDIO_MASK_FLOAT
  - [x] SDL_AUDIO_MASK_BIG_ENDIAN
  - [x] SDL_AUDIO_MASK_SIGNED
  - [x] SDL_SOFTWARE_RENDERER (const char*)
  - [x] SDL_RENDERER_VSYNC_DISABLED (int)
  - [x] SDL_RENDERER_VSYNC_ADAPTIVE (int)
  - [x] SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE (int)
  - [x] SDL_FLT_EPSILON (float)
  - [x] SDL_INIT_INTERFACE (template with concept...)
  - [x] SDL_copyp (template with static_assert())
  - [x] SDL_ICONV_ERROR (size_t)
  - [x] SDL_ICONV_E2BIG (size_t)
  - [x] SDL_ICONV_EILSEQ (size_t)
  - [x] SDL_ICONV_EINVAL (size_t)
  - [x] SDL_iconv_utf8_locale (template function)
  - [x] SDL_iconv_utf8_ucs2 (template function)
  - [x] SDL_iconv_utf8_ucs4 (template function)
  - [x] SDL_iconv_wchar_utf8 (template function)
  - [x] TTF_FONT_WEIGHT_THIN
  - [x] TTF_FONT_WEIGHT_EXTRA_LIGHT
  - [x] TTF_FONT_WEIGHT_LIGHT
  - [x] TTF_FONT_WEIGHT_NORMAL
  - [x] TTF_FONT_WEIGHT_MEDIUM
  - [x] TTF_FONT_WEIGHT_SEMI_BOLD
  - [x] TTF_FONT_WEIGHT_BOLD
  - [x] TTF_FONT_WEIGHT_EXTRA_BOLD
  - [x] TTF_FONT_WEIGHT_BLACK
  - [x] TTF_FONT_WEIGHT_EXTRA_BLACK
  - [x] SDL_WINDOWPOS_UNDEFINED_MASK (int)
  - [x] SDL_WINDOWPOS_UNDEFINED_DISPLAY (function)
  - [x] SDL_WINDOWPOS_UNDEFINED (int)
  - [x] SDL_WINDOWPOS_ISUNDEFINED (function)
  - [x] SDL_WINDOWPOS_CENTERED_MASK (int)
  - [x] SDL_WINDOWPOS_CENTERED_DISPLAY (function)
  - [x] SDL_WINDOWPOS_CENTERED (int)
  - [x] SDL_WINDOWPOS_ISCENTERED (function)
- [ ] Fix constants not properly guarded by SDL_VERSION_ATLEAST()


Backlog
=======

- [ ] All SDL_*ID representing devices must have uniform interface
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
