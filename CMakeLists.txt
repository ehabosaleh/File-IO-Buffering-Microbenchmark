cmake_minimum_required(VERSION 3.5)
project(FILE_IO LANGUAGES C)

add_executable(fileio src/fileio.c)

target_link_libraries(fileio m)
