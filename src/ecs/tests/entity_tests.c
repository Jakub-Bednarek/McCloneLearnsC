#include <unity.h>

#include "ecs/entity.h"

#define TEST_MAX_NUMBER_OF_ENTITIES 3

#define FIRST_ENTITY_ID 0
#define SECOND_ENTITY_ID 1
#define THIRD_ENTITY_ID 2
#define INVALID_ENTITY_ID 50

#define FIRST_COMPONENT_SIGNATURE 0b0001
#define SECOND_COMPONENT_SIGNATURE 0b0010
#define THIRD_COMPONENT_SIGNATURE 0b0100
#define FIRST_AND_SECOND_COMPONENT_SIGNATURE 0b0011
#define ALL_COMPONENTS_SIGNATURE 0b0111

#define EMPTY_ENTITY_SIGNATURE 0b0000
#define INVALID_SIGNATURE 0b1111

void setUp() {}
void tearDown() {}

void test_entity_manager_should_not_be_allocated_when_invalid_arguments_pased()
{
    entity_manager_t *entity_manager = NULL;

    TEST_ASSERT_EQUAL(INVALID_MAX_ENTITIES_VALUE, entity_manager_alloc(&entity_manager, -1));
    TEST_ASSERT_FALSE(entity_manager);
}

void test_entity_manager_should_allocate_max_number_of_entities()
{
    entity_manager_t *entity_manager = NULL;
    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    entity_id_t allocated_id = INVALID_ENTITY_ID;
    TEST_ASSERT_EQUAL(entity_manager_get_next_free_id(entity_manager, &allocated_id), ENTITY_OK);
    TEST_ASSERT_EQUAL(FIRST_ENTITY_ID, allocated_id);
    TEST_ASSERT_EQUAL(entity_manager_get_next_free_id(entity_manager, &allocated_id), ENTITY_OK);
    TEST_ASSERT_EQUAL(SECOND_ENTITY_ID, allocated_id);
    TEST_ASSERT_EQUAL(entity_manager_get_next_free_id(entity_manager, &allocated_id), ENTITY_OK);
    TEST_ASSERT_EQUAL(THIRD_ENTITY_ID, allocated_id);

    entity_manager_free(entity_manager);
}

void test_entity_manager_should_increment_number_of_allocations_after_retrieving_next_id()
{
    entity_manager_t *entity_manager = NULL;
    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    entity_id_t allocated_id = INVALID_ENTITY_ID;
    TEST_ASSERT_EQUAL(0, entity_manager_get_number_of_allocated_entities(entity_manager));

    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    TEST_ASSERT_EQUAL(1, entity_manager_get_number_of_allocated_entities(entity_manager));

    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    TEST_ASSERT_EQUAL(2, entity_manager_get_number_of_allocated_entities(entity_manager));

    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    TEST_ASSERT_EQUAL(3, entity_manager_get_number_of_allocated_entities(entity_manager));

    entity_manager_free(entity_manager);
}

void test_entity_manager_should_not_allow_entity_allocation_after_reaching_max_number()
{
    entity_manager_t *entity_manager = NULL;
    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    entity_id_t allocated_id = INVALID_ENTITY_ID;
    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_get_next_free_id(entity_manager, &allocated_id);

    TEST_ASSERT_EQUAL(MAX_ENTITIES_ALLOCATED, entity_manager_get_next_free_id(entity_manager, &allocated_id));

    entity_manager_free(entity_manager);
}

void test_entity_manager_should_allocate_same_entity_id_after_free()
{
    entity_manager_t *entity_manager = NULL;
    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    entity_id_t allocated_id = INVALID_ENTITY_ID;
    entity_manager_get_next_free_id(entity_manager, &allocated_id);

    entity_manager_free_entity(entity_manager, allocated_id);
    allocated_id = INVALID_ENTITY_ID;
    TEST_ASSERT_EQUAL(entity_manager_get_next_free_id(entity_manager, &allocated_id), ENTITY_OK);
    TEST_ASSERT_EQUAL(FIRST_ENTITY_ID, allocated_id);

    entity_manager_get_next_free_id(entity_manager, &allocated_id);

    entity_manager_free_entity(entity_manager, allocated_id);
    allocated_id = INVALID_ENTITY_ID;
    TEST_ASSERT_EQUAL(entity_manager_get_next_free_id(entity_manager, &allocated_id), ENTITY_OK);
    TEST_ASSERT_EQUAL(SECOND_ENTITY_ID, allocated_id);

    entity_manager_get_next_free_id(entity_manager, &allocated_id);

    entity_manager_free_entity(entity_manager, allocated_id);
    allocated_id = INVALID_ENTITY_ID;
    TEST_ASSERT_EQUAL(entity_manager_get_next_free_id(entity_manager, &allocated_id), ENTITY_OK);
    TEST_ASSERT_EQUAL(THIRD_ENTITY_ID, allocated_id);

    entity_manager_free(entity_manager);
}

