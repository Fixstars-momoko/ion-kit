project(ion-kit LANGUAGES C CXX)

cmake_minimum_required(VERSION 3.12)

#
# Options
#
set(ION_BB_DIR "" CACHE STRING "A List of building blocks to build")

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
    if(ION_BB_DIR STREQUAL "")
        set(bb_list ${CONAN_DEPENDENCIES})
        list(FILTER bb_list INCLUDE REGEX "^ion-bb-")
        foreach(bb_name IN LISTS bb_list)
            string(TOUPPER "${bb_name}" bb_name_upper)
            list(APPEND ION_BB_DIR "${CONAN_${bb_name_upper}_ROOT}")
        endforeach()
    endif()
else()
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # This is required to export symbols on windows platform

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

#
# ion utilities
#
include(IonUtil)

#
# ion-bb
#
include(ImportIonBB)
ion_import_building_block()

#
# Link directories
#
link_directories(${ION_BB_LINK_DIRS})

#
# Build
#
ion_jit(main SRCS main.cpp)
