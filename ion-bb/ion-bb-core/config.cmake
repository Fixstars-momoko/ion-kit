set(BB_SRCS src/bb.cpp)
set(RT_SRCS src/rt.cpp)

if(NOT DEFINED CONAN_PACKAGE_NAME)
    set(INCLUDE_DIRS)
    set(LINK_DIRS)
    set(LIBRARIES)
endif()
