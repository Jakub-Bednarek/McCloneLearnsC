#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdlib.h>


typedef uint32_t entity_id_t;
typedef uint32_t signature_t;
typedef struct {
    entity_id_t* entities;
    signature_t* entity_signatures;
    entity_id_t first_taken_entity;
    size_t max_entities;
    size_t next_free_entity_index;
    size_t n_allocated_entities;
} entity_manager_t;

typedef enum {
    ENTITY_OK,
    MEM_FAILURE,
    INVALID_MAX_ENTITIES_VALUE,
    MAX_ENTITIES_ALLOCATED,
    ENTITY_OUT_OF_RANGE,
    ENTITY_NOT_FOUND
} entity_error_t;

extern entity_error_t entity_manager_alloc(entity_manager_t** entity_manager, int max_number_of_entities);
extern void entity_manager_free(entity_manager_t* entity_manager);
extern entity_error_t entity_get_next_free_id(entity_manager_t* entity_manager, entity_id_t* entity_to_allocate);
extern entity_error_t entity_free(entity_manager_t*  entity_manager, entity_id_t entity_id);
extern entity_error_t entity_add_component_signature(entity_manager_t*  entity_manager, entity_id_t entity_id, signature_t component_signature);
extern entity_error_t entity_remove_component_signature(entity_manager_t* entity_manager, entity_id_t entity_id, signature_t entity_signature);
extern entity_error_t entity_get_signature(entity_manager_t*  entity_manager, entity_id_t entity_id, signature_t* entity_signature);

#endif // ENTITY_H