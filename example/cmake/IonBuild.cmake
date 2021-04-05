#
# Options
#
set(ION_BB_DIR "" CACHE STRING "A List of building blocks to build")

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(NO_OUTPUT_DIRS)
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

#
# ion-bb
#
include(ImportIonBB)
ion_import_building_block()

#
# Link directories
#
link_directories(${ION_BB_LINK_DIRS})

if (UNIX)
    set(PLATFORM_LIBRARIES pthread dl)
endif()

function(ion_compile NAME)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SRCS)
    cmake_parse_arguments(IEC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT DEFINED CACHE{HL_TARGET})
        set(HL_TARGET "host" CACHE STRING "HL_TARGET for compile")
    endif()

    # Build compile
    add_executable(${NAME} ${IEC_SRCS})
    if(UNIX)
        target_compile_options(${NAME}
            PUBLIC -fno-rtti)  # For Halide::Generator
    endif()
    set(ION_BB_LIBS ${CONAN_LIBS})
    list(FILTER ION_BB_LIBS INCLUDE REGEX "^ion-bb-")
    set(OTHER_LIBS ${CONAN_LIBS})
    list(FILTER OTHER_LIBS EXCLUDE REGEX "^ion-bb-")
    # target_include_directories(${NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include;${ION_BB_INCLUDE_DIRS};${HALIDE_INCLUDE_DIR}")
    # target_link_libraries(${NAME} ion-core Halide::Halide ${ION_BB_LIBRARIES} ${PLATFORM_LIBRARIES})
    target_link_libraries(${NAME} -Wl,--whole-archive ${ION_BB_LIBS} -Wl,--no-whole-archive ${OTHER_LIBS})

    # Run compile
    set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
    set(HEADER ${OUTPUT_PATH}/${NAME}.h)
    set(STATIC_LIB ${OUTPUT_PATH}/${NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
    if (UNIX)
        add_custom_command(OUTPUT ${HEADER} ${STATIC_LIB}
            COMMAND ${CMAKE_SOURCE_DIR}/../../../ion-core/script/invoke.sh $<TARGET_FILE:${NAME}>
                HL_TARGET ${HL_TARGET}
                LD_LIBRARY_PATH ${CMAKE_BINARY_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>
            DEPENDS ${NAME}
            WORKING_DIRECTORY ${OUTPUT_PATH})
    else()
        add_custom_command(OUTPUT ${HEADER} ${STATIC_LIB}
            COMMAND ${CMAKE_SOURCE_DIR}/../../../ion-core/script/invoke.bat $<TARGET_FILE:${NAME}>
                HL_TARGET ${HL_TARGET}
                PATH ${CMAKE_BINARY_DIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>
            DEPENDS ${NAME}
            WORKING_DIRECTORY ${OUTPUT_PATH})
    endif()
    add_custom_target(build ALL DEPENDS ${HEADER} ${STATIC_LIB})
endfunction()

function(ion_run NAME LIB_DIR PIPELINE_NAME)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SRCS)
    cmake_parse_arguments(IER "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Build run
    set(HEADER ${LIB_DIR}/${PIPELINE_NAME}.h)
    set(STATIC_LIB ${LIB_DIR}/${PIPELINE_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX})
    add_executable(${NAME} ${IER_SRCS})
    target_include_directories(${NAME} PUBLIC ${LIB_DIR})
    # target_include_directories(${NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include;${ION_BB_INCLUDE_DIRS};${HALIDE_INCLUDE_DIR};${OUTPUT_PATH}")
    # target_link_libraries(${NAME} ${STATIC_LIB} ${ION_BB_LIBRARIES} Halide::Halide ${PLATFORM_LIBRARIES})
    target_link_libraries(${NAME} ${STATIC_LIB} ${CONAN_LIBS} ${PLATFORM_LIBRARIES})
endfunction()

function(ion_jit NAME)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs SRCS)
    cmake_parse_arguments(IEJ "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    add_executable(${NAME} ${IEJ_SRCS})
    #target_include_directories(${NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include;${ION_BB_INCLUDE_DIRS};${HALIDE_INCLUDE_DIR}")
    #target_link_libraries(${NAME} ion-core Halide::Halide ${ION_BB_LIBRARIES} ${PLATFORM_LIBRARIES})
    #target_link_options(${NAME} PUBLIC -Wl,--no-as-needed)
    set(ION_BB_LIBS ${CONAN_LIBS})
    list(FILTER ION_BB_LIBS INCLUDE REGEX "^ion-bb-")
    set(OTHER_LIBS ${CONAN_LIBS})
    list(FILTER OTHER_LIBS EXCLUDE REGEX "^ion-bb-")
    target_link_libraries(${NAME} -Wl,--whole-archive ${ION_BB_LIBS} -Wl,--no-whole-archive ${OTHER_LIBS})
    set_target_properties(${NAME} PROPERTIES ENABLE_EXPORTS ON)
endfunction()
