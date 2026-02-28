# Style Instructions – Gamo

## File Header Requirement

All source and header files must begin with the standard Gamo file header.

Template:

/*
 * filename.c or filename.h
 * Gamo
 *
 * Copyright (c) 2026 Adriel Marchena
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 *
 * Description:
 *   Brief description of the file's purpose.
 */

The filename must match the actual file name.

Description must clearly state the responsibility of the file.

## Scope

Apply to:
- engine source files (.c)
- engine headers (.h)
- app source files (.c)
- app headers (.h)

Do NOT modify vendor files.

## Formatting

All files must follow:
- .clang-format
- .clang-tidy

Format automatically. Do not manually align code.

## Include Order

Source files must include their own header first.

Example:

#include <engine/platform/window.h>

Then system and vendor headers.

## Naming Conventions

Types:
EngineWindow
EngineWindowRectSize

Functions:
engine_window_create
engine_window_destroy

Files:
window.h
window_glfw.c

Use snake_case for functions and files.
Use PascalCase for types.
Use SCREAMING_SNAKE_CASE for constants.