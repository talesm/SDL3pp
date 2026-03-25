include(FetchContent)

set(SDL3PP_BUNDLE_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external)
option(SDL3PP_BUNDLE_SHALLOW "Use shallow download" OFF)
option(SDL3PP_BUNDLE_DISCONNECTED "Use disconnected download" OFF)

set(SDL3PP_USE_SDL3_URL OFF CACHE STRING "Bundle SDL3 with this named tag/branch")
if (SDL3PP_USE_SDL3_URL)
  set(SDL3_URL ${SDL3PP_USE_SDL3_URL})
elseif (WIN32 AND MSVC)
  set(SDL3_URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.2/SDL3-devel-3.4.2-VC.zip)
else ()
  set(SDL3_URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.2/SDL3-3.4.2.tar.gz)
endif ()

set(SDL_INSTALL ON) # passed to SDL3External
set(SDL_TESTS OFF)  # passed to SDL3External
set(SDL_TEST_LIBRARY OFF) # passed to SDL3External
set(SDL_X11_XTEST OFF)  # passed to SDL3External

FetchContent_Declare(SDL3External
  URL ${SDL3_URL}
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3External)
if (NOT TARGET SDL3::SDL3)
  FetchContent_GetProperties(SDL3External SOURCE_DIR SDL3External_SOURCE_DIR)
  set(SDL3 ${SDL3External_SOURCE_DIR}/cmake CACHE PATH "Path to SDL3 source directory" FORCE)
  find_package(SDL3 REQUIRED)
endif()

if (SDL3PP_ENABLE_IMAGE)
  set(SDLIMAGE_INSTALL ON) # passed to external/SDL
  set(SDL3PP_USE_SDL3IMAGE_URL OFF CACHE STRING "Bundle SDL3_image with this named tag/branch")
  set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_image::SDL3_image)
  if(SDL3PP_USE_SDL3IMAGE_URL)
    set(SDL3IMAGE_URL ${SDL3PP_USE_SDL3IMAGE_URL})
  elseif (WIN32)
    set(SDL3IMAGE_URL https://github.com/libsdl-org/SDL_image/releases/download/release-3.4.0/SDL3_image-devel-3.4.0-mingw.tar.gz)
  else ()
    set(SDL3IMAGE_URL https://github.com/libsdl-org/SDL_image/releases/download/release-3.4.0/SDL3_image-3.4.0.tar.gz)
  endif ()
  FetchContent_Declare(SDL3ImageExternal
    URL ${SDL3IMAGE_URL}
    OVERRIDE_FIND_PACKAGE
  )
  FetchContent_MakeAvailable(SDL3ImageExternal)
  if (NOT TARGET SDL3_image::SDL3_image)
    FetchContent_GetProperties(SDL3ImageExternal SOURCE_DIR SDL3ImageExternal_SOURCE_DIR)
    set(SDL3_image ${SDL3ImageExternal_SOURCE_DIR}/cmake CACHE PATH "Path to SDL3_image source directory" FORCE)
    find_package(SDL3_image REQUIRED)
  endif()
endif (SDL3PP_ENABLE_IMAGE)

if (SDL3PP_ENABLE_MIXER)
  set(SDLMIXER_INSTALL ON) # passed to external/SDL
  set(SDL3PP_USE_SDL3MIXER_URL OFF CACHE STRING "Bundle SDL3_mixer with this named tag/branch")
  set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_mixer::SDL3_mixer)
  if(SDL3PP_USE_SDL3MIXER_URL)
    set(SDL3MIXER_URL ${SDL3PP_USE_SDL3MIXER_URL})
  elseif (WIN32)
    set(SDL3MIXER_URL https://github.com/libsdl-org/SDL_mixer/releases/download/release-3.2.0/SDL3_mixer-devel-3.2.0-mingw.tar.gz)
  else ()
    set(SDL3MIXER_URL https://github.com/libsdl-org/SDL_mixer/releases/download/release-3.2.0/SDL3_mixer-3.2.0.tar.gz)
  endif ()
  FetchContent_Declare(SDL3MixerExternal
    URL ${SDL3MIXER_URL}
    OVERRIDE_FIND_PACKAGE
  )
  FetchContent_MakeAvailable(SDL3MixerExternal)
  if (NOT TARGET SDL3_mixer::SDL3_mixer)
    FetchContent_GetProperties(SDL3MixerExternal SOURCE_DIR SDL3MixerExternal_SOURCE_DIR)
    set(SDL3_mixer ${SDL3MixerExternal_SOURCE_DIR}/cmake CACHE PATH "Path to SDL3_mixer source directory" FORCE)
    find_package(SDL3_mixer REQUIRED)
  endif()
endif (SDL3PP_ENABLE_MIXER)

if (SDL3PP_ENABLE_TTF)
  set(SDLTTF_INSTALL ON) # passed to external/SDL
  set(SDL3PP_USE_SDL3TTF_URL OFF CACHE STRING "Bundle SDL3_ttf with this named tag/branch")
  set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_ttf::SDL3_ttf)
  if(SDL3PP_USE_SDL3TTF_URL)
    set(SDL3TTF_URL ${SDL3PP_USE_SDL3TTF_URL})
  elseif (WIN32)
    set(SDL3TTF_URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2-mingw.tar.gz)
  else ()
    set(SDL3TTF_URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2.tar.gz)
  endif ()
  FetchContent_Declare(SDL3TTFExternal
    URL ${SDL3TTF_URL}
    OVERRIDE_FIND_PACKAGE
  )
  FetchContent_MakeAvailable(SDL3TTFExternal)
  if (NOT TARGET SDL3_ttf::SDL3_ttf)
    FetchContent_GetProperties(SDL3TTFExternal SOURCE_DIR SDL3TTFExternal_SOURCE_DIR)
    set(SDL3_ttf ${SDL3TTFExternal_SOURCE_DIR}/cmake CACHE PATH "Path to SDL3_ttf source directory" FORCE)
    find_package(SDL3_ttf REQUIRED)
  endif()
endif (SDL3PP_ENABLE_TTF)
