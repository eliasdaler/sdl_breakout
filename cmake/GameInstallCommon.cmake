function(install_game target)
  # Will be installed in CMAKE_INSTALL_PREFIX/${target}
  set(GAME_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${target}")
  set(GAME_LUA_SOURCES_DIR "${PROJECT_SOURCE_DIR}/src/scripts")
  set(GAME_RES_DIR "${PROJECT_SOURCE_DIR}/res")

  install(TARGETS ${target}
    RUNTIME DESTINATION ${GAME_INSTALL_DIR}
  )

  install(DIRECTORY "${GAME_LUA_SOURCES_DIR}/" # last slash is needed to rename the folder
    DESTINATION "${GAME_INSTALL_DIR}/scripts"
  )

  install(DIRECTORY "${GAME_RES_DIR}"
    DESTINATION "${GAME_INSTALL_DIR}"
  )

  install_edge_deps(${GAME_INSTALL_DIR})
endfunction()
