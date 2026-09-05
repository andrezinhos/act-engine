cmake_minimum_required(VERSION 3.15)
project(monkey_render)

# standard version of C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(GLFW_HEADERS ${CMAKE_SOURCE_DIR}/external/glfw)
set(GLFW_LIB ${CMAKE_SOURCE_DIR}/external/glfw/libglfw3.a)

# executable files
add_library(mkr STATIC
  glad.cpp
  mkgl.cpp
  mktxt.cpp
  mktex.cpp
  mkr_helpers.cpp
  mkr.cpp
)

# general headers
target_include_directories(mkr PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../external
  ${GLFW_HEADERS}
)

target_link_libraries(mkr PRIVATE ${GLFW_LIB})

# OS libs
if(WIN32)
  target_link_libraries(mkr PRIVATE gdi32 opengl32 user32 shell32)
endif()
