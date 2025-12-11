cmake_minimum_required(VERSION 3.27)
project(SDL3pp 
    VERSION 0.5.3
    DESCRIPTION "A slim C++ wrapper for SDL3" 
    HOMEPAGE_URL "https://github.com/talesm/SDL3pp"
)

include(GNUInstallDirs)

option(SDL3PP_FORCE_BUNDLED "Disable compilation of SDL_Image" OFF)

if (SDL3PP_FORCE_BUNDLED)
    set(SDL_INSTALL ON) # passed to external/SDL
    add_subdirectory(external/SDL)
else ()
    find_package(SDL3 QUIET)
    if (NOT TARGET SDL3::SDL3)
        set(SDL_INSTALL ON) # passed to external/SDL
        add_subdirectory(external/SDL)
    endif ()
endif ()

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
    if (SDL3PP_FORCE_BUNDLED)
        set(SDLIMAGE_INSTALL ON) # passed to external/SDL_image
        add_subdirectory(external/SDL_image)
        set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_image::SDL3_image)
    else ()
        find_package(SDL3_image QUIET)
        set(SDL3_IMAGE_DEPENDENCY "find_dependency(SDL3_image REQUIRED)") # placeholder in cmake.in
        if (TARGET SDL3_image::SDL3_image)
            set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_image::SDL3_image)
        else ()
            set(SDLIMAGE_INSTALL ON) # passed to external/SDL_image
            if (EXISTS external/SDL_image/CMakeLists.txt)
                add_subdirectory(external/SDL_image)
                set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_image::SDL3_image)
            endif()
        endif ()
    endif ()
endif(SDL3PP_DISABLE_IMAGE)

# SDL_ttf
option(SDL3PP_DISABLE_TTF "Disable compilation of SDL_TTF" OFF)
if(SDL3PP_DISABLE_TTF)
    set(SDL3PP_AMALGAMATION_DEFS "${SDL3PP_AMALGAMATION_DEFS} -d SDL3PP_DISABLE_TTF")
else(SDL3PP_DISABLE_TTF)
    if (SDL3PP_FORCE_BUNDLED)
        set(SDLTTF_INSTALL ON) # passed to external/SDL_ttf
        add_subdirectory(external/SDL_ttf)
        set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_ttf::SDL3_ttf)
    else ()
        find_package(SDL3_ttf QUIET)
        set(SDL3_TTF_DEPENDENCY "find_dependency(SDL3_ttf REQUIRED)") # placeholder in cmake.in
        if (TARGET SDL3_ttf::SDL3_ttf)
            set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_ttf::SDL3_ttf)
        else()
            set(SDLTTF_INSTALL ON) # passed to external/SDL_ttf
            if (EXISTS external/SDL_ttf/CMakeLists.txt)
                add_subdirectory(external/SDL_ttf)
                set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_ttf::SDL3_ttf)
            endif()
        endif ()
    endif ()
endif(SDL3PP_DISABLE_TTF)

# use StringParam
option(SDL3PP_DISABLE_STRING_PARAM "Disable StringParam wrapping" OFF)
if(SDL3PP_DISABLE_STRING_PARAM)
    set(SDL3PP_AMALGAMATION_DEFS "${SDL3PP_AMALGAMATION_DEFS} -d SDL3PP_DISABLE_STRING_PARAM")
endif(SDL3PP_DISABLE_STRING_PARAM)


file(GLOB_RECURSE SDL3pp_lib_HEADERS "${PROJECT_SOURCE_DIR}/include/*.h")

file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets COPY_ON_ERROR SYMBOLIC)

set(SDL3PP_COMPILE_FEATURES cxx_std_23)

