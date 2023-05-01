cmake_minimum_required(VERSION 3.21)
project(OS_HW C)

set(CMAKE_C_STANDARD 11)

add_executable(OS_HW
        hw_11/client_1.c
        hw_11/client_2.c
        hw_11/server.c)
