#include "maths.h"

vec2i vec2i_new(int x, int y) {
  vec2i point = {x, y};
  return point;
}

void vec2i_reset(vec2i *point, int x, int y) {
  point->x = x;
  point->y = y;
}

/**                         --->    --->
 * @brief Build two vectors v0p and v0v1 and
 *        Check if the point is to the left or right of the line.
 *        if `return` < 0, point is to the left side
 *        if `return` >= 0, point is to the right side
 *        for more information to see
 *        https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage.html
 * @param p
 * @param v0
 * @param v1
 * @return int
 * @file maths.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-07
 * @copyright Copyright (c) 2024
 */
int vec2i_edge(vec2i p, vec2i v0, vec2i v1) {
  return (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
}

vec3f vec3f_new(float x, float y, float z) {
    vec3f v = {x, y, z};
    return v;
}

vec3f vec3f_cross(vec3f a, vec3f b) {
    float x = a.y * b.z - a.z * b.y;
    float y = a.z * b.x - a.x * b.z;
    float z = a.x * b.y - a.y * b.x;

    return vec3f_new(x, y, z);
}

float vec3f_dot(vec3f a, vec3f b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3f vec3f_sub(vec3f a, vec3f b) {
    float x = a.x - b.x;
    float y = a.y - b.y;
    float z = a.z - b.z;

    return vec3f_new(x, y, z);
}

vec3f vec3f_normalize(vec3f a) {
    return vec3f_div(a, vec3f_len(a));
}

vec3f vec3f_mult(vec3f a, float factor) {
    return vec3f_new(a.x * factor, a.y * factor, a.z * factor);
}

float vec3f_len(vec3f a) {
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

vec3f vec3f_div(vec3f a, float divisor) {
    return vec3f_mult(a, 1/divisor);
}