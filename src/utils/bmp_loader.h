#ifndef BMP_READER_HPP
#define BMP_READER_HPP

typedef struct {
    unsigned int width;
    unsigned int height;
    char* pixels;
} Bitmap;

extern Bitmap* bitmap_load(const char*);
extern void bitmap_free(Bitmap*);

#endif // BMP_READER_HPP% 