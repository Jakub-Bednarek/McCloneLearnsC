set(UNITY_DIR ${CMAKE_SOURCE_DIR}/vendor/Unity)
set(UNITY_LIB Unity)

if(UT_ENABLED)
    message(STATUS "Unit tests config enabled")
    include(CTest)

    add_library(${UNITY_LIB} STATIC ${UNITY_DIR}/src/unity.c)
    target_include_directories(${UNITY_LIB} PUBLIC ${UNITY_DIR}/src)
endif()

function(add_ut_subdirectory DIR)
    if(NOT UT_ENABLED)
        return()
    endif()

    add_subdirectory(${DIR})
endfunction()

function(add_ut)
    if(NOT UT_ENABLED)
        return()
    endif()

    set(options DUMMY)
    set(oneValueArgs TARGET)
    set(multiValueArgs HEADERS SOURCES LIBRARIES)
    cmake_parse_arguments(UT_ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${UT_ARGS_TARGET} ${UT_ARGS_HEADERS} ${UT_ARGS_SOURCES})
    target_link_libraries(${UT_ARGS_TARGET} ${UT_ARGS_LIBRARIES} ${UNITY_LIB})
endfunction()