#include "engine/application/application.h"
#include "engine/core/time.h"
#include "engine/general/logger.h"
#include "engine/platform/window.h"
#include "engine/renderer/renderer.h"

#include "engine/renderer/mesh.h"

float vertices[] = {
    // Positions             // Texture Coords (U, V)
    -0.5f, -0.5f, 0.0f, // Bottom left corner (0)
    0.5f,  -0.5f, 0.0f, // Bottom right corner (1)
    0.5f,  0.5f,  0.0f, // Top right corner (2)
    -0.5f, 0.5f,  0.0f, // Top left corner (3)
};

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

    engine_renderer_init(window);

    const Mesh* demo_mesh = engine_mesh_create(vertices, sizeof(vertices) / sizeof(float), indices,
                                               sizeof(indices) / sizeof(unsigned int));

    engine_log_info("Entering main loop");
    while (!engine_window_should_close(window))
    {
        const float delta_time = engine_time_delta();

        engine_log_status("\rFPS: %f", engine_time_fps());

        engine_time_update();

        if (app->update)
        {
            app->update(delta_time);
        }

        engine_renderer_begin();

        engine_renderer_draw_mesh(demo_mesh);

        engine_renderer_end();

        engine_window_swap_buffers(window);
        engine_window_poll_events(window);
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