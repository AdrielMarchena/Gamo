#pragma once

#include "events.h"

Event engine_create_window_resize_event(int32_t width, int32_t height);
Event engine_create_window_close_event();
Event engine_create_key_pressed_event(int32_t key, int32_t repeat);
Event engine_create_key_released_event(int32_t key);
Event engine_create_mouse_moved_event(double xcord, double ycord);
Event engine_create_mouse_scrolled_event(double xOffset, double yOffset);
Event engine_create_mouse_button_pressed_event(int32_t button);
Event engine_create_mouse_button_released_event(int32_t button);
