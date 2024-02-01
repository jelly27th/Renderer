#ifndef _MATHS_H
#define _MATHS_H

#include "../common.h"

typedef struct { int x, y;}   vec2_i;
typedef struct { int x, y, z;} vec3_i;

typedef struct { float x, y, z; } vec3_f;

vec2_i point_create(int x, int y);
void point_set(vec2_i *point, int x, int y);
// #define point_set(point, x0, y0) point->x = x0, point->y = y0

#endif