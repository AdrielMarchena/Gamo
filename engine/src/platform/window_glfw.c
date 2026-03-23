#include "engine/engine.h"
#include "engine/events/events.h"
#include "engine/events/event_contructors.h"
#include "engine/general/assert.h"
#include "engine/general/engine_alloc.h"
#include "engine/general/logger.h"
#include "engine/platform/window.h"
#include "engine/ui/ui.h"

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

static inline void set_callbacks(EngineWindow* window);

static void forward_nuklear_key_callback(WindowContext* ctx, GLFWwindow* window, int key,
                                         int scancode, int action, int mods)
{
    if (!ctx || !ctx->ui_user_pointer)
    {
        return;
    }

    engine_ui_forward_key_input(window, ctx->ui_user_pointer, key, scancode, action, mods);
}

static void forward_nuklear_char_callback(WindowContext* ctx, GLFWwindow* window,
                                          unsigned int codepoint)
{
    if (!ctx || !ctx->ui_user_pointer)
    {
        return;
    }

    engine_ui_forward_char_input(window, ctx->ui_user_pointer, codepoint);
}

static void forward_nuklear_scroll_callback(WindowContext* ctx, GLFWwindow* window, double xoff,
                                            double yoff)
{
    if (!ctx || !ctx->ui_user_pointer)
    {
        return;
    }

    engine_ui_forward_scroll_input(window, ctx->ui_user_pointer, xoff, yoff);
}

static void forward_nuklear_mouse_button_callback(WindowContext* ctx, GLFWwindow* window,
                                                  int button, int action, int mods)
{
    if (!ctx || !ctx->ui_user_pointer)
    {
        return;
    }

    engine_ui_forward_mouse_button_input(window, ctx->ui_user_pointer, button, action, mods);
}

EngineWindow* engine_window_create(EventQueue* event_queue)
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
        engine_free(window);
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
        engine_free(window);
        glfwTerminate();
        return NULL;
    }

    WindowContext* context = engine_alloc(sizeof(WindowContext));
    if (!context)
    {
        engine_log_error("Failed to allocate window context\n");
        glfwDestroyWindow(window->handle);
        engine_free(window);
        glfwTerminate();
        return NULL;
    }

    context->event_queue = event_queue;
    context->ui_user_pointer = NULL;

    glfwSetWindowUserPointer(window->handle, context);

    set_callbacks(window);

    return window;
}

void engine_window_destroy(EngineWindow* window)
{
    if (window)
    {
        WindowContext* context = glfwGetWindowUserPointer(window->handle);

        glfwDestroyWindow(window->handle);

        if (context)
        {
            engine_free(context);
        }
        engine_free(window);
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

void engine_window_set_should_close(EngineWindow* window, bool should_close)
{
    glfwSetWindowShouldClose(window->handle, should_close);
}

void engine_window_swap_buffers(EngineWindow* window)
{
    glfwSwapBuffers(window->handle);
}

void engine_toggle_vsync(EngineWindow* window, bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
}

void* engine_window_get_native_handle(EngineWindow* window)
{
    return window->handle;
}

WindowContext* engine_window_get_context(void* native_window_handle)
{
    if (!native_window_handle)
    {
        return NULL;
    }

    return (WindowContext*)glfwGetWindowUserPointer((GLFWwindow*)native_window_handle);
}

void engine_window_set_context(void* native_window_handle, WindowContext* context)
{
    if (!native_window_handle)
    {
        return;
    }

    glfwSetWindowUserPointer((GLFWwindow*)native_window_handle, context);
}

void engine_window_context_set_ui_user_pointer(WindowContext* context, void* ui_user_pointer)
{
    if (!context)
    {
        return;
    }

    context->ui_user_pointer = ui_user_pointer;
}

void engine_gl_basic_clear_for_test()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

static void glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
    WindowContext* ctx = glfwGetWindowUserPointer(window);
    ENGINE_ASSERT(ctx != NULL, "Window context is NULL in window size callback");

    if (!ctx->event_queue)
    {
        return;
    }

    Event event = engine_create_window_resize_event(width, height);
    engine_event_queue_push(ctx->event_queue, event);
}

static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    WindowContext* ctx = glfwGetWindowUserPointer(window);
    ENGINE_ASSERT(ctx != NULL, "Window context is NULL in cursor callback");

    forward_nuklear_key_callback(ctx, window, key, scancode, action, mods);

    if (!ctx->event_queue)
    {
        return;
    }

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        Event event = engine_create_key_pressed_event(key, action == GLFW_REPEAT);
        engine_event_queue_push(ctx->event_queue, event);
    }
    else if (action == GLFW_RELEASE)
    {
        Event event = engine_create_key_released_event(key);

        engine_event_queue_push(ctx->event_queue, event);
    }
}

