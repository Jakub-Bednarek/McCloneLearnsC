set(UNITY_DIR ${CMAKE_SOURCE_DIR}/vendor/Unity)
set(UNITY_LIB Unity)
set(DEFAULT_UT_BIN_DIR ${CMAKE_SOURCE_DIR}/build/ut/bin)

if(UT_ENABLED)
    message(STATUS "Unit tests config enabled")
    include(CTest)

    add_library(${UNITY_LIB} STATIC ${UNITY_DIR}/src/unity.c)
    target_include_directories(${UNITY_LIB} PUBLIC ${UNITY_DIR}/src)

    if("${UT_BIN_DIR}" STREQUAL "")
        message(WARNING "Unit tests bin directory not set. Defaulting to ${DEFAULT_UT_BIN_DIR}")
        set(UT_BIN_DIR ${DEFAULT_UT_BIN_DIR})
    else()
        message(STATUS "Unit Tests dir: ${UT_BIN_DIR}")
    endif()
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

    # message(FATAL_ERROR ${ENGINE_LIBRARY_NAME})
    add_executable(test_${UT_ARGS_TARGET} ${UT_ARGS_HEADERS} ${UT_ARGS_SOURCES})
    target_link_libraries(test_${UT_ARGS_TARGET} ${UT_ARGS_LIBRARIES} ${UNITY_LIB} ${ENGINE_LIBRARY_NAME})
    set_target_properties(test_${UT_ARGS_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${UT_BIN_DIR})
endfunction()