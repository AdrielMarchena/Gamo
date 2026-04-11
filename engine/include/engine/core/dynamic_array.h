#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct DynamicArray
{
    void* data;
    uint32_t count;
    uint32_t capacity;
    size_t stride;
} DynamicArray;

void dynamic_array_init(DynamicArray* array, size_t stride);
void dynamic_array_destroy(DynamicArray* array);

void dynamic_array_push_back(DynamicArray* array, void* element);
void dynamic_array_clear(DynamicArray* array);
void dynamic_array_erase(DynamicArray* array, uint32_t index);

void dynamic_array_resize(DynamicArray* array, uint32_t new_capacity);
void dynamic_array_reserve(DynamicArray* array, uint32_t new_capacity);
void dynamic_array_shrink_to_fit(DynamicArray* array);

void* dynamic_array_pop_back(DynamicArray* array);
void* dynamic_array_get(const DynamicArray* array, uint32_t index);
