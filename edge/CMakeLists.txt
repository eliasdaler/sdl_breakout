add_subdirectory(third_party)

add_library(edge
  src/Game.cpp
  src/LuaBindings.cpp
  src/Globals.cpp
)

if (BUILD_SHARED_LIBS)
  set_target_properties(edge PROPERTIES
    INSTALL_RPATH $ORIGIN
  )
endif()

add_library(edge::edge ALIAS edge)

target_include_directories(edge PUBLIC include)

target_link_libraries(edge PUBLIC
  lua::lua
  sol2::sol2
)

target_compile_definitions(edge PUBLIC SOL_ALL_SAFETIES_ON)

if(BUILD_SHARED_LIBS)
  target_link_libraries(edge PUBLIC
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
  )
else()
  target_link_libraries(edge PUBLIC
    SDL2::SDL2-static
    SDL2_image::SDL2_image-static
    SDL2_ttf::SDL2_ttf-static
    SDL2_mixer::SDL2_mixer-static
  )
endif()

target_add_extra_warnings(edge)
# strip debug symbols
# set_target_properties(edge PROPERTIES LINK_FLAGS_RELEASE -s)
