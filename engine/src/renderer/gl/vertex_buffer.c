#include "vertex_buffer.h"

#include "engine/general/engine_alloc.h"

#include "glad/glad.h"

VertexBuffer* engine_gl_vertex_buffer_create(const void* data, size_t size)
{
    VertexBuffer* vertex_buffer = engine_alloc(sizeof(VertexBuffer));
    vertex_buffer->type = GL_ARRAY_BUFFER;

    glGenBuffers(1, &vertex_buffer->id);
    glBindBuffer(vertex_buffer->type, vertex_buffer->id);
    glBufferData(vertex_buffer->type, (GLsizeiptr)size, data, GL_STATIC_DRAW);
    return vertex_buffer;
}

void engine_gl_vertex_buffer_destroy(VertexBuffer* vertex_buffer)
{
    glDeleteBuffers(1, &vertex_buffer->id);
    engine_free(vertex_buffer);
}

void engine_gl_vertex_buffer_bind(const VertexBuffer* vertex_buffer)
{
    glBindBuffer(vertex_buffer->type, vertex_buffer->id);
}

void engine_gl_vertex_buffer_unbind(void)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void engine_gl_vertex_buffer_update(VertexBuffer* vertex_buffer, const void* data, size_t size)
{
    glBindBuffer(vertex_buffer->type, vertex_buffer->id);
    glBufferSubData(vertex_buffer->type, 0, (GLsizeiptr)size, data);
}
