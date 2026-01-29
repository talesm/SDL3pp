include(FetchContent)

set(SDL3PP_BUNDLE_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external)
option(SDL3PP_BUNDLE_SHALLOW "Use shallow download" OFF)
option(SDL3PP_BUNDLE_DISCONNECTED "Use disconnected download" OFF)

set(SDL3PP_USE_SDL3_URL OFF CACHE STRING "Bundle SDL3 with this named tag/branch")
if(SDL3PP_USE_SDL3_URL)
  set(SDL3_URL ${SDL3PP_USE_SDL3_URL})
else ()
  set(SDL3_URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.0/SDL3-3.4.0.tar.gz)
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

if (SDL3PP_ENABLE_IMAGE)
  set(SDLIMAGE_INSTALL ON) # passed to external/SDL
  set(SDL3PP_USE_SDL3IMAGE_URL OFF CACHE STRING "Bundle SDL3_image with this named tag/branch")
  set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_image::SDL3_image)
  if(SDL3PP_USE_SDL3IMAGE_URL)
    set(SDL3IMAGE_URL ${SDL3PP_USE_SDL3IMAGE_URL})
  else ()
    set(SDL3IMAGE_URL https://github.com/libsdl-org/SDL_image/releases/download/release-3.4.0/SDL3_image-3.4.0.tar.gz)
  endif ()
  FetchContent_Declare(SDL3ImageExternal
    URL ${SDL3IMAGE_URL}
    OVERRIDE_FIND_PACKAGE
  )
  FetchContent_MakeAvailable(SDL3ImageExternal)
endif (SDL3PP_ENABLE_IMAGE)

if (SDL3PP_ENABLE_TTF)
  set(SDLTTF_INSTALL ON) # passed to external/SDL
  set(SDL3PP_USE_SDL3TTF_URL OFF CACHE STRING "Bundle SDL3_ttf with this named tag/branch")
  set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_ttf::SDL3_ttf)
  if(SDL3PP_USE_SDL3TTF_URL)
    set(SDL3TTF_URL ${SDL3PP_USE_SDL3TTF_URL})
  else ()
    set(SDL3TTF_URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2.tar.gz)
  endif ()
  FetchContent_Declare(SDL3TTFExternal
    URL ${SDL3TTF_URL}
    OVERRIDE_FIND_PACKAGE
  )
  FetchContent_MakeAvailable(SDL3TTFExternal)
endif (SDL3PP_ENABLE_TTF)
