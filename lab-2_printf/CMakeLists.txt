cmake_minimum_required(VERSION 2.8)
project(lab_2_printf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(SOURCE_FILES main.c)
add_executable(lab_2_printf ${SOURCE_FILES})