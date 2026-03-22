#include "engine/core/time.h"

#include "engine/general/engine_alloc.h"

#include <GLFW/glfw3.h>

struct EngineTime
{
    double last_time;
    float delta_time;
    float fps;
};

EngineTime* engine_time_create(void)
{
    EngineTime* time = engine_alloc(sizeof(EngineTime));

    if (time)
    {
        time->last_time = 0.0;
        time->delta_time = 0.0F;
        time->fps = 0.0F;
    }

    return time;
}

void engine_time_destroy(EngineTime* time)
{
    if (time)
    {
        engine_free(time);
    }
}

void engine_time_init(EngineTime* time)
{
    if (!time)
    {
        return;
    }

    time->last_time = glfwGetTime();
    time->delta_time = 0.0F;
    time->fps = 0.0F;
}

void engine_time_update(EngineTime* time)
{
    if (!time)
    {
        return;
    }

    const double now = glfwGetTime();

    time->delta_time = (float)(now - time->last_time);
    time->last_time = now;

    if (time->delta_time > 0.0F)
    {
        time->fps = 1.0F / time->delta_time;
    }
}

float engine_time_delta(const EngineTime* time)
{
    return time ? time->delta_time : 0.0F;
}

double engine_time_elapsed(const EngineTime* time)
{
    (void)time;
    return glfwGetTime();
}

float engine_time_fps(const EngineTime* time)
{
    return time ? time->fps : 0.0F;
}
