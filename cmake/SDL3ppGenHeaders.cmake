include(ExternalProject)

ExternalProject_Add(SDL3Base
  PREFIX            ${CMAKE_CURRENT_BINARY_DIR}/base/
  URL               https://github.com/libsdl-org/SDL/releases/download/release-3.4.0/SDL3-3.4.0.tar.gz
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
)
ExternalProject_Add(SDL3ImageBase
  PREFIX            ${CMAKE_CURRENT_BINARY_DIR}/base/
  URL               https://github.com/libsdl-org/SDL_image/releases/download/release-3.4.0/SDL3_image-3.4.0.tar.gz
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
)
ExternalProject_Add(SDL3TTFBase
  PREFIX            ${CMAKE_CURRENT_BINARY_DIR}/base/
  URL               https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2.tar.gz
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
)

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
  DEPENDS SDL3Base
          SDL3ImageBase
          SDL3TTFBase
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
          ${PROJECT_SOURCE_DIR}/scripts/base.json
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

add_custom_command(OUTPUT ${SDL3PP_WRAPPED_HEADERS} 
  COMMAND ${CMAKE_COMMAND} --build . -t SDL3pp_refresh
  COMMAND touch ${SDL3PP_WRAPPED_HEADERS}
  DEPENDS ${PROJECT_SOURCE_DIR}/scripts/target.json
)

# Apply patches
add_custom_target(SDL3pp_check_all
  COMMAND echo "Everything ran!"
  DEPENDS SDL3pp_amalgamated SDL3pp_doxygen SDL3pp
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
