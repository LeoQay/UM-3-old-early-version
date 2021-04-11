cmake_minimum_required(VERSION 3.19)
project(UM3)

set(CMAKE_CXX_STANDARD 14)
add_executable(UM3 main.cpp Processor.cpp Processor.h memory_um3.cpp memory_um3.h parser_um_3.cpp parser_um_3.h)