add_executable(game
  src/main.cpp
)

target_link_libraries(game PRIVATE edge::edge)

target_add_extra_warnings(game)

### resources
set(GAME_BIN_DIR "$<TARGET_FILE_DIR:game>")
set(GAME_LUA_SOURCES_DIR "${CMAKE_CURRENT_LIST_DIR}/src/Lua")
set(GAME_RES_DIR "${CMAKE_CURRENT_LIST_DIR}/res")
# Symlink edge scripts
add_custom_command(TARGET game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink "${GAME_LUA_SOURCES_DIR}" "${GAME_BIN_DIR}/scripts"
  VERBATIM
)

add_custom_command(TARGET game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink "${GAME_RES_DIR}" "${GAME_BIN_DIR}/res"
  VERBATIM
)

include(Install.cmake)
