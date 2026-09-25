cmake_minimum_required(VERSION 3.15)
project(power_audio)

# standard version of C
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# executable files
add_library(pwra SHARED
  pwra_ma.c
  pwra_aux.c
  pwra.c
)

set_target_properties(pwra PROPERTIES
  PREFIX ""
  OUTPUT_NAME "pwra"
)

target_compile_options(pwra PRIVATE
  -Wall
  -O2
  -march=x86-64-v2
  -DNDEBUG
)

# general headers
target_include_directories(pwra PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../external
)

target_compile_definitions(pwra PRIVATE PWRA_SL=1)
