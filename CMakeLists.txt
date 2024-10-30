cmake_minimum_required(VERSION 3.10)

project(LANGUAGES C)

add_library(MyLib float8.c)

add_executable(float8-converter main.c)

target_link_libraries(float8-converter PRIVATE MyLib)
