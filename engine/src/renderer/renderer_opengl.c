#include "engine/ecs/components/camera.h"
#include "engine/platform/window.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/texture.h"
#include "engine/ui/ui.h"

#include "cglm/types.h"
#include "glad/glad.h"

#include "gl/vertex_array.h"
#include "gl/shader.h"
#include "gl/shader_registry.h"
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

static EngineCamera* current_camera = NULL;

void engine_renderer_clear_color(void)
{
    glClear(gl_clear_flags);
    check_gl_error(__FILE__, __LINE__);
}

bool engine_renderer_init(EngineWindow* window)
{
    // Initialize UI system
    void* glfw_window = engine_window_get_native_handle(window);
    engine_ui_setup_window(glfw_window);

    current_camera = engine_camera_create();
    engine_camera_set_orthographic(current_camera, 0.0f, (float)800, (float)600, 0.0f, -1.0f, 1.0f);

    engine_shader_registry_init();

    shader_load_all_from_directory("shaders");

    glDisable(GL_DEPTH_TEST);
    check_gl_error(__FILE__, __LINE__);
    return true;
}

void engine_renderer_begin(void)
{
    engine_renderer_clear_color();
    Shader* shader = engine_shader_registry_get("basic");

    const mat4* projection = engine_camera_get_projection(current_camera);
    const mat4* view = engine_camera_get_view(current_camera);

    engine_gl_shader_bind(shader);
    engine_gl_shader_set_projection(shader, projection);
    check_gl_error(__FILE__, __LINE__);

    engine_gl_shader_set_view(shader, view);
    check_gl_error(__FILE__, __LINE__);
}

void engine_renderer_end(void) {}

void engine_renderer_shutdown(void)
{
    engine_shader_registry_destroy();
    engine_ui_shutdown();
}

void engine_renderer_draw_mesh(const mat4* model, const Mesh* mesh)
{
    check_gl_error(__FILE__, __LINE__);
    Shader* shader = engine_shader_registry_get("basic");
    engine_gl_shader_bind(shader);

    engine_gl_shader_set_model(shader, model);

    engine_gl_vertex_array_bind(&mesh->vao);
    check_gl_error(__FILE__, __LINE__);

    glDrawElements(GL_TRIANGLES, (GLsizei)mesh->index_count, GL_UNSIGNED_INT, 0);
    check_gl_error(__FILE__, __LINE__);
}

void engine_renderer_draw_mesh_with_texture(const mat4* model, const Mesh* mesh, Texture* texture,
                                            const vec4* color)
{
    check_gl_error(__FILE__, __LINE__);
    Shader* shader = engine_shader_registry_get("basic");
    engine_gl_shader_bind(shader);

    engine_texture_bind(texture, 0);

    engine_gl_shader_set_model(shader, model);

    engine_gl_shader_set_texture(shader, texture);
    engine_gl_shader_set_color(shader, color);

    engine_gl_vertex_array_bind(&mesh->vao);
    check_gl_error(__FILE__, __LINE__);

    glDrawElements(GL_TRIANGLES, (GLsizei)mesh->index_count, GL_UNSIGNED_INT, 0);
    check_gl_error(__FILE__, __LINE__);
}

void engine_renderer_resize(int width, int height)
{
    glViewport(0, 0, width, height);
    check_gl_error(__FILE__, __LINE__);

    if (current_camera)
    {
        engine_camera_set_orthographic(current_camera, 0.0f, (float)width, (float)height, 0.0f,
                                       -1.0f, 1.0f);
    }
}

void engine_renderer_set_clear_color(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
    check_gl_error(__FILE__, __LINE__);
}

void engine_renderer_handle_ui_input(void)
{
    engine_ui_new_frame();
}

void engine_renderer_draw_ui(void)
{
    engine_ui_render();
}
