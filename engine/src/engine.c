#include "engine/engine.h"
#include "engine/core/time.h"
#include "engine/events/events.h"
#include "engine/general/engine_alloc.h"
#include "engine/general/logger.h"
#include "engine/ui/ui.h"

Engine* engine_create()
{
    Engine* engine = engine_alloc(sizeof(Engine));

    if (!engine)
    {
        engine_log_error("Failed to allocate memory for engine");
        return NULL;
    }

    engine->window = NULL;
    engine->renderer = NULL;
    engine->event_queue = NULL;
    engine->scene = NULL;
    engine->time = NULL;
    engine->ui = NULL;

    EventQueue* event_queue = engine_event_queue_create();
    if (event_queue == NULL)
    {
        engine_log_error("Failed to create event queue");
        engine_free(engine);
        return NULL;
    }
    engine->event_queue = event_queue;

    EngineWindow* window = engine_window_create(event_queue);
    if (window == NULL)
    {
        engine_log_error("Failed to create window");
        engine_event_queue_destroy(event_queue);
        engine_free(engine);
        return NULL;
    }
    engine->window = window;

    EngineTime* time = engine_time_create();
    if (time == NULL)
    {
        engine_log_error("Failed to create time context");
        engine_window_destroy(window);
        engine_event_queue_destroy(event_queue);
        engine_free(engine);
        return NULL;
    }
    engine_time_init(time);
    engine->time = time;

    EngineUI* ui_context = engine_ui_create();
    if (ui_context == NULL)
    {
        engine_log_error("Failed to create UI context");
        engine_time_destroy(time);
        engine_window_destroy(window);
        engine_event_queue_destroy(event_queue);
        engine_free(engine);
        return NULL;
    }

    if (!engine_ui_init(ui_context, engine_window_get_native_handle(window)))
    {
        engine_log_error("Failed to initialize UI context");
        engine_ui_destroy(ui_context);
        engine_time_destroy(time);
        engine_window_destroy(window);
        engine_event_queue_destroy(event_queue);
        engine_free(engine);
        return NULL;
    }
    engine->ui = ui_context;

    EngineRenderer* renderer = engine_renderer_init(window, ui_context);
    if (renderer == NULL)
    {
        engine_log_error("Failed to initialize renderer");
        engine_ui_destroy(ui_context);
        engine_time_destroy(time);
        engine_window_destroy(window);
        engine_event_queue_destroy(event_queue);
        engine_free(engine);
        return NULL;
    }
    engine->renderer = renderer;

    EngineScene* scene = engine_scene_create();
    if (scene == NULL)
    {
        engine_log_error("Failed to create scene");
        engine_event_queue_destroy(event_queue);
        engine_renderer_shutdown(renderer);
        engine_ui_destroy(ui_context);
        engine_time_destroy(time);
        engine_window_destroy(window);
        engine_free(engine);
        return NULL;
    }
    engine->scene = scene;

    return engine;
}
void engine_destroy(Engine* engine)
{
    if (engine)
    {
        engine_scene_destroy(engine->scene);
        engine_window_destroy(engine->window);
        engine_event_queue_destroy(engine->event_queue);
        engine_renderer_shutdown(engine->renderer);
        engine_ui_destroy(engine->ui);
        engine_time_destroy(engine->time);
        engine_free(engine);
    }
}

EngineWindow* engine_get_window(Engine* engine)
{
    return engine ? engine->window : NULL;
}

EngineRenderer* engine_get_renderer(Engine* engine)
{
    return engine ? engine->renderer : NULL;
}

EventQueue* engine_get_event_queue(Engine* engine)
{
    return engine ? engine->event_queue : NULL;
}

EngineScene* engine_get_scene(Engine* engine)
{
    return engine ? engine->scene : NULL;
}

EngineTime* engine_get_time(Engine* engine)
{
    return engine ? engine->time : NULL;
}

EngineUI* engine_get_ui(Engine* engine)
{
    return engine ? engine->ui : NULL;
}