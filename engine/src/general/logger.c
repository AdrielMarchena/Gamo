#include <stdarg.h>
#include <stdio.h>

static void engine_log(FILE* stream, const char* level, const char* fmt, va_list args)
{
    fprintf(stream, "[%s] ", level);
    vfprintf(stream, fmt, args);
    fprintf(stream, "\n");
}

void engine_log_info(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    engine_log(stdout, "INFO", fmt, args);
    va_end(args);
}

void engine_log_warn(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    engine_log(stdout, "WARN", fmt, args);
    va_end(args);
}

void engine_log_error(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    engine_log(stderr, "ERROR", fmt, args);
    va_end(args);
}

void engine_log_warning(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    engine_log(stdout, "WARNING", fmt, args);
    va_end(args);
}

void engine_log_debug(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    engine_log(stdout, "DEBUG", fmt, args);
    va_end(args);
}

void engine_log_status(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf("\r");
    vfprintf(stdout, fmt, args);
    fflush(stdout);
    va_end(args);
}