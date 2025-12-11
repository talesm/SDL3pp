include(FetchContent)

set(SDL3PP_BUNDLE_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external)
option(SDL3PP_BUNDLE_SHALLOW "Use shallow download" OFF)
option(SDL3PP_BUNDLE_DISCONNECTED "Use disconnected download" OFF)

set(CACHE SDL3PP_USE_SDL3_TAG TYPE STRING HELP "Bundle SDL3 with this named tag/branch" VALUE "")
if(SDL3PP_USE_SDL3_TAG)
  set(SDL3_TAG ${SDL3PP_USE_SDL3_TAG})
else ()
  set(SDL3_TAG release-3.2.28)
endif ()

set(SDL_INSTALL ON) # passed to external/SDL
FetchContent_Declare(SDL3External
  GIT_REPOSITORY git@github.com:libsdl-org/SDL.git
  GIT_TAG ${SDL3_TAG}
  GIT_SUBMODULES_RECURSE ON
  GIT_SHALLOW ${SDL3PP_BUNDLE_SHALLOW}
  GIT_PROGRESS ON
  UPDATE_DISCONNECTED ${SDL3PP_BUNDLE_DISCONNECTED}
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3External)

set(SDLIMAGE_INSTALL ON) # passed to external/SDL
if (SDL3PP_ENABLE_IMAGE)
  set(CACHE SDL3PP_USE_SDL3IMAGE_TAG TYPE STRING HELP "Bundle SDL3_image with this named tag/branch" VALUE "")
  if(SDL3PP_USE_SDL3IMAGE_TAG)
    set(SDL3IMAGE_TAG ${SDL3PP_USE_SDL3IMAGE_TAG})
  else ()
    set(SDL3IMAGE_TAG release-3.2.4)
  endif ()
  FetchContent_Declare(SDL3ImageExternal
    GIT_REPOSITORY git@github.com:libsdl-org/SDL_image.git
    GIT_TAG ${SDL3IMAGE_TAG}
    GIT_SUBMODULES_RECURSE ON
    GIT_SHALLOW ${SDL3PP_BUNDLE_SHALLOW}
    GIT_PROGRESS ON
    UPDATE_DISCONNECTED ${SDL3PP_BUNDLE_DISCONNECTED}
  )
  FetchContent_MakeAvailable(SDL3ImageExternal)
endif (SDL3PP_ENABLE_IMAGE)

set(SDLTTF_INSTALL ON) # passed to external/SDL
if (SDL3PP_ENABLE_TTF)
  set(CACHE SDL3PP_USE_SDL3TTF_TAG TYPE STRING HELP "Bundle SDL3_ttf with this named tag/branch" VALUE "")
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
    UPDATE_DISCONNECTED ${SDL3PP_BUNDLE_DISCONNECTED}
  )
  FetchContent_MakeAvailable(SDL3TTFExternal)
endif (SDL3PP_ENABLE_TTF)
