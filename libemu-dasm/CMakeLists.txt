cmake_minimum_required(VERSION 3.27)
project(libemu-dasm)

set(CMAKE_CXX_STANDARD 23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-march=skylake)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/arch:AVX2)
endif ()

set(SOURCES mos6502/mos6502_disassembler.cpp)

add_library(libemu-dasm SHARED ${SOURCES})
add_library(libemu-dasm-static STATIC ${SOURCES})
set_target_properties(libemu-dasm PROPERTIES PREFIX "")
set_target_properties(libemu-dasm-static PROPERTIES PREFIX "")
set_target_properties(libemu-dasm-static PROPERTIES OUTPUT_NAME "libemu-dasm")
target_include_directories(libemu-dasm PUBLIC ../include)
target_include_directories(libemu-dasm-static PUBLIC ../include)
