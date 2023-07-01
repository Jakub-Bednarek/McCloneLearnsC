#include "hash_map.h"

#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    const char* key;
    bool used;
    any_val_t val;
} entry_t;

struct hash_map_t {
    size_t size;
    size_t used_size;
    entry_t* entries;
};

size_t hash_map_hash_string(const char* key, const size_t size) {
    size_t hash = 0;
    size_t key_size = strlen(key);
    for(size_t i = 0; i < key_size; i++) {
        hash += (size_t)key[i];
    }

    // printf("Hash bef mod: %d\n", hash);
    // printf("Hash after mod: %d\n", hash % size);

    return hash % size;
}

void hash_map_print(const struct hash_map_t* hash_map)
{
    for(size_t i = 0; i < hash_map->size; i++) {
            printf("I: %ld, %s, used: %d, val: %d\n", i, hash_map->entries[i].key, hash_map->entries[i].used, hash_map->entries[i].val);
    }
}

struct hash_map_t* hash_map_create(const size_t size)
{
    struct hash_map_t* hash_map = calloc(sizeof(struct hash_map_t), 1);
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

int32_t hash_map_add(struct hash_map_t* hash_map, const char* key, any_val_t val)
{
    if(hash_map == NULL) {
        return NULL_DATA_STRUCT;
    }

    if(hash_map->used_size == hash_map->size) {
        return HASH_MAP_FULL;
    }

    size_t index = hash_map_hash_string(key, hash_map->size);
    for(size_t i = 0; i < hash_map->size; ++i, ++index) {
        if(!hash_map->entries[index].used) {
                hash_map->entries[index].key = key;
                hash_map->entries[index].used = true;
                hash_map->entries[index].val = val;
                ++hash_map->used_size;
                break;
        }
        else if (strcmp(hash_map->entries[index].key, key) == 0) {
            return KEY_ALREADY_IN_MAP;
        }
    }

    return 0;
}

any_val_t hash_map_get(struct hash_map_t* hash_map, const char* key)
{
    if(hash_map == NULL) {
        return NULL;
    }

    size_t index = hash_map_hash_string(key, hash_map->size);
    for(size_t i = 0; i < hash_map->size; ++i, ++index) {
        if(index == hash_map->size) {
            index = 0;
        }
        if(strcmp(hash_map->entries[index].key, key) == 0) {
            if(!hash_map->entries[index].used) {
                return NULL;
            }
            return hash_map->entries[index].val;
        }
    }

    return NULL;
}

int32_t hash_map_remove(struct hash_map_t* hash_map, const char* key)
{
    if(hash_map == NULL) {
        return NULL_DATA_STRUCT;
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