#ifndef _MATHS_H
#define _MATHS_H

#include "../common.h"

typedef struct { int x, y;}   vec2i;
typedef struct { int x, y, z;} vec3i;

typedef struct { float x, y; } vec2f;
typedef struct { float x, y, z; } vec3f;

vec2i vec2i_new(int x, int y);
void vec2i_reset(vec2i *point, int x, int y);
int vec2i_edge(vec2i p, vec2i v0, vec2i v1);
vec2i vec2i_mult(vec2i p, float factor);
vec2i vec2i_add(vec2i a, vec2i b);

vec3f vec3f_new(float x, float y, float z);
vec3f vec3f_cross(vec3f a, vec3f b);
float vec3f_dot(vec3f a, vec3f b);
vec3f vec3f_sub(vec3f a, vec3f b);
vec3f vec3f_normalize(vec3f a);
vec3f vec3f_mult(vec3f a, float factor);
float vec3f_len(vec3f a);
vec3f vec3f_div(vec3f a, float divisor);
vec3f vec3f_add(vec3f a, vec3f b);
vec3f vec3f_add3(vec3f a, vec3f b, vec3f c);

vec3f barycentric(vec2i p, vec2i v0, vec2i v1, vec2i v2);

#endif