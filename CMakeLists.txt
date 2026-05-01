cmake_minimum_required(VERSION 4.1)
project(ROC C)

set(CMAKE_C_STANDARD 23)

add_executable(ROC
        src/main.c
)

target_include_directories(ROC PRIVATE src/)

add_executable(TEST
        src/test.c
)

get_filename_component(COMPILER_BIN_DIR "${CMAKE_C_COMPILER}" DIRECTORY)
get_filename_component(MINGW_ROOT "${COMPILER_BIN_DIR}/.." ABSOLUTE)
set(PDC_LIB "${MINGW_ROOT}/lib/pdcurses.a")
set(PDC_INCLUDE "${MINGW_ROOT}/include")

target_include_directories(ROC PRIVATE "${PDC_INCLUDE}")
target_link_libraries(ROC PRIVATE "${PDC_LIB}")

target_include_directories(TEST PRIVATE src/)