cmake_minimum_required(VERSION 3.17)
project(UM_3)

set(CMAKE_CXX_STANDARD 14)

add_executable(UM_3 main.cpp parser_um_3.h Memory.h parser_um_3.cpp  Memory.cpp Processor.cpp Processor.h)