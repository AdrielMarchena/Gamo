#ifndef ENGINE_UI_BACKEND_H
#define ENGINE_UI_BACKEND_H

#include <stdbool.h>

typedef struct
{
    // Initialize the UI backend
    bool (*init)(void* glfw_window);

    // Called each frame before rendering UI
    void (*new_frame)(void);

    // Render the UI
    void (*render)(void);

    // Shutdown the UI backend
    void (*shutdown)(void);
} EngineUIBackend;

// Register a UI backend (called by the engine)
void engine_ui_set_backend(const EngineUIBackend* backend);

// Get the current backend
const EngineUIBackend* engine_ui_get_backend(void);

#endif
