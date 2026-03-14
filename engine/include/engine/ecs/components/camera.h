#pragma once

#include "cglm/types.h"
#include <cglm/cglm.h>

enum EngineCameraType
{
    ENGINE_CAMERA_TYPE_ORTHOGRAPHIC,
    ENGINE_CAMERA_TYPE_PERSPECTIVE
};

typedef struct EngineCamera EngineCamera;

EngineCamera* engine_camera_create();
void engine_camera_shutdown(EngineCamera* camera);

void engine_camera_set_type(EngineCamera* camera, enum EngineCameraType type);
enum EngineCameraType engine_camera_get_type(const EngineCamera* camera);

void engine_camera_set_orthographic(EngineCamera* camera, float left, float right, float bottom,
                                    float top, float near_plane, float far_plane);
void engine_camera_set_perspective(EngineCamera* camera, float fov, float width, float height,
                                   float aspect_ratio, float near_plane, float far_plane);

const mat4* engine_camera_get_projection(const EngineCamera* camera);
const mat4* engine_camera_get_view(const EngineCamera* camera);
