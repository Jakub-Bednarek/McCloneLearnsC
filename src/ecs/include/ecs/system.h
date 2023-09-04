#ifndef SYSTEM_H
#define SYSTEM_H

#include "entity.h"

#include <stdint.h>
#include <stdlib.h>

#define SYSTEM_ALREADY_REGISTERED 1
#define ENTITY_ALREADY_REGISTERED_IN_SYSTEM 2
#define ENTITY_NOT_REGISTERED_IN_SYSTEM 3

#define MAX_ENTITIES_IN_SYSTEM 10000
#define MAX_NUMBER_OF_SYSTEMS 32

typedef struct {
    const char* name;
    uint32_t signature;
    uint32_t number_of_registered_entities;
    entity_id_t registered_entities[MAX_ENTITIES_IN_SYSTEM];
    void (*on_update)(double);
} System;

typedef struct {
    System registered_systems[MAX_NUMBER_OF_SYSTEMS];
    size_t number_of_registered_systems;
} system_manager_t;

extern int32_t system_manager_initialize(system_manager_t*);
extern int32_t system_manager_uninitialize(system_manager_t*);
extern int32_t system_manager_add_entity(system_manager_t*, entity_id_t, signature_t);
extern int32_t system_manager_remove_entity(system_manager_t*, entity_id_t);
extern int32_t system_manager_extend_entity_signature(system_manager_t*, entity_id_t, signature_t);
extern int32_t system_manager_reduce_entity_signature(system_manager_t*, entity_id_t, signature_t);
extern int32_t system_register(system_manager_t*, const char*, void(*)(), signature_t);
extern void system_manager_on_update(system_manager_t*, double);
extern void test_systems();

#endif // SYSTEM_H