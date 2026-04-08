#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct VertexBuffer
{
    uint32_t id;
    uint32_t type;
    size_t size;
} VertexBuffer;

VertexBuffer engine_gl_vertex_buffer_create(const void* data, size_t size);
void engine_gl_vertex_buffer_destroy(VertexBuffer* vertex_buffer);
void engine_gl_vertex_buffer_bind(const VertexBuffer* vertex_buffer);
void engine_gl_vertex_buffer_unbind(void);
void engine_gl_vertex_buffer_update(VertexBuffer* vertex_buffer, const void* data, size_t size);
