#include "engine/ecs/components/camera.h"
#include "engine/general/engine_alloc.h"
#include "engine/general/logger.h"

struct EngineCamera
{
    mat4 projection;
    mat4 view;
    enum EngineCameraType type;
};

EngineCamera* engine_camera_create()
{
    EngineCamera* camera = engine_alloc(sizeof(EngineCamera));
    if (!camera)
    {
        return NULL;
    }

    glm_mat4_identity(camera->projection);
    glm_mat4_identity(camera->view);

    return camera;
}

void engine_camera_shutdown(EngineCamera* camera)
{
    if (camera)
    {
        engine_free(camera);
    }
}

void engine_camera_set_type(EngineCamera* camera, enum EngineCameraType type)
{
    if (camera)
    {
        camera->type = type;
    }
}

enum EngineCameraType engine_camera_get_type(const EngineCamera* camera)
{
    if (camera)
    {
        return camera->type;
    }
    return ENGINE_CAMERA_TYPE_ORTHOGRAPHIC; // Default type
}

void engine_camera_set_orthographic(EngineCamera* camera, float left, float right, float bottom,
                                    float top, float near_plane, float far_plane)
{
    if (camera)
    {
        glm_ortho(left, right, bottom, top, near_plane, far_plane, camera->projection);
        camera->type = ENGINE_CAMERA_TYPE_ORTHOGRAPHIC;
    }
}

void engine_camera_set_perspective(EngineCamera* camera, float fov, float width, float height,
                                   float aspect_ratio, float near_plane, float far_plane)
{
    engine_log_warning("Perspective camera is not implemented yet.");
}

const mat4* engine_camera_get_projection(const EngineCamera* camera)
{
    if (camera)
    {
        return &camera->projection;
    }
    return NULL;
}

const mat4* engine_camera_get_view(const EngineCamera* camera)
{
    if (camera)
    {
        return &camera->view;
    }
    return NULL;
}