void test_entity_manager_should_reset_entity_signature_after_allocating_same_id()
{
    entity_manager_t *entity_manager = NULL;
    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    entity_id_t old_allocated_id = INVALID_ENTITY_ID;
    entity_manager_get_next_free_id(entity_manager, &old_allocated_id);
    entity_manager_add_component_signature_to_entity(entity_manager, old_allocated_id, FIRST_COMPONENT_SIGNATURE);
    entity_manager_free_entity(entity_manager, old_allocated_id);

    entity_id_t new_allocated_id = INVALID_ENTITY_ID;
    entity_manager_get_next_free_id(entity_manager, &new_allocated_id);

    TEST_ASSERT_EQUAL(old_allocated_id, new_allocated_id);

    signature_t new_signature = INVALID_SIGNATURE;
    entity_manager_get_entity_signature(entity_manager, new_allocated_id, &new_signature);

    TEST_ASSERT_EQUAL(EMPTY_ENTITY_SIGNATURE, new_signature);

    entity_manager_free(entity_manager);
}

void test_entity_manager_should_return_error_when_free_called_with_invalid_entity_id()
{
    entity_manager_t *entity_manager = NULL;

    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    entity_id_t allocated_id = INVALID_ENTITY_ID;
    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_get_next_free_id(entity_manager, &allocated_id);

    TEST_ASSERT_EQUAL(ENTITY_OUT_OF_RANGE, entity_manager_free_entity(entity_manager, INVALID_ENTITY_ID));
    TEST_ASSERT_EQUAL(ENTITY_NOT_FOUND, entity_manager_free_entity(entity_manager, THIRD_ENTITY_ID));

    entity_manager_free(entity_manager);
}

void test_entity_manager_should_return_error_when_free_called_on_alread_freed_entity()
{
    entity_manager_t *entity_manager = NULL;

    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    entity_id_t allocated_id = INVALID_ENTITY_ID;
    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_free_entity(entity_manager, FIRST_ENTITY_ID);

    TEST_ASSERT_EQUAL(ENTITY_NOT_FOUND, entity_manager_free_entity(entity_manager, FIRST_ENTITY_ID));

    entity_manager_free(entity_manager);
}

void test_entity_manager_should_decrement_number_of_allocations_after_freeing_entity_id()
{
    entity_manager_t *entity_manager = NULL;
    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    entity_id_t allocated_id = INVALID_ENTITY_ID;

    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_get_next_free_id(entity_manager, &allocated_id);

    entity_manager_free_entity(entity_manager, FIRST_ENTITY_ID);
    TEST_ASSERT_EQUAL(2, entity_manager_get_number_of_allocated_entities(entity_manager));

    entity_manager_free_entity(entity_manager, THIRD_ENTITY_ID);
    TEST_ASSERT_EQUAL(1, entity_manager_get_number_of_allocated_entities(entity_manager));

    entity_manager_free_entity(entity_manager, SECOND_ENTITY_ID);
    TEST_ASSERT_EQUAL(0, entity_manager_get_number_of_allocated_entities(entity_manager));

    entity_manager_free(entity_manager);
}

void test_entity_manager_should_initiate_signature_to_zero_when_entity_created()
{
    entity_manager_t *entity_manager = NULL;
    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    entity_id_t allocated_id = INVALID_ENTITY_ID;
    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_get_next_free_id(entity_manager, &allocated_id);

    signature_t retrieved_signature = INVALID_SIGNATURE;
    TEST_ASSERT_EQUAL(EMPTY_ENTITY_SIGNATURE, entity_manager_get_entity_signature(entity_manager, FIRST_ENTITY_ID, &retrieved_signature));
    TEST_ASSERT_EQUAL(EMPTY_ENTITY_SIGNATURE, entity_manager_get_entity_signature(entity_manager, SECOND_ENTITY_ID, &retrieved_signature));
    TEST_ASSERT_EQUAL(EMPTY_ENTITY_SIGNATURE, entity_manager_get_entity_signature(entity_manager, THIRD_ENTITY_ID, &retrieved_signature));

    entity_manager_free(entity_manager);
}

