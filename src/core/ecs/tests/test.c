#include <unity.h>

#include "core/ecs/ecs_manager.h"

void setUp() {}
void tearDown() {}

void test_dummy()
{
    TEST_ASSERT_EQUAL(1, 1);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_dummy);

    return UNITY_END();
}