/*
 * dynamic_array_test.c
 * Gamo
 *
 * Copyright (c) 2026 Adriel Marchena
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 *
 * Description:
 *   Behavioral tests for the dynamic array module.
 */

#include <engine/core/dynamic_array.h>

#include <stdbool.h>
#include <stdio.h>

typedef struct TestCase
{
    const char* name;
    bool (*run)(void);
} TestCase;

#define EXPECT_TRUE(expr)                                                                          \
    do                                                                                             \
    {                                                                                              \
        if (!(expr))                                                                               \
        {                                                                                          \
            fprintf(stderr, "Expectation failed: %s (%s:%d)\n", #expr, __FILE__, __LINE__);        \
            return false;                                                                          \
        }                                                                                          \
    } while (0)

static bool test_init_and_resize(void)
{
    DynamicArray array;
    dynamic_array_init(&array, sizeof(int));

    EXPECT_TRUE(array.data == NULL);
    EXPECT_TRUE(array.count == 0);
    EXPECT_TRUE(array.capacity == 0);
    EXPECT_TRUE(array.stride == sizeof(int));

    dynamic_array_resize(&array, 4);

    EXPECT_TRUE(array.data != NULL);
    EXPECT_TRUE(array.count == 0);
    EXPECT_TRUE(array.capacity == 4);

    dynamic_array_destroy(&array);
    return true;
}

static bool test_push_get_and_growth(void)
{
    DynamicArray array;
    dynamic_array_init(&array, sizeof(int));
    dynamic_array_resize(&array, 4);

    for (int i = 0; i < 5; ++i)
    {
        int value = i + 10;
        dynamic_array_push_back(&array, &value);
    }

    EXPECT_TRUE(array.count == 5);
    EXPECT_TRUE(array.capacity == 8);

    for (uint32_t i = 0; i < array.count; ++i)
    {
        int* value = dynamic_array_get(&array, i);
        EXPECT_TRUE(*value == (int)i + 10);
    }

    dynamic_array_destroy(&array);
    return true;
}

static bool test_erase_shifts_elements(void)
{
    DynamicArray array;
    dynamic_array_init(&array, sizeof(int));
    dynamic_array_resize(&array, 4);

    int values[] = {1, 2, 3, 4};
    for (uint32_t i = 0; i < 4; ++i)
    {
        dynamic_array_push_back(&array, &values[i]);
    }

    dynamic_array_erase(&array, 1);

    EXPECT_TRUE(array.count == 3);
    EXPECT_TRUE(*(int*)dynamic_array_get(&array, 0) == 1);
    EXPECT_TRUE(*(int*)dynamic_array_get(&array, 1) == 3);
    EXPECT_TRUE(*(int*)dynamic_array_get(&array, 2) == 4);

    dynamic_array_destroy(&array);
    return true;
}

static bool test_pop_back_returns_last_element(void)
{
    DynamicArray array;
    dynamic_array_init(&array, sizeof(int));
    dynamic_array_resize(&array, 4);

    int first = 7;
    int second = 9;
    dynamic_array_push_back(&array, &first);
    dynamic_array_push_back(&array, &second);

    int* last = dynamic_array_pop_back(&array);

    EXPECT_TRUE(last != NULL);
    EXPECT_TRUE(*last == 9);
    EXPECT_TRUE(array.count == 1);
    EXPECT_TRUE(*(int*)dynamic_array_get(&array, 0) == 7);

    dynamic_array_destroy(&array);
    return true;
}

static bool test_clear_and_reserve(void)
{
    DynamicArray array;
    dynamic_array_init(&array, sizeof(int));
    dynamic_array_resize(&array, 2);

    int values[] = {5, 6};
    dynamic_array_push_back(&array, &values[0]);
    dynamic_array_push_back(&array, &values[1]);

    dynamic_array_clear(&array);
    EXPECT_TRUE(array.count == 0);

    dynamic_array_reserve(&array, 10);
    EXPECT_TRUE(array.capacity >= 10);

    dynamic_array_destroy(&array);
    return true;
}

int main(void)
{
    const TestCase tests[] = {
        {"init_and_resize", test_init_and_resize},
        {"push_get_and_growth", test_push_get_and_growth},
        {"erase_shifts_elements", test_erase_shifts_elements},
        {"pop_back_returns_last_element", test_pop_back_returns_last_element},
        {"clear_and_reserve", test_clear_and_reserve},
    };

    size_t passed = 0;
    size_t total = sizeof(tests) / sizeof(tests[0]);

    for (size_t i = 0; i < total; ++i)
    {
        bool ok = tests[i].run();
        printf("[%s] %s\n", ok ? "PASS" : "FAIL", tests[i].name);
        if (ok)
        {
            ++passed;
        }
    }

    printf("\nResult: %zu/%zu tests passed\n", passed, total);
    return (passed == total) ? 0 : 1;
}