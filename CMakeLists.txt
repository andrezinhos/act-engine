cmake_minimum_required(VERSION 3.15)
project(act-engine)

# standard version of C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(LUA_HEADERS ${CMAKE_SOURCE_DIR}/external/lua/src)
set(LUA_LIB ${CMAKE_SOURCE_DIR}/external/lua/build/liblua.a)

set(GLFW_LIB ${CMAKE_SOURCE_DIR}/external/glfw/libglfw3dll.a)

set(MKR_HEADERS ${CMAKE_SOURCE_DIR}/mkr)
set(MKR_LIB ${CMAKE_SOURCE_DIR}/mkr/lib/libmkr.a)

set(PWRA_HEADERS ${CMAKE_SOURCE_DIR}/pwra)
set(PWRA_LIB ${CMAKE_SOURCE_DIR}/pwra/lib/libpwra.dll.a)

set(ENGINE_CORE
  src/core.cpp
  src/time.cpp
  src/reg.cpp
)

set(ENGINE_SYS
  src/ios.cpp
  src/script.cpp
  src/animation.cpp
  src/esys.cpp
)

# executable files
add_executable(app
  ${ENGINE_CORE}
  ${ENGINE_SYS}
  src/f_main.cpp
)

target_precompile_headers(app PRIVATE
    "external/sol.hpp"
)

target_compile_options(app PRIVATE
  -Wall
  -O2
  -march=x86-64-v2
  -DNDEBUG
  -fno-rtti
)

set_target_properties(app PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# general headers
target_include_directories(app PRIVATE
  ${CMAKE_SOURCE_DIR}/external
  ${CMAKE_SOURCE_DIR}/src
  ${LUA_HEADERS}
  ${MKR_HEADERS}
  ${PWRA_HEADERS}
)

# linking main dependecies
target_link_libraries(app PRIVATE
  ${GLFW_LIB}
  ${LUA_LIB}
  ${MKR_LIB}
  ${PWRA_LIB}
)

# OS libs
if(WIN32)
  message("WINDOWS COMPILATION")
  target_link_libraries(app PRIVATE user32 shell32)

else(UNIX)
# todo: unix links
endif()
