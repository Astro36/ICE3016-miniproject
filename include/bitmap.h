#pragma once
#include <string>

struct Color {
    float red;
    float green;
    float blue;
};

class Bitmap {
public:
    Bitmap() = delete;
    Bitmap(int width, int height);
    Bitmap(const std::string& filename);
    ~Bitmap();
    void fill_pixel(int x, int y, const Color& color);
    int get_channels() const;
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

void reverse_each_pixel(int width, int height, unsigned char* pixels);
