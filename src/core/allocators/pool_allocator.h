#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdio.h>
#include <stdint.h>

#define ELEMENT_SIZE_TOO_SMALL 1
#define MEMORY_ALLOCATION_FAILURE 2
#define MAXIMUM_NUMBER_OF_ELEMENTS_ALLOCATED 3

typedef struct Chunk Chunk;

struct Chunk {
    Chunk* next;
};

typedef struct {
    size_t used_memory;
    size_t element_size;
    size_t number_of_allocations;
    size_t total_number_of_elements;
    Chunk*  pool_start;
    Chunk* next_free_element;
} PoolAllocator;

extern int32_t pool_allocator_alloc(PoolAllocator*, size_t, size_t);
extern void          pool_allocator_free(PoolAllocator*); 
extern void*         pool_allocator_get_new_element(PoolAllocator*);
extern void          pool_allocator_free_element(PoolAllocator*, void*);

#endif // ALLOCATOR_H