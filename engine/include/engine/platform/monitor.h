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

#pragma once

/*
 * Engine Monitor Handle
 */
typedef struct EngineMonitor EngineMonitor;

/*
 * Engine Window Rect Size
 */
typedef struct EngineWindowRectSize
{
    int width;
    int height;
} EngineWindowRectSize;

/*
 * Engine Window Content Scale
 */
typedef struct EngineWindowContentScale
{
    float x;
    float y;
} EngineWindowContentScale;

/*
 * Engine Monitor Video Mode
 */
typedef struct EngineMonitorVideoMode
{
    int width;
    int height;
    int red_bits;
    int green_bits;
    int blue_bits;
    int refresh_rate;
} EngineMonitorVideoMode;

/*
 * Engine Monitor Position
 */
typedef struct EngineMonitorPosition
{
    int x;
    int y;
} EngineMonitorPosition;

/*
 * Engine Monitor Work Area
 */
typedef struct EngineMonitorWorkArea
{
    int x;
    int y;
    int width;
    int height;
} EngineMonitorWorkArea;

/*
 * Engine Monitor Info
 */
typedef struct EngineMonitorInfo
{
    const char* redable_name;
    EngineWindowRectSize physical_size;
    EngineWindowContentScale content_scale;
    EngineMonitorPosition virtual_position;
    EngineMonitorWorkArea work_area;
    EngineMonitorVideoMode current_mode;

    EngineMonitor* monitor;
} EngineMonitorInfo;
