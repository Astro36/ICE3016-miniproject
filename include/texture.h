#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "color.h"

class Bitmap {
public:
    Bitmap(int width, int height);
    Bitmap(const std::string& filename);
    ~Bitmap();
    void draw_point(int x, int y, Color color);
    int get_width() const;
    int get_height() const;
    unsigned char* get_pixels() const;
    void save(const std::string& out_filename, const std::string& ref_filename) const;

private:
    int width;
    int height;
    int channels;
    unsigned char* pixels;
};

void init_texture(int texture, const Bitmap& image);
void update_texture(int texture, const Bitmap& image);
void reverse_each_pixel(int width, int height, unsigned char* pixels);

#endif // TEXTURE_H
