#include "component.h"
#include "core/allocators/pool_allocator.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define MAX_NUMBER_OF_COMPONENTS 100

typedef struct {
    const char* name;
    size_t size;
    size_t next_index;
    size_t max_number_of_elements;
    PoolAllocator storage;
} ComponentArray;

struct ComponentsManager{
    size_t next_free_component_index;
    ComponentArray registered_components[MAX_NUMBER_OF_COMPONENTS];
} g_components_manager;

void components_manager_initialize()
{
    g_components_manager.next_free_component_index = 0;
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
    
    PoolAllocator allocator;
    if(pool_allocator_alloc(&allocator, size, number_of_elements) == -1) {
        return -1;
    }

    ComponentArray* component = &g_components_manager.registered_components[g_components_manager.next_free_component_index];
    component->storage = allocator;
    component->name = name;
    component->next_index = 0;
    component->size = 0;
    component->max_number_of_elements = number_of_elements;

    ++g_components_manager.next_free_component_index;

    return 0;
}

int32_t component_add_to_entity(const char* name, void* component_data, const size_t size)
{
    int32_t found_component_index = -1;
    for(size_t i = 0; i < g_components_manager.next_free_component_index; i++) {
        if(strcmp(g_components_manager.registered_components[i].name, name) == 0) {
            found_component_index = i;
        }
    }

    if(found_component_index == -1) {
        errno = COMPONENT_NOT_FOUND;
        return -1;
    }

    return 0;
}

void components_test() { printf("TESTED\n"); }