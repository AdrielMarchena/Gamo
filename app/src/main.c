#include "engine/application/application.h"
#include "ui/nuklear_context.h"

static bool app_init(void)
{
    return true;
}

static void app_update(float dt)
{
    struct nk_context* ctx = engine_nuklear_get_context();
    render_app_ui(ctx);
}

static void app_shutdown(void) {}

int main(void)
{
    EngineApp app = {.init = app_init, .update = app_update, .shutdown = app_shutdown};

    return engine_run(&app);
}