cmake_minimum_required(VERSION 3.8)
project(libemu-dasm)

set(CMAKE_CXX_STANDARD 23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-march=skylake)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/arch:AVX2)
endif ()

set(SOURCES mos6502/mos6502_disassembler.cpp mos6502/mos6502_operation.cpp)

add_library(libemu-dasm STATIC ${SOURCES})
set_target_properties(libemu-dasm PROPERTIES PREFIX "")
target_include_directories(libemu-dasm PUBLIC ../include)
