#ifndef COMPONENT_H
#define COMPONENT_H

#include "entity.h"
#include "pool_allocator/pool_allocator.h"

#include <stdint.h>
#include <stdlib.h>

#define COMPONENT_NAME_ALREADY_REGISTERED 1
#define COMPONENT_NOT_FOUND 2
#define COMPONENT_SIZE_INVALID 3
#define COMPONENTS_NUMBER_INVALID 4
#define MEMORY_ALLOCATION_FAILURE 5
#define COMPONENT_NOT_ASSIGNED_TO_ENTITY 6
#define COMPONENT_ALREADY_ADDED_TO_ENTITY 7

#define MAX_NUMBER_OF_COMPONENTS_TYPES 100

typedef struct {
    EntityId entity;
    void* component_data;
} ComponentMapping;

typedef struct {
    const char* name;
    size_t number_of_allocated_components;
    size_t max_number_of_elements;
    uint32_t signature;
    ComponentMapping** components_storage;
    pool_allocator_t component_allocator;
} ComponentArray;

typedef struct {
    size_t next_free_component_index;
    uint32_t next_signature;
    ComponentArray registered_components[MAX_NUMBER_OF_COMPONENTS_TYPES];
} ComponentManager;

extern void    component_manager_initialize(ComponentManager*);
extern void    component_manager_uninitialize(ComponentManager*);
extern int32_t component_register(ComponentManager*, const char*, size_t, size_t);
extern int32_t component_unregister(ComponentManager*, const char*);
extern int32_t component_add_to_entity(ComponentManager*, const char*, EntityId, void**, Signature*);
extern int32_t component_remove_from_entity(ComponentManager*, const char*, EntityId, Signature*);
extern void*   component_get_from_entity(ComponentManager*, const char*, EntityId);

#endif // COMPONENT_H