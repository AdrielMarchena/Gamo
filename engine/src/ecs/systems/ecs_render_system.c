#include "ecs_render_system.h"

#include "engine/ecs/components/transform.h"
#include "engine/ecs/components/mesh.h"

#include "engine/renderer/renderer.h"

#include <cglm/cglm.h>
#include <flecs.h>

// TODO: Move this function
void transform_to_model_matrix(const TransformComponent* trans, mat4 model)
{
    glm_mat4_identity(model);

    glm_translate(model, (float*)trans->translation);

    glm_rotate(model, trans->rotation[0], (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(model, trans->rotation[1], (vec3){0.0f, 1.0f, 0.0f});
    glm_rotate(model, trans->rotation[2], (vec3){0.0f, 0.0f, 1.0f});

    glm_scale(model, (float*)trans->scale);
}

void engine_ecs_render_system(ecs_iter_t* iter)
{
    printf("rendering system\n");
    TransformComponent* transforms = ecs_field(iter, TransformComponent, 0);
    MeshComponent* meshes = ecs_field(iter, MeshComponent, 1);

    printf("iter count: %d\n", iter->count);

    for (int i = 0; i < iter->count; i++)
    {
        TransformComponent* trans = &transforms[i];
        MeshComponent* mesh = &meshes[i];

        mat4 model;
        transform_to_model_matrix(trans, model);

        engine_renderer_handle_ui_input();

        engine_renderer_begin();

        engine_renderer_draw_mesh(&model, mesh->mesh);

        engine_renderer_draw_ui();

        engine_renderer_end();
    }
}