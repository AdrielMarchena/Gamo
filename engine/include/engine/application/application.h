#include <stdbool.h>

typedef struct EngineApp
{
    bool (*init)(void);
    void (*update)(float delta_time);
    void (*shutdown)(void);
} EngineApp;

int engine_run(const EngineApp* app);
