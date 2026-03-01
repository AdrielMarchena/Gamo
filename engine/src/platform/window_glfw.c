#include "engine/general/engine_alloc.h"
#include "engine/general/logger.h"
#include "engine/platform/window.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#define ENGINE_WINDOW_DEFAULT_WIDTH 800
#define ENGINE_WINDOW_DEFAULT_HEIGHT 600

struct EngineWindow
{
    GLFWwindow* handle;
};

struct EngineMonitor
{
    GLFWmonitor* handle;
};

EngineMonitorInfo get_glfw_monitor_info(GLFWmonitor* monitor)
{
    EngineMonitorInfo info = {};

    const char* name = glfwGetMonitorName(monitor);
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    float content_scale_x;
    float content_scale_y;
    glfwGetMonitorContentScale(monitor, &content_scale_x, &content_scale_y);

    const GLFWgammaramp* gamma = glfwGetGammaRamp(monitor);
    const GLFWvidmode* current_mode = glfwGetVideoMode(monitor);

    int physical_size_x;
    int physical_size_y;
    glfwGetMonitorPhysicalSize(monitor, &physical_size_x, &physical_size_y);

    int virtual_position_x;
    int virtual_position_y;
    glfwGetMonitorPos(monitor, &virtual_position_x, &virtual_position_y);

    int work_area_position_x;
    int work_area_position_y;
    int work_area_size_width;
    int work_area_size_height;
    glfwGetMonitorWorkarea(monitor, &work_area_position_x, &work_area_position_y,
                           &work_area_size_width, &work_area_size_height);

    info.redable_name = name;

    info.physical_size.width = physical_size_x;
    info.physical_size.height = physical_size_y;

    info.content_scale.x = content_scale_x;
    info.content_scale.y = content_scale_y;

    info.virtual_position.x = virtual_position_x;
    info.virtual_position.y = virtual_position_y;

    info.work_area.x = work_area_position_x;
    info.work_area.y = work_area_position_y;
    info.work_area.width = work_area_size_width;
    info.work_area.height = work_area_size_height;

    info.current_mode.width = current_mode->width;
    info.current_mode.height = current_mode->height;
    info.current_mode.red_bits = current_mode->redBits;
    info.current_mode.green_bits = current_mode->greenBits;
    info.current_mode.blue_bits = current_mode->blueBits;
    info.current_mode.refresh_rate = current_mode->refreshRate;

    info.monitor = engine_alloc(sizeof(EngineMonitor));
    info.monitor->handle = monitor;

    return info;
}

EngineWindow* engine_window_create()
{
    if (!glfwInit())
    {
        return NULL;
    }

    EngineWindow* window = engine_alloc(sizeof(EngineWindow));

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window->handle = glfwCreateWindow(ENGINE_WINDOW_DEFAULT_WIDTH, ENGINE_WINDOW_DEFAULT_HEIGHT,
                                      "Engine Window", NULL, NULL);

    if (!window->handle)
    {
        engine_log_error("Failed to create GLFW window\n");
        free(window);
        glfwTerminate();
        return NULL;
    }

    EngineMonitorInfo monitor_info = engine_get_primary_monitor();

    glfwSetWindowPos(window->handle, monitor_info.virtual_position.x + 100,
                     monitor_info.virtual_position.y + 100);

    glfwMakeContextCurrent(window->handle);

    // TODO: Mabe move to another file, glad init

    const int glad_init_result = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if (!glad_init_result)
    {
        engine_log_error("Failed to initialize GLAD: %d\n", glad_init_result);
        free(window);
        glfwTerminate();
        return NULL;
    }

    return window;
}

void engine_window_destroy(EngineWindow* window)
{
    if (window)
    {
        glfwDestroyWindow(window->handle);
        free(window);
        glfwTerminate();
    }
}

EngineMonitorInfo* engine_get_monitors(int* count)
{
    GLFWmonitor** monitors = glfwGetMonitors(count);

    if (!monitors || *count <= 0)
    {
        *count = 0;
        return NULL;
    }

    EngineMonitorInfo* infos = engine_alloc(*count * sizeof(EngineMonitorInfo));

    if (!infos)
    {
        *count = 0;
        return NULL;
    }

    for (int i = 0; i < *count; i++)
    {
        infos[i] = get_glfw_monitor_info(monitors[i]);
    }

    return infos;
}

EngineMonitorInfo engine_get_primary_monitor()
{
    GLFWmonitor* primary = glfwGetPrimaryMonitor();

    const EngineMonitorInfo info = get_glfw_monitor_info(primary);

    return info;
}

void engine_window_poll_events(EngineWindow* window)
{
    glfwPollEvents();
}

bool engine_window_should_close(EngineWindow* window)
{
    return glfwWindowShouldClose(window->handle);
}

void engine_window_swap_buffers(EngineWindow* window)
{
    glfwSwapBuffers(window->handle);
}

void engine_toggle_vsync(EngineWindow* window, bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
}

void engine_gl_basic_clear_for_test()
{
    glClear(GL_COLOR_BUFFER_BIT);
}
