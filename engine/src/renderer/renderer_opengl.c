#include "engine/ecs/components/camera.h"
#include "engine/general/engine_alloc.h"
#include "engine/platform/window.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/texture.h"
#include "engine/ui/ui.h"

#include "cglm/types.h"
#include "glad/glad.h"

#include "gl/gl_check.h"
#include "gl/vertex_array.h"
#include "gl/shader.h"
#include "gl/shader_registry.h"
#include "mesh_opengl.h"

static unsigned int gl_clear_flags = GL_COLOR_BUFFER_BIT;

static EngineCamera* current_camera = NULL;

void engine_renderer_clear_color(EngineRenderer* renderer)
{
    GL_CHECK(glClear(gl_clear_flags));
}

EngineRenderer* engine_renderer_init(EngineWindow* window, EngineUI* ui_context)
{
    EngineRenderer* renderer = engine_alloc(sizeof(EngineRenderer));

    if (!renderer)
    {
        return NULL;
    }

    renderer->ui_context = ui_context;

    current_camera = engine_camera_create();
    engine_camera_set_orthographic(current_camera, 0.0f, (float)800, (float)600, 0.0f, -1.0f, 1.0f);

    engine_shader_registry_init();

    shader_load_all_from_directory("shaders");

    GL_CHECK(glDisable(GL_DEPTH_TEST));

    renderer->initialized = 1;
    return renderer;
}

void engine_renderer_begin(EngineRenderer* renderer)
{
    engine_renderer_clear_color(renderer);
    Shader* shader = engine_shader_registry_get("basic");

    const mat4* projection = engine_camera_get_projection(current_camera);
    const mat4* view = engine_camera_get_view(current_camera);

    engine_gl_shader_bind(shader);
    engine_gl_shader_set_projection(shader, projection);

    engine_gl_shader_set_view(shader, view);
}

void engine_renderer_end(EngineRenderer* renderer) {}

void engine_renderer_shutdown(EngineRenderer* renderer)
{
    engine_shader_registry_destroy();
    engine_free(renderer);
}

void engine_renderer_draw_mesh(EngineRenderer* renderer, const mat4* model, const Mesh* mesh)
{
    Shader* shader = engine_shader_registry_get("basic");
    engine_gl_shader_bind(shader);

    engine_gl_shader_set_model(shader, model);

    engine_gl_vertex_array_bind(&mesh->vao);

    GL_CHECK(glDrawElements(GL_TRIANGLES, (GLsizei)mesh->index_count, GL_UNSIGNED_INT, 0));
}

void engine_renderer_draw_mesh_with_texture(EngineRenderer* renderer, const mat4* model,
                                            const Mesh* mesh, Texture* texture, const vec4* color)
{
    Shader* shader = engine_shader_registry_get("basic");
    engine_gl_shader_bind(shader);

    engine_texture_bind(texture, 0);

    engine_gl_shader_set_model(shader, model);

    engine_gl_shader_set_texture(shader, texture);
    engine_gl_shader_set_color(shader, color);

    engine_gl_vertex_array_bind(&mesh->vao);

    GL_CHECK(glDrawElements(GL_TRIANGLES, (GLsizei)mesh->index_count, GL_UNSIGNED_INT, 0));
}

void engine_renderer_resize(EngineRenderer* renderer, int width, int height)
{
    GL_CHECK(glViewport(0, 0, width, height));

    if (current_camera)
    {
        engine_camera_set_orthographic(current_camera, 0.0f, (float)width, (float)height, 0.0f,
                                       -1.0f, 1.0f);
    }
}

void engine_renderer_set_clear_color(EngineRenderer* renderer, float red, float green, float blue,
                                     float alpha)
{
    GL_CHECK(glClearColor(red, green, blue, alpha));
}

void engine_renderer_handle_ui_input(EngineRenderer* renderer)
{
    engine_ui_new_frame(renderer->ui_context);
}

void engine_renderer_draw_ui(EngineRenderer* renderer)
{
    engine_ui_render(renderer->ui_context);
}
