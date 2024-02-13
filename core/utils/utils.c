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
