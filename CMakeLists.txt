cmake_minimum_required(VERSION 2.8)
project(pseudo)
enable_testing()

add_definitions("-std=c++11")
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory("test")
