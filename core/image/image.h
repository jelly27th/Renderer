#ifndef _IMAGE_H
#define _IMAGE_H

#include "../common.h"

typedef struct {
    unsigned char* data;
    int width, height, channels;
}IMAGE;

typedef unsigned char RGBA[4];
typedef unsigned char RGB[3];

// main function
IMAGE* image_init(int width, int height, int channels);
void image_save(char const *filename, IMAGE* image);
void image_free(IMAGE* image);

// other functions
int get_image_position(IMAGE *image, int x, int y);

#endif