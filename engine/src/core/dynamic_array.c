#include "engine/core/dynamic_array.h"

#include "engine/general/assert.h"
#include "engine/general/engine_alloc.h"
#include "engine/general/logger.h"

#include <string.h>

#define DYNAMIC_ARRAY_INITIAL_CAPACITY 4
#define DYNAMIC_ARRAY_GROWTH_INCREMENT 4

void dynamic_array_init(DynamicArray* array, size_t stride)
{
    ENGINE_ASSERT(stride > 0, "Stride must be greater than 0");

    array->data = NULL;
    array->count = 0;
    array->capacity = 0;
    array->stride = stride;
}

void dynamic_array_destroy(DynamicArray* array)
{
    ENGINE_ASSERT(array->data != NULL, "Array is not initialized or is already destroyed");

    engine_free(array->data);
    array->data = NULL;
    array->count = 0;
    array->capacity = 0;
}

void dynamic_array_push_back(DynamicArray* array, void* element)
{
    ENGINE_ASSERT(array->data != NULL, "Array is not initialized");
    ENGINE_ASSERT(element != NULL, "Element cannot be NULL");

    uint32_t new_count = array->count + 1;

    bool needs_resize = new_count >= array->capacity;

    if (needs_resize)
    {
        uint32_t new_capacity = (array->capacity == 0)
                                    ? DYNAMIC_ARRAY_INITIAL_CAPACITY
                                    : array->capacity + DYNAMIC_ARRAY_GROWTH_INCREMENT;
        dynamic_array_resize(array, new_capacity);
    }

    memcpy((char*)array->data + array->count * array->stride, element, array->stride);
    array->count = new_count;
}

void dynamic_array_clear(DynamicArray* array)
{
    ENGINE_ASSERT(array->data != NULL, "Array is not initialized");

    array->count = 0;
}

void dynamic_array_erase(DynamicArray* array, uint32_t index)
{
    ENGINE_ASSERT(array->data != NULL, "Array is not initialized");
    ENGINE_ASSERT(index < array->count, "Index out of bounds");

    if (index < array->count - 1)
    {
        memmove((char*)array->data + index * array->stride,
                (char*)array->data + (index + 1) * array->stride,
                (array->count - index - 1) * array->stride);
    }

    array->count--;
}

void dynamic_array_resize(DynamicArray* array, uint32_t new_capacity)
{
    ENGINE_ASSERT(array->data != NULL || new_capacity > 0,
                  "Cannot resize uninitialized array to zero capacity");
    ENGINE_ASSERT(new_capacity > 0, "New capacity must be greater than 0");

    if (new_capacity < array->count)
    {
        engine_log_debug("New capacity is less than current count. No resizing needed.");
        return;
    }

    if (new_capacity <= array->capacity)
    {
        engine_log_debug(
            "New capacity is less than or equal to current capacity. No resizing needed.");
        return;
    }

    void* new_data = engine_realloc(array->data, new_capacity * array->stride);
    if (new_data == NULL)
    {
        ENGINE_ASSERT(false, "Failed to allocate memory for dynamic array");
        return;
    }

    array->data = new_data;
    array->capacity = new_capacity;

    if (array->count > new_capacity)
    {
        array->count = new_capacity;
    }
}

void dynamic_array_reserve(DynamicArray* array, uint32_t new_capacity)
{
    ENGINE_ASSERT(array->data != NULL, "Array is not initialized");
    ENGINE_ASSERT(new_capacity > 0, "New capacity must be greater than 0");

    if (new_capacity > array->capacity)
    {
        dynamic_array_resize(array, new_capacity);
    }
}

void dynamic_array_shrink_to_fit(DynamicArray* array)
{
    ENGINE_ASSERT(array->data != NULL, "Array is not initialized");

    if (array->count < array->capacity)
    {
        dynamic_array_resize(array, array->count);
    }
}

void* dynamic_array_pop_back(DynamicArray* array)
{
    ENGINE_ASSERT(array->data != NULL, "Array is not initialized");
    ENGINE_ASSERT(array->count > 0, "Cannot pop from an empty array");

    void* element = (char*)array->data + (array->count - 1) * array->stride;
    array->count--;
    return element;
}

void* dynamic_array_get(const DynamicArray* array, uint32_t index)
{
    ENGINE_ASSERT(array->data != NULL, "Array is not initialized");
    ENGINE_ASSERT(index < array->count, "Index out of bounds");

    return (char*)array->data + index * array->stride;
}
