#ifndef ENGINE_UI_H
#define ENGINE_UI_H

// Engine UI system
// The engine supports pluggable UI backends
// Currently supports: Nuklear

void engine_ui_init(void);
void engine_ui_setup_window(void* glfw_window);
void engine_ui_new_frame(void);
void engine_ui_render(void);
void engine_ui_shutdown(void);

#endif
