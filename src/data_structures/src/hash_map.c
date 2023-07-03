#include "hash_map.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULT_NUMBER_OF_BUCKETS 37

/* TODO
- class generalization for key [MACRO?]
*/

struct entry_t;

struct entry_t {
    const char* key;
    any_val_t data;
    struct entry_t* next;
};

typedef struct {
    size_t size;
    struct entry_t* head;
} bucket_t;

typedef struct {
    size_t size;
    bucket_t* buckets;
} hash_map_t;

// JOAAT hashing algorithm implementation for strings
size_t hash_map_hash_string(const char* key, const size_t buckets_number) {
    uint32_t hash = 0;
    size_t i = 0;
    while(key[i] != 0) {
        hash == key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash % buckets_number;
}

map_t hash_map_create()
{
    return hash_map_create_size(DEFAULT_NUMBER_OF_BUCKETS);
}

map_t hash_map_create_size(const size_t size)
{
    hash_map_t* hash_map = calloc(sizeof(hash_map_t), 1);
    if(hash_map == NULL) {
        return NULL;
    }

    hash_map->buckets = calloc(sizeof(bucket_t), size);
    if(hash_map->buckets == NULL) {
        return NULL;
    }
    hash_map->size = size;

    return hash_map;
}

int32_t hash_map_add(map_t map, const char* key, any_val_t data)
{
    hash_map_t* hash_map = (hash_map_t*)map;
    if(hash_map == NULL) {
        return HASH_MAP_IS_NULL;
    }

    struct entry_t* entry = calloc(sizeof(struct entry_t), 1);
    if(entry == NULL) {
        return MEMORY_ALLOCATION_FAILED;
    }

    entry->key = key;
    entry->next = NULL;
    entry->data = data;

    const size_t index = hash_map_hash_string(key, hash_map->size);
    bucket_t* bucket = &hash_map->buckets[index];
    if(bucket->head == NULL) {
        bucket->head = entry;
    } else {
        struct entry_t* curr = bucket->head;
        struct entry_t* prev = bucket->head;
        do {
            if(strcmp(curr->key, key) == 0) {
                return KEY_IN_MAP;
            }
            prev = curr;
            curr = curr->next;
        } while (curr != NULL);
        prev->next = entry;
    }

    return 0;
}

int32_t hash_map_get(map_t map, const char* key, any_val_t* data)
{
    hash_map_t* hash_map = (hash_map_t*)map;
    if(hash_map == NULL) {
        return HASH_MAP_IS_NULL;
    }

    const size_t index = hash_map_hash_string(key, hash_map->size);
    bucket_t* bucket = &hash_map->buckets[index];
    struct entry_t* entry = bucket->head;
    while(entry != NULL) {
        if(strcmp(entry->key, key) == 0) {
            *data = entry->data;
            break;
        }
        entry = entry->next;
    } 

    return 0;
}

int32_t hash_map_remove(map_t map, const char* key)
{
    hash_map_t* hash_map = (hash_map_t*)map;
    if(hash_map == NULL) {
        return HASH_MAP_IS_NULL;
    }

    const size_t index = hash_map_hash_string(key, hash_map->size);
    bucket_t* bucket = &hash_map->buckets[index];
    struct entry_t* prev = bucket->head;
    if(strcmp(prev->key, key) == 0) {
        bucket->head = prev->next;
        free(prev);
        return 0;
    }
    
    struct entry_t* curr = prev->next;
    while(curr != NULL) {
        if(strcmp(curr->key, key) == 0) {
            prev->next = curr->next;
            free(curr);
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }

    return KEY_NOT_IN_MAP;
}

int32_t hash_map_free(map_t map)
{
    hash_map_t* hash_map = (hash_map_t*)map;
    if(hash_map == NULL) {
        return HASH_MAP_IS_NULL;
    }

    for(size_t i = 0; i < hash_map->size; ++i) {
        struct entry_t* entry = hash_map->buckets[i].head;
        struct entry_t* next = NULL;
        while(entry != NULL) {
            entry = entry->next;
            free(entry);
        }
    }
    free(hash_map->buckets);
    free(hash_map);
    hash_map = NULL;

    return 0;
}