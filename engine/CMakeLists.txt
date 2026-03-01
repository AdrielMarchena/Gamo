file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS src/*.c)

add_library(engine STATIC
    ${ENGINE_SOURCES}
)

target_sources(engine
    PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/glad/src/glad.c
)

target_include_directories(engine
    PUBLIC
        include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/glad/include
)

find_package(OpenGL REQUIRED)

target_link_libraries(engine
    PRIVATE
        glfw
        OpenGL::GL
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_precompile_headers(engine
    PRIVATE
        src/pch.h
)
