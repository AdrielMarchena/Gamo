#include "engine/application/application.h"
#include "engine/core/time.h"
#include "engine/general/logger.h"
#include "engine/platform/window.h"

int engine_run(const EngineApp* app)
{
    EngineWindow* window = engine_window_create();

    if (window == NULL)
    {
        engine_log_error("Failed to create window");
        return -1;
    }

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

    engine_log_info("Entering main loop");
    while (!engine_window_should_close(window))
    {
        const float delta_time = engine_time_delta();

        engine_log_status("\rFPS: %f", engine_time_fps());

        engine_time_update();
        engine_window_poll_events(window);

        if (app->update)
        {
            app->update(delta_time);
        }

        engine_window_swap_buffers(window);
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