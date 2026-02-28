# General Project Instructions

Language: C  
Build system: CMake  
Architecture: engine (library) + app (executable)  
Development platform: Linux (Fedora KDE)  
Target: cross-platform

Rules:
- Prefer simple, explicit C code
- Avoid unnecessary abstractions
- Avoid hidden macros
- Use clear ownership and lifetime
- No C++ features or patterns
- Keep engine independent from app

CMake:
- Use target-based commands only
- Use target_link_libraries
- Use target_include_directories
- Do not use global include_directories