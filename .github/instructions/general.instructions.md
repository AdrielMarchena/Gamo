# General Project Instructions

## Language and Build System
- Language: C (C23)
- Build system: CMake
- Architecture: engine (static library) + app (executable)
- Development platform: Linux (Fedora KDE)
- Target platforms: cross-platform (Linux, Windows, macOS)

## Project Structure
- engine/ → reusable engine library
- app/ → executable using engine
- vendor/ → third-party libraries as git submodules
- engine/include → public headers
- engine/src → implementation

## Coding Rules
- Prefer simple, explicit C code
- Avoid unnecessary abstractions
- Avoid C++ patterns
- Avoid hidden macros unless necessary
- Prefer clear ownership and lifetime
- Minimize global state

## Include Rules
- Public headers must compile standalone
- Use engine-prefixed includes:
  #include <engine/foo.h>

- Never require relative includes from outside the module

## CMake Rules
- Use target-based CMake only
- Use target_link_libraries
- Use target_include_directories
- Do NOT use global include_directories
- Engine must export include directory using PUBLIC

## Formatting Rules (clang-format)
- Formatting is defined by .clang-format
- All code must be formatted automatically
- Never manually align or format inconsistently
- Format on save is expected

## Linting Rules (clang-tidy)
- Follow .clang-tidy configuration
- Code must compile cleanly under clang-tidy
- Avoid undefined behavior
- Avoid non-portable constructs

## Precompiled Headers
- Use CMake target_precompile_headers
- PCH header should be engine/pch.h
- Do not rely on include order hacks