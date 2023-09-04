#include <unity.h>

#include "pool_allocator/pool_allocator.h"

typedef struct
{
    int x;
    int y;
    double z;
    char c;
} dummy_struct_t;

const size_t non_primitive_type_size = sizeof(dummy_struct_t);
const size_t primitive_type_size = sizeof(double);
const size_t dummy_number_of_elements = 3;

void setUp() {}
void tearDown() {}

void test_pool_allocator_should_be_created_with_proper_data_for_primitive_type()
{
    pool_allocator_t alloc = pool_allocator_alloc(primitive_type_size, dummy_number_of_elements);

    TEST_ASSERT_EQUAL(primitive_type_size, pool_allocator_get_element_size(alloc));
    TEST_ASSERT_EQUAL(dummy_number_of_elements, pool_allocator_get_max_number_of_allocations(alloc));
    TEST_ASSERT_EQUAL(0, pool_allocator_get_memory_used(alloc));
    TEST_ASSERT_EQUAL(0, pool_allocator_get_number_of_allocations(alloc));
}

void test_pool_allocator_should_be_created_with_proper_data_for_non_primitive_type()
{
    pool_allocator_t alloc = pool_allocator_alloc(non_primitive_type_size, dummy_number_of_elements);

    TEST_ASSERT_EQUAL(non_primitive_type_size, pool_allocator_get_element_size(alloc));
    TEST_ASSERT_EQUAL(dummy_number_of_elements, pool_allocator_get_max_number_of_allocations(alloc));
    TEST_ASSERT_EQUAL(0, pool_allocator_get_memory_used(alloc));
    TEST_ASSERT_EQUAL(0, pool_allocator_get_number_of_allocations(alloc));
}

void test_pool_allocator_should_fail_when_type_size_smaller_that_void_pointer()
{
    const size_t element_size = sizeof(int);
    TEST_ASSERT_FALSE(pool_allocator_alloc(element_size, dummy_number_of_elements));
}

void test_pool_allocator_should_increment_number_of_allocation_and_memory_used_when_new_element_allocated()
{
    pool_allocator_t alloc = pool_allocator_alloc(primitive_type_size, dummy_number_of_elements);

    pool_allocator_alloc_element(alloc);
    TEST_ASSERT_EQUAL(1, pool_allocator_get_number_of_allocations(alloc));
    TEST_ASSERT_EQUAL(primitive_type_size, pool_allocator_get_memory_used(alloc));

    pool_allocator_alloc_element(alloc);
    TEST_ASSERT_EQUAL(2, pool_allocator_get_number_of_allocations(alloc));
    TEST_ASSERT_EQUAL(2 * primitive_type_size, pool_allocator_get_memory_used(alloc));

    pool_allocator_alloc_element(alloc);
    TEST_ASSERT_EQUAL(3, pool_allocator_get_number_of_allocations(alloc));
    TEST_ASSERT_EQUAL(3 * primitive_type_size, pool_allocator_get_memory_used(alloc));
}

void test_pool_allocator_should_decrement_number_of_allocations_and_memory_used_when_element_freed()
{
    pool_allocator_t alloc = pool_allocator_alloc(primitive_type_size, dummy_number_of_elements);

    element_t first_allocation  = pool_allocator_alloc_element(alloc);
    element_t second_allocation = pool_allocator_alloc_element(alloc);
    element_t third_allocation  = pool_allocator_alloc_element(alloc);

    pool_allocator_free_element(alloc, third_allocation);
    TEST_ASSERT_EQUAL(2, pool_allocator_get_number_of_allocations(alloc));
    TEST_ASSERT_EQUAL(2 * primitive_type_size, pool_allocator_get_memory_used(alloc));
    
    pool_allocator_free_element(alloc, second_allocation);
    TEST_ASSERT_EQUAL(1, pool_allocator_get_number_of_allocations(alloc));
    TEST_ASSERT_EQUAL(1 * primitive_type_size, pool_allocator_get_memory_used(alloc));
    
    pool_allocator_free_element(alloc, first_allocation);
    TEST_ASSERT_EQUAL(0, pool_allocator_get_number_of_allocations(alloc));
    TEST_ASSERT_EQUAL(0 * primitive_type_size, pool_allocator_get_memory_used(alloc));
}

