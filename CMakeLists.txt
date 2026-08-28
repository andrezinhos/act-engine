cmake_minimum_required(VERSION 3.15)
project(act-engine)

# standard version of C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(LUA_HEADERS "C:/lua/src")
set(LUA_LIB "C:/lua/src/liblua.a")

set(GLFW_HEADERS ${CMAKE_SOURCE_DIR}/external/glfw)
set(GLFW_LIB ${CMAKE_SOURCE_DIR}/external/glfw/libglfw3dll.a)

set(MKR_HEADERS ${CMAKE_SOURCE_DIR}/src/mkr)
set(MKR_LIB ${CMAKE_SOURCE_DIR}/src/mkr/lib/libmkr.a)

# executable files
add_executable(app
  src/main.cpp
  src/core.cpp
  src/inputs.cpp
  src/script.cpp
)

# general headers
target_include_directories(app PRIVATE
  ${CMAKE_SOURCE_DIR}/external
  ${CMAKE_SOURCE_DIR}/src
  ${GLFW_HEADERS}
  ${LUA_HEADERS}
  ${MKR_HEADERS}
)

# linking main dependecies
target_link_libraries(app PRIVATE ${GLFW_LIB} ${LUA_LIB} ${MKR_LIB})

# OS libs
if(WIN32)
  message("WINDOWS COMPILATION")
  target_link_libraries(app PRIVATE gdi32 opengl32 user32 shell32)
endif()
