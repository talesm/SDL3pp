cmake_minimum_required(VERSION 3.27)
project(SDL3pp 
    VERSION 0.5.8
    DESCRIPTION "A slim C++ wrapper for SDL3" 
    HOMEPAGE_URL "https://github.com/talesm/SDL3pp"
)

include(GNUInstallDirs)

#############################
### Enable/Disable options ##
#############################
option(SDL3PP_FORCE_BUNDLED "Force use of bundled SDL libraries" OFF)
option(SDL3PP_ENABLE_IMAGE "Enable compilation of SDL_Image" ON)
option(SDL3PP_ENABLE_TTF "Enable compilation of SDL_TTF" ON)

if (SDL3PP_FORCE_BUNDLED)
    include(cmake/SDL3ppAddExternalProjects.cmake)
    set(SDL3PP_DEPENDENCIES SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)

else ()
    find_package(SDL3 CONFIG REQUIRED)
    set(SDL3PP_DEPENDENCIES SDL3::SDL3)

    # SDL_image
    if (SDL3PP_ENABLE_IMAGE)
        find_package(SDL3_image QUIET)
        set(SDL3_IMAGE_DEPENDENCY "find_dependency(SDL3_image QUIET)") # placeholder in cmake.in
        if (TARGET SDL3_image::SDL3_image)
            set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_image::SDL3_image)
        endif ()
    endif ()
    
    # SDL_ttf
    if (SDL3PP_ENABLE_TTF)
        find_package(SDL3_ttf QUIET)
        set(SDL3_TTF_DEPENDENCY "find_dependency(SDL3_ttf QUIET)") # placeholder in cmake.in
        if (TARGET SDL3_ttf::SDL3_ttf)
            set(SDL3PP_DEPENDENCIES ${SDL3PP_DEPENDENCIES} SDL3_ttf::SDL3_ttf)
        endif ()
    endif ()
endif ()


###############
# The library #
###############

file(GLOB_RECURSE SDL3pp_lib_HEADERS "${PROJECT_SOURCE_DIR}/include/*.h")

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets COPY_ON_ERROR SYMBOLIC)
endif ()

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
set(SDL3PP_AMALGAMATION_DIR "${PROJECT_SOURCE_DIR}/amalgamation")
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
option(SDL3PP_BUILD_EXAMPLES "Build the examples" OFF)
option(SDL3PP_BUILD_EXAMPLES_AGAINST_AMALGAMATION "Build examples against amalgamation" OFF)
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples AND SDL3PP_BUILD_EXAMPLES)
    if (SDL3PP_BUILD_EXAMPLES_AGAINST_AMALGAMATION)
        add_library(SDL3pp_ExampleBase ALIAS SDL3pp_amalgamated)
    else ()
        add_library(SDL3pp_ExampleBase ALIAS SDL3pp)
    endif()
    add_subdirectory(examples)
endif ()

# Unit tests
option(SDL3PP_BUILD_TESTING "Build the tests" OFF)
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test AND SDL3PP_BUILD_TESTING)
    add_library(test_main OBJECT test/doctest.cpp)
    target_include_directories(test_main INTERFACE test/)
    enable_testing()

    file(GLOB unitTestSources CONFIGURE_DEPENDS test/SDL3pp_*.cpp)
    add_executable(SDL3pp_unitTests ${unitTestSources})
    target_link_libraries(SDL3pp_unitTests PRIVATE SDL3pp test_main)
    add_test(NAME SDL3pp_unitTests COMMAND SDL3pp_unitTests)
endif ()

# The documentation
option(SDL3PP_GEN_DOCS "Generated docs" OFF)

if (SDL3PP_GEN_DOCS AND EXISTS ${PROJECT_SOURCE_DIR}/docs/Doxyfile)
    # check if Doxygen is installed
    find_package(Doxygen COMPONENTS dot)
    if (DOXYGEN_FOUND)
        # Main doc
        doxygen_add_docs(SDL3pp_doxygen
            COMMENT "Generate API docs for SDL3pp"
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            CONFIG_FILE ${PROJECT_SOURCE_DIR}/docs/Doxyfile
        )
    endif ()
endif ()

#########################
## Refresh the headers ##
#########################
option(SDL3PP_GEN_HEADERS "Generate headers" OFF)

if (SDL3PP_GEN_HEADERS)
    include(cmake/SDL3ppGenHeaders.cmake)
endif (SDL3PP_GEN_HEADERS)

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

set(SDL3PP_HEADERS_ONLY_FILEBASE "${PROJECT_BINARY_DIR}/SDL3pp-${PROJECT_VERSION}")
add_custom_target(SDL3pp_zip_headers
    COMMAND rm -f ${SDL3PP_HEADERS_ONLY_FILEBASE}.zip ${SDL3PP_HEADERS_ONLY_FILEBASE}.tar.gz
    COMMAND tar -czvf ${SDL3PP_HEADERS_ONLY_FILEBASE}.tar.gz include/ cmake/ LICENSE.txt README.md changelog.md CMakeLists.txt
    COMMAND zip -r ${SDL3PP_HEADERS_ONLY_FILEBASE}.zip include/ cmake/ LICENSE.txt README.md changelog.md CMakeLists.txt
    DEPENDS ${SDL3PP_WRAPPED_HEADERS}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/
)
