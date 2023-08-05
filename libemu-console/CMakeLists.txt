cmake_minimum_required(VERSION 3.27)
project(libemu-console)

set(CMAKE_CXX_STANDARD 23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-march=skylake)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/arch:AVX2)
endif ()

add_executable(libemu-console main.cpp)
add_dependencies(libemu-console libemu-dasm libemu-dasm-static fmt::fmt)
target_include_directories(libemu-console PUBLIC ../include ../libemu-dasm ../lib/fmt/include)
target_link_directories(libemu-console PUBLIC ${libemu-dasm_BINARY_DIR} ${FMT_BINARY_DIR})
target_link_libraries(libemu-console PUBLIC libemu-dasm fmt::fmt)
