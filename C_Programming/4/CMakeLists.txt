cmake_minimum_required(VERSION 4.1.0)
project(AIOT)
set(SRC_FILES
    src/pointer.c
 
)

add_executable(pointer ${SRC_FILES})

target_include_directories(pointer PUBLIC include/)