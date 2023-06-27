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

int32_t component_add_to_entity(const char* name, const EntityId entity, void** component_data_ptr, const size_t size)
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

    ComponentArray* component_array = &g_components_manager.registered_components[found_component_index];
    printf("ComponentArray ptr: 0x%x\n", component_array);
    ComponentMapping* allocated_component = pool_allocator_get_new_element(&component_array->component_allocator);
    printf("ComponentMapping ptr: 0x%x\n", allocated_component);
    allocated_component->entity = entity;
    component_array->components_storage[component_array->number_of_allocated_components++] = allocated_component;

    printf("ComponentMapping: 0x%x, component_dat: 0x%x\n", allocated_component, &allocated_component->component_data);
    *component_data_ptr = &allocated_component->component_data;

    return 0;
}

void* component_get_for_entity(const char* name, const EntityId entity)
{
    ComponentArray* component_array = &g_components_manager.registered_components[0];
    return &component_array->components_storage[0]->component_data;
}

// typedef struct {
//     unsigned int texture_id;
//     float left;
//     float right;
//     float top;
//     float bottom;
// } Texture;

// void components_test() { 
//     components_manager_initialize();
//     if(component_register("texture", sizeof(Texture), 100) == -1) {
//         printf("Error\n");
//     }
//     EntityId e = 5;
//     Texture* t = NULL;
//     if(component_add_to_entity("texture", e, (void**)(&t), sizeof(Texture)) == -1) {
//         printf("Error adding\n");
//     }
//     printf("Add: 0x%x\n", t);
//     t->bottom = 0.5;
//     t->top = 0.4;
//     t->left = 0.3;
//     t->right = 0.2;
//     printf("Texture address: 0x%x, data: bottom: %f, top: %f, left: %f, right: %f\n", t, t->bottom, t->top, t->left, t->right);
//     Texture* my_tex = component_get_for_entity("ab", 0);
//     printf("Addr: 0x%x\n", my_tex);
//     printf("%f\n", my_tex->top);
//     my_tex->top = 0.583f;
//     printf("NEW T: %f\n", t->top);
//     // printf("Get component: 0x%x, data: bottom: %f, top: %f, left: %f, right: %f\n", my_tex, my_tex->bottom, my_tex->top, my_tex->left, my_tex->right);
// }