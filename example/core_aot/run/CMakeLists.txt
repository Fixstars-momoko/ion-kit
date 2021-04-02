project(core_run)
cmake_minimum_required(VERSION 3.12)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../cmake")
include(IonBuild)

ion_run(main ${CMAKE_CURRENT_SOURCE_DIR}/../compile/build core SRCS main.cpp)
