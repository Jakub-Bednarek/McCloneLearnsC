#include "pool_allocator.h"

#include <errno.h>
#include <stdlib.h>

int32_t pool_allocator_alloc(PoolAllocator *pool_allocator, const size_t element_size, const size_t number_of_elements)
{
    if (element_size < sizeof(void *)) {
        errno = ELEMENT_SIZE_TOO_SMALL;
        return -1;
    }

    Chunk *chunk_begin = calloc(number_of_elements, element_size);
    if (chunk_begin == NULL) {
        errno = MEMORY_ALLOCATION_FAILURE;
        return -1;
    }

    pool_allocator->pool_start = chunk_begin;
    pool_allocator->next_free_element = chunk_begin;
    pool_allocator->element_size = element_size;
    pool_allocator->total_number_of_elements = number_of_elements;

    Chunk *current_chunk = chunk_begin;
    for (size_t i = 0; i < number_of_elements - 1; ++i) {
        current_chunk->next = (Chunk *)((char *)(current_chunk) + element_size);
        current_chunk = current_chunk->next;
    }

    current_chunk->next = NULL;

    return 0;
}

void pool_allocator_free(PoolAllocator *pool_allocator)
{
    free(pool_allocator->pool_start);
    pool_allocator->pool_start = NULL;
    pool_allocator->next_free_element = NULL;
}

void *pool_allocator_get_new_element(PoolAllocator *pool_allocator)
{
    if (pool_allocator->next_free_element == NULL) {
        errno = MAXIMUM_NUMBER_OF_ELEMENTS_ALLOCATED;
        return NULL;
    }

    Chunk *new_element = pool_allocator->next_free_element;
    pool_allocator->next_free_element = new_element->next;

    return new_element;
}

void pool_allocator_free_element(PoolAllocator *pool_allocator, void *element_to_free)
{
    Chunk *free_chunk = (Chunk *)(element_to_free);
    free_chunk->next = pool_allocator->next_free_element;
    pool_allocator->next_free_element = free_chunk;
}