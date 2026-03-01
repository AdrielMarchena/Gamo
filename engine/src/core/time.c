#include "engine/core/time.h"

#include <GLFW/glfw3.h>

static double last_time;
static float delta_time;
static float fps;

float engine_time_fps(void)
{
    return fps;
}

void engine_time_init(void)
{
    last_time = glfwGetTime();
    delta_time = 0.0F;
}

void engine_time_update(void)
{
    double now = glfwGetTime();

    delta_time = (float)(now - last_time);
    last_time = now;

    if (delta_time > 0.0F)
    {
        fps = 1.0F / delta_time;
    }
}

float engine_time_delta(void)
{
    return delta_time;
}

double engine_time_elapsed(void)
{
    return glfwGetTime();
}
