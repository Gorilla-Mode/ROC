cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_COMPILER clang)

project(ROC C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(ROC
        src/main.c
)

target_include_directories(ROC PRIVATE src/)

add_executable(TEST
        src/test.c
)

target_include_directories(TEST PRIVATE src/)

get_filename_component(COMPILER_BIN_DIR "${CMAKE_C_COMPILER}" DIRECTORY)
get_filename_component(CLANG_ROOT "${COMPILER_BIN_DIR}/.." ABSOLUTE)

set(PDC_LIB "${CLANG_ROOT}/lib/pdcurses.a")
set(PDC_INCLUDE "${CLANG_ROOT}/include")

target_include_directories(ROC PRIVATE "${PDC_INCLUDE}")
target_link_libraries(ROC PRIVATE "${PDC_LIB}")