#include "engine/general/read_file.h"

#include "engine/general/logger.h"
#include "engine/general/engine_alloc.h"

#include <stdio.h>

char* engine_read_file(const char* path)
{
    engine_log_info("Opening file: %s\n", path);
    FILE* file = fopen(path, "rb");
    if (!file)
    {
        engine_log_error("Failed to open file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)engine_alloc(length + 1);
    if (!buffer)
    {
        engine_log_error("Failed to allocate memory for file: %s\n", path);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

int engine_file_has_extension(const char* path, const char* extension)
{
    size_t path_len = strlen(path);
    size_t ext_len = strlen(extension);

    return path_len > ext_len && strcmp(path + path_len - ext_len, extension) == 0;
}