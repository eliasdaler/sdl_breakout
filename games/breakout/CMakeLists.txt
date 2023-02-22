project(breakout
  LANGUAGES CXX
  VERSION 1.0
)

add_executable(breakout
  src/main.cpp
)

if (BUILD_SHARED_LIBS)
  set_target_properties(breakout PROPERTIES
    INSTALL_RPATH $ORIGIN/lib
  )
endif()

if(WIN32)
	target_link_libraries(breakout PUBLIC SDL2::SDL2main)
endif()

target_link_libraries(breakout PRIVATE edge::edge)

target_add_extra_warnings(breakout)
symlink_common_dirs(breakout)
install_game(breakout)
