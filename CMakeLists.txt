cmake_minimum_required(VERSION 3.27)
project(SDL3pp DESCRIPTION "A slim C++ wrapper for SDL3")

add_subdirectory(external/SDL)

set(CMAKE_CXX_STANDARD 23)

set(SDL3PP_ORIGINAL_INCLUDE_DIRS 
    ${PROJECT_SOURCE_DIR}/external/SDL/include/SDL3/
    ${PROJECT_SOURCE_DIR}/external/SDL_image/include/SDL3_image/
    ${PROJECT_SOURCE_DIR}/external/SDL_ttf/include/SDL3_ttf/
)
list(TRANSFORM SDL3PP_ORIGINAL_INCLUDE_DIRS APPEND "*.h" OUTPUT_VARIABLE SDL3PP_HEADER_GLOB)

file(GLOB SDL3PP_ORIGINAL_HEADERS CONFIGURE_DEPENDS ${SDL3PP_HEADER_GLOB})

#############################
### Enable/Disable options ##
#############################
set(SDL3PP_DEPENDENCIES SDL3::SDL3)

# SDL_image
option(SDL3PP_DISABLE_IMAGE "Disable compilation of SDL_Image" OFF)
if(SDL3PP_DISABLE_IMAGE)
    set(SDL3PP_AMALGAMATION_DEFS "${SDL3PP_AMALGAMATION_DEFS} -d SDL3PP_DISABLE_IMAGE")
else(SDL3PP_DISABLE_IMAGE)
    add_subdirectory(external/SDL_image)
    set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_image::SDL3_image)
endif(SDL3PP_DISABLE_IMAGE)

# SDL_ttf
option(SDL3PP_DISABLE_TTF "Disable compilation of SDL_TTF" OFF)
if(SDL3PP_DISABLE_TTF)
    set(SDL3PP_AMALGAMATION_DEFS "${SDL3PP_AMALGAMATION_DEFS} -d SDL3PP_DISABLE_TTF")
else(SDL3PP_DISABLE_TTF)
    add_subdirectory(external/SDL_ttf)
    set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_ttf::SDL3_ttf)
endif(SDL3PP_DISABLE_TTF)

# use StringParam
option(SDL3PP_DISABLE_STRING_PARAM "Disable StringParam wrapping" OFF)
if(SDL3PP_DISABLE_STRING_PARAM)
    set(SDL3PP_AMALGAMATION_DEFS "${SDL3PP_AMALGAMATION_DEFS} -d SDL3PP_DISABLE_STRING_PARAM")
endif(SDL3PP_DISABLE_STRING_PARAM)


# The SDL3pp library
add_library(SDL3pp_lib INTERFACE)
target_include_directories(SDL3pp_lib INTERFACE include)
target_link_libraries(SDL3pp_lib INTERFACE ${SDL3PP_DEPENDENCIES})

file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets COPY_ON_ERROR SYMBOLIC)

