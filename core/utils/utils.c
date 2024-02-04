#include "utils.h"

void swap_char(unsigned char* a, unsigned char* b) {
    unsigned char tmp = *a;
    *a = *b;
    *b = tmp;
}

void swap_int(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void error_check(const char* filename, int errno) {
  fprintf(stderr, "%s: %s\n", filename, strerror(errno));
  exit(-1);
}

/**
 * @brief Mapping one interval to another interval, for more information see
 *        https://blog.csdn.net/Touch_Dream/article/details/62076236
 *
 * @param Omin the min value of original interval
 * @param Omax the max value of original interval
 * @param Nmax the max value of mapped interval
 * @param Nmin the min value of mapped interval
 * @param a input value for original interval
 * @return int
 * @file utils.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-03
 * @copyright Copyright (c) 2024
 */
int mapping_interval(int Omin, int Omax, int Nmin, int Nmax, float a) {
  return (float)Nmin + ((Nmax - Nmin) / (float)(Omax - Omin)) * (a - Omin);
}