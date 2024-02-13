#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "../common.h"
#include "../utils/utils.h"
#include "../image/image.h"
#include "../maths/maths.h"

typedef struct {
  unsigned char* pixels;
  int width, height, channels;
}texture_t;

/* main function */
texture_t* texture_new(int width, int height, int channels);
texture_t* image_to_texture(image_t* image);
void texture_free(texture_t* texture);

/* misc function */
int get_texture_position(texture_t* texture, int x, int y);
void get_texture_pixel(texture_t* texture, unsigned char* color, vec2i p);
void calculate_lighting_color(texture_t* texture, unsigned char* color, float intensity);
vec3f convert_texture_point(vec3f point, texture_t* texture);

#endif