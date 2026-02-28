file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS src/*.c)

add_executable(app
    ${APP_SOURCES}
)

target_link_libraries(app
    PRIVATE
        engine
)
