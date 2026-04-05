#pragma once

#include <glad/glad.h>

typedef struct VertexArray
{
    GLuint id;
    GLenum type;
} VertexArray;

VertexArray engine_gl_vertex_array_create(void);
void engine_gl_vertex_array_destroy(VertexArray* vertex_array);
void engine_gl_vertex_array_bind(const VertexArray* vertex_array);
void engine_gl_vertex_array_unbind(void);
