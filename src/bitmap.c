#define _CRT_SECURE_NO_WARNINGS
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>

int new_image(bitmap_t* out_image, const char* filename) {
	FILE* fp = fopen(filename, "rb");
	printf("load %s\n", filename);
	if (!fp) {
		perror("File opening failed\n");
		return EXIT_FAILURE;
	}

	unsigned char header[54]; // bitmap header
	if (fread(header, sizeof(unsigned char), 54, fp) != 54 || header[0] != 'B' || header[1] != 'M') {
		perror("Invalid BMP file\n");
		fclose(fp);
		return EXIT_FAILURE;
	}

	unsigned int offset = *(unsigned int*)&(header[0x0A]);
	out_image->width = *(int*)&(header[0x12]);
	out_image->height = *(int*)&(header[0x16]);
	unsigned int size = *(unsigned int*)&(header[0x22]); // image size

	if (size == out_image->width * out_image->height) {
		out_image->channels = 1;
	}
	else {
		out_image->channels = 3;
	}
	size = out_image->width * out_image->height * out_image->channels;
	if (offset == 0) {
		offset = 54; // The BMP header is done that way
	}
	out_image->pixels = (unsigned char*)malloc(size); // image pixel data
	if (out_image->pixels == NULL) {
		perror("Memory allocation failed\n");
		fclose(fp);
		return EXIT_FAILURE;
	}
	if (fread(out_image->pixels, sizeof(unsigned char), size, fp) != size) {
		perror("Invalid image data\n");
		fclose(fp);
		return EXIT_FAILURE;
	}
	fclose(fp);

	// flip R & B // BGR order -> RGB order
	if (out_image->channels == 3) {
		for (int i = 0; i < out_image->height; i++) {
			for (int j = 0; j < out_image->width; j++) {
				unsigned char tmp = out_image->pixels[3 * (i * out_image->width + j)]; // R
				out_image->pixels[3 * (i * out_image->width + j)] = out_image->pixels[3 * (i * out_image->width + j) + 2];
				out_image->pixels[3 * (i * out_image->width + j) + 2] = tmp;
			}
		}
	}
	return EXIT_SUCCESS;
}

void delete_image(bitmap_t* image) {
	free(image->pixels);
}
