#ifndef _IMAGE_H
#define _IMAGE_H

#include "../common.h"
#include "../macro.h"
#include "../maths/maths.h"

typedef struct {
    unsigned char* data;
    int width, height, channels;
    float* depthBuffer; // z-buffer
}image_t;

typedef unsigned char RGBA[4];
typedef unsigned char RGB[3];

// main function
image_t* image_new(int width, int height, int channels);
void image_save(char const* filename, image_t* image);
void image_free(image_t* image);
image_t* image_load(char const* filename);

// misc functions
int get_image_position(image_t *image, int x, int y);
vec2i convert_vertex_point(vec3f point, image_t* image);

#endif