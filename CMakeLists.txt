cmake_minimum_required(VERSION 3.18)

# set(CMAKE_MESSAGE_LOG_LEVEL "ERROR" CACHE STRING "CMake log level" FORCE)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/prod" CACHE FILEPATH "Default install dir" FORCE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/edge/cmake")

project(games
  LANGUAGES C CXX
  VERSION 1.0
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH $ORIGIN/lib)

include(ColorDiagnostics)
include(TargetAddExtraWarnings)

# compile Lua as C++ for better errors (proper stack unwinding)
# TODO: set to "ON" when my PR to lfs is merged
set(BUILD_LUA_AS_CPP FALSE CACHE BOOL "Build Lua as C++")
set(BUILD_SHARED_LIBS ON CACHE BOOL "Link to libs dynamically")

add_subdirectory(edge)

# Running luacheck gets hard when we use static linking...
if (BUILD_SHARED_LIBS)
  include(Luacheck)
endif()

add_subdirectory(breakout)
