cmake_minimum_required(VERSION 3.15)
project(audio_make)

# standard version of C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# executable files
add_library(amk STATIC
  amk_helpers.cpp
  amk_impl.cpp
)

# general headers
target_include_directories(amk PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../external
)

target_link_libraries(amk PRIVATE ${GLFW_LIB})

# OS libs
if(WIN32)
  target_link_libraries(amk PRIVATE user32 shell32 kernel32)
endif()
