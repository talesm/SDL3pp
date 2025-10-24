Todo for next version
====================

- [ ] Review macros that could be constexpr function or constant.
  - [x] SDL_AUDIO_MASK_BITSIZE
  - [x] SDL_AUDIO_MASK_FLOAT
  - [x] SDL_AUDIO_MASK_BIG_ENDIAN
  - [x] SDL_AUDIO_MASK_SIGNED
  - [ ] SDL_SOFTWARE_RENDERER (const char*)
  - [ ] SDL_RENDERER_VSYNC_DISABLED (int)
  - [ ] SDL_RENDERER_VSYNC_ADAPTIVE (int)
  - [ ] SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE (int)
  - [ ] SDL_SIZE_MAX (size_t)
  - [ ] SDL_FLT_EPSILON (float)
  - [ ] SDL_INIT_INTERFACE (template with concept...)
  - [ ] SDL_copyp (template with static_assert())
  - [ ] SDL_ICONV_ERROR (size_t)
  - [ ] SDL_ICONV_E2BIG (size_t)
  - [ ] SDL_ICONV_EILSEQ (size_t)
  - [ ] SDL_ICONV_EINVAL (size_t)
  - [ ] SDL_iconv_utf8_locale (template function)
  - [ ] SDL_iconv_utf8_ucs2 (template function)
  - [ ] SDL_iconv_utf8_ucs4 (template function)
  - [ ] SDL_iconv_wchar_utf8 (template function)
  - [ ] FontWeight enum:
    - [ ] TTF_FONT_WEIGHT_THIN
    - [ ] TTF_FONT_WEIGHT_EXTRA_LIGHT
    - [ ] TTF_FONT_WEIGHT_LIGHT
    - [ ] TTF_FONT_WEIGHT_NORMAL
    - [ ] TTF_FONT_WEIGHT_MEDIUM
    - [ ] TTF_FONT_WEIGHT_SEMI_BOLD
    - [ ] TTF_FONT_WEIGHT_BOLD
    - [ ] TTF_FONT_WEIGHT_EXTRA_BOLD
    - [ ] TTF_FONT_WEIGHT_BLACK
    - [ ] TTF_FONT_WEIGHT_EXTRA_BLACK
  - [ ] SDL_WINDOWPOS_UNDEFINED_MASK (int)
  - [ ] SDL_WINDOWPOS_UNDEFINED_DISPLAY (function)
  - [ ] SDL_WINDOWPOS_UNDEFINED (int)
  - [ ] SDL_WINDOWPOS_ISUNDEFINED (function)
  - [ ] SDL_WINDOWPOS_CENTERED_MASK (int)
  - [ ] SDL_WINDOWPOS_CENTERED_DISPLAY (function)
  - [ ] SDL_WINDOWPOS_CENTERED (int)
  - [ ] SDL_WINDOWPOS_ISCENTERED (function)

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
