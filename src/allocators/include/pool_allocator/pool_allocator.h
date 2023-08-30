#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdio.h>
#include <stdint.h>

#define ELEMENT_SIZE_TOO_SMALL 1
#define POOL_MEMORY_ALLOCATION_FAILURE 2
#define MAXIMUM_NUMBER_OF_ELEMENTS_ALLOCATED 3

typedef void* pool_allocator_t;
typedef void* element_t;

extern pool_allocator_t  pool_allocator_create(size_t element_size, size_t number_of_elements);
extern void              pool_allocator_free(pool_allocator_t allocator); 
extern element_t         pool_allocator_alloc(pool_allocator_t allocator);
extern void              pool_allocator_free_element(pool_allocator_t allocator, element_t element_to_free);
extern const size_t      pool_allocator_get_element_size(pool_allocator_t allocator);
extern const size_t      pool_allocator_get_number_of_allocations(pool_allocator_t allocator);
extern const size_t      pool_allocator_get_memory_used(pool_allocator_t allocator);
extern const size_t      pool_allocator_get_max_number_of_allocations(pool_allocator_t allocator);

#endif // ALLOCATOR_H