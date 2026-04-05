#include "vertex_array.h"
#include "gl_check.h"

#include "engine/general/engine_alloc.h"

#include "glad/glad.h"

VertexArray engine_gl_vertex_array_create(void)
{
    VertexArray vertex_array;
    vertex_array.type = GL_VERTEX_ARRAY;

    GL_CHECK(glGenVertexArrays(1, &vertex_array.id));
    GL_CHECK(glBindVertexArray(vertex_array.id));
    return vertex_array;
}

void engine_gl_vertex_array_destroy(VertexArray* vertex_array)
{
    GL_CHECK(glDeleteVertexArrays(1, &vertex_array->id));
    vertex_array->id = 0;
    vertex_array->type = 0;
}

void engine_gl_vertex_array_bind(const VertexArray* vertex_array)
{
    GL_CHECK(glBindVertexArray(vertex_array->id));
}

void engine_gl_vertex_array_unbind(void)
{
    GL_CHECK(glBindVertexArray(0));
}
