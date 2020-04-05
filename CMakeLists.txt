cmake_minimum_required (VERSION 2.6)

project(EXAMPLE)
#enable_testing()

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)
add_subdirectory(src)