void test_entity_manager_should_add_signature_to_entity_id()
{
    entity_manager_t *entity_manager = NULL;
    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    entity_id_t allocated_id = INVALID_ENTITY_ID;
    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_get_next_free_id(entity_manager, &allocated_id);

    signature_t retrieved_signature = INVALID_SIGNATURE;

    entity_manager_add_component_signature_to_entity(entity_manager, FIRST_ENTITY_ID, FIRST_COMPONENT_SIGNATURE);
    entity_manager_get_entity_signature(entity_manager, FIRST_ENTITY_ID, &retrieved_signature);
    TEST_ASSERT_EQUAL(FIRST_COMPONENT_SIGNATURE, retrieved_signature);

    retrieved_signature = INVALID_SIGNATURE;
    entity_manager_add_component_signature_to_entity(entity_manager, FIRST_ENTITY_ID, SECOND_COMPONENT_SIGNATURE);
    entity_manager_get_entity_signature(entity_manager, FIRST_ENTITY_ID, &retrieved_signature);
    TEST_ASSERT_EQUAL(FIRST_AND_SECOND_COMPONENT_SIGNATURE, retrieved_signature);

    retrieved_signature = INVALID_SIGNATURE;
    entity_manager_add_component_signature_to_entity(entity_manager, FIRST_ENTITY_ID, THIRD_COMPONENT_SIGNATURE);
    entity_manager_get_entity_signature(entity_manager, FIRST_ENTITY_ID, &retrieved_signature);
    TEST_ASSERT_EQUAL(ALL_COMPONENTS_SIGNATURE, retrieved_signature);

    retrieved_signature = INVALID_SIGNATURE;
    entity_manager_get_entity_signature(entity_manager, SECOND_ENTITY_ID, &retrieved_signature);
    TEST_ASSERT_EQUAL(EMPTY_ENTITY_SIGNATURE, retrieved_signature);

    retrieved_signature = INVALID_SIGNATURE;
    entity_manager_get_entity_signature(entity_manager, THIRD_ENTITY_ID, &retrieved_signature);
    TEST_ASSERT_EQUAL(EMPTY_ENTITY_SIGNATURE, retrieved_signature);

    entity_manager_free(entity_manager);
}

void test_entity_manager_should_return_error_when_signature_func_called_with_invalid_entity_id()
{
    entity_manager_t *entity_manager = NULL;
    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    TEST_ASSERT_EQUAL(ENTITY_OUT_OF_RANGE, entity_manager_add_component_signature_to_entity(entity_manager, INVALID_ENTITY_ID, INVALID_SIGNATURE));
    TEST_ASSERT_EQUAL(ENTITY_OUT_OF_RANGE, entity_manager_remove_component_signature_from_entity(entity_manager, INVALID_ENTITY_ID, INVALID_SIGNATURE));

    signature_t retrieved_signature = INVALID_SIGNATURE;
    TEST_ASSERT_EQUAL(ENTITY_OUT_OF_RANGE, entity_manager_get_entity_signature(entity_manager, INVALID_ENTITY_ID, &retrieved_signature));

    entity_manager_free(entity_manager);
}

