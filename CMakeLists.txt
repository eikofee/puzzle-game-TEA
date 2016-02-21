cmake_minimum_required(VERSION 2.6)

project(Rush-Hour)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
include_directories (src/)
add_subdirectory(./src/)

