project(McCloneLearnsC)
cmake_minimum_required(VERSION 3.21)

set(COMPILE_OPTIONS -Wall)

include(cmake/unit_test.cmake)

add_subdirectory(src)
add_subdirectory(vendor)

add_compile_options(${COMPILE_OPTIONS})

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} engine)