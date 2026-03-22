#pragma once

typedef struct EngineTime EngineTime;

EngineTime* engine_time_create(void);
void engine_time_destroy(EngineTime* time);

void engine_time_init(EngineTime* time);
void engine_time_update(EngineTime* time);

float engine_time_delta(const EngineTime* time);
double engine_time_elapsed(const EngineTime* time);
float engine_time_fps(const EngineTime* time);
