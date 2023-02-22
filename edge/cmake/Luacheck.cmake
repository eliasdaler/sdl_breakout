set(LUACHECK_DIR "${CMAKE_CURRENT_LIST_DIR}/../third_party/luacheck")
## lua bin (for luacheck)
## TODO: support static linkage
set(LUA_BINARY_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../third_party/lua/lua.c")
add_executable(luabin ${LUA_BINARY_SOURCES})
if (${BUILD_LUA_AS_CPP})
  set_source_files_properties(${LUA_BINARY_SOURCES} PROPERTIES
    LANGUAGE CXX
    LINKER_LANGUAGE CXX
  )
endif()
target_link_libraries(luabin PUBLIC lua::lua)

## lfs
message("${CMAKE_CURRENT_LIST_DIR}/../third_party/luafilesystem/src/lfs.c")
set(LFS_SOURCES "${CMAKE_CURRENT_LIST_DIR}/../third_party/luafilesystem/src/lfs.c")
add_library(lfs ${LFS_SOURCES})

set_target_properties (lfs PROPERTIES
  PREFIX ""
  LIBRARY_OUTPUT_DIRECTORY "lua/luafilesystem"
)
if (${BUILD_LUA_AS_CPP})
  set_source_files_properties(${LFS_SOURCES} PROPERTIES
    LANGUAGE CXX
    LINKER_LANGUAGE CXX
  )
endif()

target_link_libraries(lfs PRIVATE lua::lua)
add_library(lfs::lfs ALIAS lfs)

############

set(LUACHECK_BIN "${LUACHECK_DIR}/bin/luacheck.lua")

set(LUACHECK_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/.luacheckrc")

file(WRITE ${LUACHECK_CONFIG} "ignore = {\n")
# file(APPEND ${LUACHECK_CONFIG} "\"212/self\", \"212/e\", \"212/dt\", \"212/args\", \"212/event\", \"212/.*_\",\n")
file(APPEND ${LUACHECK_CONFIG} "  \"212\",\n") # unused argument - in progress
file(APPEND ${LUACHECK_CONFIG} "  \"432\", \"631\" }\n") # 432 - shadowing upvalues (common), 631 - line to long

file(APPEND ${LUACHECK_CONFIG} "quiet = 1\n") # only show errors
file(APPEND ${LUACHECK_CONFIG} "stds.game = { read_globals = {\n")

set(LUA_GLOBALS
  # allowed globals
  "edge"
  "im"
)

set(LUA_GLOBAL_WRITE_OK
  "package" # setting package path
)

foreach(LUA_GLOBAL_NAME ${LUA_GLOBALS})
  file(APPEND ${LUACHECK_CONFIG} "  \"${LUA_GLOBAL_NAME}\",\n")
endforeach()

file(APPEND ${LUACHECK_CONFIG} "},\n")

file(APPEND ${LUACHECK_CONFIG} "globals = {\n")

foreach(LUA_GLOBAL_NAME ${LUA_GLOBAL_WRITE_OK})
  file(APPEND ${LUACHECK_CONFIG} "  \"${LUA_GLOBAL_NAME}\",\n")
endforeach()

file(APPEND ${LUACHECK_CONFIG} "}}\n")
file(APPEND ${LUACHECK_CONFIG} "std='lua54+game'\n")

set(LUACHECK_EXCLUDED_FILES
  "temp"
)

add_custom_target(luacheck
  COMMAND ${CMAKE_COMMAND} -E echo "Running luacheck..."
  COMMAND
  $<TARGET_FILE:luabin> -e 'package.path="${LUACHECK_DIR}/src/?.lua$<SEMICOLON>${LUACHECK_DIR}/src/?/init.lua$<SEMICOLON>${PROJECT_SOURCE_DIR}/src/Lua/lib/?.lua$<SEMICOLON>" .. package.path$<SEMICOLON> package.cpath="$<TARGET_FILE:lfs>"'
  ${LUACHECK_BIN} --config ${LUACHECK_CONFIG} "${PROJECT_SOURCE_DIR}/src" --exclude-files "${LUACHECK_EXCLUDED_FILES}"
)

add_dependencies(luacheck lfs::lfs)

