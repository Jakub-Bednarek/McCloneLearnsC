#include "bmp_loader.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#pragma pack(push, 1)

typedef struct
{
    uint16_t header_field;
    uint32_t size;
    uint16_t reserved_1;
    uint16_t reserved_2;
    uint32_t offset;
} BitmapFileHeader;

#pragma pack(pop)

typedef struct
{
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t compressed_image_size;
    uint32_t x_pixels_per_m;
    uint32_t y_pixels_per_m;
    uint32_t colors_used;
    uint32_t colors_important;
} BitmapInfoHeader;

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Pixel __attribute__((__packed__));

Bitmap* load_bmp(const char* path)
{
    FILE* file_ptr = fopen(path, "r");
    if(file_ptr == NULL) {
        printf("ERROR: Failed to open file: %s\n", path);
        return NULL;
    }

    BitmapFileHeader bitmap_file_header;
    fread(&bitmap_file_header, sizeof(BitmapFileHeader), 1, file_ptr);

    BitmapInfoHeader bitmap_info_header;
    fread(&bitmap_info_header, sizeof(BitmapInfoHeader), 1, file_ptr);

    const int padding = (4 - ((bitmap_info_header.width * 3) % 4)) % 4;
    fseek(file_ptr, bitmap_file_header.offset, SEEK_SET);

    const int bitmap_size = bitmap_info_header.width * bitmap_info_header.height * sizeof(Pixel);
    Pixel* pixels = calloc(bitmap_size, sizeof(Pixel));
    if(pixels == NULL) {
        printf("ERROR: Failed to allocate memory for pixels in bitmap\n");
        return NULL;
    }

    int pixel_to_access = 0;
    for(int i = bitmap_info_header.height - 1; i >= 0; --i) {
        for(int j = 0; j < bitmap_info_header.width; ++j) {
            pixel_to_access = i * bitmap_info_header.height + j;
            fread(&pixels[pixel_to_access], sizeof(Pixel), 1, file_ptr);
            uint8_t blue = pixels[pixel_to_access].blue;
            pixels[pixel_to_access].blue = pixels[pixel_to_access].red;
            pixels[pixel_to_access].red = blue;
        }
        if(padding != 0) {
            fseek(file_ptr, padding, SEEK_CUR);
        }
    }

    fclose(file_ptr);

    Bitmap* bmp = calloc(1, sizeof(Bitmap));
    if(bmp == NULL) {
        printf("ERROR: Failed to allocate memory for bitmap\n");
        free(pixels);
        return NULL;
    }

    bmp->width = bitmap_info_header.width;
    bmp->height = bitmap_info_header.height;
    bmp->pixels = pixels;
    return bmp;
}

void free_bmp(Bitmap* bitmap)
{
    free(bitmap->pixels);
    free(bitmap);
}