#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct ElementArrayBuffer
{
    uint32_t id;
    uint32_t type;
    size_t size;
} ElementArrayBuffer;

ElementArrayBuffer engine_gl_element_array_buffer_create(const void* data, size_t size);
void engine_gl_element_array_buffer_destroy(ElementArrayBuffer* element_array_buffer);
void engine_gl_element_array_buffer_bind(const ElementArrayBuffer* element_array_buffer);
void engine_gl_element_array_buffer_unbind(void);
void engine_gl_element_array_buffer_update(ElementArrayBuffer* element_array_buffer,
                                           const void* data, size_t size);

unsigned int
engine_gl_element_array_buffer_get_size(const ElementArrayBuffer* element_array_buffer);
