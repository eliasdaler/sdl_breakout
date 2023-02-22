add_subdirectory(third_party)

add_library(edge
  src/Game.cpp
  src/LuaBindings.cpp
  src/LuaImGui.cpp
  src/Globals.cpp
)

add_library(edge::edge ALIAS edge)

target_include_directories(edge PUBLIC include)

target_link_libraries(edge PUBLIC
  lua::lua
  sol2::sol2
  imgui::imgui
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

include(Install.cmake)
