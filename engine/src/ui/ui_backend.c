#include "engine/ui/ui_backend.h"
#include <stdio.h>

static const EngineUIBackend* s_backend = NULL;

void engine_ui_set_backend(const EngineUIBackend* backend)
{
    s_backend = backend;
}

const EngineUIBackend* engine_ui_get_backend(void)
{
    return s_backend;
}
