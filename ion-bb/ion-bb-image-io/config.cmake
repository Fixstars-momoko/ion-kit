set(BB_SRCS src/bb.cpp)
set(RT_SRCS
    src/rt_common.cpp
    src/rt_display.cpp
    src/rt_file.cpp
    src/rt_v4l2.cpp
    src/rt_realsense.cpp
)

set(ENABLE_REALSENSE off CACHE BOOL "Enable realsense BB")

if(ENABLE_REALSENSE)
    set_property(SOURCE src/rt_realsense.cpp PROPERTY COMPILE_DEFINITIONS ENABLE_REALSENSE)
endif()

if(NOT DEFINED CONAN_PACKAGE_NAME)
    find_package(OpenCV REQUIRED)

    set(INCLUDE_DIRS
        ${OpenCV_INCLUDE_DIRS})

    set(LINK_DIRS
        ${OpenCV_DIR}/lib)

    if (UNIX)
        set(LIBRARIES
            rt
            dl
            pthread
            m
            z
            ${OpenCV_LIBS})
    else()
        set(LIBRARIES ${OPENCV_LIBS})
    endif()
endif()
