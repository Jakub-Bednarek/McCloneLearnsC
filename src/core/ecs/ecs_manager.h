#ifndef ECS_MANAGER_H
#define ECS_MANAGER_H

#include "entity.h"

#include <stdint.h>

extern int32_t ecs_initialize();
extern int32_t ecs_uninitialize();
extern EntityId ecs_get_free_entity();
extern int32_t ecs_remove_entity(EntityId);
extern int32_t ecs_register_component(const char*, size_t, size_t);
extern int32_t ecs_unregister_component(const char*);
extern int32_t ecs_add_component_to_entity(const char*, EntityId, void**);
extern int32_t ecs_remove_component_from_entity(const char*, EntityId);
extern int32_t ecs_register_system(const char*, void(*)(double), const char**, size_t);

#endif // ECS_MANAGER_H