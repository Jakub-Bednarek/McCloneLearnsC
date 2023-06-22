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
    graphics/window.h
    graphics/texture.h
    graphics/texture_atlas.h)

set(GRAPHICS_SOURCES
    graphics/renderer.c
    graphics/window.c
    graphics/texture.c
    graphics/texture_atlas.c)

set(WORLD_HEADERS
    world/world.h)

set(WORLD_SOURCES)

set(UTILS_HEADERS
    utils/file_loader.h
    utils/bmp_loader.h)

set(UTILS_SOURCES
    utils/file_loader.c
    utils/bmp_loader.c)

add_library(${LIBRARY_NAME} STATIC ${CORE_SOURCES} ${GRAPHICS_SOURCES} ${WORLD_SOURCES} ${UTILS_SOURCES})
target_include_directories(${LIBRARY_NAME} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${LIBRARY_NAME} ${LINUX_LIBS})