#include "component.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>

#define MAX_NUMBER_OF_COMPONENTS 100

struct Chunk{
    struct Chunk* next;
};

typedef struct Chunk Chunk;

typedef struct {
    const char* name;
    size_t size;
    size_t next_index;
    size_t max_number_of_elements;
    Chunk* storage;
} ComponentArray;

struct ComponentsManager{
    size_t next_free_component_index;
    ComponentArray registered_components[MAX_NUMBER_OF_COMPONENTS];
} g_components_manager;

Chunk* chunk_allocate_storage(size_t size, size_t number_of_elements)
{
    Chunk* chunk_begin = malloc(number_of_elements * size);
    if(chunk_begin == NULL) {
        errno = COMPONENT_STORAGE_ALLOCATION_FAILURE;
        return NULL;
    }

    Chunk* current_chunk = chunk_begin;
    for(size_t i = 0; i < number_of_elements - 1; ++i) {
        current_chunk->next = (Chunk*)((char*)(current_chunk) + size);
        current_chunk = current_chunk->next;
    }

    current_chunk->next = NULL;

    return chunk_begin;
}

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
    
    Chunk* allocated_storage = chunk_allocate_storage(size, number_of_elements);
    if(allocated_storage == NULL) {
        return -1;
    }

    ComponentArray* component = &g_components_manager.registered_components[g_components_manager.next_free_component_index];
    component->storage = allocated_storage;
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
        printf("Loop i: %ld, Name: %s\n", i, g_components_manager.registered_components[i].name);
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