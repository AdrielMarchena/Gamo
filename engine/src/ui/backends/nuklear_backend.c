#include "engine/ui/ui_backend.h"

#include <glad/glad.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include <nuklear.h>

#define NK_GLFW_GL3_IMPLEMENTATION
#include "demo/glfw_opengl3/nuklear_glfw_gl3.h"

static struct nk_context* s_nk_ctx = NULL;
static struct nk_glfw s_nk_glfw = {0};

static bool nuklear_init(void* glfw_window)
{
    if (!glfw_window)
    {
        fprintf(stderr, "Error: glfw_window is NULL\n");
        return false;
    }

    s_nk_ctx = nk_glfw3_init(&s_nk_glfw, (GLFWwindow*)glfw_window, NK_GLFW3_INSTALL_CALLBACKS);

    if (!s_nk_ctx)
    {
        fprintf(stderr, "Error: nk_glfw3_init failed\n");
        return false;
    }

    // Setup default font
    {
        struct nk_font_atlas* atlas;
        nk_glfw3_font_stash_begin(&s_nk_glfw, &atlas);
        struct nk_font* font = nk_font_atlas_add_default(atlas, 13.0f, NULL);
        nk_glfw3_font_stash_end(&s_nk_glfw);
        nk_style_set_font(s_nk_ctx, &font->handle);
    }

    fprintf(stderr, "Nuklear initialized successfully\n");
    return true;
}

static void nuklear_new_frame(void)
{
    if (s_nk_ctx)
    {
        nk_glfw3_new_frame(&s_nk_glfw);
    }
}

static void nuklear_render(void)
{
    if (!s_nk_ctx)
    {
        fprintf(stderr, "Warning: nuklear_render called but s_nk_ctx is NULL\n");
        return;
    }

    // Application handles UI rendering through engine_nuklear_get_context()

    nk_glfw3_render(&s_nk_glfw, NK_ANTI_ALIASING_ON, 512 * 1024, 128 * 1024);
}

static void nuklear_shutdown(void)
{
    if (s_nk_ctx)
    {
        nk_glfw3_shutdown(&s_nk_glfw);
        s_nk_ctx = NULL;
    }
}

const EngineUIBackend nuklear_backend = {
    .init = nuklear_init,
    .new_frame = nuklear_new_frame,
    .render = nuklear_render,
    .shutdown = nuklear_shutdown,
};

// Public API for Nuklear backend
struct nk_context* engine_nuklear_get_context(void)
{
    return s_nk_ctx;
}
