#ifndef ENGINE_UI_H
#define ENGINE_UI_H

#include <stdbool.h>

// Engine UI system
// The engine supports pluggable UI backends
// Currently supports: Nuklear

typedef struct EngineUI EngineUI;

EngineUI* engine_ui_create(void);
void engine_ui_destroy(EngineUI* ui_context);

bool engine_ui_init(EngineUI* ui_context, void* glfw_window);
void engine_ui_new_frame(EngineUI* ui_context);
void engine_ui_render(EngineUI* ui_context);
void engine_ui_shutdown(EngineUI* ui_context);

#endif
