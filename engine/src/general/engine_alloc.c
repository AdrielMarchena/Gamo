#include "engine/general/engine_alloc.h"
#include <stdlib.h>

void* engine_alloc(size_t size)
{
    return malloc(size);
}

void engine_free(void* ptr)
{
    free(ptr);
}

void* engine_realloc(void* ptr, size_t new_size)
{
    return realloc(ptr, new_size);
}
