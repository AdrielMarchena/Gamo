#include "glad/glad.h"

#include "gl/vertex_array.h"
#include "gl/shader.h"
#include "mesh_opengl.h"

static unsigned int gl_clear_flags = GL_COLOR_BUFFER_BIT;

static void check_gl_error(const char* file, int line)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        fprintf(stderr, "OpenGL error in %s at line %d: %d\n", file, line, err);
    }
}

Shader* shader;

bool engine_renderer_init()
{
    shader = engine_gl_shader_create("#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main(){ gl_Position = vec4(aPos, 1.0); }\n",
                                     "#version 330 core\n"
                                     "out vec4 FragColor;\n"
                                     "void main(){ FragColor = vec4(0.2, 0.7, 0.3, 1.0); }\n");
    check_gl_error(__FILE__, __LINE__);
}

void engine_renderer_draw_mesh(const Mesh* mesh)
{
    engine_gl_shader_bind(shader);
    check_gl_error(__FILE__, __LINE__);

    engine_gl_vertex_array_bind(&mesh->vao);
    check_gl_error(__FILE__, __LINE__);

    glDrawElements(GL_TRIANGLES, (GLsizei)mesh->index_count, GL_UNSIGNED_INT, 0);
    check_gl_error(__FILE__, __LINE__);
}

void engine_renderer_set_clear_color(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
    check_gl_error(__FILE__, __LINE__);
}

void engine_renderer_clear_color(void)
{
    glClear(gl_clear_flags);
    check_gl_error(__FILE__, __LINE__);
}
