#pragma once

typedef struct Mesh Mesh;

Mesh* engine_mesh_create(const float* vertices, size_t vertex_count, const unsigned int* indices,
                         size_t index_count);
void engine_mesh_destroy(Mesh* mesh);
