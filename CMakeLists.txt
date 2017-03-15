cmake_minimum_required(VERSION 3.6)
project(SoftwareProject)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp SPConfig.c unit_tests/sp_config_unit_test.c SPKDArray.c SPKDArray.h)
add_executable(SoftwareProject ${SOURCE_FILES})