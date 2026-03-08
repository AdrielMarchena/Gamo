// UI customization layer
// The engine handles Nuklear initialization and basic rendering
// Applications can add custom UI code here if needed

#include "nuklear_context.h"
#include <stdio.h>

void render_app_ui(struct nk_context* ctx)
{
    if (!ctx)
    {
        return;
    }

    // Your demo window
    if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE |
                     NK_WINDOW_TITLE))
    {
        nk_layout_row_static(ctx, 30, 80, 1);
        if (nk_button_label(ctx, "button"))
        {
            printf("button pressed\n");
        }
    }
    nk_end(ctx);
}

void app_render_custom_ui(void)
{
    // Custom UI can be added here if needed
    // The engine's Nuklear backend will handle the main UI
}
