#pragma once

#include "glad/glad.h"

typedef struct VertexBuffer
{
    GLuint id;
    GLenum type;
} VertexBuffer;

VertexBuffer* engine_gl_vertex_buffer_create(const void* data, size_t size);
void engine_gl_vertex_buffer_destroy(VertexBuffer* vertex_buffer);
void engine_gl_vertex_buffer_bind(const VertexBuffer* vertex_buffer);
void engine_gl_vertex_buffer_unbind(void);
void engine_gl_vertex_buffer_update(VertexBuffer* vertex_buffer, const void* data, size_t size);
