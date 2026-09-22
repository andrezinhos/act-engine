cmake_minimum_required(VERSION 3.15)
project(power_audio)

# standard version of C++
set(CMAKE_CC_STANDARD 17)
set(CMAKE_CC_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# executable files
add_library(pwra STATIC
  pwra_aux.c
  pwra.c
)

target_compile_options(pwra PRIVATE
  -Wall
  -O2
  -march=x86-64-v2
  -DNDEBUG
)

# general headers
target_include_directories(pwra PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../external
)
