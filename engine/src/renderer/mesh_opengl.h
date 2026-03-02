

#include <glad/glad.h>

#include "gl/vertex_buffer.h"
#include "gl/vertex_array.h"
#include "gl/element_array_buffer.h"

typedef struct Mesh
{
    VertexArray vao;
    VertexBuffer vbo;
    ElementArrayBuffer ebo;
    unsigned int index_count;
} Mesh;
