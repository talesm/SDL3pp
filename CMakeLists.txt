cmake_minimum_required(VERSION 3.5)
project(SDL3pp)

set(CMAKE_CXX_STANDARD 23)

# Create an option to switch between a system sdl library and a vendored SDL library
option(SDL3PP_VENDORED "Use vendored libraries" OFF)

if(SDL3PP_VENDORED)
    # This assumes you have added SDL as a submodule in vendored/SDL
    message("Not implemented")
    add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
else()
    # 1. Look for a SDL3 package,
    # 2. look for the SDL3-shared component, and
    # 3. fail if the shared component cannot be found.
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

# The SDL3pp library
add_library(SDL3pp INTERFACE)
target_include_directories(SDL3pp INTERFACE include/)
target_link_libraries(SDL3pp INTERFACE SDL3::SDL3)

# Manual tests
add_executable(test_window WIN32 test/window.cpp)
target_link_libraries(test_window PRIVATE SDL3pp)
add_executable(test_renderer WIN32 test/renderer.cpp)
target_link_libraries(test_renderer PRIVATE SDL3pp)

# Unit tests
add_library(test_main OBJECT test/doctest.cpp)
enable_testing()

add_executable(SDL3pp_unitTests 
    test/callbackWrapper.cpp
    test/freeWrapper.cpp
    test/objectWrapper.cpp
    test/stringParam.cpp
)
target_link_libraries(SDL3pp_unitTests PRIVATE SDL3pp test_main)
add_test(NAME SDL3pp_unitTests COMMAND SDL3pp_unitTests)
