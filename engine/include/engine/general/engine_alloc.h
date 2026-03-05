
#pragma once

#include <stddef.h>

void* engine_alloc(size_t size);
void engine_free(void* ptr);
void* engine_realloc(void* ptr, size_t new_size);
