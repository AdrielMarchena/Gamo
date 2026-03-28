#include "mesh_opengl.h"

#include "engine/general/engine_alloc.h"

#include "gl/vertex_buffer.h"
#include "gl/vertex_array.h"
#include "gl/element_array_buffer.h"
#include "gl/gl_check.h"

#include <glad/glad.h>

Mesh* engine_mesh_create(const float* vertices, size_t vertex_count, const unsigned int* indices,
                         size_t index_count)
{
    Mesh* mesh = engine_alloc(sizeof(Mesh));

    mesh->vao = *engine_gl_vertex_array_create();
    mesh->vbo = *engine_gl_vertex_buffer_create(vertices, vertex_count * sizeof(float));
    mesh->ebo = *engine_gl_element_array_buffer_create(indices, index_count * sizeof(unsigned int));

    GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
    GL_CHECK(glEnableVertexAttribArray(0));

    GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                   (void*)(3 * sizeof(float))));
    GL_CHECK(glEnableVertexAttribArray(1));

    mesh->index_count = (unsigned int)index_count;

    return mesh;
}

void engine_mesh_destroy(Mesh* mesh)
{
    engine_gl_vertex_array_destroy(&mesh->vao);
    engine_gl_vertex_buffer_destroy(&mesh->vbo);
    engine_gl_element_array_buffer_destroy(&mesh->ebo);
    engine_free(mesh);
}
