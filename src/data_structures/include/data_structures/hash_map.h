#ifndef HASH_MAP_C
#define HASH_MAP_C

#include <stdlib.h>
#include <stdint.h>

#define MEMORY_ALLOCATION_FAILED 1
#define NULL_DATA_STRUCT 2
#define HASH_MAP_FULL 3
#define KEY_ALREADY_IN_MAP 4
#define KEY_NOT_IN_MAP 5

typedef void* any_val_t;

struct hash_map_t;

extern struct hash_map_t* hash_map_create(size_t size);
extern int32_t hash_map_add(struct hash_map_t* map, const char* key, any_val_t val);
extern any_val_t hash_map_get(struct hash_map_t* map, const char* key);
extern int32_t hash_map_remove(struct hash_map_t* hash_map, const char* key);

#endif // HASH_MAP_C