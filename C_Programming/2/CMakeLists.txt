cmake_minimum_required(VERSION 4.1.0)
project(AIOT)
set(SRC_FILES
    src/Basic.c

)

add_executable(Basic ${SRC_FILES})

target_include_directories(Basic PUBLIC include/)