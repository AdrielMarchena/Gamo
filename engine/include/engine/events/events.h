#pragma once

#include <stdint.h>
#define BIT(X) (1 << (X))

typedef enum
{
    EVENT_TYPE_NONE = 0,
    EVENT_TYPE_WINDOW_CLOSE,
    EVENT_TYPE_WINDOW_RESIZE,
    EVENT_TYPE_KEY_PRESS,
    EVENT_TYPE_KEY_RELEASE,
    EVENT_TYPE_MOUSE_BUTTON_PRESS,
    EVENT_TYPE_MOUSE_BUTTON_RELEASE,
    EVENT_TYPE_MOUSE_MOVE,
    EVENT_TYPE_MOUSE_SCROLL,
} EventType;

typedef enum
{
    EVENT_CATEGORY_NONE = 0,
    EVENT_CATEGORY_APPLICATION = BIT(0),
    EVENT_CATEGORY_INPUT = BIT(1),
    EVENT_CATEGORY_KEYBOARD = BIT(2),
    EVENT_CATEGORY_MOUSE = BIT(3),
    EVENT_CATEGORY_MOUSE_BUTTON = BIT(4),
} EventCategory;

typedef struct
{
    int32_t width;
    int32_t height;
} WindowResizeEvent;

typedef struct
{
    int32_t key;
    int32_t repeat;
} KeyPressedEvent;

typedef struct
{
    double x, y;
} MouseMovedEvent;

typedef struct
{
    double xOffset, yOffset;
} MouseScrolledEvent;

typedef struct
{
    int32_t button;
} MouseButtonEvent;

typedef struct
{
    int32_t key;
} KeyReleasedEvent;

typedef struct
{
    EventType type;
    EventCategory category;
    int32_t handled;

    union
    {
        WindowResizeEvent window_resize;
        KeyPressedEvent key_pressed;
        KeyReleasedEvent key_released;
        MouseMovedEvent mouse_moved;
        MouseScrolledEvent mouse_scrolled;
        MouseButtonEvent mouse_button_pressed;
        MouseButtonEvent mouse_button_released;
    } data;
} Event;

int32_t engine_event_is_in_category(const Event* event, EventCategory category);
EventCategory engine_event_get_category(EventType type);

// Dispacher
typedef int (*EventFn)(Event*);

int32_t engine_event_dispatch(Event* event, EventType type, EventFn handler);

// Event queue
#define MAX_EVENTS 256

typedef struct
{
    Event events[MAX_EVENTS];
    int head;
    int tail;
} EventQueue;

EventQueue* engine_event_queue_create();
void engine_event_queue_destroy(EventQueue* queue);

int32_t engine_event_queue_push(EventQueue* queue, Event event);
int32_t engine_event_queue_pop(EventQueue* queue, Event* event);
int32_t engine_event_queue_is_empty(const EventQueue* queue);