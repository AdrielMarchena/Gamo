cmake_minimum_required(VERSION 3.15)
project(gamo C)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable AddressSanitizer for Debug builds
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

# Add GLFW
add_subdirectory(vendor/glfw)

add_subdirectory(engine)
add_subdirectory(app)

# Enable clang-tidy for all targets
set(CMAKE_C_CLANG_TIDY clang-tidy)