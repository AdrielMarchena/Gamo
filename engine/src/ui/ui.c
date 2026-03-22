#include "engine/ui/ui.h"
#include "engine/ui/ui_backend.h"

#include "engine/general/engine_alloc.h"

// Forward declare the Nuklear backend
extern const EngineUIBackend nuklear_backend;

struct EngineUI
{
    const EngineUIBackend* backend;
    void* glfw_window;
    bool initialized;
};

EngineUI* engine_ui_create(void)
{
    EngineUI* ui_context = engine_alloc(sizeof(EngineUI));

    if (ui_context)
    {
        ui_context->backend = NULL;
        ui_context->glfw_window = NULL;
        ui_context->initialized = false;
    }

    return ui_context;
}

void engine_ui_destroy(EngineUI* ui_context)
{
    if (ui_context)
    {
        engine_ui_shutdown(ui_context);
        engine_free(ui_context);
    }
}

bool engine_ui_init(EngineUI* ui_context, void* glfw_window)
{
    if (!ui_context)
    {
        return false;
    }

    engine_ui_set_backend(&nuklear_backend);

    ui_context->backend = engine_ui_get_backend();
    ui_context->glfw_window = glfw_window;
    ui_context->initialized = false;

    if (ui_context->backend && ui_context->backend->init)
    {
        ui_context->initialized = ui_context->backend->init(glfw_window);
    }

    return ui_context->initialized;
}

void engine_ui_new_frame(EngineUI* ui_context)
{
    if (ui_context && ui_context->initialized && ui_context->backend &&
        ui_context->backend->new_frame)
    {
        ui_context->backend->new_frame();
    }
}

void engine_ui_render(EngineUI* ui_context)
{
    if (ui_context && ui_context->initialized && ui_context->backend && ui_context->backend->render)
    {
        ui_context->backend->render();
    }
}

void engine_ui_shutdown(EngineUI* ui_context)
{
    if (ui_context && ui_context->initialized && ui_context->backend &&
        ui_context->backend->shutdown)
    {
        ui_context->backend->shutdown();
        ui_context->initialized = false;
    }
}
