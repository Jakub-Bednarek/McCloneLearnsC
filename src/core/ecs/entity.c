#include "entity.h"

#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_NUMBER_OF_ENTITIES 10000

struct EntityManager {
    EntityId entities[MAX_NUMBER_OF_ENTITIES];
    size_t first_taken_entity;
    size_t next_free_entity_index;
    size_t currently_allocated_entities;
} g_entity_manager;

void entity_manager_initialize()
{
    static bool is_initialized = false;
    if(is_initialized) {
        printf("EntityManager is already initialized\n");
        return;
    }

    printf("Initializng EntityManager with %d entity allocations\n", MAX_NUMBER_OF_ENTITIES);

    for(EntityId i = 0; i < MAX_NUMBER_OF_ENTITIES; ++i) {
        g_entity_manager.entities[i] = i;
    }

    g_entity_manager.first_taken_entity = 0;
    g_entity_manager.next_free_entity_index = 0;
    g_entity_manager.currently_allocated_entities = 0;
    is_initialized = true;
}

EntityId entity_get_next_free_id()
{
    if(g_entity_manager.currently_allocated_entities == MAX_NUMBER_OF_ENTITIES) {
        errno = MAX_ENTITIES_ALLOCATED;
        return -1;
    }

    const EntityId free_entity = g_entity_manager.entities[g_entity_manager.next_free_entity_index];
    ++g_entity_manager.next_free_entity_index;
    ++g_entity_manager.currently_allocated_entities;

    return free_entity;
}

int32_t entity_free(const EntityId entity)
{
    int32_t entity_index = -1;
    for(size_t index = g_entity_manager.first_taken_entity; index < g_entity_manager.next_free_entity_index; ++index) {
        if(g_entity_manager.entities[index] == entity) {
            entity_index = index;
            break;
        }
    }

    if(entity_index == -1) {
        errno = ENTITY_NOT_FOUND;
        return -1;
    }
    
    for(size_t index = entity_index; index < g_entity_manager.next_free_entity_index - 1; index++) {
        g_entity_manager.entities[index] = g_entity_manager.entities[index + 1];
    }
    g_entity_manager.entities[g_entity_manager.next_free_entity_index - 1] = entity;
    --g_entity_manager.next_free_entity_index;

    return 0;
}