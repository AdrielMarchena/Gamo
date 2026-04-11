file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS src/*.c)
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS tests/*.c)

add_library(engine STATIC ${ENGINE_SOURCES})

target_compile_features(engine PUBLIC c_std_23)

target_sources(engine
    PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/glad/src/glad.c
        ${CMAKE_SOURCE_DIR}/vendor/flecs/distr/flecs.c
)

find_package(OpenGL REQUIRED)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/glad/include
        ${CMAKE_SOURCE_DIR}/vendor/cglm/include
        ${CMAKE_SOURCE_DIR}/vendor/flecs/include
        ${CMAKE_SOURCE_DIR}/vendor/nuklear
        ${CMAKE_SOURCE_DIR}/vendor/nuklear/demo/glfw_opengl3
        ${CMAKE_SOURCE_DIR}/vendor/stb
)

target_link_libraries(engine PRIVATE glfw OpenGL::GL)

enable_testing()

foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_FILE})

    target_link_libraries(${TEST_NAME}
        PRIVATE engine
    )

    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
    )
endforeach()