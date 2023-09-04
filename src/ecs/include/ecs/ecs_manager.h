#ifndef ECS_MANAGER_H
#define ECS_MANAGER_H

#include "entity.h"

#include <stdint.h>

extern int32_t ecs_initialize(size_t max_number_of_entities, size_t max_number_of_components);
extern int32_t ecs_uninitialize();
extern entity_id_t ecs_get_free_entity();
extern int32_t ecs_remove_entity(entity_id_t);
extern int32_t ecs_register_component(const char*, size_t, size_t);
extern int32_t ecs_unregister_component(const char*);
extern int32_t ecs_add_component_to_entity(const char*, entity_id_t, void**);
extern int32_t ecs_remove_component_from_entity(const char*, entity_id_t);
extern int32_t ecs_register_system(const char*, void(*)(double), const char**, size_t);

#endif // ECS_MANAGER_H