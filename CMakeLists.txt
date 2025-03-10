cmake_minimum_required(VERSION 3.27)
project(SDL3pp)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(external/SDL)

# The SDL3pp library
add_library(SDL3pp INTERFACE)
target_include_directories(SDL3pp INTERFACE include/)
target_link_libraries(SDL3pp INTERFACE SDL3::SDL3)

file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets COPY_ON_ERROR SYMBOLIC)

# The documentation
option(BUILD_DOC "Build documentation" OFF)

if (BUILD_DOC)
    # check if Doxygen is installed
    find_package(Doxygen REQUIRED dot)

    doxygen_add_docs(doxygen_docs
        ALL
        COMMENT "Generate API docs for SDL3pp"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        CONFIG_FILE ${PROJECT_SOURCE_DIR}/docs/Doxyfile
    )
endif (BUILD_DOC)

# Manual tests
add_executable(example_window_with_surface WIN32 examples/window_with_surface.cpp)
target_link_libraries(example_window_with_surface PRIVATE SDL3pp)
add_executable(example_window_with_renderer WIN32 examples/window_with_renderer.cpp)
target_link_libraries(example_window_with_renderer PRIVATE SDL3pp)
add_executable(example_environment_list WIN32 examples/environment_list.cpp)
target_link_libraries(example_environment_list PRIVATE SDL3pp)

# Unit tests
add_library(test_main OBJECT test/doctest.cpp)
enable_testing()

file(GLOB unitTestSources CONFIGURE_DEPENDS test/SDL3pp_*.cpp)
add_executable(SDL3pp_unitTests ${unitTestSources})
target_link_libraries(SDL3pp_unitTests PRIVATE SDL3pp test_main)
add_test(NAME SDL3pp_unitTests COMMAND SDL3pp_unitTests)
