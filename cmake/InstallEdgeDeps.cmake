function(install_edge_deps dir)
  if (BUILD_SHARED_LIBS)
    install(TARGETS edge
      LIBRARY DESTINATION "${dir}/lib"
    )

    install(TARGETS lua
      LIBRARY DESTINATION "${dir}/lib"
    )

    install(TARGETS SDL2
      LIBRARY DESTINATION "${dir}/lib"
    )

    install(TARGETS SDL2_image
      LIBRARY DESTINATION "${dir}/lib"
    )

    install(TARGETS SDL2_ttf
      LIBRARY DESTINATION "${dir}/lib"
    )

    install(TARGETS SDL2_mixer
      LIBRARY DESTINATION "${dir}/lib"
    )
  endif()

  get_target_property(EDGE_DIR edge SOURCE_DIR)
  set(EDGE_LUA_SOURCES_DIR "${EDGE_DIR}/src/Lua")
  install(DIRECTORY "${EDGE_LUA_SOURCES_DIR}/" # last slash is needed to rename the folder
    DESTINATION "${dir}/edge"
  )
endfunction()
