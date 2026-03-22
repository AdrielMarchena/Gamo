#include "engine/events/event_contructors.h"

Event engine_create_window_resize_event(int32_t width, int32_t height)
{
    Event event = {0};
    event.type = EVENT_TYPE_WINDOW_RESIZE;
    event.category = EVENT_CATEGORY_APPLICATION;
    event.data.window_resize.width = width;
    event.data.window_resize.height = height;
    return event;
}

Event engine_create_key_pressed_event(int32_t key, int32_t repeat)
{
    Event event = {0};
    event.type = EVENT_TYPE_KEY_PRESS;
    event.category = EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT;
    event.data.key_pressed.key = key;
    event.data.key_pressed.repeat = repeat;
    return event;
}

Event engine_create_key_released_event(int32_t key)
{
    Event event = {0};
    event.type = EVENT_TYPE_KEY_RELEASE;
    event.category = EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT;
    event.data.key_released.key = key;
    return event;
}

Event engine_create_mouse_moved_event(float x, float y)
{
    Event event = {0};
    event.type = EVENT_TYPE_MOUSE_MOVE;
    event.category = EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT;
    event.data.mouse_moved.x = x;
    event.data.mouse_moved.y = y;
    return event;
}

Event engine_create_mouse_scrolled_event(float xOffset, float yOffset)
{
    Event event = {0};
    event.type = EVENT_TYPE_MOUSE_SCROLL;
    event.category = EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT;
    event.data.mouse_scrolled.xOffset = xOffset;
    event.data.mouse_scrolled.yOffset = yOffset;
    return event;
}

Event engine_create_mouse_button_pressed_event(int32_t button)
{
    Event event = {0};
    event.type = EVENT_TYPE_MOUSE_BUTTON_PRESS;
    event.category = EVENT_CATEGORY_MOUSE_BUTTON | EVENT_CATEGORY_INPUT;
    event.data.mouse_button_pressed.button = button;
    return event;
}

Event engine_create_mouse_button_released_event(int32_t button)
{
    Event event = {0};
    event.type = EVENT_TYPE_MOUSE_BUTTON_RELEASE;
    event.category = EVENT_CATEGORY_MOUSE_BUTTON | EVENT_CATEGORY_INPUT;
    event.data.mouse_button_released.button = button;
    return event;
}
