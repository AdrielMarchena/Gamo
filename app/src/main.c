#include "engine/application/application.h"
#include "ui/nuklear_context.h"

#include "engine/general/logger.h"
#include "engine/ecs/components/transform.h"
#include "engine/ecs/components/mesh.h"
#include "engine/ecs/components/texture.h"
#include "engine/renderer/mesh.h"

float vertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                    0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};

unsigned int indices[] = {0, 1, 2, 2, 3, 0};

static void on_create(EngineLayer* layer)
{
    EngineApp* app = engine_get_current_app();
    EngineEntity entity = engine_scene_entity_create(app->engine->scene);

    TransformComponent* trans = engine_entity_add(entity, TransformComponent);
    trans->translation[0] = 300.0F;
    trans->translation[1] = 200.0F;

    trans->scale[0] = 100.0F;
    trans->scale[1] = -100.0F;

    TextureComponent* texture = engine_entity_add(entity, TextureComponent);
    texture->texture = engine_texture_load("assets/gato.jpg");

    texture->color[0] = 1.0f;
    texture->color[1] = 1.0f;
    texture->color[2] = 1.0f;
    texture->color[3] = 1.0f;

    MeshComponent* mesh = engine_entity_add(entity, MeshComponent);

    mesh->mesh = engine_mesh_create(vertices, sizeof(vertices), indices, sizeof(indices));
}

static void ui_update(EngineLayer* layer, UpdateData update_data)
{
    struct nk_context* ctx = engine_nuklear_get_context();
    render_app_ui(ctx, update_data);
}

static void app_shutdown(void) {}

static void handle_event_ui(EngineLayer* layer, Event* event)
{
    event->handled = false;
}

static void handle_event(EngineLayer* layer, Event* event)
{
    event->handled = false;
}

int main(void)
{
    EngineApp* app = engine_create_app(NULL, app_shutdown);

    EngineLayer ui_layer = {
        .on_update = ui_update,
        .on_event = handle_event_ui,

    };
    engine_push_ui_layer(app, &ui_layer);

    EngineLayer layer = {
        .on_create = on_create,
        .on_event = handle_event,
    };
    engine_push_layer(app, &layer);

    return engine_run(app);
}