cmake_minimum_required(VERSION 3.3)
project(malloc_free)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")

set(SOURCE_FILES main.c)
add_executable(malloc_free ${SOURCE_FILES})