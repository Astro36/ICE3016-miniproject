#define _CRT_SECURE_NO_WARNINGS
#include "bitmap.h"
#include <GL/freeglut.h>
#include <algorithm>
#include <cstdio>
#include <string>

Bitmap::Bitmap(int width, int height)
: width(width), height(height), channels(3) {
    int size = width * height * 3;
    pixels = new unsigned char[size]; // image pixel data
    std::fill(pixels, pixels + size, 0xff);
}

Bitmap::Bitmap(const std::string& filename) {
    FILE* fp = fopen(filename.c_str(), "rb");
    if (!fp) {
        perror("File opening failed\n");
    }

    unsigned char header[54]; // bitmap header
    if (fread(header, sizeof(unsigned char), 54, fp) != 54 || header[0] != 'B' || header[1] != 'M') {
        perror("Invalid BMP file\n");
        fclose(fp);
    }

    unsigned int offset = *(unsigned int*) &(header[0x0A]);
    width = *(int*) &(header[0x12]);
    height = *(int*) &(header[0x16]);
    unsigned int size = *(unsigned int*) &(header[0x22]); // image size

    if (size == width * height) {
        channels = 1;
    } else {
        channels = 3;
    }
    size = width * height * channels;
    if (offset == 0) {
        offset = 54; // The BMP header is done that way
    }
    pixels = new unsigned char[size]; // image pixel data
    fread(pixels, sizeof(unsigned char), size, fp);
    fclose(fp);

    if (channels == 3) { // BGR order -> RGB order
        reverse_each_pixel(width, height, pixels);
    }
}

Bitmap::~Bitmap() {
    delete[] pixels;
}

void Bitmap::fill_pixel(int x, int y, const Color& color) {
    pixels[channels * (y * width + x)] = color.red * 255;
    pixels[channels * (y * width + x) + 1] = color.green * 255;
    pixels[channels * (y * width + x) + 2] = color.blue * 255;
}

int Bitmap::get_channels() const {
    return channels;
}

int Bitmap::get_width() const {
    return width;
}

int Bitmap::get_height() const {
    return height;
}

unsigned char* Bitmap::get_pixels() const {
    return pixels;
}

void Bitmap::save(const std::string& out_filename, const std::string& ref_filename) const {
    FILE* ifp = fopen(ref_filename.c_str(), "rb");
    if (!ifp) {
        perror("File opening failed\n");
        return;
    }
    fseek(ifp, 10, SEEK_SET);

    int ref_offset;
    fread(&ref_offset, sizeof(int), 1, ifp);

    fseek(ifp, 18, SEEK_SET);

    int ref_width;
    int ref_height;
    fread(&ref_width, sizeof(int), 1, ifp);
    fread(&ref_height, sizeof(int), 1, ifp);

    fseek(ifp, 0, SEEK_SET);

    unsigned char* ref_header = new unsigned char[ref_offset];
    fread(ref_header, sizeof(unsigned char), ref_offset, ifp);
    fclose(ifp);

    FILE* ofp = fopen(out_filename.c_str(), "wb");
    if (!ofp) {
        perror("File opening failed\n");
        return;
    }
    fwrite(ref_header, sizeof(unsigned char), ref_offset, ofp);
    delete[] ref_header;

    int size = channels * width * height;
    unsigned char* out_pixels = new unsigned char[size];
    std::copy(pixels, pixels + size, out_pixels);
    if (channels == 3) {
        reverse_each_pixel(width, height, out_pixels);
    }
    fwrite(out_pixels, sizeof(unsigned char), size, ofp);
    delete[] out_pixels;

    fclose(ofp);
}

void reverse_each_pixel(int width, int height, unsigned char* pixels) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::swap(pixels[3 * (i * width + j)], pixels[3 * (i * width + j) + 2]); // R, B
        }
    }
}
