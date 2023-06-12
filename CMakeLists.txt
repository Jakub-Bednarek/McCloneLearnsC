project(McCloneLearnsC)
cmake_minimum_required(VERSION 3.21)

add_subdirectory(vendor/glad2)
add_subdirectory(vendor/cglm)

set(SOURCES graphics/window.c graphics/renderer.c core/timer.c graphics/shader_loader.c main.c)

add_compile_options("-Wall")
add_executable(McCloneLearnsC ${SOURCES})

target_include_directories(McCloneLearnsC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad2/include)
target_link_libraries(McCloneLearnsC 
    "-lGLU -lGL -lX11 -lXfixes -lm" 
    GLAD
    cglm_headers)