#include "pool_allocator/pool_allocator.h"

#include <errno.h>
#include <stdlib.h>

typedef struct Chunk Chunk;

struct Chunk {
    Chunk* next;
};

struct pool_allocator_t {
    size_t memory_used;
    size_t element_size;
    size_t number_of_allocations;
    size_t max_allocations;
    Chunk* pool_start;
    Chunk* next_free_element;
};

pool_allocator_error_t pool_allocator_alloc(pool_allocator_t** allocator, const size_t element_size, const size_t number_of_elements)
{
    if (element_size < sizeof(void *)) {
        return MALLOC_ERROR;
    }

    pool_allocator_t* pool_allocator = calloc(sizeof(pool_allocator_t), 1);
    if(!pool_allocator) {
        return MALLOC_ERROR;
    }

    Chunk *chunk_begin = calloc(number_of_elements, element_size);
    if (chunk_begin == NULL) {
        return MALLOC_ERROR;
    }

    pool_allocator->pool_start = chunk_begin;
    pool_allocator->next_free_element = chunk_begin;
    pool_allocator->element_size = element_size;
    pool_allocator->max_allocations = number_of_elements;
    pool_allocator->number_of_allocations = 0;
    pool_allocator->memory_used = 0;

    Chunk *current_chunk = chunk_begin;
    for (size_t i = 0; i < number_of_elements - 1; ++i) {
        current_chunk->next = (Chunk *)((char *)(current_chunk) + element_size);
        current_chunk = current_chunk->next;
    }

    current_chunk->next = NULL;

    return POOL_ALLOCATOR_OK;
}

void pool_allocator_free(pool_allocator_t* allocator)
{
    if(!allocator)
    {
        return;
    }

    free(allocator->pool_start);

    allocator->pool_start = NULL;
    allocator->next_free_element = NULL;
}

element_t pool_allocator_alloc_element(pool_allocator_t* allocator)
{
    if(!allocator)
    {
        return NULL;
    }

    pool_allocator_t* pool_allocator = (pool_allocator_t*)allocator;
    if (pool_allocator->next_free_element == NULL) {
        errno = MAXIMUM_NUMBER_OF_ELEMENTS_ALLOCATED;
        return NULL;
    }

    Chunk *new_element = pool_allocator->next_free_element;
    pool_allocator->next_free_element = new_element->next;
    pool_allocator->memory_used += pool_allocator->element_size;
    ++pool_allocator->number_of_allocations;

    return new_element;
}

void pool_allocator_free_element(pool_allocator_t* allocator, void *element_to_free)
{
    if(!allocator)
    {
        return;
    }

    Chunk *freed_element = (Chunk *)(element_to_free);
    freed_element->next = allocator->next_free_element;
    allocator->next_free_element = freed_element;
    allocator->memory_used -= allocator->element_size;
    --allocator->number_of_allocations;
}

const size_t pool_allocator_get_element_size(pool_allocator_t* allocator)
{
    if(!allocator) {
        return 0;
    }

    return allocator->element_size;
}

const size_t pool_allocator_get_number_of_allocations(pool_allocator_t* allocator)
{
    if(!allocator) {
        return 0;
    }

    return allocator->number_of_allocations;
}

const size_t pool_allocator_get_memory_used(pool_allocator_t* allocator)
{
    if(!allocator) {
        return 0;
    }

    return allocator->memory_used;
}

const size_t pool_allocator_get_max_number_of_allocations(pool_allocator_t* allocator)
{
    if(!allocator) {
        return 0;
    }

    return allocator->max_allocations;
}
