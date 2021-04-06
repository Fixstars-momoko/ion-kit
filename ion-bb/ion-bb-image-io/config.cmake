set(BB_SRCS src/bb.cpp)
set(RT_SRCS
    src/rt_common.cpp
    src/rt_display.cpp
    src/rt_file.cpp
    src/rt_v4l2.cpp
)

if(ENABLE_REALSENSE)
    list(APPEND BB_SRCS src/bb_realsense.cpp)
    list(APPEND RT_SRCS src/rt_realsense.cpp)
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
