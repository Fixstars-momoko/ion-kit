set(BB_SRCS src/bb.cpp)
set(RT_SRCS src/rt.cpp
)

if(NOT DEFINED CONAN_PACKAGE_NAME)
    find_package(OpenCV 4 REQUIRED)
    if (UNIX)
        add_compile_options(-Wno-format-security)
    endif()

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
