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
        ${OpenCV_LIBRARIES})
else()
    set(LIBRARIES ${OpenCV_LIBRARIES})
endif()
