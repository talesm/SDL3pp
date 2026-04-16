# Tests

# Test library
add_library(test_main OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/doctest.cpp)
target_include_directories(test_main INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)
target_link_libraries(test_main PUBLIC SDL3pp)
enable_testing()

# Unit tests
file(GLOB unitTestSources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/unit/SDL3pp_*.cpp)
add_executable(SDL3pp_unitTests ${unitTestSources})
target_link_libraries(SDL3pp_unitTests PRIVATE test_main)
add_test(NAME SDL3pp_unitTests COMMAND SDL3pp_unitTests)

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(SDL3pp_unitTests PRIVATE -Wall -Wextra -Wpedantic)    
endif(CMAKE_COMPILER_IS_GNUCXX)
