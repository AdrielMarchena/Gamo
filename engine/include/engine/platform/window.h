/*
 * Copyright (c) 2026 Adriel Marchena
 *
 * This file is part of Gamo.
 *
 * Licensed under the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction.
 */

#include "engine/platform/monitor.h"
#include <stdbool.h>

#define ENGINE_GAMMA_RAMP_SIZE 256

/*
 * Engine Window Handle
 */
typedef struct EngineWindow EngineWindow;

/*
 * Creates a new Engine Window instance. Returns NULL on failure.
 */
EngineWindow *engine_window_create(void);

/*
 * Destroys the given Engine Window instance and releases all associated
 * resources.
 */
void engine_window_destroy(EngineWindow *window);

/**
 * Polls for and processes events for the given Engine Window instance.
 */
void engine_window_poll_events(EngineWindow *window);

/**
 * Checks if the given Engine Window instance should close.
 * Returns true if the window should close, false otherwise.
 */
bool engine_window_should_close(EngineWindow *window);

/**
 * Retrieves the size of the given Engine Window instance.
 * Returns a struct containing the width and height of the window.
 */
EngineWindowRectSize engine_window_get_size(EngineWindow *window);

/**
 * Swaps the front and back buffers of the given Engine Window instance.
 */
void engine_window_swap_buffers(EngineWindow *window);

/**
 * Sets the title of the given Engine Window instance.
 */
void engine_window_set_title(EngineWindow *window, const char *title);

/**
 * Retrieves the list of available monitors.
 * Returns heap-allocated array. Caller must free().
 */
EngineMonitorInfo *engine_get_monitors(int *count);

/**
 * Retrieves the primary monitor.
 */
EngineMonitorInfo engine_get_primary_monitor(void);
