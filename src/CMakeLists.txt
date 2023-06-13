set(LIBRARY_NAME engine)

set(LINUX_LIBS "-lGLU -lGL -lX11 -lXfixes -lm" GLAD cglm_headers)
set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR/vender/glad2/include})

set(CORE_HEADERS
    core/error.h
    core/input.h
    core/timer.h)

set(CORE_SOURCES
    core/input.c
    core/timer.c)

set(GRAPHICS_HEADERS
    graphics/renderer.h
    graphics/shader_loader.h
    graphics/window.h)

set(GRAPHICS_SOURCES
    graphics/renderer.c
    graphics/shader_loader.c
    graphics/window.c)

set(WORLD_HEADERS
    world/world.h)

set(WORLD_SOURCES)

add_library(${LIBRARY_NAME} STATIC ${CORE_SOURCES} ${GRAPHICS_SOURCES} ${WORLD_SOURCES})
target_include_directories(${LIBRARY_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${LIBRARY_NAME} ${LINUX_LIBS})