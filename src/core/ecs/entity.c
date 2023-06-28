#include "entity.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void entity_manager_initialize(EntityManager* entity_manager)
{
    static bool is_initialized = false;
    if(is_initialized) {
        printf("EntityManager is already initialized\n");
        return;
    }

    printf("Initializng EntityManager with %d entity allocations\n", MAX_NUMBER_OF_ENTITIES);

    for(EntityId i = 0; i < MAX_NUMBER_OF_ENTITIES; ++i) {
        entity_manager->entities[i] = i;
    }

    for(size_t i = 0; i < MAX_NUMBER_OF_ENTITIES; ++i) {
        entity_manager->entity_signatures[i] = 0;
    }

    entity_manager->first_taken_entity = 0;
    entity_manager->next_free_entity_index = 0;
    entity_manager->currently_allocated_entities = 0;
    is_initialized = true;
}

EntityId entity_get_next_free_id(EntityManager* entity_manager)
{
    if(entity_manager->currently_allocated_entities == MAX_NUMBER_OF_ENTITIES) {
        errno = MAX_ENTITIES_ALLOCATED;
        return -1;
    }

    const EntityId free_entity = entity_manager->entities[entity_manager->next_free_entity_index];
    ++entity_manager->next_free_entity_index;
    ++entity_manager->currently_allocated_entities;

    return free_entity;
}

int32_t entity_free(EntityManager* entity_manager, const EntityId entity)
{
    if(entity >= MAX_NUMBER_OF_ENTITIES) {
        errno = ENTITY_OUT_OF_RANGE;
        return -1;
    }

    int32_t entity_index = -1;
    for(size_t index = entity_manager->first_taken_entity; index < entity_manager->next_free_entity_index; ++index) {
        if(entity_manager->entities[index] == entity) {
            entity_index = index;
            break;
        }
    }

    if(entity_index == -1) {
        errno = ENTITY_NOT_FOUND;
        return -1;
    }
    
    for(size_t index = entity_index; index < entity_manager->next_free_entity_index - 1; index++) {
        entity_manager->entities[index] = entity_manager->entities[index + 1];
    }
    entity_manager->entities[entity_manager->next_free_entity_index - 1] = entity;
    --entity_manager->next_free_entity_index;

    return 0;
}

int32_t entity_add_to_signature(EntityManager* entity_manager, EntityId entity, Signature signature)
{
    if(entity >= MAX_NUMBER_OF_ENTITIES) {
        errno = ENTITY_OUT_OF_RANGE;
        return -1;
    }

    entity_manager->entity_signatures[entity] |= signature;
    return 0;
}

int32_t entity_remove_from_signature(EntityManager* entity_manager, EntityId entity, Signature signature)
{
    if(entity >= MAX_NUMBER_OF_ENTITIES) {
        errno = ENTITY_OUT_OF_RANGE;
        return -1;
    }

    entity_manager->entity_signatures[entity] ^= signature;
    return 0;
}

int32_t entity_get_signature(EntityManager* entity_manager, EntityId entity, Signature* signature)
{
    if(entity >= MAX_NUMBER_OF_ENTITIES) {
        errno = ENTITY_OUT_OF_RANGE;
        return -1;
    }

    *signature = entity_manager->entity_signatures[entity];
    return 0;
}
