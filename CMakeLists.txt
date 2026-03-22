cmake_minimum_required(VERSION 3.5)
project(FILE_IO LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

add_executable(fileio ${CMAKE_CURRENT_SOURCE_DIR}/src/fileio.c)

target_include_directories(fileio PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(fileio m)
