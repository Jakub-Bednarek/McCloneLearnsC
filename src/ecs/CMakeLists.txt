target_include_engine_directory(include)
target_add_engine_sources(
    src/component.c
    src/entity.c
    src/system.c
    src/ecs_manager.c
)

add_ut_subdirectory(tests)