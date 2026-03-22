#include "engine/engine.h"
#include "engine/general/engine_alloc.h"
#include "engine/general/logger.h"

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

    EngineWindow* window = engine_window_create(NULL);
    if (window == NULL)
    {
        engine_log_error("Failed to create window");
        return NULL;
    }
    engine->window = window;

    EngineRenderer* renderer = engine_renderer_init(window);
    if (renderer == NULL)
    {
        engine_log_error("Failed to initialize renderer");
        engine_window_destroy(window);
        return NULL;
    }
    engine->renderer = renderer;

    EventQueue* event_queue = engine_event_queue_create();
    if (event_queue == NULL)
    {
        engine_log_error("Failed to create event queue");
        engine_renderer_shutdown(renderer);
        engine_window_destroy(window);
        return NULL;
    }
    engine->event_queue = event_queue;

    EngineScene* scene = engine_scene_create();
    if (scene == NULL)
    {
        engine_log_error("Failed to create scene");
        engine_event_queue_destroy(event_queue);
        engine_renderer_shutdown(renderer);
        engine_window_destroy(window);
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
        engine_event_queue_destroy(engine->event_queue);
        engine_renderer_shutdown(engine->renderer);
        engine_window_destroy(engine->window);
        engine_free(engine);
    }
}