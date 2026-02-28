cmake_minimum_required(VERSION 3.15)
project(gamo C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add GLFW
add_subdirectory(vendor/glfw)

add_subdirectory(engine)
add_subdirectory(app)

# Enable clang-tidy for all targets
set(CMAKE_C_CLANG_TIDY clang-tidy)