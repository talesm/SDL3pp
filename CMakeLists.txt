cmake_minimum_required(VERSION 3.27)
project(SDL3pp)

add_subdirectory(external/Heady)
add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)

set(CMAKE_CXX_STANDARD 23)


file(GLOB SDL3PP_ORIGINAL_HEADERS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/external/SDL/include/SDL3/*.h)

# Enable/Disable options
option(SDL3PP_DISABLE_IMAGE "Disable compilation SDL_Image" OFF)
if(SDL3PP_DISABLE_IMAGE)
    set(SDL3PP_AMALGAMATION_DEFS "${SDL3PP_AMALGAMATION_DEFS} -d SDL3PP_DISABLE_IMAGE")
else((SDL3PP_DISABLE_IMAGE))
    set(SDL3PP_DEPENDENCIES SDL3::SDL3 SDL3_image::SDL3_image)
    file(GLOB SDL3PP_IMAGE_ORIGINAL_HEADERS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/external/SDL_image/include/SDL3_image/*.h)
    set(SDL3PP_ORIGINAL_HEADERS ${SDL3PP_ORIGINAL_HEADERS} ${SDL3PP_IMAGE_ORIGINAL_HEADERS})
endif(SDL3PP_DISABLE_IMAGE)

option(SDL3PP_DISABLE_STRING_PARAM "Disable StringParam wrapping" OFF)
if(SDL3PP_DISABLE_STRING_PARAM)
    set(SDL3PP_AMALGAMATION_DEFS "${SDL3PP_AMALGAMATION_DEFS} -d SDL3PP_DISABLE_STRING_PARAM")
endif(SDL3PP_DISABLE_STRING_PARAM)


# The SDL3pp library
add_library(SDL3pp_lib INTERFACE)
target_include_directories(SDL3pp_lib INTERFACE include/)
target_link_libraries(SDL3pp_lib INTERFACE ${SDL3PP_DEPENDENCIES})


file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets COPY_ON_ERROR SYMBOLIC)

# The amalgamation header
set(SDL3PP_AMALGAMATION_DIR "${PROJECT_SOURCE_DIR}/dist" CACHE FILEPATH "Amalgamation output directory")
set(SDL3PP_AMALGAMATION_FILENAME "${SDL3PP_AMALGAMATION_DIR}SDL3pp/SDL3pp.h")
file(GLOB SDL3PP_WRAPPED_HEADERS CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/include/SDL3pp/*.h)
message("${SDL3PP_AMALGAMATION_FILENAME}")

add_library(SDL3pp_amalgamated INTERFACE ${SDL3PP_AMALGAMATION_FILENAME})
target_include_directories(SDL3pp_amalgamated INTERFACE ${SDL3PP_AMALGAMATION_DIR})
target_link_libraries(SDL3pp_amalgamated INTERFACE ${SDL3PP_DEPENDENCIES})

option(SDL3PP_BUILD_EXAMPLES_AGAINST_AMALGAMATION "Build examples against amalgamation" ON)

if (SDL3PP_BUILD_EXAMPLES_AGAINST_AMALGAMATION)
    add_library(SDL3pp ALIAS SDL3pp_amalgamated)
else (SDL3PP_BUILD_EXAMPLES_AGAINST_AMALGAMATION)
    add_library(SDL3pp ALIAS SDL3pp_lib)
endif(SDL3PP_BUILD_EXAMPLES_AGAINST_AMALGAMATION)

add_custom_command(OUTPUT ${SDL3PP_AMALGAMATION_FILENAME}
    COMMAND Heady --source ${PROJECT_SOURCE_DIR}/include/SDL3pp/ --output ${SDL3PP_AMALGAMATION_FILENAME} ${SDL3PP_AMALGAMATION_DEFS}
    DEPENDS ${SDL3PP_WRAPPED_HEADERS} Heady
)

# Manual tests
add_executable(example_environment_list WIN32 examples/environment_list.cpp)
target_link_libraries(example_environment_list PRIVATE SDL3pp)
add_executable(example_locale_list WIN32 examples/locale_list.cpp)
target_link_libraries(example_locale_list PRIVATE SDL3pp)
add_executable(example_open_url WIN32 examples/open_url.cpp)
target_link_libraries(example_open_url PRIVATE SDL3pp)
add_executable(example_window_with_renderer WIN32 examples/window_with_renderer.cpp)
target_link_libraries(example_window_with_renderer PRIVATE SDL3pp)
add_executable(example_window_with_surface WIN32 examples/window_with_surface.cpp)
target_link_libraries(example_window_with_surface PRIVATE SDL3pp)

# Unit tests
add_library(test_main OBJECT test/doctest.cpp)
enable_testing()

file(GLOB unitTestSources CONFIGURE_DEPENDS test/SDL3pp_*.cpp)
add_executable(SDL3pp_unitTests ${unitTestSources})
target_link_libraries(SDL3pp_unitTests PRIVATE SDL3pp_lib test_main)
add_test(NAME SDL3pp_unitTests COMMAND SDL3pp_unitTests)

# The documentation
# check if Doxygen is installed
find_package(Doxygen REQUIRED dot)
if (DOXYGEN_FOUND)
    doxygen_add_docs(SDL3pp_doxygen
        COMMENT "Generate API docs for SDL3pp"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        CONFIG_FILE ${PROJECT_SOURCE_DIR}/docs/Doxyfile
    )
endif (DOXYGEN_FOUND)

# Refresh the headers
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/scripts/source.json
    COMMAND node scripts/ parse -d ./external/SDL/include/SDL3/ -d ./external/SDL_image/include/SDL3_image/ scripts/config.json 
    DEPENDS ${SDL3PP_ORIGINAL_HEADERS}
)
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/scripts/target.json
    COMMAND node scripts/ transform scripts/config.json
    DEPENDS ${PROJECT_SOURCE_DIR}/scripts/source.json  ${PROJECT_SOURCE_DIR}/scripts/transform.json
)

add_custom_target(SDL3pp_refresh
    COMMAND node scripts/ update scripts/config.json
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    DEPENDS ${PROJECT_SOURCE_DIR}/scripts/target.json
)
