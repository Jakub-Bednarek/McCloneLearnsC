#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdlib.h>

struct entity_manager_t;
typedef struct entity_manager_t entity_manager_t;
typedef uint32_t entity_id_t;
typedef uint32_t signature_t;

typedef enum {
    ENTITY_OK,
    MEM_FAILURE,
    INVALID_MAX_ENTITIES_VALUE,
    MAX_ENTITIES_ALLOCATED,
    ENTITY_OUT_OF_RANGE,
    ENTITY_NOT_FOUND
} entity_error_t;

entity_error_t entity_manager_alloc(entity_manager_t** entity_manager, int max_number_of_entities);
void           entity_manager_free(entity_manager_t* entity_manager);
entity_error_t entity_manager_get_next_free_id(entity_manager_t* entity_manager, entity_id_t* entity_to_allocate);
entity_error_t entity_manager_free_entity(entity_manager_t*  entity_manager, entity_id_t entity_id);
entity_error_t entity_manager_add_component_signature_to_entity(entity_manager_t*  entity_manager, entity_id_t entity_id, signature_t component_signature);
entity_error_t entity_manager_remove_component_signature_from_entity(entity_manager_t* entity_manager, entity_id_t entity_id, signature_t entity_signature);
entity_error_t entity_manager_get_entity_signature(entity_manager_t*  entity_manager, entity_id_t entity_id, signature_t* entity_signature);
size_t         entity_manager_get_number_of_allocated_entities(entity_manager_t* entity_manager);

#endif // ENTITY_H