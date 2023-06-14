#include "file_loader.h"

char* file_load(const char* path)
{
    FILE* fptr = fopen(path, "r");
    if(fptr == NULL) {
        printf("Failed to open file: %s\n", path);
        return "";
    }

    fseek(fptr, 0L, SEEK_END);
    const long num_bytes = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);

    char* buffer = (char*)calloc(num_bytes, sizeof(char));
    if(buffer == NULL) {
        printf("Failed to allocate buffer for read\n");
        return "";
    }

    fread(buffer, sizeof(char), num_bytes, fptr);
    fclose(fptr);

    return buffer;
}