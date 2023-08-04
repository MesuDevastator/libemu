cmake_minimum_required(VERSION 3.27)
project(libemu-console)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-march=skylake -L. -lemu-dasm)

include_directories(../include ../libemu-dasm)

add_executable(libemu-console main.cpp)
