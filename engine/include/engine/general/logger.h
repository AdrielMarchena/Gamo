#pragma once

void engine_log_info(const char* fmt, ...);
void engine_log_warn(const char* fmt, ...);
void engine_log_error(const char* fmt, ...);
void engine_log_debug(const char* fmt, ...);

void engine_log_status(const char* fmt, ...);