#include "component.h"
#include "core/allocators/pool_allocator.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define MAX_NUMBER_OF_COMPONENTS_TYPES 100
#define MAX_NUMBER_OF_COMPONENTS_IN_ARRAY 10000

typedef struct {
    EntityId entity;
    void* component_data;
} ComponentMapping;

typedef struct {
    const char* name;
    size_t number_of_allocated_components;
    size_t max_number_of_elements;
    ComponentMapping** components_storage;
    PoolAllocator component_allocator;
} ComponentArray;

struct ComponentsManager{
    size_t next_free_component_index;
    ComponentArray registered_components[MAX_NUMBER_OF_COMPONENTS_TYPES];
} g_components_manager;

void components_manager_initialize()
{
    g_components_manager.next_free_component_index = 0;
}

void components_manager_unitialize()
{
    for(size_t i = 0; i < g_components_manager.next_free_component_index; i++) {
        g_components_manager.registered_components[i].components_storage = NULL;
        pool_allocator_free(&g_components_manager.registered_components[i].component_allocator);
    }
}

int32_t component_register(const char* name, const size_t size, const size_t number_of_elements)
{
    if(size <= 0) {
        errno = COMPONENT_SIZE_INVALID;
        return -1;
    }

    if(number_of_elements <= 0) {
        errno = COMPONENTS_NUMBER_INVALID;
        return -1;
    }

    for(size_t i = 0; i < g_components_manager.next_free_component_index; i++) {
        if(strcmp(g_components_manager.registered_components[i].name, name) == 0) {
            errno = COMPONENT_NAME_ALREADY_REGISTERED;
            return -1;
        }
    }

    ComponentMapping** component_mapping = calloc(number_of_elements, sizeof(ComponentMapping*));
    if(component_mapping == NULL) {
        errno = MEMORY_ALLOCATION_FAILURE;
        return -1;
    }
    
    PoolAllocator allocator;
    if(pool_allocator_alloc(&allocator, sizeof(EntityId) + size, number_of_elements) == -1) {
        return -1;
    }

    ComponentArray* component = &g_components_manager.registered_components[g_components_manager.next_free_component_index];
    component->component_allocator = allocator;
    component->components_storage = component_mapping;
    component->name = name;
    component->number_of_allocated_components = 0;
    component->max_number_of_elements = number_of_elements;

    ++g_components_manager.next_free_component_index;

    return 0;
}

int32_t component_unregister(const char* name)
{
    int32_t found_component_index = -1;
    for(size_t i = 0; i < g_components_manager.next_free_component_index; i++) {
        if(strcmp(g_components_manager.registered_components[i].name, name) == 0) {
            found_component_index = i;
            break;
        }
    }

    if(found_component_index == -1) {
        errno = COMPONENT_NOT_FOUND;
        return -1;
    }

    pool_allocator_free(&g_components_manager.registered_components[found_component_index].component_allocator);
    for(size_t i = found_component_index; i < g_components_manager.next_free_component_index - 1; i++) {
        // No need for deep copy considering current implementation of PoolAllocator
        g_components_manager.registered_components[i] = g_components_manager.registered_components[i + 1];
    }
    --g_components_manager.next_free_component_index;

    return 0;
}

int32_t component_add_to_entity(const char* name, const EntityId entity, void** component_data_ptr)
{
    int32_t found_component_index = -1;
    for(size_t i = 0; i < g_components_manager.next_free_component_index; i++) {
        if(strcmp(g_components_manager.registered_components[i].name, name) == 0) {
            found_component_index = i;
            break;
        }
    }

    if(found_component_index == -1) {
        errno = COMPONENT_NOT_FOUND;
        return -1;
    }

    ComponentArray* component_array = &g_components_manager.registered_components[found_component_index];
    for(size_t i = 0; i < component_array->number_of_allocated_components; i++) {
        if(component_array->components_storage[i]->entity == entity) {
            errno = COMPONENT_ALREADY_ADDED_TO_ENTITY;
            return -1;
        }
    }

    ComponentMapping* allocated_component = pool_allocator_get_new_element(&component_array->component_allocator);
    allocated_component->entity = entity;
    component_array->components_storage[component_array->number_of_allocated_components++] = allocated_component;

    if(component_data_ptr != NULL) {
        *component_data_ptr = &allocated_component->component_data;
    }

    return 0;
}

void* component_get_from_entity(const char* name, const EntityId entity)
{
    ComponentArray* component_array = NULL;
    for(size_t i = 0; i < g_components_manager.next_free_component_index; ++i) {
        if(strcmp(g_components_manager.registered_components[i].name, name) == 0) {
            component_array = &g_components_manager.registered_components[i];
        }
    }

    if(component_array == NULL) {
        errno = COMPONENT_NOT_FOUND;
        return NULL;
    }

    for(size_t i = 0; i < component_array->number_of_allocated_components; i++) {
        if(component_array->components_storage[i]->entity == entity) {
            return &component_array->components_storage[i]->component_data;
        }
    }
    
    errno = COMPONENT_NOT_ASSIGNED_TO_ENTITY;
    return NULL;
}

int32_t component_remove_from_entity(const char* name, const EntityId entity)
{
    ComponentArray* component_array = NULL;
    for(size_t i = 0; i < g_components_manager.next_free_component_index; ++i) {
        if(strcmp(g_components_manager.registered_components[i].name, name) == 0) {
            component_array = &g_components_manager.registered_components[i];
        }
    }

    if(component_array == NULL) {
        errno = COMPONENT_NOT_FOUND;
        return NULL;
    }

    for(size_t i = 0; i < component_array->number_of_allocated_components; i++) {
        if(component_array->components_storage[i]->entity == entity) {
            pool_allocator_free_element(&component_array->component_allocator, (void*)(component_array->components_storage[i]));
        }
    }

    errno = COMPONENT_NOT_ASSIGNED_TO_ENTITY;
    return -1;
}
