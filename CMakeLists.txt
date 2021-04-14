cmake_minimum_required(VERSION 3.19)
project(UM3)

set(CMAKE_CXX_STANDARD 14)
add_executable(UM3 main.cpp Processor.cpp Processor.h memory_cl.h memory_cl.cpp parser.cpp parser.h)