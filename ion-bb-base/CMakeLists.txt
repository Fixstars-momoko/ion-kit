project(ion-bb-core)
cmake_minimum_required(VERSION 3.12)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

include(config.cmake)

set(BB_NAME "" CACHE STRINGS "BB name")
set(BUILD_BB off CACHE BOOL "Build bb lib")
set(BUILD_RT off CACHE BOOL "Build runtime lib")

link_directories(${ION_BB_LINK_DIRS})

if (UNIX)
    set(PLATFORM_LIBRARIES pthread dl)
endif()

if(BUILD_BB AND BB_SRCS)
    add_library("${BB_NAME}-bb" ${BB_SRCS})
    target_include_directories("${BB_NAME}-bb" PRIVATE include)
    target_link_libraries("${BB_NAME}-bb" ${CONAN_LIBS} ${PLATFORM_LIBRARIES})
endif()

if(BUILD_RT AND RT_SRCS)
    add_library("${BB_NAME}-rt" ${RT_SRCS})
    target_include_directories("${BB_NAME}-rt" PRIVATE include)
    target_link_libraries("${BB_NAME}-rt" ${CONAN_LIBS} ${PLATFORM_LIBRARIES})
endif()
