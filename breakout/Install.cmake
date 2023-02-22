# Will be installed in CMAKE_INSTALL_PREFIX/game
set(GAME_INSTALL_DIR "game")
set(CMAKE_INSTALL_RPATH $ORIGIN $ORIGIN/lib)

install(TARGETS game
  RUNTIME DESTINATION ${GAME_INSTALL_DIR}
  COMPONENT game
)

install(DIRECTORY "${GAME_LUA_SOURCES_DIR}/" # last slash is needed to rename the folder
  DESTINATION "${GAME_INSTALL_DIR}/scripts"
  COMPONENT game
)

install(DIRECTORY "${GAME_RES_DIR}"
  DESTINATION "${GAME_INSTALL_DIR}"
  COMPONENT game
)

# copy SDL dlls
if (WIN32)
  # TODO
  # 3.21 - use this https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#genex:TARGET_RUNTIME_DLLS
endif()

