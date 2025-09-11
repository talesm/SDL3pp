
string(LENGTH ${PROJECT_SOURCE_DIR} examplesDirLength)
math(EXPR examplesDirLength "${examplesDirLength} + 10")

file(GLOB_RECURSE exampleSourcesLegacy CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/examples/legacy/*.cpp)

foreach(exampleSrc ${exampleSourcesLegacy})
    string(SUBSTRING ${exampleSrc} ${examplesDirLength} -1 exampleNameMatch)
    message(${exampleNameMatch})
    string(LENGTH ${exampleNameMatch} exampleNameLength)
    math(EXPR exampleNameLength "${exampleNameLength} - 4")
    string(SUBSTRING ${exampleNameMatch} 0 ${exampleNameLength} exampleName)
    string(REPLACE "/" "_" exampleName ${exampleName})

    add_executable(example_legacy_${exampleName} WIN32 ${exampleSrc})
    target_link_libraries(example_legacy_${exampleName} PRIVATE SDL3pp_legacy)
endforeach(exampleSrc ${exampleSourcesLegacy})
