#include "vertex_buffer.h"
#include "gl_check.h"

#include "engine/general/engine_alloc.h"

#include "glad/glad.h"

VertexBuffer engine_gl_vertex_buffer_create(const void* data, size_t size)
{
    VertexBuffer vertex_buffer = {
        .id = 0,
        .type = GL_ARRAY_BUFFER,
        .size = size,
    };

    GL_CHECK(glGenBuffers(1, &vertex_buffer.id));
    GL_CHECK(glBindBuffer(vertex_buffer.type, vertex_buffer.id));
    GL_CHECK(glBufferData(vertex_buffer.type, (GLsizeiptr)size, data, GL_STATIC_DRAW));
    return vertex_buffer;
}

void engine_gl_vertex_buffer_destroy(VertexBuffer* vertex_buffer)
{
    GL_CHECK(glDeleteBuffers(1, &vertex_buffer->id));
    vertex_buffer->id = 0;
    vertex_buffer->type = 0;
    vertex_buffer->size = 0;
}

void engine_gl_vertex_buffer_bind(const VertexBuffer* vertex_buffer)
{
    GL_CHECK(glBindBuffer(vertex_buffer->type, vertex_buffer->id));
}

void engine_gl_vertex_buffer_unbind(void)
{
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void engine_gl_vertex_buffer_update(VertexBuffer* vertex_buffer, const void* data, size_t size)
{
    GL_CHECK(glBindBuffer(vertex_buffer->type, vertex_buffer->id));
    GL_CHECK(glBufferSubData(vertex_buffer->type, 0, (GLsizeiptr)size, data));

    vertex_buffer->size = size;
}
