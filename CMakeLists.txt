cmake_minimum_required (VERSION 3.21)
project(GBA-compress)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()

add_subdirectory(src)
add_subdirectory(test)