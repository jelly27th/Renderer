#ifndef _UTILS_H
#define _UTILS_H

#include "../maths/maths.h"
#include "../common.h"

void swap_char(unsigned char *a, unsigned char *b);
void swap_int(int *a, int *b);

void error_check(const char *filename, int errno);
void bubble_sort(int *arr, int length);

int mapping_interval(int Omin, int Omax, int Nmin, int Nmax, float a);
vec3f barycentric(vec2i p, vec2i v0, vec2i v1, vec2i v2);

#endif