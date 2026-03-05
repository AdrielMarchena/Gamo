#include "cglm/types.h"
#include "glad/glad.h"

#include "gl/vertex_array.h"
#include "gl/shader.h"
#include "gl/shader_registry.h"
#include "mesh_opengl.h"
#include <math.h>

static unsigned int gl_clear_flags = GL_COLOR_BUFFER_BIT;

static void check_gl_error(const char* file, int line)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        fprintf(stderr, "OpenGL error in %s at line %d: %d\n", file, line, err);
    }
}

static mat4 projection;
static mat4 view;

void engine_renderer_clear_color(void)
{
    glClear(gl_clear_flags);
    check_gl_error(__FILE__, __LINE__);
}

bool engine_renderer_init()
{
    glm_mat4_identity(view);
    glm_ortho(0.0f, (float)800, (float)600, 0.0f, -1.0f, 1.0f, projection);
    engine_shader_registry_init();

    shader_load_all_from_directory("app/shaders");

    check_gl_error(__FILE__, __LINE__);
    return true;
}

void engine_renderer_begin(void)
{
    engine_renderer_clear_color();
    engine_gl_shader_bind(engine_shader_registry_get("basic"));
    engine_gl_shader_set_projection(engine_shader_registry_get("basic"), &projection);
    check_gl_error(__FILE__, __LINE__);
    engine_gl_shader_set_view(engine_shader_registry_get("basic"), &view);
    check_gl_error(__FILE__, __LINE__);
}

void engine_renderer_end(void)
{
    // Currently, we don't have any specific end operations, but this function can be used for
    // future extensions.
}

void engine_renderer_shutdown(void)
{
    engine_shader_registry_destroy();
}

void engine_renderer_draw_mesh(const Mesh* mesh)
{
    check_gl_error(__FILE__, __LINE__);

    mat4 model;
    glm_mat4_identity(model);

    glm_translate(model, (vec3){0, 0, 0.0f});
    glm_scale(model, (vec3){1, 1, 1.0f});

    engine_gl_shader_set_model(engine_shader_registry_get("basic"), &model);

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
