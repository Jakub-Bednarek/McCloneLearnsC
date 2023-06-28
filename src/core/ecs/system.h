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

int32_t system_manager_init(void);
int32_t system_manager_add_entity(EntityId, unsigned long);
int32_t system_manager_remove_entity(EntityId);
int32_t system_register(const char*, void(*)(), unsigned long);
void system_manager_on_update(double);
void test_systems();

#endif // SYSTEM_H