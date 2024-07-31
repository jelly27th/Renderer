#ifndef _MATHS_H
#define _MATHS_H

#include "../common.h"
#include "../macro.h"

typedef struct { int x, y;}    vec2i;
typedef struct { int x, y, z;} vec3i;

typedef struct { float x, y; } vec2f;
typedef struct { float x, y, z; } vec3f;

typedef struct { float x, y, z, w; } vec4f;

typedef struct { float m[4][4]; } mat4f;

vec2i vec2_new(int x, int y);
void vec2_reset(vec2i *point, int x, int y);
int vec2_edge(vec2i p, vec2i v0, vec2i v1);
vec2i vec2_mult(vec2i p, float factor);
vec2i vec2_add(vec2i a, vec2i b);
void vec2_print(vec2i p);
void vec2_fprint(FILE* opf, vec2i p);

vec3f vec3_new(float x, float y, float z);
vec3f vec3_cross(vec3f a, vec3f b);
float vec3_dot(vec3f a, vec3f b);
vec3f vec3_sub(vec3f a, vec3f b);
vec3f vec3_normalize(vec3f a);
vec3f vec3_mult(vec3f a, float factor);
float vec3_len(vec3f a);
vec3f vec3_div(vec3f a, float divisor);
vec3f vec3_add(vec3f a, vec3f b);
vec3f vec3_add3(vec3f a, vec3f b, vec3f c);
vec4f vec3_2_vec4(vec3f a, float w);
void vec3_print(vec3f p);
void vec3_fprint(FILE* opf, vec3f p);

vec4f vec4_new(float x, float y, float z, float w);
vec3f vec4_2_vec3(vec4f a, float w);
void vec4_print(vec4f p);
void vec4_fprint(FILE* opf, vec4f p);

mat4f mat4_identity();
mat4f mat4_zero();
mat4f mat4_mult_mat4(mat4f a, mat4f b);
vec4f mat4_mult_vec4(mat4f a, vec4f b);

mat4f mat4_ortho(float left, float right, float bottom, float top, float near, float far);
mat4f mat4_perspective(float fov, float aspect, float near, float far);
mat4f mat4_translate(float x, float y, float z);
mat4f mat4_scale(float x, float y, float z);
mat4f mat4_rotate_x(float angle);
mat4f mat4_rotate_y(float angle);
mat4f mat4_rotate_z(float angle);
mat4f mat4_look_at(vec3f eye, vec3f target, vec3f up);
mat4f mat4_viewport(float width, float height);

vec3f barycentric(vec2i p, vec2i v0, vec2i v1, vec2i v2);
vec3f perspective_correct_interp(vec3f v[3], vec3f weight, vec3f w);

#endif