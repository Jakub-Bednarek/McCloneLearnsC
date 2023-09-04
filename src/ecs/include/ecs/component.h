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
    entity_id_t entity;
    void* component_data;
} component_mapping_t;

typedef struct {
    const char* name;
    size_t number_of_allocated_components;
    size_t max_number_of_elements;
    uint32_t signature;
    component_mapping_t** components_storage;
    pool_allocator_t component_allocator;
} component_array_t;

typedef struct {
    size_t next_free_component_index;
    uint32_t next_signature;
    component_array_t registered_components[MAX_NUMBER_OF_COMPONENTS_TYPES];
} component_manager_t;

extern void    component_manager_initialize(component_manager_t*);
extern void    component_manager_uninitialize(component_manager_t*);
extern int32_t component_register(component_manager_t*, const char*, size_t, size_t);
extern int32_t component_unregister(component_manager_t*, const char*);
extern int32_t component_add_to_entity(component_manager_t*, const char*, entity_id_t, void**, signature_t*);
extern int32_t component_remove_from_entity(component_manager_t*, const char*, entity_id_t, signature_t*);
extern void*   component_get_from_entity(component_manager_t*, const char*, entity_id_t);

#endif // COMPONENT_H