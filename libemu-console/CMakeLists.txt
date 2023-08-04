cmake_minimum_required(VERSION 3.27)
project(libemu-console)

set(CMAKE_CXX_STANDARD 23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-march=skylake)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/arch:AVX2)
endif ()

include_directories(../include ../libemu-dasm)

add_executable(libemu-console main.cpp)
add_dependencies(libemu-console libemu-dasm)
target_link_libraries(libemu-console PUBLIC libemu-dasm.so)
