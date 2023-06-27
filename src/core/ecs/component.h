#ifndef COMPONENT_H
#define COMPONENT_H

#include "entity.h"

#include <stdint.h>
#include <stdlib.h>

#define COMPONENT_NAME_ALREADY_REGISTERED 1
#define COMPONENT_NOT_FOUND 2
#define COMPONENT_SIZE_INVALID 3
#define COMPONENTS_NUMBER_INVALID 4
#define MEMORY_ALLOCATION_FAILURE 5
#define COMPONENT_NOT_ASSIGNED_TO_ENTITY 6
#define COMPONENT_ALREADY_ADDED_TO_ENTITY 7

extern void    components_manager_initialize(void);
extern int32_t component_register(const char*, size_t, size_t);
extern int32_t component_unregister(const char*);
extern int32_t component_add_to_entity(const char*, EntityId, void**);
extern void*   component_get_from_entity(const char*, EntityId);
extern int32_t component_remove_from_entity(const char*, EntityId);
extern void    components_test();

#endif // COMPONENT_H