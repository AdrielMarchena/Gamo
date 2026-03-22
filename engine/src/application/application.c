#include "engine/application/application.h"
#include "engine/core/time.h"
#include "engine/engine.h"
#include "engine/general/engine_alloc.h"
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

static EngineApp* app = NULL;

EngineApp* engine_get_current_app()
{
    return app;
}

Engine* engine_get_current_engine()
{
    return app->engine;
}

EngineApp* engine_create_app(bool (*init)(void), void (*update)(UpdateData), void (*shutdown)(void))
{
    app = engine_alloc(sizeof(EngineApp));
    app->init = init;
    app->update = update;
    app->shutdown = shutdown;

    Engine* engine = engine_create();

    app->engine = engine;

    return app;
}

int engine_run(const EngineApp* app)
{

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

    { // For testing
        EngineEntity entity = engine_scene_entity_create(app->engine->scene);

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
    while (!engine_window_should_close(app->engine->window))
    {
        const float delta_time = engine_time_delta();

        engine_time_update();

        engine_window_poll_events(app->engine->window);

        if (app->update)
        {
            app->update((UpdateData){
                .delta_time = delta_time,
                .frame_count = frame_count,
                .frame_rate = engine_time_fps(),
            });
        }

        engine_scene_update(app->engine->scene, delta_time);

        engine_window_swap_buffers(app->engine->window);
        frame_count++;
    }

    engine_log_info("\nExiting main loop");

    if (app->shutdown)
    {
        engine_log_info("Shutting down application");
        app->shutdown();
        engine_log_info("Application shutdown complete");
    }

    engine_log_info("Destroying application");
    engine_app_destroy((EngineApp*)app);

    return 0;
}

void engine_app_destroy(EngineApp* app)
{
    if (app)
    {
        engine_destroy(app->engine);
        engine_free(app);
        app = NULL;
    }
}