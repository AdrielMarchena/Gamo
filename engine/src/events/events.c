#include "engine/events/events.h"
#include "engine/general/assert.h"
#include "engine/core/debug_call.h"
#include <stdint.h>

static inline int debug_event_type(EventType type)
{
    switch (type)
    {
    case EVENT_TYPE_NONE:
        return 0;
    case EVENT_TYPE_WINDOW_CLOSE:
        return 1;
    case EVENT_TYPE_WINDOW_RESIZE:
        return 2;
    case EVENT_TYPE_KEY_PRESS:
        return 3;
    case EVENT_TYPE_KEY_RELEASE:
        return 4;
    case EVENT_TYPE_MOUSE_BUTTON_PRESS:
        return 5;
    case EVENT_TYPE_MOUSE_BUTTON_RELEASE:
        return 6;
    case EVENT_TYPE_MOUSE_MOVE:
        return 7;
    case EVENT_TYPE_MOUSE_SCROLL:
        return 8;
    default:
        return -1; // Unknown event type
    }
}

int32_t engine_event_is_in_category(const Event* event, EventCategory category)
{
    return (event->category & category) != 0;
}

EventCategory engine_event_get_category(EventType type)
{
    switch (type)
    {
    case EVENT_TYPE_WINDOW_CLOSE:
    case EVENT_TYPE_WINDOW_RESIZE:
        return EVENT_CATEGORY_APPLICATION;
    case EVENT_TYPE_KEY_PRESS:
    case EVENT_TYPE_KEY_RELEASE:
        return EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT;
    case EVENT_TYPE_MOUSE_BUTTON_PRESS:
    case EVENT_TYPE_MOUSE_BUTTON_RELEASE:
        return EVENT_CATEGORY_MOUSE_BUTTON | EVENT_CATEGORY_INPUT;
    case EVENT_TYPE_MOUSE_MOVE:
    case EVENT_TYPE_MOUSE_SCROLL:
        return EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT;
    default:
        return EVENT_CATEGORY_NONE;
    }
}

int32_t engine_event_dispatch(Event* event, EventType type, EventFn handler)
{
    // Will prevent dispatching of events with invalid types in debug builds, while allowing it in
    // release builds (for example, if an event type is added but the dispatch function isn't
    // updated yet)
    ENGINE_ASSERT(ENGINE_DEBUG_ONLY(debug_event_type(type)),
                  "Invalid event type passed to dispatch function");

    if (event->type == type)
    {
        event->handled = handler(event);
        return 1; // Event handled
    }
    return 0; // Event not handled
}

int32_t engine_event_queue_push(EventQueue* queue, Event event)
{
    int next = (queue->head + 1) % MAX_EVENTS;
    if (next == queue->tail)
    {
        return 0; // full
    }
    queue->events[queue->head] = event;
    queue->head = next;
    return 1;
}

int32_t engine_event_queue_pop(EventQueue* queue, Event* event)
{
    if (queue->head == queue->tail)
    {
        return 0; // empty
    }
    *event = queue->events[queue->tail];
    queue->tail = (queue->tail + 1) % MAX_EVENTS;
    return 1;
}

int32_t engine_event_queue_is_empty(const EventQueue* queue)
{
    return queue->head == queue->tail;
}
