project(core_compile)
cmake_minimum_required(VERSION 3.12)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
include(IonBuild)

ion_compile(core SRCS main.cpp)
