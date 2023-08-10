#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct {
    unsigned int texture_id;
} Texture;

extern Texture create_texture(const char*);

#endif // TEXTURE_H