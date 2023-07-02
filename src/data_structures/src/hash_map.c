#include "hash_map.h"

#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

/* TODO
- Code cleanup
- proper hash function
- class generalization for key [MACRO?]
- rehash func
- non-constant map size
- buckets split
- proper unified error handling
*/

typedef struct {
    const char* key;
    bool used;
    any_val_t data;
} entry_t;

typedef struct {
    size_t size;
    size_t used_size;
    entry_t* entries;
} hash_map_t;

size_t hash_map_hash_string(const char* key, const size_t size) {
    size_t hash = 0;
    size_t key_size = strlen(key);
    for(size_t i = 0; i < key_size; i++) {
        hash += (size_t)key[i];
    }

    return hash % size;
}

void hash_map_print(const hash_map_t* hash_map)
{
    for(size_t i = 0; i < hash_map->size; i++) {
            printf("I: %ld, %s, used: %d, val: %d\n", i, hash_map->entries[i].key, hash_map->entries[i].used, hash_map->entries[i].data);
    }
}

map_t hash_map_create(const size_t size)
{
    hash_map_t* hash_map = calloc(sizeof(hash_map_t), 1);
    if(hash_map == NULL) {
        errno = MEMORY_ALLOCATION_FAILED;
        return NULL;
    }

    hash_map->entries = calloc(sizeof(entry_t), size);
    if(hash_map->entries == NULL) {
        errno = MEMORY_ALLOCATION_FAILED;
        return NULL;
    }
    hash_map->size = size;
    hash_map->used_size = 0;

    return hash_map;
}

int32_t hash_map_add(map_t map, const char* key, any_val_t data)
{
    hash_map_t* hash_map = (hash_map_t*)map;
    if(hash_map == NULL) {
        return HASH_MAP_IS_NULL;
    }

    if(hash_map->used_size == hash_map->size) {
        return 10;
    }

    size_t index = hash_map_hash_string(key, hash_map->size);
    for(size_t i = 0; i < hash_map->size; ++i, ++index) {
        if(!hash_map->entries[index].used) {
                hash_map->entries[index].key = key;
                hash_map->entries[index].used = true;
                hash_map->entries[index].data = data;
                ++hash_map->used_size;
                break;
        }
        else if (strcmp(hash_map->entries[index].key, key) == 0) {
            return KEY_IN_MAP;
        }
    }

    return 0;
}

int32_t hash_map_get(map_t map, const char* key, any_val_t* data)
{
    hash_map_t* hash_map = (hash_map_t*)map;
    if(hash_map == NULL) {
        return HASH_MAP_IS_NULL;
    }

    size_t index = hash_map_hash_string(key, hash_map->size);
    for(size_t i = 0; i < hash_map->size; ++i, ++index) {
        if(index == hash_map->size) {
            index = 0;
        }
        if(strcmp(hash_map->entries[index].key, key) == 0) {
            if(!hash_map->entries[index].used) {
                return KEY_NOT_IN_MAP;
            }
            *data = hash_map->entries[index].data;
            break;
        }
    }

    return 0;
}

int32_t hash_map_remove(map_t map, const char* key)
{
    hash_map_t* hash_map = (hash_map_t*)map;
    if(hash_map == NULL) {
        return HASH_MAP_IS_NULL;
    }

    size_t index = hash_map_hash_string(key, hash_map->size);
    for(size_t i = 0; i < hash_map->size; i++) {
        if(index == hash_map->size) {
            index = 0;
        }
        if(strcmp(hash_map->entries[index].key, key) == 0) {
            hash_map->entries[index].used = false;
            --hash_map->used_size;
            return 0;
        }
    }

    return KEY_NOT_IN_MAP;
}

int32_t hash_map_free(map_t map)
{
    hash_map_t* hash_map = (hash_map_t*)map;
    if(hash_map == NULL) {
        return HASH_MAP_IS_NULL;
    }

    free(hash_map->entries);
    free(hash_map);
    hash_map = NULL;

    return 0;
}