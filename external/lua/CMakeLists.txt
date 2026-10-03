# -----------
# WARNING
#
# this CMakeLists IS NOT part of the official lua project
# it was added to be practical to compile for people who
# do not have the Makefile or is not using it to compile
# if you want you could take and modify for your purpose
# -----------

cmake_minimum_required(VERSION 3.15)
project(lua-lang)

# lua is compiled in C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

set(LUA_CORE
  src/lapi.c
  src/lcode.c
  src/lctype.c
  src/ldebug.c
  src/ldo.c
  src/ldump.c
  src/lfunc.c
  src/lgc.c
  src/llex.c
  src/lmem.c
  src/lobject.c
  src/lopcodes.c
  src/lparser.c
  src/lstate.c
  src/lstring.c
  src/ltable.c
  src/ltm.c
  src/lundump.c
  src/lvm.c
  src/lzio.c
)

set(LUA_LIB
  src/lauxlib.c
  src/lbaselib.c
  src/lcorolib.c
  src/ldblib.c
  src/linit.c
  src/liolib.c
  src/lmathlib.c
  src/loadlib.c
  src/loslib.c
  src/lstrlib.c
  src/ltablib.c
  src/lutf8lib.c
)

set(LUA_BASE ${LUA_CORE} ${LUA_LIB})

add_library(lua_obj OBJECT ${LUA_BASE})
target_include_directories(lua_obj PUBLIC src)
target_compile_options(lua_obj PRIVATE -O2 -Wall -Wextra)
# target_compile_definitions(lua_obj PUBLIC LUA_COMPAT_5_3)

if(UNIX)
  target_compile_definitions(lua_obj PUBLIC LUA_USE_POSIX)
endif()

# static library
add_library(lua STATIC $<TARGET_OBJECTS:lua_obj>)
target_include_directories(lua PUBLIC src)

# dynamic library
add_library(lua54 SHARED $<TARGET_OBJECTS:lua_obj>)
target_include_directories(lua54 PUBLIC src)
set_target_properties(lua54 PROPERTIES POSITION_INDEPENDENT_CODE ON)

# unix systems specifications
if(UNIX)
  target_link_libraries(lua PUBLIC m dl)
  target_link_libraries(lua54 PUBLIC m dl)
endif()

# interpreter
add_executable(lua_rt src/lua.c)
target_link_libraries(lua_rt PRIVATE lua)
set_target_properties(lua_rt PROPERTIES OUTPUT_NAME lua)
target_compile_options(lua_rt PRIVATE -O2 -Wall -Wextra)

# bytecode compiler
add_executable(luac src/luac.c)
target_link_libraries(luac PRIVATE lua)
target_compile_options(luac PRIVATE -O2 -Wall -Wextra)
