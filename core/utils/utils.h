#ifndef _UTILS_H
#define _UTILS_H

// #include "../api.h"
#include "../common.h"

void swap_char(unsigned char *a, unsigned char *b);
void swap_int(int *a, int *b);
void error_check(const char *filename, int errno);

#endif