void test_pool_allocator_should_allocate_multiple_elments_with_proper_address()
{
    pool_allocator_t alloc = pool_allocator_alloc(primitive_type_size, dummy_number_of_elements);

    double *starting_address = pool_allocator_alloc_element(alloc);
    TEST_ASSERT_TRUE(starting_address);

    double *next_element = pool_allocator_alloc_element(alloc);
    TEST_ASSERT_EQUAL_MEMORY(starting_address + 1, next_element, primitive_type_size);

    next_element = pool_allocator_alloc_element(alloc);
    TEST_ASSERT_EQUAL_MEMORY(starting_address + 2, next_element, primitive_type_size);
}

void test_pool_allocator_should_allocate_next_element_on_same_memory_address_as_last_deleted_element()
{
    pool_allocator_t alloc = pool_allocator_alloc(primitive_type_size, dummy_number_of_elements);

    double *first_allocation = pool_allocator_alloc_element(alloc);
    TEST_ASSERT_TRUE(first_allocation);

    pool_allocator_free_element(alloc, (void*)first_allocation);
    double *new_allocation = pool_allocator_alloc_element(alloc);
    
    TEST_ASSERT_EQUAL_MEMORY(first_allocation, new_allocation, primitive_type_size);
}

void test_pool_allocator_should_not_allow_allocation_after_reaching_predefined_number_of_elements()
{
    pool_allocator_t alloc = pool_allocator_alloc(primitive_type_size, dummy_number_of_elements);

    TEST_ASSERT_TRUE(pool_allocator_alloc_element(alloc));
    TEST_ASSERT_TRUE(pool_allocator_alloc_element(alloc));
    TEST_ASSERT_TRUE(pool_allocator_alloc_element(alloc));

    TEST_ASSERT_FALSE(pool_allocator_alloc_element(alloc));
    TEST_ASSERT_EQUAL(3, pool_allocator_get_number_of_allocations(alloc));
    TEST_ASSERT_EQUAL(primitive_type_size * 3, pool_allocator_get_memory_used(alloc));
}

void test_pool_allocator_should_allocate_and_free_elements_in_random_order()
{
    pool_allocator_t alloc = pool_allocator_alloc(primitive_type_size, dummy_number_of_elements);

    element_t first_allocation = pool_allocator_alloc_element(alloc);
    element_t second_allocation = pool_allocator_alloc_element(alloc);

    TEST_ASSERT_TRUE(first_allocation);
    TEST_ASSERT_TRUE(second_allocation);

    pool_allocator_free_element(alloc, first_allocation);

    element_t third_allocation = pool_allocator_alloc_element(alloc);
    first_allocation = pool_allocator_alloc_element(alloc);

    TEST_ASSERT_TRUE(third_allocation);
    TEST_ASSERT_TRUE(first_allocation);

    pool_allocator_free_element(alloc, second_allocation);
    pool_allocator_free_element(alloc, third_allocation);
    pool_allocator_free_element(alloc, first_allocation);

    TEST_ASSERT_EQUAL(0, pool_allocator_get_number_of_allocations(alloc));
    TEST_ASSERT_EQUAL(0, pool_allocator_get_memory_used(alloc));
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_pool_allocator_should_be_created_with_proper_data_for_primitive_type);
    RUN_TEST(test_pool_allocator_should_be_created_with_proper_data_for_non_primitive_type);
    RUN_TEST(test_pool_allocator_should_fail_when_type_size_smaller_that_void_pointer);
    RUN_TEST(test_pool_allocator_should_increment_number_of_allocation_and_memory_used_when_new_element_allocated);
    RUN_TEST(test_pool_allocator_should_decrement_number_of_allocations_and_memory_used_when_element_freed);
    RUN_TEST(test_pool_allocator_should_allocate_multiple_elments_with_proper_address);
    RUN_TEST(test_pool_allocator_should_allocate_next_element_on_same_memory_address_as_last_deleted_element);
    RUN_TEST(test_pool_allocator_should_not_allow_allocation_after_reaching_predefined_number_of_elements);
    RUN_TEST(test_pool_allocator_should_allocate_and_free_elements_in_random_order);

    return UNITY_END();
}