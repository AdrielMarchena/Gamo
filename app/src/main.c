#include "engine/application/application.h"

static bool app_init(void)
{
    return true;
}

static void app_update(float dt) {}

static void app_shutdown(void) {}

int main(void)
{
    EngineApp app = {.init = app_init, .update = app_update, .shutdown = app_shutdown};

    return engine_run(&app);
}