#include "engine/ecs/components/camera.h"
#include "engine/general/assert.h"
#include "engine/general/engine_alloc.h"
#include "engine/platform/window.h"
#include "engine/renderer/framebuffer.h"
#include "engine/renderer/renderer.h"
#include "engine/renderer/texture.h"
#include "engine/ui/ui.h"

#include "cglm/types.h"
#include "glad/glad.h"

#include "gl/gl_check.h"
#include "engine/renderer/vertex_array.h"
#include "gl/shader.h"
#include "gl/shader_registry.h"
#include "engine/renderer/mesh.h"

static unsigned int gl_clear_flags = GL_COLOR_BUFFER_BIT;

static EngineCamera* current_camera = NULL;

void engine_renderer_clear_color(EngineRenderer* renderer)
{
    GL_CHECK(glClear(gl_clear_flags));
}

EngineRenderer* engine_renderer_init(EngineWindow* window, EngineUI* ui_context)
{
    ENGINE_ASSERT(window != NULL, "Window pointer is NULL");
    ENGINE_ASSERT(ui_context != NULL, "UI context pointer is NULL");

    EngineRenderer* renderer = engine_alloc(sizeof(EngineRenderer));

    if (!renderer)
    {
        return NULL;
    }

    renderer->ui_context = ui_context;

    EngineWindowRectSize window_size = engine_window_get_size(window);

    current_camera = engine_camera_create();
    engine_camera_set_orthographic(current_camera, 0.0f, (float)window_size.width, 0.0f,
                                   (float)window_size.height, -1.0f, 1.0f);

    engine_shader_registry_init();

    shader_load_all_from_directory("shaders");

    renderer->scene_target = engine_framebuffer_create(window_size.width, window_size.height);
    // Temp
    renderer->present_quad = engine_mesh_create_quad();

    ENGINE_ASSERT(renderer->present_quad.index_count != 0, "Failed to create present quad mesh");

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
    if (renderer)
    {
        engine_shader_registry_destroy();
        engine_framebuffer_destroy(renderer->scene_target);

        if (renderer->present_quad.index_count != 0)
        {
            // Temp
            engine_mesh_destroy_quad(&renderer->present_quad);
        }

        engine_free(renderer);
    }
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
        engine_camera_set_orthographic(current_camera, 0.0f, (float)width, 0.0f, (float)height,
                                       -1.0f, 1.0f);
    }
    engine_framebuffer_resize(renderer->scene_target, width, height);
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

void engine_renderer_present(EngineRenderer* renderer)
{
    ENGINE_ASSERT(renderer != NULL, "Renderer pointer is NULL");
    ENGINE_ASSERT(renderer->scene_target != NULL, "Scene target framebuffer is NULL");
    ENGINE_ASSERT(renderer->present_quad.index_count != 0, "Present quad mesh is NULL");

    engine_framebuffer_unbind();
    engine_renderer_begin(renderer);

    Texture scene_texture = {
        .id = renderer->scene_target->color_attachment,
        .width = renderer->scene_target->width,
        .height = renderer->scene_target->height,
        .channels = 3,
    };

    mat4 model;
    glm_mat4_identity(model);

    // Translate to viewport center, then scale
    // Quad vertices are -0.5 to 0.5, so after scaling by window size they'd be off-screen.
    // We need to place them in the visible area.
    float center_x = (float)renderer->scene_target->width / 2.0f;
    float center_y = (float)renderer->scene_target->height / 2.0f;

    glm_translate(model, (vec3){center_x, center_y, 0.0f});
    glm_scale(model, (vec3){(float)renderer->scene_target->width,
                            -(float)renderer->scene_target->height, 1.0f});

    const vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

    engine_renderer_draw_mesh_with_texture(renderer, &model, &renderer->present_quad,
                                           &scene_texture, &color);
}
