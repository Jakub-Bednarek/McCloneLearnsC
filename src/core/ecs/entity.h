#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>

#define MAX_ENTITIES_ALLOCATED 1
#define ENTITY_NOT_FOUND 2

typedef int32_t EntityId;

extern void entity_manager_initialize();
extern EntityId entity_get_next_free_id();
extern int32_t entity_free();

#endif // ENTITY_H