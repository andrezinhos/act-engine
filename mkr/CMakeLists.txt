cmake_minimum_required(VERSION 3.15)
project(monkey_render)

# standard version of C
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

add_library(mkr STATIC
  glad.c
  mklib.c
  mktext.c
  mktex.c
  mkgeo.c
  mkr.c
)

target_compile_options(mkr PRIVATE
  -Wall
  -Wextra
  -O0
  # -march=x86-64-v2
  -DDEBUG
)

# set_target_properties(mkr PROPERTIES INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

# general headers
target_include_directories(mkr PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

# target_compile_definitions(mkr PRIVATE MKR_SL=0)
