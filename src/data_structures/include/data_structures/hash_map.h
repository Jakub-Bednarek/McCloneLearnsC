#ifndef HASH_MAP_C
#define HASH_MAP_C

#include <stdlib.h>
#include <stdint.h>

#define MEMORY_ALLOCATION_FAILED 1
#define HASH_MAP_IS_NULL 2
#define KEY_IN_MAP 3
#define KEY_NOT_IN_MAP 4

typedef void* any_val_t;
typedef void* map_t;

extern map_t   hash_map_create();
extern map_t   hash_map_create_size(size_t size);
extern int32_t hash_map_add(map_t map, const char* key, any_val_t data);
extern int32_t hash_map_get(map_t map, const char* key, any_val_t* data);
extern int32_t hash_map_remove(map_t hash_map, const char* key);
extern int32_t hash_map_free(map_t map);

#endif // HASH_MAP_C