# The SDL3pp library
add_library(SDL3pp INTERFACE)
target_sources(SDL3pp INTERFACE FILE_SET sdl3pp_headers TYPE HEADERS BASE_DIRS include/ FILES ${SDL3pp_lib_HEADERS})
add_library(SDL3pp::SDL3pp ALIAS SDL3pp)
target_compile_features(SDL3pp INTERFACE ${SDL3PP_COMPILE_FEATURES})
target_link_libraries(SDL3pp INTERFACE ${SDL3PP_DEPENDENCIES})
target_include_directories(SDL3pp INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# The amalgamation header
set(SDL3PP_AMALGAMATION_DIR "${PROJECT_SOURCE_DIR}/amalgamation" CACHE FILEPATH "Amalgamation output directory")
set(SDL3PP_AMALGAMATION_FILENAME "${SDL3PP_AMALGAMATION_DIR}/SDL3pp/SDL3pp.h")
set(SDL3PP_AMALGAMATION_MAIN "${SDL3PP_AMALGAMATION_DIR}/SDL3pp/SDL3pp_main.h")
set(SDL3PP_AMALGAMATION_VULKAN "${SDL3PP_AMALGAMATION_DIR}/SDL3pp/SDL3pp_vulkan.h")
file(GLOB SDL3PP_WRAPPED_HEADERS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/include/SDL3pp/*.h)

add_library(SDL3pp_amalgamated INTERFACE ${SDL3PP_AMALGAMATION_FILENAME} ${SDL3PP_AMALGAMATION_MAIN} ${SDL3PP_AMALGAMATION_VULKAN})
target_compile_features(SDL3pp_amalgamated INTERFACE ${SDL3PP_COMPILE_FEATURES})
target_include_directories(SDL3pp_amalgamated INTERFACE
    $<BUILD_INTERFACE:${SDL3PP_AMALGAMATION_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(SDL3pp_amalgamated INTERFACE ${SDL3PP_DEPENDENCIES})
set_target_properties(SDL3pp_amalgamated PROPERTIES EXCLUDE_FROM_ALL TRUE)

add_custom_command(OUTPUT ${SDL3PP_AMALGAMATION_FILENAME}
    COMMAND npx ts-node scripts/amalgamate --base-dir include/SDL3pp/ ${SDL3PP_AMALGAMATION_FILENAME} ${SDL3PP_AMALGAMATION_DEFS}
    DEPENDS ${SDL3PP_WRAPPED_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/amalgamate.ts
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_command(OUTPUT ${SDL3PP_AMALGAMATION_MAIN}
    COMMAND cp include/SDL3pp/SDL3pp_main.h ${SDL3PP_AMALGAMATION_MAIN}
    DEPENDS "include/SDL3pp/SDL3pp_main.h"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_custom_command(OUTPUT ${SDL3PP_AMALGAMATION_VULKAN}
    COMMAND cp include/SDL3pp/SDL3pp_vulkan.h ${SDL3PP_AMALGAMATION_VULKAN}
    DEPENDS "include/SDL3pp/SDL3pp_vulkan.h"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Examples
option(BUILD_EXAMPLES "Build the examples" ON)
option(SDL3PP_BUILD_EXAMPLES_AGAINST_AMALGAMATION "Build examples against amalgamation" OFF)
if (BUILD_EXAMPLES)
    if (SDL3PP_BUILD_EXAMPLES_AGAINST_AMALGAMATION)
        add_library(SDL3pp_ExampleBase ALIAS SDL3pp_amalgamated)
    else ()
        add_library(SDL3pp_ExampleBase ALIAS SDL3pp)
    endif()
    add_subdirectory(examples)
endif ()

# Unit tests
option(BUILD_TESTING "Build the tests" ON)
if (BUILD_TESTING)
    add_library(test_main OBJECT test/doctest.cpp)
    target_include_directories(test_main INTERFACE test/)
    enable_testing()

    file(GLOB unitTestSources CONFIGURE_DEPENDS test/SDL3pp_*.cpp)
    add_executable(SDL3pp_unitTests ${unitTestSources})
    target_link_libraries(SDL3pp_unitTests PRIVATE SDL3pp test_main)
    add_test(NAME SDL3pp_unitTests COMMAND SDL3pp_unitTests)
endif ()

# The documentation
# check if Doxygen is installed
find_package(Doxygen COMPONENTS dot)
if (DOXYGEN_FOUND)
    # Main doc
    doxygen_add_docs(SDL3pp_doxygen
        COMMENT "Generate API docs for SDL3pp"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        CONFIG_FILE ${PROJECT_SOURCE_DIR}/docs/Doxyfile
    )
endif (DOXYGEN_FOUND)

#########################
## Refresh the headers ##
#########################
file(GLOB CPPFIER_SOURCES CONFIGURE_DEPENDS scripts/cppfier/*.ts)

add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/node_modules 
    COMMAND npm install
)

set(CPPFIER_GRAMMAR_SOURCES 
    ${PROJECT_SOURCE_DIR}/scripts/cppfier/grammar/CHeaderLexer.ts
    ${PROJECT_SOURCE_DIR}/scripts/cppfier/grammar/CHeaderListener.ts
    ${PROJECT_SOURCE_DIR}/scripts/cppfier/grammar/CHeaderParser.ts
    ${PROJECT_SOURCE_DIR}/scripts/cppfier/grammar/DoxyCommentLexer.ts
    ${PROJECT_SOURCE_DIR}/scripts/cppfier/grammar/DoxyCommentListener.ts
    ${PROJECT_SOURCE_DIR}/scripts/cppfier/grammar/DoxyCommentParser.ts
)

add_custom_command(OUTPUT ${CPPFIER_GRAMMAR_SOURCES}
    COMMAND npm run antlr
    DEPENDS ${PROJECT_SOURCE_DIR}/node_modules 
            ${PROJECT_SOURCE_DIR}/scripts/cppfier/grammar/CHeader.g4
            ${PROJECT_SOURCE_DIR}/scripts/cppfier/grammar/DoxyComment.g4
)

# Source SDL and libs state
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/scripts/source.json
    COMMAND npm run check
    COMMAND ${PROJECT_SOURCE_DIR}/scripts/parse.sh
    DEPENDS ${SDL3PP_ORIGINAL_HEADERS} 
            ${PROJECT_SOURCE_DIR}/scripts/config-source.json 
            ${CPPFIER_SOURCES}
            ${CPPFIER_GRAMMAR_SOURCES}
            ${PROJECT_SOURCE_DIR}/node_modules 
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# Target SDL3pp state
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/scripts/target.json
    COMMAND ${PROJECT_SOURCE_DIR}/scripts/transform.sh
    DEPENDS ${PROJECT_SOURCE_DIR}/scripts/source.json 
            ${PROJECT_SOURCE_DIR}/scripts/config-target.json
            ${PROJECT_SOURCE_DIR}/scripts/gen-transform.js
            ${CPPFIER_SOURCES}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# Generate sources
add_custom_target(SDL3pp_generate
    COMMAND ${PROJECT_SOURCE_DIR}/scripts/generate.sh
    DEPENDS ${PROJECT_SOURCE_DIR}/scripts/config-target.json 
            ${PROJECT_SOURCE_DIR}/scripts/target.json
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# Apply patches
add_custom_target(SDL3pp_refresh
    COMMAND scripts/applyPatches.sh
    DEPENDS SDL3pp_generate
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# Apply patches
add_custom_target(SDL3pp_check_all
    COMMAND echo "Everything ran!"
    DEPENDS SDL3pp_refresh SDL3pp_amalgamated SDL3pp_doxygen SDL3pp
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

#########################
##      Install        ##
#########################

install(TARGETS SDL3pp
    EXPORT SDL3ppTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET sdl3pp_headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT SDL3ppTargets
    FILE SDL3ppTargets.cmake
    NAMESPACE SDL3pp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SDL3pp
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SDL3ppConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)
configure_file("${PROJECT_SOURCE_DIR}/cmake/SDL3ppConfig.cmake.in" SDL3ppConfig.cmake @ONLY)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SDL3ppConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SDL3ppConfig.cmake"
    DESTINATION lib/cmake/SDL3pp
)

############################
## Zip amalgamated files  ##
############################

# Create zip for amalgamated files
set(SDL3PP_AMALGAMATION_FILEBASE "${PROJECT_BINARY_DIR}/SDL3pp_amalgamated-${PROJECT_VERSION}")
add_custom_target(SDL3pp_zip_amalgamation
    COMMAND rm -f ${SDL3PP_AMALGAMATION_FILEBASE}.zip ${SDL3PP_AMALGAMATION_FILEBASE}.tar.gz
    COMMAND tar -czvf ${SDL3PP_AMALGAMATION_FILEBASE}.tar.gz SDL3pp/ ../LICENSE.txt ../README.md ../changelog.md
    COMMAND zip -r ${SDL3PP_AMALGAMATION_FILEBASE}.zip SDL3pp/
    COMMAND zip -ju ${SDL3PP_AMALGAMATION_FILEBASE}.zip ../LICENSE.txt ../README.md ../changelog.md
    DEPENDS SDL3pp_amalgamated
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/amalgamation/
)

set(SDL3PP_FULL_FILEBASE "${PROJECT_BINARY_DIR}/SDL3pp_full-${PROJECT_VERSION}")
set(SDL3PP_FULL_FILES 
    include/
    assets/
    cmake/
    external/
    docs/
    examples/
    test/
    CMakeLists.txt
    LICENSE.txt
    README.md
    changelog.md
)

add_custom_target(SDL3pp_zip_full
    COMMAND rm -f ${SDL3PP_FULL_FILEBASE}.zip ${SDL3PP_FULL_FILEBASE}.tar.gz
    COMMAND tar -czvf ${SDL3PP_FULL_FILEBASE}.tar.gz ${SDL3PP_FULL_FILES}
    COMMAND zip -r ${SDL3PP_FULL_FILEBASE}.zip ${SDL3PP_FULL_FILES}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
