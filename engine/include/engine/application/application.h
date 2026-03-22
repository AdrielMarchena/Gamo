#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "engine/engine.h"

typedef struct UpdateData
{
    float delta_time;
    uint64_t frame_count;
    float frame_rate;

} UpdateData;

typedef struct EngineApp
{
    bool (*init)(void);
    void (*update)(UpdateData delta_time);
    void (*shutdown)(void);

    Engine* engine;
} EngineApp;

int engine_run(const EngineApp* app);
EngineApp* engine_create_app(bool (*init)(void), void (*update)(UpdateData),
                             void (*shutdown)(void));
void engine_app_destroy(EngineApp* app);
EngineApp* engine_get_current_app();
Engine* engine_get_current_engine();
