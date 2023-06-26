#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdint.h>
#include <stdlib.h>

#define COMPONENT_NAME_ALREADY_REGISTERED 1
#define COMPONENT_STORAGE_ALLOCATION_FAILURE 2
#define COMPONENT_NOT_FOUND 3
#define COMPONENT_STORAGE_CAPACITY_EXCEEDED 4
#define COMPONENT_SIZE_INVALID 5
#define COMPONENTS_NUMBER_INVALID 6

extern void components_manager_initialize();
extern int32_t component_register(const char*, size_t, size_t);
extern int32_t component_add_to_entity(const char*, void*, size_t);

#endif // COMPONENT_H