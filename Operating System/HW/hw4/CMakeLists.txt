cmake_minimum_required(VERSION 3.24)
project(hw4)

set(CMAKE_CXX_STANDARD 17)

add_executable(hw4-main.c hw4.c hw4-main.c)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )