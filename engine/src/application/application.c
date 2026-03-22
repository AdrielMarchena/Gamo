#include "engine/application/application.h"
#include "engine/core/time.h"
#include "engine/engine.h"
#include "engine/general/engine_alloc.h"
#include "engine/general/logger.h"
#include "engine/platform/window.h"
#include "engine/ecs/scene.h"

static inline void process_events(Engine* engine)
{
    Event event;

    while (engine_event_queue_pop(engine->event_queue, &event))
    {
        engine_log_info("Event popped from queue: %d", event.type);
    }
}

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

    uint64_t frame_count = 0;

    engine_log_info("Entering main loop");
    while (!engine_window_should_close(app->engine->window))
    {
        const float delta_time = engine_time_delta(app->engine->time);

        engine_time_update(app->engine->time);

        engine_window_poll_events(app->engine->window);
        process_events(app->engine);

        if (app->update)
        {
            app->update((UpdateData){
                .delta_time = delta_time,
                .frame_count = frame_count,
                .frame_rate = engine_time_fps(app->engine->time),
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