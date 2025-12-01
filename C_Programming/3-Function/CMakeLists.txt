cmake_minimum_required(VERSION 4.1.0)
project(AIOT)
set(SRC_FILES
    src/function.c
    src/user_function.c
    inc/user_function.h
)

add_executable(function ${SRC_FILES})

target_include_directories(function PUBLIC inc/)