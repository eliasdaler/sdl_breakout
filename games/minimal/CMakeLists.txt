project(minimal
  LANGUAGES CXX
  VERSION 1.0
)

add_executable(minimal
  src/main.cpp
)

if (BUILD_SHARED_LIBS)
  set_target_properties(minimal PROPERTIES
    INSTALL_RPATH $ORIGIN/lib
  )
endif()

if(WIN32)
	target_link_libraries(breakout PUBLIC SDL2::SDL2main)
endif()

target_link_libraries(minimal PRIVATE edge::edge)

target_add_extra_warnings(minimal)
symlink_common_dirs(minimal)
install_game(minimal)
