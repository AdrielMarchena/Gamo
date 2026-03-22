#include "engine/application/application.h"
#include "ui/nuklear_context.h"

static bool app_init(void)
{
    return true;
}

static void app_update(UpdateData update_data)
{
    struct nk_context* ctx = engine_nuklear_get_context();
    render_app_ui(ctx, update_data);
}

static void app_shutdown(void) {}

int main(void)
{
    EngineApp* app = engine_create_app(app_init, app_update, app_shutdown);

    return engine_run(app);
}