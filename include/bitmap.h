#ifndef BITMAP_H
#define BITMAP_H

typedef struct bitmap {
    int width;
    int height;
    int channels;
    unsigned char* pixels;
} bitmap_t;

int new_image(bitmap_t* out_image, const char* filename);
void delete_image(bitmap_t* image);

#endif // BITMAP_H