static void glfw_char_callback(GLFWwindow* window, unsigned int codepoint)
{
    WindowContext* ctx = glfwGetWindowUserPointer(window);
    ENGINE_ASSERT(ctx != NULL, "Window context is NULL in char callback");

    forward_nuklear_char_callback(ctx, window, codepoint);
}

static void glfw_cursor_callback(GLFWwindow* window, double xcord, double ycord)
{
    WindowContext* ctx = glfwGetWindowUserPointer(window);
    ENGINE_ASSERT(ctx != NULL, "Window context is NULL in cursor callback");

    if (!ctx->event_queue)
    {
        return;
    }

    Event event = engine_create_mouse_moved_event(xcord, ycord);

    engine_event_queue_push(ctx->event_queue, event);
}

static void glfw_scroll_callback(GLFWwindow* window, double xoff, double yoff)
{
    WindowContext* ctx = glfwGetWindowUserPointer(window);
    ENGINE_ASSERT(ctx != NULL, "Window context is NULL in cursor callback");

    forward_nuklear_scroll_callback(ctx, window, xoff, yoff);

    if (!ctx->event_queue)
    {
        return;
    }

    Event event = engine_create_mouse_scrolled_event(xoff, yoff);

    engine_event_queue_push(ctx->event_queue, event);
}

static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    WindowContext* ctx = glfwGetWindowUserPointer(window);
    ENGINE_ASSERT(ctx != NULL, "Window context is NULL in cursor callback");

    forward_nuklear_mouse_button_callback(ctx, window, button, action, mods);

    if (!ctx->event_queue)
    {
        return;
    }

    Event event = {0};

    if (action == GLFW_PRESS)
    {
        event = engine_create_mouse_button_pressed_event(button);
    }
    else if (action == GLFW_RELEASE)
    {
        event = engine_create_mouse_button_released_event(button);
    }
    else
    {
        return;
    }

    engine_event_queue_push(ctx->event_queue, event);
}

static void glfw_window_close_callback(GLFWwindow* window)
{
    WindowContext* ctx = glfwGetWindowUserPointer(window);
    ENGINE_ASSERT(ctx != NULL, "Window context is NULL in cursor callback");

    if (!ctx->event_queue)
    {
        return;
    }

    Event event = engine_create_window_close_event();

    engine_event_queue_push(ctx->event_queue, event);
}

static inline void set_callbacks(EngineWindow* window)
{
    glfwSetWindowSizeCallback(window->handle, (GLFWwindowsizefun)glfw_window_size_callback);
    glfwSetKeyCallback(window->handle, (GLFWkeyfun)glfw_key_callback);
    glfwSetCharCallback(window->handle, (GLFWcharfun)glfw_char_callback);
    glfwSetCursorPosCallback(window->handle, (GLFWcursorposfun)glfw_cursor_callback);
    glfwSetScrollCallback(window->handle, (GLFWscrollfun)glfw_scroll_callback);
    glfwSetMouseButtonCallback(window->handle, (GLFWmousebuttonfun)glfw_mouse_button_callback);
    glfwSetWindowCloseCallback(window->handle, (GLFWwindowclosefun)glfw_window_close_callback);
}