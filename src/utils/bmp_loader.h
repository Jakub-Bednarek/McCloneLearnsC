#ifndef BMP_READER_HPP
#define BMP_READER_HPP

typedef struct {
    unsigned int width;
    unsigned int height;
    char* pixels;
} Bitmap;

extern Bitmap* load_bmp(const char* path);
extern void free_bmp(Bitmap*);

#endif // BMP_READER_HPP% 