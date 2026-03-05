#include "engine/platform/window.h"
#include <stdbool.h>

#include <cglm/cglm.h>

#include "mesh.h"

// Renderer API

bool engine_renderer_init(EngineWindow* window);
void engine_renderer_shutdown(void);

void engine_renderer_begin(void);
void engine_renderer_end(void);

// Draw functions

void engine_renderer_draw_quad(float xpos, float ypos, float width, float height);

void engine_renderer_draw_mesh(const mat4* model, const Mesh* mesh);
// Viewport

void engine_renderer_resize(int width, int height);
void engine_renderer_set_clear_color(float red, float green, float blue, float alpha);
void engine_renderer_clear_color(void);
