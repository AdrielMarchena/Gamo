#include "engine/application/application.h"
#include "engine/platform/window.h"

int engine_run(const EngineApp* app)
{
    EngineWindow* window = engine_window_create();

    if (app->init)
    {
        app->init();
    }

    while (!engine_window_should_close(window))
    {
        engine_window_poll_events(window);

        if (app->update)
        {
            app->update(0.016F); // Assuming a fixed delta time for simplicity
        }

        engine_window_swap_buffers(window);
    }

    if (app->shutdown)
    {
        app->shutdown();
    }

    engine_window_destroy(window);

    return 0;
}