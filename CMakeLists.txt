cmake_minimum_required(VERSION 3.6)
project(sort_file)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(sort_file ${SOURCE_FILES})