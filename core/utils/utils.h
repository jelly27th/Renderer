#ifndef _UTILS_H
#define _UTILS_H

#include "../common.h"
#include "../macro.h"

void swap_char(unsigned char *a, unsigned char *b);
void swap_int(int *a, int *b);

void error_check(const char *filename, int errno);
void bubble_sort(int *arr, int length);

#endif