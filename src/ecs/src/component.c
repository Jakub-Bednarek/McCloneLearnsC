#include "ecs/component.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

void component_manager_initialize(component_manager_t* component_manager)
{
    printf("Component manager initialize\n");
    component_manager->next_free_component_index = 0;
    component_manager->next_signature = 0b1;
}

void component_manager_uninitialize(component_manager_t* component_manager)
{
    printf("Component manager unitialize\n");
    for(size_t i = 0; i < component_manager->next_free_component_index; i++) {
        component_manager->registered_components[i].components_storage = NULL;
        pool_allocator_free(&component_manager->registered_components[i].component_allocator);
    }
}

signature_t component_manager_get_next_signature(component_manager_t* component_manager)
{
    signature_t next_signature = component_manager->next_signature;
    component_manager->next_signature = component_manager->next_signature << 1;

    return next_signature;
}

int32_t component_register(component_manager_t* component_manager, const char* name, const size_t size, const size_t number_of_elements)
{
    if(size <= 0) {
        errno = COMPONENT_SIZE_INVALID;
        return -1;
    }

    if(number_of_elements <= 0) {
        errno = COMPONENTS_NUMBER_INVALID;
        return -1;
    }

    for(size_t i = 0; i < component_manager->next_free_component_index; i++) {
        if(strcmp(component_manager->registered_components[i].name, name) == 0) {
            errno = COMPONENT_NAME_ALREADY_REGISTERED;
            return -1;
        }
    }

    component_mapping_t** component_mapping = calloc(number_of_elements, sizeof(component_mapping_t*));
    if(component_mapping == NULL) {
        errno = MEMORY_ALLOCATION_FAILURE;
        return -1;
    }
    
    pool_allocator_t allocator;
    // if(pool_allocator_alloc(&allocator, sizeof(entity_id_t) + size, number_of_elements) == -1) {
    //     return -1;
    // }

    component_array_t* component = &component_manager->registered_components[component_manager->next_free_component_index];
    component->component_allocator = allocator;
    component->components_storage = component_mapping;
    component->name = name;
    component->signature = component_manager_get_next_signature(component_manager);
    component->number_of_allocated_components = 0;
    component->max_number_of_elements = number_of_elements;

    ++component_manager->next_free_component_index;

    return 0;
}

int32_t component_unregister(component_manager_t* component_manager, const char* name)
{
    int32_t found_component_index = -1;
    for(size_t i = 0; i < component_manager->next_free_component_index; i++) {
        if(strcmp(component_manager->registered_components[i].name, name) == 0) {
            found_component_index = i;
            break;
        }
    }

    if(found_component_index == -1) {
        errno = COMPONENT_NOT_FOUND;
        return -1;
    }

    pool_allocator_free(&component_manager->registered_components[found_component_index].component_allocator);
    for(size_t i = found_component_index; i < component_manager->next_free_component_index - 1; i++) {
        // No need for deep copy considering current implementation of PoolAllocator
        component_manager->registered_components[i] = component_manager->registered_components[i + 1];
    }
    --component_manager->next_free_component_index;

    return 0;
}

int32_t component_add_to_entity(component_manager_t* component_manager, const char* name, const entity_id_t entity, void** component_data_ptr, signature_t* signature)
{
    int32_t found_component_index = -1;
    for(size_t i = 0; i < component_manager->next_free_component_index; i++) {
        if(strcmp(component_manager->registered_components[i].name, name) == 0) {
            found_component_index = i;
            break;
        }
    }

    if(found_component_index == -1) {
        errno = COMPONENT_NOT_FOUND;
        return -1;
    }

    component_array_t* component_array = &component_manager->registered_components[found_component_index];
    for(size_t i = 0; i < component_array->number_of_allocated_components; i++) {
        if(component_array->components_storage[i]->entity == entity) {
            errno = COMPONENT_ALREADY_ADDED_TO_ENTITY;
            return -1;
        }
    }

    component_mapping_t* allocated_component = pool_allocator_alloc_element(&component_array->component_allocator);
    allocated_component->entity = entity;
    component_array->components_storage[component_array->number_of_allocated_components++] = allocated_component;
    *signature = component_array->signature;

    if(component_data_ptr != NULL) {
        *component_data_ptr = &allocated_component->component_data;
    }

    return 0;
}

void* component_get_from_entity(component_manager_t* component_manager, const char* name, const entity_id_t entity)
{
    component_array_t* component_array = NULL;
    for(size_t i = 0; i < component_manager->next_free_component_index; ++i) {
        if(strcmp(component_manager->registered_components[i].name, name) == 0) {
            component_array = &component_manager->registered_components[i];
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

int32_t component_remove_from_entity(component_manager_t* component_manager, const char* name, const entity_id_t entity, signature_t* signature)
{
    component_array_t* component_array = NULL;
    for(size_t i = 0; i < component_manager->next_free_component_index; ++i) {
        if(strcmp(component_manager->registered_components[i].name, name) == 0) {
            component_array = &component_manager->registered_components[i];
        }
    }

    if(component_array == NULL) {
        errno = COMPONENT_NOT_FOUND;
        return -1;
    }

    *signature = component_array->signature;
    for(size_t i = 0; i < component_array->number_of_allocated_components; i++) {
        if(component_array->components_storage[i]->entity == entity) {
            pool_allocator_free_element(&component_array->component_allocator, (void*)(component_array->components_storage[i]));
        }
    }

    errno = COMPONENT_NOT_ASSIGNED_TO_ENTITY;
    return -1;
}