void test_entity_manager_should_remove_existing_signature_from_entity()
{
    entity_manager_t *entity_manager = NULL;
    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    entity_id_t allocated_id = INVALID_ENTITY_ID;
    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_get_next_free_id(entity_manager, &allocated_id);
    entity_manager_get_next_free_id(entity_manager, &allocated_id);

    entity_manager_add_component_signature_to_entity(entity_manager, FIRST_ENTITY_ID, ALL_COMPONENTS_SIGNATURE);
    entity_manager_remove_component_signature_from_entity(entity_manager, FIRST_ENTITY_ID, THIRD_COMPONENT_SIGNATURE);

    signature_t retrieved_signature = INVALID_SIGNATURE;
    TEST_ASSERT_EQUAL(ENTITY_OK, entity_manager_get_entity_signature(entity_manager, FIRST_ENTITY_ID, &retrieved_signature));
    TEST_ASSERT_EQUAL(FIRST_AND_SECOND_COMPONENT_SIGNATURE, retrieved_signature);

    retrieved_signature = INVALID_SIGNATURE;
    entity_manager_remove_component_signature_from_entity(entity_manager, FIRST_ENTITY_ID, FIRST_COMPONENT_SIGNATURE);
    entity_manager_get_entity_signature(entity_manager, FIRST_ENTITY_ID, &retrieved_signature);
    TEST_ASSERT_EQUAL(SECOND_COMPONENT_SIGNATURE, retrieved_signature);

    retrieved_signature = INVALID_SIGNATURE;
    entity_manager_remove_component_signature_from_entity(entity_manager, FIRST_ENTITY_ID, SECOND_COMPONENT_SIGNATURE);
    entity_manager_get_entity_signature(entity_manager, FIRST_ENTITY_ID, &retrieved_signature);
    TEST_ASSERT_EQUAL(EMPTY_ENTITY_SIGNATURE, retrieved_signature);

    retrieved_signature = INVALID_SIGNATURE;
    entity_manager_get_entity_signature(entity_manager, SECOND_ENTITY_ID, &retrieved_signature);
    TEST_ASSERT_EQUAL(EMPTY_ENTITY_SIGNATURE, retrieved_signature);

    retrieved_signature = INVALID_SIGNATURE;
    entity_manager_get_entity_signature(entity_manager, THIRD_ENTITY_ID, &retrieved_signature);
    TEST_ASSERT_EQUAL(EMPTY_ENTITY_SIGNATURE, retrieved_signature);

    entity_manager_free(entity_manager);
}

void test_entity_manager_should_do_nothing_when_free_of_signature_that_is_not_present_for_entity()
{
    entity_manager_t *entity_manager = NULL;
    entity_manager_alloc(&entity_manager, TEST_MAX_NUMBER_OF_ENTITIES);

    entity_id_t allocated_id = INVALID_ENTITY_ID;
    entity_manager_get_next_free_id(entity_manager, &allocated_id);

    entity_manager_add_component_signature_to_entity(entity_manager, FIRST_ENTITY_ID, FIRST_AND_SECOND_COMPONENT_SIGNATURE);
    entity_manager_remove_component_signature_from_entity(entity_manager, FIRST_ENTITY_ID, THIRD_COMPONENT_SIGNATURE);

    signature_t retrieved_signature = INVALID_SIGNATURE;
    TEST_ASSERT_EQUAL(ENTITY_OK, entity_manager_get_entity_signature(entity_manager, FIRST_ENTITY_ID, &retrieved_signature));
    TEST_ASSERT_EQUAL(FIRST_AND_SECOND_COMPONENT_SIGNATURE, retrieved_signature);

    entity_manager_free(entity_manager);
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_entity_manager_should_not_be_allocated_when_invalid_arguments_pased);
    RUN_TEST(test_entity_manager_should_allocate_max_number_of_entities);
    RUN_TEST(test_entity_manager_should_increment_number_of_allocations_after_retrieving_next_id);
    RUN_TEST(test_entity_manager_should_not_allow_entity_allocation_after_reaching_max_number);
    RUN_TEST(test_entity_manager_should_allocate_same_entity_id_after_free);
    RUN_TEST(test_entity_manager_should_reset_entity_signature_after_allocating_same_id);
    RUN_TEST(test_entity_manager_should_return_error_when_free_called_with_invalid_entity_id);
    RUN_TEST(test_entity_manager_should_return_error_when_free_called_on_alread_freed_entity);
    RUN_TEST(test_entity_manager_should_decrement_number_of_allocations_after_freeing_entity_id);
    RUN_TEST(test_entity_manager_should_initiate_signature_to_zero_when_entity_created);
    RUN_TEST(test_entity_manager_should_add_signature_to_entity_id);
    RUN_TEST(test_entity_manager_should_return_error_when_signature_func_called_with_invalid_entity_id);
    RUN_TEST(test_entity_manager_should_remove_existing_signature_from_entity);
    RUN_TEST(test_entity_manager_should_do_nothing_when_free_of_signature_that_is_not_present_for_entity);

    return UNITY_END();
}