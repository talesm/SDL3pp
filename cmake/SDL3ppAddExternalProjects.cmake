include(FetchContent)

set(SDL3PP_BUNDLE_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external)
option(SDL3PP_BUNDLE_SHALLOW "Use shallow download" OFF)
option(SDL3PP_BUNDLE_DISCONNECTED "Use disconnected download" OFF)

set(SDL3PP_USE_SDL3_TAG OFF CACHE STRING "Bundle SDL3 with this named tag/branch")
if(SDL3PP_USE_SDL3_TAG)
  set(SDL3_TAG ${SDL3PP_USE_SDL3_TAG})
else ()
  set(SDL3_TAG release-3.4.0)
endif ()

set(SDL_INSTALL ON) # passed to external/SDL
FetchContent_Declare(SDL3External
  GIT_REPOSITORY git@github.com:libsdl-org/SDL.git
  GIT_TAG ${SDL3_TAG}
  GIT_SUBMODULES_RECURSE ON
  GIT_SHALLOW ${SDL3PP_BUNDLE_SHALLOW}
  GIT_PROGRESS ON
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3External)

if (SDL3PP_ENABLE_IMAGE)
  set(SDLIMAGE_INSTALL ON) # passed to external/SDL
  set(SDL3PP_USE_SDL3IMAGE_TAG OFF CACHE STRING "Bundle SDL3_image with this named tag/branch")
  if(SDL3PP_USE_SDL3IMAGE_TAG)
    set(SDL3IMAGE_TAG ${SDL3PP_USE_SDL3IMAGE_TAG})
  else ()
    set(SDL3IMAGE_TAG release-3.2.6)
  endif ()
  FetchContent_Declare(SDL3ImageExternal
    GIT_REPOSITORY git@github.com:libsdl-org/SDL_image.git
    GIT_TAG ${SDL3IMAGE_TAG}
    GIT_SUBMODULES_RECURSE ON
    GIT_SHALLOW ${SDL3PP_BUNDLE_SHALLOW}
    GIT_PROGRESS ON
  )
  FetchContent_MakeAvailable(SDL3ImageExternal)
endif (SDL3PP_ENABLE_IMAGE)

if (SDL3PP_ENABLE_TTF)
  set(SDLTTF_INSTALL ON) # passed to external/SDL
  set(SDL3PP_USE_SDL3TTF_TAG OFF CACHE STRING "Bundle SDL3_ttf with this named tag/branch")
  if(SDL3PP_USE_SDL3TTF_TAG)
    set(SDL3TTF_TAG ${SDL3PP_USE_SDL3TTF_TAG})
  else ()
    set(SDL3TTF_TAG release-3.2.2)
  endif ()
  FetchContent_Declare(SDL3TTFExternal
    GIT_REPOSITORY git@github.com:libsdl-org/SDL_ttf.git
    GIT_TAG ${SDL3TTF_TAG}
    GIT_SUBMODULES_RECURSE ON
    GIT_SHALLOW ${SDL3PP_BUNDLE_SHALLOW}
    GIT_PROGRESS ON
  )
  FetchContent_MakeAvailable(SDL3TTFExternal)
endif (SDL3PP_ENABLE_TTF)
