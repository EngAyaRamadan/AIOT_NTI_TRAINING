cmake_minimum_required(VERSION 4.1.0)
project(AIOT)
set(SRC_FILES
    src/memory.c
    src/stack.c
    src/queue.c
    include/stack.h
    include/queue.h
)

add_executable(memory ${SRC_FILES})

target_include_directories(memory PUBLIC include/)