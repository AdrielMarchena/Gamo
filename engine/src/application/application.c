#include "engine/application/application.h"
#include "engine/core/time.h"
#include "engine/general/logger.h"
#include "engine/platform/window.h"
#include "engine/ecs/scene.h"
#include "engine/renderer/renderer.h"
#include "engine/ui/ui.h"

#include "engine/ecs/components/transform.h"
#include "engine/ecs/components/mesh.h"
#include "engine/ecs/components/texture.h"

float vertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                    0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};

unsigned int indices[] = {0, 1, 2, 2, 3, 0};

int engine_run(const EngineApp* app)
{
    EngineWindow* window = engine_window_create();

    if (window == NULL)
    {
        engine_log_error("Failed to create window");
        return -1;
    }

    engine_toggle_vsync(window, true);

    engine_log_info("Window created successfully");

    if (app->init)
    {
        engine_log_info("Initializing application");

        const int init_result = app->init();

        if (init_result == 0)
        {
            engine_log_error("Application initialization failed with code %d", init_result);
            return init_result;
        }

        engine_log_info("Application initialized successfully");
    }

    engine_time_init();

    engine_ui_init();

    engine_renderer_init(window);

    EngineScene* scene = engine_scene_create();

    if (scene == NULL)
    {
        engine_log_error("Failed to create scene");
        return -1;
    }

    { // For testing
        EngineEntity entity = engine_scene_entity_create(scene);

        TransformComponent* trans = engine_entity_add(entity, TransformComponent);
        trans->translation[0] = 300.0F;
        trans->translation[1] = 200.0F;

        trans->scale[0] = 100.0F;
        trans->scale[1] = -100.0F;

        TextureComponent* texture = engine_entity_add(entity, TextureComponent);
        texture->texture = engine_texture_load("assets/gato.jpg");

        texture->color[0] = 1.0f;
        texture->color[1] = 1.0f;
        texture->color[2] = 1.0f;
        texture->color[3] = 1.0f;

        MeshComponent* mesh = engine_entity_add(entity, MeshComponent);

        mesh->mesh = engine_mesh_create(vertices, sizeof(vertices), indices, sizeof(indices));
    }

    uint64_t frame_count = 0;

    engine_log_info("Entering main loop");
    while (!engine_window_should_close(window))
    {
        const float delta_time = engine_time_delta();

        engine_time_update();

        engine_window_poll_events(window);

        if (app->update)
        {
            app->update((UpdateData){
                .delta_time = delta_time,
                .frame_count = frame_count,
                .frame_rate = engine_time_fps(),
            });
        }

        engine_scene_update(scene, delta_time);

        engine_window_swap_buffers(window);
        frame_count++;
    }

    engine_log_info("\nExiting main loop");

    if (app->shutdown)
    {
        engine_log_info("Shutting down application");
        app->shutdown();
        engine_log_info("Application shutdown complete");
    }

    engine_log_info("Destroying window");
    engine_window_destroy(window);

    return 0;
}