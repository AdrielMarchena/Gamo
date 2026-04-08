#pragma once

#include "vertex_array.h"
#include "vertex_buffer.h"
#include "element_array_buffer.h"

typedef struct Mesh
{
    VertexArray vao;
    VertexBuffer vbo;
    ElementArrayBuffer ebo;
    unsigned int index_count;
} Mesh;

/*
    This creates a simple quad mesh with the following vertices and indices:
    Vertices:
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
    -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,

    Indices:
    0, 1, 2,
    2, 3, 0

    Call engine_mesh_destroy_quad to destroy the mesh created by this function.
*/
Mesh engine_mesh_create_quad(void);
void engine_mesh_destroy_quad(Mesh* mesh);

Mesh engine_mesh_create(const float* vertices, size_t vertex_float_count,
                        const unsigned int* indices, size_t index_count);
void engine_mesh_destroy(Mesh* mesh);
