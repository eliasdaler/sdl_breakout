function(symlink_common_dirs target)
  ### resources
  set(GAME_BIN_DIR "$<TARGET_FILE_DIR:${target}>")
  set(GAME_LUA_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src/scripts")

  # Symlink edge scripts
  get_target_property(EDGE_DIR edge SOURCE_DIR)
  set(EDGE_LUA_SOURCES_DIR "${EDGE_DIR}/src/Lua")
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${EDGE_LUA_SOURCES_DIR}" "${GAME_BIN_DIR}/edge"
    VERBATIM
  )

  # Symlink game scripts
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${GAME_LUA_SOURCES_DIR}" "${GAME_BIN_DIR}/scripts"
    VERBATIM
  )

  # Symlink resources
  set(GAME_RES_DIR "${PROJECT_SOURCE_DIR}/res")
  add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${GAME_RES_DIR}" "${GAME_BIN_DIR}/res"
    VERBATIM
  )
endfunction()
