cmake_minimum_required(VERSION 3.24)
project(flita2dz C)

set(CMAKE_C_STANDARD 11)

add_executable(flita2dz main.c graph.c graph.h list.h list.c set.h set.c)
