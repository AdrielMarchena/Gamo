# App Instructions

## Role
The app is the executable entry point using the engine.

It must:
- Link against engine
- Initialize systems
- Contain application-specific logic

It must NOT contain reusable engine logic.

## Include Rules
App must include engine headers as:

#include <engine/header.h>

Never include engine src files.

## CMake Rules
App must link engine:

target_link_libraries(app PRIVATE engine)

## Formatting and Linting
App code must:
- Follow .clang-format
- Pass clang-tidy
- Compile without warnings