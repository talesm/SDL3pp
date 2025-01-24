cmake_minimum_required(VERSION 3.5)
project(SDL3pp)

set(CMAKE_CXX_STANDARD 20)

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

# Create your game executable target as usual
add_executable(test_window WIN32 test/window.cpp)
target_link_libraries(test_window PRIVATE SDL3pp)

# Unit tests
add_library(test_main OBJECT test/doctest.cpp)

add_executable(test_callbackWrapper test/callbackWrapper.cpp)
target_link_libraries(test_callbackWrapper PRIVATE SDL3pp test_main)

add_executable(test_objectWrapper test/objectWrapper.cpp)
target_link_libraries(test_objectWrapper PRIVATE SDL3pp test_main)

add_executable(test_stringWrapper test/stringWrapper.cpp)
target_link_libraries(test_stringWrapper PRIVATE SDL3pp test_main)

