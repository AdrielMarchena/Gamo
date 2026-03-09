// UI customization layer
// The engine handles Nuklear initialization and basic rendering
// Applications can add custom UI code here if needed

#include "nuklear_context.h"
#include <stdio.h>

void render_app_ui(struct nk_context* ctx, UpdateData update_data)
{
    if (!ctx)
    {
        return;
    }

    static float last_frame_rate = 0;

    if (update_data.frame_count % 60 == 0) // Ads a delay to FPS updates for better readability
    {
        last_frame_rate = update_data.frame_rate;
    }

    // Your demo window
    if (nk_begin(ctx, "Main Window", nk_rect(50, 50, 230, 250),
                 NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE |
                     NK_WINDOW_TITLE))
    {
        nk_layout_row_static(ctx, 30, 80, 1);
        if (nk_button_label(ctx, "button"))
        {
            printf("button pressed\n");
        }
        nk_layout_row_dynamic(ctx, 30, 2);
        nk_label(ctx, "Frame Rate:", NK_TEXT_LEFT);
        char fps_text[32];
        snprintf(fps_text, sizeof(fps_text), "%.2f FPS", last_frame_rate);
        nk_label(ctx, fps_text, NK_TEXT_LEFT);
    }
    nk_end(ctx);
}

void app_render_custom_ui(void)
{
    // Custom UI can be added here if needed
    // The engine's Nuklear backend will handle the main UI
}
