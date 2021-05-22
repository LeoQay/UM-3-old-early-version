cmake_minimum_required(VERSION 3.13)

project(UM3 VERSION 1.0.0)

set(CMAKE_CXX_EXTENSIONS OFF )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_subdirectory(src)

