cmake_minimum_required(VERSION 3.27)
project(libemu-dasm)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-march=skylake)

include_directories(../include)

add_library(libemu-dasm SHARED ../include/operation.h mos6502/mos6502_disassembler.h mos6502/mos6502_disassembler.cpp disassembler.h ../include/type_define.h mos6502/mos6502_operation.h)
set_target_properties(libemu-dasm PROPERTIES PREFIX "")
