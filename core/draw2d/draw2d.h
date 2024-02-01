#ifndef _DRAEW2D_H
#define _DRAEW2D_H

#include "../common.h"
#include "../image/image.h"
#include "../maths/maths.h"
#include "../utils/utils.h"

void draw2d_point(vec2_i p0, unsigned char *color, IMAGE *image);
void draw2d_line(vec2_i p0, vec2_i p1, unsigned char *color, IMAGE *image);

#endif