set(ENGINE_LIBRARY_NAME engine)

add_subdirectory(data_structures)
add_subdirectory(graphics)
add_subdirectory(core)
add_subdirectory(utils)
add_subdirectory(world)

set(LINUX_LIBS "-lGLU -lGL -lX11 -lXfixes -lm" GLAD cglm_headers)
target_include_engine_directory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR/vender/glad2/include}
)

get_property(ENGINE_INCLUDE_PATHS GLOBAL PROPERTY ENGINE_INCLUDE_PATHS)
get_property(ENGINE_SOURCE_FILES GLOBAL PROPERTY ENGINE_SOURCE_FILES)

add_library(${ENGINE_LIBRARY_NAME} STATIC ${CORE_SOURCES} ${GRAPHICS_SOURCES} ${WORLD_SOURCES} ${UTILS_SOURCES} ${ENGINE_SOURCE_FILES})
target_include_directories(${ENGINE_LIBRARY_NAME} PUBLIC ${ENGINE_INCLUDE_PATHS})
target_link_libraries(${ENGINE_LIBRARY_NAME}
    ${LINUX_LIBS}
)
