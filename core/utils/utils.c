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