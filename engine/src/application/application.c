#include "engine/application/application.h"
#include "engine/core/time.h"
#include "engine/engine.h"
#include "engine/general/engine_alloc.h"
#include "engine/general/logger.h"
#include "engine/platform/window.h"
#include "engine/ecs/scene.h"
#include "engine/renderer/renderer.h"
#include <stdbool.h>

static inline void process_critical_events(const EngineApp* app, Event* event)
{
    Engine* engine = app->engine;

    switch (event->type)
    {
    case EVENT_TYPE_WINDOW_CLOSE:
        engine_window_set_should_close(engine->window, true);

        break;
    case EVENT_TYPE_WINDOW_RESIZE:
        engine_renderer_resize(app->engine->renderer, event->data.window_resize.width,
                               event->data.window_resize.height);
        break;
    default:
        break;
    }

    event->handled =
        false; // Critical events are not marked as handled, they are always processed by the engine
}

static inline void process_and_dispatch_events(const EngineApp* app)
{
    Engine* engine = app->engine;
    Event event = {0};

    while (engine_event_queue_pop(engine->event_queue, &event))
    {
        process_critical_events(app, &event);

        // Iterate from top to bottom (reverse order)
        for (int i = app->layer_stack.ui_count - 1; i >= 0; --i)
        {
            const EngineLayer* ui_layer = &app->layer_stack.ui_layers[i];
            if (ui_layer->on_event)
            {
                ui_layer->on_event(ui_layer, &event);
            }

            if (event.handled)
            {
                break;
            }
        }

        // Iterate regular layers if not handled
        if (!event.handled)
        {
            for (int i = app->layer_stack.count - 1; i >= 0; --i)
            {
                const EngineLayer* layer = &app->layer_stack.layers[i];
                if (layer->on_event)
                {
                    layer->on_event(layer, &event);
                }

                if (event.handled)
                {
                    break;
                }
            }
        }
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

EngineApp* engine_create_app(bool (*init)(void), void (*shutdown)(void))
{
    app = engine_alloc(sizeof(EngineApp));
    app->init = init;
    app->shutdown = shutdown;

    Engine* engine = engine_create();

    app->engine = engine;
    app->layer_stack.count = 0;
    app->layer_stack.ui_count = 0;

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
        process_and_dispatch_events(app);

        UpdateData update_data = {
            .delta_time = delta_time,
            .frame_count = frame_count,
            .frame_rate = frame_count > 0 ? 1.0f / delta_time : 0.0f,
        };

        for (int i = 0; i < app->layer_stack.ui_count; i++)
        {
            const EngineLayer* ui_layer = &app->layer_stack.ui_layers[i];
            if (ui_layer->on_update)
            {
                ui_layer->on_update(ui_layer, update_data);
            }
        }

        for (int i = 0; i < app->layer_stack.count; i++)
        {
            const EngineLayer* layer = &app->layer_stack.layers[i];
            if (layer->on_update)
            {
                layer->on_update(layer, update_data);
            }
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

void engine_push_layer(EngineApp* app, EngineLayer* layer)
{
    if (app->layer_stack.count < ENGINE_MAX_LAYERS)
    {
        layer->id = app->layer_stack.count;
        app->layer_stack.layers[app->layer_stack.count++] = *layer;
        if (layer->on_create)
        {
            layer->on_create(layer);
        }
    }
    else
    {
        engine_log_error("Cannot push layer: maximum layer count reached");
    }
}

void engine_push_ui_layer(EngineApp* app, EngineLayer* layer)
{
    if (app->layer_stack.ui_count < ENGINE_MAX_LAYERS)
    {
        // TODO: use a real id generator for this, maybe a simple incrementing counter or a UUID
        // generator, for now we just use the index in the array
        layer->id = app->layer_stack.ui_count;
        app->layer_stack.ui_layers[app->layer_stack.ui_count++] = *layer;
        if (layer->on_create)
        {
            layer->on_create(layer);
        }
    }
    else
    {
        engine_log_error("Cannot push UI layer: maximum UI layer count reached");
    }
}

void engine_pop_layer(EngineApp* app, uint64_t layer_id)
{
    for (int i = 0; i < app->layer_stack.count; i++)
    {
        if (app->layer_stack.layers[i].id == layer_id)
        {
            if (app->layer_stack.layers[i].on_destroy)
            {
                app->layer_stack.layers[i].on_destroy(&app->layer_stack.layers[i]);
            }

            // Shift layers down to fill the gap
            for (int j = i; j < app->layer_stack.count - 1; j++)
            {
                app->layer_stack.layers[j] = app->layer_stack.layers[j + 1];
            }
            app->layer_stack.count--;
            return;
        }
    }
    engine_log_error("Cannot pop layer: layer with id %lu not found", layer_id);
}

void engine_pop_ui_layer(EngineApp* app, uint64_t layer_id)
{
    for (int i = 0; i < app->layer_stack.ui_count; i++)
    {
        if (app->layer_stack.ui_layers[i].id == layer_id)
        {
            if (app->layer_stack.ui_layers[i].on_destroy)
            {
                app->layer_stack.ui_layers[i].on_destroy(&app->layer_stack.ui_layers[i]);
            }

            // Shift layers down to fill the gap
            for (int j = i; j < app->layer_stack.ui_count - 1; j++)
            {
                app->layer_stack.ui_layers[j] = app->layer_stack.ui_layers[j + 1];
            }
            app->layer_stack.ui_count--;
            return;
        }
    }
    engine_log_error("Cannot pop UI layer: layer with id %lu not found", layer_id);
}