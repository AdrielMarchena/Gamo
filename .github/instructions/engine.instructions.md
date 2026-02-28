# Engine Instructions

## Role
The engine is a reusable static library.

It must:
- Be independent from app
- Be reusable across projects
- Contain reusable systems only

## Structure
engine/
  include/engine/ → public headers
  src/ → implementation
  CMakeLists.txt

## Include Rules
Public headers must be included as:

#include <engine/header.h>

Never expose src/ headers publicly.

## CMake Rules
Engine must define:

target_include_directories(engine PUBLIC include)

Engine must compile as STATIC library.

Engine may use:

target_precompile_headers(engine PRIVATE include/engine/pch.h)

## Dependency Rules
Engine may depend on vendor libraries.
Engine must NOT depend on app.

## Formatting and Linting
Engine code must:
- Follow .clang-format exactly
- Pass clang-tidy checks
- Avoid warnings