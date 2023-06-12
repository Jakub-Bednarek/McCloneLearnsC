#include "shader_loader.h"

char* shader_load_source(const char* path)
{
    FILE* fptr;
    char* buffer;
    long num_bytes;
    fptr = fopen(path, "r");

    if(fptr == NULL) {
        printf("Failed to open file: %s\n", path);
        return "";
    }

    fseek(fptr, 0L, SEEK_END);
    num_bytes = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);

    buffer = (char*)calloc(num_bytes, sizeof(char));

    if(buffer == NULL) {
        printf("Failed to allocate buffer for read\n");
        return "";
    }

    fread(buffer, sizeof(char), num_bytes, fptr);
    fclose(fptr);

    return buffer;
}