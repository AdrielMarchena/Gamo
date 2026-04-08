#include "engine/renderer/mesh.h"

#include "engine/renderer/vertex_buffer.h"
#include "engine/renderer/vertex_array.h"
#include "engine/renderer/element_array_buffer.h"

#include "gl/gl_check.h"

#include <glad/glad.h>

Mesh engine_mesh_create(const float* vertices, size_t vertex_float_count,
                        const unsigned int* indices, size_t index_count)
{
    Mesh mesh = {0};

    mesh.vao = engine_gl_vertex_array_create();
    engine_gl_vertex_array_bind(&mesh.vao);

    mesh.vbo = engine_gl_vertex_buffer_create(vertices, vertex_float_count * sizeof(float));
    engine_gl_vertex_buffer_bind(&mesh.vbo);

    mesh.ebo = engine_gl_element_array_buffer_create(indices, index_count * sizeof(unsigned int));
    engine_gl_element_array_buffer_bind(&mesh.ebo);

    GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    GL_CHECK(glEnableVertexAttribArray(0));

    GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                   (void*)(3 * sizeof(float))));
    GL_CHECK(glEnableVertexAttribArray(1));

    mesh.index_count = (unsigned int)index_count;

    engine_gl_vertex_array_unbind();
    engine_gl_vertex_buffer_unbind();
    engine_gl_element_array_buffer_unbind();

    return mesh;
}

void engine_mesh_destroy(Mesh* mesh)
{
    engine_gl_vertex_array_destroy(&mesh->vao);
    engine_gl_vertex_buffer_destroy(&mesh->vbo);
    engine_gl_element_array_buffer_destroy(&mesh->ebo);

    mesh->index_count = 0;
}

static float vertices[] = {
    // x, y, z, u, v
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
};

static unsigned int indices[] = {0, 1, 2, 2, 3, 0};

Mesh engine_mesh_create_quad(void)
{
    return engine_mesh_create(vertices, sizeof(vertices) / sizeof(float), indices,
                              sizeof(indices) / sizeof(unsigned int));
}

void engine_mesh_destroy_quad(Mesh* mesh)
{
    engine_mesh_destroy(mesh);
}
