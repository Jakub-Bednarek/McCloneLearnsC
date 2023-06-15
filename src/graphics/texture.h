#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct {
    unsigned int texture_id;
} Texture;

typedef struct {
    unsigned int atlas_width;
    unsigned int atlas_height;
    unsigned int texture_width;
    unsigned int texture_height;
};

extern Texture create_texture(const char*);

#endif // TEXTURE_H