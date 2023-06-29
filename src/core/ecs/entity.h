#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>
#include <stdlib.h>

#define MAX_ENTITIES_ALLOCATED 1
#define ENTITY_NOT_FOUND 2
#define ENTITY_OUT_OF_RANGE 3

#define MAX_NUMBER_OF_ENTITIES 10000

typedef int32_t EntityId;
typedef uint32_t Signature;

typedef struct {
    EntityId entities[MAX_NUMBER_OF_ENTITIES];
    Signature entity_signatures[MAX_NUMBER_OF_ENTITIES];
    size_t first_taken_entity;
    size_t next_free_entity_index;
    size_t currently_allocated_entities;
} EntityManager;

extern void entity_manager_initialize(EntityManager*);
extern void entity_manager_unitialize(EntityManager*);
extern EntityId entity_get_next_free_id(EntityManager*);
extern int32_t entity_free(EntityManager*, EntityId);
extern int32_t entity_add_to_signature(EntityManager*, EntityId, Signature);
extern int32_t entity_remove_from_signature(EntityManager*, EntityId, Signature);
extern int32_t entity_get_signature(EntityManager*, EntityId, Signature*);

#endif // ENTITY_H