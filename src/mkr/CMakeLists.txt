cmake_minimum_required(VERSION 3.15)
project(monkey_render)

# standard version of C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# executable files
add_library(mkr STATIC
  glad.cpp
  mkgl.cpp
  mkr_impl.cpp
)

# general headers
target_include_directories(mkr PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../external
)

# OS libs
if(WIN32)
  message("WINDOWS COMPILATION")
  target_link_libraries(mkr PRIVATE gdi32 opengl32 user32 shell32)
endif()
