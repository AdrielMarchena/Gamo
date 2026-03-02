#include "vertex_array.h"

#include "engine/general/engine_alloc.h"

#include "glad/glad.h"

VertexArray* engine_gl_vertex_array_create(void)
{
    VertexArray* vertex_array = engine_alloc(sizeof(VertexArray));
    vertex_array->type = GL_VERTEX_ARRAY;

    glGenVertexArrays(1, &vertex_array->id);
    glBindVertexArray(vertex_array->id);
    return vertex_array;
}

void engine_gl_vertex_array_destroy(VertexArray* vertex_array)
{
    glDeleteVertexArrays(1, &vertex_array->id);
    engine_free(vertex_array);
}

void engine_gl_vertex_array_bind(const VertexArray* vertex_array)
{
    glBindVertexArray(vertex_array->id);
}

void engine_gl_vertex_array_unbind(void)
{
    glBindVertexArray(0);
}
