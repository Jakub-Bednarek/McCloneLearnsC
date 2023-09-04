#include "ecs/entity.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

entity_error_t entity_manager_alloc(entity_manager_t** entity_manager, int max_number_of_entities)
{
    if(max_number_of_entities <= 0) {
        *entity_manager = NULL;
        return INVALID_MAX_ENTITIES_VALUE;
    }

    *entity_manager = calloc(1, sizeof(entity_manager_t));
    if(!*entity_manager) {
        *entity_manager = NULL;
        return MEM_FAILURE;
    }

    printf("Initializng entity_manager with %d entity allocations\n", max_number_of_entities);

    for(entity_id_t i = 0; i < max_number_of_entities; ++i) {
        (*entity_manager)->entities[i] = i;
    }

    for(size_t i = 0; i < max_number_of_entities; ++i) {
        (*entity_manager)->entity_signatures[i] = 0;
    }

    (*entity_manager)->first_taken_entity = 0;
    (*entity_manager)->max_entities = max_number_of_entities;
    (*entity_manager)->next_free_entity_index = 0;
    (*entity_manager)->n_allocated_entities = 0;

    return ENTITY_OK;
}

void entity_manager_free(entity_manager_t* entity_manager)
{
    free(entity_manager);
    printf("Entity manager unitialize\n");
}

entity_error_t entity_get_next_free_id(entity_manager_t* entity_manager, entity_id_t* entity_to_allocate)
{
    if(entity_manager->n_allocated_entities == entity_manager->max_entities) {
        return MAX_ENTITIES_ALLOCATED;
    }

    const entity_id_t free_entity = entity_manager->entities[entity_manager->next_free_entity_index];
    ++entity_manager->next_free_entity_index;
    ++entity_manager->n_allocated_entities;

    return ENTITY_OK;
}

entity_error_t entity_free(entity_manager_t* entity_manager, const entity_id_t entity_id)
{
    if(entity_id < 0 || entity_id >= entity_manager->max_entities) {
        return ENTITY_OUT_OF_RANGE;
    }

    entity_id_t entity_index = entity_manager->max_entities;
    for(size_t index = entity_manager->first_taken_entity; index < entity_manager->next_free_entity_index; ++index) {
        if(entity_manager->entities[index] == entity_id) {
            entity_index = index;
            break;
        }
    }

    if(entity_index == entity_manager->max_entities) {
        return ENTITY_NOT_FOUND;
    }
    
    for(size_t index = entity_index; index < entity_manager->next_free_entity_index - 1; index++) {
        entity_manager->entities[index] = entity_manager->entities[index + 1];
    }
    entity_manager->entities[entity_manager->next_free_entity_index - 1] = entity_id;
    --entity_manager->next_free_entity_index;

    return ENTITY_OK;
}

entity_error_t entity_add_component_signature(entity_manager_t* entity_manager, entity_id_t entity_id, signature_t component_signature)
{
    if(entity_id < 0 || entity_id >= entity_manager->max_entities) {
        return ENTITY_OUT_OF_RANGE;
    }

    entity_manager->entity_signatures[entity_id] |= component_signature;
    return ENTITY_OK;
}

entity_error_t entity_remove_component_signature(entity_manager_t* entity_manager, entity_id_t entity_id, signature_t signature_t)
{
    if(entity_id < 0 || entity_id >= entity_manager->max_entities) {
        return ENTITY_OUT_OF_RANGE;
    }

    entity_manager->entity_signatures[entity_id] ^= signature_t;
    return ENTITY_OK;
}

entity_error_t entity_get_signature(entity_manager_t* entity_manager, entity_id_t entity_id, signature_t* signature)
{
    if(entity_id < 0 || entity_id >= entity_manager->max_entities) {
        return ENTITY_OUT_OF_RANGE;
    }

    *signature = entity_manager->entity_signatures[entity_id];
    return ENTITY_OK;
}
