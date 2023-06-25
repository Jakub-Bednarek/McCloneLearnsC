#include "texture_atlas.h"
#include "core/error.h"
#include "utils/bmp_loader.h"

#include <GL/gl.h>
#include <stdbool.h>
#include <stdlib.h>

int g_max_texture_atlas_size = 0;

bool texture_atlas_upload_sub_texture(TextureAtlas* texture_atlas, const char* texture_path)
{
    Bitmap* bmp = bitmap_load(texture_path);

    if(bmp == NULL) {
        printf("Failed to load texture: %s", texture_path);
        return false;
    }

    GL_CALL(glTexSubImage2D(GL_TEXTURE_2D,
                 0,
                 texture_atlas->current_x_offset,
                 texture_atlas->current_y_offset,
                 texture_atlas->single_texture_width,
                 texture_atlas->single_texture_height,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 bmp->pixels));

    bitmap_free(bmp);
    return true;
}

TextureAtlas texture_atlas_create(const unsigned int texture_width, const unsigned int texture_height, const char** textures_paths, const unsigned int number_of_textures)
{
    static bool is_max_texture_size_initialized = false;
    if(!is_max_texture_size_initialized) {
        // glGetIntegerv(GL_MAX_TEXTURE_SIZE, &g_max_texture_atlas_size);
        g_max_texture_atlas_size = 32;
        is_max_texture_size_initialized = true;
    }

    TextureAtlas texture_atlas;
    texture_atlas.error_code = MC_ERROR_NONE;
    if(texture_width > g_max_texture_atlas_size || texture_height > g_max_texture_atlas_size) {
        texture_atlas.error_code = TEXTURE_DIMENSIONS_EXCEEDS_MAX_SIZE;
        return texture_atlas;
    }

    texture_atlas.width = g_max_texture_atlas_size;
    texture_atlas.height = g_max_texture_atlas_size;
    texture_atlas.single_texture_width = texture_width;
    texture_atlas.single_texture_height = texture_height;
    unsigned int max_textures_horizontally = (unsigned int)(DEFAULT_TEXTURE_ATLAS_WIDTH / texture_width);
    unsigned int max_textures_vertically = (unsigned int)(DEFAULT_TEXTURE_ATLAS_HEIGHT / texture_height);

    texture_atlas.max_textures = max_textures_horizontally * max_textures_vertically;
    if(texture_atlas.max_textures < number_of_textures) {
        texture_atlas.error_code = BUFFER_OVERFLOW;
        return texture_atlas;
    }

    texture_atlas.current_x_offset = 0;
    texture_atlas.current_y_offset = 0;

    GL_CALL(glGenTextures(1, &texture_atlas.texture_id));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture_atlas.texture_id));
    GL_CALL(glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 32, 32));
    
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));

    static float boarder_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GL_CALL(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, boarder_color));

    for(size_t i = 0; i < number_of_textures; i++) {
        if(texture_atlas_upload_sub_texture(&texture_atlas, textures_paths[i])) {
            if(texture_atlas.current_x_offset + texture_width >= texture_atlas.width) {
                texture_atlas.current_x_offset = 0;
                texture_atlas.current_y_offset += texture_height;
            }
            else {
                texture_atlas.current_x_offset += texture_width;
            }
            printf("Successfuly loaded texture: %s\n", textures_paths[i]);
        }
    }

    return texture_atlas;
}