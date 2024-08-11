#ifndef _DRAEW2D_H
#define _DRAEW2D_H

#include "../common.h"
#include "../image/image.h"
#include "../maths/maths.h"
#include "../utils/utils.h"
#include "../texture/texture.h"

void draw2d_point(vec2i p0, unsigned char *color, image_t *image);
void draw2d_line(vec2i p0, vec2i p1, unsigned char *color, image_t *image);
void draw2d_triangle_raster(vec4f v[3], vec3f vt[3],
                            texture_t* texture, image_t *image,
                            unsigned char *color, float intensity);

#endif