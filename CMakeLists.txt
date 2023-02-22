cmake_minimum_required(VERSION 3.18)

# set(CMAKE_MESSAGE_LOG_LEVEL "ERROR" CACHE STRING "CMake log level" FORCE)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/prod" CACHE FILEPATH "Default install dir" FORCE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/edge/cmake")

if(WIN32)
 set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

include(ColorDiagnostics)
include(TargetAddExtraWarnings)
include(InstallEdgeDeps)
include(SymlinkCommonDirs)
include(GameInstallCommon)

project(games
  LANGUAGES C CXX
  VERSION 1.0
)

# Check that git submodules were cloned
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/edge/third_party/SDL/CMakeLists.txt")
  message("${CMAKE_CURRENT_SOURCE_DIR}/third_party/SDL/CMakeLists.txt")
  message(FATAL_ERROR "Please initialize submodules using:\n  git submodule update --init --recursive")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# compile Lua as C++ for better errors (proper stack unwinding)
# TODO: set to "ON" when my PR to lfs is merged
set(BUILD_LUA_AS_CPP FALSE CACHE BOOL "Build Lua as C++")
set(BUILD_SHARED_LIBS ON CACHE BOOL "Link to libs dynamically")

add_subdirectory(edge)
add_subdirectory(games)
