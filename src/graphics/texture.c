#include "texture.h"
#include "utils/bmp_loader.h"

#include <GL/gl.h>

#include <stddef.h>

float texture_coords[] = {
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 1.0f
};

Texture create_texture(const char* path)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    static float boarder_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, boarder_color);

    Bitmap* bmp = bitmap_load(path);
    if(bmp == NULL) {
        return;
    }

    unsigned int texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp->width, bmp->height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    bitmap_free(bmp);

    printf("Texture: %s with id: %d loaded successfuly.\n", path, texture_id);

    Texture tex = {.texture_id = texture_id};
    return tex;
}