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

void bubble_sort(int* arr, int length) {
  for (int i = length - 1; i >= 0; i--) {
    for (int j = 0; j < i; j++) {
      if (arr[j] > arr[j + 1]) {
        int temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}

/**
 * @brief compute barycentric coordinates from Cartesian coordinates.
 *        for more information to see https://en.wikipedia.org/wiki/Barycentric_coordinate_system
 *        Edge approach from chapter Conversion between barycentric and Cartesian coordinates. 
 *
 * @param p
 * @param v1
 * @param v2
 * @param v3
 * @return vec3f
 * @file utils.c
 * @version 0.1
 * @author Jelly (wugd827@163.com)
 * @date 2024-02-07
 * @copyright Copyright (c) 2024
 */
vec3f barycentric(vec2i p, vec2i v1, vec2i v2, vec2i v3) {
    float lambda1 = (float)((v2.y - v3.y) * (p.x - v3.x) + (v3.x - v2.x) * (p.y - v3.y)) \
                    / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
    
    float lambda2 = (float)((v3.y - v1.y) * (p.x - v3.x) + (v1.x - v3.x) * (p.y - v3.y)) \
                    / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
    
    float lambda3 = 1 - lambda1 - lambda2;
    
    vec3f f = vec3f_new(lambda1, lambda2, lambda3);
    return f;
}
