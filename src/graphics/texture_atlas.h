#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#define DEFAULT_TEXTURE_ATLAS_WIDTH 2048
#define DEFAULT_TEXTURE_ATLAS_HEIGHT 2048
#define DEFAULT_TEXTURE_WIDTH 32
#define DEFAULT_TEXTURE_HEIGHT 32

#define TEXTURE_DIMENSIONS_EXCEEDS_MAX_SIZE 1
#define BUFFER_OVERFLOW 2 // Dimensions x number_of_textures exceeds TextureAtlas capacity

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int single_texture_width;
    unsigned int single_texture_height;
    unsigned int current_x_offset;
    unsigned int current_y_offset;
    unsigned int max_textures;
    unsigned int texture_id;
    unsigned int error_code;
} TextureAtlas;

extern TextureAtlas texture_atlas_create(unsigned int, unsigned int, const char**, unsigned int);

#endif // TEXTURE_ATLAS_H