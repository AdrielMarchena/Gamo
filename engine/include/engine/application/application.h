#pragma once

#include <stdbool.h>
#include <stdint.h>

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
} EngineApp;

int engine_run(const EngineApp* app);
