### lua
set(LUA_SOURCES lapi.c lauxlib.c lbaselib.c lcode.c lcorolib.c
		lctype.c ldblib.c ldebug.c ldo.c ldump.c lfunc.c lgc.c linit.c liolib.c
		llex.c lmathlib.c lmem.c loadlib.c lobject.c lopcodes.c loslib.c
		lparser.c lstate.c lstring.c lstrlib.c ltable.c ltablib.c ltm.c lundump.c
		lutf8lib.c lvm.c lzio.c)

list(TRANSFORM LUA_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/lua/")

add_library(lua ${LUA_SOURCES})
add_library(lua::lua ALIAS lua)

if (${BUILD_LUA_AS_CPP})
  target_compile_definitions(lua PUBLIC SOL_USE_CXX_LUA)
  set_source_files_properties(${LUA_SOURCES} PROPERTIES
    LANGUAGE CXX
    LINKER_LANGUAGE CXX
  )
endif()

if (WIN32)
  target_compile_definitions(lua PRIVATE LUA_USE_WINDOWS)
endif()

target_include_directories(lua PUBLIC lua)
if (UNIX)
  target_compile_definitions(lua PUBLIC LUA_USE_LINUX)
  target_link_libraries(lua PUBLIC dl m)
endif()

### sol2
set(SOL2_LUA_VERSION "5.4.4" CACHE STRING "The version of Lua used")
# we build Lua manually
set(SOL2_BUILD_LUA FALSE CACHE BOOL "Always build Lua, do not search for it in the system")
option(SOL2_ENABLE_INSTALL "Enable installation of Sol2" OFF)
add_subdirectory(sol2)

### SDL
if (NOT BUILD_SHARED_LIBS)
  set(SDL_SHARED_ENABLED_BY_DEFAULT OFF CACHE BOOL "Don't build SDL as shared lib")
endif()
option(SDL_TEST "Build the SDL2_test library" OFF)
add_subdirectory(SDL)

### SDL_image
# PNG = GOOD
option(SDL2IMAGE_PNG "Support loading PNG images" ON)

# don't need the rest, lol
option(SDL2IMAGE_SAMPLES "Build the SDL2_image sample program(s)" OFF)
option(SDL2IMAGE_INSTALL "Enable SDL2_image install target" OFF)
option(SDL2IMAGE_DEPS_SHARED "Load dependencies dynamically" OFF)
option(SDL2IMAGE_SAMPLES "Build the SDL2_image sample program(s)" OFF)
option(SDL2IMAGE_BMP "Support loading BMP images" OFF)
option(SDL2IMAGE_GIF "Support loading GIF images" OFF)
option(SDL2IMAGE_JPG "Support loading JPEG images" OFF)
option(SDL2IMAGE_LBM "Support loading LBM images" OFF)
option(SDL2IMAGE_PCX "Support loading PCX images" OFF)
option(SDL2IMAGE_PNM "Support loading PNM images" OFF)
option(SDL2IMAGE_QOI "Support loading QOI images" OFF)
option(SDL2IMAGE_SVG "Support loading SVG images" OFF)
option(SDL2IMAGE_TGA "Support loading TGA images" OFF)
option(SDL2IMAGE_XCF "Support loading XCF images" OFF)
option(SDL2IMAGE_XPM "Support loading XPM images" OFF)
option(SDL2IMAGE_XV "Support loading XV images" OFF)

add_subdirectory(SDL_image)
# To quiet down "target X has PUBLIC_HEADER files but no PUBLIC_HEADER DESTINATION"
set_target_properties(SDL2_image PROPERTIES
  PUBLIC_HEADER ""
)

### SDL_ttf
option(SDL2TTF_SAMPLES "Build the SDL2_ttf sample program(s)" OFF)
option(SDL2TTF_INSTALL "Enable SDL2_ttf install target" OFF)
option(SDL2TTF_VENDORED "Use vendored third-party libraries" ON)
add_subdirectory(SDL_ttf)
# To quiet down "target X has PUBLIC_HEADER files but no PUBLIC_HEADER DESTINATION"
set_target_properties(SDL2_ttf PROPERTIES
  PUBLIC_HEADER ""
)

### SDL_Mixer
option(SDL2MIXER_INSTALL "Enable SDL2mixer install target" OFF)
option(SDL2MIXER_DEPS_SHARED "Default value for loading dependencies dynamically" OFF)
option(SDL2MIXER_SAMPLES "Build the SDL2_mixer sample program(s)" OFF)
option(SDL2MIXER_CMD "Support an external music player" OFF)
option(SDL2MIXER_FLAC "Enable FLAC music" OFF)
option(SDL2MIXER_MOD "Support loading MOD music" OFF)
option(SDL2MIXER_MP3 "Enable MP3 music" OFF)
option(SDL2MIXER_MIDI "Enable MIDI music" OFF)
option(SDL2MIXER_OPUS "Enable Opus music" OFF)

add_subdirectory(SDL_mixer)
# To quiet down "target X has PUBLIC_HEADER files but no PUBLIC_HEADER DESTINATION"
set_target_properties(SDL2_mixer PROPERTIES
  PUBLIC_HEADER ""
)

### Dear ImGui
set(IMGUI_SOURCES
  "imgui.cpp"
  "imgui_draw.cpp"
  "imgui_tables.cpp"
  "imgui_widgets.cpp"
  "imgui_demo.cpp"
  # C++ additions
  "misc/cpp/imgui_stdlib.cpp"
  # SDL backend
  "backends/imgui_impl_sdlrenderer.cpp"
  "backends/imgui_impl_sdl2.cpp"
)

list(TRANSFORM IMGUI_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/imgui/")

add_library(imgui_sdl STATIC ${IMGUI_SOURCES})

target_include_directories(imgui_sdl
  PUBLIC
    "imgui"
    "imgui/backends"
)

if(BUILD_SHARED_LIBS)
  target_link_libraries(imgui_sdl
    PUBLIC
      SDL2::SDL2
  )
else()
  target_link_libraries(imgui_sdl
    PUBLIC
      SDL2::SDL2-static
  )
endif()
add_library(imgui::imgui ALIAS imgui_sdl)
