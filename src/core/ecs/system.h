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
    EntityId registered_entities[MAX_ENTITIES_IN_SYSTEM];
    void (*on_update)(double);
} System;

typedef struct {
    System registered_systems[MAX_NUMBER_OF_SYSTEMS];
    size_t number_of_registered_systems;
} SystemManager;

extern int32_t system_manager_init(SystemManager*);
extern int32_t system_manager_add_entity(SystemManager*, EntityId, Signature);
extern int32_t system_manager_remove_entity(SystemManager*, EntityId);
extern int32_t system_register(SystemManager*, const char*, void(*)(), Signature);
extern void system_manager_on_update(SystemManager*, double);
extern void test_systems();

#endif // SYSTEM_H