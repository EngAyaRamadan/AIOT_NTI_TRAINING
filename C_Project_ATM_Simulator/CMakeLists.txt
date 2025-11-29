cmake_minimum_required(VERSION 4.1.0)
project(AIOT)
set(SRC_FILES
    src/main.c
    src/admin.c
    src/customer.c
    src/app.c
    include/admin.h
     include/private.h
      include/app.h
)

add_executable(main ${SRC_FILES})

target_include_directories(main PUBLIC incude/)