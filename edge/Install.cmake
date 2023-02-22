# Will be installed in CMAKE_INSTALL_PREFIX/game
set(edge_INSTALL_DIR "game") # TODO: don't do this

install(TARGETS edge
  LIBRARY DESTINATION "${edge_INSTALL_DIR}/lib"
  COMPONENT edge
)

if (BUILD_SHARED_LIBS)
  install(TARGETS lua
    LIBRARY DESTINATION "${edge_INSTALL_DIR}/lib"
  )

  install(TARGETS SDL2
    LIBRARY DESTINATION "${edge_INSTALL_DIR}/lib"
  )

  install(TARGETS SDL2_image
    LIBRARY DESTINATION "${edge_INSTALL_DIR}/lib"
  )

  install(TARGETS SDL2_ttf
    LIBRARY DESTINATION "${edge_INSTALL_DIR}/lib"
  )

  install(TARGETS SDL2_mixer
    LIBRARY DESTINATION "${edge_INSTALL_DIR}/lib"
  )
endif()

# copy SDL dlls
if (WIN32)
  # TODO
  # 3.21 - use this https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#genex:TARGET_RUNTIME_DLLS
endif()

