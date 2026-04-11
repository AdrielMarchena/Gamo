file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS src/*.c)

add_executable(app
    ${APP_SOURCES}
)

target_link_libraries(app
    PRIVATE
        engine
)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/vendor/nuklear
    ${CMAKE_SOURCE_DIR}/vendor/cglm/include
)

target_compile_features(app PUBLIC c_std_23)

target_compile_definitions(app
    PRIVATE
        $<$<CONFIG:Debug>:APP_DEBUG>
        $<$<CONFIG:Release>:APP_RELEASE>
)