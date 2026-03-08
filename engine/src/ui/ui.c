#include "engine/ui/ui.h"
#include "engine/ui/ui_backend.h"
#include "engine/platform/window.h"

// Forward declare the Nuklear backend
extern const EngineUIBackend nuklear_backend;

static const EngineUIBackend* s_current_backend = NULL;
static void* s_glfw_window = NULL;

void engine_ui_init(void)
{
    // Set Nuklear as the default UI backend
    engine_ui_set_backend(&nuklear_backend);
    s_current_backend = engine_ui_get_backend();
}

void engine_ui_setup_window(void* glfw_window)
{
    s_glfw_window = glfw_window;
    if (s_current_backend && s_current_backend->init)
    {
        s_current_backend->init(glfw_window);
    }
}

void engine_ui_new_frame(void)
{
    if (s_current_backend && s_current_backend->new_frame)
    {
        s_current_backend->new_frame();
    }
}

void engine_ui_render(void)
{
    if (s_current_backend && s_current_backend->render)
    {
        s_current_backend->render();
    }
}

void engine_ui_shutdown(void)
{
    if (s_current_backend && s_current_backend->shutdown)
    {
        s_current_backend->shutdown();
    }
}
