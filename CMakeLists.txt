cmake_minimum_required(VERSION 3.6)
project(SoftwareProject)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp SPConfig.c)
add_executable(SoftwareProject ${SOURCE_FILES})