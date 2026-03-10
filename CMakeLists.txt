cmake_minimum_required(VERSION 4.1)
project(ROC C)

set(CMAKE_C_STANDARD 23)

add_executable(ROC
        src/main.c
)

target_include_directories(ROC PRIVATE src/)