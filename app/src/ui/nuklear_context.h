#ifndef NUKLEAR_CONTEXT_H
#define NUKLEAR_CONTEXT_H

#include "engine/application/application.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include <nuklear.h>

struct nk_context* engine_nuklear_get_context(void);

void render_app_ui(struct nk_context* ctx, UpdateData update_data);

#endif
