#include <unity.h>

#include "data_structures/hash_map.h"

#include <stdlib.h>
#include <stdbool.h>

void setUp() {}
void tearDown() {}

const char* dummy_key = "dummy_key";

typedef struct {
    int id;
    const char* name;
    double offset; 
} dummy_struct_t;

void test_hash_map_should_add_one_element()
{
    map_t map = hash_map_create();

    TEST_ASSERT_TRUE(map != NULL);

    dummy_struct_t* dummy = calloc(1, sizeof(dummy_struct_t));
    if(dummy == NULL) {
        TEST_ASSERT_TRUE(false);
    }

    hash_map_add(map, dummy_key, (void*)dummy);

    TEST_ASSERT_EQUAL(hash_map_size(map), 1);

    hash_map_free(map);
    free(dummy);
}

void test_hash_map_should_not_add_multiple_elements_with_same_key()
{
    map_t map = hash_map_create();

    TEST_ASSERT_TRUE(map != NULL);

    dummy_struct_t* dummy = calloc(1, sizeof(dummy_struct_t));
    if(dummy = NULL) {
        TEST_ASSERT_TRUE(false);
    }

    hash_map_add(map, dummy_key, (void*)dummy);
    hash_map_add(map, dummy_key, (void*)dummy);
    hash_map_add(map, dummy_key, (void*)dummy);

    TEST_ASSERT_EQUAL(hash_map_size(map), 1);
    
    hash_map_free(map);
    free(dummy);
}

void test_hash_map_should_add_multiple_elements_with_different_keys()
{
    map_t map = hash_map_create();

    TEST_ASSERT_TRUE(map != NULL);

    dummy_struct_t* dummy = calloc(1, sizeof(dummy_struct_t));
    if(dummy = NULL) {
        TEST_ASSERT_TRUE(false);
    }

    hash_map_add(map, "key_1", (void*)dummy);
    hash_map_add(map, "key_2", (void*)dummy);
    hash_map_add(map, "key_3", (void*)dummy);

    TEST_ASSERT_EQUAL(hash_map_size(map), 3);

    hash_map_free(map);
    free(dummy);
}

void test_hash_map_should_delete_one_element()
{
    map_t map = hash_map_create();

    TEST_ASSERT_TRUE(map != NULL);

    dummy_struct_t* dummy = calloc(1, sizeof(dummy_struct_t));
    if(dummy == NULL) {
        TEST_ASSERT_TRUE(false);
    }

    hash_map_add(map, dummy_key, (void*)(dummy));
    hash_map_remove(map, dummy_key);

    TEST_ASSERT_EQUAL(hash_map_size(map), 0);

    hash_map_free(map);
    free(dummy);
}

void test_hash_map_should_delete_first_element_when_multiple_keys_present()
{
    map_t map = hash_map_create();

    TEST_ASSERT_TRUE(map != NULL);

    dummy_struct_t* dummy = calloc(1, sizeof(dummy_struct_t));
    if(dummy = NULL) {
        TEST_ASSERT_TRUE(false);
    }

    hash_map_add(map, "key_1", (void*)dummy);
    hash_map_add(map, "key_2", (void*)dummy);
    hash_map_add(map, "key_3", (void*)dummy);
    hash_map_remove(map, "key_1");

    TEST_ASSERT_EQUAL(hash_map_size(map), 2);

    hash_map_free(map);
    free(dummy);
}

void test_hash_map_should_delete_last_element_when_multiple_keys_present()
{
    map_t map = hash_map_create();

    TEST_ASSERT_TRUE(map != NULL);

    dummy_struct_t* dummy = calloc(1, sizeof(dummy_struct_t));
    if(dummy = NULL) {
        TEST_ASSERT_TRUE(false);
    }

    hash_map_add(map, "key_1", (void*)dummy);
    hash_map_add(map, "key_2", (void*)dummy);
    hash_map_add(map, "key_3", (void*)dummy);
    hash_map_remove(map, "key_3");

    TEST_ASSERT_EQUAL(hash_map_size(map), 2);

    hash_map_free(map);
    free(dummy);
}

