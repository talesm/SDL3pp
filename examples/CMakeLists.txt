
string(LENGTH ${PROJECT_SOURCE_DIR} examplesDirLength)
math(EXPR examplesDirLength "${examplesDirLength} + 10")

file(GLOB_RECURSE exampleSources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/examples/*.cpp)

foreach(exampleSrc ${exampleSources})
    string(SUBSTRING ${exampleSrc} ${examplesDirLength} -1 exampleNameMatch)
    message(${exampleNameMatch})
    string(LENGTH ${exampleNameMatch} exampleNameLength)
    math(EXPR exampleNameLength "${exampleNameLength} - 4")
    string(SUBSTRING ${exampleNameMatch} 0 ${exampleNameLength} exampleName)
    string(REPLACE "/" "_" exampleName ${exampleName})

    add_executable(example_${exampleName} WIN32 ${exampleSrc})
    target_link_libraries(example_${exampleName} PRIVATE SDL3pp)
endforeach(exampleSrc ${exampleSources})