# The amalgamation header
set(SDL3PP_AMALGAMATION_DIR "${PROJECT_SOURCE_DIR}/amalgamation" CACHE FILEPATH "Amalgamation output directory")
set(SDL3PP_AMALGAMATION_FILENAME "${SDL3PP_AMALGAMATION_DIR}/SDL3pp/SDL3pp.h")
set(SDL3PP_AMALGAMATION_MAIN "${SDL3PP_AMALGAMATION_DIR}/SDL3pp/SDL3pp_main.h")
file(GLOB SDL3PP_WRAPPED_HEADERS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/include/SDL3pp/*.h)

add_library(SDL3pp_amalgamated INTERFACE ${SDL3PP_AMALGAMATION_FILENAME} ${SDL3PP_AMALGAMATION_MAIN})
target_include_directories(SDL3pp_amalgamated INTERFACE ${SDL3PP_AMALGAMATION_DIR})
target_link_libraries(SDL3pp_amalgamated INTERFACE ${SDL3PP_DEPENDENCIES})

option(SDL3PP_BUILD_EXAMPLES_AGAINST_AMALGAMATION "Build examples against amalgamation" ON)

if (SDL3PP_BUILD_EXAMPLES_AGAINST_AMALGAMATION)
    add_library(SDL3pp ALIAS SDL3pp_amalgamated)
else (SDL3PP_BUILD_EXAMPLES_AGAINST_AMALGAMATION)
    add_library(SDL3pp ALIAS SDL3pp_lib)
endif(SDL3PP_BUILD_EXAMPLES_AGAINST_AMALGAMATION)

add_custom_command(OUTPUT ${SDL3PP_AMALGAMATION_FILENAME}
    COMMAND node scripts/amalgamate --base-dir include/SDL3pp/ ${SDL3PP_AMALGAMATION_FILENAME} ${SDL3PP_AMALGAMATION_DEFS}
    DEPENDS ${SDL3PP_WRAPPED_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/amalgamate.js
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_command(OUTPUT ${SDL3PP_AMALGAMATION_MAIN}
    COMMAND cp include/SDL3pp/SDL3pp_main.h ${SDL3PP_AMALGAMATION_MAIN}
    DEPENDS "include/SDL3pp/SDL3pp_main.h"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Examples
add_subdirectory(examples)

# Unit tests
add_library(test_main OBJECT test/doctest.cpp test/doctest.h)
target_include_directories(test_main INTERFACE test/)
enable_testing()

file(GLOB UNITTEST CONFIGURE_DEPENDS test/SDL3pp*.cpp)
add_executable(SDL3pp_unitTests ${UNITTEST})
target_link_libraries(SDL3pp_unitTests PRIVATE SDL3pp_lib test_main)
add_test(NAME SDL3pp_unitTests COMMAND SDL3pp_unitTests)

# The documentation
# check if Doxygen is installed
find_package(Doxygen REQUIRED dot)
if (DOXYGEN_FOUND)
    # Main doc
    doxygen_add_docs(SDL3pp_doxygen
        COMMENT "Generate API docs for SDL3pp"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        CONFIG_FILE ${PROJECT_SOURCE_DIR}/docs/Doxyfile
    )
    # New Doxygen
    set(DOXYGEN_GENERATE_HTML NO)
    set(DOXYGEN_GENERATE_XML YES)
    set(DOXYGEN_EXCLUDE 
        "android-project/"
        "build/"
        "build-config/"
        "build-scripts/"
        "dist/"
        "examples/"
        "external/"
        "scripts/"
        "src/"
        "test/"
        "VisualC/"
        "VisualC-GDK/"
        "Xcode/"
    )
    set(DOXYGEN_BRIEF_MEMBER_DESC YES)
    set(DOXYGEN_REPEAT_BRIEF NO)
    set(DOXYGEN_JAVADOC_AUTOBRIEF YES)
    set(DOXYGEN_ALIASES
        "threadsafety=@par Thread safety:^^"
        "cat=@par Category:^^@ref "
    )
    set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
    set(DOXYGEN_STRIP_FROM_PATH "include/")
    set(DOXYGEN_MACRO_EXPANSION YES)
    set(DOXYGEN_EXPAND_ONLY_PREDEF YES)
    set(DOXYGEN_PREDEFINED
        "SDL_VERSION_ATLEAST(x,y,z)=1"
        "SDL3PP_DOC=1"
        "DOXYGEN_SHOULD_IGNORE_THIS=1"
        "SDL_WIKI_DOCUMENTATION_SECTION=1"
        "SDL_ALLOC_SIZE2(x,y)="
        "SDL_ALLOC_SIZE(x)="
        "SDL_DECLSPEC="
        "SDL_MALLOC="
        "SDL_OUT_Z_CAP(x)="
        "SDL_INOUT_Z_CAP(x)="
        "SDL_OUT_BYTECAP(x)="
        "SDL_IN_BYTECAP(x)="
        "SDL_SLOW_MEMCPY=1"
        "SDL_SLOW_MEMMOVE=1"
        "SDL_SLOW_MEMSET=1"
        "SDL_PRINTF_VARARG_FUNCV(x)="
        "SDL_PRINTF_VARARG_FUNC(x)="
        "SDL_SCANF_VARARG_FUNC(x)="
        "SDL_SCANF_VARARG_FUNCV(x)="
        "SDL_WPRINTF_VARARG_FUNC(x)="
        "SDL_WPRINTF_VARARG_FUNCV(x)="
    )
    set(DOXYGEN_MARKDOWN_ID_STYLE GITHUB)
    set(DOXYGEN_SHOW_HEADERFILE NO)
    set(DOXYGEN_SHOW_INCLUDE_FILES NO)
    set(DOXYGEN_INCLUDE_GRAPH NO)
    set(DOXYGEN_INCLUDED_BY_GRAPH NO)
    set(DOXYGEN_CALL_GRAPH NO)
    set(DOXYGEN_CALLER_GRAPH NO)
    set(DOXYGEN_GROUP_GRAPHS NO)

    set(SDL3PP_PARSE_DIR_PARAMS ${SDL3PP_PARSE_DIR_PARAMS} -x "${PROJECT_BINARY_DIR}/xml-sdl")
    set(DOXYGEN_XML_OUTPUT "xml-sdl")
    doxygen_add_docs(SDL3pp_SdlXml
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/SDL/
    )
    set(SDL3PP_PARSE_DIR_PARAMS ${SDL3PP_PARSE_DIR_PARAMS} -x "${PROJECT_BINARY_DIR}/xml-sdl_image")
    set(DOXYGEN_XML_OUTPUT "xml-sdl_image")
    doxygen_add_docs(SDL3pp_SdlImageXml
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/SDL_image/
    )
    set(SDL3PP_PARSE_DIR_PARAMS ${SDL3PP_PARSE_DIR_PARAMS} -x "${PROJECT_BINARY_DIR}/xml-sdl_ttf")
    set(DOXYGEN_XML_OUTPUT "xml-sdl_ttf")
    doxygen_add_docs(SDL3pp_SdlTtfXml
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/external/SDL_ttf/
    )
endif (DOXYGEN_FOUND)

#########################
## Refresh the headers ##
#########################
set(TRANSFORM_JSON ${PROJECT_SOURCE_DIR}/scripts/transform.json)
set(TARGET_JSON ${PROJECT_SOURCE_DIR}/scripts/target.json)
set(CONFIG_TARGET ${PROJECT_SOURCE_DIR}/scripts/config-target.json)
file(GLOB CPPFIER_SOURCES CONFIGURE_DEPENDS scripts/cppfier/src/*.js)

# Source SDL and libs state
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/scripts/source_xml.json
    COMMAND node scripts/cppfier xml scripts/config-source.json ${SDL3PP_PARSE_DIR_PARAMS}
    DEPENDS ${SDL3PP_ORIGINAL_HEADERS} scripts/config-source.json SDL3pp_SdlXml SDL3pp_SdlImageXml SDL3pp_SdlTtfXml
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/scripts/source.json
    COMMAND node scripts/gen-source
    DEPENDS ${PROJECT_SOURCE_DIR}/scripts/source_xml.json ${PROJECT_SOURCE_DIR}/scripts/gen-source.js
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# Generate the transform
add_custom_command(OUTPUT ${TRANSFORM_JSON}
    COMMAND node scripts/gen-transform ${TRANSFORM_JSON}
    DEPENDS ${PROJECT_SOURCE_DIR}/scripts/gen-transform.js
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# Target SDL3pp state
add_custom_command(OUTPUT ${TARGET_JSON}
    COMMAND node scripts/cppfier transform ${CONFIG_TARGET}
    DEPENDS ${PROJECT_SOURCE_DIR}/scripts/source.json 
            ${CONFIG_TARGET} 
            ${TRANSFORM_JSON}
            ${CPPFIER_SOURCES}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# Generate sources
add_custom_target(SDL3pp_generate
    COMMAND mkdir -p ${PROJECT_SOURCE_DIR}/src/generated/
    COMMAND node scripts/cppfier generate ${CONFIG_TARGET}
    COMMAND scripts/applyFormat.sh ${CONFIG_TARGET}
    DEPENDS ${CONFIG_TARGET} ${TARGET_JSON}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# Apply patches
add_custom_target(SDL3pp_refresh
    COMMAND scripts/applyPatches.sh
    DEPENDS SDL3pp_generate
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