void test_hash_map_should_delete_middle_element_when_multiple_keys_present()
{
    map_t map = hash_map_create();

    TEST_ASSERT_TRUE(map != NULL);

    dummy_struct_t* dummy = calloc(1, sizeof(dummy_struct_t));
    if(dummy = NULL) {
        TEST_ASSERT_TRUE(false);
    }

    hash_map_add(map, "key_1", (void*)dummy);
    hash_map_add(map, "key_2", (void*)dummy);
    hash_map_add(map, "key_3", (void*)dummy);
    hash_map_remove(map, "key_3");

    TEST_ASSERT_EQUAL(hash_map_size(map), 2);

    hash_map_free(map);
    free(dummy);
}

void test_hash_map_should_get_first_element_when_multiple_keys_present()
{
    map_t map = hash_map_create();

    TEST_ASSERT_TRUE(map != NULL);

    dummy_struct_t* dummy = calloc(3, sizeof(dummy_struct_t));
    if(dummy = NULL) {
        TEST_ASSERT_TRUE(false);
    }

    hash_map_add(map, "key_1", (void*)&dummy[0]);
    hash_map_add(map, "key_2", (void*)&dummy[1]);
    hash_map_add(map, "key_3", (void*)&dummy[2]);

    dummy_struct_t* first_elem = NULL;
    hash_map_get(map, "key_1", &first_elem);

    TEST_ASSERT_EQUAL_PTR(first_elem, &dummy[0]);

    hash_map_free(map);
    free(dummy);
}

void test_hash_map_should_get_middle_element_when_multiple_keys_present()
{
    map_t map = hash_map_create();

    TEST_ASSERT_TRUE(map != NULL);

    dummy_struct_t* dummy = calloc(3, sizeof(dummy_struct_t));
    if(dummy = NULL) {
        TEST_ASSERT_TRUE(false);
    }

    hash_map_add(map, "key_1", (void*)&dummy[0]);
    hash_map_add(map, "key_2", (void*)&dummy[1]);
    hash_map_add(map, "key_3", (void*)&dummy[2]);

    dummy_struct_t* middle_elem = NULL;
    hash_map_get(map, "key_2", &middle_elem);

    TEST_ASSERT_EQUAL_PTR(middle_elem, &dummy[1]);

    hash_map_free(map);
    free(dummy);
}

void test_hash_map_should_get_last_element_when_multiple_keys_present()
{
    map_t map = hash_map_create();

    TEST_ASSERT_TRUE(map != NULL);

    dummy_struct_t* dummy = calloc(3, sizeof(dummy_struct_t));
    if(dummy = NULL) {
        TEST_ASSERT_TRUE(false);
    }

    hash_map_add(map, "key_1", (void*)&dummy[0]);
    hash_map_add(map, "key_2", (void*)&dummy[1]);
    hash_map_add(map, "key_3", (void*)&dummy[2]);

    dummy_struct_t* last_elem = NULL;
    hash_map_get(map, "key_3", &last_elem);

    TEST_ASSERT_EQUAL_PTR(last_elem, &dummy[2]);

    hash_map_free(map);
    free(dummy);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_hash_map_should_add_one_element);
    RUN_TEST(test_hash_map_should_delete_one_element);
    RUN_TEST(test_hash_map_should_not_add_multiple_elements_with_same_key);
    RUN_TEST(test_hash_map_should_add_multiple_elements_with_different_keys);
    RUN_TEST(test_hash_map_should_delete_first_element_when_multiple_keys_present);
    RUN_TEST(test_hash_map_should_delete_last_element_when_multiple_keys_present);
    RUN_TEST(test_hash_map_should_delete_middle_element_when_multiple_keys_present);
    RUN_TEST(test_hash_map_should_get_first_element_when_multiple_keys_present);
    RUN_TEST(test_hash_map_should_get_middle_element_when_multiple_keys_present);
    RUN_TEST(test_hash_map_should_get_last_element_when_multiple_keys_present);

    return UNITY_END();
}